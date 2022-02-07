
#include "typedef.h"
#include "parameter.h"
#include "state.h"
#include "call.h"
#include "can.h"
#include "drive.h"
#include "pos.h"
#include "rtc.h"
#include "history.h"
#include "eeprom.h"
#include "io.h"
#include "temperature.h"
#include "load.h"
#include "test.h"
#include "nice5000.h"

/****************************************************************************************************/
/* read HSE inputs																		*/
/****************************************************************************************************/
const DWORD HSE_MARIN[] =
{
	0x00ul,
	0x01ul, 0x03ul, 0x07ul, 0x0ful,
	0x1ful, 0x3ful, 0x7ful, 0xfful,
	0x1fful, 0x3fful, 0x7fful, 0xffful,
	0x1ffful, 0x3ffful, 0x7ffful, 0xfffful,
	0x1fffful, 0x3fffful, 0x7fffful, 0xffffful,
	0x1ffffful, 0x3ffffful, 0x7ffffful, 0xfffffful,
	0x1fffffful, 0x3fffffful, 0x7fffffful, 0xffffffful,
	0x1ffffffful, 0x3ffffffful, 0x7ffffffful, 0xfffffffful,
};


void read_hse_inputs (void)
{
	BYTE help[2];
	BYTE i, j;
	DWORD instate;

	instate = in;
	if ((instate != oldin) || (in_polarity != oldin_polarity))
	{
		for (i = 0; i < MAX_HSE_IN + MAX_HSE_EX_IN; i++)
		{
			if ((bit_select (instate, i) != bit_select (oldin, i)) ||
			(bit_select (in_polarity, i) != bit_select (oldin_polarity, i)))
			{
				if (i < MAX_HSE_IN)
				{
					if (p.hse_inpar [i][IO_BASIC_FUNC] && (p.hse_inpar [i][IO_ENABLE] == ENABLE))
					{
						for (j = 0; j < 5; j++)
							virt_in [j] = p.hse_inpar [i][j];
						help[0] = bit_select (in_polarity, i) ^ bit_select (instate, i);
						help[1] = p.hse_inpar [i][IO_ENABLE];
						virt_in [IO_STATE] = help[0];
						handle_input (LIFT1, 1);					/* handle input					*/
						switch (p.hse_inpar [i][IO_BASIC_FUNC])
						{
							case (LOAD_IN):
								transmit_load (load);			/* transmit state of load inputs		*/
								break;

							case (HALL_CALL):
							case (HALL_CALL_SPECIAL):			// special hall call
							case (HALL_CALL_ADVANCED):			// advanced hall call
							case (HALL_CALL_EMERGENCY): 		// emergency hall call
								if (p.groupsize > 1)
									set_hallcall_c();
								transmit_inputs (p.hse_inpar [i][IO_BASIC_FUNC], p.hse_inpar [i][IO_SUB_FUNC], help[0]);
								break;

							default:							/* transmit state of input			*/
								transmit_inputs (p.hse_inpar [i][IO_BASIC_FUNC], p.hse_inpar [i][IO_SUB_FUNC], help[0]);
								break;
						}
					}
				}
				else
				{
					if (p.hse_ex_inpar [i-MAX_HSE_IN][IO_BASIC_FUNC] && (p.hse_ex_inpar [i-MAX_HSE_IN][IO_ENABLE] == ENABLE))
					{
						for (j = 0; j < 5; j++)
							virt_in [j] = p.hse_ex_inpar [i-MAX_HSE_IN][j];
						help[0] = bit_select (in_polarity, i) ^ bit_select (instate, i);
						help[1] = p.hse_ex_inpar [i-MAX_HSE_IN][IO_ENABLE];
						virt_in [IO_STATE] = help[0];
						handle_input (LIFT1, 1);					/* handle input					*/
						switch (p.hse_ex_inpar [i-MAX_HSE_IN][IO_BASIC_FUNC])
						{
							case (LOAD_IN):
								transmit_load (load);			/* transmit state of load inputs		*/
								break;

							case (HALL_CALL):
							case (HALL_CALL_SPECIAL):			// special hall call
							case (HALL_CALL_ADVANCED):			// advanced hall call
							case (HALL_CALL_EMERGENCY): 		// emergency hall call
								if (p.groupsize > 1)
									set_hallcall_c();
								transmit_inputs (p.hse_ex_inpar [i-MAX_HSE_IN][IO_BASIC_FUNC], p.hse_ex_inpar [i-MAX_HSE_IN][IO_SUB_FUNC], help[0]);
								break;

							default:							/* transmit state of input			*/
								transmit_inputs (p.hse_ex_inpar [i-MAX_HSE_IN][IO_BASIC_FUNC], p.hse_ex_inpar [i-MAX_HSE_IN][IO_SUB_FUNC], help[0]);
								break;
						}
					}
				}
			}
		}
		oldin = in;
		oldin_polarity = in_polarity;
	}
}

/****************************************************************************************************/
/*	check if input/output is car call or hall call												*/
/****************************************************************************************************/
BYTE check_for_call (BYTE value)
{
	switch (value)
	{
		case (HALL_CALL):									/* standard hall call				*/
		case (HALL_CALL_SPECIAL):						/* special hall call				*/
		case (HALL_CALL_ADVANCED):						/* advanced hall call				*/
		case (HALL_CALL_EMERGENCY):						/* emergency hall call				*/
		case (CAR_CALL):
			return (1);									/* car call						*/

		default:
			return (0);									/* all other values				*/
	}
}
	
/****************************************************************************************************/
/* set HSE outputs																		*/
/****************************************************************************************************/
void hse_out (BYTE basic, BYTE sub, BYTE floor, BYTE doors, BYTE state)
{
	BYTE i;
	BYTE iotype;

	iotype = basic;
	if (check_for_call (iotype))								/* car call, hall call or priority call	*/
	{
		if (iotype == HALL_CALL)								/* read floor number				*/
			i = floor;
		else
			i = sub;
		if (i == 0xFF)										/* clear all calls					*/
		{
			for (i = 0; i < MAX_HSE_OUT; i++)					/* search output parameter list		*/
			{
				if (p.hse_outpar [i][IO_BASIC_FUNC] == iotype)
				{
					bit_reset (out, i);
				}
			}
			for (i = 0; i < MAX_HSE_EX_OUT; i++)					/* search output parameter list		*/
			{
				if (p.hse_ex_outpar [i][IO_BASIC_FUNC] == iotype)
				{
					bit_reset (out, (i+MAX_HSE_OUT));
				}
			}
		}
		else												/* set or reset single call			*/
		{
			for (i = 0; i < MAX_HSE_OUT; i++)					/* search output parameter list		*/
			{
				if (basic == p.hse_outpar [i][IO_BASIC_FUNC])	/* virtual output matches with physical	*/
					if (sub == p.hse_outpar [i][IO_SUB_FUNC])
						if (floor == p.hse_outpar [i][IO_FLOOR])
							if (!((~doors) & (p.hse_outpar [i][IO_DOOR] & 0x0F)))
							{
								if (state & 0x01)			/* set acknowledgement			*/
								{
									bit_set (out, i);
								}
								else						/* reset acknowledgement			*/
								{
									bit_reset (out, i);			/* clear physical output			*/
								}
							}
			}
			for (i = 0; i < MAX_HSE_EX_OUT; i++)					/* search output parameter list		*/
			{
				if (basic == p.hse_ex_outpar [i][IO_BASIC_FUNC])	/* virtual output matches with physical	*/
					if (sub == p.hse_ex_outpar [i][IO_SUB_FUNC])
						if (floor == p.hse_ex_outpar [i][IO_FLOOR])
							if (!((~doors) & (p.hse_ex_outpar [i][IO_DOOR] & 0x0F)))
							{
								if (state & 0x01)			/* set acknowledgement			*/
								{
									bit_set (out, (i+MAX_HSE_OUT));
								}
								else						/* reset acknowledgement			*/
								{
									bit_reset (out, (i+MAX_HSE_OUT));			/* clear physical output			*/
								}
							}
			}
		}
	}
	else if ((iotype == LIGHT_FUNC) &&
			(sub & (DIRECTION_IND_DN | DIRECTION_IND_UP | HALL_LANTERN_UP | HALL_LANTERN_DN)))
	{
		for (i = 0; i < MAX_HSE_OUT; i++)						/* search output parameter list		*/
		{
			if (basic == p.hse_outpar [i][IO_BASIC_FUNC])
			{
				if (((sub & p.hse_outpar [i][IO_SUB_FUNC] & 0x0F) || (!p.hse_outpar [i][IO_SUB_FUNC])) &&
				((floor == p.hse_outpar [i][IO_FLOOR]) || (floor == 0xFF) || (!p.hse_outpar [i][IO_FLOOR])) &&
				((doors & p.hse_outpar [i][IO_DOOR]) || (!p.hse_outpar [i][IO_DOOR])))
				{
					if (state & 0x01)
					{
						bit_set (out, i);
					}
					else
					{
						bit_reset (out, i);
					}
				}
				else										/* reset all not matching indications*/
				{
					bit_reset (out, i);
				}
			}
		}
		for (i = 0; i < MAX_HSE_EX_OUT; i++)						/* search output parameter list		*/
		{
			if (basic == p.hse_ex_outpar [i][IO_BASIC_FUNC])
			{
				if (((sub & p.hse_ex_outpar [i][IO_SUB_FUNC] & 0x0F) || (!p.hse_ex_outpar [i][IO_SUB_FUNC])) &&
				((floor == p.hse_ex_outpar [i][IO_FLOOR]) || (floor == 0xFF) || (!p.hse_ex_outpar [i][IO_FLOOR])) &&
				((doors & p.hse_ex_outpar [i][IO_DOOR]) || (!p.hse_ex_outpar [i][IO_DOOR])))
				{
					if (state & 0x01)
					{
						bit_set (out, (i+MAX_HSE_OUT));
					}
					else
					{
						bit_reset (out, (i+MAX_HSE_OUT));
					}
				}
				else										/* reset all not matching indications*/
				{
					bit_reset (out, (i+MAX_HSE_OUT));
				}
			}
		}
	}
	else if (iotype == ARRIVAL_INDICATION)					/* arrival indication not possible on HSE		*/
	{
	}
	else if(iotype == SPEAKER_BUZ)		//����������
		{
		}
	else
	{
		for (i = 0; i < MAX_HSE_OUT; i++)						/* search output parameter list		*/
		{
			if (basic == p.hse_outpar [i][IO_BASIC_FUNC])
				if ((sub == p.hse_outpar [i][IO_SUB_FUNC]) || (!sub))
					if (floor == p.hse_outpar [i][IO_FLOOR])
						if ((doors & p.hse_outpar [i][IO_DOOR]) || (!p.hse_outpar [i][IO_DOOR]))
						{
							if (state & 0x01)
							{
								bit_set (out, i);
							}
							else
							{
								bit_reset (out, i);				/* reset physical output			*/
							}
						}
		}
		for (i = 0; i < MAX_HSE_EX_OUT; i++)						/* search output parameter list		*/
		{
			if (basic == p.hse_ex_outpar [i][IO_BASIC_FUNC])
				if ((sub == p.hse_ex_outpar [i][IO_SUB_FUNC]) || (!sub))
					if (floor == p.hse_ex_outpar [i][IO_FLOOR])
						if ((doors & p.hse_ex_outpar [i][IO_DOOR]) || (!p.hse_ex_outpar [i][IO_DOOR]))
						{
							if (state & 0x01)
							{
								bit_set (out, (i+MAX_HSE_OUT));
							}
							else
							{
								bit_reset (out, (i+MAX_HSE_OUT));				/* reset physical output			*/
							}
						}
		}
	}
}

/****************************************************************************************************/
/* get evacuation floor in case of fire alarm													*/
/****************************************************************************************************/
BYTE get_firealarmfloor (BYTE firefloor_old)
{
	BYTE i, j;
	BYTE floor;
	BYTE fire;

	if ((firefloor_old != 0xFF) && (!(p.firealarm_function & FIREALARM_DYNAMIC)) && firealarmfloor_ind)
		return (firefloor_old);								/* no dynamic fire alarm floor		*/
	if (hse_state == H_MOVING)							/* during moving use next possible floor	*/
		floor = next_floor;
	else if (hse_state == H_ARRIVAL)				/* during arrival use target floor	*/
		floor = target_floor;
	else
		floor = level;
	for (i = 0; i <= (p.top_floor - p.bot_floor); i++)
	{
		if (!bit_selectd (fireinput [p.fire_floor [i] / 32], p.fire_floor [i] % 32))
		{/* fire alarm input in this floor not on		*/
			if (p.firealarm_function & FIREALARM_CROSS_FIREFLOOR)								/* possible to cross already burning floor	*/
				return (p.fire_floor [i]);						/* go to this floor							*/
			else											/* check if burning floor on the way			*/
			{
				fire = 0;
				if (floor < p.fire_floor [i])						/* lift below fire alarm floor		*/
				{
					for (j = floor + 1; j < p.fire_floor [i]; j++)
					{
						if (bit_selectd (fireinput [j / 32], j % 32))
							fire = 1;
					}
					if (!fire)								/* no burning floor on the way		*/
						return (p.fire_floor [i]);				/* go to this floor					*/
				}
				else										/* lift above or in fire alarm floor	*/
				{
					for (j = floor; j > (p.fire_floor [i] + 1); j--)
					{
						if (bit_selectd (fireinput [(j - 1) / 32], (j - 1) % 32))
							fire = 1;
					}
					if (!fire)								/* no burning floor on the way		*/
						return (p.fire_floor [i]);				/* go to this floor					*/
				}
			}
		}
	}
	return (p.fire_floor [p.bot_floor]);
}

/****************************************************************************************************/
/* check if forced door closing is necessary													*/
/****************************************************************************************************/
BYTE forced_closing (void)
{
	if (hse_state == H_DOORTEST)
		return (0);	
	if ((((calltype == EMERGENCY_CALL) || (callpriority == C_EMERGENCY)) && p.forced_close_emerg) ||
			(callpriority == C_FIREMAN) ||
			((callpriority == C_FIREALARM) && (p.firealarm_function & FIREALARM_FORCE_CLOSE)))
		return (1);
	return (0);
}

/****************************************************************************************************/
/* check if start enabled																	*/
/****************************************************************************************************/
BYTE start_enabled (void)
{
	if ((evacuation) && (!evacuation_normal))
	{
		if ((!evacuation_ready) &&
		(evacuationdelay < timer) && 
		((evacuation_enable | evacuation_key) || ((evacuationtimer < timer) && p.evacuationtime)))
			return (1);
		else
			return (0);
	}
	return (1);
}

BYTE check_up_down_peak(void){
//ȫ���л��������	
	BYTE peak = 0;			//bit0: up   bit1: down
	DWORD up_start_time1, up_start_time2, up_start_time3;
	DWORD up_stop_time1, up_stop_time2, up_stop_time3;
	DWORD dn_start_time1, dn_start_time2, dn_start_time3;
	DWORD dn_stop_time1, dn_stop_time2, dn_stop_time3;
	DWORD active_time;
	
	up_start_time1 = (DWORD)p.up_peak_start_h1 * 3600 + (DWORD)p.up_peak_start_m1 * 60;
	up_stop_time1 = (DWORD)p.up_peak_stop_h1 * 3600 + (DWORD)p.up_peak_stop_m1 * 60;
	up_start_time2 = (DWORD)p.up_peak_start_h2 * 3600 + (DWORD)p.up_peak_start_m2 * 60;
	up_stop_time2 = (DWORD)p.up_peak_stop_h2 * 3600 + (DWORD)p.up_peak_stop_m2 * 60;
	up_start_time3 = (DWORD)p.up_peak_start_h3 * 3600 + (DWORD)p.up_peak_start_m3 * 60;
	up_stop_time3 = (DWORD)p.up_peak_stop_h3 * 3600 + (DWORD)p.up_peak_stop_m3 * 60;

	dn_start_time1 = (DWORD)p.dn_peak_start_h1 * 3600 + (DWORD)p.dn_peak_start_m1 * 60;
	dn_stop_time1 = (DWORD)p.dn_peak_stop_h1 * 3600 + (DWORD)p.dn_peak_stop_m1 * 60;
	dn_start_time2 = (DWORD)p.dn_peak_start_h2 * 3600 + (DWORD)p.dn_peak_start_m2 * 60;
	dn_stop_time2 = (DWORD)p.dn_peak_stop_h2 * 3600 + (DWORD)p.dn_peak_stop_m2 * 60;
	dn_start_time3 = (DWORD)p.dn_peak_start_h3 * 3600 + (DWORD)p.dn_peak_start_m3 * 60;
	dn_stop_time3 = (DWORD)p.dn_peak_stop_h3 * 3600 + (DWORD)p.dn_peak_stop_m3 * 60;

	active_time = (DWORD)t.hour * 3600 + (DWORD)t.minute * 60;

	if(((active_time >= up_start_time1) && (active_time <= up_stop_time1)) || 
			((active_time >= up_start_time2) && (active_time <= up_stop_time2)) ||
			((active_time >= up_start_time3) && (active_time <= up_stop_time3)))
		peak |= 0x01;	
	
	if(((active_time >= dn_start_time1) && (active_time <= dn_stop_time1)) || 
			((active_time >= dn_start_time2) && (active_time <= dn_stop_time2)) ||
			((active_time >= dn_start_time3) && (active_time <= dn_stop_time3)))
		peak |= 0x02;	
	
	return peak;
}

/****************************************************************************************************/
/* handle priority states																	*/
/****************************************************************************************************/
void set_call_priority (void){
	BYTE handle;
	BYTE enabled;
	BYTE up_dn_peak = 0;
	static BYTE firekey_old = 0;
	static BYTE firekeystate = 0;		
	static BYTE firemodestate = 0;		
	static BYTE firecall_old = 0;
	static BYTE special_key_old = 0;
	static BYTE advanced_key_old = 0;
	static BYTE emergency_key_old = 0;
	static BYTE firealarmfloor_old = 0xFF;
	static BYTE firealarm_old = 0;	
	static BYTE firecallstate = 0;	
	static BYTE firecallready = 0;	
	static BYTE firekeymisuse = 0;	
	static BYTE emp_power_state = 0;
	
	enabled = ALL_CALLS;									/* enable all calls					*/
	handle = ALL_CALLS;									/* handle all calls					*/
	if ((hse_state != H_STANDSTILL) ||						/* not in standstill state			*/
			(callpriority != C_STANDARD) ||							/* or no standard call priority		*/
			(correct_level))
		parkingtimer = timer + (p.parkingtime SEC);				/* restart parking trip timer		*/

	if (firecall || firekey || auto_fire)							/* fire key in car or fireman floor is on	*/
		firemode = 1;										/* set fireman mode				*/

	if (firealarmreturn)										/* input to return from fire alarm is on	*/
		firealarmmode = 0;
	else if (firealarm || ((p.firealarm_function & FIREALARM_ACTIVE_DETECT) && (fireinput [0] || fireinput [1])))
		firealarmmode = 1;
	else if (p.firealarm_function & FIREALARM_RESET_MODE)	/* automatic back to normal mode	*/
		firealarmmode = 0;

	if (!firealarmmode)									/* no fire alarm					*/
		firealarmfloor_old = 0xFF;								/* reset fire alarm evacuation floor	*/

	if (firemode)											/* fireman trip key is on			*/
		callpriority = C_FIREMAN;
	else if (firealarmmode)									/* fire alarm is on				*/
		callpriority = C_FIREALARM;
	else if (evacuation && (!evacuation_normal))				/* evacuation is on				*/
		callpriority = C_EVACUATION;
	else if ((emergency_key) && (callpriority <= C_EMERGENCY))	/* emergency key is on and no higher priority	*/
		callpriority = C_EMERGENCY;						/* set emergency call mode		*/
	else if ((advanced_key) && (callpriority <= C_ADVANCED))	/* emergency key is on and no higher priority	*/
		callpriority = C_ADVANCED;							/* set advanced call mode			*/
	else if ((special_key) && (callpriority <= C_SPECIAL))		/* special key is on and no higher priority		*/
		callpriority = C_SPECIAL;							/* set special call mode			*/

	if (fullload || hallcall_bypass)							/* car is full or hall call bypass input on			*/
		handle &= ~ALL_HALL_CALLS;						/* ignore landing calls			*/
	if (callpriority > C_STANDARD)
		handle &= ~(ALL_HALL_CALLS | CARCALL);			/* dont handle hall and standard car calls		*/
	if (callpriority >= C_SPECIAL)
		handle &= ~SPECIAL_CALL;							/* dont handle special calls		*/
	if (callpriority >= C_ADVANCED)
		handle &= ~ADVANCED_CALL;						/* dont handle advanced calls		*/
	if (callpriority >= C_EMERGENCY)
		handle &= ~EMERGENCY_CALL;						/* dont handle emergency calls		*/
	if (overload)											/* overload is on					*/
		handle = 0;										/* don't handle calls				*/
	if ((limit_switch_error) ||								/* lift was in limit switch			*/
			(temperature & (OVERTEMP1 | OVERTEMP2 | ROOMTEMP_MIN | ROOMTEMP_MAX)) ||
			(!carlight) || (steel_broke))											/* car light is off 				*/
		enabled = 0;										/* disable all hall calls			*/
	adt = DOOR_NORMAL;									/* select door table for normal mode	*/
	adt_hall = DOOR_N_HALL;
	if (timer2_trip)
		{
			adt = DOOR_TIMER_2;								/* select door table for timer 2 mode	*/
			adt_hall = DOOR_T2_HALL;
		}
	else if (timer1_trip || timer_trip)
		{
			adt = DOOR_TIMER_1;								/* select door table for timer 1 mode	*/
			adt_hall = DOOR_T1_HALL;
		}
	if (callpriority == C_STANDARD)							/* standard call mode				*/
	{
		if (intermediate_door ||							/* intermediate door is open		*/
				car_switched_off ||								/* lift is switched off				*/
				earthquake ||									/* earthquake input is on			*/
				auto_call ||										/* auto call mode ("Sabbat control")	*/
				limitstatus ||
				remote_off)										/* lift is remote switched off		*/
			enabled = 0;									/* disable all calls				*/
		if (occupied)										/* lift is occupied					*/
			{
				enabled &= ~ALL_HALL_CALLS;					/* disable landing calls			*/
				if (number_of_calls (1, CARCALL))				/* already car call				*/
					enabled &= ~CARCALL;						/* also disable car calls			*/
			}
		if (intermediate_door)								/* intermediate door is open		*/
			{
				if (number_of_calls (1, ALL_CALLS))				/* clear all calls					*/
					clearcalls (ALL_CALLS);
			}
		if (((p.timertrip_start < p.timertrip_stop) &&
				(t.hour >= p.timertrip_start) && (t.hour < p.timertrip_stop))||
				((p.timertrip_start > p.timertrip_stop) &&
				((t.hour >= p.timertrip_start) || (t.hour < p.timertrip_stop))))
			timer_trip = 1;
		else
			timer_trip = 0;
		enabled &= ~PRIOR_CARCALL;						/* disable priority car calls			*/

//���Ӹ߷����е�ʱ���ж�   2017-02-14
		if((p.up_peak_start_h1 == 0) && (p.up_peak_start_m1 == 0) &&
				(p.up_peak_start_h2 == 0) && (p.up_peak_start_m2 == 0) &&
				(p.up_peak_start_h3 == 0) && (p.up_peak_start_m3 == 0) &&
				(p.up_peak_stop_h1 == 0) && (p.up_peak_stop_m1 == 0) &&
				(p.up_peak_stop_h2 == 0) && (p.up_peak_stop_m2 == 0) &&
				(p.up_peak_stop_h3 == 0) && (p.up_peak_stop_m3 == 0) &&
				(p.dn_peak_start_h1 == 0) && (p.dn_peak_start_m1 == 0) &&
				(p.dn_peak_start_h2 == 0) && (p.dn_peak_start_m2 == 0) &&
				(p.dn_peak_start_h3 == 0) && (p.dn_peak_start_m3 == 0) &&
				(p.dn_peak_stop_h1 == 0) && (p.dn_peak_stop_m1 == 0) &&
				(p.dn_peak_stop_h2 == 0) && (p.dn_peak_stop_m2 == 0) &&
				(p.dn_peak_stop_h3 == 0) && (p.dn_peak_stop_m3 == 0))
			{
				up_peak_time = 0;
				dn_peak_time = 0;
			}
		else
			{
				up_dn_peak = check_up_down_peak();
				up_peak_time = up_dn_peak & 0x01;
				dn_peak_time = up_dn_peak & 0x02;
			}
		if (up_peak_time || up_peak_input)
			enabled &= ~LANDINGCALL_DN;					/* disable down landing calls		*/
		else if (dn_peak_time || dn_peak_input)
			enabled &= ~LANDINGCALL_UP;					/* disable up landing calls			*/

//����Ӧ����Դ�ĳ��빦��(����ϵ�Դ���磬���ݲ�ֹͣ)		
		if(emp_power)
			{
				enabled = 0;
				if(!emp_power_state)
					{
						clearcalls (ALL_CALLS);						
						if((emp_floor >= p.bot_floor) && (emp_floor <= p.top_floor))
							{
								calltab [emp_floor].calltype |= PRIOR_CARCALL;		/* write call to call table 		*/
								calltab [emp_floor].cc_door |= (emp_doors & p.doorpos [emp_floor]);
							}
						emp_power_state = 1;
					}
			}
		else
			emp_power_state = 0;
//����Ӧ����Դ�ĳ��빦��(����ϵ�Դ���磬���ݲ�ֹͣ)		

	}
	if (callpriority == C_SPECIAL)							/* special call mode				*/
	{
		if (special_key != special_key_old)					/* state of special key changed		*/
		{
			if (special_key)								/* special key is now on			*/
			{
				if (p.carcall_special & 0x01)					/* cancel car calls when key switched on	*/
					clearcalls (CARCALL);
				if (p.hallcall_special)							/* clear hall calls in special mode	*/
					clearcalls (ALL_HALL_CALLS);
				callenabledtimer = timer + (p.calltime_special SEC);
			}
			else											/* special key is now off			*/
			{
				if (p.carcall_special & 0x02)					/* cancel car calls when key switched off	*/
					clearcalls (CARCALL);
			}
		}
		if (special_key ||									/* special key on					*/
		(drive_state != D_STOPPED) ||				/* or all doors closed				*/
		(door_state [0] == DOOR_OPENING) ||				/* or door is opening				*/
		(door_state [1] == DOOR_OPENING))
		{/* retrigger timer					*/
			if (callprioritytimer < (timer + (p.time_back_special SEC)))
				callprioritytimer = timer + (p.time_back_special SEC);
		}
		if ((callenabledtimer >= timer) &&					/* calls enabled					*/
		((door_state [0] == DOOR_OPENING) ||				/* and door is opening			*/
		(door_state [1] == DOOR_OPENING)))					/* retrigger timer until door is opened 	*/
			callenabledtimer = timer + (p.calltime_special SEC);
		if (!p.normal_cc_special)								/* normal car calls not enabled		*/
			enabled &= ~CARCALL;							/* disable car calls				*/
		if ((p.call_active_special && (!special_key))	||			/* calls enabled only if special key is on		*/
		(p.calltime_special && (callenabledtimer < timer)))		/* or only for a time enabled and time out	*/
			enabled &= ~PRIOR_CARCALL;					/* disable priority car calls			*/
		else
			adt = DOOR_SPECIAL;							/* select door table for special mode		*/
		if (p.hallcall_special)
			enabled &= ~ALL_HALL_CALLS;
		if ((lastcallfloor == level) &&							/* lift is in floor of last call		*/
		(lastcalltype == SPECIAL_CALL) &&					/* last call was special call			*/
		(lastcalldoors & priorstate [0][level] & ~doorstate & 0x0007))
		{
			enabled &= ~CARCALL;							/* disable car calls				*/
			if (callprioritytimer < (timer + (p.time_back_special SEC)))
				callprioritytimer = timer + (p.time_back_special SEC);
		}
	}
	if (callpriority == C_ADVANCED)							/* advanced call mode			*/
	{
		if (advanced_key != advanced_key_old)				/* state of advanced key changed	*/
		{
			if (advanced_key)								/* advanced key is now on			*/
			{
				if (p.carcall_advanced & 0x01)					/* cancel car calls when key switched on	*/
					clearcalls (CARCALL);
				if (p.hallcall_advanced)						/* clear hall calls in advanced mode	*/
					clearcalls (ALL_HALL_CALLS);
				callenabledtimer = timer + (p.calltime_advanced SEC);
			}
			else											/* advanced key is now off		*/
			{
				if (p.carcall_advanced & 0x02)					/* cancel car calls when key switched off	*/
					clearcalls (CARCALL);
			}
		}
		if (advanced_key ||													/* advanced key on				*/
		(drive_state != D_STOPPED) ||								/* or all doors closed				*/
		(door_state [0] == DOOR_OPENING) ||				/* or door is opening				*/
		(door_state [1] == DOOR_OPENING))
		{/* retrigger timer					*/
			if (callprioritytimer < (timer + (p.time_back_advanced SEC)))
				callprioritytimer = timer + (p.time_back_advanced SEC);
		}
		if ((callenabledtimer >= timer) &&					/* calls enabled					*/
		((door_state [0] == DOOR_OPENING) ||				/* and door is opening			*/
		(door_state [1] == DOOR_OPENING)))					/* retrigger timer until door is opened 	*/
			callenabledtimer = timer + (p.calltime_advanced SEC);
		if (!(p.normal_cc_advanced & VIPRUN_CARCALL_ENABLE))							/* normal car calls not enabled		*/
			enabled &= ~CARCALL;							/* disable car calls						*/
		if ((p.call_active_advanced && (!advanced_key))	||		/* calls enabled only if advanced key is on	*/
		(p.calltime_advanced && (callenabledtimer<timer)))		/* or only for a time enabled and time out	*/
			enabled &= ~PRIOR_CARCALL;					/* disable priority car calls			*/
		else
			adt = DOOR_ADV;								/* select door table for advanced mode		*/
		if (p.hallcall_advanced)
			enabled &= ~ALL_HALL_CALLS;
		if ((lastcallfloor == level) &&							/* lift is in floor of last call		*/
		(lastcalltype == ADVANCED_CALL) &&					/* last call was advanced call		*/
		(lastcalldoors & priorstate [1][level] & ~doorstate & 0x0007))
		{
			enabled &= ~CARCALL;							/* disable car calls				*/
			if (callprioritytimer < (timer + (p.time_back_advanced SEC)))
				callprioritytimer = timer + (p.time_back_advanced SEC);
		}
	}
	if (callpriority == C_EMERGENCY)						/* emergency call mode			*/
	{
		if (emergency_key != emergency_key_old)				/* state of emergency key changed	*/
		{
			if (emergency_key)							/* emergency key is now on		*/
			{
				if (p.carcall_emergency & 0x01)				/* cancel car calls when key switched on	*/
					clearcalls (CARCALL);
				if (p.hallcall_emergency)						/* clear hall calls in emergency mode		*/
					clearcalls (ALL_HALL_CALLS);
				callenabledtimer = timer + (p.calltime_emergency SEC);
			}
			else											/* emergency key is now off		*/
			{
				if (p.carcall_emergency & 0x02)				/* cancel car calls when key switched off	*/
					clearcalls (CARCALL);
			}
		}
		if (emergency_key ||								/* emergency key on				*/
		(drive_state != D_STOPPED) ||				/* or all doors closed				*/
		(door_state [0] == DOOR_OPENING) ||				/* or door is opening				*/
		(door_state [1] == DOOR_OPENING))
		{/* retrigger timer					*/
			if (callprioritytimer < (timer + (p.time_back_emergency SEC)))
				callprioritytimer = timer + (p.time_back_emergency SEC);
		}
		if ((callenabledtimer >= timer) &&					/* calls enabled					*/
		((door_state [0] == DOOR_OPENING) ||				/* and door is opening			*/
		(door_state [1] == DOOR_OPENING)))					/* retrigger timer until door is opened 	*/
			callenabledtimer = timer + (p.calltime_emergency SEC);
		if (!p.normal_cc_emergency)							/* normal car calls not enabled		*/
			enabled &= ~CARCALL;							/* disable car calls				*/
		if ((p.call_active_emergency && (!emergency_key)) ||	/* calls enabled only if emergency key is on	*/
		(p.calltime_emergency && (callenabledtimer<timer)))		/* or only for a time enabled and time out	*/
			enabled &= ~PRIOR_CARCALL;					/* disable priority car calls			*/
		else
			adt = DOOR_EMERG;							/* select door table for emergency mode	*/
		if (p.hallcall_emergency)
			enabled &= ~ALL_HALL_CALLS;
		if ((lastcallfloor == level) &&							/* lift is in floor of last call		*/
		(lastcalltype == EMERGENCY_CALL) &&				/* last call was emergency call		*/
		(lastcalldoors & priorstate [2][level] & ~doorstate & 0x0007))
		{
			enabled &= ~CARCALL;							/* disable car calls				*/
			if (callprioritytimer < (timer + (p.time_back_emergency SEC)))
				callprioritytimer = timer + (p.time_back_emergency SEC);
		}
	}
	if (callpriority == C_EVACUATION)
	{
		enabled = 0;										/* disable all calls				*/
		if (evacuation)										/* fire key or fire call on			*/
			callprioritytimer = timer;						/* retrigger timer					*/
		if ((!evacuation_ready) &&							/* evacuation not ready			*/
				(evacuationdelay < timer) &&	//evacuation_enable)					/* min. delay time over			*/
				((evacuation_enable | evacuation_key) || ((evacuationtimer < timer) && p.evacuationtime)))
			{/* evacuation enable input or max. time out	*/
				if (((level != evacuationfloor) || (((~doorstate) & EXISTING_DOORS) != evacuationdoor)) &&
				(evacuationfloor <= p.top_floor))					/* lift not in evacuation floor		*/
					{
						if (hse_state != H_ARRIVAL)					/* in arrival state don't set new call	*/
							{										/* set call to evacuation floor		*/
								calltab [evacuationfloor].calltype = PRIOR_CARCALL;
								calltab [evacuationfloor].cc_door	= evacuationdoor;
							}
					}
				else if ((hse_state == H_STANDSTILL)				/* standstill in evacuation floor		*/
						|| ((evaction_waittime_nomove < timer) && p.evaction_waittime))
					{
						evacuation_ready = 1;						/* evacuation is ready				*/
						carlight_flag = 1;
						set_out (SPECIAL_FUNC, EVACUATION_READY, 0, 0, 1, (O_CANA|O_CANB|O_HSE));
					}
			}
		else
			{
				if (number_of_calls (1, ALL_CALLS))
					clearcalls (ALL_CALLS);
			}
	}
	if (callpriority == C_FIREALARM)							/* fire alarm						*/
	{
		if (!firealarm_old)
		{
			clearcalls (ALL_CALLS);							/* clear all calls					*/
			firealarm_old = 1;
		}
		enabled = 0;										/* disable all calls				*/
		adt = DOOR_ALARM;								/* set door table for fire alarm mode		*/
		firealarmfloor = get_firealarmfloor (firealarmfloor_old);	/* search fire alarm floor			*/
		if (firealarmmode)
		{
			callprioritytimer = timer;						/* retrigger timer					*/
			if ((firealarmfloor != firealarmfloor_old) ||			/* new fire alarm floor			*/
			((hse_state == H_STANDSTILL) &&				/* or lift is standstill and lift not in	*/
			(level != firealarmfloor) &&						/* fire alarm floor and no call		*/
			(!(calltab [firealarmfloor].calltype & PRIOR_CARCALL))))
			{											/* write call to call table			*/
				if (firealarmfloor_old != 0xFF)					/* active fire alarm floor before		*/
				{
					calltab [firealarmfloor_old].calltype = 0;		/* clear old call					*/
					calltab [firealarmfloor_old].cc_door	= 0;
				}
				calltab [firealarmfloor].calltype |= PRIOR_CARCALL;
				calltab [firealarmfloor].cc_door |= (p.doortable [adt][firealarmfloor] & p.doorpos [firealarmfloor]);
				parkcall = firefloor + 1;
				firealarmfloor_old = firealarmfloor;
			}						
			if(firealarmfloor == level)
				{
					if(!alarmfloor_flag)
						{
							carlight_flag = 1;
							alarmfloor_flag = 1;
						}
				}			
		}
	}
	else
	{
		firealarm_old = 0;
		alarmfloor_flag = 0;
	}

	if (callpriority == C_FIREMAN)							/* fireman mode					*/
	{
		if (firemode)										/* fire key or fire call on			*/
			callprioritytimer = timer ;	   					/* retrigger timer			*/
		if (firekey || auto_fire)								/* fire key in car is on				*/
		{ 
			if (((!firecall_exist) || firecallstate)		// no fire call input or fire call ready
					&& (!firekeymisuse))		// if key misused -> must be off before
			{	
				enabled = PRIOR_CARCALL;	/* only priority car calls enabled	*/
				if ((level == firefloor) &&					/* lift is in fireman floor			*/
				(!(p.fireman_function1 & FIREMAN1_DOOR_MAINFLOOR)))							/* and in fireman floor automatic door op.	*/
					firedoormode = 0;						/* standard door opening			*/
				else
					firedoormode = 1;						/* doormode depending on parameter (push)*/
				firekeystate = 1;
				firemodestate = 1;
			}
			else
			{
				firekeymisuse = 1;					/* misuse of fire key				*/
				enabled = 0;								/* disable all calls				*/
			}
		}
		else
		{
			firedoormode = 0;								/* normal door mode				*/
			firekeymisuse = 0;								/* clear misuse marker			*/
			enabled = 0;									/* disable all calls				*/
			if (p.fireman_function1 & FIREMAN1_REPEAT_FIRECALL)								/* repeat fire call if fire key is off	*/
				firekeystate = 0;							/* reset state of fire key			*/
		}
		
		// start fire call trip	
		if (firecall && (!firecall_old) &&						/* firecall switched on			*/
		(!firekeystate))										/* but no fire key in car			*/
		{
			clearcalls (ALL_CALLS);		/* clear all calls					*/
			if ((!firekeystate) &&								/* firekey was not on or repeat fire call		*/
			(firefloor >= p.bot_floor) && (firefloor <= p.top_floor))
			{
				calltab [firefloor].calltype |= PRIOR_CARCALL;	/* write call to call table			*/
				calltab [firefloor].cc_door |= (firedoors & p.doorpos [firefloor]);
				firecallstate = 1;							/* fire call started				*/
				parkcall = firefloor + 1;
			}
		}
	
		// automatic change mode to fireman
		if (p.fireman_function1 & FIREMAN1_AUTO_FIREMODE)									/* automatic fireman mode		*/
		{
				//set_out (SPEAKER_BUZ, BUZZER_FIRE, 0, EXISTING_DOORS, 0 , O_CANA);   //buzzer on

			if (firecall && (level == firefloor) &&				/* fire call is ready				*/
			(doorstate != ALL_DOORS_CLOSED))				/* not all doors closed			*/
				firecallready = 1;
			
			if (firecall && firecallready)						/* fire call input is on and firecall ready		*/
				auto_fire = 1;								/* set firekey as on				*/
			else
			{
				if (p.fireman_function1 & FIREMAN1_REPEAT_FIRECALL)							/* repeat fire call enabled			*/
				{
					firecallready = 0;
					auto_fire = 0;							/* set firekey as off				*/
				}
			}
		}
		if (firekey_old != (firekey | auto_fire))					/* state of fireman key in car changed		*/
			clearcalls (ALL_CALLS);							/* clear all calls					*/
		if (!firekeymisuse)									/* only if no misuse of fire key		*/
			adt = DOOR_FIREMAN;							/* select door table for fireman mode		*/
		if ((!firekey) && (!firecall) && (!auto_fire))				/* fire key and fire call off			*/
		{
			if ((firemodestate && (level == firefloor) && 		/* fire key was on, lift in fire call floor 	*/
						(doorstate != ALL_DOORS_CLOSED))				/* and not all doors closed 		*/
						|| (!firecallstate) 							/* or fire call not started 		*/
						|| (!firecall_exist)								/* or no fire call input				*/
						|| ((p.fireman_function1 & FIREMAN1_AUTO_FIREMODE) && (level == firefloor) &&		/* or automatic fireman mode		*/
								(doorstate != ALL_DOORS_CLOSED))) 		/* and not all doors closed 		*/
			{
				firemode = 0;								/* go back to normal mode		*/
				firemodestate = 0;

				firedoormode = 0;							/* normal door mode				*/
				firekeystate = 0;							/* reset state that fire key was on	*/
				firecallstate = 0;							/* reset fire call state				*/
				firecallready = 0;							/* reset fire call ready state		*/
			}
		}
	}
	firecall_old = firecall;									/* save state of key inputs			*/
	firekey_old = (firekey | auto_fire);
	special_key_old = special_key;
	advanced_key_old = advanced_key;
	emergency_key_old = emergency_key;
	if ((callprioritytimer < timer) && (!number_of_calls (1, PRIOR_CARCALL)))
		callpriority = C_STANDARD;	
	
	if ((calls_off) || (triperrorcount >= MAX_TRIP_ERROR) || (door_short_conn) ||
			(ucmp_state) || (brake_value == BRAKE_NO_GOOD) || (bTest_brake) || (pitbox_reset_active & PIO_1))
		enabled = 0;										/* all calls disabled				*/
	if ((landings_off) || (landings_off_key) || (correct_level))		/* landing calls off (from menu) landing calls off (from key)	*/
		enabled &= ~ALL_HALL_CALLS;
	calls_enabled = enabled;
	handle_calls = handle;
}

/****************************************************************************************************/
/* set all outputs																			*/
/****************************************************************************************************/
void set_outputs (void)
{
	static BYTE toggle;
	static BYTE direction_ind_old = 0;
	static BYTE hall_lantern_old = 0;
	static BYTE fault_signal_old = 0;
	static BYTE out_of_order_display = 0;
	static BYTE out_of_order_old = 1;
	static BYTE special_old = 0;
	static BYTE advanced_old = 0;
	static BYTE emergency_old = 0;
	static BYTE switched_off_old = 0;
	static BYTE remote_off_old = 0;
	static BYTE firemode_old = 0;
	static BYTE firekey_old = 0;
	static BYTE firecall_old = 0;
	static BYTE firefloor_old = 0;
	static BYTE firealarm_old = 0;
	static BYTE firealarmfloor_old = 0;
	static BYTE specialmode_old = 0;
	static BYTE door_closing_old = 0;
	static BYTE buzzer_old = 0;
	static BYTE doorstate_old = 0;
	static BYTE level_old = 0xFE;
	static BYTE door_short_conn_flag = 0;
	static BYTE evacuation_old = 0;
	static BYTE backlight_mode_old = 10;
	static BYTE alarm_filter_old = 0;
	BYTE arrival_ind = ALL_DOORS_CLOSED;

	if (toggle)
	{
		if ((direction) && (!relevelflag))						/* set direction indication			*/
			direction_ind = direction | (direction << 4);		/* when travelling use travel direction		*/
		else
			direction_ind = calldir;							/* when standstill use call direction			*/
		if (direction_ind != direction_ind_old)					/* state of direction indication changed		*/
		{
			set_out (DIRECTION_IND, direction_ind, level + 1, 0, 1, (O_CANA|O_CANB|O_HSE));
			direction_ind_old = direction_ind;
		}
		if ((send_level) || (level_old != level) || ((insp || em_op) && send_level_insp))
		{
			level_display(level, O_CANA | O_CANB);
			send_level = 0;
			level_old = level;
			send_level_insp = 0;
		}
		if ((hse_state == H_ARRIVAL) || (hse_state == H_STANDSTILL))
		{												/* set gong only in normal state	*/
			arrival_ind = (doorstate ^ doorstate_old) & (~doorstate);
			if ((arrival_ind) && (gongoutputenable) && 
					(!(bFunc_flag & FUNC_ARRIVEGONG_TIMEIN) || !(p.cooperate_sel1 & COOP1_ARRIVE_NIGHTGONG)))	/* new opened doors in arrival state			*/
			{
				//set_out (ARRIVAL_INDICATION, gong_direction, level + 1, arrival_ind, 1, (O_CANA|O_CANB|O_HSE));
				set_out (ARRIVAL_INDICATION, DIR_DN, level + 1, arrival_ind, 1, (O_CANA|O_CANB|O_HSE));
				gongoutputenable = 0;
			}
			if (hse_state == H_ARRIVAL)						/* arrival state					*/
				hall_lantern = (doornew << 4);				/* use all still to open doors		*/
			else											/* in standstill state use opened doors		*/
				hall_lantern = (((~doorstate) & EXISTING_DOORS) << 4);
			if (hall_lantern)								/* doors opened or to open		*/
			{
				if (hse_state == H_ARRIVAL)	
					hall_lantern |= startdir;
				else
				{
					if (calldir)
						hall_lantern |= calldir;					/* set call direction				*/
					else
						hall_lantern |= (DIR_UP | DIR_DN);		/* set both directions				*/
				}
			}			
		}
		else												/* all other states					*/
			hall_lantern = 0;

		if (hall_lantern != hall_lantern_old)						/* new hall lantern to set			*/
		{
			if (hall_lantern)								/* hall lantern to set				*/
				set_out (LIGHT_FUNC, hall_lantern & DIRECTION_BITS, target_floor + 1, hall_lantern >> 4, 1, (O_CANA|O_CANB|O_HSE));
			else
				set_out (LIGHT_FUNC, ARROW_UP | ARROW_DOWN, 0xFF, ALL_DOORS_CLOSED, 0, (O_CANA|O_CANB|O_HSE));
			hall_lantern_old = hall_lantern;					/* save actual state of hall lantern	*/
		}		

		if(backlight_mode_old != p.lcd_backlight)
			{
				backlight_mode_old = p.lcd_backlight;
				set_out (OPERATION_DATA, BACKLIGHT_MODE >> 8, BACKLIGHT_MODE & 0xFF, 0, backlight_mode_old, (O_CANA | O_CANB));
			}

		doorstate_old = doorstate;							/* save actual door state			*/
	}
	else
	{
		if ((out_of_order) ||						/* lift out of order							*/
		(out_of_order_input) ||						/* activated by input							*/
		(out_of_order_menu) ||						/* activated by menu							*/
		(door_off) ||						/* doors disabled								*/
		(!calls_enabled))
			out_of_order_display = 1;
		else
			out_of_order_display = 0;
		if (out_of_order_display != out_of_order_old)	/* state of out of order output changed			*/
		{
			set_out (SPECIAL_FUNC, OUT_OF_ORDER, 0, 0, out_of_order_display, (O_CANA|O_CANB|O_HSE));
			set_out (SPECIAL_INDICATION,OUT_OF_ORDER_IND, 0, 0, out_of_order_display, (O_CANA|O_CANB|O_HSE));
			out_of_order_old = out_of_order_display;		/* save actual state							*/
		}
		if (fault_signal != fault_signal_old)						/* state of fault output changed	*/
		{
			set_out (SPECIAL_FUNC, FAULT_SIGNAL, 0, 0, fault_signal, (O_CANA|O_CANB|O_HSE));
			fault_signal_old = fault_signal;					/* save actual state				*/
		}

		if (hallcall_bypass != hallcall_bypass_ind)			/* hall call bypass state changed	*/
		{
			set_out (SPECIAL_FUNC, HALLCALL_BYPASS, 0, 0, hallcall_bypass, (O_CANA|O_CANB|O_HSE));
			hallcall_bypass_ind = hallcall_bypass;
		}
		if (fullload != fullload_ind)							/* full load state changed			*/
		{
			set_out (SPECIAL_FUNC, FULL_LOAD_STATE, 0, 0, fullload, (O_CANA|O_CANB|O_HSE));
			set_out (SPECIAL_INDICATION,FULL_LOAD_IND, 0, 0, fullload, (O_CANA|O_CANB|O_HSE));
			fullload_ind = fullload;
		}
		if (overload != overload_ind)						/* overload state changed			*/
		{
			set_out (SPECIAL_FUNC, OVER_LOAD_STATE, 0, 0, overload, (O_CANA|O_CANB|O_HSE));
			set_out (SPECIAL_INDICATION,OVER_LOAD_IND, 0, 0, overload, (O_CANA|O_CANB|O_HSE));
			overload_ind = overload;
		}

		emergency_ind = 0;
		advanced_ind = 0;
		special_ind = 0;
		if ((callpriority == C_EMERGENCY) || (calltype == EMERGENCY_CALL))
			emergency_ind = 1;
		else if ((callpriority == C_ADVANCED) || (calltype == ADVANCED_CALL))
			advanced_ind = 1;
		else if ((callpriority == C_SPECIAL) || (calltype == SPECIAL_CALL))
			special_ind = 1;
		if (emergency_ind != emergency_old)					/* emergency state changed		*/
		{
			set_out (SPECIAL_FUNC, KEY_EMERGENCY, 0, 0, emergency_ind, (O_CANA|O_CANB|O_HSE));
			emergency_old = emergency_ind;
		}
		if (advanced_ind != advanced_old)					/* advanced state changed			*/
		{
			set_out (SPECIAL_FUNC, KEY_ADVANCED, 0, 0, advanced_ind, (O_CANA|O_CANB|O_HSE));
			advanced_old = advanced_ind;
		}
		if (special_ind != special_old)						/* special state changed			*/
		{
			set_out (SPECIAL_FUNC, KEY_SPECIAL, 0, 0, special_ind, (O_CANA|O_CANB|O_HSE));
			special_old = special_ind;
		}

		switched_off_ind = 0;
		remote_off_ind = 0;
		if (callpriority == C_STANDARD)
		{
			if (car_switched_off || remote_off)				/* remote off or switched off from car		*/
				switched_off_ind = 1;
			if (remote_switched_off)						/* remote off, last trip finished		*/
				remote_off_ind = 1;
		}
		if (switched_off_ind != switched_off_old)				/* switched off state changed		*/
		{
			set_out (SPECIAL_FUNC, REMOTE_OFF_STATE, 0, 0, switched_off_ind, (O_CANA|O_CANB|O_HSE));
			switched_off_old = switched_off_ind;
		}
		if (remote_off_ind != remote_off_old)					/* remote switch off trip			*/
		{
			set_out (SPECIAL_FUNC, REMOTE_OFF, 0, 0, remote_off_ind, (O_CANA|O_CANB|O_HSE));
			remote_off_old = remote_off_ind;
		}

		if (firemode != firemode_old)							/* fire mode state changed			*/
		{
			set_out (SPECIAL_FUNC, FIRE_STATE, 0, 0, firemode, (O_CANA | O_CANB | O_HSE));
			firemode_old = firemode;
		}
		firekey_ind = (firekey | auto_fire);
		if (firekey_old != firekey_ind)							/* state of fireman key in car changed		*/
		{
			set_out (SPECIAL_FUNC, FIRE_SERVICE_ENABLE, 0, 0, firekey, (O_CANA|O_CANB|O_HSE));
			firekey_old = firekey_ind;
		}
		if (firemode && (!firekey) && (!auto_fire) && (calltab [firefloor].calltype & CARCALL))
			firecall_ind = 1;
		else
			firecall_ind = 0;
		if (firecall_ind != firecall_old)							/* fire call state changed			*/
		{
			set_out (SPECIAL_FUNC, FIRE_SERVICE, 0, 0, firecall_ind, (O_CANA|O_CANB|O_HSE));
			firecall_old = firecall_ind;
		}

		if (firemode && (!firekey) && (!auto_fire) && (level == firefloor) && (hse_state == H_STANDSTILL))
			firefloor_ind = 1;
		else
			firefloor_ind = 0;
		if (firefloor_ind != firefloor_old)							/* fire call state changed			*/
		{
			set_out (SPECIAL_FUNC, FIREFLOOR_IND, 0, 0, firefloor_ind, (O_CANA|O_CANB|O_HSE));
			firefloor_old = firefloor_ind;
		}

		if ((callpriority == C_FIREALARM) && (level == firealarmfloor) && (hse_state == H_STANDSTILL))
			firealarmfloor_ind = 1;							/* lift is in firealarm floor			*/
		else
			firealarmfloor_ind = 0;
		if (firealarmfloor_ind != firealarmfloor_old)				/* fire alarm floor reached state changed	*/
		{
			set_out (SPECIAL_FUNC, FIRE_ALARM_FLOOR, 0, 0, firealarmfloor_ind, (O_CANA|O_CANB|O_HSE));
			firealarmfloor_old = firealarmfloor_ind;
		}
		if (firealarmmode != firealarm_old)					/* fire alarm mode state changed	*/
		{
			set_out (SPECIAL_FUNC, FIRE_ALARM, 0, 0, firealarmmode, (O_CANA|O_CANB|O_HSE));
			firealarm_old = firealarmmode;
		}
		if(evacuation_old != evacuation)
			{
				set_out (SPECIAL_FUNC, EVACUATION_ACTIVE, 0, 0, evacuation, (O_CANA | O_CANB));
				evacuation_old = evacuation;
			}
		if ((callpriority > C_STANDARD) ||
		(emergency_ind) ||
		(advanced_ind) ||
		(special_ind) ||
		(switched_off_ind)	||
		(landings_off) ||
		(landings_off_key)	||
		(timer_trip || timer1_trip || timer2_trip))
			specialmode = 1;
		else
			specialmode = 0;
		if (specialmode != specialmode_old)					/* special mode state changed		*/
		{
			set_out (SPECIAL_FUNC, SPECIAL_MODE, 0, 0, specialmode, (O_CANA|O_CANB|O_HSE));
			specialmode_old = specialmode;
		}

		if ((door_state [0] == DOOR_CLOSING) || (door_state [1] == DOOR_CLOSING))
		{
			door_closing = 1;
			if (forced_closing ())
				buzzer = 1;
		}
		else
		{
			door_closing = 0;
			buzzer = 0;
		}
		if (door_closing != door_closing_old)					/* door closing state changed		*/
		{
			set_out (SPECIAL_FUNC, DOOR_IS_CLOSING, 0, 0, door_closing, (O_CANA|O_CANB|O_HSE));
			door_closing_old = door_closing;
		}
		if (buzzer != buzzer_old)								/* buzzer signal changed			*/
		{
			set_out (SPECIAL_FUNC, BUZZER, 0, 0, buzzer, (O_CANA|O_CANB|O_HSE));
			buzzer_old = buzzer;
		}
		if(door_short_conn)
			{//���ӽ������ŵĶ̽���ʾ
				if((drive_state != D_STOPPED) && (!door_short_conn_flag))
					{//����״̬
						set_out (SPECIAL_FUNC, DOOR_CONN_ALARM, 0, 0, 1, O_CANA);
						door_short_conn_flag = 1;
					}
				else if((drive_state == D_STOPPED) && (door_short_conn_flag))
					{
						set_out (SPECIAL_FUNC, DOOR_CONN_ALARM, 0, 0, 0, O_CANA);
						door_short_conn_flag = 0;
					}
			}
		else if(door_short_conn_flag)
			{
				door_short_conn_flag = 0;
				set_out (SPECIAL_FUNC, DOOR_CONN_ALARM, 0, 0, 0, O_CANA);
			}
		if(alarm_filter != alarm_filter_old)
			{
				alarm_filter_old = alarm_filter;
				set_out (SPECIAL_FUNC, ALARM_FILTER, 0, 0, alarm_filter_old, O_CANA);
			}
	}
	toggle = ~toggle;
}

//�������ƹ���: 
BYTE Limitcall(void)
{
	if (p.limitenable) 
	{
		if (p.limittimes && (tripcount_limit >= p.limittimes))
			return 1;
		if ((p.t_limit.year) && (p.t_limit.month) && (p.t_limit.day))
		{
			if (p.t_limit.year < t.year)
				return 1;
			if (p.t_limit.year == t.year)
			{
				if (p.t_limit.month < t.month)
					return 1;
				if (p.t_limit.month == t.month)
				{
					if (p.t_limit.day < t.day)
						return 1;
					if (p.t_limit.day == t.day)
					{
						if (p.t_limit.hour < t.hour)
							return 1;
					}
				}
			}
		}
	}

	return 0;
}

BYTE check_floorrecord(BYTE* floorrecord){
	BYTE i;
	for(i = 1; i<p.top_floor; i++)
		{
			if(floorrecord[i] == 0)
				return 0;
		}
	return 1;
}


/****************************************************************************************************/
/* do all cyclic functions																	*/
/****************************************************************************************************/
void cycle (void)
{
	static DWORD outputtimer = 0;
	static DWORD readtemp_timer = 0;
	static DWORD sum_checksum = 0;
	static WORD ee_readoffset = 0;
	static BYTE check_checksum = 0;
	static BYTE floor_record = 0;
	static BYTE ucmp_state_old = 0;
	DWORD temp_checksum;
	WORD ee_readsize;
	BYTE last_pointer;
	BYTE limit = 0;	
	WORD minute_new, minute_set;
	static BYTE can_init_start = 0;
	static DWORD can_init_timer = 0;
	
	ClrWdt();										/* reset watchdog					*/
	timer = realtimer;									/* read timer count value				*/
	if (!teach_in)
		set_call_priority ();									/* enable / disable calls				*/
	if (rca)											/* read CAN A						*/
		ReadCanA ();
	if (rcb)											/* read CAN B						*/
		ReadCanB ();
	if (rcc)
		ReadCanC ();
	read_hse_inputs ();
	if (outputtimer < timer)
		{
			if(!teach_in)
				{
					set_outputs ();
					if (p.groupsize > 1)
						send_heartbeat_c();
				}
			if(send_pos_fg)
				send_drive_pos(); 		
			if(p.simulation && p.simu_mult)
				pos_control();
			outputtimer = timer;
		}
	if ((handle_ese) && (!teach_in))
	{
		tse();
		ese();
		gse();
		exe();
		handle_ese = 0;
	}
	
	ConductNiceSpiData();
	if ((hse_state != H_DRIVE_ERROR) && (iv_para_valid))
		drive_nice5000();
	ClrWdt();										/* reset watchdog					*/

	if (send_heartbeat)
	{
		send_heartbeat_b();
		send_heartbeat = 0;
	}

	if (state_msi != state_sgm)
	{
		door_zone_old = state_sgm;
		state_sgm = state_msi;
		msi_trans_num++;
		if (state_sgm)
			door_zone |= POS_SGM;
		else
			door_zone &= ~POS_SGM;
		transmit_inputs (POS_IN, POS_SGM, state_sgm);
		relevelnumber = 0;
		if (!msi_trans_state)
			msi_trans_state = 1;
		else
			relev_new = 0;
 	}
	switch (hse_state)
	{
#if	SRU_OLD
		case (H_ARRIVAL):
			if ((p.adv_door_op) && (level == target_floor) && (nice5000_Rstore_feedspd < p.arrival_speed))
			{
				if (door_zone & POS_SGM)
				{
					SetSwitch(SWITCH_ULSI, SWITCH_ON);
				}
				else
				{
					if (state_ulsi != SWITCH_OFF)
						ulsi_off_timer = timer + 1 SEC;
					SetSwitch(SWITCH_ULSI, SWITCH_OFF);
				}
			}
			break;
			
		case (H_STANDSTILL):
			if ((p.relevel) &&
					(dl_short_test_finish || (!check_dl_short && !(p.lift_func_sel1 & LIFTFUN1_DOORSHORT_REVEL))))
			{//�̽Ӽ���Ѿ���ɻ��߲���Ҫ���̽ӹ���		
				if((((door_zone & (POS_SGU | POS_SGD)) == (POS_SGU | POS_SGD)) || (relevelnumber >= 6))
							&& (drive_state == D_STOPPED))
					{//��ƽ�����						
						if (state_ulsi != SWITCH_OFF)
							ulsi_off_timer = timer + 1 SEC;
						SetSwitch(SWITCH_ULSI, SWITCH_OFF);
					}
				else if (door_zone & POS_SGM)
				{
					SetSwitch(SWITCH_ULSI, SWITCH_ON);
				}
			}
			break;

		case H_INSP:
		case H_EM_OP:
			if ((bTestUCMP) && ((em_op & INSP_ON) || (insp & INSP_ON)))
				{//����ʱ������Ҫ�ض�SRU
					if (door_zone & POS_SGM)
						{
							if(state_ulsi == SWITCH_OFF)						
								ulsi_off_timer = timer + 15;
							SetSwitch(SWITCH_ULSI, SWITCH_ON);					
						}
				}
			break;
			
		default:
			if (!(door_zone & POS_SGM) && !bTestUCMP)			//������״̬��һ�ɹر�SR3
				{
					if (state_ulsi != SWITCH_OFF)
						ulsi_off_timer = timer + 1 SEC;
					SetSwitch(SWITCH_ULSI, SWITCH_OFF);
				}
			break;

#elif	SRU_NEW
		case (H_STANDSTILL):
			if ((p.relevel) &&
					(dl_short_test_finish || (!check_dl_short && !(p.lift_func_sel1 & LIFTFUN1_DOORSHORT_REVEL))))
				{//�̽Ӽ���Ѿ���ɻ��߲���Ҫ���̽ӹ���		
					SetSwitch(SWITCH_ULSI, SWITCH_OFF);
				}
			break;

		case H_ERROR:	
		case H_CAN_ERROR	:
		case H_HB_ERROR:
		case H_SC_ERROR	:
		case H_DOORERROR:	
		case H_DOOROPEN:	
		case H_DRIVE_ERROR:
		case H_ERROR_RELEV:
		case H_EE_ERROR:
			if(drive_state == D_STOPPED)				
				SetSwitch(SWITCH_ULSI, SWITCH_ON);				//���ݴ����쳣״̬���Ͽ�K2�̵���
			break;
		
		default:
//			if (!(door_zone & POS_SGM) && !bTestUCMP) 		//������״̬��һ�ɹر�SRu
//				{
//					if (state_ulsi != SWITCH_OFF)
//						ulsi_off_timer = timer + 1 SEC;
//				SetSwitch(SWITCH_ULSI, SWITCH_OFF);
//				}
			if (state_ulsi != SWITCH_OFF)
				SetSwitch(SWITCH_ULSI, SWITCH_OFF);
			break;			

#endif
	}
	if ((dz_input) && (state_ulsi == SWITCH_OFF) && (ulsi_off_timer < timer) && (p.adv_door_op || p.relevel)
			&& ((hse_state == H_STANDSTILL) || (hse_state == H_ARRIVAL)))
	{//ֻ����Ԥ�����Լ���ƽ��״̬�²��ж��������
#if	SRU_OLD
		if ((!(poserror & P_KH5_ON)) && (!bTestUCMP))
#elif	SRU_NEW
		if ((!(poserror & P_KH5_ON)) && (!bTestUCMP) && !(door_zone & POS_SGM))
#endif
		{
			poserror |= P_KH5_ON;
			write_errorhist (E_KH5_ON, HSE_ID, C_KH5_ON, 0);
		}
	}

	if (sdo_size && (sdo_timer < timer))					/* SDO segment transfer time out		*/
	{
		sdo_size = 0;
		abort_sdo (SDO_TIMEOUT);						/* send SDO abort request				*/
	}

	if (key_doorcommand)								/* new door command from keys		*/
	{
		if (drive_state == D_STOPPED)					/* Lift is stopped						*/
		{
			switch (key_doorcommand)
			{
				case (DR1_OPEN):						/* open door 1						*/
					if ((door_zone & POS_SGM)  ||	/* Lift is in door zone (SGO or SGU)		*/
							(!p.setup_ready) || (!p.motor_tuned))					/* or setup not ready					*/
						door_command (OPEN_DOOR, DOOR_OFF);
					break;

				case (DR2_OPEN):						/* open door 2						*/
					if ((door_zone & POS_SGM)  ||	/* Lift is in door zone (SGO or SGU)		*/
							(!p.setup_ready) || (!p.motor_tuned))					/* or setup not ready					*/
						door_command (DOOR_OFF, OPEN_DOOR);
					break;

				case (DR_CLOSE):						/* close all doors						*/
					door_command (CLOSE_DOOR, CLOSE_DOOR);
					break;
			}
		}
		key_doorcommand = 0;
	}

	if (fan_state)										/* cabin fan is on						*/
	{
		if((p.fan_mode & FAN_FIRE) &&	((callpriority == C_FIREMAN) || (callpriority == C_FIREALARM)))
			{				
				fan_state = 0;								/* switch fan off 					*/
				set_out (SPECIAL_FUNC, FAN_1, 0, 0, fan_state, (O_CANA|O_CANB|O_HSE));
			}
		else if (((p.fan_mode & FAN_AUTOMATIC) && (fantimer < timer)) ||		/* fan time over in automatic mode		*/
					((!(p.fan_mode & FAN_AUTOMATIC)) && (fantimer < timer) && (p.fan_time)))
			{											/* fan time over in manual mode; time > 0		*/
				fan_state = 0;								/* switch fan off						*/
				if (!fan_switch_status)
					set_out (SPECIAL_FUNC, FAN_1, 0, 0, fan_state, (O_CANA|O_CANB|O_HSE));
			}
	}

	if (monitoring_state & MONITORING_SEND)
	{
		monitoring_message_cana();
		monitoring_state &= ~MONITORING_SEND;
	}
	if (second_toggle)
	{
		second_toggle = 0;
		if ((!iic_busy) && (!check_para_doing))
		{
			if (settimeflag)
			{
				SetRtc();
				settimeflag = 0;
			}
			else
			{
				GetRtc();
				time_message_a();
				time_message_b();
			}
			CLRENA0 = ISE_TIMER1;
			rtc_iic_doing = 3;
			SETENA0 = ISE_TIMER1;
		}
		
		monitoring_message_canb();		//���� canb �� monitor ��Ϣ������
		
		minute_new = timer / 36000;		//Сʱ
		if(!minute_new)
			minute_new = 1;		//δ��1Сʱ����1Сʱ��
		minute_new *= 5;
		can_disturb[0] = cana_error_count / minute_new;
		can_disturb[1] = canb_error_count / minute_new;
		can_disturb[2] = canc_error_count / minute_new;
		
/*========	Modify By Jca 0f limit funtion	============*/
		if((!testtrip) && (p.limitenable))
		{
			limit = Limitcall();
			if(limit && (callpriority == C_STANDARD))
			{//ֻ����ͨ���ݲŻ�����
				limitstatus = 1;
			}
			else
				limitstatus = 0;
		}
		else
			limitstatus = 0;
/*========	Modify By Jca 0f limit funtion	============*/
//���Ӽ�¼¥�ߵĲ���
		if((check_floorrecord(floor_record_fg)) && (!floor_record) && (p.simulation != SIMULATION_AUTO))
			{
				floor_record = 1;				
				WriteParaData(&floor_higth, EE_FLOOR_HIGH, sizeof(floor_higth));
			}
//�����Զ���Ⱨբ���Ĺ���
		if((p.brake_check_auto) && (p.brake_check_time))
			{
				minute_new = (WORD)t.hour * 60 + t.minute;
				minute_set = (WORD)(p.brake_check_time * 60) / 100 + (p.brake_check_time % 100);
				
				if((((minute_set > minute_new) && ((minute_set - minute_new) <= 5)) ||	
						((minute_new > minute_set) && ((minute_new - minute_set) <= 5)))
						&& (!driveerror)
						&& (hse_state == H_STANDSTILL)
						&& (drive_state == D_STOPPED)
						&& (doorstate == ALL_DOORS_CLOSED)
						&& (safety_circuit & SC1)
						&& (safety_circuit & SC2)
						&& (safety_circuit & SC3))
					{
						if((!bTest_brake) && (!brake_check_return))
							{
								if(brake_check_count < 5)
									{//�쳣��������������5��
										brake_check_count++;
										bTest_brake = 1;
										brake_check_forced = 1;
									}
								else
									{
										bTest_brake = 0;
										brake_check_forced = 0;
									}
							}
					}
				else
					{
						bTest_brake = 0;
						brake_check_forced = 0;
						brake_check_count = 0;
						brake_check_return = 0;
					}
			}
//�����Զ���Ⱨբ���Ĺ���

//��������ʱ����ж�
		if(p.remote_off_func & REMOTE_TIME)
			{
				if (((p.remote_starttime < p.remote_stoptime) &&
						(t.hour >= p.remote_starttime) && (t.hour < p.remote_stoptime)) ||
						((p.remote_starttime > p.remote_stoptime) &&
						((t.hour >= p.remote_starttime) || (t.hour < p.remote_stoptime))))
					remote_off |= REMOTE_TIME;
				else				
					remote_off &= ~REMOTE_TIME;
			}
		else
			remote_off &= ~REMOTE_TIME;
//��������ʱ����ж�

//���ӵ�վ��ҹ��ʱ����ж�
		if(p.cooperate_sel1 & COOP1_ARRIVE_NIGHTGONG)
			{
				if (((p.arrivegong_starttime < p.arrivegong_stoptime) &&
						(t.hour >= p.arrivegong_starttime) && (t.hour < p.arrivegong_stoptime)) ||
						((p.arrivegong_starttime > p.arrivegong_stoptime) &&
						((t.hour >= p.arrivegong_starttime) || (t.hour < p.arrivegong_stoptime))))
					bFunc_flag |= FUNC_ARRIVEGONG_TIMEIN;
				else				
					bFunc_flag &= ~FUNC_ARRIVEGONG_TIMEIN;
			}
		else
			bFunc_flag &= ~FUNC_ARRIVEGONG_TIMEIN;
//�������ӵ�վ��ҹ��ʱ����ж�

//����2��ǿ���ź���Ч����ʾ
		if ((force_switch & (FORCE_SDU1 | FORCE_SDD1)) == (FORCE_SDU1 | FORCE_SDD1))
			bFunc_flag |= FUNC_SDD1_SDU1_ON;
		else
			bFunc_flag &= ~FUNC_SDD1_SDU1_ON;
//����2��ǿ���ź���Ч����ʾ

//���ӱ����������źŵ��ж�	2020-03-13
		if((drive_state != D_STOPPED) || 
				(((door_state[0] == DOOR_OPENED) || (door_state[1] == DOOR_OPENED)) && (door_zone & POS_SGM) && !(safety_circuit & (SC3 | SC2))))
			alarm_filter = 1;
		else
			alarm_filter = 0;
//���ӱ����������źŵ��ж�	2020-03-13

	}
	if (readtemp_timer < timer)
	{
		mrt_state = GetCurrentTemperature();
//���ӻ����¶ȼ��Ľ�ֹ����
			if(p.lift_func_sel1 & LIFTFUN1_CHECK_ROOM_TEMP)
			{
				if (mrt_state == MMT_H)
					{
						virt_in[IO_BASIC_FUNC] = TEMPERATURE;
						virt_in[IO_SUB_FUNC] = ROOMTEMP_MAX;
						virt_in [IO_STATE] = 0x01;
						handle_input(LIFT1, 1);
					}
				else if (mrt_state == MMT_L)
					{
						virt_in[IO_BASIC_FUNC] = TEMPERATURE;
						virt_in[IO_SUB_FUNC] = ROOMTEMP_MIN;
						virt_in [IO_STATE] = 0x01;
						handle_input(LIFT1, 1);
					}
				else
					temperature &= ~(ROOMTEMP_MAX | ROOMTEMP_MIN);
			}
		else			
			temperature &= ~(ROOMTEMP_MAX | ROOMTEMP_MIN);
//���ӻ����¶ȼ��Ľ�ֹ����
		readtemp_timer = timer + TEMPERATURE_TIME;
	}

	if ((write_checksum) && (!iic_busy))
	{
		para_checksum = GetChecksum(&p, sizeof(p));
		WriteParaData(&para_checksum, EE_CHECKSUM, sizeof(para_checksum));
		write_checksum = 0;
		check_checksum = 1;
		sum_checksum = 0;
		ee_readoffset = 0;
	}
	else if ((check_checksum) && (drive_state == D_STOPPED) && (!iic_busy) && (!rtc_iic_doing))
	{
		if (ee_readoffset + EE_READCHECK_SIZE >= sizeof(p))
			ee_readsize = sizeof(p) - ee_readoffset;
		else
			ee_readsize = EE_READCHECK_SIZE;

		CLRENA0 = ISE_TIMER0 | ISE_TIMER1 | ISE_TIMER2 | ISE_TIMER3 | ISE_EINT1 | ISE_EINT3;
		check_para_doing = 3;
		ReadEEData(para_checkbuff, ee_readoffset, ee_readsize);
		SETENA0 = ISE_TIMER0 | ISE_TIMER1 | ISE_TIMER2 | ISE_TIMER3 | ISE_EINT1 | ISE_EINT3;
		sum_checksum += GetChecksum(para_checkbuff, ee_readsize);
		if (ee_readoffset + EE_READCHECK_SIZE >= sizeof(p))
		{
			CLRENA0 = ISE_TIMER0 | ISE_TIMER1 | ISE_TIMER2 | ISE_TIMER3 | ISE_EINT1 | ISE_EINT3;
			check_para_doing = 3;
			ReadEEData(&temp_checksum, EE_CHECKSUM, sizeof(temp_checksum));
			SETENA0 = ISE_TIMER0 | ISE_TIMER1 | ISE_TIMER2 | ISE_TIMER3 | ISE_EINT1 | ISE_EINT3;
			if (sum_checksum == temp_checksum)
			{
				check_checksum = 0;				// У�� ok
				check_times = 0;
			}
			else
			{
				++ check_times;
				if (check_times >= MAX_PARA_CHECKTIMES)
				{
					DisableIrq();		// ǿ�ƿ��Ź���λ
					while(1);
				}
				else
				{
					WriteParaData(&p, EE_PARAMETER, sizeof(p));
					check_checksum = 0;
				}
			}
		}
		else
			ee_readoffset += ee_readsize;
	}

	if (save_hist | save_all)
	{
		if (save_all)
		{
			WriteParaData(&errorhist, EE_ERRHISTORY, sizeof(errorhist));
			WriteParaData(&errorcounter, EE_ERRCOUNTER, sizeof(errorcounter));
			save_all = 0;
			save_hist = 0;
		}
		else
		{
			if (errorhist.histpointer)
				last_pointer = errorhist.histpointer - 1;
			else
				last_pointer = MAX_HIST - 1;
			WriteParaData(&errorhist.hist[last_pointer], EE_ERRHISTORY+((DWORD)&errorhist.hist[last_pointer]-(DWORD)&errorhist), sizeof(struct Hist));
			WriteParaData(&errorhist.histpointer, EE_ERRHISTORY+((DWORD)&errorhist.histpointer-(DWORD)&errorhist), sizeof(errorhist.histpointer));
			WriteParaData(wptr_errcounter, EE_ERRCOUNTER+((DWORD)wptr_errcounter-(DWORD)&errorcounter), sizeof(WORD));
			save_hist = 0;
		}
	}
	if (p.loadtype == ANALOG_ON_MCU)
	{
		if (readload_timer < mstimer)
		{
			GetLoadValue();
			readload_timer = mstimer + LOAD_TIME;
		}
	}

	ConductLedDisplay();

//���� UCM ����
	if(p.ucmp_enable && (!bTestUCMP))
		{
			check_UCMP();
			if(ucmp_state != ucmp_state_old)
				{
					if(ucmp_state)
						set_out (SPECIAL_FUNC, UCMP_LOCK, 0, 0, 1, O_HSE); 	//UCM �ź���Ч
					else				
						set_out (SPECIAL_FUNC, UCMP_LOCK, 0, 0, 0, O_HSE);	//UCM �ź���Ч
					ucmp_state_old = ucmp_state;
				}
		}
//���� UCM ����

	if((p.simulation == SIMULATION_AUTO) && p.simu_mult)
		handle_drive();

//���� Can ���ϴ���
	if(p.canbus_check)
		{//���й����в�����can����
			if ((canerrorstate & (TXA_SW_OVERRUN | RXA_SW_OVERRUN | CANA_BUSOFF | SXA_SW_OVERRUN))
					|| (hb_error & B_CAN))		// CAN bus error (overrun, bus off, tx error)
				{
					if((can_init_timer < timer) && (!can_init_start)	&& ((drive_state == D_STOPPED) || !(insp & (INSP_TOPCAR_ACTIVE | INSP_PITBOX_ACTIVE))))
						{
							can_init_start = 1;
							can_init_timer = timer + 5;
							bFunc_flag &= ~FUNC_CANA_INIT;
						}
					if(can_init_start && (can_init_timer < timer))
						{
							can_init_start = 0;
							sra = 1;
							tra = 1;
							init_can_a();
							cana_busoff_checktime = timer + 3 SEC;
							bFunc_flag |= FUNC_CANA_INIT;
							if(!(canerrorstate & CANA_BUSOFF))
								tea = 1;
							if(canerrorstate & CANA_BUSOFF)
								{
									tea = 0;
									canerrorstate &= ~CANA_BUSOFF;
								}
							if(canerrorstate & TXA_SW_OVERRUN)
								{
									tca = 0;
									canerrorstate &= ~TXA_SW_OVERRUN;
								}
							if(canerrorstate & RXA_SW_OVERRUN)
								{
									canerrorstate &= ~RXA_SW_OVERRUN;
								}
							if(canerrorstate & SXA_SW_OVERRUN)
								{
									sca = 0;
									canerrorstate &= ~SXA_SW_OVERRUN;
								}
							if(hb_error & B_CAN)
								{
									canatxerror = 0;
									hb_error &= ~B_CAN;
								}
							can_init_timer = timer + 3 SEC;
						}					
					if ((!(canerrorstate & CANA_BUSOFF)) && (cana_busoff_checktime < timer)
							&& (bFunc_flag |= FUNC_CANA_INIT))
						tea = 1;
				}
			else
				bFunc_flag &= ~FUNC_CANA_INIT;
		}
//���� Can ���ϴ���

//����¥�����ݵ����ദ��
	p.top_floor %= MAX_FLOOR;
	p.bot_floor %= MAX_FLOOR;
	target_floor %= (p.top_floor + 1);
	start_floor %= (p.top_floor + 1);
	next_floor %= (p.top_floor + 1);
	max_target %= (p.top_floor + 1);
	level %= (p.top_floor + 1);
//����¥�����ݵ����ദ��
}


