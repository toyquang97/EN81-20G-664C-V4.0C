
#include "typedef.h"
#include "string.h"
#include "lpc1700.h"
#include "parameter.h"
#include "state.h"
#include "can.h"
#include "dict.h"
#include "pos.h"
#include "history.h"
#include "io.h"
#include "bootloader.h"
#include "version.h"


//***************************************************
// º¯ÊýÃû: WriteCanA
// ¹¦ÄÜ: ÍùCAN AÉÏ·¢ËÍÒ»ÌõÏûÏ¢
// Èë²Î: ÎÞ
// ³ö²Î: ÎÞ
// ·µ»ØÖµ: ÎÞ
//***************************************************
void WriteCanA (void)
{
	DWORD id;
	DWORD len;

	CLRENA0 = ISE_CAN;
	if (tia >= (TXASW_SIZE - 1))						// set write pointer
		tia = 0;
	else
		tia ++;

	if (tra)																		// Tx access 
	{																						// if so, send from here
		len = txa[toa][0] & 0x0F;
		CAN1TFI1 = (len << 16) | SEND_PRIORITY_LOW;
		id = txa[toa][0] & 0xF0;
		CAN1TID1 = (id << 3) | txa[toa][1];
		CAN1TDA1 = *(DWORD *)&txa[toa][2];
		CAN1TDB1 = *(DWORD *)&txa[toa][6];
		CAN1CMR = CMD_TRAN_BUFF1;
		tra = 0;															// Tx not ready
		if (toa >= (TXASW_SIZE - 1))					// set read pointer
			toa = 0;
		else
			toa++;
	}
	else												// no Tx access
		tca++;
	SETENA0 = ISE_CAN;
}

//***************************************************
// º¯ÊýÃû: ReadCanA
// ´¦ÀíCAN AÉÏµÄÒ»ÌõÏûÏ¢
// ÎÞ
// ³ö²Î: ÎÞ
// ·µ»ØÖµ: ÎÞ
//***************************************************
void ReadCanA (void)
{
	WORD wordvalue;
	BYTE id;
	BYTE i, j;
	WORD index;
	BYTE subindex;
	DWORD result;

	id = rxa[roa][1];

	switch (rxa[roa][0])								// function code of identifier
	{
		case (PDO_IN):
			for (i = 0; i < MAX_IO_TYPE; i++)						// read input function
				virt_in [i] = rxa[roa][i + 2];			// write to virtual input mapping
			if(virt_in[IO_ENABLE] == ENABLE)
				handle_input (LIFT1, 1);					// handle input
			break;
		case (PDO_OUT):							// send message from FVE to shaft bus
			i = rxa[roa][2];							// read basic function
			if (i)
			{
				if (i == DOOR_IO)						// door output to set
				{
					if (rxa[roa][7])						// set door output
						dooroutputstate [rxa[roa][6] / 2] |= rxa[roa][3];
					else								// set door output
						dooroutputstate [rxa[roa][6] / 2] &= ~rxa[roa][3];
				}
				set_out (i, rxa[roa][3], rxa[roa][5], rxa[roa][6], rxa[roa][7], O_CANB | O_HSE);
			}
			break;

		case (DOORS):							// door state message
			switch (id)							// ID acts as additional function code
			{
				case (1):									// door state door 1
				case (2):									// door state door 2
				case (3):									// door state door 3
					door_state [id - 1] = *(WORD *)&rxa [roa][2];		// read door state
					if (door_state [id - 1] == DOOR_CLOSED)	// door is closed
					{
						bit_set (doorstate, id - 1);				// set door state in 1 variable
						doorcloseerrorcount [id - 1] = 0;		// reset door close error count
					}
					else										// door is not closed
					{
						bit_reset (doorstate, id - 1);
						if (door_state [id - 1] == DOOR_OPENED)	// door is openend
						{
							dooropenerrorcount [id - 1] = 0;		// reset door open error count
							nextfloortrycount = 0;
							memset(floortried, 0x00, sizeof(floortried));
						}
					}
					break;
				case (5):										// light screen door 1
				case (6):										// light screen door 2
				case (7):										// light screen door 3
					if (bit_select (rxa [roa][2], 6))			// light screen is interrupted
					{
						bit_set (ls_state, id - 5);				// set state bit
						carcallcounter = 0;						// reset misuse counter
					}
					else
						bit_reset (ls_state, id - 5);			// reset state bit
					break;
			}
			break;
			
		case (EMERGENCY):
			wordvalue = *(WORD *)&rxa [roa][2];
			switch (wordvalue)
			{
				case (E_CAN_OVERRUN_A):
				case (E_BUS_OFF_A):
					if ((id >= TSE_ID) && (id < (TSE_ID + MAX_TSE)))
						tse_state_request |= (0x0001 << (id - TSE_ID));
					else if((id >= EXE_ID) && (id < (EXE_ID + MAX_EXE)))						
						exe_state_request = exe_enabled & 0x0F;
					break;
					
				case (E_DOOR_CLOSING_1):
				case (E_DOOR_CLOSING_2):
				case (E_DOOR_CLOSING_3):
					i = ((wordvalue >> 8) & 0x0F) - 1;		// read door number
					if ((i < p.doornumber) && (doorcloseerrorcount [i] < 100))
						doorcloseerrorcount [i]++;
					break;
					
				case (E_DOOR_OPENING_1):
				case (E_DOOR_OPENING_2):
				case (E_DOOR_OPENING_3):
					i = ((wordvalue >> 8) & 0x0F) - 1;		// read door number
					if ((i < p.doornumber) && (dooropenerrorcount [i] < 100) && (dooropenerrorfloor [i] != level))
					{
						dooropenerrorcount [i]++;				// increment door open error count
						dooropenerrorfloor [i] = level;			// save floor of last open error
					}
					break;
				
				case (E_DOOR_SC_1):
				case (E_DOOR_SC_2):
				case (E_DOOR_SC_3):
					if(p.door_connect)
						dl_shorted |= SC3;
					else
						dl_shorted |= SC2;
					break;
					
				case (E_INTERNAL_SW):
					break;
			}
			if ((wordvalue) && (wordvalue != E_DOOR_SC_1) && (wordvalue != E_DOOR_SC_2) && (id != DSE_ID))
				{//ÊÖ³Ö²Ù×÷Æ÷µÄ¹ÊÕÏ²»ÔÚ¼ÇÂ¼
					if((wordvalue != E_DOOR_REV_1) && (wordvalue != E_DOOR_REV_2))
						{
							if((wordvalue != E_BUS_OFF_A) && (wordvalue != E_CAN_OVERRUN_A) &&
									(wordvalue != E_CAN_OV_SW_A) && (wordvalue != E_CAN_TX_SW_A) &&
									(wordvalue != E_CAN_SX_SW_A) && (wordvalue != E_CAN_TX_A))
								write_errorhist (wordvalue, id, 0, 5);
							else if((!p.canbus_check) || (drive_state == D_STOPPED))
								write_errorhist (wordvalue, id, 0, 5);
						}
					else 
						{
							i = ((wordvalue >> 8) & 0x0F) - 1;		// read door number
							write_errorhist (wordvalue, id, 0, 5);
						}					
				}
//Can ´íÎó¼ÆÊý
			if((wordvalue == E_BUS_OFF_A) || (wordvalue == E_CAN_PASSIVE_A))				
				cana_error_count += 2;
			else if	((wordvalue == E_CAN_OVERRUN_A) || (wordvalue == E_CAN_OV_SW_A) || 
							(wordvalue == E_CAN_TX_SW_A) || (wordvalue == E_CAN_SX_SW_A) || 
							(wordvalue == E_CAN_TX_A))
				cana_error_count ++;
			break;
				
		case (FC_3):									// Function code is 3
			switch (id) 								// ID acts as additional function code
				{
					case (LOAD_ID): 							// Load message
						if (p.loadtype == ANALOG_ON_CDU)
							load = rxa [roa][2];					// read load state
						break;
				}
			break;

		case (RSDO):									// Function code is 0xC0 (RSDO and FU Lust bus)
			switch (id)
			{
				case (HSE_ID):								// RSDO message for HSE
					i = rxa [roa][2];							// read SDO type
					switch (i & COMMAND_SPECIFIER)			// check command specifier of SDO
					{
						case (INIT_WRITE_REQ):					// init write or expedited write
							index = *(WORD *)&rxa[roa][3];		// read object index
							subindex = rxa[roa][5];				// read object subindex
							sdo_size = 0;							// reset old SDO object size (normal transfer)
							result = write_dict (index, subindex, i, &rxa[roa][6], 0);
							if (result)							// wrong access to object dictionary
								abort_sdo (result);					// abort SDO transfer
							break;
						case (WRITE_SEGM_REQ):					// write segment
							result = write_segment (i, &rxa[roa][3], 0);
							if (result)							// wrong access to object dictionary
							{
								abort_sdo (result);					// abort SDO transfer
								sdo_size = 0;
							}
							break;
						case (INIT_READ_REQ):					// init read or expedited read
							index = *(WORD *)&rxa[roa][3];		// read object index
							subindex = rxa[roa][5];				// read object subindex
							sdo_size = 0;							// reset old SDO object size (normal transfer)
							result = read_dict (index, subindex, 0);	// read object dictionary and send data
							if (result)							// wrong access to object dictionary
								abort_sdo (result);					// abort SDO transfer
							break;
						case (READ_SEGM_REQ):					// read segment
							result = read_segment (i);			// read next object segment
							if (result)							// wrong access to object dictionary
							{
								abort_sdo (result);					// abort SDO transfer
								sdo_size = 0;
							}
							break;
						case (ABORT_REQ):						// abort request
							sdo_size = 0;							// reset old SDO object size (normal transfer)
							break;
						default:								// unknown SDO command specifier
							sdo_size = 0;							// reset old SDO object size (normal transfer)
							abort_sdo (SDO_NOT_VALID);
					}
					break;
			}
			break;
			
		case (MPDO):
			if (id == MON_ID)
				{
					monitoring_state = rxa [roa][2];			// read state from monitoring
					monitoring_delay_time = rxa[roa][3];		// read the delay time from	monitoring
				}
			else
			{
				wordvalue = *(WORD *)&rxa [roa][3];			// read object index
				i = rxa[roa][5];							// read object sub-index
				switch (wordvalue)
				{
					case (DOORTIMER):
						if (i && (i <= p.doornumber))
							doortimer [i - 1] = rxa[roa][6];		// read value
						break;
					case (OS_PROMPT):							// from handheld terminal
						if (((rxa[roa][2] & 0x7F) == HSE_ID) && (i == 1))	// MPDO for HSE
						{
							wordvalue = *(WORD *)&rxa[roa][6];	// read object value
							if (wordvalue == 2)
							{// handheld is not active
								handheld_active = 0;
								handheld_dly = HANDHELDDLY;
								handheld_bak = 1;
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
							if (handheld_active == 1)
							{
								handheldtimer = HANDHELDTIME;		// retrigger timer
								handheld_key = handheld_key_read (wordvalue);
							}
						}
						break;
				}
			}
			break;

//Ôö¼Ó²âÊÔÒÇµÄÆô¶¯¹¦ÄÜ		
		case (NMT):			
			if (id == MON_ID)
				{
					wordvalue = *(WORD *)&rxa [roa][3]; 		// read object index
					if(wordvalue == 0xAA55)
						send_pos_fg = 1;
					else if(wordvalue == 0xFFFF)
						send_pos_fg = 0;
				}
			break;
//Ôö¼Ó²âÊÔÒÇµÄÆô¶¯¹¦ÄÜ		
	}
	if (roa >= (RXASW_SIZE - 1))
		roa = 0;
	else
		roa++;
	rca--;
}


//**************************************************************************************************
// check transmit buffer
//**************************************************************************************************
BYTE check_txa (void)
{
	if (canatxerror >= TXERRORLIMIT)
		return (0);
	if (!tea)											// TX not enabled
		return (0);
	if (tca >= TXASW_SIZE)											// TX software buffer overrun
	{
		if ((!(canerrorstate & TXA_SW_OVERRUN)) && (cana_busoff_checktime < timer))
		{
			canerrorstate |= TXA_SW_OVERRUN;				// set CAN error state
			cana_error_count += 5;
			write_errorhist (E_CAN_TX_SW_A, HSE_ID, 0, W_TXA_OVERRUN);
			cana_busoff_checktime = timer + 3 SEC;
		}
		return (0);										// buffer overrun
	}
	if(cana_error_count)
		--cana_error_count;
	return (1);										// buffer empty
}

//**************************************************************************************************
// Send heartbeat on CAN bus A
//**************************************************************************************************
void send_heartbeat_a (void){//100ms ·¢ËÍ
	if (CAN1SR & SR_TCS2)
	{
		if (tea) 										// TX not enabled
			{
				CLRENA0 = ISE_CAN;
				CAN1TFI2 = (0x01ul << 16) | SEND_PRIORITY_HIGH;
				CAN1TID2 = (HEARTBEAT << 3) | HSE_ID;
				CAN1TDA2 = OPERATIONAL;
				SETENA0 = ISE_CAN;
				canatxerror = 0;
			}
		CAN1CMR = CMD_TRAN_BUFF2;
	}
	else
	{
		if (canatxerror >= TXERRORLIMIT)
		{
			if (!(hb_error & B_CAN))
				{
					write_errorhist (E_CAN_TX_A, HSE_ID, 0, W_CAN_TX_A);
					hb_error |= B_CAN;
				}
			cana_error_count += 5;
		}
		if (canatxerror <= TXERRORLIMIT)
			canatxerror++;
		txa_fve_error = 200;//20;			//Éè¶¨ 10s ÐÄÌø
		txa_tse_error = 20;
		txa_exe_error = 20;
		CAN1CMR = CMD_TRAN_BUFF2;
	}
}

//**************************************************************************************************
// transmit HSE state
//**************************************************************************************************
void send_hse_state_a (void)
{
	if (check_txa ())									// transmit buffer empty
	{
		txa [tia][0] = MPDO | 8;						// write Function code and data length
		txa [tia][1] = HSE_ID;							// write node ID
		txa [tia][2] = HSE_ID;							// source address mode
		txa [tia][3] = (BYTE)HSE_STATE;						// write Index
		txa [tia][4] = HSE_STATE >> 8;
		txa [tia][5] = 0;								// write sub index
		txa [tia][6] = hse_state;						// write HSE state
		txa [tia][7] = 0;
		txa [tia][8] = 0;
		txa [tia][9] = 0;
		WriteCanA ();									// transmit message and set FIFO pointer
	}
}

//**************************************************************************************************
// transmit NMT message
//**************************************************************************************************
void send_nmt (BYTE state, BYTE node_id)
{
	if (check_txa ())									// transmit buffer empty
	{
		txa [tia][0] = NMT | 2;							// write identifier
		txa [tia][1] = 0;								// no node id necessary, set to 0
		txa [tia][2] = state;							// NMT state
		txa [tia][3] = node_id;							// ID of node to start
		WriteCanA ();									// transmit message and set FIFO pointer
	}
}

//**************************************************************************************************
// transmit SDO request
//**************************************************************************************************
void transmit_sdo_req (BYTE node_id, BYTE command, WORD index, BYTE subindex, DWORD value)
{
	if (check_txa ())									// transmit buffer empty
	{
		txa [tia][0] = RSDO | 8;						// write Function code and data length
		txa [tia][1] = node_id;							// write node ID
		txa [tia][2] = command;							// write SDO command
		txa [tia][3] = index;							// write index
		txa [tia][4] = index >> 8;
		txa [tia][5] = subindex;						// write sub-index
		txa [tia][6] = value;							// write value
		txa [tia][7] = value >>	8;						// write value
		txa [tia][8] = value >> 16;						// write value
		txa [tia][9] = value >> 24;						// write value
		WriteCanA ();									// transmit message and set FIFO pointer
	}
}

//**************************************************************************************************
// transmit SDO segment
//**************************************************************************************************
void transmit_sdo_seg (BYTE node_id, BYTE command, BYTE *pointer, BYTE size)
{
	BYTE i;

	if (check_txa ())									// transmit buffer empty
	{
		txa [tia][0]	 = RSDO | 8;					// write Function code and data length
		txa [tia][1]	 = node_id;					// write node ID
		txa [tia][2]	 = command;					// command for SDO segment
		for (i = 0; i < size; i++)
			txa [tia][i + 3] = *pointer++;				// write data bytes
		for (i = size; i < 7; i++)
			txa [tia][i + 3] = 0;
		WriteCanA ();									// transmit message and set FIFO pointer
	}
}


//**************************************************************************************************
// transmit inputs
//**************************************************************************************************
void transmit_inputs (BYTE func, BYTE sub_func, BYTE state)
{
	if (check_txa ())									// transmit buffer empty
	{
		txa [tia][0] = PDO_IN | 6;		// write Function code and data length
		txa [tia][1] = LIFT1;			// write node ID
		txa [tia][IO_BASIC_FUNC + 2] = func;			// write basic function
		txa [tia][IO_SUB_FUNC + 2] = sub_func;		// write sub function
		txa [tia][IO_LIFT + 2] = LIFT1;				// write lift number (set to 1 on control bus)
		txa [tia][IO_FLOOR + 2] = 0;				// not used
		txa [tia][IO_DOOR + 2] = 0;				// not used
		if (state)
			txa [tia][IO_STATE + 2] = 1;				// inputs are on
		else
			txa [tia][IO_STATE + 2] = 0;				// all inputs off
		WriteCanA ();									// transmit message and set FIFO pointer
	}
}

//**************************************************************************************************
// transmit door controlword
//**************************************************************************************************
void door_command (BYTE door1, BYTE door2)
{
	static BYTE door1command_old = 0;
	static BYTE door2command_old = 0;
	
	if (forced_closing ())
	{
		if (door1 == CLOSE_DOOR)						// door close command
			door1 = CLOSE_DOOR_L;							// close door with force limit
		if (door2 == CLOSE_DOOR)						// door close command
			door2 = CLOSE_DOOR_L;							// close door with force limit
	}
	if (check_txa ())									// transmit buffer empty
	{
		txa [tia][0] = DOORS | 4;						// write Function code and data length
		txa [tia][1] = DOOR_ID;
		txa [tia][2] = 0;
		txa [tia][3] = door1;							// Control for door 1
		txa [tia][4] = 0;
		txa [tia][5] = door2;							// Control for door 2
		WriteCanA ();									// transmit message and set FIFO pointer
	}

//	Ôö¼Ó¿ª¹ØÃÅ°´Å¥Ö¸Ê¾µÆµÄ²Ù×÷  2014-06-26
	if (!firedoormode && ((door1 != door1command_old) || ((door2 != door2command_old) && (p.doornumber > 1))))		// transmit buffer empty
		{
			if((((door1 == CLOSE_DOOR) || (door1 == CLOSE_DOOR_L)))
				|| (((door2 == CLOSE_DOOR) || (door2 == CLOSE_DOOR_L))))
				{//¹ØÃÅ 		
					if((door1 == CLOSE_DOOR) || (door1 == CLOSE_DOOR_L))
						door_close_open = (door_close_open & 0x03) | 1;
					if((door2 == CLOSE_DOOR) || (door2 == CLOSE_DOOR_L))
						door_close_open = (door_close_open & 0x03) | 2;
					cl_op_fg = 0;
				}
			else if((((door1 == OPEN_DOOR) || (door1 == OPEN_DOOR_L)))
				|| (((door2 == OPEN_DOOR) || (door2 == OPEN_DOOR_L))))
				{//¿ªÃÅ				
					door_close_open = 8;
					cl_op_fg = 0;
				}
			door1command_old = door1;
			door2command_old = door2;
		}
//	Ôö¼Ó¿ª¹ØÃÅ°´Å¥Ö¸Ê¾µÆµÄ²Ù×÷	2014-06-26

}

//**************************************************************************************************
// transmit load measurement inputs
//**************************************************************************************************
void transmit_load (BYTE state)
{
	BYTE i;

	if (check_txa ())									// transmit buffer empty
	{
		txa [tia][0] = FC_3 | 1;					// write Function code and data length
		txa [tia][1] = LOAD_ID;					// write second part of ID
		txa [tia][2] = state;						// write state of load inputs
		for (i = 3; i <= 9; i++)
			txa [tia][i] = 0;
		WriteCanA ();									// transmit message and set FIFO pointer
	}
}

//**************************************************************************************************
// transmit emergency message
//**************************************************************************************************
void transmit_emergency (WORD errortype)
{
	if (check_txa ())									// transmit buffer empty
	{
		txa [tia][0] = EMERGENCY | 8;					// On Lust bus used as system start message
		txa [tia][1] = LIFT1;							// write node ID
		txa [tia][2] = errortype;						// write error code
		txa [tia][3] = errortype >> 8;
		txa [tia][4] = 0;								// error register not used now (to be done)
		txa [tia][5] = 0;								// user specific error code (not used)
		txa [tia][6] = 0;
		txa [tia][7] = 0;
		txa [tia][8] = 0;
		txa [tia][9] = 0;
		WriteCanA ();									// transmit message and set FIFO pointer
	}
}

//**************************************************************************************************
// transmit position and speed
//**************************************************************************************************
void transmit_position (DWORD act_pos, WORD act_speed)
{
	if (check_txa ())									// transmit buffer empty
	{
		txa [tia][0] = FC_3 | 6;
		txa [tia][1] = POS_ID;							// write node ID
		txa [tia][2] = act_pos;							// write actual position
		txa [tia][3] = act_pos >>	8;
		txa [tia][4] = act_pos >> 16;
		txa [tia][5] = act_pos >> 24;
		txa [tia][6] = act_speed;						// write actual speed
		txa [tia][7] = act_speed >> 8;
		WriteCanA ();									// transmit message and set FIFO pointer
	}
}

//**************************************************************************************************
// Set level display
//**************************************************************************************************
void level_display_a (BYTE floor){
	if (check_txa ())									// transmit buffer empty
		{
			txa [tia][0] = PDO_OUT | 6;		// write Function code and data length
			txa [tia][1] = 0;				// write Lift number (Lift 1 = 0 on control bus)
			txa [tia][IO_BASIC_FUNC + 2] = POSITION_INDICATOR;
			if (floor == 0xFF)
				{
					txa [tia][IO_SUB_FUNC + 2] = 0;				// display off
					txa [tia][IO_DOOR + 2] = 0;
					txa [tia][IO_STATE	+ 2] = 0;
				}
			else
				{
					txa [tia][IO_SUB_FUNC + 2] = floor + 1;		// write floor number
					if ((hse_state == H_STANDSTILL) && (car_switched_off || remote_switched_off))
						{
							txa [tia][IO_DOOR + 2] = 'S';
							txa [tia][IO_STATE + 2] = 'T';
						}
					else if((insp || em_op) && level_insp_fg)
						{
							txa [tia][IO_DOOR + 2] = 'J';
							txa [tia][IO_STATE + 2] = 'X';
						}
					else
						{
							txa [tia][IO_DOOR + 2] = p.sign[floor] >> 8;
							txa [tia][IO_STATE + 2] = p.sign[floor];			// write floor sign
						}
				}
			txa [tia][IO_LIFT + 2] = LIFT1;				// write lift number (set to 1 on control bus)
			txa [tia][IO_FLOOR + 2] = 0;				// for car display = 0
			WriteCanA ();									// transmit message and set FIFO pointer
		}
}

//**************************************************************************************************
//Set monitoring message
//**************************************************************************************************
void monitoring_message_cana(void)
{
	BYTE i,j;

	if (check_txa ())									// transmit buffer empty
	{
		txa [tia][0] = MPDO | 8;						// write function code + data length
		txa [tia][1] = EMS_ID;							// write node id of monitorng
		txa [tia][2] = 0;								// write command specifier

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
			txa [tia][3] = 0;								// display off
			txa [tia][4] = 0;
			txa [tia][5] = 0;
		}
		else
		{
			txa [tia][3] = level + 1;						// write floor number
			txa [tia][4] = p.sign[level] >> 8;
			txa [tia][5] = p.sign[level];					// write floor sign
		}
		txa [tia][6] = i;								// direction and door1 state
		i = door_state[1] >> 8;							// set the door1 state to message
		j = 0;
		txa [tia][7] = i | j;
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
		txa [tia][8] = i;

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
		txa [tia][9] = i;
		WriteCanA ();									// transmit message and set FIFO pointer
	}
}
//**************************************************************************************************
// abort SDO transfer
//**************************************************************************************************
void abort_sdo (DWORD errorcode)
{
	if (check_txa ())									// transmit buffer empty
	{
		txa [tia][0] = TSDO | 8;						// write function code + data length
		txa [tia][1] = LIFT1;							// write node id of HSE
		txa [tia][2] = ABORT_REQ;						// write command specifier
		txa [tia][3] = 0;
		txa [tia][4] = 0;
		txa [tia][5] = 0;
		txa [tia][6] = errorcode;						// errorcode (reason for abort request)
		txa [tia][7] = errorcode >> 8;
		txa [tia][8] = errorcode >> 16;
		txa [tia][9] = errorcode >> 24;
		WriteCanA ();									// transmit message and set FIFO pointer
	}
}

//**********************************************************************************************
// SDO response
//**********************************************************************************************
void sdo_response (BYTE command, WORD index, BYTE subindex, DWORD value)
{
	if (check_txa ())									// transmit buffer empty
	{
		txa [tia][0] = TSDO | 8;						// write function code + data length
		txa [tia][1] = LIFT1;							// write node id of HSE
		txa [tia][2] = command;							// write command specifier
		txa [tia][3] = index;							// write index
		txa [tia][4] = index >> 8;
		txa [tia][5] = subindex;						// write sub-index
		txa [tia][6] = value;							// write value
		txa [tia][7] = value >>	8;
		txa [tia][8] = value >> 16;
		txa [tia][9] = value >> 24;
		WriteCanA ();									// transmit message and set FIFO pointer
	}
}

//**************************************************************************************************
// transmit SDO segment
//**************************************************************************************************
void sdo_segment (BYTE command, BYTE *value)
{
	BYTE i;

	if (check_txa ())									// transmit buffer empty
	{
		txa [tia][0] = TSDO | 8;					// write Function code and data length
		txa [tia][1] = LIFT1;					// write node id of HSE
		txa [tia][2] = command;					// command for SDO segment
		for (i = 0; i < 7; i++)
			txa [tia][i + 3] = *value++;					// write data bytes
		WriteCanA ();									// transmit message and set FIFO pointer
	}
}

//**************************************************************************************************
// Set outputs on CAN bus A (control bus)
//**************************************************************************************************
void set_out_a (BYTE func, BYTE sub_func, BYTE floor, BYTE doors, BYTE state)
{
	if (check_txa ())									// transmit buffer empty
	{
		txa [tia][0] = PDO_OUT | 6;		// write Function code and data length
		txa [tia][1] = 0;				// write Lift number (Lift 1 = 0 on control bus)
		txa [tia][IO_BASIC_FUNC + 2] = func;			// write output function
		txa [tia][IO_SUB_FUNC + 2] = sub_func;		// write output sub function
		txa [tia][IO_LIFT + 2] = LIFT1;				// write lift number (set to 1 on control bus)
		txa [tia][IO_FLOOR + 2] = floor;			// write floor number
		txa [tia][IO_DOOR + 2] = doors;			// write door numbers
		txa [tia][IO_STATE + 2] = state;			// write state
		WriteCanA ();									// transmit message and set FIFO pointer
	}
}

/****************************************************************************************************/
/* Send time message																				*/
/****************************************************************************************************/
void time_message_a (void)
{
	if (check_txa ())
	{
		txa [tia][0] = FC_D | 6;
		txa [tia][1] = EMS_ID;
		txa [tia][2] = t.second;
		txa [tia][3] = t.minute;
		txa [tia][4] = t.hour;
		txa [tia][5] = t.day;
		txa [tia][6] = (t.weekday & 0x07) + (t.month << 3);
		txa [tia][7] = t.year;
		WriteCanA ();
	}
}

//·¢ËÍ²âÊÔÒÇËùÐèµÄÎ»ÖÃÐÅÏ¢
void	send_drive_pos(void){	
	if (check_txa ())
	{
		txa [tia][0] = FC_5 | 4;
		txa [tia][1] = MON_ID;
		txa [tia][2] = dCurrentPulse;
		txa [tia][3] = dCurrentPulse >> 8;
		txa [tia][4] = dCurrentPulse >> 16;
		txa [tia][5] = dCurrentPulse >> 24;
		WriteCanA ();
	}
}

