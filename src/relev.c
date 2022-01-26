
#include "typedef.h"
#include "parameter.h"
#include "nice5000.h"
#include "state.h"
#include "call.h"
#include "can.h"
#include "drive.h"
#include "pos.h"
#include "history.h"


/****************************************************************************************************/
/* Re-levelling function																	*/
/****************************************************************************************************/
BYTE relevelling (BYTE state, BYTE call)
{
	static DWORD relev_timer;
	static BYTE levelswitch = 0;
	static BYTE relev_dir = 0;

	if (!state)										/* no re-levelling active					*/
	{
		if ((door_zone & (POS_SGU | POS_SGD)) == (POS_SGU | POS_SGD))
		{
			relev_timer = timer + 3 SEC;				/* lift inside level zone					*/
			relevelnumber = 0;					/* clear number of not successfull relevelling	*/
		}
		else if ((relev_timer < timer) &&				/* lift outside level zone, debounce time over*/
			p.relevel &&								/* lift shell re-level (parameter)			*/
			(!overload) &&					/* no overload or hydraulic lift				*/
			(!relev_new) &&
			(relevelnumber < 6) &&						/* not more than 6 attemps to relevel		*/
			(start_enabled ()) &&						/* start not disabled (by evacuation input)	*/
			(door_zone & (POS_SGU | POS_SGD)) && (dz_input) && 	//增加门旁路装置的输入检测
			(!(temperature & (OVERTEMP1 | OVERTEMP2 | ROOMTEMP_MIN | ROOMTEMP_MAX))) &&
			((nice5000_Rcmd_data2 & NICE5000_RCMD2_STATE) == RCMD2_STATE_RELEVEL) &&
			(dl_short_test_finish || (!check_dl_short)) && 	//门锁短接检测完成之后才可以再平层
			((call == NO_CALL) || p.relevel_trip))			/* no call or re-level enabled before trip		*/
		{			
			if (door_zone & POS_SGU)				/* lift below level zone					*/
				relev_dir = DIR_UP;
			else if (door_zone & POS_SGD)			/* lift above level zone					*/
				relev_dir = DIR_DN;
			start_drive (relev_dir);				/* start drive								*/
			check_ilu_ild();
			relevelflag = 1;
			relevelnumber++;
			levelswitch = 0;						/* SGU or SGO off						*/
			relev_timer = timer + (p.triptime SEC);		/* set maximum re-levelling trip time		*/
			state = 1;
		}		
	}
	else											/* re-levelling active						*/
	{
		if (drive_state == D_STOPPED)
		{
			if (((door_zone & (POS_SGU | POS_SGD)) != (POS_SGU | POS_SGD))
			&& (state != 2))
			{
				if (((relev_dir == DIR_UP) && (!(door_zone & POS_SGD))) ||
				((relev_dir == DIR_DN) && (!(door_zone & POS_SGU))))
					write_errorhist (E_RELEV_SHORT, HSE_ID, 0, W_RELEV_SHORT);
				else								/* lift stopped too late					*/
					write_errorhist (E_RELEV_LONG, HSE_ID, 0, W_RELEV_LONG);
			}
			nice5000_Tcmd_data2 = (nice5000_Tcmd_data2 & (~(NICE5000_TCMD2_STATE|NICE5000_TCMD2_TARGET))) | TCMD2_STATE_RELEVEL | level;
			relev_timer = timer + 3 SEC;				/* re-levelling debouncing 3 sec			*/
			state = 0;
		}
		else if ((drive_state != D_STOPPING) &&		/* lift not in stopping state					*/
		(drive_state != D_SWITCHED_OFF) &&		/* and lift not in switched off state			*/
		(drive_state != D_WAIT_STOP) &&			/* and lift not in waiting for astop state		*/
		(drive_state != D_OPERATION_DISABLED))	/* and lift not in operation disabled state	*/
		{
			if (relev_timer < timer)					/* time out while re-levelling				*/
			{									/* to be done							*/
				emergency_stop ();				/* send emergency stop					*/
				general_error |= G_TRIPTIME;		/* set trip time exceeded error				*/
				write_errorhist (E_TRIP_TIME, HSE_ID, C_TRIP_TIME, 0);
			}
			else if (direction == DIR_UP)				/* Lift is moving upwards					*/
			{
				if (!levelswitch)						/* switch SGU was off						*/
				{
					if (door_zone & POS_SGD)		/* SGU now on							*/
					{
						levelswitch = 1;				/* set position to position of level switch	*/
						stop_drive ();
					}
				}
			}
			else									/* Lift is moving downwards				*/
			{
				if (!levelswitch)						/* switch SGO was off						*/
				{
					if (door_zone & POS_SGU)		/* SGO now on							*/
					{
						levelswitch = 1;				/* set position to position of level switch	*/
						stop_drive ();
					}
				}
			}
		}		
	}

	return (state);
}


