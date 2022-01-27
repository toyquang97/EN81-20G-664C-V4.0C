
#include "typedef.h"
#include "parameter.h"
#include "state.h"
#include "call.h"
#include "can.h"
#include "io.h"
#include "pos.h"
#include "history.h"
#include "Nice5000.h"

DWORD Nudging_timer_count =0-1;
BYTE  NudingMode =0;
BYTE  Enabal_opendoor =1;
BYTE hhhhhhhhh =0,tttttttt =0;
DWORD Nudging_Buz_timer_count =0-1;
DWORD Nudging_opendoor_timer_count =0-1;
static BYTE callpriorityold;

/****************************************************************************************************/
/* Definitions for door open state																	*/
/****************************************************************************************************/
#define	D_CLOSED			0					/* door is closed						*/
#define	D_STAYTIME			1					/* minimum open stay time			*/
#define	D_HANDLE_CALLS	2					/* minimum stay time over; handle calls	*/
#define	D_CLOSING			3					/* door is closing						*/

/****************************************************************************************************/
/* Open or close doors																*/
/****************************************************************************************************/
void set_door (BYTE doors, BYTE command)
{
	BYTE i;
	BYTE doorcommand [MAX_DOOR];

	for (i = 0; i < MAX_DOOR; i++)						/* for all doors						*/
	{
		if (bit_select (doors, i))
			doorcommand [i] = command;
		else
			doorcommand [i] = DOOR_OFF;
	}
	door_command (doorcommand [0], doorcommand [1]);
}

/****************************************************************************************************/
/* Open or close doors in fireman mode																*/
/****************************************************************************************************/
void fire_door (BYTE doors, BYTE command)
{
	BYTE i;
	WORD state;
	BYTE doorcommand [MAX_DOOR];

	switch (command)
	{
		case (OPEN_DOOR):
			state = DOOR_CLOSED;
			break;
		case (CLOSE_DOOR):
			state = DOOR_OPENED;
			break;
		default:
			state = ~command;
			break;
	}
	doors &= (p.doortable [adt][level] & p.doorpos [level]);
	for (i = 0; i < MAX_DOOR; i++)						/* for all doors						*/
	{
		if ((bit_select (doors, i)) && (door_state [i] != state))
			doorcommand [i] = command;
		else
			doorcommand [i] = DOOR_OFF;
	}
	door_command (doorcommand [0], doorcommand [1]);
}

/****************************************************************************************************/
/* Set park door state																				*/
/****************************************************************************************************/
BYTE set_park_state (void)
{
	BYTE i;
	BYTE doorcommand [MAX_DOOR];
	BYTE new_command = 0;
	BYTE parkstate = 0;

	if (callpriority == C_STANDARD)
	{
		if (adt == DOOR_NORMAL)					/* normal mode						*/
			parkstate = p.parkdoor [level];				/* use parking door settings			*/
		else										/* timer trip							*/
		{
			parkstate = p.doortable [adt][level];			/* open doors only if car+landing calls	enabled	*/
			parkstate &= p.doortable [adt_hall][level];
		}
	}
	else if(callpriority == C_ADVANCED)
		parkstate = p.parkdoor [level];
	else
		parkstate = p.doortable [adt][level];

	if ((p.parkdoormode [0] & DOOR1) != P_DOOR_OPEN)			/* parking with closed door 1			*/
		parkstate &= 0xFE;							/* clear door 1						*/
	if ((p.parkdoormode [1] & DOOR1) != P_DOOR_OPEN)			/* parking with closed door 2			*/
		parkstate &= 0xFD;							/* clear door 2						*/
	if (((callpriority > C_STANDARD) && (callpriority != C_ADVANCED))||				/* priority mode						*/
		landings_off ||								/* landing calls off (from menu)		*/
		landings_off_key ||							/* landing calls off (from key)			*/
		intermediate_door ||							/* intermediate door is open			*/
		fullload ||									/* car is full (if no call -> defect)		*/
		overload ||									/* car is overloaded					*/
		earthquake ||								/* earthquake						*/
		(car_attendance && (!(p.attend_carcall & ATTEND_DOOR_MODE))) ||	/* car attendance mode				*/
		correct_level ||
		emp_power )
	{
		parkingtimer = timer + (p.parkingtime SEC);		/* restart parking trip timer			*/
		parkstate = p.doortable [adt][level];
		if((p.cooperate_sel1 & COOP1_THROUGH_DOOR) && (p.doorpos [level] == (DOOR1 | DOOR2)))
			{//��ǰ¥��Ϊ��ͨ�ţ���ѡ�� p.she_doornumber ����
				parkstate &= (p.she_doornumber & (DOOR1 | DOOR2));
			}
		else
			parkstate &= p.doorpos [level];
		if(emp_power)
			parkstate &= p.emp_power_door;
		if (!parkstate)								/* no parking doors, but door must stay open	*/
			return (0xFF);							/* leave doors open					*/
	}

	if (callpriority == C_FIREMAN)					/* dont change door state in fireman mode	*/
		return (0xFF);
	if (door_off && (callpriority != C_FIREALARM) && (callpriority != C_FIREMAN))
		return (0xFF);
	if (remote_switched_off &&						/* lift is remote switched off			*/
	(callpriority == C_STANDARD))
		parkstate = p.remote_off_door;

	if((callpriority == C_EVACUATION) && evacuation)
		{
			if(p.evac_door)
				parkstate = evacuationdoor;
			else
				parkstate = (evacuationdoor ^ evacuationdoor) & 0x03;
		}

	if (callpriority == C_FIREALARM)
	{
		if (level != firealarmfloor)					/* lift not in fire alarm floor			*/
			return (0xFF);
		if(p.firealarm_function & FIREALARM_DOORPARK_MODE)
			parkstate = 0;
	}
	parkstate &= p.doorpos [level];
	for (i = 0; i < MAX_DOOR; i++)
	{
		if (bit_select (parkstate, i) &&					/* door should stay open				*/
		(door_state [i] == DOOR_CLOSED))			/* but door is closed					*/
		{
			if (!door_off)							/* no door opening off				*/
			{
				doorcommand [i] = OPEN_DOOR;		/* open this door						*/
				new_command = 1;
			}
			else
	   	{
	   		doorcommand [i] =  DOOR_OFF;
	   	}
		}
		else if ((!bit_select (parkstate, i))	&&			/* door should stay closed				*/
		(door_state [i] == DOOR_OPENED))			/* but door is open					*/
		{
			doorcommand [i] = CLOSE_DOOR;			/* close this door						*/
			new_command = 1;
		}
		else
			doorcommand [i] = DOOR_OFF;			/* no door command necessary			*/
	}

	if (new_command)								/* new command to send				*/
		door_command (doorcommand [0], doorcommand [1]);
	return (parkstate);
}

/****************************************************************************************************/
/* Handle door open push																*/
/****************************************************************************************************/
BYTE handle_dooropenpush (void)
{

	BYTE i = 0;
	static BYTE dooropenpush_old = 0;
	if(!Enabal_opendoor)
		return 0;
	dooropenpush &= p.doorpos [level];				/* only use existing shaft doors		*/
	if (callpriority == C_FIREMAN)					/* lift is in fireman mode				*/
	{
		dooropenpush &= p.doortable [adt] [level];		/* only use fireman doors				*/
		if ((!firekey) && (!auto_fire))					/* fire key off in fireman mode			*/
			dooropenpush = 0;						/* ignore door open push				*/
	}
	else											/* not in fireman mode				*/
	{
		if ((doorstate != ALL_DOORS_CLOSED) && (p.parkdoormode[0] & 0x80) &&		/* not all doors closed				*/
					(p.parkdoormode[1] & 0x80))
			dooropenpush &= ~doorstate;			/* only reopen not already closed doors	*/
	}
	if (dooropenpush_old != dooropenpush)			/* door open push pressed				*/
	{
		if (dooropenpush)							/* doors to open						*/
		{
			set_door (dooropenpush, OPEN_DOOR);	/* send door command				*/
			i = 1;
			doorstate &= ~dooropenpush;			/* mark doors as open				*/
			if (car_attendance)						/* in car attendance mode don't reclose autom.	*/
				door_attendance = 1;
		}
		else if (firedoormode)
		{
			if (p.fire_dooropenmode == FIREDOOR_STOP)
				fire_door (dooropenpush_old, STOP_DOOR);	/* stop door					*/
			else if (p.fire_dooropenmode == FIREDOOR_REVERSE)
				fire_door (dooropenpush_old, CLOSE_DOOR);	/* reclose door				*/
		}
		dooropenpush_old = dooropenpush;			/* save old state of door open push		*/
	}
	return (i);
}

/****************************************************************************************************/
/* Handle door stop push																*/
/****************************************************************************************************/
void handle_doorstoppush (void){
	if(p.landingcall_push & PUSH_STOP_SHIP)
		{
			doorstoppush &= p.doorpos [level];				/* only use existing shaft doors		*/
			if(doorstoppush & DOOR1)
				{
					if(doorstate & DOOR2)
						{//��2��عرգ�����û��ִ�п��Ŷ���
							if(doorstoppush_sheopen_flag != DOOR1)
								{
									doorstoppush_sheopen_flag = DOOR1;
									doorstoppush_sheclose_flag = 0;
									set_door (DOOR1, OPEN_DOOR);	/* send door command				*/
									doorstate &= ~DOOR1;
								}
						}
					else if(doorstoppush_sheclose_flag != DOOR2)
						{//��2δ�رգ��ȹر���2
							set_door (DOOR2, CLOSE_DOOR);	/* send door command				*/
							doorstoppush_sheclose_flag = DOOR2;
							doorstoppush_sheopen_flag = 0;
						}
				}
			else if(doorstoppush == DOOR2)
				{
					if(doorstate & DOOR1)
						{//��1��عرգ�����û��ִ�п��Ŷ���
							if(doorstoppush_sheopen_flag != DOOR2)
								{
									doorstoppush_sheopen_flag = DOOR2;
									doorstoppush_sheclose_flag = 0;
									set_door (DOOR2, OPEN_DOOR);	/* send door command				*/
									doorstate &= ~DOOR2;
								}
						}
					else if(doorstoppush_sheclose_flag != DOOR1)
						{//��1δ�رգ��ȹر���1
							set_door (DOOR1, CLOSE_DOOR); /* send door command				*/
							doorstoppush_sheclose_flag = DOOR1;
							doorstoppush_sheopen_flag = 0;
						}
				}
			if(doorstate == ALL_DOORS_CLOSED)
				{
					doorstoppush_sheopen_flag = 0;
					doorstoppush_sheclose_flag = 0;
				}
		}
	else
		{
			doorstopstate &= ~doorstate;					/* reset door stop state for closed doors */
			if ((doorstoptimer < timer) && p.doorstoptime)		/* time out for door stop 			*/
				{
					if((doorstopstate) && (p.landingcall_push & PUSH_ALARM))
						{
							bFunc_flag |= FUNC_DOORSTOP_TIMEOUT;
							set_out(SPEAKER_BUZ, BUZZER_NORMAL, 5, 0, 1, O_CANA | O_CANB | O_HSE);		//����5s
						}
					doorstopstate = 0;
				}
			doorstoppush &= p.doorpos [level];				/* only use existing shaft doors		*/
			if (callpriority >= C_EVACUATION) 			/* firemen, evacuation or fire alarm mode */
				doorstopstate = 0;							/* ignore door stop push				*/
			else if (doorstoppush)							/* door stop push was pressed 		*/
			{
				if (doorstate != ALL_DOORS_CLOSED)		/* not all doors closed 			*/
					doorstoppush &= ~doorstate; 		/* only reopen not already closed doors */
				doorstoppush &= ~doorstopstate; 		/* check if new doors to stop		*/
				if (doorstoppush) 						/* new doors to stop					*/
				{
					doorstoptimer = timer + (p.doorstoptime SEC); /* start door stop timer timer			*/
					doorstopstate |= doorstoppush;				/* save new state of door stop function */
					set_door (doorstopstate, OPEN_DOOR);	/* send door command				*/
					doorstate &= ~doorstopstate;			/* mark doors as open 			*/
					bFunc_flag &=	~FUNC_DOORSTOP_TIMEOUT;
					if(p.landingcall_push & PUSH_ALARM)
						set_out(SPEAKER_BUZ, BUZZER_NORMAL, 0, 0, 0, O_CANA | O_CANB | O_HSE);
				}
				else										/* no new doors to stop 			*/
					doorstopstate = 0;						/* switch off door stop state 		*/
			}
			doorstoppush = 0; 							/* to be done 					*/
		}
}

BYTE she_parkdoor(void){
	if((!(p.cooperate_sel1 & COOP1_THROUGH_DOOR)) || (!she_calldoor_double))
		return 1;

	return 0;
}

/****************************************************************************************************/
/* Standstill in normal mode																		*/
/****************************************************************************************************/
void standstill_state (void)
{
	DWORD carlighttimer;
	DWORD doorclosetimer = 0;
	DWORD occupiedtimer;
	DWORD doordelaytimer = 0;
	DWORD anti_faint_timer = 0;
	DWORD dlshortchktimer = 0;
	WORD doorstaytime_cc;
	WORD doorstaytime_lc;
	WORD doorstaytime_nc;
	BYTE call;
	BYTE doornew;
	BYTE doorstaystate = D_STAYTIME;
	BYTE doormode;
	BYTE doorclosepush_old;
	BYTE doorclosepush_act;
	BYTE forced_door_cl = 0;
	BYTE lcmode_actfloor;
	BYTE handle_landingcalls = 1;
	BYTE rel_active = 2;
	BYTE i;
	BYTE anti_faint_output = 0;
	DWORD cl_op_timer = 0;
	BYTE doorcommand = 0;
	DWORD shutoff_delay = 0;
	BYTE speaker_fg = 0;
	BYTE door = 0;
	BYTE shutoff_power = 0;
	BYTE check_all_DL = 0;
	BYTE check_door_short = 0;
	BYTE ulsi_out = 0;
	BYTE ulsi_on_enable = 0;
	BYTE ulsi_off_enable = 0;
	BYTE main_relay_status = 0;

	diving_fail_cnt = 0;
	start_test_trip = 0;
	target_floor = level;
	overload = 0;
	fullload = 0;
	zeroload = 0;
	bTestDL = 0;
	dl_short_test_finish = 0;			//ÿ���ڽ������ģʽ�����¼�������̽�
	carlight_switched_off = 0;
	send_hse_state ();								/* send new state on CAN bus					*/
	calls_off = 0;									/* enable calls								*/
	doorclosepush_old = 0;							/* save state of door close push				*/
	if(((callpriority == C_EVACUATION) && (p.evac_fire_carlightmode & 0x01))
				|| ((callpriority == C_FIREALARM) && (p.evac_fire_carlightmode & 0x02)))
		carlighttimer = timer + (p.carlight_fan_time_fire SEC);			/* start car light off timer	*/
	else
		carlighttimer = timer + (p.carlighttimer SEC);			/* start car light off timer	*/

	anti_faint_timer = timer + (p.antifainttime SEC);
	dlshortchktimer = timer + 1 SEC;
	if(!(p.landingcall_push & PUSH_STOP_SHIP))
		{
			doorstoppush = 0;
			doorstopstate = 0;
		}
	doorstoppush_sheopen_flag = 0;
	doorstoppush_sheclose_flag = 0;
	if (level == p.main_floor)
	{
		doorstaytime_cc = p.doorstaytime_main_cc;
		doorstaytime_lc = p.doorstaytime_main_lc;
		doorstaytime_nc = p.doorstaytime_main_nc;
	}
	else
	{
		doorstaytime_cc = p.doorstaytime_cc;
		doorstaytime_lc = p.doorstaytime_lc;
		doorstaytime_nc = p.doorstaytime_nc;
	}
	if (landingcalldir)								/* floor reached by landing call					*/
		ct = timer + (doorstaytime_lc SEC);				/* Timer for door open stay time with land.call	*/
	else											/* floor reached by car call or without call		*/
		ct = timer + (doorstaytime_cc SEC);			/* Timer for door open stay time with car call	*/
	occupiedtimer = timer + 3 SEC;					/* delay timer after occupied state				*/
	fault_signal = 0;								/* reset fault signal							*/
	out_of_order = 0;								/* reset out of order indication					*/
	if ((door_state [0] == DOOR_STOPPED) ||			/* door in stopped state						*/
		(door_state [1] == DOOR_STOPPED))
	{
		if ((!firedoormode) || (p.fire_doorclosemode == FIREDOOR_AUTO))
		{
			set_door (0x07, CLOSE_DOOR);			/* close all doors								*/
			doorstaystate = D_CLOSING;				/* door open mode: wait until door is closed		*/
			doorclosetimer = timer + 60 SEC;			/* max. waiting time until door is closed		*/
			doordelaytimer = timer + p.max_door_close_group SEC;
			forced_door_cl = forced_closing ();		/* check if forced door closing necessary		*/
		}
	}
	if (car_attendance)								/* car attendance operation					*/
	{
		door_attendance = 1;						/* don't close door automatically				*/
		lcmode_actfloor = LCM_NO_REOPEN;			/* landing call mode: no reopen				*/
	}
	else
	{
		door_attendance = 0;						/* close door automatically					*/
		lcmode_actfloor = p.lcmode_actfloor;			/* landing call mode like parameter				*/
	}
	hallcall_bypass = 0;							/* switch off the hall call bypass function		*/
	if ((insp_return) && (!number_of_calls (1, ALL_CALLS)))
	{
		if ((!p.relevel) || (!(door_zone & (POS_SGU | POS_SGD))))
		{
			i = 0;
			if ((level < p.top_floor) && (level >= p.bot_floor))
			{
				for (i = level + 1; i <= p.top_floor; ++i)
				{
					if (p.doorpos [i])
					{
						calltab[i].calltype = CARCALL;
						calltab[i].cc_door = p.doorpos [i];
						break;
					}
				}
			}
			if ((level == p.top_floor || i > p.top_floor) && (level > p.bot_floor))
			{
				for (i = level - 1; i >= p.bot_floor; --i)
				{
					if (p.doorpos [i])
					{
						calltab[i].calltype = CARCALL;
						calltab[i].cc_door = p.doorpos [i];
						break;
					}
					if (i == p.bot_floor)
						break;
				}
			}
		}
		insp_return = 0;
	}
#if SRU_OLD
	if (!p.relevel)
#endif
	{//�µ�20/50��׼������״̬��K2�Ȳ�����
		if (state_ulsi != SWITCH_OFF)
			ulsi_off_timer = timer + 1 SEC;
		SetSwitch(SWITCH_ULSI, SWITCH_OFF);
	}
	if((floor_record_fg[level] == 0) && (p.simulation != SIMULATION_AUTO))
		{
			floor_record_fg[level] = 1;
			floor_higth[level] = nice5000_Rstore_position;
		}
	while (1)
	{
		cycle ();									/*do all cyclic functions						*/
		set_state ();								/*set new state								*/
		if (hse_state != H_STANDSTILL)				/* state changed								*/
		{
			if (carlight_switched_off)				/* car light is switched off						*/
			{
				if (!light_switch_status)
					set_out (CARLIGHT, 0, 0, 0, 1, (O_CANA|O_CANB|O_HSE));
				carlight_switched_off = 0;
			}
			start_test_trip = 0;
			if (dooropenpush_ind)
			{
				set_out (SPECIAL_FUNC, DOOR_OPEN, 0, EXISTING_DOORS, 0, O_CANA);
				dooropenpush_ind = 0;
			}
			if (doorclosepush_ind)
			{
				set_out (SPECIAL_FUNC, DOOR_CLOSE, 0, EXISTING_DOORS, 0, O_CANA);
				doorclosepush_ind = 0;
			}
			if (doorstop_ind)
			{
				set_out (SPECIAL_FUNC, DOOR_STOP, 0, EXISTING_DOORS, 0, O_CANA | O_CANB);
				doorstop_ind = 0;
				doorstopstate = 0;
			}
			overload = 0;							/* clear overload								*/
			if (anti_faint_output)
			{
				set_out (SPECIAL_FUNC, ANTI_FAINT_OUTPUT, 0, 0, 0, (O_CANA|O_CANB|O_HSE));
				anti_faint_output = 0;
			}
			dl_short_test_finish = 0;
			
#if	SRU_NEW
			if(main_relay_status)
				{
					SetSwitch(SWITCH_MAIN_RELAY, SWITCH_OFF);
					out ^= 1;
				}
#endif

			return;							/* go to new state							*/
		}

		special_trip ();								/* handle special trips							*/
		if (doorstaystate == D_CLOSING)				/* all doors are closing						*/
			doormode = ALL_DOORS_CLOSED;		/* mark all doors as already closed				*/
		else										/* doors are not in closing state				*/
			doormode = doorstate;					/* use real door state for call function			*/

		call = get_call (landingcalldir, level, doormode, handle_landingcalls);
		if ((call != NO_CALL) ||						/* new call to handle							*/
				dooropenpush ||							/* or door open push pressed					*/
				doorstopstate)							/* or door stop push activew					*/
			parkingtimer = timer + (p.parkingtime SEC);	/* restart parking trip timer					*/
		for (i = 0; i < p.doornumber; i++)
		{
			if (door_state [i] == DOOR_CLOSED)		/* if door is closed							*/
				doorcloseerrorcount [i] = 0;			/* reset close error count						*/
		}
		switch (call)
		{
			case (NO_CALL):						/* no call									*/
				if (doorstate == ALL_DOORS_CLOSED)	/* all doors closed						*/
					landingcalldir = 0;				/* clear landingcall direction					*/
				break;

			case (NEW_CALL):						/* call from other floor						*/
				if ((doorstate == ALL_DOORS_CLOSED) &&	(she_parkdoor()) &&	(!rel_active))	/* all doors closed					*/
				{
					if (carlight_switched_off)		/* car light is off								*/
					{
						if (!light_switch_status)
							set_out (CARLIGHT, 0, 0, 0, 1, (O_CANA|O_CANB|O_HSE));
						carlight_switched_off = 0;
					}
					if (dooropenpush_ind)
					{
						set_out (SPECIAL_FUNC, DOOR_OPEN, 0, EXISTING_DOORS, 0, O_CANA);
						dooropenpush_ind = 0;
					}
					if (doorclosepush_ind)
					{
						set_out (SPECIAL_FUNC, DOOR_CLOSE, 0, EXISTING_DOORS, 0, O_CANA);
						doorclosepush_ind = 0;
					}
					if (doorstop_ind)
					{
						set_out (SPECIAL_FUNC, DOOR_STOP, 0, EXISTING_DOORS, 0, O_CANA | O_CANB);
						doorstop_ind = 0;
						doorstopstate = 0;
					}
					hse_state = H_START;			/* start a new trip							*/
					start_test_trip = 0;
					move_dooropen_flag = 0;
					if (p.controltype)				/* single call control (not collecting)			*/
					{							/* lift not occupied before						*/
						if (!occupied)
						{						/* Disable landing calls (occupied)				*/
							occupied = 1;
							set_out (SPECIAL_FUNC, OCCUPIED, 0, 0, 1, (O_CANA|O_CANB|O_HSE));
						}						/* set special indication						*/
					}
					if (anti_faint_output)
					{
						set_out (SPECIAL_FUNC, ANTI_FAINT_OUTPUT, 0, 0, 0, (O_CANA|O_CANB|O_HSE));
						anti_faint_output = 0;
					}
					overload = 0;					/* clear overload								*/
					if(((door_state[0] == DOOR_OPENED) || ((door_state[1] == DOOR_OPENED) && (p.doornumber > 1)))
							&& (door_close_open == 8))
						{
							set_out (SPECIAL_FUNC, DOOR_OPEN, 0, EXISTING_DOORS, 0, O_CANA);
							door_close_open = 0;
							cl_op_fg = 0;
						}
					else if(((door_state[0] == DOOR_CLOSED) && (door_close_open & 1))
								|| ((door_state[1] == DOOR_CLOSED) && (door_close_open & 2)))
						{
							set_out (SPECIAL_FUNC, DOOR_CLOSE, 0, EXISTING_DOORS, 0, O_CANA);
							door_close_open = 0;
							cl_op_fg = 0;
						}
					
#if	SRU_NEW
					if(main_relay_status)
						{
							CLOSE_MAIN_RELAY();
							main_relay_status = 0;
							out ^= 1;
						}
#endif

					return;
				}
				else if(doorstate != ALL_DOORS_CLOSED)
					{
						if((doorstate & DOOR1) == 0)
							move_dooropen_flag = 1;
						else if((doorstate & DOOR2) == 0)
							move_dooropen_flag = 2;
					}
				break;

			case (CALL_ACT_FLOOR):				/* call from actual floor						*/
				doornew = calldoors;				/* open all called doors						*/
				if (!landingcalldir)
				{
					if (calltype == LANDINGCALL_UP)			/* process landing call up						*/
						landingcalldir = LANDINGCALL_UP;	/* set new landing call direction				*/
					else if (calltype == LANDINGCALL_DN)		/* process landing call down					*/
						landingcalldir = LANDINGCALL_DN;	/* set new landing call direction				*/
					else if (calltype == LANDINGCALL)		/* process landing call without direction			*/
						landingcalldir = LANDINGCALL;		/* set new landing call direction				*/
				}
				doornew &= p.doorpos [target_floor];			/* only open existing doors					*/
				if((p.cooperate_sel1 & COOP1_THROUGH_DOOR) &&
						(p.doorpos [target_floor] == (DOOR1 | DOOR2)))
					{
						door = doornew;		//����֮ǰ���Ų���
						she_calldir = landingcalldir >> 1;
						if(doorstate != ALL_DOORS_CLOSED)
							{
								if((doorstate == (DOOR2 | DOOR3)) && (doornew & DOOR1))
									doornew = DOOR1;
								else if((doorstate == (DOOR1 | DOOR3)) && (doornew & DOOR2))
									doornew = DOOR2;
								else
									doornew = 0;
							}
						else
							{
								if(doornew == (DOOR1 | DOOR2))
									doornew = DOOR1;
								else if(doornew & DOOR1)
									doornew = DOOR1;
								else if(doornew & DOOR2)
									doornew = DOOR2;
								else
									doornew = 0;
							}
						if(door)
							she_calldoor_double |= ((door & (~doornew)) % (DOOR1 | DOOR2));
						else
							she_calldoor_double = 0;

					}
				if ((callpriority != C_STANDARD) &&			/* priority mode								*/
						(doorstate != ALL_DOORS_CLOSED)	&&		/* and not all doors closed						*/
						(!((~doorstate) & calldoors)))				/* and wrong doors open						*/
					doornew = 0;							/* ignore call until doors are closed				*/

				if (doornew)								/* doors to open								*/
				{
					if ((!door_off) &&						/* no door opening off						*/
					((!firedoormode) || (p.fire_dooropenmode == FIREDOOR_AUTO)))
					{
						doorstaystate = D_STAYTIME;			/* door open mode: minimum open stay time	*/
						if (landingcalldir)					/* floor reached by landing call					*/
							ct = timer + (doorstaytime_lc SEC);	/* Timer for door open stay time with land.call	*/
						else								/* floor reached by car call or without call		*/
							ct = timer + (doorstaytime_cc SEC);	/* Timer for door open stay time with car call	*/
						set_door (doornew, OPEN_DOOR);	/* open new doors							*/
						doorstate &= ~doornew;				/* mark doors as open						*/
						if((p.custumer1 & CUSTUMER1_SHE_MODE) && (p.doorpos [target_floor] == (DOOR1 | DOOR2)))
							{
								set_out (CAR_CALL, level + 1, 0, doornew, 0, O_CANA);		//����ָʾ��
								if(she_calldir == DIR_UP)
									i = HALL_CALL_UP;
								else if(she_calldir == DIR_DN)
									i = HALL_CALL_DOWN;
								else
									i = HALL_CALL_UP;
								set_out (HALL_CALL, i, level + 1, doornew, 0, O_CANB);

								calltab [level].calltype &= ~calltype;
							}
					}
				}
				break;
		}

		if (doorstate == ALL_DOORS_CLOSED)				/* all doors closed							*/
			doorstaystate = D_CLOSED;						/* door stay mode: door is closed				*/
		switch (doorstaystate)
		{
			case (D_STAYTIME):							/* door is open, wait open stay time			*/
				if (ct < timer)								/* door open stay time is over					*/
				{
					if(landingcall_push && (p.landingcall_push & PUSH_NOT_OPEN))
						{
							ct = timer + 1;
							landingcalldir = 0; 					/* clear landing call direction 				*/
						}
					else
						{
							doorstaystate = D_HANDLE_CALLS;		/* handle calls								*/
							doorclosetimer = timer + (doorstaytime_nc SEC);
							landingcalldir = 0;						/* clear landing call direction					*/
						}
				}
				else if ((ct-timer) > (doorstaytime_cc SEC))
				{										/* open stay time with landing calls				*/
					if ((door_state [0] == DOOR_OPENING) ||	/* door is opening							*/
					(door_state [1] == DOOR_OPENING))
						ct = timer + (doorstaytime_lc SEC);		/* restart door open stay timer					*/
				}
				else if ((ct-timer) <= (doorstaytime_cc SEC))
				{										/* or open stay time with landing calls over		*/
					if (dooropenpush ||					/* door open push still pressed					*/
					(door_state [0] == DOOR_OPENING) ||	/* door is opening							*/
					(door_state [1] == DOOR_OPENING))
						ct = timer + (doorstaytime_cc SEC);	/* restart door open stay timer					*/
				}
				if ((lcmode_actfloor == LCM_NO_CALL) &&		/* landing call mode: don't handle actual floor	*/
				(call != CALL_ACT_FLOOR))					/* no call from actual floor						*/
					handle_landingcalls = 0;					/* don't handle landing calls from actual floor	*/
				else
					handle_landingcalls = 1;					/* handle landing calls from actual floor			*/

				if(calldir != calldir_old)
					{
						if(p.clear_cc_opp_dir && (calldir != DIR_NULL) && callpriority == C_STANDARD)
							{//ֻ����ͨ����������
								if(calldir_old == DIR_UP)
									{
										for(i=p.bot_floor; i<level; i++)
											{
												if(calltab[i].calltype & CARCALL)
													{
														set_out (CAR_CALL, i + 1, 0, calltab[i].cc_door, 0, O_CANA | O_HSE);
														calltab[i].calltype &= ~CARCALL;
														calltab[i].cc_door = 0;
													}
											}
									}
								else if(calldir_old == DIR_DN)
									{
										for(i=level; i<p.top_floor; i++)
											{
												if(calltab[i].calltype & CARCALL)
													{
														set_out (CAR_CALL, i + 1, 0, calltab[i].cc_door, 0, O_CANA | O_HSE);
														calltab[i].calltype &= ~CARCALL;
														calltab[i].cc_door = 0;
													}
											}
									}
							}
						calldir_old = calldir;
					}
				break;

			case (D_HANDLE_CALLS):						/* door is open, handle calls					*/
				if (((call == NEW_CALL) && (she_parkdoor())) ||					/* call from other floor						*/
				((call == CALL_ACT_FLOOR)	 &&				/* call from actual floor						*/
				(callpriority != C_STANDARD) &&			/* and priority mode							*/
				(doorstate != ALL_DOORS_CLOSED)	&&		/* and not all doors closed						*/
				(!((~doorstate) & calldoors))))				/* and wrong doors open						*/
				{
					if ((carcallcounter > p.cc_no_ls) &&		/* more calls without light screen then allowed	*/
					(callpriority == C_STANDARD))
					{
						clearcalls (CARCALL);
						write_errorhist (E_LIGHT_SCREEN, HSE_ID, 0, W_LIGHT_SCREEN);
						carcallcounter = 0;					/* reset counter								*/
					}
					else
					{
						if (((!firedoormode) || (p.fire_doorclosemode == FIREDOOR_AUTO)) &&
						((!door_attendance) || (callpriority != C_STANDARD) || (p.attend_carcall & ATTEND_DOOR_MODE)))
						{
							set_door (~doorstate, CLOSE_DOOR);		/* close all not already closed doors		*/
							doorstaystate = D_CLOSING;				/* door open mode: wait until door is closed	*/
							doordelaytimer = timer + p.max_door_close_group SEC;
							doorclosetimer = timer + 60 SEC;			/* max. waiting time until door is closed	*/
							forced_door_cl = forced_closing ();		/* check if forced door closing necessary	*/
						}
					}
				}
				else if (doorclosetimer < timer)				/* door stay time without new call over			*/
				{
					if (testtrip && (!start_test_trip))			/* test trip mode								*/
					{
						start_test_trip = 1;
						testtimer = timer + test_interval SEC;
						doorclosetimer = timer + 1 SEC;		/* wait for test trip call						*/
					}
					else if (!set_park_state()) 			/* close command for all doors					*/
					{
						doorstaystate = D_CLOSING;			/* door open mode: wait until door is closed		*/
						doorclosetimer = timer + 60 SEC;		/* max. waiting time until door is closed		*/
						doordelaytimer = timer + p.max_door_close_group SEC;
						forced_door_cl = forced_closing ();	/* check if forced door closing necessary		*/
					}
					else
						doorclosetimer = timer + 1 SEC;
					handle_landingcalls = 1;					/* handle landing calls from actual floor			*/
				}
				break;

			case (D_CLOSING):							/* door is closing								*/
				i = forced_closing ();						/* check if forced door closing necessary		*/
				if (i != forced_door_cl)						/* state changed								*/
				{
					forced_door_cl = i;						/* save state									*/
					doorclosetimer = 0;						/* reset timer to send new door command		*/
				}
				if (overload)								/* overload									*/
				{
					doorstaystate = D_STAYTIME;				/* door open mode: minimum open stay time	*/
					ct = timer + (doorstaytime_cc SEC);
					set_door ((~doorstate) & EXISTING_DOORS, OPEN_DOOR);
				}
				else if (doorclosetimer < timer)				/* max. waiting time until door is closed over	*/
				{										/* send new door command for all doors		*/
					if ((!firedoormode) || (p.fire_doorclosemode == FIREDOOR_AUTO))
						door_command (CLOSE_DOOR, CLOSE_DOOR);
					doorclosetimer = timer + 60 SEC;			/* restart waiting time							*/
				}
				landingcalldir = 0;							/* clear landing call direction	(again!)			*/
				if ((lcmode_actfloor != LCM_REOPEN) &&		/* no landing call mode: reopen				*/
				(call == NEW_CALL))						/* and no call from other floor					*/
					handle_landingcalls = 0;					/* don't handle landing calls from actual floor	*/
				else
					handle_landingcalls = 1;					/* handle landing calls from actual floor			*/
				if (call != NEW_CALL)						/* not closing by new call						*/
					doordelaytimer = timer + p.max_door_close_group SEC;
				else if (p.max_door_close_group &&
				(doordelaytimer < timer))						/* max. waitingtime for group control over		*/
				{
					doordelaytimer = timer + p.max_door_close_group SEC;
					if (doordelay < 250)					/* increase door delay for group control			*/
						doordelay += 5;
				}
				break;

			case (D_CLOSED):								/* all doors are closed						*/
				if (doorstate != ALL_DOORS_CLOSED)		/* not all doors closed						*/
				{
					doorstaystate = D_STAYTIME;				/* door open mode: minimum open stay time	*/
					ct = timer + (doorstaytime_cc SEC);		/* Timer for door open stay time with car call	*/
				}
				else if (call != NEW_CALL)
				{
					handle_landingcalls = 1;					/* handle landing calls from actual floor			*/
					if (ct < timer)							/* door stay time over							*/
					{
						if (testtrip && (!start_test_trip))		/* test trip mode								*/
						{
							start_test_trip = 1;
							ct = timer + 1 SEC;				/* wait for test trip call						*/
							testtimer = timer + test_interval SEC;
						}
						else
						{
							i = set_park_state ();			/* search parking doors						*/
							if (i && (i != 0xFF))				/* doors to open								*/
							{
								doorstaystate = D_STAYTIME;	/* door open mode: minimum open stay time	*/
								ct = timer + (doorstaytime_cc SEC);
								doorstate &= ~i;			/* mark doors as open						*/
							}
						}
					}
				}
				if (safety_circuit & SC3)						/* door and safety circuit shaft door closed		*/
					doordelay = 0;						/* reset door delay for group control			*/

				break;
		}

		if (carlight_switched_off &&
			((callpriority != C_EVACUATION) || !(p.evac_fire_carlightmode & 0x01))&&
			((callpriority != C_FIREALARM) || !(p.evac_fire_carlightmode & 0x02))) 	/* car light is switched off						*/
		{
			if ((doorstate != ALL_DOORS_CLOSED) &&		/* not all doors closed						*/
			((!remote_switched_off) ||						/* and lift not remote switched off				*/
			(!(p.remote_off_func & REMOTE_LIGHT))))							/* or remote off but light should stay on			*/
			{
				if (!light_switch_status)
					set_out (CARLIGHT, 0, 0, 0, 1, (O_CANA|O_CANB|O_HSE));
				carlight_switched_off = 0;					/* save new state								*/
			}
		}
		else if (callpriority == C_STANDARD)					/* car light is not already switched off			*/
		{												/* and standard call priority					*/
			if (((doorstate == ALL_DOORS_CLOSED) &&		/* all doors closed							*/
				p.carlighttimer &&							/* and switch off car light enabled				*/
				(carlighttimer < timer) &&						/* and car light timer over						*/
				(!remote_switched_off))					/* and lift is not remote switched off			*/
			|| (remote_switched_off &&						/* or lift is remote switched off					*/
				(p.remote_off_func & REMOTE_LIGHT)))							/* and light should go off when remote off		*/
			{
				if (!light_switch_status)
					set_out (CARLIGHT, 0, 0, 0, 0, (O_CANA|O_CANB|O_HSE));			/* switch car light off (relay on)				*/
				carlight_switched_off = 1;					/* save new state								*/
			}
		}
		else if(((callpriority == C_EVACUATION) && (p.evac_fire_carlightmode & 0x01))
					|| ((callpriority == C_FIREALARM) && (p.evac_fire_carlightmode & 0x02)))
			{
				if(p.carlighttimer && 						/* and switch off car light enabled 			*/
					(carlighttimer < timer))						/* and car light timer over 					*/
					{
						if (!light_switch_status && carlight_flag)
							{
								carlight_flag = 0;
								set_out (CARLIGHT, 0, 0, 0, 0, (O_CANA|O_CANB|O_HSE));			/* switch car light off (relay on)				*/
								carlight_switched_off = 1;					/* save new state 							*/
							}
					}
			}

		if (doorstate != ALL_DOORS_CLOSED)				/* not all doors closed						*/
			{
				if(((callpriority != C_EVACUATION) || ((p.evac_fire_carlightmode & 0x01) == 0))
						&& ((callpriority != C_FIREALARM) || ((p.evac_fire_carlightmode & 0x02) == 0)))
					carlighttimer = timer + (p.carlighttimer SEC);			/* retrigger timer								*/
			}
		// sua nudging
		//if((firecall) && (!firekey)) {
			//clearcalls (CARCALL | PRIOR_CARCALL);	
			//clearcalls (ALL_HALL_CALLS);
		
		if(callpriorityold != callpriority)
		{
			Nudging_timer_count = (0-1);
			callpriorityold == callpriority;
		}
		if(level != p.fire_floor[0])
		{
				if(!(callpriority == C_FIREALARM)) // C_FIREALARM
				{
					NudingMode = 0;
					Nudging_timer_count = 0-1;
					Nudging_Buz_timer_count = 0-1;
				}
				else
				{
					if((Nudging_timer_count == (0-1)) && (door_state[0] != DOOR_CLOSED) )
					{
						Nudging_timer_count = timer+ 15 SEC;
					}
				}
				if( timer > Nudging_timer_count  )
				{
					//over 2min
					Nudging_timer_count = 0-1;
					set_door(ALL_DOORS_CLOSED,CLOSE_DOOR); // close door
					//out nudging
					set_out (DOOR_IO, DOOR_REV, 0, EXISTING_DOORS,1 , O_CANA);  
					set_out (SPEAKER_BUZ, BUZZER_FIRE, 0, EXISTING_DOORS, 1 , O_CANA);   //buzzer on
					set_out (SPECIAL_FUNC, DOOR_OPEN, 0, EXISTING_DOORS, 0, O_CANA);  //turn off open led
					set_out (SPECIAL_FUNC, DOOR_CLOSE, 0, EXISTING_DOORS, 1, O_CANA); //turn on open led
					NudingMode = 1;
					Enabal_opendoor =0;
					Nudging_Buz_timer_count = timer +4 SEC;
				}
				if( timer > Nudging_Buz_timer_count  )
				{
					set_out (SPEAKER_BUZ, BUZZER_FIRE, 0, EXISTING_DOORS, 1 , O_CANA);
					Nudging_Buz_timer_count = timer +4 SEC;
				}
		}
		
		if(NudingMode)
		{
			if(callpriority != C_FIREALARM) //C_FIREALARM
				{
						Enabal_opendoor =1;
						Nudging_Buz_timer_count = 0-1;
						NudingMode = 0;
						set_out (DOOR_IO, DOOR_REV, 0, EXISTING_DOORS,0 , O_CANA); 
						set_out (SPEAKER_BUZ, BUZZER_FIRE, 0, EXISTING_DOORS, 0 , O_CANA); 
				}
		}
		if (handle_dooropenpush ())							/* handle door open push						*/
		{
			doorstaystate = D_STAYTIME;
			if ((ct < timer) ||								/* door open stay time already over				*/
			((ct - timer) <= (doorstaytime_cc SEC)))			/* or open stay time with landing calls over		*/
				ct = timer + (doorstaytime_cc SEC);			/* Timer for door open stay time with car call	*/
		}
		if(!Enabal_opendoor)
		{
			if(timer > Nudging_opendoor_timer_count)
				{
					Nudging_opendoor_timer_count = 0-1;
					Enabal_opendoor = 1;
				}
		}
		//sua nudging
		handle_doorstoppush ();
		if ((doorstopstate) || (she_photonsensor) || (she_doorstoppush))
		{
			doorstaystate = D_STAYTIME;
			if ((ct < timer) ||								/* door open stay time already over				*/
			((ct - timer) <= (doorstaytime_cc SEC)))			/* or open stay time with landing calls over		*/
				ct= timer + (doorstaytime_cc SEC);			/* Timer for door open stay time with car call	*/
			if (!doorstop_ind)
			{
				set_out (SPECIAL_FUNC, DOOR_STOP, 0, EXISTING_DOORS, 1, O_CANA | O_CANB);
				doorstop_ind = 1;
			}
		}
		else if (doorstop_ind)
		{
			set_out (SPECIAL_FUNC, DOOR_STOP, 0, EXISTING_DOORS, 0, O_CANA | O_CANB);
			doorstop_ind = 0;
		}
		if(!(p.landingcall_push & PUSH_STOP_SHIP))
			doorclosepush_act = doorclosepush & (~dooropenpush) & (~doorstopstate) & p.doorpos [level];
		else
			doorclosepush_act = doorclosepush & (~dooropenpush) & (~doorstoppush) & p.doorpos [level];
		if ((callpriority == C_FIREMAN) && (!firekey) && (!auto_fire))
					doorclosepush_act = 0;							/* ignore door close push 					*/
		if (doorclosepush_act != doorclosepush_old)				/* door close push changed					*/
		{
			if (doorclosepush_act)							/* doors to close								*/
			{
				if (!overload)								/* no overload								*/
				{
					if ((!(p.lift_func_sel1 & LIFTFUN1_PRESS_CLOSEBUTTOM))
							|| (door_state [0] == DOOR_OPENED)
							|| (door_state [1] == DOOR_OPENED))
						set_door (doorclosepush, CLOSE_DOOR);	/* send door command						*/
				}
				door_attendance = 0;
			}
			else if (firedoormode)
			{
				if (p.fire_doorclosemode == FIREDOOR_STOP)
					fire_door (doorclosepush_old, STOP_DOOR);	/* stop door									*/
				else if (p.fire_doorclosemode == FIREDOOR_REVERSE)
					fire_door (doorclosepush_old, OPEN_DOOR);	/* reopen door								*/
			}
			doorclosepush_old = doorclosepush_act;				/* save old state of door close push				*/
		}
		if (p.controltype)									/* single call control (not collecting)			*/
		{
			if ((doorstaystate == D_CLOSED) &&				/* all doors closed							*/
			(call == NO_CALL))								/* and no other call							*/
			{
				if (occupiedtimer < timer)					/* call delay time over (finish occupied state)	*/
				{
					if (occupied)							/* lift occupied before							*/
					{
						occupied = 0;
						set_out (SPECIAL_FUNC, OCCUPIED, 0, 0, 0, (O_CANA|O_CANB|O_HSE));
					}									/* reset special indication						*/
				}
			}
			else
			{
				occupiedtimer = timer + 3 SEC;				/* delay timer after occupied state				*/
				if (!occupied)								/* lift not occupied before						*/
				{
					occupied = 1;
					set_out (SPECIAL_FUNC, OCCUPIED, 0, 0, 1, (O_CANA|O_CANB|O_HSE));
				}										/* set special indication						*/
			}
		}
		if (doorstate != ALL_DOORS_CLOSED)				/* not all doors closed						*/
		{
			if ((load & OVERLOAD) && (!bTestLoad))		/* set overload state							*/
				overload = 1;
			else
				overload = 0;
			if (load & FULL_LOAD)							/* set full load state							*/
				fullload = 1;
			else
				fullload = 0;
			if (load & ZERO_LOAD)							/* set zero load state							*/
			{
				if (p.max_cc_empty &&						/* watch maximum number of car calls			*/
				(callpriority == C_STANDARD) &&
				number_of_calls (p.max_cc_empty + 1, CARCALL))
					clearcalls (CARCALL);
				zeroload = 1;
			}
			else
				zeroload = 0;
			anti_faint_timer = timer + (p.antifainttime SEC);
			if (anti_faint_output)
			{
				anti_faint_output = 0;
				set_out (SPECIAL_FUNC, ANTI_FAINT_OUTPUT, 0, 0, 0, (O_CANA|O_CANB|O_HSE));
			}
		}
		else
		{
			if (p.antifainttime)
			{
				if ((!zeroload) && (!(load & HALF_LOAD)) && (!overload) && (!fullload) && (anti_faint_timer < timer))
				{
					if (!anti_faint_output)
					{
						anti_faint_output = 1;
						set_out (SPECIAL_FUNC, ANTI_FAINT_OUTPUT, 0, 0, 1, (O_CANA|O_CANB|O_HSE));
					}
				}
				else
				{
					if (anti_faint_output)
					{
						anti_faint_output = 0;
						set_out (SPECIAL_FUNC, ANTI_FAINT_OUTPUT, 0, 0, 0, (O_CANA|O_CANB|O_HSE));
					}
				}
			}
		}

		if (check_dl_short && (!door_short_conn))
		{//������·�Ķ̽Ӽ��
			if((door_state [0] == DOOR_OPENED) || (door_state [1] == DOOR_OPENED))
				{//ֻ�ڿ���״̬��������̽�
					if(p.door_connect)
						{
							i = SC3;
							door = SC2;
						}
					else
						{
							i = SC2;
							door = SC3;
						}
					if((dl_shorted & i) || (safety_circuit & i))
						{
							if (!(general_error & G_DL_SHORTED))
								{
									general_error |= G_DL_SHORTED;
									dl_shorted = safety_circuit & i;
									write_errorhist(E_DL_SHORTED_CAR, HSE_ID, 0, 0);
								}
						}
					if(!check_door_short)
						{//����ƽ�㹦��
							if(!ulsi_on_enable)
								{
									dlshortchktimer = timer + 5;		//
									ulsi_on_enable = 1;
								}
							if((state_ulsi == SWITCH_OFF) && (dlshortchktimer < timer) && (!ulsi_out) && ulsi_on_enable)
								{
									if (door_zone & POS_SGM)
										{
											SetSwitch(SWITCH_ULSI, SWITCH_ON);
											dlshortchktimer = timer + 6;
											ulsi_off_enable = 1;
											ulsi_out = 1;
										}
								}
#if	SRU_NEW
							else if((ulsi_out == 1) && (dlshortchktimer < timer))
								{
									ulsi_out = 2;	
									dlshortchktimer = timer + 1 SEC;
									SetSwitch(SWITCH_MAIN_RELAY, SWITCH_ON);
									main_relay_status = 1;
									out |= 1;
								}
#endif
							else if((state_ulsi == SWITCH_ON) && (dlshortchktimer < timer) && (ulsi_off_enable) && (ulsi_out == 2) && main_relay_status)
								{
									if (door_zone & POS_SGM)
										{
//											if(!p.relevel)
//												{
//													if (state_ulsi != SWITCH_OFF)
//														ulsi_off_timer = timer + 1 SEC;
//													SetSwitch(SWITCH_ULSI, SWITCH_OFF);
//												}
//											dlshortchktimer = timer + 1 SEC;
//											check_door_short = 1; 	//�Ѿ���⵽�̽ӣ����
//											dl_short_test_finish = 1;
											
#if	SRU_NEW											
											SetSwitch(SWITCH_MAIN_RELAY, SWITCH_OFF);
											main_relay_status = 0;
											out ^= 1;
											ulsi_out = 3;											
											dlshortchktimer = timer + 5;
										}
								}
							else if((ulsi_out == 3) && (!main_relay_status) && (dlshortchktimer < timer))
								{
//									if(!p.relevel)
										{
											if (state_ulsi != SWITCH_OFF)
												ulsi_off_timer = timer + 1 SEC;
											SetSwitch(SWITCH_ULSI, SWITCH_OFF);
										}
//									dlshortchktimer = timer + 1 SEC;
									check_door_short = 1; 	//�Ѿ���⵽�̽ӣ����
									dl_short_test_finish = 1;								
								}
#endif	
						}
				}
		}

#if	SRU_NEW
//		if((p.relevel) && dl_short_test_finish)
#endif
			rel_active = relevelling (rel_active, call);					/* re-levelling function						*/
		if (firedoormode)
		{
			fan_on ();
			if ((!dooropenpush_ind) && (p.fire_dooropenmode != FIREDOOR_AUTO))
			{
				dooropenpush_ind = 1;
				set_out (SPECIAL_FUNC, DOOR_OPEN, 0, EXISTING_DOORS, 1, O_CANA);
			}
			if ((!doorclosepush_ind)&& (p.fire_doorclosemode != FIREDOOR_AUTO))
			{
				doorclosepush_ind = 1;
				set_out (SPECIAL_FUNC, DOOR_CLOSE, 0, EXISTING_DOORS, 1, O_CANA);
			}
		}
		else
		{
			if (dooropenpush_ind)
			{
				set_out (SPECIAL_FUNC, DOOR_OPEN, 0, EXISTING_DOORS, 0, O_CANA);
				dooropenpush_ind = 0;
			}
			if (doorclosepush_ind)
			{
				set_out (SPECIAL_FUNC, DOOR_CLOSE, 0, EXISTING_DOORS, 0, O_CANA);
				doorclosepush_ind = 0;
			}
			if(door_close_open & 0x03)
				{
					if(!cl_op_fg)
						{
							set_out (SPECIAL_FUNC, DOOR_OPEN, 0, EXISTING_DOORS, 0, O_CANA);
							cl_op_timer = mstimer + 3;	//15ms �ٿ����Ű�ť��
							cl_op_fg = 1;
						}
					else if((cl_op_timer < mstimer) && (cl_op_fg == 1))
						{
							set_out (SPECIAL_FUNC, DOOR_CLOSE, 0, EXISTING_DOORS, 1, O_CANA);
							cl_op_fg = 0xAA;
						}
				}
			else if(door_close_open == 8)
				{
					if(!cl_op_fg)
						{
							set_out (SPECIAL_FUNC, DOOR_CLOSE, 0, EXISTING_DOORS, 0, O_CANA);
							cl_op_timer = mstimer + 3;
							cl_op_fg = 1;
						}
					else if((cl_op_timer < mstimer) && (cl_op_fg == 1))
						{
							set_out (SPECIAL_FUNC, DOOR_OPEN, 0, EXISTING_DOORS, 1, O_CANA);
							cl_op_fg = 0xAA;
						}
				}
			if(((door_state[0] == DOOR_OPENED) || ((door_state[1] == DOOR_OPENED) && (p.doornumber > 1)))
					&& (door_close_open == 8))
				{
					set_out (SPECIAL_FUNC, DOOR_OPEN, 0, EXISTING_DOORS, 0, O_CANA);
					door_close_open = 0;
					cl_op_fg = 0;
				}
			else if(((door_state[0] == DOOR_CLOSED) && (door_close_open & 1))
						|| ((door_state[1] == DOOR_CLOSED) && (door_close_open & 2)))
				{
					set_out (SPECIAL_FUNC, DOOR_CLOSE, 0, EXISTING_DOORS, 0, O_CANA);
					door_close_open = 0;
					cl_op_fg = 0;
				}
		}
		if (car_attendance)
			{
				if(!calldir)						/* no call direction							*/
					door_attendance = 1;							/* don't close door automatically 			*/
				doordelay = 250;
			}
		if ((calls_enabled & LANDINGCALL_UP) &&				/* in up peak traffic set new call to park floor*/
		(up_peak_time || up_peak_input)&&
		(level != p.up_peak_parkfloor) &&
		(!(calltab [p.up_peak_parkfloor].calltype & LANDINGCALL_UP)))
			calltab [p.up_peak_parkfloor].calltype |= LANDINGCALL_UP;

//�ж�ǿ���ź�
	if (level == p.bot_floor)
		{
			if (door_zone & POS_SDD1)
				sdd1_unwork_times = 0;
			else
				{
					if (check_sdd1_working_status)
						{
							check_sdd1_working_status = 0;
							++ sdd1_unwork_times;
							if (sdd1_unwork_times >= C_SDS_UNWORK_CHECKTIMES)
								{
									poserror |= P_VU_NOT_ON;
									write_errorhist (E_VO_VU_ERROR, HSE_ID, C_VO_VU_ON, W_VO_VU_ON);
								}
						}
				}
		}
	else if (level == p.top_floor)
		{
			if (door_zone & POS_SDU1)
				sdu1_unwork_times = 0;
			else
				{
					if (check_sdu1_working_status)
						{
							check_sdu1_working_status = 0;
							++ sdu1_unwork_times;
							if (sdu1_unwork_times >= C_SDS_UNWORK_CHECKTIMES)
								{
									poserror |= P_VO_NOT_ON;
									write_errorhist (E_VO_VU_ERROR, HSE_ID, C_VO_VU_ON, W_VO_VU_ON);
								}
						}
				}
		}
	else
		{
			if (door_zone & (POS_SDU1 | POS_SDD1))
				{
					if (check_sds_working_status)
						{
							check_sds_working_status = 0;
							++ sds_unwork_times;
							if (sds_unwork_times >= C_SDS_UNWORK_CHECKTIMES)
								{
									poserror |= P_VO_VU_ERROR;
									write_errorhist (E_VO_VU_ERROR, HSE_ID, C_VO_VU_ON, W_VO_VU_ON);
								}
						}
				}
			else
				sds_unwork_times = 0;
		}

		if(emp_power)
			{//Ӧ����Դ����
				if (((door_zone & POS_SGM) && (level == emp_floor)) ||
						((poserror) ||									/* lift in level zone or positioning error	*/
						(doorcommand))) 								/* or already doorcommand 		*/
					{
						if (!doorcommand) 					/* no door command					*/
							{
								if (p.doornumber == 1)				/* lift has only 1 car door 			*/
									door = p.doorpos [level] & 0x01;
								else								/* lift has more than 1 car door		*/
									{
										if (level != 0xFF)				/* lift did already start counting		*/
											door = p.doorpos [level];
										else
											{
												for (i = 0; i < p.doornumber; i++)
													bit_set (door, i);
											}
									}
								if(door)
									{
										doorcommand = 1;
										level_display (level, O_ALL);
										set_door (door, OPEN_DOOR); 			/* open doors 								*/
										set_out (SPECIAL_FUNC, UPS_EVAC_READY_SPEAKER, 0, 0, 1, (O_CANA|O_CANB|O_HSE)); 	//The car display will show "Overload", which is not good
										shutoff_delay = timer + 30 SEC; 	//Turn off UPS power after 30S
									}
							}
						else
							{
								if(shutoff_delay < timer + 15 SEC)
									{//The buzzer will beep for 15 seconds and then turn off
										if(!speaker_fg)
											{
												speaker_fg = 1;
												set_out (SPECIAL_FUNC, UPS_EVAC_READY_SPEAKER, 0, 0, 0, (O_CANA|O_CANB|O_HSE)); 	//The car display will show "Overload", which is not good
											}
									}
								if ((shutoff_delay < timer) && ((door_state[0] == DOOR_OPENED) || (door_state[1] == DOOR_OPENED)))
									{
										if (!shutoff_power)
											{
												shutoff_power = 1;
												set_out (SPECIAL_FUNC, UPS_EVAC_READY, 0, 0, 1, O_HSE); 	//Wait for 1 minute to turn off the UPS output
											}
									}
							}
					}
			}

	if((p.cooperate_sel1 & COOP1_THROUGH_DOOR) &&
			(p.doorpos [level] == (DOOR1 | DOOR2)))
		{
			switch(she_calldoor_double)
				{
					case (DOOR1 | DOOR2):
						if(doorstaystate == D_STAYTIME)
							{//���Ŵ���
								door = she_calldoor_double & p.she_doornumber;
								if(!door)
									door = DOOR1;
								she_calldoor_double = (she_calldoor_double - door) % (DOOR1 | DOOR2);
								set_door (door, OPEN_DOOR); 	/* close all not already closed doors 	*/
								doorstate &= ~door;
								set_out (CAR_CALL, level + 1, 0, door, 0, O_CANA);		//����ָʾ��
								if(she_calldir == DIR_UP)
									i = HALL_CALL_UP;
								else if(she_calldir == DIR_DN)
									i = HALL_CALL_DOWN;
								else
									i = HALL_CALL_UP;
								set_out (HALL_CALL, i, level + 1, door, 0, O_CANB);
							}
						break;

					case DOOR1:
					case DOOR2:
						if((doorstaystate == D_CLOSED) || (doorstate == ALL_DOORS_CLOSED))
							{
								if(she_calldoor_double == DOOR1)
									{
										set_door (DOOR1, OPEN_DOOR);		/* close all not already closed doors 	*/
										doorstate &= ~DOOR1;
									}
								else
									{
										set_door (DOOR2, OPEN_DOOR);		/* close all not already closed doors 	*/
										doorstate &= ~DOOR2;
									}
								doorstaystate = D_STAYTIME; 			/* door open mode: minimum open stay time */
								ct = timer + (doorstaytime_cc SEC); 	/* Timer for door open stay time with car call	*/
								set_out (CAR_CALL, level + 1, 0, she_calldoor_double, 0, O_CANA);
								if(she_calldir == DIR_UP)
									i = HALL_CALL_UP;
								else if(she_calldir == DIR_DN)
									i = HALL_CALL_DOWN;
								else
									i = HALL_CALL_UP;
								set_out (HALL_CALL, i, level + 1, she_calldoor_double, 0, O_CANB);
								she_calldoor_double = 0;
								she_calldir = 0;
							}
						break;
				}
		}

		if ((check_dl_short & bTestDL) && (level == p.bot_floor) && (!check_all_DL))
			{
				for(i = 0; i < MAX_FLOOR; i++)
					{
						if (p.doortable [adt][i] & p.doorpos [i])
							{
								calltab [i].calltype |= CARCALL;	/* set new car call 					*/
								calltab [i].cc_door = p.doortable [adt][i] & p.doorpos [i];
							}
					}
				check_all_DL = 1;
				landings_off = 1;
				p.relevel = 0;			//�ֶ����Ե�ʱ�����ȡ����ƽ�㹦��
			}
	}
}


