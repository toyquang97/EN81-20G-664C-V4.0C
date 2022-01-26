
#include "typedef.h"
#include "lpc1700.h"
#include "parameter.h"
#include "state.h"
#include "can.h"
#include "text.h"
#include "history.h"

/****************************************************************************************************/
/* table with all parameters for initialization of TSE boards										*/
/****************************************************************************************************/
const struct _initpara exe_initpara [] = {
	{&exe_para.exe_inpar [0][0][0],	5,		INPUT_GROUP,	1,	MAX_EXE_IN,		1	},
	{&exe_para.exe_outpar [0][0][0],	5,		OUTPUT_GROUP,	1,	MAX_EXE_OUT,	1	},
	{&exe_para.exe_inpar [0][0][5],	1,		INPUT_PARA3,	1,	MAX_EXE_IN,		1	},
	{&exe_para.exe_outpar [0][0][5],	1,		OUTPUT_PARA3,	1,	MAX_EXE_OUT,	1	},
	{&exe_para.exe_inpar [0][0][6],	1,		INPUT_PARA4,	1,		MAX_EXE_IN, 		1 },
	{&exe_para.exe_outpar [0][0][6], 1,		OUTPUT_PARA4, 1,		MAX_EXE_OUT,		1 },
	{(void *)1,					1,		LIFT_NUMBER,	0,	0,				1	},
	{0,							0xFF,	0,				0,	0,				0	},
};

/****************************************************************************************************/
/* decide if input or output should be requested													*/
/****************************************************************************************************/
BYTE request_exe_io (BYTE exe_number, BYTE io_number){
	BYTE function;

	if (io_number < MAX_EXE_IN)
		function = exe_para.exe_inpar [exe_number][io_number][IO_BASIC_FUNC];
	else
		function = exe_para.exe_outpar [exe_number][io_number - MAX_EXE_IN][IO_BASIC_FUNC];
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
			if (io_number < MAX_EXE_IN)
				return (0);
			else
				return (1);

		default:
			if (io_number < MAX_EXE_IN)
				return (1);
			else
				return (0);
	}
}

/****************************************************************************************************/
/* handle TSE boards																				*/
/****************************************************************************************************/
void exe (void){
	static DWORD exe_timer;
	static BYTE subindex;
	static BYTE exe_board;
	static BYTE i;
	static DWORD value;
	BYTE help;
	BYTE j;

	if (exe_state == EXE_WATCH)
		exe_sdo_master = 0;
	else
		exe_sdo_master = 1;
	switch (exe_state)
	{
		case (EXE_WATCH):
			if (exe_init_error)							/* 1 or more TSE not init						*/
				{
					exe_state = EXE_INIT_START;			/* goto init state								*/
					exe_board = 0;							/* reset TSE board number					*/
				}
			else if (exe_state_request)					/* TSE state request necessary					*/
				{
					exe_state = EXE_STATE_START;			/* goto state search							*/
					exe_board = 0;							/* reset TSE board number					*/
				}
			break;

		case (EXE_INIT_START):
			while ((exe_board < MAX_EXE) &&				/* search 1. board for initialization				*/
						(!(exe_init_error & (1 << exe_board))))
				exe_board++;
			if (exe_board < MAX_EXE)					/* next board for init							*/
				{
					i = 0 ;								/* set to 1. parameter							*/
					subindex = exe_initpara [i].subindex_start;	/* set to 1. sub-index of 1. parameter			*/
					exe_state = EXE_INIT_REQ;
				}
			else
				{
					exe_state = EXE_STATE_START;			/* goto state search							*/
					exe_board = 0;							/* reset TSE board number					*/
				}
			break;

		case (EXE_INIT_REQ):
			exe_sdo_id = 0xFFFF;						/* reset marker for SDO received				*/
			if (exe_initpara [i].size <= 4)					/* expedited SDO transfer						*/
				{
					if ((exe_initpara [i].index == OUTPUT_PARA3)
							|| (exe_initpara [i].index == INPUT_PARA3))
						{/* only for input/output polarity parameter		*/
							value = *((BYTE *)exe_initpara [i].pointer + (((MAX_EXE_IN * exe_board) + (subindex - 1)) * MAX_IO_TYPE));
							value <<= 8;
						}
					
					else if ((exe_initpara [i].index == OUTPUT_PARA4)
									|| (exe_initpara [i].index == INPUT_PARA4))
						{
							value = *((BYTE *)exe_initpara [i].pointer + (((MAX_EXE_IN * exe_board) + (subindex - 1)) * MAX_IO_TYPE));
						}
					else if (exe_initpara [i].index == LIFT_NUMBER)
						value = 1;							/* lift number always 1 on control bus			*/
					else
						{
							if ((!exe_initpara [i].subindex_start) ||
									(exe_initpara [i].subindex_start == exe_initpara [i].subindex_stop))
								help = 0;					/* parameter without subindex					*/
							else								/* parameter with subindex > 0				*/
								help = 1;
							switch (exe_initpara [i].size)
								{
									case (1):
										value = *((BYTE *)exe_initpara [i].pointer + (((MAX_EXE_IN * exe_board) + subindex - 1) * help));
										break;					/* read BYTE parameter						*/

									case (2):
										value = *((WORD *)exe_initpara [i].pointer + (((MAX_EXE_IN * exe_board) + subindex - 1) * help));
										break;						/* read WORD parameter							*/

									case (4):
										value = *((DWORD *)exe_initpara [i].pointer + (((MAX_EXE_IN * exe_board) + subindex - 1) * help));
										break;						/* read DWORD parameter							*/
								}
						}
					if(!exe_board)
						transmit_sdo_req (exe_board + EXE_ID, INIT_WRITE_REQ | EXPEDITED_BIT, exe_initpara [i].index, subindex, value);
					else
						transmit_sdo_req_ese (exe_board + EXE_ID, INIT_WRITE_REQ | EXPEDITED_BIT, exe_initpara [i].index, subindex, value);
					exe_state = EXE_INIT_RES;
				}
			else											/* normal SDO transfer							*/
				{
					if(!exe_board)
						transmit_sdo_req (exe_board + EXE_ID, INIT_WRITE_REQ, exe_initpara [i].index, subindex, exe_initpara [i].size);
					else
						transmit_sdo_req_ese (exe_board + EXE_ID, INIT_WRITE_REQ, exe_initpara [i].index, subindex, exe_initpara [i].size);
					exe_state = EXE_INIT_SEG;
				}
			exe_timer = timer + 20;							/* wait max 2 s for response				*/
			break;

		case (EXE_INIT_SEG):								/* send init segment						*/
			if (exe_sdo_id == (exe_board + EXE_ID))			/* response from TSE						*/
				{
					exe_sdo_id = 0xFFFF;						/* reset marker for SDO received			*/
					if(!exe_board)
						transmit_sdo_seg (exe_board + EXE_ID, WRITE_SEGM_REQ | LAST_SEGM_BIT, (BYTE *)exe_initpara [i].pointer + (((MAX_EXE_IN * exe_board) + (subindex - 1)) * MAX_IO_TYPE),
															exe_initpara [i].size);
					else
						transmit_sdo_seg_ese (exe_board + EXE_ID, WRITE_SEGM_REQ | LAST_SEGM_BIT, (BYTE *)exe_initpara [i].pointer + (((MAX_EXE_IN * exe_board) + (subindex - 1)) * MAX_IO_TYPE),
														exe_initpara [i].size);
					exe_timer = timer + 20;						/* wait max 2 s for response				*/
					exe_state = EXE_INIT_RES;
				}
			else if (exe_timer < timer)						/* time out								*/
				exe_state = EXE_INIT_START;				/* start init of next board					*/
			break;

		case (EXE_INIT_RES):								/* response from TSE						*/
			if (exe_sdo_id == (exe_board + EXE_ID))			/* response from TSE						*/
				{
					if (subindex < exe_initpara [i].subindex_stop)
						subindex++;							/* go to next sub-index					*/
					else
						{
							i++;									/* go to next parameter					*/
							subindex = exe_initpara [i].subindex_start;	/* set to 1. sub-index of 1. parameter		*/
						}
					if (exe_initpara [i].size == 0xFF)				/* last parameter							*/
						exe_state = EXE_INIT_OP;				/* search next TSE board					*/
					else
						exe_state = EXE_INIT_REQ;				/* request next sub-index or next parameter	*/
				}
			else if (exe_timer < timer)						/* time out								*/
				exe_state = EXE_INIT_START;				/* start init of next board					*/
			break;

		case (EXE_INIT_OP):			
			if(!exe_board)
				send_nmt (START_NODE, EXE_ID + exe_board);		/* set node to operational state				*/
			else
				send_nmt_ese (START_NODE, EXE_ID + exe_board);		/* set node to operational state				*/
			bit_reset (exe_init_error, exe_board);				/* reset init error bit for this TSE			*/
			bit_set (exe_heartbeat, exe_board);
			bit_set (exe_state_request, exe_board);
			exe_board++;									/* go to next TSE board					*/
			exe_state = EXE_INIT_START;					/* search next TSE board					*/
			break;

		case (EXE_STATE_START):
			while ((exe_board < MAX_EXE) &&					/* search 1. board for initialization			*/
						(!(exe_state_request & (1 << exe_board))))
				exe_board++;
			if (exe_board < MAX_EXE)						/* next board for init						*/
				{
					i = 0 ;									/* set to 1. parameter						*/
					exe_state = EXE_STATE_REQ;
				}
			else
				{
					i = 0 ;									/* set to 1. parameter						*/
					exe_state = EXE_SET_OUT;					/* send outputs to TSE boards				*/
				}
			break;

		case (EXE_STATE_REQ):
			while ((i < (MAX_EXE_IN + MAX_EXE_OUT)) &&		/* search next programmed input or output	*/
						(!request_exe_io (exe_board, i)))
				i++;
			if (i >= (MAX_EXE_IN + MAX_EXE_OUT))			/* no more input to request				*/
			{
				bit_reset (exe_state_request, exe_board);		/* state request ready						*/
				exe_board++;								/* go to next TSE board					*/
				exe_state = EXE_STATE_START;
			}
			else
			{
				exe_sdo_id = 0xFFFF;						/* reset marker for SDO received			*/
				if(!exe_board)
					{
						if (i < MAX_EXE_IN)							/* request input							*/
							transmit_sdo_req (exe_board + EXE_ID, INIT_READ_REQ, INPUT_GROUP, i + 1, 0);
						else										/* request output								*/
							transmit_sdo_req (exe_board + EXE_ID, INIT_READ_REQ, OUTPUT_GROUP, i - MAX_EXE_IN + 1, 0);
					}
				else					
					{
						if (i < MAX_EXE_IN) 						/* request input							*/
							transmit_sdo_req_ese (exe_board + EXE_ID, INIT_READ_REQ, INPUT_GROUP, i + 1, 0);
						else										/* request output 							*/
							transmit_sdo_req_ese (exe_board + EXE_ID, INIT_READ_REQ, OUTPUT_GROUP, i - MAX_EXE_IN + 1, 0);
					}
				exe_state = EXE_STATE_SEG;
				exe_timer = timer + 20;						/* wait max 2 s for response				*/
			}
			break;

		case (EXE_STATE_SEG):
			if (exe_sdo_id == (exe_board + EXE_ID))			/* response from TSE						*/
				{												
					exe_sdo_id = 0xFFFF;						/* reset marker for SDO received			*/
					if(!exe_board)
						transmit_sdo_seg (exe_board + EXE_ID, READ_SEGM_REQ, 0, 0);
					else						
						transmit_sdo_seg_ese (exe_board + EXE_ID, READ_SEGM_REQ, 0, 0);
					exe_state = EXE_STATE_RES;
					exe_timer = timer + 20;						/* wait max 2 s for response				*/
				}
			else if (exe_timer < timer)						/* time out								*/
				{
					exe_board++;								/* go to next TSE board					*/
					exe_state = EXE_STATE_START;				/* start init of next board					*/
				}
			break;

		case (EXE_STATE_RES):
			if (exe_sdo_id == (exe_board + EXE_ID))			/* response from TSE						*/
			{
				if ((exe_sdo_data [0] & COMMAND_SPECIFIER) == READ_SEGM_RESP)
				{
					for (j = 0; j < MAX_IO_TYPE; j++)						/* write to virtual input mapping			*/
						virt_in [j] = exe_sdo_data [j + 1];
					if(i < MAX_EXE_IN)
						{
							if(exe_para.exe_inpar[exe_board][i][IO_ENABLE] == ENABLE)
								{
									if (((exe_sdo_data [IO_BASIC_FUNC + 1] == HALL_CALL_SPECIAL)	||
											(exe_sdo_data [IO_BASIC_FUNC + 1] == HALL_CALL_ADVANCED) ||
											(exe_sdo_data [IO_BASIC_FUNC + 1] == HALL_CALL_EMERGENCY))) // &&
															/* priority call input						*/
										handle_input (LIFT1, 1);				/* handle input							*/
									else
										handle_input (LIFT1, 0);				/* handle input							*/
								}
						}					
					else if(exe_para.exe_outpar[exe_board][i - MAX_EXE_IN][IO_ENABLE] == ENABLE)
						{							
							handle_input (LIFT1, 0);	/* handle input 								*/
						}
				}
				i++;										/* go to next parameter					*/
				exe_state = EXE_STATE_REQ;				/* request next input or output				*/
			}
			else if (exe_timer < timer)						/* time out								*/
			{
				exe_board++;								/* go to next TSE board					*/
				exe_state = EXE_STATE_START;				/* start init of next board					*/
			}
			break;

		case (EXE_SET_OUT):
			if (outstate [i].function != ENDSTRING)
			{
				set_out_state (i, O_CANA | O_CANB);
				i++;
			}
			else
				exe_state = EXE_WATCH;					/* go back to normal state					*/
			break;
	}
}


