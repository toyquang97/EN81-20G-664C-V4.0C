
#include "typedef.h"
#include "stdio.h"
#include "parameter.h"
#include "nice5000.h"
#include "state.h"
#include "can.h"
#include "text.h"
#include "drive.h"
#include "history.h"
#include "pos.h"
#include "io.h"
#include "version.h"


/****************************************************************************************************/
/* variables for state search																		*/
/****************************************************************************************************/
const struct _statevar statevar [] =
{
	{	&door_state [0],		sizeof (door_state [0]),		DOOR_STATUS,		1,					MAX_DOOR,		B_FVE	},
	{	&dooroutputstate [0],	sizeof (dooroutputstate [0]),	DOOROUTPUTSTATE,	1,					MAX_DOOR,		B_FVE	},
	{	&p.fve_inpar [0][0],	5,								INPUT_GROUP,		1,					MAX_FVE_IN,		B_FVE	},
	{	&p.fve_outpar [0][0],	5,								OUTPUT_GROUP,		1,					MAX_FVE_OUT,	B_FVE	},
	{	&version_fve [0],		5,								SW_VERSION, 		0,					0,				B_FVE	},
	{	&hardware_fve [0],		5,								HW_VERSION,			0,					0,				B_FVE	},
	{	&p.fve_ex_inpar[0][0],	5,								INPUT_GROUP,		MAX_FVE_IN+1,		MAX_FVE_IN+MAX_FVE_EX_IN, 	B_FVE	},

	{	0,						0xFF,							0,					0,					0,				0		},
};

/****************************************************************************************************/
/* output variables to set after state search													*/
/****************************************************************************************************/
const struct _outstate outstate [] = {
	{	POSITION_INDICATOR,		0,					&level					},
	{	SPECIAL_FUNC,			FAN_1,				&fan_state				},
	{	DIRECTION_IND,			0,					&direction_ind			},
	{	SPECIAL_FUNC,			FULL_LOAD_STATE,	&fullload					},
	{	SPECIAL_FUNC,			OVER_LOAD_STATE,	&overload				},
	{	LIGHT_FUNC,				0x03,					&hall_lantern				},
	{	CARLIGHT,				0,					&carlight_switched_off		},
	{	SPECIAL_FUNC,			DOOR_OPEN,			&dooropenpush_ind		},
	{	SPECIAL_FUNC,			DOOR_CLOSE,		&doorclosepush_ind		},
	{	SPECIAL_FUNC,			DOOR_STOP,			&doorstop_ind			},
	{	SPECIAL_FUNC,			OCCUPIED,			&occupied				},
	{	SPECIAL_FUNC,			OUT_OF_ORDER,		&out_of_order			},
	{	SPECIAL_FUNC,			FAULT_SIGNAL,		&fault_signal				},
	{	SPECIAL_FUNC,			KEY_SPECIAL,		&special_ind				},
	{	SPECIAL_FUNC,			KEY_ADVANCED,		&advanced_ind			},
	{	SPECIAL_FUNC,			KEY_EMERGENCY,	&emergency_ind			},
	{	SPECIAL_FUNC,			FIRE_SERVICE_ENABLE,&firekey				},
	{	SPECIAL_FUNC,			FIRE_SERVICE,		&firecall					},
	{	SPECIAL_FUNC,			FIRE_ALARM,			&firealarmmode			},
	{	SPECIAL_FUNC,			FIRE_ALARM_FLOOR,	&firealarmfloor_ind		},
	{	SPECIAL_FUNC,			BUZZER,				&buzzer					},
	{	SPECIAL_FUNC,			REMOTE_OFF,		&remote_off_ind			},
	{	SPECIAL_FUNC,			FIRE_STATE,			&firemode				},
	{	SPECIAL_FUNC,			REMOTE_OFF_STATE,	&switched_off_ind			},
	{	SPECIAL_FUNC,			DOOR_IS_CLOSING,	&door_closing			},
	{	SPECIAL_FUNC,			SPECIAL_MODE,		&specialmode			},
	{	CONNECTED_IO,			0,					(BYTE *)0					},
	{	CONNECTED_IO,			0,					(BYTE *)1					},
	{	CONNECTED_IO,			0,					(BYTE *)2					},
	{	CONNECTED_IO,			0,					(BYTE *)3					},
	{	CONNECTED_IO,			0,					(BYTE *)4					},
	{	CONNECTED_IO,			0,					(BYTE *)5					},
	{	CONNECTED_IO,			0,					(BYTE *)6					},
	{	CONNECTED_IO,			0,					(BYTE *)7					},
	{	DOOR_IO,				0,					(BYTE *)DOOR_OP			},
	{	DOOR_IO,				0,					(BYTE *)DOOR_CL			},
	{	DOOR_IO,				0,					(BYTE *)DOOR_REV		},
	{	DOOR_IO,				0,					(BYTE *)DOOR_RAMP		},
	{	DOOR_IO,				1,					(BYTE *)DOOR_OP			},
	{	DOOR_IO,				1,					(BYTE *)DOOR_CL			},
	{	DOOR_IO,				1,					(BYTE *)DOOR_REV		},
	{	DOOR_IO,				1,					(BYTE *)DOOR_RAMP		},
	{	SAFETY_CIRCUIT,			SC1,					&safety_circuit			},
	{	SAFETY_CIRCUIT,			SC2,					&safety_circuit			},
	{	SAFETY_CIRCUIT,			SC3,					&safety_circuit			},
	{	POS_IN,					POS_SGM,			NULL				},
	{	ENDSTRING,				0,					0						},
};

/****************************************************************************************************/
/* set special outputs after state search														*/
/****************************************************************************************************/
void set_out_state (BYTE pos, BYTE bus){
	BYTE sub = 0;
	switch (outstate [pos].function)
	{
		case (POSITION_INDICATOR):				/* position indicator						*/
			level_display (level, bus);
			break;

		case (DIRECTION_IND):
			set_out (DIRECTION_IND, direction_ind, level + 1, 0, 1, bus);
			break;

		case (LIGHT_FUNC):
			sub = outstate [pos].subfunction;
			if(sub & (HALL_LANTERN_UP | HALL_LANTERN_DN))
				{
					if (hall_lantern)						/* hall lantern to set						*/
						set_out (LIGHT_FUNC, hall_lantern & DIRECTION_BITS, target_floor + 1, hall_lantern >> 4, 1, bus);
					else
						set_out (LIGHT_FUNC, ARROW_UP | ARROW_DOWN, 0xFF, ALL_DOORS_CLOSED, 0, bus);
				}
			if(sub & (DIRECTION_IND_UP | DIRECTION_IND_DN))
				{
				}
			break;

		case (CARLIGHT):
			if (light_switch_status)
				set_out (CARLIGHT, 0, 0, 0, 0, bus);
			else
			{
				if (carlight_switched_off)
					set_out (CARLIGHT, 0, 0, 0, 0, bus);
				else
					set_out (CARLIGHT, 0, 0, 0, 1, bus);
			}
			break;

		case (CONNECTED_IO):
			set_out (CONNECTED_IO, 0, (BYTE)((DWORD)outstate [pos].value), 0, (connected_io >> (BYTE)((DWORD)outstate [pos].value)) & 0x01, bus);
			break;

		case (SAFETY_CIRCUIT):
			if (bus == O_CANA1)	
				transmit_inputs (outstate [pos].function, outstate [pos].subfunction, (safety_circuit & outstate [pos].subfunction) ? 1 : 0);
			break;
			
		case (POS_IN):
			if (bus == O_CANA1)	
				transmit_inputs (outstate [pos].function, outstate [pos].subfunction, (door_zone & POS_SGM) ? 1 : 0);
			break;

		case (DOOR_IO):
			if (bus == O_CANA1)	
				set_out (DOOR_IO, (BYTE)((DWORD)outstate [pos].value), 0, 1 << outstate [pos].subfunction, (dooroutputstate [outstate [pos].subfunction] & (BYTE)((DWORD)outstate [pos].value)) ? 1 : 0, bus);
			break;

		default:
			if (outstate [pos].subfunction == FAN_1)
			{
				if (fan_switch_status)
					set_out (SPECIAL_FUNC, FAN_1, 0, 0, 0, (O_CANA|O_CANB|O_HSE));
				else
					set_out (SPECIAL_FUNC, FAN_1, 0, 0, fan_state, (O_CANA|O_CANB|O_HSE));
			}
			else
				set_out (outstate [pos].function, outstate [pos].subfunction, 0, 0, *outstate [pos].value, bus);
			break;
	}
}

/****************************************************************************************************/
/* default state (for test only)																*/
/****************************************************************************************************/
void defaultstate (void)
{
	set_state ();
	cycle ();
}

/****************************************************************************************************/
/* decide if input or output has to be requested with SDO											*/
/****************************************************************************************************/
BYTE request_io (WORD index, BYTE subindex, WORD pos)
{
	BYTE function;

	switch (index)
	{
		case (INPUT_GROUP):
		case (OUTPUT_GROUP):
			if ((statevar[pos].bg & B_FVE) &&
			(((index == INPUT_GROUP) && (subindex > MAX_FVE_IN)) || ((index == OUTPUT_GROUP) && (subindex > MAX_FVE_OUT))) &&
			((hardware_fve [1] == '1') && (hardware_fve [3] < '3')))
				return (0);	

			function = *((BYTE *)statevar [pos].pointer + ((subindex - statevar [pos].subindex_start) * MAX_IO_TYPE));
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
					if (index == OUTPUT_GROUP)
						return (1);
					else
						return (0);

				default:
					if (index == INPUT_GROUP)
						return (1);
					else
						return (0);
			}

		default:
			return (1);					/* request all other object in every case		*/
	}
}

/****************************************************************************************************/
/* search state of a special node															*/
/****************************************************************************************************/
BYTE search_nodestate (BYTE node)
{
	WORD i = 0;
	BYTE j;
	BYTE subindex;
	BYTE help;
	BYTE node_id;
	DWORD statetimer;

	switch (node)
	{
		case (B_FVE):									/* init message	for FVE - SDO		*/
			node_id = FVE_ID;
			break;

		case (B_PSE):									/* init message	for PSE - SDO		*/
			node_id = PSE_ID;
			break;

		case (B_DRV):									/* init message	for PSE - SDO		*/
			node_id = DCU_ID;
			break;

		default:
			return (0);
	}
	while (statevar [i].size != 0xFF)						/* variables for polling				*/
	{
		if (statevar [i].bg & node)						/* variables for this node				*/
		{
			if ((!statevar [i].subindex_start) ||				/* variables without subindex			*/
			(statevar [i].subindex_start == statevar [i].subindex_stop))
				help = 0;
			else										/* parameter with subindex > 0		*/
				help = 1;
			for (subindex = statevar [i].subindex_start; subindex <= statevar [i].subindex_stop; subindex++)
			{
				if (request_io (statevar [i].index, subindex, i))
				{									/* necessary to request (only for io) ?	*/
					sdo_id = 0xFFFF;					/* reset marker for SDO received		*/
					transmit_sdo_req (node_id, INIT_READ_REQ, statevar [i].index, subindex, 0);
					statetimer = timer + 2 SEC;			/* wait 2 seconds for SDO from node	*/
					while ((statetimer > timer) && (sdo_id == 0xFFFF))
						cycle ();						/* do all cyclic functions				*/
					if (sdo_id == 0xFFFF)
						return (1);					/* no message received				*/
					if ((sdo_data [0] & 0xF0) == INIT_READ_RESP)
					{								/* response to read request			*/
						if (sdo_data [0] & EXPEDITED_BIT)	/* expedited transfer				*/
						{
							switch (statevar [i].size)
							{
								case (1):
									*((BYTE *)statevar [i].pointer + ((subindex - statevar [i].subindex_start) * help)) = *(BYTE *)&sdo_data[4];
									break;			/* write BYTE variables				*/

								case (2):
									*((WORD *)statevar [i].pointer + ((subindex - statevar [i].subindex_start) * help)) = *(WORD *)&sdo_data[4];
									break;			/* write WORD variables				*/
								case (4):
									*((DWORD *)statevar [i].pointer + ((subindex - statevar [i].subindex_start) * help)) = *(DWORD *)&sdo_data[4];
									break;			/* write DWORD parameter			*/
							}
						}
						else							/* normal transfer					*/
						{
							sdo_id = 0xFFFF;			/* reset marker for SDO received		*/
							transmit_sdo_seg (node_id, READ_SEGM_REQ, 0, 0);
							statetimer = timer + 2 SEC;	/* wait 2 seconds for SDO from node	*/
							while ((statetimer > timer) && (sdo_id == 0xFFFF))
								cycle ();				/* do all cyclic functions				*/
							if (sdo_id == 0xFFFF)
								return (1);			/* no message received				*/
							if ((sdo_data [0] & COMMAND_SPECIFIER) == READ_SEGM_RESP)
							{
								switch (statevar [i].index)
								{
									case (INPUT_GROUP):
										for (j = 0; j < MAX_IO_TYPE; j++)	/* write to virtual input mapping	*/
											virt_in [j] = sdo_data [j + 1];
										if (((sdo_data [IO_BASIC_FUNC + 1] == HALL_CALL_SPECIAL)	||
												(sdo_data [IO_BASIC_FUNC + 1] == HALL_CALL_ADVANCED) ||
												(sdo_data [IO_BASIC_FUNC + 1] == HALL_CALL_EMERGENCY))
												 &&(sdo_data [IO_ENABLE + 1] == ENABLE))
											handle_input (LIFT1, 1);		/* handle sdo input data	*/
										else if (((sdo_data [IO_BASIC_FUNC + 1] != CAR_CALL) && (sdo_data [IO_BASIC_FUNC + 1] != HALL_CALL))
													&&(sdo_data [IO_ENABLE + 1] == ENABLE))
											handle_input (LIFT1, 0);		/* handle sdo input data	*/
										break;

									case (OUTPUT_GROUP):
										if ((sdo_data [IO_BASIC_FUNC + 1] == CAR_CALL) || (sdo_data [IO_BASIC_FUNC + 1] == HALL_CALL))
										{				/* only handle call acknowledgements	*/
											for (j = 0; j < MAX_IO_TYPE; j++)			/* write to virtual input mapping	*/
												virt_in [j] = sdo_data [j + 1];
											if(virt_in[IO_ENABLE] == ENABLE)
												handle_input (LIFT1, 0);		/* handle sdo input data			*/
										}
										break;

									default:
										for (j = 0; j < statevar [i].size; j++)
											*((BYTE *)statevar [i].pointer + j) = sdo_data[j + 1];
										break;
								}
							}
						}
					}
					else if ((sdo_data[0]&0xF0) != ABORT_REQ)	/* ignore abort SDO				*/
						return (1);							/* return error					*/
				}
			}
		}
		i++;
	}
	return (0);
}

/****************************************************************************************************/
/* active HSE state search																	*/
/****************************************************************************************************/
void search_state (void)
{
	DWORD statetimer;
	BYTE i;

	sdo_master = 1;
	statetimer = timer + 5;							/* wait 0.5 second							*/
	while (statetimer > timer)
		cycle ();									/* do all cyclic functions					*/
	if (!search_nodestate (B_FVE))
	{
		hb_error &= ~(B_FVE | B_CAN);				/* reset heartbeat error bit					*/
		for (i = 0; i < p.doornumber; i++)				/* set door state in 1 variable				*/
		{
			if (door_state [i] == DOOR_CLOSED)		/* door is closed							*/
				bit_set (doorstate, i);
			else									/* door is not closed						*/
				bit_reset (doorstate, i);
		}
	}

	i = 0;
	while (outstate [i].function != ENDSTRING)
	{
		outtimer = 2;								/* wait about 10 ms between 2 messages	*/
		while (outtimer)
			cycle ();								/* do all cyclic functions					*/
		set_out_state (i, O_CANA1);
		i++;
	}
	sdo_id = 0;									/* reset marker to wait for SDO response	*/
	sdo_master = 0;
}																		  

/****************************************************************************************************/
/* set HSE state																			*/
/****************************************************************************************************/
void set_state (void){
	BYTE i = 0;
	DWORD j = 0;

//Can 故障判定
	if (((canerrorstate & (TXA_SW_OVERRUN | RXA_SW_OVERRUN | CANA_BUSOFF | SXA_SW_OVERRUN))
			|| (hb_error & B_CAN)))			/* CAN bus error (overrun, bus off, tx error) */
		{
			bFunc_flag |= FUNC_CANA_ERROR;
			if(p.canbus_check == 1)
				{
					if(((drive_state > D_STOPPED) && (drive_state < D_OPERATION_ENABLED)) ||
							((drive_state > D_SPEED_ZERO)  && (drive_state < D_SWITCHED_ON)))
						i = 1;
					else if((drive_state == D_OPERATION_ENABLED) && (canbus_check_timer > timer))
						i = 1;				
				} 
			else if(p.canbus_check == 2)
				{
					if(drive_state != D_STOPPED)
						i = 1;
				} 
				
			if((!p.canbus_check) || !i)
				{
					if (drive_state != D_STOPPED) 			/* Lift is moving 						*/
						{
							stop_drive();
						}
					else										/* drive is in stopped state					*/
						{
							hse_state = H_CAN_ERROR;
							hse_text = HT_CAN_ERROR;
						}
					return;
				}
			else
				{
					if(canerrorstate & CANA_BUSOFF)
						{
							CLRENA0 = ISE_CAN;
							CAN1MOD |= 01ul;
							__nop();__nop();
							j = CAN1GSR;
							j &= 0x000000FF;
							CAN1GSR = j;					//清除故障计数
							CAN1MOD &= ~01ul; 
							__nop();__nop();
							SETENA0 = ISE_CAN;
						}
					canerrorstate &= (~CANA_MASK);
					hb_error &= ~B_CAN;
				} 		
		}
	else		
		bFunc_flag &= ~FUNC_CANA_ERROR;

	if (init_error)
	{											/* heartbeat from mand.node, but not operational*/
		if (drive_state != D_STOPPED)				/* Lift is moving							*/
		{
			if(init_error & B_DRV)
				{
					emergency_stop ();					/* send emergency stop					*/
					hse_state = H_WAIT_STOP;
					hse_text = HT_WAIT_STOP;
				}
			else
				stop_drive();
		}
		else										/* drive is in stopped state					*/
		{
			hse_state = H_INIT;
			hse_text = HT_INIT;
		}
		return;
	}

	if (hb_error & B_DRV)
	{
		if (drive_state != D_STOPPED)				/* Lift is moving							*/
		{
			emergency_stop ();					/* send emergency stop					*/
			hse_state = H_WAIT_STOP;
			hse_text = HT_WAIT_STOP;
		}
		else
		{
			hse_state = H_HB_ERROR;
			hse_text = HT_HB_DRV;
		}
		return;
	}
	
	if (doortest_active & 0x02)								/* Doortest mode								*/
	{
		if ((drive_state == D_STOPPED) || (hse_state == H_INSP))	/* only if lift is stopped						*/
		{
			hse_state = H_DOORTEST;
			hse_text = HT_DOORTEST;
			return;
		}
	}
	if (driveerror & D_EMERG_STOP)					/* Drive is in error state					*/
	{
		if (drive_state != D_STOPPED)				/* Lift is moving							*/
		{
			emergency_stop ();					/* send emergency stop					*/
			hse_state = H_WAIT_STOP;
			hse_text = HT_WAIT_STOP;
		}
		else										/* drive is in stopped state					*/
		{
			hse_state = H_DRIVE_ERROR;
			if (driveerror & D_NO_SC)
				hse_text = HT_DRV_SC;
			else if (driveerror & (D_RUNCONT_NOTON | D_RUNCONT_NOTOFF))
				hse_text = HT_DRV_RUNCON;
			else if (driveerror & (D_BRAKE_NOTON | D_BRAKE_NOTOFF))
				hse_text = HT_DRV_BRAKE;
			else if (driveerror & (D_NICE_MOVE_NOTON | D_NICE_MOVE_NOTOFF))
				hse_text = HT_DRV_MOVE;
			else if (driveerror & (D_BRKCONT_NOTON | D_BRKCONT_NOTOFF))
				hse_text = HT_DRV_BRKCON;
			else if (driveerror & (D_FXCONT_NOTON | D_FXCONT_NOTOFF))
				hse_text = HT_DRV_FXCON;
			else if(driveerror & (D_NICE_RUN_NOTON | D_NICE_RUN_NOTOFF))
				hse_text = HT_DRV_NICE_RUNSIG;
			else if(driveerror & (D_NICE_BRAKE_NOTON |D_NICE_BRAKE_NOTOFF))
				hse_text = HT_DRV_NICE_BRAKESIG;
			else if(driveerror & D_NICE_DIR_NOTON)
				hse_text = HT_DRV_NICE_DIRSIG;
			else
				hse_text = HT_DRIVE_ERROR;
		}											  
		return;
	}

#if	SRU_OLD
	if (((term_active) && (p.motor_tuned))			/* Terminal mode							*/
#elif	SRU_NEW
	if (((term_active) || (bTestUCMP && p.ucmp_test_dir)) && (p.motor_tuned))			/* Terminal mode							*/
#endif
	{
		hse_state = H_TERM;
		hse_text = HT_TERM;
		return;
	}

	if ((insp & (INSP_TOPCAR_ACTIVE | INSP_PITBOX_ACTIVE)) && (p.motor_tuned))		/* Inspection is on						*/
	{
		hse_state = H_INSP;
		hse_text = HT_INSP;
		return;
	}

	if (em_op & INSP_ON)								/* Emergency recall operation				*/
	{
		if (p.motor_tuned)
		{
			hse_state = H_EM_OP;
			hse_text = HT_EM_OP;
		}
		else
		{
			hse_state = H_IV_TEACH_IN;
			hse_text = HT_IV_TEACH_IN;
		}
		return;
	}

	if (hb_error || (exe_hb_error & 0x03))									/* no heartbeat from mandatory node		*/
	{
		if (drive_state != D_STOPPED)				/* Lift is moving							*/
		{
			if(hb_error & B_DRV)
				{
					emergency_stop ();					/* send emergency stop					*/
					hse_state = H_WAIT_STOP;
					hse_text = HT_WAIT_STOP;
				}
			else
				stop_drive();
		}
		else										/* drive is in stopped state					*/
		{
			hse_state = H_HB_ERROR;
			if (hb_error & B_CAN)
				hse_text = HT_NO_CAN_A;
			else if (hb_error & B_FVE)
				hse_text = HT_HB_FVE;
			else if(exe_hb_error & 0x03)
				hse_text = HT_HB_EXE;
			else
				hse_text = HT_HB_ERROR;
		}
		return;
	}
	if (!(safety_circuit & SC1))						/* primary safety circuit is off				*/
	{
		if (drive_state == D_STOPPED)				/* Lift is stopped							*/
		{
			hse_state = H_SC_ERROR;
			if (limit_switch_error)					/* Lift probably in final limit witch			*/
				hse_text = HT_SC_LIMIT;
			else
				hse_text = HT_SC_ERROR;
			return;
		}
	}
	else
		sc_limit_fg = 0;
	if ((general_error) || (door_short_conn))		/* general error							*/
	{
		if (drive_state != D_STOPPED)				/* Lift is moving							*/
		{
			if(door_short_conn)
				stop_drive();
			else
				{
					emergency_stop ();					/* send emergency stop					*/
					hse_state = H_WAIT_STOP;
					hse_text = HT_WAIT_STOP;
				}
		}
		else										/* drive is in stopped state					*/
		{
			hse_state = H_ERROR;					/* go to error state						*/
			if (general_error & G_TRIPTIME)			/* trip time exceeded						*/
				hse_text = HT_TRIP_TIME;
			else if (general_error & (G_OPENDOOR1 | G_OPENDOOR2 | G_CLOSEDOOR1 | G_CLOSEDOOR2))
				hse_text = HT_DOORERROR;
			else if (general_error & G_DL_SHORTED)
				hse_text = HT_DL_SHORTED;			
			else if (general_error & G_MSI_NOTOFF)			/* trip time exceeded						*/
				hse_text = HT_MSI_NOTOFF;
			else if(door_short_conn)
				hse_text = HT_DOOR_SHORT;
			else
				hse_text = HT_ERROR;
		}
		return;
	}

	if (teach_in && p.motor_tuned)		/* teach in state							*/
	{
		hse_state = H_TEACH_IN;
		hse_text = HT_TEACH_IN;
		return;
	}
	if ((!p.setup_ready) || (!p.motor_tuned) || (nice5000_Rcmd_data2 & NICE5000_RCMD2_SETUP_READY))
	{
		if (drive_state != D_STOPPED)				/* Lift is moving							*/
		{
			emergency_stop ();
			hse_state = H_WAIT_STOP;
			hse_text = HT_WAIT_STOP;
		}
		else										/* drive is in stopped state					*/
		{
			hse_state = H_NO_SETUP;				/* go to state "setup not done"				*/
			hse_text = HT_NO_SETUP;
		}
		return;
	}
	if (ups_power)								/* emergency power supply from UPS		*/
	{
		hse_state = H_EVACUATION;
		hse_text = HT_EVACUATION;
		return;
	}
	if (poserror)
	{
		if (drive_state != D_STOPPED)				/* Lift is moving							*/
		{
			if((poserror & P_KH5_ON) || (poserror & P_KH5_OFF))
				{
					emergency_stop ();					/* send emergency stop					*/
					hse_state = H_WAIT_STOP;
					hse_text = HT_WAIT_STOP;
				}
			else
				stop_drive();
		}
		else										/* drive is in stopped state					*/
		{
			hse_state = H_ERROR;					/* go to error state						*/
			if (poserror & P_ILO_ON)
				hse_text = HT_ILO_ON;
			else if (poserror & P_ILU_ON)
				hse_text = HT_ILU_ON;
			else if (poserror & P_KH5_ON)
				hse_text = HT_KH5_ON;
			else if (poserror & P_KH5_OFF)
				hse_text = HT_KH5_OFF;
			else if (poserror & (P_VU_NOT_ON | P_VO_NOT_ON | P_VO_VU_ERROR))
				hse_text = HT_VO_VU;
			else if (poserror & P_DIVING_ERR)
				hse_text = HT_DIVING_ERR;
		}
		return;
	}
	
	if(bForceResetTrip)
		{// Reset
			hse_state = H_RESET;
			hse_text = HT_RESET;
			return;
		}
	if (((drive_state == D_STOPPED) && (!(door_zone & POS_SGM))) ||
		(diving))
	{
		if ((doorstate != ALL_DOORS_CLOSED) ||		/* Door is open							*/
			(!(safety_circuit & SC2)) ||					/* or safety circuit car door is off			*/
			(!(safety_circuit & SC3)))						/* or safety circuit shaft door is off			*/
		{
			hse_state = H_DOOROPEN;
			if (doorstate != ALL_DOORS_CLOSED)
				hse_text = HT_DOOROPEN;
			else if (!(safety_circuit & SC2))
				hse_text = HT_SC2;
			else if (!(safety_circuit & SC3))
				hse_text = HT_SC3;
		}
		else
		{
			hse_state = H_DIVING;
			hse_text = HT_DIVING;
		}
		return;
	}
	if ((driveerror & D_STOP_NEXT_FLOOR) &&		/* Stop lift in next floor					*/
	(drive_state == D_STOPPED))					/* Lift is stopped							*/
	{
		hse_state = H_DRIVE_ERROR;
		if (driveerror & D_BRAKESTATE)
			hse_text = HT_BRAKESTATE;
		else
			hse_text = HT_DRIVE_ERROR;
		return;
	}
	if (triperrorcount >= MAX_TRIP_ERROR)			/* error count did reach limit				*/
	{
		if (drive_state == D_STOPPED)				/* finish last trip							*/
		{
			hse_state = H_ERROR;
			hse_text = HT_RESTART_INH;
			return;
		}
	}
	if (drive_state == D_STOPPED)					/* only if drive is stopped					*/
	{
		for (i = 0; i < p.doornumber; i++)
		{
			if (doorcloseerrorcount [i])
			{									/* lift cant close doors				*/
				general_error |= (G_CLOSEDOOR1 << i);
				hse_state = H_ERROR;
				hse_text = HT_DOORERROR;
				return;
			}
			if (dooropenerrorcount [i])
			{									/* lift cant open doors				*/
				general_error |= (G_OPENDOOR1 << i);
				hse_state = H_ERROR;
				hse_text = HT_DOORERROR;
				return;
			}
		}
	}
	if (relevelflag)
	{
		if ((!carlight) ||							/* car light is off							*/
		(temperature & (OVERTEMP1 | OVERTEMP2 | ROOMTEMP_MIN | ROOMTEMP_MAX)) ||		/* motor overtemperature		*/
		(limit_switch_error) || (steel_broke))						/* Lift was in final limit switch before		*/
			hse_state = H_ERROR_RELEV;
		else
			hse_state = H_STANDSTILL;
		hse_text = HT_RELEVEL;
		return;
	}
	switch (drive_state)
	{
		case (D_START_MOVING) :
		case (D_READY_TO_SWITCH_ON):
		case (D_OPERATION_ENABLED):
			hse_state = H_MOVING;					/* go to moving state						*/
			hse_text = HT_MOVING;
			break;

		case (D_SLOW_DOWN) :
		case (D_STOPPING) :
		case (D_OPERATION_DISABLED):
		case (D_WAIT_STOP):
		case (D_SWITCHED_OFF):
			hse_state = H_ARRIVAL;
			hse_text = HT_ARRIVAL;
			break;

		default :
			if (hse_state != H_START)				/* don't change while in start state			*/
			{
				if (temperature & (OVERTEMP1 | OVERTEMP2 | ROOMTEMP_MIN | ROOMTEMP_MAX))
				{								/* motor overtemperature or machine room temp.	*/
					hse_state = H_ERROR_RELEV;
					hse_text = HT_OVERTEMP;
				}
				else if (overload)					/* car overloaded							*/
				{
					hse_state = H_STANDSTILL;
					hse_text = HT_OVERLOAD;		/* display text for overload				*/
				}
				else if (!carlight)					/* car light is off							*/
				{
					hse_state = H_ERROR_RELEV;
					hse_text = HT_CARLIGHT;		/* display text for car light off				*/
				}
				else if (limit_switch_error)			/* Lift was in final limit switch before		*/
				{
					hse_state = H_ERROR_RELEV;
					hse_text = HT_SC_LIMIT;		/* display text for car light off				*/
				}
				else if (steel_broke)			/* Lift was in final limit switch before		*/
				{
					hse_state = H_ERROR_RELEV;
					hse_text = HT_STEEL_BROKE;		/* display text for car light off				*/
				}
				else if (p.doorpos [level])			/* Shaft doors for this floor				*/
				{
					hse_state = H_STANDSTILL;
					if (limitstatus)
						hse_text = HT_LIMITCALL;
					else
						hse_text = HT_STANDSTILL;		/* display text for standstill				*/
				}
				else								/* go to other floor						*/
				{
					hse_state = H_DIVING;
					hse_text = HT_DIVING;
				}
			}
			break;
	}
}


