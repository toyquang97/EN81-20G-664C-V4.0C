
#include "typedef.h"
#include "parameter.h"
#include "nice5000.h"
#include "state.h"
#include "call.h"
#include "can.h"
#include "drive.h"
#include "history.h"
#include "pos.h"
#include "io.h"

/****************************************************************************************************/
/* Moving in normal mode																	*/
/****************************************************************************************************/
void moving_state (void)
{
	WORD door_zone_state;
	BYTE calltarget;

	send_hse_state ();
	door_zone_state = door_zone;						/* save old state of door zone switches	*/
	ct = timer + (p.triptime SEC);							/* set trip time						*/
	calls_off = 0;										/* enable calls						*/
	fault_signal = 0;									/* reset fault signal					*/
	out_of_order = 0;									/* reset out of order indication			*/
	doordelay = 0;									/* set door delay for group control to 0	*/
	bFunc_flag &= ~FUNC_OVERSPEEP_ACTIVE;
	while (1)
	{
		check_ilu_ild ();
		check_speed();
		cycle ();
		set_state ();									/* set new state						*/
		if (hse_state != H_MOVING)						/* state changed						*/
			{
				floorcount [target_floor] ++;
				return;								/* go to new state					*/
			}
		if ((door_zone & POS_SGM) != (door_zone_state & POS_SGM))					/* state of door zone switches changed	*/
		{
			ct = timer + (p.triptime SEC);					/* retrigger trip time					*/
			door_zone_state = door_zone;				/* save old state of door zone switches	*/
		}
		if (ct < timer)									/* time out, lift did not stop			*/
		{
			emergency_stop ();						/* send emergency stop				*/
			general_error |= G_TRIPTIME;				/* set trip time exceeded error			*/
			write_errorhist (E_TRIP_TIME, HSE_ID, C_TRIP_TIME, 0);
			hse_state = H_WAIT_STOP;
			return;
		}
		special_trip ();									/* handle special trips					*/
		next_floor = ((nice5000_Rcmd_data3 & NICE5000_RCMD3_PSTOPFLOOR) >> 8) - 1;
		calltarget = get_moving_call (next_floor);
		if ((calltarget != start_floor) && (calltarget != 0xff))
		{
			target_floor = calltarget;
			if (direction == DIR_UP)							/* Lift is moving upwards				*/
			{
				if (target_floor < next_floor)					/* target floor in other direction		*/
					target_floor = next_floor;
			}
			else											/* Lift is moving downwards			*/
			{
				if (target_floor > next_floor)					/* target floor in other direction		*/
					target_floor = next_floor;
			}
		}

		if(evacuation)
			evaction_waittime_nomove = timer + (DWORD)p.evaction_waittime SEC;

		target_floor %= (p.top_floor + 1);
		nice5000_Tcmd_data2 = (nice5000_Tcmd_data2 & (~NICE5000_TCMD2_TARGET)) | (target_floor + 1);
		fan_on ();										/* cabin fan on						*/
	}
}


