
#include "typedef.h"
#include "parameter.h"
#include "state.h"
#include "can.h"
#include "text.h"
#include "nice5000.h"

BYTE tmp_id;

/****************************************************************************************************/
/* table with all parameters for the nodes													*/
/* parameter pointer,			parameter size,						index,				1. sub-index,		last sub-index,		PCB		*/
/****************************************************************************************************/
const struct _initpara initpara [] =
{
	{&p.doornumber,				sizeof (p.doornumber),				DOORNUMBER,			0,					0,					B_FVE	},
	{&p.loadtype,				sizeof (p.loadtype),				LOADTYPE,			0,					0,					B_FVE	},
	{&p.lift_func_sel1,		sizeof (p.lift_func_sel1), 	LIFT_FUNCTION_SEL1,	0,					0,					B_FVE },
	{&p.adlimit [0],			sizeof (p.adlimit [0]),				LOAD_SIG_LIMIT, 	1,					C_LOADTYPE_CNT,		B_FVE	},

	{&p.lstime [0],				sizeof (p.lstime [0]),				DOOR1_PARA,			LS_RECLOSE_TIME,	LS_RECLOSE_TIME,	B_FVE	},
	{&p.lstime [1],				sizeof (p.lstime [1]),				DOOR2_PARA,			LS_RECLOSE_TIME,	LS_RECLOSE_TIME,	B_FVE	},
	{&p.revtime [0],			sizeof (p.revtime [0]),				DOOR1_PARA,			REV_RECLOSE_TIME,	REV_RECLOSE_TIME,	B_FVE	},
	{&p.revtime [1],			sizeof (p.revtime [1]),				DOOR2_PARA,			REV_RECLOSE_TIME,	REV_RECLOSE_TIME,	B_FVE	},
	{&p.nudgingtime [0],		sizeof (p.nudgingtime [0]),			DOOR1_PARA,			LS_BROKEN_TIME,		LS_BROKEN_TIME,		B_FVE	},
	{&p.nudgingtime [1],		sizeof (p.nudgingtime [1]),			DOOR2_PARA,			LS_BROKEN_TIME,		LS_BROKEN_TIME,		B_FVE	},
	{&p.rev_door [0],			sizeof (p.rev_door [0]),			REVDOOR,			1,					MAX_DOOR,			B_FVE	},
	{&p.dooropentime [0],		sizeof (p.dooropentime [0]),		DOOROPENTIME,		1,					MAX_DOOR,			B_FVE	},
	{&p.doorclosetime [0],		sizeof (p.doorclosetime [0]),		DOORCLOSETIME,		1,					MAX_DOOR,			B_FVE	},
	{&p.doorswitchtime [0],		sizeof (p.doorswitchtime [0]),		DOORSWITCHTIME,		1,					MAX_DOOR,			B_FVE	},
	{&p.doordelaytime [0],		sizeof (p.doordelaytime [0]),		DOORDELAYTIME,		1,					MAX_DOOR,			B_FVE	},
	{&p.doorrelaymode [0],		sizeof (p.doorrelaymode [0]),		DOORRELAYMODE,		1,					MAX_DOOR,			B_FVE	},
	{&p.doorrelaytime_cl [0],	sizeof (p.doorrelaytime_cl [0]),	DOORRELAYTIME_CL,	1,					MAX_DOOR,			B_FVE	},
	{&p.doorrelaytime_op[0],	sizeof (p.doorrelaytime_op [0]),	DOORRELAYTIME_OP,	1,					MAX_DOOR,			B_FVE	},
	{&p.max_ls_on [0],			sizeof (p.max_ls_on [0]),			MAX_LS_ON_TIME,		1,					MAX_DOOR,			B_FVE	},
	{&p.rampmode [0],			sizeof (p.rampmode [0]),			RAMPMODE,			1,					MAX_DOOR,			B_FVE	},
	{&p.ramptiming [0],			sizeof (p.ramptiming [0]),			RAMPTIMING,			1,					MAX_DOOR,			B_FVE	},
	{&p.rampdelay [0],			sizeof (p.rampdelay [0]),			RAMPDELAY,			1,					MAX_DOOR,			B_FVE	},
	{&p.dooropendelay [0],		sizeof (p.dooropendelay [0]),		DOOROPENDELAY,		1,					MAX_DOOR,			B_FVE	},

	{&p.fve_inpar [0][0],		5,									INPUT_GROUP,		1,					MAX_FVE_IN,			B_FVE	},
	{&p.fve_outpar [0][0],		5,									OUTPUT_GROUP,		1,					MAX_FVE_OUT,		B_FVE	},
	{&p.fve_inpar [0][5],		1,									INPUT_PARA3,		1,					MAX_FVE_IN,			B_FVE	},
	{&p.fve_outpar [0][5],		1,									OUTPUT_PARA3,		1,					MAX_FVE_OUT,		B_FVE	},
	{&p.fve_inpar [0][6],		1,									INPUT_PARA4,		1,					MAX_FVE_IN, 		B_FVE	},
	{&p.fve_outpar [0][6], 		1,									OUTPUT_PARA4, 		1,					MAX_FVE_OUT,		B_FVE	},

	{&p.fve_ex_inpar [0][0],	5,									INPUT_GROUP,		MAX_FVE_IN+1,		MAX_FVE_IN+MAX_FVE_EX_IN,		B_FVE	},
	{&p.fve_ex_inpar [0][5],	1,									INPUT_PARA3,		MAX_FVE_IN+1,		MAX_FVE_IN+MAX_FVE_EX_IN,		B_FVE	},
	{&p.fve_ex_inpar [0][6],	1,									INPUT_PARA4,		MAX_FVE_IN+1,		MAX_FVE_IN+MAX_FVE_EX_IN,		B_FVE	},

	{0,							0xFF,								0,					0,					0,					0		},
};

/****************************************************************************************************/
/* init node																						*/
/****************************************************************************************************/
void init_node (BYTE node)
{
	DWORD value = 0;
	WORD i = 0;
	WORD index;
	BYTE subindex;
	BYTE help;
	BYTE node_id;
	BYTE node_bit;

	node_bit = node;
	switch (node)
	{
		case (B_FVE):									/* init message	for FVE - SDO		*/
			node_id = FVE_ID;
			break;
		default:
			return;
	}
	while (initpara [i].size != 0xFF)						/* search parameter table				*/
	{
		if (initpara [i].bg & node)							/* parameter for this node				*/
		{
			if ((!initpara [i].subindex_start) ||				/* parameter without subindex			*/
			(initpara [i].subindex_start == initpara [i].subindex_stop))
				help = 0;
			else										/* parameter with subindex > 0		*/
				help = 1;
			index = initpara [i].index;

			if ((node_id != FVE_ID) ||
				(fve_hardware_ver_after_v13) ||
				((index != INPUT_GROUP) &&
					(index != OUTPUT_GROUP) &&
					(index != INPUT_PARA3) &&
					(index != OUTPUT_PARA3) &&
					(index != INPUT_PARA4) &&
					(index != OUTPUT_PARA4)) ||
				((index == INPUT_GROUP || index == INPUT_PARA3 || index == INPUT_PARA4) && (initpara[i].subindex_stop <= MAX_FVE_IN)) ||
				((index == OUTPUT_GROUP || index == OUTPUT_PARA3 || index == OUTPUT_PARA4) && (initpara[i].subindex_stop <= MAX_FVE_OUT)))
			{
				if (initpara [i].size <= 4)						/* expedited SDO transfer				*/
				{
					for (subindex = initpara [i].subindex_start; subindex <= initpara [i].subindex_stop; subindex++)
					{
						if ((initpara [i].index == OUTPUT_PARA3)	/* only for input/output polarity parameter	*/
						|| (initpara [i].index == INPUT_PARA3))
						{
							value = *((BYTE *)initpara [i].pointer + ((subindex - initpara[i].subindex_start) * help * MAX_IO_TYPE));
							value <<= 8;
						}
						else if ((initpara [i].index == OUTPUT_PARA4) || (initpara [i].index == INPUT_PARA4))
						{
							value = *((BYTE *)initpara [i].pointer + ((subindex - initpara[i].subindex_start) * help * MAX_IO_TYPE));
						}
						else
						{
							switch (initpara [i].size)
							{
								case (1):
									value = *((BYTE *)initpara [i].pointer + ((subindex - initpara[i].subindex_start) * help));
									break;				/* read BYTE parameter				*/

								case (2):
									value = *((WORD *)initpara [i].pointer + ((subindex - initpara[i].subindex_start) * help));
									break;				/* read WORD parameter				*/

								case (4):
									value = *((DWORD *)initpara [i].pointer + ((subindex - initpara[i].subindex_start) * help));
									break;				/* read DWORD parameter				*/
							}
						}
						sdo_id = 0xFFFF;					/* reset marker for SDO received		*/
						transmit_sdo_req (node_id, INIT_WRITE_REQ | EXPEDITED_BIT, initpara [i].index, subindex, value);
						ct = timer + 2 SEC;					/* wait 2 seconds for SDO from node	*/
						while ((ct > timer) && (sdo_id == 0xFFFF))
							cycle ();						/* do all cyclic functions				*/
						if (sdo_id == 0xFFFF)
							return;						/* no message received				*/
					}
				}
				else										/* normal SDO transfer				*/
				{
					for (subindex = initpara [i].subindex_start; subindex <= initpara [i].subindex_stop; subindex++)
					{
						sdo_id = 0xFFFF;					/* reset marker for SDO received		*/
						transmit_sdo_req (node_id, INIT_WRITE_REQ, initpara [i].index, subindex, initpara [i].size);
						ct = timer + 20;					/* wait 2 seconds for SDO from node	*/
						while ((ct > timer) && (sdo_id == 0xFFFF))
							cycle ();						/* do all cyclic functions				*/
						if (sdo_id == 0xFFFF)
							return;						/* no message received				*/
						sdo_id = 0xFFFF;					/* reset marker for SDO received		*/
						transmit_sdo_seg (node_id, WRITE_SEGM_REQ | LAST_SEGM_BIT, (BYTE *)initpara [i].pointer + ((subindex - initpara[i].subindex_start) * help * MAX_IO_TYPE) , initpara [i].size);
						ct = timer + 2 SEC;					/* wait 2 seconds for SDO from node	*/
						while ((ct > timer) && (sdo_id == 0xFFFF))
							cycle ();						/* do all cyclic functions				*/
						if (sdo_id == 0xFFFF)
							return;						/* no message received				*/
					}
				}
			}
		}
		i++;
	}
	send_nmt (START_NODE, node_id);					/* set node to operational state			*/
	start_init |=  node_bit;							/* set "HSE new start init" bit					*/
	init_error &= ~node_bit;							/* clear "not init" bit							*/
}

/****************************************************************************************************/
/* init all boards																					*/
/****************************************************************************************************/
void ini_state (void)
{
	DWORD statetimer;

	sdo_master = 1;
	if (sdo_interrupt)
	{
		statetimer = timer + 2 SEC;
		while ((statetimer > timer) && sdo_interrupt)
			cycle ();
	}
	send_hse_state ();
	if (init_error & B_FVE)
	{
		sdo_id = 0xFFFF;
		transmit_sdo_req (FVE_ID, INIT_READ_REQ, HW_VERSION, 0, 0);
		statetimer = timer + 2 SEC;
		while ((statetimer > timer) && (sdo_id == 0xFFFF))
			cycle ();
		if ((sdo_id != 0xFFFF) && ((sdo_data [0] & 0xF0) == INIT_READ_RESP) && (!(sdo_data[0]&EXPEDITED_BIT)))
		{
			sdo_id = 0xFFFF;
			transmit_sdo_seg (FVE_ID, READ_SEGM_REQ, 0, 0);
			statetimer = timer + 2 SEC;
			while ((statetimer > timer) && (sdo_id == 0xFFFF))
				cycle ();
			if (sdo_id != 0xFFFF)
			{
				if ((sdo_data [2] == '1') && (sdo_data [4] < '3'))
					fve_hardware_ver_after_v13 = 0;
				else
					fve_hardware_ver_after_v13 = 1;

				init_node (B_FVE);
				if (!carlight)
					set_out (EMERGENCY_LIGHT, 0, 0, 0, 1, (O_CANA|O_CANB|O_HSE));
				else
					set_out (EMERGENCY_LIGHT, 0, 0, 0, 0, (O_CANA|O_CANB|O_HSE));
			}
		}
	}
	sdo_id = 0;
	sdo_master = 0;

	if ((init_error & B_DRV) && (p.simulation < SIMULATION_AUTO))
		init_nice5000 ();
	if ((p.simulation) && p.simu_mult)
		{
			if(p.simulation == SIMULATION_AUTO)
				{
					init_error &= ~B_DRV;		//仿真模式不需要与变频器通信
					set_out (POS_IN, POS_LPLS, 0, 0, 0, O_HSE); 	/* for simulation only				*/
					set_out (POS_IN, POS_UPLS, 0, 0, 1, O_HSE); 	/* for simulation only	注意SDU1是反相的	*/
					set_out (POS_IN, POS_SGM, 0, 0, 1, O_HSE);	/* for simulation only					*/
				}
			else if(p.simulation == SIMULATION_DRIVE)
				{//初始化设定电梯在1楼	
					set_out (POS_IN, POS_LPLS, 0, 0, 0, O_HSE); 	/* for simulation only				*/
					set_out (POS_IN, POS_UPLS, 0, 0, 1, O_HSE); 	/* for simulation only	注意SDU1是反相的	*/
					set_out (POS_IN, POS_SGM, 0, 0, 1, O_HSE);	/* for simulation only					*/
					send_nice5000_parameter(NICE5000_RSTATE_CURRFLOOR, 1);
					niv.F4_01 = 1;
				}
			ct = timer + 2 SEC;
			while(ct > timer)
				cycle();
		}	
	hse_state = H_SEARCH;
}

/****************************************************************************************************/
/* send changed parameter on CAN bus if necessary											*/
/****************************************************************************************************/
BYTE send_parameter (BYTE *pointer, WORD subindex, DWORD value)
{
	WORD i, j;
	WORD node_id;
	BYTE buffer[8];

	i = 0;
	node_id = 0;
	while (initpara [i].size != 0xFF)						/* search parameter table				*/
	{
		for (j = 0; j <= (initpara [i].subindex_stop - initpara [i].subindex_start); j++)
		{
			if ((pointer + (initpara [i].size * subindex)) == ((BYTE *)initpara [i].pointer + (initpara [i].size * (subindex + j))))
			{										/* found parameter in structure			*/
				if (sdo_master)
					return (1); 					/* transmit not possible				*/
				if (initpara [i].bg & B_FVE)
					node_id = FVE_ID;
				else if (initpara [i].bg & B_PSE)
					node_id = PSE_ID;
				else if (initpara [i].bg & B_DRV)
					node_id = DCU_ID;
				else if (initpara [i].bg & B_DSE)
					node_id = DSE_ID;
				if (initpara [i].size <= 4)				/* expedited transfer					*/
				{
					buffer [0] = INIT_WRITE_REQ | EXPEDITED_BIT;
					buffer [1] = initpara [i].index;
					buffer [2] = initpara [i].index >> 8;
					if (initpara [i].index == LIFT_NUMBER)
						buffer [3] = 0;
					else
						buffer [3] = subindex + j + initpara [i].subindex_start;
					buffer [4] = value;	/* write value						*/
					buffer [5] = value >>8;
					buffer [6] = value >> 16;
					buffer [7] = value >> 24;
					if (initpara [i].bg & B_ESE)
						send_message(RSDO, node_id, 8, buffer, CAN_B);
					else
						send_message(RSDO, node_id, 8, buffer, CAN_A);
				}
				return (0);						/* transmit okay						*/
			}
		}
		i++;
	}

	if ((pointer == &p.bot_floor) || (pointer == &p.top_floor))
	{
		if (nice5000_Tstate_send)
			return (1);
		if (pointer == &p.bot_floor)
			send_nice5000_parameter(NICE5000_RSTATE_BOTFLOOR, (WORD)(value + 1));
		else
			send_nice5000_parameter(NICE5000_RSTATE_TOPFLOOR, (WORD)(value + 1));
	}

	return (0);
}

/****************************************************************************************************/
/* send changed io parameter on CAN bus if necessary (write request)								*/
/****************************************************************************************************/
BYTE send_write_request (WORD menunumber, WORD paranumber, WORD pcb, BYTE bus, BYTE invers)
{
	WORD index;
	WORD node_id = 0;
	BYTE buffer[8];

	switch (menunumber)
	{
		case (M_IN_FVE):
		case (M_IN_TSE):
		case (M_IN_ESE):
		case (M_IN_EXE):
			if (!invers)
				index = INPUT_GROUP;
			else
				index = OUTPUT_GROUP;
			break;

		case (M_OUT_FVE):
		case (M_OUT_TSE):
		case (M_OUT_ESE):
		case (M_OUT_EXE):
			if (!invers)
				index = OUTPUT_GROUP;
			else
				index = INPUT_GROUP;
			break;

		default:
			return (0);
	}
	switch (menunumber)
	{
		case (M_IN_TSE):
		case (M_OUT_TSE):
			node_id = TSE_ID + pcb;
			break;

		case (M_IN_ESE):
		case (M_OUT_ESE):
			node_id = ESE_ID + pcb;
			break;
			
		case (M_IN_EXE):
		case (M_OUT_EXE):
			node_id = EXE_ID + pcb;
			break;

		case (M_IN_FVE):
		case (M_OUT_FVE):
			if (sdo_id == 0xFFFF)
				return (1);
			node_id = FVE_ID;
			break;
	}
	buffer [0] = INIT_WRITE_REQ;
	buffer [1] = index;						/* write index						*/
	buffer [2] = index >> 8;
	buffer [3] = paranumber + 1;				/* write sub index					*/
	buffer [4] = 6;							/* write parameter size				*/
	buffer [5] = 0;
	buffer [6] = 0;
	buffer [7] = 0;
	send_message(RSDO, node_id, 8, buffer, bus);
	tmp_id = node_id;
	return (0);
}

/****************************************************************************************************/
/* send changed io parameter on CAN bus if necessary (from menu)												*/
/****************************************************************************************************/
void send_iodata (BYTE *para, WORD pcb, BYTE bus)
{
	BYTE buffer[8];

	buffer [0] = WRITE_SEGM_REQ | LAST_SEGM_BIT;
	buffer [1] = para [0];
	buffer [2] = para [1];
	buffer [3] = LIFT1;
	buffer [4] = para [3];
	buffer [5] = para [4];
	send_message(RSDO, tmp_id, 8, buffer, bus);
}

/****************************************************************************************************/
/* send polarity of changed io parameter on CAN bus if necessary									*/
/****************************************************************************************************/
void send_iopolarity (WORD menunumber, WORD paranumber, WORD pcb, BYTE para, BYTE bus, BYTE invers)
{
	WORD index;
	BYTE buffer[8];

	switch (menunumber)
	{
		case (M_IN_FVE):
		case (M_IN_TSE):
		case (M_IN_ESE):
		case (M_IN_EXE):
			if (!invers)
				index = INPUT_PARA3;
			else
				index = OUTPUT_PARA3;
			break;

		case (M_OUT_FVE):
		case (M_OUT_TSE):
		case (M_OUT_ESE):
		case (M_OUT_EXE):
			if (!invers)
				index = OUTPUT_PARA3;
			else
				index = INPUT_PARA3;
			break;

		default:
			return;
	}

	buffer [0] = INIT_WRITE_REQ | EXPEDITED_BIT;
	buffer [1] = index;						/* write index						*/
	buffer [2] = index >> 8;
	buffer [3] = paranumber + 1;				/* write sub index					*/
	buffer [4] = 0;
	buffer [5] = para;
	buffer [6] = 0;
	buffer [7] = 0;
	send_message(RSDO, tmp_id, 8, buffer, bus);
}

void send_ioenable (WORD menunumber, WORD paranumber, WORD pcb, BYTE para, BYTE bus, BYTE invers)
{
	WORD index;
	BYTE buffer[8];

	switch (menunumber)
	{
		case (M_IN_FVE):
		case (M_IN_TSE):
		case (M_IN_ESE):
		case (M_IN_EXE):
			if (!invers)
				index = INPUT_PARA4;
			else
				index = OUTPUT_PARA4;
			break;

		case (M_OUT_FVE):
		case (M_OUT_TSE):
		case (M_OUT_ESE):
		case (M_OUT_EXE):
			if (!invers)
				index = OUTPUT_PARA4;
			else
				index = INPUT_PARA4;
			break;

		default:
			return;
	}

	buffer [0] = INIT_WRITE_REQ | EXPEDITED_BIT;
	buffer [1] = index;						/* write index						*/
	buffer [2] = index >> 8;
	buffer [3] = paranumber + 1;				/* write sub index					*/
	buffer [4] = para;		//One Byte
	buffer [5] = 0;
	buffer [6] = 0;
	buffer [7] = 0;
	send_message(RSDO, tmp_id, 8, buffer, bus);
}

