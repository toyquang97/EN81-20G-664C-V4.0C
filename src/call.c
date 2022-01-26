
#include "typedef.h"
#include "parameter.h"
#include "state.h"
#include "call.h"
#include "can.h"

struct CallTab calltab [MAX_FLOOR];							// call table
struct GroupCall groupcall [MAX_GR];						// calls of other lifts

//**************************************************************************************************
// calculate absolute value
//**************************************************************************************************
BYTE abs_val (BYTE value1, BYTE value2)
{
	if (value1 > value2)
		return (value1 - value2);
	else
		return (value2 - value1);
}

//**************************************************************************************************
// clear group varables
//**************************************************************************************************
void clear_groupcall (void)
{
	calldir = 0;
	callfloor = 0xFF;
	calltype = 0;
	calldoors = 0;
	next_calltime = 0;
	next_callfloor = 0xFF;
	next_calltype = 0;
	next_calldoors = 0;
	calltype_old = 0;
	callfloor_old = 0;
	calldoors_old = 0;
}

//**************************************************************************************************
// clear all calls
//**************************************************************************************************
void clearcalls (BYTE calltype)
{
	BYTE i;

	if (calltype == ALL_CALLS)								// clear all calls
		clear_groupcall ();									// clear all variables for group control
	if (calltype & CARCALL)									// clear car calls
	{
		set_out (CAR_CALL, 0xFF, 0, 0, 0, (O_CANA|O_CANB|O_HSE));
		for (i = 0; i < MAX_FLOOR; i++)
		{
			calltab[i].calltype &= ~(CARCALL | PRIOR_CARCALL);
			calltab[i].cc_door = 0;
		}
		parkcall = 0;
	}
	if (calltype & ALL_HALL_CALLS)							// clear all hall calls
	{
		set_out (HALL_CALL, 0, 0xFF, 0, 0, (O_CANA|O_CANB|O_HSE));
		for (i = 0; i < MAX_FLOOR; i++)
		{
			calltab[i].calltype &= ~ALL_HALL_CALLS;
			calltab[i].lcu_door = 0;
			calltab[i].lcd_door = 0;
			calltab[i].lc_door = 0;
		}
	}
	if (calltype & SPECIAL_CALL)							// clear all special hall calls
	{
		set_out (HALL_CALL_SPECIAL, 0xFF, 0, 0, 0, (O_CANA|O_CANB|O_HSE));
		for (i = 0; i < MAX_FLOOR; i++)
		{
			calltab[i].calltype &= ~SPECIAL_CALL;
			calltab[i].sc_door = 0;
		}
	}
	if (calltype & ADVANCED_CALL)							// clear all advanced hall calls
	{
		set_out (HALL_CALL_ADVANCED, 0xFF, 0, 0, 0, (O_CANA|O_CANB|O_HSE));
		for (i = 0; i < MAX_FLOOR; i++)
		{
			calltab[i].calltype &= ~ADVANCED_CALL;
			calltab[i].ac_door = 0;
		}
	}
	if (calltype & EMERGENCY_CALL)							// clear all emergency hall calls
	{
		set_out (HALL_CALL_EMERGENCY, 0xFF, 0, 0, 0, (O_CANA|O_CANB|O_HSE));
		for (i = 0; i < MAX_FLOOR; i++)
		{
			calltab[i].calltype &= ~EMERGENCY_CALL;
			calltab[i].ec_door = 0;
		}
	}
}

//**************************************************************************************************
// if car call opposite to actual direction than cancel hall call in opposite direction
//**************************************************************************************************
void cancel_other_landing_call (BYTE callfloor)
{
	BYTE type;

	type = calltab [level].calltype & handle_calls;
	if (hse_state == H_STANDSTILL)
	{
		if ((calldir == DIR_UP) && (callfloor < level) && (type & LANDINGCALL_DN))
		{													// car call to floor below actual level
			if ((!(doorstate & calltab [level].lcd_door & p.doorpos [level]))
			|| door_off)									// all doors open or or no door opening
			{
				set_out (HALL_CALL, HALL_CALL_DOWN, level + 1, calltab [level].lcd_door, 0, O_ALL);
				calltab [level].calltype &= ~LANDINGCALL_DN;
				calltab [level].lcd_door = 0;				// clear landing call down and all it doors
			}
		}
		else if ((calldir == DIR_DN) && (callfloor > level) && (type & LANDINGCALL_UP))
		{													// car call to floor above actual level
			if ((!(doorstate & calltab [level].lcu_door & p.doorpos [level]))
			|| door_off)									// all doors open or or no door opening
			{
				set_out (HALL_CALL, HALL_CALL_UP, level + 1, calltab [level].lcu_door, 0, O_ALL);
				calltab [level].calltype &= ~LANDINGCALL_UP;
				calltab [level].lcu_door = 0;				// clear landing call up and all it doors
			}
		}
	}
}

//**************************************************************************************************
// get number of calls
//**************************************************************************************************
BYTE number_of_calls (BYTE max, BYTE type)
{
	BYTE i;
	BYTE number = 0;

	for (i = p.bot_floor; i <= p.top_floor; i++)
	{
		if (calltab [i].calltype & type)					// car call for this floor
			number++;
	}
	if (number >= max)
		return (1);
	else
		return (0);
}

/****************************************************************************************************/
/* Calculate time to process a call																	*/
/****************************************************************************************************/
WORD calc_calltime (BYTE targetfloor){
	BYTE distance = 0;
	BYTE drivestateoffset = 0;
	BYTE new_call_offset;

	switch (drive_state)
	{
		case(D_START_MOVING):					// moving state
		case(D_READY_TO_SWITCH_ON):		
		case(D_OPERATION_ENABLED):
			distance = abs_val (targetfloor, next_floor);	// distance to target
			if (direction == DIR_UP)						// Lift is moving upwards
			{
				if ((targetfloor >= next_floor) &&	// possible to stop in target floor
				(targetfloor <= max_target))				// possible to go with actual speed to target
				{
					if (next_calltype & CARCALL)			// also car call for this floor
						drivestateoffset = 1;
					else									// only hall call from this floor
						drivestateoffset = 2;
				}
				else										// lift must stop before going to this floor
				{
					if (next_calltype & CARCALL)			// also car call for this floor
						drivestateoffset = 8;
					else									// only hall call from this floor
						drivestateoffset = 9;
				}
			}
			else
			{
				if ((targetfloor <= next_floor) &&			// possible to stop in target floor
				(targetfloor >= max_target))				// possible to go with actual speed to target
				{
					if (next_calltype & CARCALL)			// also car call for this floor
						drivestateoffset = 1;
					else									// only hall call from this floor
						drivestateoffset = 2;
				}
				else										// lift must stop before going to this floor
				{
					if (next_calltype & CARCALL)			// also car call for this floor
						drivestateoffset = 8;
					else									// only hall call from this floor
						drivestateoffset = 9;
				}
			}
			break;

		case(D_SLOW_DOWN):									// arrival state
		case(D_STOPPING):
		case(D_OPERATION_DISABLED):			
		case(D_SWITCHED_OFF):
		case(D_WAIT_STOP):
			distance = abs_val(targetfloor, target_floor);	// distance between actual and next target
			if (distance)									// call from another floor
			{
				if (next_calltype & CARCALL)				// also car call for this floor
					drivestateoffset = 6;
				else										// only hall call from this floor
					drivestateoffset = 8;
			}
			else
				drivestateoffset = 0;	
			break;

		case(D_STOPPED):				// standstill state  
			distance = abs_val (targetfloor, level);		// distance to target
			if (distance)									// call from another floor
			{
				if (next_calltype & CARCALL)				// also car call for this floor
					drivestateoffset = 6;
				else										// only hall call from this floor
					drivestateoffset = 8;
			}
			else											// call from actual floor
				drivestateoffset = 0;
			break;
	}
	if (level > p.top_floor)
		distance = 0;
	if ((next_calltype != calltype) || (targetfloor != callfloor))
		new_call_offset = 5;				// new target -> bigger time
	else													// if already actual target time must be smaller
		new_call_offset = 0;
	return ((WORD)(distance + drivestateoffset + new_call_offset + doordelay) * MAX_GR + p.groupnumber);
}

/****************************************************************************************************/
/* Compare call type with other lifts in group														*/
/****************************************************************************************************/
BYTE compare_group (void){
	BYTE i;

	for (i = 0; i < MAX_GR; i++)
	{
		if ((next_callfloor == groupcall [i].floor) &&		// other lift processes same floor
		(next_calltype	== groupcall [i].type) &&			// and other lift processes same call type
		(next_calldoors == groupcall [i].doors) &&			// and other lift processes same doors
		(next_calltime > groupcall [i].time))				// and other lift can process the call faster
			return (0);										// than other lift should process this call
	}
	return (1);												// process the call
}

/****************************************************************************************************/
/* Compare call type with other lifts in group														*/
/****************************************************************************************************/
BYTE get_groupcall (BYTE fl, BYTE target_dir){
	BYTE type;

	type = calltab [fl].calltype & handle_calls;
	next_callfloor = fl;
	if (target_dir == DIR_UP)						// up direction
	{
		if (type & LANDINGCALL_UP)							// landing call in up direction 
		{
			next_calltype = LANDINGCALL_UP;
			next_calldoors = calltab [fl].lcu_door;			// set doors for landing call up
			next_calltime = calc_calltime (fl);				// calculate time to process this call
			if (compare_group ())							// compare call with other lifts in group
				return (1);
		}
		if (type & LANDINGCALL)								// landing call without direction
		{
			next_calltype = LANDINGCALL;
			next_calldoors = calltab [fl].lc_door;			// set doors for landing call without direction
			next_calltime = calc_calltime (fl);				// calculate time to process this call
			if (compare_group ())							// compare call with other lifts in group
				return (1);
		}
		if (type & CARCALL)									// only car call
		{
			next_calltype = CARCALL;
			next_calldoors = calltab [fl].cc_door;			// set doors for car calls
			next_calltime = calc_calltime (fl);				// calculate time to process this call
			return (1);
		}
		return (0);											// in all other cases no call
	}
	else													// down direction
	{
		if (type & LANDINGCALL_DN)							// landing call in down direction
		{
			next_calltype = LANDINGCALL_DN;
			next_calldoors = calltab [fl].lcd_door;			// set doors for landing call down
			next_calltime = calc_calltime (fl);				// calculate time to process this call
			if (compare_group ())							// compare call with other lifts in group
				return (1);
		}
		if (type & LANDINGCALL)								// landing call without direction
		{
			next_calltype = LANDINGCALL;
			next_calldoors = calltab [fl].lc_door;				// set doors for landing call without direction
			next_calltime = calc_calltime (fl);					// calculate time to process this call
			if (compare_group ())							// compare call with other lifts in group
				return (1);
		}
		if (type & CARCALL)									// car call
		{
			next_calltype = CARCALL;
			next_calldoors = calltab [fl].cc_door;			// set doors for car calls
			next_calltime = calc_calltime (fl);				// calculate time to process this call
			return (1);
		}
		return (0);											// in all other cases no call
	}
}

//**************************************************************************************************
// set doors for priority call
//**************************************************************************************************
BYTE get_calldoor (BYTE fl, BYTE type)
{
	switch (type)
	{
		case (PRIOR_CARCALL):
			return calltab [fl].cc_door;

		case (EMERGENCY_CALL):
			return calltab [fl].ec_door;

		case (ADVANCED_CALL):
			return calltab [fl].ac_door;

		case (SPECIAL_CALL):
			return calltab [fl].sc_door;
	}
	return (0);
}

/****************************************************************************************************/
/* search priority call above actual floor															*/
/****************************************************************************************************/
BYTE priority_call_above (BYTE fl, BYTE type, BYTE max){
	BYTE i;

	if (!(handle_calls & type))				// dont handle this calls now (higher priority)
		return (0);											// ignore call
	for (i = fl; i <= max; i++)								// search priority call above
	{
		if (calltab [i].calltype & type)					// priority call
		{
			next_callfloor = i;
			next_calldoors = get_calldoor (i, type);		// set doors for this call
			next_calltype = type;							// set next call type
			next_calltime = calc_calltime (i);				// calculate time to process this call
			if (type == PRIOR_CARCALL)		// if priority car call return call
				return (1);
			else if (compare_group ())		// first compare priority call with other lifts
				return (1);									// than return call
		}
	}
	return (0);												// return no call
}

/****************************************************************************************************/
/* search priority call above actual floor															*/
/****************************************************************************************************/
BYTE priority_call_below (BYTE fl, BYTE type, BYTE max){
	BYTE i;

	if (!(handle_calls & type))								// dont handle this calls now (higher priority)
		return (0);											// ignore call
	if (max > fl)
		max = fl;
	for (i = 0; i <= (fl - max); i++)						// search priority call below
	{
		if (calltab [fl - i].calltype & type)				// priority call
		{
			next_callfloor = fl - i;
			next_calldoors = get_calldoor (fl - i, type);	// set doors for this call
			next_calltype = type;							// set next call type
			next_calltime = calc_calltime (fl - i);			// calculate time to process this call
			if (compare_group ())							// compare priority call with other lifts
				return (1);
		}
	}
	return (0);
}

/****************************************************************************************************/
/* search priority calls																			*/
/****************************************************************************************************/
BYTE search_priority_call (BYTE fl, BYTE dir){
	BYTE i;
	BYTE type = 0;

	for (i = 0; i < 4; i++)
	{
		switch (i)
		{
			case (0):
				type = PRIOR_CARCALL;
				break;

			case (1):
				type = EMERGENCY_CALL;
				break;

			case (2):
				type = ADVANCED_CALL;
				break;

			case (3):
				type = SPECIAL_CALL;
				break;
		}
		if (dir != DIR_DN)		
		{
			if (priority_call_above (fl, type, p.top_floor))
				return (1);									// priority call above actual floor
		}
		if (priority_call_below (fl, type, p.bot_floor))
			return (1);										// priority call below actual floor
		if (dir == DIR_DN)
		{
			if (priority_call_above (fl, type, p.top_floor))
				return (1);									// priority call above actual floor
		}
	}
	return (0);
}

/****************************************************************************************************/
/* search call in actual floor																		*/
/****************************************************************************************************/
BYTE call_act_floor (BYTE fl, BYTE type, BYTE dir){
	if (calltab [fl].calltype & type & handle_calls)		// car or landing call up in actual floor
	{
		if (get_groupcall (fl, dir))
			return (1);
	}
	return (0);
}

/****************************************************************************************************/
/* search car call in actual floor																	*/
/****************************************************************************************************/
BYTE car_call_act_floor (BYTE fl){
	if (calltab [fl].calltype & CARCALL & handle_calls)
	{														// car call in actual floor
		next_callfloor = fl;								// set call floor to actual floor
		next_calldoors = calltab [fl].cc_door;				// set doors for car calls
		next_calltype = CARCALL;							// set type of call
		next_calltime = calc_calltime (fl);					// calculate time to process this call
		return (1);											// return call for actual floor
	}
	return (0);
}

/****************************************************************************************************/
/* search up call above actual floor																*/
/****************************************************************************************************/
BYTE up_call_above (BYTE fl, BYTE max, BYTE next){
	BYTE i;

	for (i = (fl + next); i <= max; i++)					// search up or car calls above actual floor
	{// car or landing call up
		if ((calltab [i].calltype & CALL_UP & handle_calls) &&
		((!call_bypass) || (i != (fl + next))))				// if bypass is on ignore call for next floor
		{
			if (get_groupcall (i, DIR_UP))
				return (1);
		}
	}
	return (0);
}

/****************************************************************************************************/
/* search down call below actual floor																*/
/****************************************************************************************************/
BYTE down_call_below (BYTE fl, BYTE max, BYTE next){
	BYTE i;

	if (max > fl)
		max = fl;
	for (i = next; i <= (fl - max); i++)					// search down or car calls below actual floor
	{// search from below actual to lowest floor
		if ((calltab [fl - i].calltype & CALL_DN & handle_calls) &&
		((!call_bypass) || (i != next)))					// if bypass is on ignore call for next floor
		{
			if (get_groupcall (fl - i, DIR_DN))
				return (1);
		}
	}
	return (0);
}

/****************************************************************************************************/
/* search down call above actual floor																*/
/****************************************************************************************************/
BYTE down_call_above (BYTE fl, BYTE max, BYTE next){
	BYTE i;

	for (i = max; i >= (fl + next); i--)					// search down landing calls above actual floor
	{														// search from highest to above actual floor
		if ((calltab [i].calltype & CALL_DN & handle_calls) &&
		((!call_bypass) || (i != (fl + next))))				// if bypass is on ignore call for next floor
		{
			if (get_groupcall (i, DIR_DN))
				return (1);
		}
	}
	return (0);
}

/****************************************************************************************************/
/* search up call below actual floor																*/
/****************************************************************************************************/
BYTE up_call_below (BYTE fl, BYTE max, BYTE next){
	BYTE i;

	for (i = max; (i + next) <= fl; i++)					// search up landing calls below actual floor
	{														// search from lowest to below actual floor
		if ((calltab [i].calltype & CALL_UP & handle_calls) &&
		((!call_bypass) || ((i + next) != fl)))				// if bypass is on ignore call for next floor
		{
			if (get_groupcall (i, DIR_UP))
				return (1);
		}
	}
	return (0);
}

/****************************************************************************************************/
/* search standard calls																			*/
/****************************************************************************************************/
BYTE search_standard_call (BYTE fl, BYTE dir, BYTE handle_lc){
	if (!(handle_calls & ALL_STANDARD_CALLS))				// dont handle standard calls
		return (0);

	switch (dir)
	{
		case (0):											// no actual call direction
			if (handle_lc)							// handle landing calls in actual floor
			{
				if (call_act_floor (fl, CALL_UP, DIR_UP))
					return (1);
				if (call_act_floor (fl, LANDINGCALL_DN, DIR_DN))
					return (1);
			}
			else if (car_call_act_floor (fl))				// handle only car calls in actual floor
				return (1);									// return call for actual floor
			if (up_call_above (fl, p.top_floor, 1))			// search up or car calls above actual floor
				return (1);
			if (down_call_below (fl, p.bot_floor, 1))		// search down or car calls below actual floor
				return (1);
			if (down_call_above (fl, p.top_floor, 1))		// search down calls above actual floor
				return (1);
			if (up_call_below (fl, p.bot_floor, 1))			// search up calls below actual floor
				return (1);
			return (0);

		case (DIR_UP) :										// actual call direction is up direction
			if (handle_lc)									// handle landing calls in actual floor
			{
				if (call_act_floor (fl, CALL_UP, DIR_UP))
					return (1);
			}
			else if (car_call_act_floor (fl))				// handle only car calls in actual floor
				return (1);									// return call for actual floor
			if (up_call_above (fl, p.top_floor, 1))			// search up or car calls above actual floor
				return (1);
			if (down_call_above (fl, p.top_floor, 1))		// search down calls above actual floor
				return (1);
			return (0);

		case (DIR_DN) :										// actual call direction is down direction
			if (handle_lc)									// handle landing calls in actual floor
			{
				if (call_act_floor (fl, CALL_DN, DIR_DN))
					return (1);
			}
			else if (car_call_act_floor (fl))				// handle only car calls in actual floor
				return (1);									// return call for actual floor
			if (down_call_below (fl, p.bot_floor, 1))		// search down or car calls below actual floor
				return (1);
			if (up_call_below (fl, p.bot_floor, 1))			// search up calls below actual floor
				return (1);
			return (0);
	}
	return (0);
}

/****************************************************************************************************/
/* Search next call while lift is standstill, slowing down or starting								*/
/****************************************************************************************************/
void search_call (BYTE calldir, BYTE fl, BYTE handle_lc){
	if (!search_priority_call (fl, calldir))				// search new priority call
	{// no priority call
		if (!search_standard_call (fl, calldir, handle_lc))
		{// search new standard call
			next_calltype = 0;								// no call
			next_calltime = 0;								// set call time to 0
			next_callfloor = 0xFF;
			next_calldoors = 0;
		}
	}
	if ((calltype_old != next_calltype) ||					// other call than before
	(callfloor_old != next_callfloor) ||
	(calldoors_old != next_calldoors))
	{
		groupcalldelay = mstimer + 40;					// start group control delay timer
		calltype_old = next_calltype;						// save old values
		callfloor_old = next_callfloor;
		calldoors_old = next_calldoors;
	}
}

/****************************************************************************************************/
/* Handle next call while lift is standstill, slowing down or starting								*/
/****************************************************************************************************/
BYTE get_call (BYTE lc_dir, BYTE fl, BYTE door, BYTE handle_lc){
	BYTE target_door = 0;
	
	if ((callfloor == fl) && calltype && (calltype & calltab [fl].calltype))
	{
		if ((!(door & calldoors & p.doorpos [fl])) ||		// all called doors opened
			door_off)	// or no door opening	
		{
			target_door = p.doorpos[fl];
			switch (calltype)
			{
				case (CARCALL):								// car call for actual floor
				case (PRIOR_CARCALL):				// priority car call for actual floor
					if((!(p.custumer1 & CUSTUMER1_SHE_MODE) || (target_door != (DOOR1 | DOOR2))))
						{
							set_out (CAR_CALL, fl + 1, 0, EXISTING_DOORS, 0, (O_CANA|O_CANB|O_HSE));
						}
					parkcall = 0; 						// clear also park call
					calltab [fl].cc_door = 0; 			// clear all doors from car call
					calltab [fl].calltype &= ~(CARCALL | PRIOR_CARCALL);
					if (p.cc_no_ls)							// watch number of calls without lightscreen
						carcallcounter++;					// increment counter
					break;

				case (EMERGENCY_CALL):						// emergency call for actual floor
					set_out (HALL_CALL_EMERGENCY, fl + 1, 0, EXISTING_DOORS, 0, O_ALL);
					calltab [fl].ec_door = 0;				// clear all doors from emergency call
					callpriority = C_EMERGENCY;				// set new call priority
					callprioritytimer = timer + (p.time_emergency SEC);
					callenabledtimer = timer + (p.calltime_emergency SEC);
					if (p.carcall_emergency_lc)				// clear car calls in emergency mode
						clearcalls (CARCALL | PRIOR_CARCALL);		// clear all car calls
					if (p.hallcall_emergency)				// clear hall calls in emergency mode
						clearcalls (ALL_HALL_CALLS);
					break;									// set priority timer

				case (ADVANCED_CALL):						// advanced call for actual floor
					set_out (HALL_CALL_ADVANCED, fl + 1, 0, EXISTING_DOORS, 0, O_ALL);
					calltab [fl].ac_door = 0;				// clear all doors from advanced call
					callpriority = C_ADVANCED;				// set new call priority
					callprioritytimer = timer + (p.time_advanced SEC);
					callenabledtimer = timer + (p.calltime_advanced SEC);
					if (p.carcall_advanced_lc)				// clear car calls in advanced mode
						clearcalls (CARCALL | PRIOR_CARCALL);		// clear all car calls
					if (p.hallcall_advanced)				// clear hall calls in advanced mode
						clearcalls (ALL_HALL_CALLS);
					break;									// set priority timer

				case (SPECIAL_CALL):						// special call for actual floor
					set_out (HALL_CALL_SPECIAL, fl + 1, 0, EXISTING_DOORS, 0, O_ALL);
					calltab [fl].sc_door = 0;				// clear all doors from special call
					callpriority = C_SPECIAL;				// set new call priority
					callprioritytimer = timer + (p.time_special SEC);
					callenabledtimer = timer + (p.calltime_special SEC);
					if (p.carcall_special_lc)				// clear car calls in special mode
						clearcalls (CARCALL | PRIOR_CARCALL);		// clear all car calls
					if (p.hallcall_special)					// clear hall calls in special mode
						clearcalls (ALL_HALL_CALLS);
					break;									// set priority timer

				case (LANDINGCALL_UP):						// landing call up for actual floor
					landingcall_actfloor = landingcall_push;
					if((!(p.custumer1 & CUSTUMER1_SHE_MODE) || (target_door != (DOOR1 | DOOR2))))
						{
							set_out (HALL_CALL, HALL_CALL_UP, fl + 1, EXISTING_DOORS, 0, O_ALL);
						}
					calltab [fl].lcu_door = 0;	// clear landing call up and all it doors
					if ((!p.dir_pending) ||			// not direction pending call cancelling
							(car_attendance))				// if car attendance always direct. independend
					{
						if (calltab [fl].calltype & LANDINGCALL_DN)
						{									// also landing call down for actual floor
							if ((!(door & calltab [fl].lcd_door & p.doorpos [fl]))
									|| door_off)					// all doors open or no door opening
							{
								set_out (HALL_CALL, HALL_CALL_DOWN, fl + 1, EXISTING_DOORS, 0, O_ALL);
								calltab [fl].calltype &= ~LANDINGCALL_DN;
								calltab [fl].lcd_door = 0;	// clear also landing call down and all it doors
							}
						}
					}
					break;

				case (LANDINGCALL_DN):						// landing call down for actual floor
					landingcall_actfloor = landingcall_push;
					if((!(p.custumer1 & CUSTUMER1_SHE_MODE) || (target_door != (DOOR1 | DOOR2))))
						{
							set_out (HALL_CALL, HALL_CALL_DOWN, fl + 1, EXISTING_DOORS, 0, O_ALL);
						}
					calltab [fl].lcd_door = 0;	// clear landing call down and all it doors
					if ((!p.dir_pending) ||			// not direction pending call cancelling
						(car_attendance)) 				// if car attendance always direct. independend
					{
						if (calltab [fl].calltype & LANDINGCALL_UP)
						{									// also landing call up for actual floor
							if ((!(door & calltab [fl].lcu_door & p.doorpos [fl]))
									|| door_off)					// all doors open or no door opening
							{
								set_out (HALL_CALL, HALL_CALL_UP, fl + 1, EXISTING_DOORS, 0, O_ALL);
								calltab [fl].calltype &= ~LANDINGCALL_UP;
								calltab [fl].lcu_door = 0;	// clear also landing call up and all it doors
							}
						}
					}
					break;

				case (LANDINGCALL):							// landing call down for actual floor
					landingcall_actfloor = landingcall_push;
					if((!(p.custumer1 & CUSTUMER1_SHE_MODE) || (target_door != (DOOR1 | DOOR2))))
						{
							set_out (HALL_CALL, HALL_CALL_NO_DIR, fl + 1, EXISTING_DOORS, 0, O_ALL);
						}
					calltab [fl].lc_door = 0;				// clear landing call and all it doors
					break;
			}
			calltab [fl].calltype &= ~calltype;				// clear actual call in call table
		}
	}

	search_call (calldir, fl, handle_lc);					// search next possible floor
	if ((groupcalldelay <= mstimer) || dir_attendance)		// group call delay time over; set calls valid
	{// if direction from attendance no delay 
		calltype = next_calltype;
		callfloor = next_callfloor;
		calldoors = next_calldoors;
		dir_attendance = 0;
	}
	if (calltype)											// save last active call
	{
		lastcalltype = calltype;
		lastcallfloor = callfloor;
		lastcalldoors = calldoors;
	}
	switch (calldir)										// search next call
	{
		case (0) :											// actual no call direction
			if (calltype & PRIORITY_CALL)					// valid priority call found
			{
				if (callfloor == fl)						// priority call for actual floor
				{
					if (calltab [fl].calltype & calltype)	// call still not cleared
						return (CALL_ACT_FLOOR);
					else
						return (NO_CALL);
				}
				else if (callfloor > fl)					// priority call above actual floor
					calldir = DIR_UP;						// set new call direction
				else										// priority call below actual floor
					calldir = DIR_DN;						// set new call direction
				return (NEW_CALL);
			}
			if (calltype)									// valid standard call found
			{
				if (callfloor == fl)						// call for actual floor
				{
					if (calltype & LANDINGCALL_UP)			// landing call in up direction
						calldir = DIR_UP;					// set new call direction
					if (calltype & LANDINGCALL_DN)			// landing call in down direction
						calldir = DIR_DN;					// set new call direction
					if (calltab [fl].calltype & calltype)	// call still not cleared
						return (CALL_ACT_FLOOR);			// car call or landing call without direction
					else
						return (NO_CALL);
				}
				else if (callfloor > fl)					// call above actual floor
					calldir = DIR_UP;						// set new call direction
				else										// priority call below actual floor
					calldir = DIR_DN;						// set new call direction
				return (NEW_CALL);
			}
			break;

		case (DIR_UP) :										// actual call direction is up direction
			if (calltype & PRIORITY_CALL)					// valid priority call found
			{
				if (callfloor > fl)							// priority call above actual floor
					return (NEW_CALL);						// return call
				else										// priority call below actual floor
				{
					calldir = 0;							// clear call direction
					return (NO_CALL);						// don't return this call now
				}
			}
			if (calltype)									// valid standard call found
			{
				if (callfloor == fl)						// call for actual floor
				{
					if (calltab [fl].calltype & calltype)	// call still not cleared
						return (CALL_ACT_FLOOR);
					else
						return (NO_CALL);
				}
				else if (callfloor > fl)					// call above actual floor
					return (NEW_CALL);
			}
			if (!(lc_dir & LANDINGCALL_UP))					// hold time for landing call direction over
				calldir = 0;								// clear call direction
			break;

		case (DIR_DN) :										// actual call direction is down direction
			if (calltype & PRIORITY_CALL)					// valid priority call found
			{
				if (callfloor < fl)							// priority call below actual floor
					return (NEW_CALL);						// return call
				else										// priority call above actual floor
				{
					calldir = 0;							// clear call direction
					return (NO_CALL);						// don't return this call now
				}
			}
			if (calltype)									// valid standard call found
			{
				if (callfloor == fl)						// call for actual floor
				{
					if (calltab [fl].calltype & calltype)	// call still not cleared
						return (CALL_ACT_FLOOR);
					else
						return (NO_CALL);
				}
				else if (callfloor < fl)					// call below actual floor
					return (NEW_CALL);
			}
			if (!(lc_dir & LANDINGCALL_DN))					// hold time for landing call direction over
				calldir = 0;								// clear call direction
			break;
	}

	callfloor = fl;
	calltype = 0;
	calldoors = 0;
	return (NO_CALL);
}

/****************************************************************************************************/
/* Search next call while lift is moving															*/
/****************************************************************************************************/
BYTE get_moving_call (BYTE next_fl){
	BYTE i;

	calldir = direction;									// set actual drive direction as call direction
	if (direction == DIR_UP)								// lift is moving upwards
	{
		i = search_priority_call (next_fl, calldir);		// search new priority call
		if (!i)
			i = up_call_above (next_fl, max_target, 0);
		if (!i)
			i = down_call_above (next_fl, max_target, 0);
	}
	else													// lift is moving downwards
	{
		i = search_priority_call (next_fl, calldir);		// search new priority call
		if (!i)
			i = down_call_below (next_fl, max_target, 0);
		if (!i)
			i = up_call_below (next_fl, max_target, 0);
	}
	if (!i)													// no call found
	{
		next_calltype = 0;						// clear calltype
		next_calltime = 0;						// set call time to 0
		next_calldoors = 0;
		next_callfloor = next_fl;
		if (direction == DIR_UP)			// lift is moving upwards
		{
			for (i = next_fl; i <= max_target; i++)			// search for valid floor to stop
			{
				if (p.doorpos [i])							// lift has shaft doors in this floor
				{
					callfloor = i;							// stop in next valid floor
					break;
				}
			}
		}
		else
		{
			for (i = 0; i <= (next_fl - max_target); i++)	// search for valid floor to stop
			{
				if (p.doorpos [next_fl - i])				// lift has shaft doors in this floor
				{
					callfloor = next_fl - i;				// stop in next valid floor
					break;
				}
			}
		}
	}
	if ((calltype_old != next_calltype) ||					// other call than before
	(callfloor_old != next_callfloor) ||
	(calldoors_old != next_calldoors))
	{
		groupcalldelay = mstimer + 40;							// start group control delay timer
		calltype_old = next_calltype;						// save old values
		callfloor_old = next_callfloor;
		calldoors_old = next_calldoors;
	}
	if ((next_calltype & PRIORITY_CALL) &&					// new call is priority call
	(next_calltype != calltype))							// and call type changed
	{
		switch (next_calltype)
		{
			case (PRIOR_CARCALL) :
				i = 0;										// dont finish last trip
				break;

			case (EMERGENCY_CALL):
				i = p.last_trip_emergency;					// finish last trip if parameter is set
				break;

			case (ADVANCED_CALL) :
				i = p.last_trip_advanced;					// finish last trip if parameter is set
				break;

			case (SPECIAL_CALL) :
				i = p.last_trip_special;					// finish last trip if parameter is set
				break;
		}
		if (i)												// finish last trip
			groupcalldelay = mstimer + 40;						// retrigger group control delay timer
		else if (groupcalldelay <= mstimer)					// group call delay time over; set calls valid
		{
			if (direction == DIR_UP)						// lift is moving upwards
			{
				if ((next_callfloor > max_target) ||		// cant go to this floor with this speed
				(next_callfloor < next_fl))					// or too late to stop in this floor
				{
					calltype = 0;							// clear call
					callfloor = next_fl;					// stop in next floor
					calldoors = 0;						//²»¿ªÃÅ
					return (callfloor);						// return next floor to stop
				}
			}
			else											// lift is moving downwards
			{
				if ((max_target > next_callfloor) ||		// cant go to this floor with this speed
				(next_fl < next_callfloor))					// or too late to stop in this floor
				{
					calltype = 0;							// clear call
					callfloor = next_fl;					// stop in next floor
					calldoors = 0;
					return (callfloor);						// return next floor to stop
				}
			}
		}
	}

	if (groupcalldelay <= mstimer)							// group call delay time over; set calls valid
	{
		calltype = next_calltype;
		callfloor = next_callfloor;
		calldoors = next_calldoors;
	}
	return (callfloor);										// return next floor to stop
}

