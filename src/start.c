
#include "typedef.h"
#include "parameter.h"
#include "state.h"
#include "drive.h"
#include "call.h"
#include "can.h"
#include "pos.h"
#include "eeprom.h"
#include "history.h"
#include "io.h"
#include "nice5000.h"


/****************************************************************************************************/
/* Moving in normal mode																		*/
/****************************************************************************************************/
void start_trip_state (void){
	BYTE call;
	BYTE door;
	BYTE i;
	BYTE doorcommand [MAX_DOOR];
	static BYTE doorerrorcount = 0;
	static BYTE save_tripcount = 0;
	static DWORD limitcount_old = 0;

	ct = timer + (p.starttime / 100);				/* wait for safety circuit door debouncing		*/
	start_floor = level;								/* save start floor							*/
	door_command (CLOSE_DOOR, CLOSE_DOOR);		/* send door command to make sure door is closed*/
	for (i = 0; i < MAX_DOOR; i++)
		doorcommand [i] = DOOR_OFF;				/* door don't care							*/
	fault_signal = 0;								/* reset fault signal							*/
	out_of_order = 0;								/* reset out of order indication					*/
	
	while (ct > timer)
	{
		cycle ();
		special_trip ();								/* handle special trips							*/
		call = get_call (0, level, ALL_DOORS_CLOSED, 0);
		set_state ();								/* set new state								*/
		if (hse_state != H_START)					/* other state								*/
			return;
	}
	ct = timer + 10 SEC;								/* wait max. 5s for safety circuit door			*/
	__nop(); __nop(); __nop(); __nop();
	do											/* do this program part at least 1 time			*/
	{
		cycle ();
		special_trip ();								/* handle special trips							*/
		set_state ();								/* set new state								*/
		if (hse_state != H_START)					/* other state								*/
			return;
		call = get_call (0, level, ALL_DOORS_CLOSED, 0);
		if (ct < timer)								/* time out while waiting for doors				*/
		{
			doorerrorcount++;
			if (doordelay < 250)					/* increase door delay for group control			*/
				doordelay += 5;
			if (doorerrorcount == MAX_DOOR_ERROR)	/* every 4. time								*/
			{
				clearcalls (ALL_CALLS);				/* clear all calls								*/
				write_errorhist (E_SC_VOLTAGE_DOOR, HSE_ID, 0, W_SC_SHAFTDOOR);
				doorerrorcount = 0;
			}
			door = p.doorpos[level];

			if((p.cooperate_sel1 & COOP1_THROUGH_DOOR) && 
					(door == (DOOR1 | DOOR2)))
				{
					if(doorerrorcount % 2)
						door = DOOR1;
					else
						door = DOOR2;
				}
			if((doorstate & DOOR1) == 0)
				move_dooropen_flag = 1;
			else if((doorstate & DOOR2) == 0)
				move_dooropen_flag = 2;
			
			if (callpriority == C_FIREMAN)
				door = 0;
			if (door)								/* door to open								*/
			{
				for (i = 0; i < MAX_DOOR; i++)
				{
					if (bit_select (door, i))			/* check if door is available on this floor		*/
					{
						doorcommand [i] = OPEN_DOOR;		/* open door							*/
						bit_reset (doorstate, i);				/* mark door as not closed			*/
					}
					else
						doorcommand [i] = DOOR_OFF;		/* door don't care					*/
				}
				door_command (doorcommand [0], doorcommand [1]);
				hse_state = H_STANDSTILL;					/* go to state standstill								*/
				return;
			}
			else											/* no shaft doors on this floor			*/
			{
				hse_state = H_STANDSTILL;
				return;
			}
		}
		if (handle_dooropenpush ())							/* handle door open push				*/
		{
			hse_state = H_STANDSTILL;						/* if doors to open go to standstill		*/
			return;
		}
		if ((call != NEW_CALL) ||							/* no call from other floor				*/
			(!calldir)	 ||										/* no call direction					*/
			(callfloor == level))								/* call from actual floor				*/
		{
			hse_state = H_STANDSTILL;						/* go to standstill						*/
			return;
		}
	}
	while ((doorstate != ALL_DOORS_CLOSED) ||				/* Door is open						*/
		(!(safety_circuit & SC1)) || (!(safety_circuit & SC2)) ||		/* or safety circuit car door is off		*/
		(!(safety_circuit & SC3)));							/* or safety circuit shaft door is off		*/

	target_floor = callfloor;									/* set target floor						*/

	if(p.forced_stop)
		{
			if (((p.forced_starttime < p.forced_stoptime) &&
					(t.hour >= p.forced_starttime) && (t.hour < p.forced_stoptime)) ||
					((p.forced_starttime > p.forced_stoptime) &&
					((t.hour >= p.forced_starttime) || (t.hour < p.forced_stoptime))))
				forced_stop |= FORCED_TIME;
			else				
				forced_stop &= ~FORCED_TIME;
		}
	else
		forced_stop &= ~FORCED_TIME;

	if ((forced_stop) &&									/* forced stop enabled				*/
	((calls_enabled & ALL_HALL_CALLS) == ALL_HALL_CALLS))
	{													/* landing calls not disabled			*/
		if (((p.forced_stop_floor > level) && (p.forced_stop_floor < callfloor) && (p.forced_stop_dir & HALL_CALL_UP)) ||
		((p.forced_stop_floor < level) && (p.forced_stop_floor > callfloor) && (p.forced_stop_dir & HALL_CALL_DOWN)))
		{												/* set call for forced stop floor			*/
			calltab [p.forced_stop_floor].calltype |= CARCALL;
			calltab [p.forced_stop_floor].cc_door |= p.forced_stop_door;
			call = get_call (0, level, ALL_DOORS_CLOSED, 0);
		}
	}
	max_target = get_max_target (calldir);
	next_floor = ((nice5000_Rcmd_data3 & NICE5000_RCMD3_PSTOPFLOOR) >> 8) - 1;
	if ((call != NEW_CALL) ||								/* no call from other floor				*/
	(!calldir) ||											/* no call direction					*/
	(callfloor == level))									/* call from actual floor				*/
	{
		hse_state = H_STANDSTILL;							/* go to standstill						*/
		return;
	}
	if (((door_zone & POS_ILU) && (calldir == DIR_UP)) ||
		((door_zone & POS_ILD) && (calldir == DIR_DN)))
	{
		clearcalls (ALL_CALLS);
		hse_state = H_STANDSTILL;
		return;
	}
	
	if (((force_switch & FORCE_SDU1) && (calldir == DIR_UP)) ||
		((force_switch & FORCE_SDD1) && (calldir == DIR_DN)))
	{
		clearcalls (ALL_CALLS);
		hse_state = H_STANDSTILL;
		return;
	}

	target_floor %= (p.top_floor + 1);
	nice5000_Tcmd_data2 = (nice5000_Tcmd_data2 & (~(NICE5000_TCMD2_STATE|NICE5000_TCMD2_TARGET))) | TCMD2_STATE_OTHER | (target_floor + 1);
	while(1)
	{
		cycle ();
		special_trip ();
		set_state ();
		if (hse_state != H_START)
			return;
		if ((nice5000_Rcmd_data2 & NICE5000_RCMD2_STATE) == RCMD2_STATE_OTHER)
			break;
	}
	
	tripcount++;											/* increment trip counter 			*/
	if (((tripcount > 1000) && (!(tripcount % 100))) ||
			((tripcount > 100) && (!(tripcount % 10))) ||
			(tripcount <= 100)) 								/* save trip counter					*/
		save_tripcount = 1;
	if (save_tripcount)
		{
			WriteParaData (&tripcount, EE_TRIPCOUNT, sizeof (tripcount));
			WriteParaData (floorcount, EE_FLOORCOUNT, sizeof (floorcount));
			save_tripcount = 0;
		}
	if (save_op_time) 									/* save operation time				*/
		{
			WriteParaData (&op_time, EE_OPERTIME, sizeof (op_time));
			save_op_time = 0;
		}
	if((p.limitenable) && p.limittimes && (!testtrip) && (callpriority == C_STANDARD))
		tripcount_limit ++; 	
	if(tripcount_limit != limitcount_old)
		{ 		
			WriteParaData(&tripcount_limit, EE_TRIPCOUNT_LIMIT, sizeof(tripcount_limit));
			limitcount_old = tripcount_limit;
		}  

	if (triperrorcount && (triperrorcount < MAX_TRIP_ERROR))
		triperrorcount--;
	if (errorwarning && (errorwarning < ERROR_WARNINGLIMIT))
		errorwarning--;

	doorerrorcount = 0;
	move_dooropen_flag = 0;

	start_drive (calldir);									/* start drive							*/
	fan_on ();
	hse_state = H_MOVING;									/* go to moving state					*/
}


