
#include "typedef.h"
#include "lpc1700.h"
#include "parameter.h"
#include "state.h"
#include "can.h"
#include "call.h"
#include "history.h"
#include "lcd.h"
#include "io.h"

BYTE cana_rx_err, cana_tx_err, error_bit, error_code;

void init_can_a (void){
	DWORD reg;
	CAN1MOD = 0x0Dul;				// Reset Mode
	CAN1BTR = BRP_125K;	
	CLRENA0 = ISE_CAN;
	CAN1MOD |= 01ul;
	__nop();__nop();
	reg = CAN1GSR;
	reg &= 0x000000FF;
	CAN1GSR = reg;					//������ϼ���
	
	CAN1CMR = CMD_CLR_RECV;
	CAN1IER = 0x48Ful;				//�����жϣ����ͻ���1�жϣ����󱨾��жϣ����ݳ����жϣ����ߴ����жϣ����ͻ���3�ж�
	CAN1MOD &= ~0x01ul;
	CAN1CMR = CMD_CLR_DATAOVERRUN;
	SETENA0 = ISE_CAN;

	CAN1TFI2 = (0x01ul << 16) | SEND_PRIORITY_HIGH;		//������������(ʹ�� buf2 )
	CAN1TID2 = (HEARTBEAT << 3) | HSE_ID;
	CAN1TDA2 = OPERATIONAL;
}

void init_can_b (void)
{
	CAN2MOD |= 0x0dul;				// Reset Mode
	CAN2BTR = BRP_50K;//BRP_50K;		//BRP_50K_OLD;				//
	CAN2CMR = CMD_CLR_RECV;
	CAN2IER = 0x48Ful;
	CAN2MOD &= ~0x01ul;
	CAN2CMR = CMD_CLR_DATAOVERRUN;

	CAN2TFI2 = (0x01ul << 16) | SEND_PRIORITY_HIGH;
	CAN2TID2 = (HEARTBEAT << 3) | HSE_ID;
	CAN2TDA2 = OPERATIONAL;
}

/****************************************************
* ������: CAN_IRQHandler
* ����: CAN�жϷ������
* ���: ��
* ����: ��
* ����ֵ: ��
****************************************************/
void CAN_IRQHandler (void)
{
	DWORD reg, icr;
	WORD i;

	icr = CAN1ICR;
	if (icr & CAN_INT_RECV)				// �����ж�
	{
		CAN1ICR &= ~CAN_INT_RECV;
		if (rca >= RXASW_SIZE)
		{
			CAN1CMR |= CMD_CLR_RECV;
			if ((!(canerrorstate & RXA_SW_OVERRUN)) && (cana_busoff_checktime < timer))
			{
				canerrorstate |= RXA_SW_OVERRUN;
				cana_error_count += 5;
				write_errorhist (E_CAN_OV_SW_A, HSE_ID, 0, W_RXA_SW_OVERRUN);
				tse_state_request = tse_enabled;
				exe_state_request = exe_enabled & 0x0F;
				cana_busoff_checktime = timer + 3 SEC;
			}
		}
		else
		{
			reg = CAN1RID & 0x7ff;			
			rxa[ria][0] = (BYTE)((reg >> 3) & 0xf0ul);
			rxa[ria][1] = reg & 0x7f;
			*(DWORD *)&rxa[ria][2] = CAN1RDA;
			*(DWORD *)&rxa[ria][6] = CAN1RDB;
			CAN1CMR |= CMD_CLR_RECV;
			if (rxa[ria][0] == HEARTBEAT)
			{
				switch(rxa[ria][1])
				{
					case (FVE_ID):
						heartbeat |= B_FVE;
						if ((rxa[ria][2] == OPERATIONAL) && (start_init & B_FVE))
						{
							init_error &= ~B_FVE;
						}
						else
							init_error |= B_FVE;
						break;

					default:
						if ((rxa[ria][1] >= TSE_ID) && (rxa[ria][1] < (TSE_ID + MAX_TSE)))
							{
								tse_heartbeat |= (0x0001 << (rxa[ria][1] - TSE_ID));
								tse_hb_error &= (~(0x0001 << (rxa[ria][1] - TSE_ID)));
								if (rxa[ria][2] == OPERATIONAL)
									{
										tse_init_error &= ~(0x0001 << (rxa[ria][1] - TSE_ID));
										tse_start_init &= ~(0x0001 << (rxa[ria][1] - TSE_ID));
									}
								else
									{
										if (!(tse_start_init & (0x0001 << (rxa[ria][1] - TSE_ID))))
											tse_start_init |= (0x0001 << (rxa[ria][1] - TSE_ID));
										tse_init_error |= ((0x0001 << (rxa[ria][1] - TSE_ID)) & tse_enabled);
										bit_set (tse_hb_state, rxa[ria][1] - TSE_ID);
									}
							}						
						else if ((rxa[ria][1] >= EXE_ID) && (rxa[ria][1] < (EXE_ID + MAX_EXE)))
							{//������չ����������
								exe_heartbeat |= (0x0001 << (rxa[ria][1] - EXE_ID));
								exe_hb_error &= (~(0x0001 << (rxa[ria][1] - EXE_ID)));
								if (rxa[ria][2] == OPERATIONAL)
									{
										exe_init_error &= ~(0x0001 << (rxa[ria][1] - EXE_ID));
										exe_start_init &= ~(0x0001 << (rxa[ria][1] - EXE_ID));
									}
								else
									{
										if (!(exe_start_init & (0x0001 << (rxa[ria][1] - EXE_ID))))
											exe_start_init |= (0x0001 << (rxa[ria][1] - EXE_ID));
										exe_init_error |= ((0x0001 << (rxa[ria][1] - EXE_ID)) & exe_enabled);
										bit_set (exe_hb_state, rxa[ria][1] - EXE_ID);
									}
							}
						break;
				}
			}
			else if (rxa[ria][0] == TSDO)
			{
				if ((rxa[ria][1] >= TSE_ID) && (rxa[ria][1] < (TSE_ID + MAX_TSE)))
					{
						tse_sdo_id = rxa[ria][1];
						for (i = 0; i < 8; i++)
							tse_sdo_data [i] = rxa[ria][2 + i];
					}
				else if ((rxa[ria][1] >= EXE_ID) && (rxa[ria][1] < (EXE_ID + MAX_EXE)))
					{
						exe_sdo_id = rxa[ria][1];
						for (i = 0; i < 8; i++)
							exe_sdo_data [i] = rxa[ria][2 + i];
					}
				else
					{
						sdo_id = rxa[ria][1];
						for (i = 0; i < 8; i++)
							sdo_data [i] = rxa[ria][2 + i];
					}
			}
			else
			{
				if (ria >= (RXASW_SIZE - 1)) 
					ria= 0;
				else
					ria++;
				rca++;
			}
		}
	}
	if (icr & CAN_INT_SEND1)
	{
		CAN1ICR &= ~CAN_INT_SEND1;
		if (tca)
		{
			reg = txa[toa][0] & 0x0F;
			CAN1TFI1 = (reg << 16) | SEND_PRIORITY_LOW;
			reg = txa[toa][0] & 0xF0;
			CAN1TID1 = (reg << 3) | txa[toa][1];
			CAN1TDA1 = *(DWORD *)&txa[toa][2];
			CAN1TDB1 = *(DWORD *)&txa[toa][6];
			CAN1CMR = CMD_TRAN_BUFF1;
			--tca;
			if (toa >= (TXASW_SIZE - 1))
				toa = 0;
			else
				++ toa;
		}
		else
		{
			tra = 1;
		}
	}
	
	if (icr & CAN_INT_SEND3)
	{
		CAN1ICR &= ~CAN_INT_SEND3;
		if (sca)
		{
			reg = sxa[soa][0] & 0x0F;
			CAN1TFI3 = (reg << 16) | SEND_PRIORITY_MIDD;
			reg = sxa[soa][0] & 0xF0;
			CAN1TID3 = (reg << 3) | sxa[soa][1];
			CAN1TDA3 = *(DWORD *)&sxa[soa][2];
			CAN1TDB3 = *(DWORD *)&sxa[soa][6];
			CAN1CMR = CMD_TRAN_BUFF3;
			--sca;
			if (soa >= (SXASW_SIZE - 1))
				soa = 0;
			else
				++ soa;
		}
		else
			sra = 1;
	}
	
	if (icr & 0xcc)
	{
		cana_rx_err = (BYTE)((CAN1GSR & 0x00FF0000) >> 16);
		cana_tx_err = (BYTE)((CAN1GSR & 0xFF000000) >> 24);
		error_bit = (BYTE)((CAN1ICR & 0x001F0000) >> 16);
		error_code = (BYTE)((CAN1ICR & 0x00C00000) >> 22);
		
		if ((CAN1GSR & 0x02) && (CAN1SR & 0x02))
		{
			if ((!(canerrorstate & RXA_HW_OVERRUN)) && (cana_busoff_checktime < timer))
			{
				canerrorstate |= RXA_HW_OVERRUN;				
				cana_error_count ++;
				write_errorhist (E_CAN_OVERRUN_A, HSE_ID, 0, W_RXA_HW_OVERRUN);
				tse_state_request = tse_enabled;
				exe_state_request = exe_enabled & 0x0F;
				cana_busoff_checktime = timer + 3 SEC;
			}
			CAN1CMR = CMD_CLR_DATAOVERRUN;
			tra = 1;
			sra = 1;
		}
		if ((CAN1GSR & 0x80) && (CAN1SR & 0x80))
		{
			if ((!(canerrorstate & CANA_BUSOFF)) && (cana_busoff_checktime < timer))
			{
				canerrorstate |= CANA_BUSOFF;				
				cana_error_count += 5;
				write_errorhist (E_BUS_OFF_A, HSE_ID, 0, W_BUSA_OFF);
				tse_state_request = tse_enabled;
				exe_state_request = exe_enabled & 0x0F;
				cana_busoff_checktime = timer + 3 SEC;
			}
			tra = 1;
			sra = 1;
		}
//���� can ���󱨾����� 2014-08-18
		if ((icr & 0x04) && (p.canbus_check) && (drive_state != D_STOPPED))
			{
				CLRENA0 = ISE_CAN;
				CAN1MOD |= 01ul;
				__nop();__nop();
				reg = CAN1GSR;
				reg &= 0x000000FF;
				CAN1GSR = reg;					//������ϼ���
				CAN1MOD &= ~01ul; 
				__nop();__nop();
				SETENA0 = ISE_CAN;
			}
//���� can ���󱨾����� 2014-08-18	
	}

	icr = CAN2ICR;
	if (icr & CAN_INT_RECV)				// �����ж�
	{
		CAN2ICR &= ~CAN_INT_RECV;
		if (rcb >= RXBSW_SIZE)
		{
			CAN2CMR = CMD_CLR_RECV;
			if (!(canerrorstate & RXB_SW_OVERRUN))
			{
				canerrorstate |= RXB_SW_OVERRUN;				
				canb_error_count += 5;
				for (i = 0; i < MAX_ESE_DW; i++)
					ese_state_request [i] = ese_enabled [i];
			}
		}
		else
		{
			reg = CAN2RID & 0x7ff;
			rxb[rib][0] = (reg >> 3) & 0xf0;
			rxb[rib][1] = reg & 0x7f;
			*(DWORD *)&rxb[rib][2] = CAN2RDA;
			*(DWORD *)&rxb[rib][6] = CAN2RDB;
			reg = (CAN2RFS & 0x0F0000ul) >> 16;
			CAN2CMR = CMD_CLR_RECV;
			if (rxb[rib][0] == HEARTBEAT)
			{
				i = rxb[rib][1];
				if ((i >= ESE_ID) && (i < (ESE_ID + MAX_ESE)))
				{
					i -= ESE_ID;
					bit_setd (ese_heartbeat [i / 32], i % 32);
					bit_resetd (ese_hb_error  [i / 32], i % 32);
					if (rxb[rib][2] == OPERATIONAL)
					{
						bit_resetd (ese_init_error [i / 32], i % 32);
						bit_resetd (ese_start_init [i / 32], i % 32);
					}
					else
					{
						if (!bit_selectd (ese_start_init [i / 32], i % 32))
							{
								bit_setd (ese_start_init [i / 32], i % 32);
							}
						bit_setd (ese_init_error [i / 32], i % 32);
						bit_setd (ese_hb_state [i / 32], i % 32);
					}
				}				
				else if ((i >= EXE_ID + 1) && (i < (EXE_ID + MAX_EXE)))
					{//������չ����������
						exe_heartbeat |= (0x0001 << (i - EXE_ID));
						exe_hb_error &= (~(0x0001 << (i - EXE_ID)));
						if (rxb[rib][2] == OPERATIONAL)
							{
								exe_init_error &= ~(0x0001 << (i - EXE_ID));
								exe_start_init &= ~(0x0001 << (i - EXE_ID));
							}
						else
							{
								if (!(exe_start_init & (0x0001 << (i - EXE_ID))))
									exe_start_init |= (0x0001 << (i - EXE_ID));
								exe_init_error |= ((0x0001 << (i - EXE_ID)) & exe_enabled);
								bit_set (exe_hb_state, i - EXE_ID);
							}
					}
			}
			else if (rxb[rib][0] == TSDO)
			{
				i = rxb[rib][1];
				if ((i >= ESE_ID) && (i < (ESE_ID + MAX_ESE)))
				{
					ese_sdo_id = rxb[rib][1];
					for (i = 0; i < 8; i++)
						ese_sdo_data [i] = rxb[rib][i + 2];
				}				
				else if ((i >= EXE_ID + 1) && (i < (EXE_ID + MAX_EXE)))
					{
						exe_sdo_id = i;
						for (i = 0; i < 8; i++)
							exe_sdo_data [i] = rxb[rib][2 + i];
					}
			}			
			else
			{
				if (rib >= (RXBSW_SIZE - 1)) 
					rib = 0;
				else
					rib ++;
				rcb++;
			}
		}
	}
	if (icr & CAN_INT_SEND1)
	{
		CAN2ICR &= ~CAN_INT_SEND1;
		if (tcb)
		{
			reg = txb[tob][0] & 0x0F;
			CAN2TFI1 = (reg << 16) | SEND_PRIORITY_LOW;
			reg = txb[tob][0] & 0xF0;
			CAN2TID1 = (reg << 3) | txb[tob][1];
			CAN2TDA1 = *(DWORD *)&txb[tob][2];
			CAN2TDB1 = *(DWORD *)&txb[tob][6];
			CAN2CMR = CMD_TRAN_BUFF1;
			--tcb;
			if (tob >= (TXBSW_SIZE - 1))
				tob = 0;
			else
				++ tob;
		}
		else
			trb = 1;
	}
	if (icr & CAN_INT_SEND3)
	{
		CAN2ICR &= ~CAN_INT_SEND3;
		if (scb)
		{
			reg = sxb[sob][0] & 0x0F;
			CAN2TFI3 = (reg << 16) | SEND_PRIORITY_MIDD;
			reg = sxb[sob][0] & 0xF0;
			CAN2TID3 = (reg << 3) | sxb[sob][1];
			CAN2TDA3 = *(DWORD *)&sxb[sob][2];
			CAN2TDB3 = *(DWORD *)&sxb[sob][6];
			CAN2CMR = CMD_TRAN_BUFF3;
			--scb;
			if (sob >= (SXBSW_SIZE - 1))
				sob = 0;
			else
				++ sob;
		}
		else
			srb = 1;
	}

	if (icr & 0xcc)
	{
		if ((CAN2GSR & 0x02) && (CAN2SR & 0x02))
		{
			if ((!(canerrorstate & RXB_HW_OVERRUN)) && (canb_busoff_checktime < timer))
			{
				canerrorstate |= RXB_HW_OVERRUN;				
				canb_error_count ++;
				for (i = 0; i < MAX_ESE_DW; i++)
					ese_state_request [i] = ese_enabled [i];
			}
			CAN2CMR = CMD_CLR_DATAOVERRUN;
			trb = 1;
			srb = 1;
		}
		if ((CAN2GSR & 0x80) && (CAN2SR & 0x80))
		{
			if ((!(canerrorstate & CANB_BUSOFF)) && (canb_busoff_checktime < timer))
			{
				canerrorstate |= CANB_BUSOFF;				
				canb_error_count += 5;
				write_errorhist (E_BUS_OFF_B, HSE_ID, 0, W_BUSB_OFF);
				for (i = 0; i < MAX_ESE_DW; i++)
					ese_state_request [i] = ese_enabled [i];
			}
			trb = 1;
			srb = 1;
		}
	}
}


//**************************************************************************************************
// Set outputs on HSE or on CAN bus
//**************************************************************************************************
void set_out (BYTE func, BYTE sub_func, BYTE floor, BYTE doors, BYTE state, BYTE bus)
{
	if (bus & O_CANA)									// output is for CAN A
		set_out_a (func, sub_func, floor, doors, state);		// transmit message on CAN A (control bus)
	if (bus & O_CANB)									// output is for CAN B
		set_out_b (func, sub_func, floor, doors, state);		// transmit message on CAN B (shaft bus)
	if ((bus & O_CANC) && (p.groupsize > 1))	// output is for CAN C
		set_out_c (func, sub_func, floor, doors, state);		// transmit message on CAN B (shaft bus)
	if (bus & O_HSE)									// output is for HSE
		hse_out (func, sub_func, floor, doors, state);		// set outputs on HSE
}								   

//**************************************************************************************************
// Set level display
//**************************************************************************************************
void level_display (BYTE floor, BYTE bus)
{
	level_insp_fg = !level_insp_fg;
	if (bus & O_CANA)									// output is for CAN A
		level_display_a (floor);							// transmit message on CAN A (control bus)
	if (bus & O_CANB)									// output is for CAN B
		level_display_b (floor);							// transmit message on CAN B (shaft bus)
}

//**************************************************************************************************
// Send HSE state
//**************************************************************************************************
void send_hse_state (void)
{
	send_hse_state_a ();
	send_hse_state_b ();
}

/****************************************************************************************************/
/* send changed io parameter on CAN bus if necessary												*/
/****************************************************************************************************/
void send_message (BYTE fct_code, BYTE node_id, BYTE length, BYTE * data, BYTE bus)
{
	BYTE i;

	if (bus)
	{
		if (teb)
		{
			if (scb >= SXBSW_SIZE)
			{
				if (!(canerrorstate & SXB_SW_OVERRUN))
				{					
					canb_error_count += 5;
					canerrorstate |= SXB_SW_OVERRUN;			// set CAN error state
				}
			}
			else
			{
				CLRENA0 = ISE_CAN;
				if (srb)
				{
					CAN2TFI3 = (length << 16) | SEND_PRIORITY_MIDD;
					CAN2TID3 = (fct_code << 3) | node_id;
					CAN2TDA3 = *(DWORD *)data;
					CAN2TDB3 = *(DWORD *)(data + 4) ;
					CAN2CMR = CMD_TRAN_BUFF3;
					srb = 0;										// Tx not ready
				}
				else
				{
					sxb [sib][0] = fct_code | length;
					sxb [sib][1] = node_id;
					for (i=0; i<length; ++i)
						sxb [sib][2+i] = * data ++;
					if (sib >= (SXBSW_SIZE - 1))
						sib = 0;
					else
						sib ++;
					++ scb;
				}
				SETENA0 = ISE_CAN;
			}
		}
 	}
	else
	{
		if (tea)
		{
			if (sca >= SXASW_SIZE)
			{
				if ((!(canerrorstate & SXA_SW_OVERRUN)) && (cana_busoff_checktime < timer))
				{
					canerrorstate |= SXA_SW_OVERRUN;
					cana_error_count += 5;
					write_errorhist (E_CAN_SX_SW_A, HSE_ID, 0, W_RXA_SW_OVERRUN);
					cana_busoff_checktime = timer + 3 SEC;
				}
			}
			else
			{
				CLRENA0 = ISE_CAN;
				if (sra)
				{
					CAN1TFI3 = (length << 16) | SEND_PRIORITY_MIDD;
					CAN1TID3 = (fct_code << 3) | node_id ;
					CAN1TDA3 = *(DWORD *)data;
					CAN1TDB3 = *(DWORD *)(data + 4);
					CAN1CMR = CMD_TRAN_BUFF3;
					sra = 0;											// Tx not ready
				}
				else
				{
					sxa [sia][0] = fct_code | length;
					sxa [sia][1] = node_id;
					for (i=0; i<length; ++i)
						sxa [sia][2+i] = * data ++;
					if (sia >= (SXASW_SIZE - 1))
						sia = 0;
					else
						sia ++;
					++ sca;
				}
				SETENA0 = ISE_CAN;
			}
		}
 	}
}

/****************************************************************************************************/
/* Send LCD display on CAN A																		*/
/****************************************************************************************************/
void transmit_sign (BYTE sign1, BYTE sign2, BYTE sign3, BYTE sign4, BYTE bus)
{
	BYTE fct_code;
	BYTE node_id;
	BYTE data[8];

	fct_code = MPDO;
	node_id = HSE_ID;
	data [0] = HSE_ID;
	data [1] = (BYTE)OS_PROMPT;
	data [2] = OS_PROMPT >> 8;
	data [3] = 2;
	data [4] = sign1;
	data [5] = sign2;
	data [6] = sign3;
	data [7] = sign4;

	send_message(fct_code, node_id, 8, data, bus);
}

/****************************************************************************************************/
/* Send LCD cursor type on CAN A																	*/
/****************************************************************************************************/
void transmit_cursortype (BYTE type, BYTE bus)
{
	BYTE fct_code;
	BYTE node_id;
	BYTE data[8];

	fct_code = MPDO;
	node_id = HSE_ID;
	data [0] = HSE_ID;
	data [1] = (BYTE)OS_PROMPT;
	data [2] = OS_PROMPT >> 8;
	data [3] = 2;
	data [4] = KEY_ESC;
	if (type == _NOCURSOR)
		data [5] = 'f';
	else
		data [5] = 'e';
	data [6] = type;
	data [7] = 0;
	send_message(fct_code, node_id, 8, data, bus);
}

/****************************************************************************************************/
/* Send LCD cursor position on CAN A																*/
/****************************************************************************************************/
void transmit_cursorpos (BYTE row, BYTE column, BYTE bus)
{
	BYTE fct_code;
	BYTE node_id;
	BYTE data[8];

	fct_code = MPDO;
	node_id = HSE_ID;
	data [0] = HSE_ID;
	data [1] = (BYTE)OS_PROMPT;
	data [2] = OS_PROMPT >> 8;
	data [3] = 2;
	data [4] = KEY_ESC;
	data [5] = 'Y';
	data [6] = row;
	data [7] = column;
	send_message(fct_code, node_id, 8, data, bus);
}


