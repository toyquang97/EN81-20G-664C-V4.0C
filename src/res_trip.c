
#include "typedef.h"
#include "parameter.h"
#include "nice5000.h"
#include "state.h"
#include "call.h"
#include "can.h"
#include "drive.h"
#include "pos.h"
#include "io.h"
#include "history.h"


/****************************************************************************************************/
/* Reset trip state state																	*/
/****************************************************************************************************/
void reset_trip_state (void)
{
	WORD doorzone_state = 0;
	BYTE dir;
	BYTE reset_finished = 0;
	DWORD delaytimer = 0;
	DWORD stoptimer = 0;
	BYTE stop = 0;
	BYTE cmd = 0;
	BYTE floor = 0;

	calls_off = 0;										/* enable calls						*/
	she_calldoor_double = 0;

	door_command (CLOSE_DOOR, CLOSE_DOOR);

	if (force_switch & FORCE_SDD1)
		{
			if(door_zone & POS_SGM)
				{
					doorzone_state = 1;
					floor = p.bot_floor;
					ct = timer + 3 SEC; 								/* wait 1 seconds 					*/
				}
		}
	else if (force_switch & FORCE_SDU1)
		{
			if(door_zone & POS_SGM)
				{
					doorzone_state = 1;
					floor = p.top_floor;
					ct = timer + 3 SEC; 								/* wait 1 seconds 					*/
				}
		}
	else
		doorzone_state = 0;
	if(doorzone_state)
		{//当前电梯已经处于端站的门区内
			do
				{
					cycle ();
					set_state (); 								/* set new state						*/
					if((level == floor) && !(nice5000_Rcmd_data4 & NICE5000_RCMD4_POSERROR)
							|| (hse_state != H_RESET))
						{//已经纠正							
							bForceResetTrip = 0;
							insp_return = 0;
							bResetTripDoing = 0;
							set_state (); 								/* set new state						*/
							return;
						}
				}
			while((ct > timer) && (door_zone & POS_SGM) &&
					((force_switch & FORCE_SDD1) || (force_switch & FORCE_SDU1)));
		}

	ct = timer + 1 SEC;									/* wait 1 seconds						*/
	nice5000_Tcmd_data2 = (nice5000_Tcmd_data2 & (~(NICE5000_TCMD2_STATE|NICE5000_TCMD2_TARGET))) | TCMD2_STATE_RESET_TRIP;
	start_floor = level;
	if (force_switch & FORCE_SDD1)
		{
			target_floor = p.top_floor;
			dir = DIR_UP;
		}
	else if(force_switch & FORCE_SDU1)
		{
			target_floor = p.bot_floor;
			dir = DIR_DN;
		}	
	else
		{//根据中心楼层判定方向
			floor = p.top_floor >> 1;
			if(level > floor)
				{					
					target_floor = p.top_floor;
					dir = DIR_UP;
				}
			else
				{
					target_floor = p.bot_floor;
					dir = DIR_DN;
				}
		}
	target_floor %= (p.top_floor + 1);
	nice5000_Tcmd_data2 = (nice5000_Tcmd_data2 & (~NICE5000_TCMD2_TARGET)) | (target_floor + 1);
	send_hse_state ();	
	clearcalls (ALL_CALLS);									/* clear all calls		*/
	cmd = 0;
	
	bResetTripDoing = 1;
	
	while ((ct > timer)
				|| (!start_enabled ())
				|| ((nice5000_Rcmd_data2 & NICE5000_RCMD2_STATE) != RCMD2_STATE_RESET_TRIP))
		{
			cycle ();
			set_state ();									/* set new state						*/
			if (hse_state != H_RESET)			/* state changed						*/
				{	
					if((door_zone & (POS_ILU | POS_ILD)) && (door_zone & POS_SGM))
						{				
							bForceResetTrip = 0;
							insp_return = 0;
							bResetTripDoing = 0;
						}
					return;								/* go to new state					*/
				}		
		}
	if ((force_switch & FORCE_SDU1) && (force_switch & FORCE_SDD1))
		{
			if (!(poserror & P_VO_VU_ERROR))
				{
					poserror |= P_VO_VU_ERROR;
					write_errorhist (E_VO_VU_ERROR, HSE_ID, C_VO_VU_ON, W_VO_VU_ON);
				}
			set_state ();
			bResetTripDoing = 0;
			return;
		}
	ct = timer + 2 SEC;
	do											/* do this program part at least 1 time 		*/
		{
			cycle ();
			set_state (); 							/* set new state								*/
			if(ct < timer)
				{
					door_command (CLOSE_DOOR, CLOSE_DOOR);		//每2s发送一次关门命令
					ct = timer + 2 SEC;
				}
			if (hse_state != H_RESET) 				/* other state								*/
				return;
		}	while ((doorstate != ALL_DOORS_CLOSED) || 			/* Door is open 					*/
			(!(safety_circuit & SC1)) || (!(safety_circuit & SC2)) || 	/* or safety circuit car door is off		*/
			(!(safety_circuit & SC3))); 						/* or safety circuit shaft door is off		*/

	if(bResetTripDoing)		
		write_errorhist (E_RESET_TRIP, HSE_ID, C_RESET_TIP, 0);
	start_drive (dir);
	ct = timer + (p.triptime SEC);
	doorzone_state = door_zone;
	while (1)
	{
		check_ilu_ild();
		cycle ();
		set_state ();
		if (hse_state != H_RESET)
		{			
			if(((door_zone & (POS_ILU | POS_ILD)) && (door_zone & POS_SGM))
					|| (reset_finished))
				{
					bForceResetTrip = 0;
					insp_return = 0;
				}
			bResetTripDoing = 0;
			return;
		}
		if (ct < timer)
		{
			emergency_stop ();
			general_error |= G_TRIPTIME;
			write_errorhist (E_TRIP_TIME, HSE_ID, C_TRIP_TIME, 0);
			hse_state = H_WAIT_STOP;
			bResetTripDoing = 0;
			return;
		}
		if (dir == DIR_UP)
		{			
			if ((door_zone & POS_SDU1) && (door_zone & POS_SGM))
			{
				if (!stop)
				{
					stop = 1;
					stoptimer = timer + (p.rsttrip_stop_dly / 100);
				}
				else if ((stoptimer < timer) && (!cmd))
				{
					stop_drive();
					cmd = 1;
				}
			}
		}
		else if (dir == DIR_DN)
		{
			if ((door_zone & POS_SDD1) && (door_zone & POS_SGM))
			{
				if (!stop)
				{
					stop = 1;
					stoptimer = timer + (p.rsttrip_stop_dly / 100);
				}
				else if ((stoptimer <= timer) && (!cmd))
				{
					stop_drive();
					cmd = 1;
				}
			}
		}
		if ((doorzone_state & POS_SGM) != (door_zone & POS_SGM))
		{
			ct = timer + (p.triptime SEC);
			doorzone_state = (door_zone & POS_SGM);
		}
		if (drive_state == D_STOPPED)
		{
			if (!reset_finished)
			{
				bResetTripDoing = 0;
				reset_finished = 1;
				delaytimer = timer + 3 SEC;
			}
			else if (delaytimer < timer)
			{
				nice5000_poserr_flag = 0;
				bForceResetTrip = 0;
				insp_return = 0;
				bResetTripDoing = 0;

				if ((adt == DOOR_NORMAL) && (door_zone & POS_SGM))
				{
					dooropenpush = DOOR1 | DOOR2;
					handle_dooropenpush();
					dooropenpush = 0;
				}

				set_state ();
				return;
			}
		}		
		
		fan_on ();
	}
}

