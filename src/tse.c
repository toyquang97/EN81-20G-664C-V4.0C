
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
const struct _initpara tse_initpara [] =
{
	{&p.tse_inpar [0][0][0],	5,		INPUT_GROUP,	1,	MAX_TSE_IN,		1	},
	{&p.tse_outpar [0][0][0],	5,		OUTPUT_GROUP,	1,	MAX_TSE_OUT,	1	},
	{&p.tse_inpar [0][0][5],	1,		INPUT_PARA3,	1,	MAX_TSE_IN,		1	},
	{&p.tse_outpar [0][0][5],	1,		OUTPUT_PARA3,	1,	MAX_TSE_OUT,	1	},
	{&p.tse_inpar [0][0][6],	1,		INPUT_PARA4,	1,		MAX_TSE_IN, 		1 },
	{&p.tse_outpar [0][0][6], 1,		OUTPUT_PARA4, 1,		MAX_TSE_OUT,		1 },
	{(void *)1,					1,		LIFT_NUMBER,	0,	0,				1	},
	{&p.tse_extnums,			1,		IOU_EXT_NUMS,	0,	0,				1	},
	{&p.arrowtype,				1,		ARROW_MODE,		0,	0,				1	},
	{0,							0xFF,	0,				0,	0,				0	},
};

/****************************************************************************************************/
/* decide if input or output should be requested													*/
/****************************************************************************************************/
BYTE request_tse_io (BYTE tse_number, BYTE io_number)
{
	BYTE function;

	if (io_number < MAX_TSE_IN)
		function = p.tse_inpar [tse_number][io_number][IO_BASIC_FUNC];
	else
		function = p.tse_outpar [tse_number][io_number - MAX_TSE_IN][IO_BASIC_FUNC];
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
			if (io_number < MAX_TSE_IN)
				return (0);
			else
				return (1);

		default:
			if (io_number < MAX_TSE_IN)
				return (1);
			else
				return (0);
	}
}

/****************************************************************************************************/
/* handle TSE boards																				*/
/****************************************************************************************************/
void tse (void)
{
	static DWORD tse_timer;
	static BYTE subindex;
	static BYTE tse_board;
	static BYTE i;
	static DWORD value;
	BYTE help;
	BYTE j;

	if (oldtse_arrowtype != p.arrowtype || oldtse_extnums != p.tse_extnums)
	{
		oldtse_arrowtype = p.arrowtype;
		oldtse_extnums = p.tse_extnums;
		tse_init_error = tse_enabled;
	}

	if (tse_state == TSE_WATCH)
		tse_sdo_master = 0;
	else
		tse_sdo_master = 1;
	switch (tse_state)
	{
		case (TSE_WATCH):
			if (tse_init_error)							/* 1 or more TSE not init						*/
			{
				tse_state = TSE_INIT_START;			/* goto init state								*/
				tse_board = 0;							/* reset TSE board number					*/
			}
			else if (tse_state_request)					/* TSE state request necessary					*/
			{
				tse_state = TSE_STATE_START;			/* goto state search							*/
				tse_board = 0;							/* reset TSE board number					*/
			}
			break;

		case (TSE_INIT_START):
			while ((tse_board < MAX_TSE) &&				/* search 1. board for initialization				*/
			(!(tse_init_error & (1 << tse_board))))
				tse_board++;
			if (tse_board < MAX_TSE)					/* next board for init							*/
			{
				i = 0 ;								/* set to 1. parameter							*/
				subindex = tse_initpara [i].subindex_start;	/* set to 1. sub-index of 1. parameter			*/
				tse_state = TSE_INIT_REQ;
			}
			else
			{
				tse_state = TSE_STATE_START;			/* goto state search							*/
				tse_board = 0;							/* reset TSE board number					*/
			}
			break;

		case (TSE_INIT_REQ):
			tse_sdo_id = 0xFFFF;						/* reset marker for SDO received				*/
			if (tse_initpara [i].size <= 4)					/* expedited SDO transfer						*/
			{
				if ((tse_initpara [i].index == OUTPUT_PARA3)
				|| (tse_initpara [i].index == INPUT_PARA3))
				{									/* only for input/output polarity parameter		*/
					value = *((BYTE *)tse_initpara [i].pointer + (((MAX_TSE_IN * tse_board) + (subindex - 1)) * MAX_IO_TYPE));
					value <<= 8;
				}
				
				else if ((tse_initpara [i].index == OUTPUT_PARA4)
								|| (tse_initpara [i].index == INPUT_PARA4))
					{
						value = *((BYTE *)tse_initpara [i].pointer + (((MAX_TSE_IN * tse_board) + (subindex - 1)) * MAX_IO_TYPE));
					}
				else if (tse_initpara [i].index == LIFT_NUMBER)
					value = 1;							/* lift number always 1 on control bus			*/
				else
				{
					if ((!tse_initpara [i].subindex_start) ||
					(tse_initpara [i].subindex_start == tse_initpara [i].subindex_stop))
						help = 0;						/* parameter without subindex					*/
					else								/* parameter with subindex > 0				*/
						help = 1;
					switch (tse_initpara [i].size)
					{
						case (1):
							value = *((BYTE *)tse_initpara [i].pointer + (((MAX_TSE_IN * tse_board) + subindex - 1) * help));
							break;					/* read BYTE parameter						*/

						case (2):
							value = *((WORD *)tse_initpara [i].pointer + (((MAX_TSE_IN * tse_board) + subindex - 1) * help));
							break;						/* read WORD parameter							*/

						case (4):
							value = *((DWORD *)tse_initpara [i].pointer + (((MAX_TSE_IN * tse_board) + subindex - 1) * help));
							break;						/* read DWORD parameter							*/
					}
				}
				transmit_sdo_req (tse_board + TSE_ID, INIT_WRITE_REQ | EXPEDITED_BIT, tse_initpara [i].index, subindex, value);
				tse_state = TSE_INIT_RES;
			}
			else											/* normal SDO transfer							*/
			{
				transmit_sdo_req (tse_board + TSE_ID, INIT_WRITE_REQ, tse_initpara [i].index, subindex, tse_initpara [i].size);
				tse_state = TSE_INIT_SEG;
			}
			tse_timer = timer + 20;							/* wait max 2 s for response				*/
			break;

		case (TSE_INIT_SEG):								/* send init segment						*/
			if (tse_sdo_id == (tse_board + TSE_ID))			/* response from TSE						*/
			{
				tse_sdo_id = 0xFFFF;						/* reset marker for SDO received			*/
				transmit_sdo_seg (tse_board + TSE_ID, WRITE_SEGM_REQ | LAST_SEGM_BIT, (BYTE *)tse_initpara [i].pointer + (((MAX_TSE_IN * tse_board) + (subindex - 1)) * MAX_IO_TYPE),
				tse_initpara [i].size);
				tse_timer = timer + 20;						/* wait max 2 s for response				*/
				tse_state = TSE_INIT_RES;
			}
			else if (tse_timer < timer)						/* time out								*/
				tse_state = TSE_INIT_START;				/* start init of next board					*/
			break;

		case (TSE_INIT_RES):								/* response from TSE						*/
			if (tse_sdo_id == (tse_board + TSE_ID))			/* response from TSE						*/
			{
				if (subindex < tse_initpara [i].subindex_stop)
					subindex++;							/* go to next sub-index					*/
				else
				{
					i++;									/* go to next parameter					*/
					subindex = tse_initpara [i].subindex_start;	/* set to 1. sub-index of 1. parameter		*/
				}
				if (tse_initpara [i].size == 0xFF)				/* last parameter							*/
					tse_state = TSE_INIT_OP;				/* search next TSE board					*/
				else
					tse_state = TSE_INIT_REQ;				/* request next sub-index or next parameter	*/
			}
			else if (tse_timer < timer)						/* time out								*/
				tse_state = TSE_INIT_START;				/* start init of next board					*/
			break;

		case (TSE_INIT_OP):
			send_nmt (START_NODE, TSE_ID + tse_board);		/* set node to operational state				*/
			bit_reset (tse_init_error, tse_board);				/* reset init error bit for this TSE			*/
			bit_set (tse_heartbeat, tse_board);
			bit_set (tse_state_request, tse_board);
			tse_board++;									/* go to next TSE board					*/
			tse_state = TSE_INIT_START;					/* search next TSE board					*/
			break;

		case (TSE_STATE_START):
			while ((tse_board < MAX_TSE) &&					/* search 1. board for initialization			*/
			(!(tse_state_request & (1 << tse_board))))
				tse_board++;
			if (tse_board < MAX_TSE)						/* next board for init						*/
			{
				i = 0 ;									/* set to 1. parameter						*/
				tse_state = TSE_STATE_REQ;
			}
			else
			{
				i = 0 ;									/* set to 1. parameter						*/
				tse_state = TSE_SET_OUT;					/* send outputs to TSE boards				*/
			}
			break;

		case (TSE_STATE_REQ):
			while ((i < (MAX_TSE_IN + MAX_TSE_OUT)) &&		/* search next programmed input or output	*/
			(!request_tse_io (tse_board, i)))
				i++;
			if (i >= (MAX_TSE_IN + MAX_TSE_OUT))			/* no more input to request				*/
			{
				bit_reset (tse_state_request, tse_board);		/* state request ready						*/
				tse_board++;								/* go to next TSE board					*/
				tse_state = TSE_STATE_START;
			}
			else
			{
				tse_sdo_id = 0xFFFF;						/* reset marker for SDO received			*/
				if (i < MAX_TSE_IN)							/* request input							*/
					transmit_sdo_req (tse_board + TSE_ID, INIT_READ_REQ, INPUT_GROUP, i + 1, 0);
				else										/* request output								*/
					transmit_sdo_req (tse_board + TSE_ID, INIT_READ_REQ, OUTPUT_GROUP, i - MAX_TSE_IN + 1, 0);
				tse_state = TSE_STATE_SEG;
				tse_timer = timer + 20;						/* wait max 2 s for response				*/
			}
			break;

		case (TSE_STATE_SEG):
			if (tse_sdo_id == (tse_board + TSE_ID))			/* response from TSE						*/
			{												
				tse_sdo_id = 0xFFFF;						/* reset marker for SDO received			*/
				transmit_sdo_seg (tse_board + TSE_ID, READ_SEGM_REQ, 0, 0);
				tse_state = TSE_STATE_RES;
				tse_timer = timer + 20;						/* wait max 2 s for response				*/
			}
			else if (tse_timer < timer)						/* time out								*/
			{
				tse_board++;								/* go to next TSE board					*/
				tse_state = TSE_STATE_START;				/* start init of next board					*/
			}
			break;

		case (TSE_STATE_RES):
			if (tse_sdo_id == (tse_board + TSE_ID))			/* response from TSE						*/
			{
				if ((tse_sdo_data [0] & COMMAND_SPECIFIER) == READ_SEGM_RESP)
				{
					for (j = 0; j < MAX_IO_TYPE; j++)						/* write to virtual input mapping			*/
						virt_in [j] = tse_sdo_data [j + 1];
					if(i < MAX_TSE_IN)
						{
							if(p.tse_inpar[tse_board][i][IO_ENABLE] == ENABLE)
								{
									if (((tse_sdo_data [IO_BASIC_FUNC + 1] == HALL_CALL_SPECIAL)	||
											(tse_sdo_data [IO_BASIC_FUNC + 1] == HALL_CALL_ADVANCED) ||
											(tse_sdo_data [IO_BASIC_FUNC + 1] == HALL_CALL_EMERGENCY))) // &&
															/* priority call input						*/
										handle_input (LIFT1, 1);				/* handle input							*/
									else
										handle_input (LIFT1, 0);				/* handle input							*/
								}
						}					
					else if(p.tse_outpar[tse_board][i - MAX_TSE_IN][IO_ENABLE] == ENABLE)
						{							
							handle_input (LIFT1, 0);	/* handle input 								*/
						}
				}
				i++;										/* go to next parameter					*/
				tse_state = TSE_STATE_REQ;				/* request next input or output				*/
			}
			else if (tse_timer < timer)						/* time out								*/
			{
				tse_board++;								/* go to next TSE board					*/
				tse_state = TSE_STATE_START;				/* start init of next board					*/
			}
			break;

		case (TSE_SET_OUT):
			if (outstate [i].function != ENDSTRING)
			{
				set_out_state (i, O_CANA);
				i++;
			}
			else
				tse_state = TSE_WATCH;					/* go back to normal state					*/
			break;
	}
}

