
#include "typedef.h"
#include "lpc1700.h"
#include "parameter.h"
#include "state.h"
#include "text.h"
#include "can.h"
#include "history.h"


/****************************************************************************************************/
/* table with all parameters for initialization of ESE boards										*/
/****************************************************************************************************/
const struct _initpara ese_initpara [] = {
	{&p.ese_inpar	[0][0][0],	5,		INPUT_GROUP,	1,		MAX_ESE_IN,			1	},
	{&p.ese_outpar [0][0][0],	5,		OUTPUT_GROUP,	1,		MAX_ESE_OUT,		1	},
	{&p.ese_inpar	[0][0][5],	1,		INPUT_PARA3,	1,		MAX_ESE_IN,			1	},
	{&p.ese_outpar [0][0][5],	1,		OUTPUT_PARA3,	1,		MAX_ESE_OUT,		1	},
	{&p.ese_inpar [0][0][6],	1,		INPUT_PARA4,	1,		MAX_ESE_IN, 		1 },
	{&p.ese_outpar [0][0][6], 1,		OUTPUT_PARA4, 1,		MAX_ESE_OUT,		1 },
	{&p.arrowtype,						1,		ARROW_MODE,		0,		0,					1	},
	{&p.lcd_backlight,				1,		BACKLIGHT_MODE, 	0,		0,					1 },
	{0,							0xFF,	0,				0,		0,					0	},
};

//**************************************************************************************************
// clear CAN B RX buffer
//**************************************************************************************************
void clear_rxb_buffer (void)
{
	BYTE i, j;
	for (i = 0; i < RXBSW_SIZE; i++)
	{
		for (j = 0; j < 10; j++)
			rxb [i][j] = 0;
	}
	rcb = 0;													// RX message counter
	rob = 0;												// pointer for RX FIFO read
	rib = 0;												// pointer for RX FIFO write
}
//**************************************************************************************************
// clear CAN B SX buffer
//**************************************************************************************************
void clear_sxb_buffer (void)
{
	BYTE i, j;
	for (i = 0; i < SXBSW_SIZE; i++)
	{
		for (j = 0; j < 10; j++)
			sxb [i][j] = 0;
	}
	scb = 0;												// RX message counter
	sob = 0;												// pointer for RX FIFO read
	sib = 0;												// pointer for RX FIFO write
}

//**************************************************************************************************
// clear CAN B TX buffer
//**************************************************************************************************
void clear_txb_buffer (void)
{
	BYTE i, j;
	for (i = 0; i < TXBSW_SIZE; i++)
	{
		for (j = 0; j < 10; j++)
			txb [i][j] = 0;
	}
	tcb = 0;												// TX message counter
	tob = 0;												// pointer for TX FIFO read
	tib = 0;												// pointer for TX FIFO write
	trb = 1;											// TX ready

	for (i = 0; i < SXBSW_SIZE; i++)
	{
		for (j = 0; j < 10; j++)
			sxb [i][j] = 0;
	}
	scb = 0;												// TX message counter
	sob = 0;												// pointer for TX FIFO read
	sib = 0;												// pointer for TX FIFO write
	srb = 1;											// TX ready
}

/****************************************************************************************************/
/* decide if input or output should be requested													*/
/****************************************************************************************************/
BYTE request_ese_io (WORD ese_number, BYTE io_number)
{
	BYTE function;

	if (io_number < MAX_ESE_IN)
		function = p.ese_inpar [ese_number][io_number][IO_BASIC_FUNC];
	else
		function = p.ese_outpar [ese_number][io_number - MAX_ESE_IN][IO_BASIC_FUNC];
	switch (function)
	{
		case (0):
			return (0);
		case (HALL_CALL_SPECIAL):
		case (HALL_CALL_ADVANCED):
		case (HALL_CALL_EMERGENCY):
			return (1);
		case (HALL_CALL):
		case (CAR_CALL):
			if (io_number < MAX_ESE_IN)
				return (0);
			else
				return (1);
		default:
			if (io_number < MAX_ESE_IN)
				return (1);
			else
				return (0);
	}
}

/****************************************************************************************************/
/* handle ESE boards																				*/
/****************************************************************************************************/
void ese (void)
{
	static DWORD ese_timer;
	static BYTE subindex;
	static WORD ese_board;
	static WORD row;
	static DWORD value;
	BYTE help;
	BYTE i;
	
	if (oldese_arrowtype != p.arrowtype)
	{
		oldese_arrowtype = p.arrowtype;
		for (i = 0; i < MAX_ESE_DW; i++)
			ese_init_error[i] = ese_enabled[i];
	}
	if (ese_state == ESE_WATCH)
		ese_sdo_master = 0;
	else
		ese_sdo_master = 1;

	if((ese_state != ESE_BUS_OFF) && (ese_state != ESE_WAIT_CAN))
		teb = 1;
	
	switch (ese_state)
	{
		case (ESE_WATCH):
			help = 0;
			for (i = 0; i < MAX_ESE_DW; i++)
			{
				if (ese_init_error [i])
					help = 1;
			}
			if (canerrorstate & (CANB_BUSOFF | RXB_SW_OVERRUN | CANB_PCB | TXB_SW_OVERRUN | SXB_SW_OVERRUN))					/* CAN B bus off state							*/
			{
				ese_state = ESE_BUS_OFF;
				ese_timer = timer + 1 SEC;					/* wait 1 s									*/
				if(canerrorstate & CANB_BUSOFF)
					teb = 0;									/* disable transmission on CAN B				*/
			}
			else if (help)								/* 1 or more ESE not init						*/
			{
				ese_state = ESE_INIT_START;					/* goto init state								*/
				ese_board = 0;								/* reset ESE board number						*/
				teb = 1;
			}
			else
			{
				help = 0;
				for (i = 0; i < MAX_ESE_DW; i++)
				{
					if (ese_state_request [i])
						help = 1;
				}
				if (help)									/* ESE state request necessary					*/
				{
					ese_state = ESE_STATE_START;				/* goto state search							*/
					ese_board = 0;							/* reset ESE board number						*/
				}
			}
			break;

		case (ESE_BUS_OFF):
			if (ese_timer < timer)						/* waiting time over							*/
			{
				rcb = 0;										/* RX message counter							*/
				rob = 0;										/* pointer for FIFO read						*/
				rib = 0;										/* pointer for FIFO write								*/
				clear_txb_buffer ();
				clear_rxb_buffer ();
				clear_sxb_buffer ();
				init_can_b ();								/* init CAN B									*/
				canb_busoff_checktime = timer + 5 SEC;
				ese_timer = timer + 5 SEC;		/* wait 5 s	*/
				ese_state = ESE_WAIT_CAN;
			}
			break;

		case (ESE_WAIT_CAN):
			if (ese_timer < timer)						/* waiting time over							*/
			{
				if (!(CAN2GSR & 0x80))
				{
					canerrorstate &= ~CANB_MASK;								/* reset CAN B error							*/
					teb = 1;									/* enable transmission on CAN B					*/
					ese_state = ESE_WATCH;						/* go back to normal state						*/
				}
				else
				{
					ese_state = ESE_BUS_OFF;
					ese_timer = timer + 1 SEC;
				}
			}
			else
			{
				if (!(CAN2GSR & 0x80))
				{
					canerrorstate &= ~CANB_MASK;								/* reset CAN B error							*/
					teb = 1;									/* enable transmission on CAN B					*/
					ese_state = ESE_WATCH;						/* go back to normal state						*/
				}
			}
			break;

		case (ESE_INIT_START):
			while ((ese_board < MAX_ESE) &&				/* search 1. board for initialization			*/
						(!(ese_init_error [ese_board / 32] & ((DWORD)1 << (ese_board % 32)))))
				ese_board++;
			if (ese_board < MAX_ESE)						/* next board for init							*/
			{
				row = 0 ;									/* set to 1. parameter							*/
				subindex = ese_initpara [0].subindex_start;	/* set to 1. sub-index of 1. parameter			*/
				ese_state = ESE_INIT_REQ;
			}
			else
			{
				ese_state = ESE_STATE_START;				/* goto state search							*/
				ese_board = 0;								/* reset ESE board number						*/
			}
			break;

		case (ESE_INIT_REQ):
			ese_sdo_id = 0xFFFF;							/* reset marker for SDO received				*/
			if (ese_initpara [row].size <= 4)				/* expedited SDO transfer						*/
			{
				if ((ese_initpara [row].index == OUTPUT_PARA3)
				|| (ese_initpara [row].index == INPUT_PARA3))
				{											/* only for input/output polarity parameter		*/
					value = *((BYTE *)ese_initpara [row].pointer + (((MAX_ESE_IN * ese_board) + (subindex - 1)) * MAX_IO_TYPE));
					value <<= 8;
				}
				else if ((ese_initpara [row].index == OUTPUT_PARA4)
								|| (ese_initpara [row].index == INPUT_PARA4))
					{
						value = *((BYTE *)ese_initpara [row].pointer + (((MAX_ESE_IN * ese_board) + (subindex - 1)) * MAX_IO_TYPE));
				}
				else
				{
					if ((!ese_initpara [row].subindex_start) ||
					(ese_initpara [row].subindex_start == ese_initpara [row].subindex_stop))
						help = 0;								/* parameter without subindex					*/
					else										/* parameter with subindex > 0					*/
						help = 1;
					switch (ese_initpara [row].size)
					{
						case (1):
							value = *((BYTE *)ese_initpara [row].pointer + (((MAX_ESE_IN * ese_board) + subindex - 1) * help));
							break;						/* read BYTE parameter							*/
						case (2):
							value = *((WORD *)ese_initpara [row].pointer + (((MAX_ESE_IN * ese_board) + subindex - 1) * help));
							break;						/* read WORD parameter							*/
						case (4):
							value = *((DWORD *)ese_initpara [row].pointer + (((MAX_ESE_IN * ese_board) + subindex - 1) * help));
							break;						/* read DWORD parameter							*/
					}
				}
				transmit_sdo_req_ese (ese_board + ESE_ID, INIT_WRITE_REQ | EXPEDITED_BIT, ese_initpara [row].index, subindex, value);
				ese_state = ESE_INIT_RES;
			}
			else											/* normal SDO transfer							*/
			{
				transmit_sdo_req_ese (ese_board + ESE_ID, INIT_WRITE_REQ, ese_initpara [row].index, subindex, ese_initpara [row].size);
				ese_state = ESE_INIT_SEG;
			}
			ese_timer = timer + 2 SEC;						/* wait max 2 s for response					*/
			break;

		case (ESE_INIT_SEG):								/* send init segment							*/
			if (ese_sdo_id == (ese_board + ESE_ID))			/* response from ESE							*/
			{
				ese_sdo_id = 0xFFFF;						/* reset marker for SDO received				*/
				transmit_sdo_seg_ese (ese_board + ESE_ID, WRITE_SEGM_REQ | LAST_SEGM_BIT,
				(BYTE *)ese_initpara [row].pointer + (((MAX_ESE_IN * ese_board) + (subindex - 1)) * MAX_IO_TYPE),
					ese_initpara [row].size);
				ese_timer = timer + 2 SEC;					/* wait max 2 s for response					*/
				ese_state = ESE_INIT_RES;
			}
			else if (ese_timer < timer)						/* time out										*/
				ese_state = ESE_INIT_START;				/* start init of next board						*/
			break;

		case (ESE_INIT_RES):								/* response from ESE							*/
			if (ese_sdo_id == (ese_board + ESE_ID))			/* response from ESE							*/
			{											/* response from ESE							*/
				if (subindex < ese_initpara [row].subindex_stop)
					subindex++;							/* go to next sub-index							*/
				else
				{
					row++;								/* go to next parameter							*/
					subindex = ese_initpara [row].subindex_start;	/* set to 1. sub-index of 1. parameter		*/
				}
				if (ese_initpara [row].size == 0xFF)				/* last parameter								*/
					ese_state = ESE_INIT_OP;				/* search next ESE board						*/
				else
					ese_state = ESE_INIT_REQ;				/* request next sub-index or next parameter		*/
			}
			else if (ese_timer < timer)						/* time out										*/
				ese_state = ESE_INIT_START;				/* start init of next board						*/
			break;

		case (ESE_INIT_OP):
			send_nmt_ese (START_NODE, ese_board + ESE_ID);			/* set node to operational state				*/
			bit_resetd (ese_init_error [ese_board / 32], ese_board % 32);
			bit_setd (ese_heartbeat [ese_board / 32], ese_board % 32);
			bit_setd (ese_state_request [ese_board / 32], ese_board % 32);
			ese_board++;									/* go to next ESE board							*/
			ese_state = ESE_INIT_START;					/* search next ESE board						*/
			break;

		case (ESE_STATE_START):
			while ((ese_board < MAX_ESE) &&				/* search 1. board for state request			*/
			(!(ese_state_request [ese_board / 32] & ((DWORD)1 << (ese_board % 32)))))
				ese_board++;
			if (ese_board < MAX_ESE)						/* next board for state request					*/
			{
				row = 0;									/* set to 1. parameter							*/
				ese_state = ESE_STATE_REQ;
			}
			else
			{
				row = 0;									/* set to 1. output								*/
				ese_state = ESE_SET_OUT;					/* send outputs to ESE boards					*/
				canerrorstate &= CANB_BUSOFF;				/* reset all CAN errors without bus off error	*/
			}
			break;

		case (ESE_STATE_REQ):
			while ((row < (MAX_ESE_IN + MAX_ESE_OUT)) &&	/* search next programmed input or output		*/
			(!request_ese_io (ese_board, row)))
				row++;
			if (row >= (MAX_ESE_IN + MAX_ESE_OUT))		/* no more things to request					*/
			{
				bit_resetd (ese_state_request [ese_board / 32], ese_board % 32);
				ese_board++;								/* go to next ESE board							*/
				ese_state = ESE_STATE_START;
			}
			else
			{
				ese_sdo_id = 0xFFFF;						/* reset marker for SDO received				*/
				if (row < MAX_ESE_IN)						/* request input								*/
					transmit_sdo_req_ese (ese_board + ESE_ID, INIT_READ_REQ, INPUT_GROUP, row + 1, 0);
				else										/* request output								*/
					transmit_sdo_req_ese (ese_board + ESE_ID, INIT_READ_REQ, OUTPUT_GROUP, row - MAX_ESE_IN + 1, 0);
				ese_state = ESE_STATE_SEG;
				ese_timer = timer + 2 SEC;					/* wait max 2 s for response					*/
			}
			break;

		case (ESE_STATE_SEG):
			if (ese_sdo_id == (ese_board + ESE_ID))		/* response from ESE							*/
			{
				ese_sdo_id = 0xFFFF;						/* reset marker for SDO received				*/
				transmit_sdo_seg_ese (ese_board + ESE_ID, READ_SEGM_REQ, 0, 0);
				ese_state = ESE_STATE_RES;
				ese_timer = timer + 2 SEC;				/* wait max 2 s for response					*/
			}
			else if (ese_timer < timer)					/* time out										*/
			{
				ese_board++;								/* go to next ESE board							*/
				ese_state = ESE_STATE_START;				/* start init of next board						*/
			}
			break;

		case (ESE_STATE_RES):
			if (ese_sdo_id == (ese_board + ESE_ID))		/* response from ESE							*/
			{
				if ((ese_sdo_data [0] & COMMAND_SPECIFIER) == READ_SEGM_RESP)
				{
					for (i = 0; i < MAX_IO_TYPE; i++)					/* write to virtual input mapping				*/
						virt_in [i] = ese_sdo_data [i + 1];
					if(row < MAX_ESE_IN)
						{
							if(p.ese_inpar[ese_board][row][IO_ENABLE] == ENABLE)
								{
									if (((ese_sdo_data [IO_BASIC_FUNC + 1] == HALL_CALL_SPECIAL) ||
											(ese_sdo_data [IO_BASIC_FUNC + 1] == HALL_CALL_ADVANCED) ||
											(ese_sdo_data [IO_BASIC_FUNC + 1] == HALL_CALL_EMERGENCY)))
												/* priority call input							*/
										handle_input (LIFT1, 1);	/* handle input									*/
									else
										handle_input (LIFT1, 0);	/* handle input 								*/
								}
						}
					else if(p.ese_outpar[ese_board][row - MAX_ESE_IN][IO_ENABLE] == ENABLE)
						{							
							handle_input (LIFT1, 0);	/* handle input 								*/
						}
				}
				row++;										/* go to next parameter							*/
				ese_state = ESE_STATE_REQ;					/* request next input or output					*/
			}
			else if (ese_timer < timer)					/* time out										*/
			{
				ese_board++;								/* go to next ESE board							*/
				ese_state = ESE_STATE_START;				/* start init of next board						*/
			}
			break;

		case (ESE_SET_OUT):
			if (outstate [row].function != ENDSTRING)
			{
				set_out_state (row, O_CANB);
				row++;
			}
			else
				ese_state = ESE_WATCH;						/* go back to normal state						*/
			break;
	}
}

