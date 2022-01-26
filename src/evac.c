
#include "typedef.h"
#include "parameter.h"
#include "nice5000.h"
#include "state.h"
#include "call.h"
#include "can.h"
#include "drive.h"
#include "io.h"
#include "pos.h"
#include "history.h"

/****************************************************************************************************/
/* Evacuation with UPS power supply														*/
/****************************************************************************************************/
void evacuation_state (void)
{
	BYTE doorcommand = 0;
	BYTE door_closing = 0;
	BYTE shutoff_power = 0;
	BYTE stopcommand = 0; 
	BYTE half_load = 0;
	BYTE door = 0;
	BYTE i;
	BYTE startdir = 0;
	DWORD shutoff_delay = 0;
	DWORD stoptimer = 0;
	BYTE stop = 0;
	BYTE speaker_fg = 0;
	DWORD ups_run_time = 0;
	BYTE	start_fg = 0;

	send_hse_state ();
	calls_off = 1;										/* disable calls						*/
	ups_startcount = 0;	
	clearcalls (ALL_CALLS);								/* clear all calls						*/
	if (load & HALF_LOAD)
		half_load = 1;
	she_calldoor_double = 0;
	nice5000_Tcmd_data2 = (nice5000_Tcmd_data2 & (~(NICE5000_TCMD2_STATE|NICE5000_TCMD2_TARGET))) | TCMD2_STATE_EVACUATION;

	ct = timer + 1 SEC;	
	
	while(ct > timer)
		{
			check_ilu_ild();
			cycle ();
			set_state ();									/* set new state						*/
		}

	ct = timer + 2 SEC;									/* wait 2 seconds						*/

	while (1)
	{
		check_ilu_ild();
		cycle ();
		set_state ();									/* set new state						*/
		if (hse_state != H_EVACUATION)					/* state changed						*/
		{
			if (drive_state != D_STOPPED)				/* drive not in stopped state			*/
			{
				stop_drive();
			}
			if ((p.setup_ready) && (p.cooperate_sel1 & COOP1_INSP_RETURN))
				insp_return = 1;
			return;								/* go to new state					*/
		}
		if ((door_zone & POS_SGM) && 
		(((level == 0xFF) || (p.doorpos[level])) ||
		(poserror) || 								/* lift in level zone or positioning error	*/
//		(doorstate != ALL_DOORS_CLOSED) ||			/* or doors already opened			*/
		(doorcommand) ||								/* or already doorcommand 		*/
		(ups_startcount == 4))) 					/* or lift did try to go to level 		*/
		{
			if (drive_state == D_STOPPED)
			{
				if (!doorcommand)						/* no door command					*/
				{
					if (p.doornumber == 1)				/* lift has only 1 car door				*/
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
							set_door (door, OPEN_DOOR);				/* open doors									*/
							set_out (SPECIAL_FUNC, UPS_EVAC_READY_SPEAKER, 0, 0, 1, (O_CANA|O_CANB|O_HSE));		//轿厢显示器会显示"Overload",不好
							shutoff_delay = timer + 30 SEC;		//30S后切断UPS电源
						}
				}
				else
				{
					if(shutoff_delay < timer + 15 SEC)
						{//蜂鸣器鸣叫 15S 后关闭
							if(!speaker_fg)
								{
									speaker_fg = 1;
									set_out (SPECIAL_FUNC, UPS_EVAC_READY_SPEAKER, 0, 0, 0, (O_CANA|O_CANB|O_HSE)); 	//轿厢显示器会显示"Overload",不好
								}							
						}
					if ((shutoff_delay < timer) && ((door_state[0] == DOOR_OPENED) || (door_state[1] == DOOR_OPENED)))
					{
						if (!shutoff_power)
						{
							shutoff_power = 1;
							set_out (SPECIAL_FUNC, UPS_EVAC_READY, 0, 0, 1, O_HSE);		//等待 1 分钟关闭UPS输出
						}
					}
				}
				start_fg = 0;
			}
			else
				{
					if (!stop && p.doorpos [level])
						{
							stop = 1;
							stoptimer = timer + (p.upstrip_stop_delay / 50);
						}
					else if ((stoptimer <= timer) && (!stopcommand) && stop)
						{
							stop_drive();
							stopcommand = 1;
						}
				}
		}
		else if (((doorstate != ALL_DOORS_CLOSED) ||	/* not all doors closed				*/
		(!(safety_circuit & SC3)))						/* or safety circuit shaft door is off		*/
		&& (drive_state == D_STOPPED))
		{
			if (ct <= timer)								/* time over							*/
			{
				door_command (CLOSE_DOOR, CLOSE_DOOR);
				ct = timer + 60 SEC;					/* wait 60 seconds for door closed		*/
			}
			door_closing = 1;
		}
		else											/* lift not in level, door closed			*/
		{
			if (door_closing)
			{
				door_closing = 0;
				ct = timer + 2 SEC;						/* start delay 2 seconds				*/
			}
			if ((drive_state == D_STOPPED)
				&& (level != 0xff)
				&& ((nice5000_Rcmd_data2 & NICE5000_RCMD2_STATE) == RCMD2_STATE_EVACUATION))
			{
				if ((ct <= timer) && (ups_startcount < 4))	/* time over, not more than 4 starts		*/
				{
					start_floor = level;
					startdir = ((ups_startcount + half_load) % 2) + 1;
					if(p.ups_direction_mode & UPS_DIR_SEL)
						start_drive (0);
					else
						{
							startdir = get_target_floor_ups(startdir);
							nice5000_Tcmd_data2 |= target_floor + 1;
							start_drive (startdir);
						}
					ups_startcount ++;					/* start drive							*/
					stopcommand = 0;					
					start_fg = 1;
					ups_run_time = timer + ((p.triptime + 10) SEC);		/* set trip time						*/
					
					if(p.ups_direction_mode & UPS_ALARM)		//UPS救援时，轿厢间歇提示
						set_out(SPEAKER_BUZ, BUZZER_NORMAL, 2, SPEEKER_PAUSE, 1, O_CANA | O_CANB | O_HSE);
				}
			}
			else
				ct = timer + 2 SEC;						/* start delay 2 seconds				*/
		}
		if((start_fg) && (ups_run_time < timer) && (!(p.ups_direction_mode & UPS_NO_TIME))) 								
		{
			emergency_stop ();						/* send emergency stop				*/
			general_error |= G_TRIPTIME;	/* set trip time exceeded error 		*/
			write_errorhist (E_TRIP_TIME, HSE_ID, C_TRIP_TIME, 0);
			hse_state = H_WAIT_STOP;
			return;
		}
	}
}

