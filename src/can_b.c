
#include "typedef.h"
#include "lpc1700.h"
#include "parameter.h"
#include "state.h"
#include "can.h"
#include "call.h"
#include "rtc.h"
#include "io.h"
#include "history.h"
#include "dict.h"
#include "bootloader.h"
#include "version.h"

//***************************************************
// 函数名: WriteCanB
// 功能: 往CAN B上发送一条消息
// 入参: 
// 出参: 无
// 返回值: 无
//***************************************************
void WriteCanB ()
{
	DWORD id;
	DWORD len;

	CLRENA0 = ISE_CAN;
	if (tib >= (TXBSW_SIZE - 1))						// set write pointer
		tib = 0;
	else
		tib ++;
	if (trb)											// Tx access ?
	{																						// if so, send from here
 		len = txb[tob][0] & 0x0F;
		CAN2TFI1 = (len << 16) | SEND_PRIORITY_LOW;
		id = txb[tob][0] & 0xF0;
		CAN2TID1 = (id << 3) | txb[tob][1];
		CAN2TDA1 = *(DWORD *)&txb[tob][2];
		CAN2TDB1 = *(DWORD *)&txb[tob][6];
		CAN2CMR = CMD_TRAN_BUFF1;
		trb = 0;											// Tx not ready
		if (tob >= (TXBSW_SIZE - 1))					// set read pointer
			tob = 0;
		else
			tob++;
	}
	else												// no Tx access
		tcb++;	
	SETENA0 = ISE_CAN;
}

//***************************************************
// 函数名: ReadCanB
// 功能: 处理CAN B上的一条消息
// 入参: 无
// 出参: 无
// 返回值: 无
//***************************************************
void ReadCanB (void)
{
	WORD wordvalue;
	BYTE i, j;
	WORD id;
	WORD index;
	BYTE subindex;
	DWORD result;

	id = rxb[rob][1];									// read node ID
	switch (rxb[rob][0])								// function code of identifier
	{
		case (PDO_IN):
			for (i = 0; i < MAX_IO_TYPE; i++) 					// read input function
				virt_in [i] = rxb [rob][i + 2]; 		// write to virtual input mapping
			if(virt_in[IO_ENABLE] == ENABLE)
				handle_input (LIFT1, 1);						// handle input
			if ((virt_in[0] == HALL_CALL) ||
				(virt_in[0] == HALL_CALL_SPECIAL) ||
				(virt_in[0] == HALL_CALL_ADVANCED) ||
				(virt_in[0] == HALL_CALL_EMERGENCY))
			{
				if (p.groupsize > 1)
					set_hallcall_c();
			}
			break;

		case (EMERGENCY):
			if (rxb [rob][4] & ER_COMMUNICATION)			// Communication error
			{
				wordvalue = *(WORD *)&rxb [rob][2];
				switch (wordvalue)
				{
					case (E_CAN_OVERRUN_B):
					case (E_CAN_OV_SW_B):
					case (E_BUS_OFF_B):
					case (E_CAN_PASSIVE_B):
						if(wordvalue != E_CAN_PASSIVE_B)
							{
								if ((id >= ESE_ID) && (id < (ESE_ID + MAX_ESE)))
									{
										for (i = 0; i < MAX_ESE_DW; i++)		// request state of all ESE boards
											ese_state_request [i] = ese_enabled [i];
									}
							}
						canb_error_count ++;
						if(wordvalue == E_BUS_OFF_B)
							write_errorhist (wordvalue, id + 128, 0, 5);
						break;
				}
			}
			break;
			
		case (MPDO):
			wordvalue = *(WORD *)&rxb [rob][3]; 		// read object index
			i = rxb[rob][5];							// read object sub-index
			switch (wordvalue)
				{
					case (OS_PROMPT):							// from handheld terminal
						if (((rxb[rob][2] & 0x7F) == HSE_ID) && (i == 1))	// MPDO for this HSE and stdin (key)
							{
								wordvalue = *(WORD *)&rxb[rob][6];	// read object value
								if (wordvalue == 2)
									{
										handheld_active = 0;
										handheld_dly = HANDHELDDLY;
										handheld_bak = 2;
										for (i = 0; i < MAX_LCD_ROW; i++)
											{
												for (j = 0; j < MAX_LCD_COL; j++)
													lcd_old [i][j] = 0x00;
											}
										cursorpos_old [0] = 0xFF;
										cursorpos_old [1] = 0xFF;
										cursortype_old = 0xFF;
										handheldtimer = HANDHELDTIME;
									}
								if (handheld_active == 2)
									{
										handheldtimer = HANDHELDTIME;		// retrigger timer
										handheld_key = handheld_key_read (wordvalue);
									}
							}
						break;
					}
				break;
			
		case (RSDO):
			if (id == HSE_ID)		// SDO for this HSE
			{
				i = rxb [rob][2];							// read SDO type
				switch (i & COMMAND_SPECIFIER)				// check command specifier of SDO
				{
					case (INIT_WRITE_REQ):					// init write or expedited write
						index = *(WORD *)&rxb[rob][3];			// read object index
						subindex = rxb[rob][5]; 				// read object subindex
						sdo_size = 0;							// reset old SDO object size (normal transfer)
						result = write_dict (index, subindex, i, &rxb[rob][6], 1);
						if (result) 							// wrong access to object dictionary
							abort_sdo_b (result);					// abort SDO transfer
						break;
						
					case (WRITE_SEGM_REQ):					// write segment
						result = write_segment (i, &rxb[rob][3], 1);
						if (result) 							// wrong access to object dictionary
						{
							abort_sdo_b (result);					// abort SDO transfer
							sdo_size = 0;
						}
						break;
						
					case (INIT_READ_REQ):						// init read or expedited read
						index = *(WORD *)&rxb[rob][3];			// read object index
						subindex = rxb[rob][5]; 				// read object subindex
						sdo_size = 0;							// reset old SDO object size (normal transfer)
						result = read_dict (index, subindex, 1);// read object dictionary and send data
						if (result) 							// wrong access to object dictionary
							abort_sdo_b (result);					// abort SDO transfer
						break;
						
					case (READ_SEGM_REQ):						// read segment
						result = read_segment (i);				// read next object segment
						if (result) 							// wrong access to object dictionary
						{
							abort_sdo_b (result);					// abort SDO transfer
							sdo_size = 0;
						}
						break;
						
					case (ABORT_REQ):							// abort request
						sdo_size = 0;							// reset old SDO object size (normal transfer)
						break;
						
					default:									// unknown SDO command specifier
						sdo_size = 0;							// reset old SDO object size (normal transfer)
						abort_sdo_b (SDO_NOT_VALID);
						break;
				}
			}
			break;

		case (LSS):
			if (id == LSS_RES_ID && rxb[rob][2] == SET_NODE_ID)
				lss_res = rxb[rob][3];
			break;
	}
	if (rob >= (RXBSW_SIZE - 1))
		rob = 0;
	else
		rob++;
	rcb--;
}


//**************************************************************************************************
// check transmit buffer
//**************************************************************************************************
BYTE check_txb (void)
{
	if (!teb)											// TX not enabled
		return (0);
	if (canbtxerror >= TXERRORLIMIT)
		return (0);
	if (tcb >= TXBSW_SIZE)											// TX software buffer overrun
	{
		if (!(canerrorstate & TXB_SW_OVERRUN))
		{
			canerrorstate |= TXB_SW_OVERRUN;					// set CAN error state
			canb_error_count += 5;
		}
		return (0);										// buffer overrun
	}	
	if(canb_error_count)
		--canb_error_count;
	return (1);										// buffer empty
}

//**************************************************************************************************
// transmit SDO request
//**************************************************************************************************
void transmit_sdo_req_ese (WORD node_id, BYTE command, WORD index, BYTE subindex, DWORD value)
{
	if (check_txb ())									// transmit buffer empty
	{
		txb [tib][0] = RSDO | 8;						// write Function code and data length
		txb [tib][1] = node_id;
		txb [tib][2] = command;
		txb [tib][3] = index;						// write index
		txb [tib][4] = index >> 8;
		txb [tib][5] = subindex;						// write sub-index
		txb [tib][6] = value;						// write value
		txb [tib][7] = value >> 8;					// write value
		txb [tib][8] = value >> 16;					// write value
		txb [tib][9] = value >> 24;					// write value
		WriteCanB ();									// transmit message and set FIFO pointer
	}
}

//**************************************************************************************************
// transmit SDO segment
//**************************************************************************************************
void transmit_sdo_seg_ese (WORD node_id, BYTE command, BYTE *pointer, BYTE size)
{
	BYTE i;

	if (check_txb ())									// transmit buffer empty
	{
		txb [tib][0] = RSDO | 8;						// write Function code and data length
		txb [tib][1] = node_id;
		txb [tib][2] = command;
		for (i = 0; i < size; i++)
			txb [tib][i + 3] = *pointer++;				// write data bytes
		for (i = size; i < 7; i++)
			txb [tib][i + 3] = 0;							// clear unused bytes
		WriteCanB ();									// transmit message and set FIFO pointer
	}
}

//**********************************************************************************************
// SDO response
//**********************************************************************************************
void sdo_response_b (BYTE command, WORD index, BYTE subindex, DWORD value)
{
	if (check_txb ())									// transmit buffer empty
	{
		txb [tib][0] = TSDO | 8;						// write function code + data length
		txb [tib][1] = HSE_ID;
		txb [tib][2] = command;							// write command specifier
		txb [tib][3] = index;							// write index
		txb [tib][4] = index >> 8;
		txb [tib][5] = subindex;						// write sub-index
		txb [tib][6] = value;							// write value
		txb [tib][7] = value >> 8;
		txb [tib][8] = value >> 16;
		txb [tib][9] = value >> 24;
		WriteCanB ();									// transmit message and set FIFO pointer
	}
}

//**************************************************************************************************
// transmit SDO segment
//**************************************************************************************************
void sdo_segment_b (BYTE command, BYTE *value)
{
	BYTE i;

	if (check_txb ())									// transmit buffer empty
	{
		txb [tib][0] = TSDO | 8;							// write Function code and data length
		txb [tib][1] = HSE_ID;
		txb [tib][2] = command;							// command for SDO segment
		for (i = 0; i < 7; i++)
			txb [tib][i + 3] = *value++;						// write data bytes
		WriteCanB ();									// transmit message and set FIFO pointer
	}
}

//**************************************************************************************************
// abort SDO transfer
//**************************************************************************************************
void abort_sdo_b (DWORD errorcode)
{
	if (check_txb ())									// transmit buffer empty
	{
		txb [tib][0] = TSDO | 8;							// write function code + data length
		txb [tib][1] = HSE_ID;
		txb [tib][2] = ABORT_REQ;						// write command specifier
		txb [tib][3] = 0;
		txb [tib][4] = 0;
		txb [tib][5] = 0;
		txb [tib][6] = errorcode;							// errorcode (reason for abort request)
		txb [tib][7] = errorcode >> 8;
		txb [tib][8] = errorcode >> 16;
		txb [tib][9] = errorcode >> 24;
		WriteCanB ();									// transmit message and set FIFO pointer
	}
}

//**************************************************************************************************
// transmit NMT message for ESE board
//**************************************************************************************************
void send_nmt_ese (BYTE state, WORD ese_number)
{
	if (check_txb ())									// transmit buffer empty
	{
		txb [tib][0] = NMT | 3;						// write Function code and data length
		txb [tib][1] = 0;									// no node id necessary, set to 0
		txb [tib][2] = state;								// NMT state
		txb [tib][3] = ese_number;
		WriteCanB ();									// transmit message and set FIFO pointer
	}
}

//**************************************************************************************************
// Set level display
//**************************************************************************************************
void level_display_b (BYTE floor){
	if (check_txb ())									// transmit buffer empty
		{
			txb [tib][0] = PDO_OUT | 6;						// write Function code and data length
			txb [tib][1] = 0;						// write Lift number
			txb [tib][IO_BASIC_FUNC+2]	= POSITION_INDICATOR;
			if (floor == 0xFF)
			{
				txb [tib][IO_SUB_FUNC + 2] = 0;				// display off
				txb [tib][IO_DOOR + 2] = 0;
				txb [tib][IO_STATE	+ 2] = 0;
			}
			else
				{
					txb [tib][IO_SUB_FUNC + 2] = floor + 1;			// write floor number
					if ((hse_state == H_STANDSTILL) && (car_switched_off || remote_switched_off))
						{
							txb [tib][IO_DOOR + 2] = 'S';
							txb [tib][IO_STATE	+ 2] = 'T';
						}
					else if((insp || em_op) && level_insp_fg)
						{
							txb [tib][IO_DOOR + 2] = 'J';
							txb [tib][IO_STATE + 2] = 'X';
						}
					else
						{
							txb [tib][IO_DOOR + 2] = p.sign[floor] >> 8;
							txb [tib][IO_STATE	+ 2] = p.sign[floor];			// write floor sign
						}
				}
			txb [tib][IO_LIFT + 2] = LIFT1;
			txb [tib][IO_FLOOR + 2]	= 0xFF;					// for all floors = 0xFF
			WriteCanB ();									// transmit message and set FIFO pointer
		}
}

//**************************************************************************************************
// Set outputs on CAN bus B (shaft bus)
//**************************************************************************************************
void set_out_b (BYTE func, BYTE sub_func, BYTE floor, BYTE doors, BYTE state)
{
	if (check_txb ())									// transmit buffer empty
	{
		txb [tib][0] = PDO_OUT | 6;						// write Function code and data length
		txb [tib][1] = 0;						// write node ID
		txb [tib][IO_BASIC_FUNC + 2] = func;				// write output function
		txb [tib][IO_SUB_FUNC + 2] = sub_func;			// write output sub function
		txb [tib][IO_LIFT + 2] = LIFT1;
		txb [tib][IO_FLOOR + 2] = floor;					// write floor number
		txb [tib][IO_DOOR + 2] = doors;					// write door numbers
		txb [tib][IO_STATE + 2] = state;					// write state
		WriteCanB ();									// transmit message and set FIFO pointer
	}
}

//**************************************************************************************************
// Send heartbeat on CAN bus B (shaft bus)
//**************************************************************************************************
void send_heartbeat_b (void){///1s 发送
	BYTE help, i;

	if (CAN2SR & SR_TCS2)
	{
		CLRENA0 = ISE_CAN;
		CAN2TFI2 = (0x01ul << 16) | SEND_PRIORITY_HIGH;
		CAN2TID2 = (HEARTBEAT << 3) | HSE_ID;
		CAN2TDA2 = OPERATIONAL;
		CAN2CMR = CMD_TRAN_BUFF2;
		SETENA0 = ISE_CAN;
		canbtxerror = 0;
	}
	else
	{
		help = 0;
		for (i = 0; i < MAX_ESE_DW; i++)
		{
			if (ese_enabled [i])
				help = 1;
		}
		if ((help) && (canbtxerror >= TXERRORLIMIT))
		{
			canerrorstate |= CANB_PCB;
			canb_error_count += 5;
		}
		if (canbtxerror <= TXERRORLIMIT)
			canbtxerror ++;
		txb_ese_error = 20;
		CAN2CMR = CMD_TRAN_BUFF2;
	}
}

//**************************************************************************************************
// transmit HSE state
//**************************************************************************************************
void send_hse_state_b (void)
{
	if (check_txb ())									// transmit buffer empty
	{
		txb [tib][0] = MPDO | 8;						// write Function code and data length
		txb [tib][1] = HSE_ID;				// write node ID
		txb [tib][2] = HSE_ID;				// source address mode
		txb [tib][3] = (BYTE)HSE_STATE;						// write Index
		txb [tib][4] = HSE_STATE >> 8;
		txb [tib][5] = 0;								// write sub index
		txb [tib][6] = hse_state;						// write HSE state
		txb [tib][7] = 0;
		txb [tib][8] = 0;
		txb [tib][9] = 0;
		WriteCanB ();									// transmit message and set FIFO pointer
	}
}

//**************************************************************************************************
// Send time message
//**************************************************************************************************
void time_message_b (void)
{
	if (check_txb ())									// transmit buffer empty
	{
		txb [tib][0] = FC_D | 6;						// write function code + data length
		txb [tib][1] = EMS_ID;
		txb [tib][2] = t.second;						// write time message
		txb [tib][3] = t.minute;
		txb [tib][4] = t.hour;
		txb [tib][5] = t.day;
		txb [tib][6] = (t.weekday & 0x07) + (t.month << 3);
		txb [tib][7] = t.year;
		WriteCanB ();									// transmit message and set FIFO pointer
	}
}

//**************************************************************************************************
//Set monitoring message
//**************************************************************************************************
void monitoring_message_canb(void){
	BYTE i,j;

	if (check_txb ())									// transmit buffer empty
	{
		txb [tib][0] = MPDO | 8;						// write function code + data length
		txb [tib][1] = EMS_ID;							// write node id of monitorng
		txb [tib][2] = 0;								// write command specifier

		i = door_state[0] >> 12;						// set the door1 state to message
		if(direction_ind & DIR_UP)						// set the up direction to message
			i |= 0x80;
		else
			i &=~0x80;

		if(direction_ind & DIR_DN)						// set the down direction to message
			i |= 0x40;
		else
			i &=~0x40;

		if(direction_ind & (direction_ind >> 4))		// set the moving bit to the message
			i |= 0x20;
		else
			i &=~0x20;

		if (level == 0xFF)
		{
			txb [tib][3] = 0;								// display off
			txb [tib][4] = 0;
			txb [tib][5] = 0;
		}
		else
		{
			txb [tib][3] = level + 1;						// write floor number
			txb [tib][4] = p.sign[level] >> 8;
			txb [tib][5] = p.sign[level];					// write floor sign
		}
		txb [tib][6] = i;								// direction and door1 state
		i = door_state[1] >> 8;							// set the door1 state to message
		j = 0;
		txb [tib][7] = i | j;
		i = 0x00;
		if(car_attendance)
			i |= 0x80;
		if(firealarm)
			i |= 0x40;
		if(callpriority > 0)
			i |= 0x20;
		if(overload)
			i |= 0x10;
		if(out_of_order)
			i |= 0x08;
		if(ups_power)
			i |= 0x04;
		if(fault_signal)
			i |= 0x02;
		if(carlight)
			i |= 0x01;
		txb [tib][8] = i;

		i = 0x00;
		if (firemode)
			i |= 0x40;
		if (firealarmmode)
			i |= 0x20;
		if (evacuation)
			i |= 0x10;
		if(fullload)
			i |= 0x80;
		if (ups_power)
			i |= 0x08;
		if(hse_state == H_INSP)
			i |= 0x04;
		if(hse_state == H_EM_OP)
			i |= 0x02;
		if(switched_off_ind)
			i |= 0x01;
		txb [tib][9] = i;
		WriteCanB ();									// transmit message and set FIFO pointer
	}
}

