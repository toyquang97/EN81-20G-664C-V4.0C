
#include "typedef.h"
#include "parameter.h"
#include "nice5000.h"
#include "state.h"
#include "call.h"
#include "can.h"
#include "pos.h"
#include "io.h"
#include "drive.h"
#include "history.h"


//**************************************************************************************************
// Arrival in normal mode
//**************************************************************************************************
void arrival_state (void)
{
	BYTE doorold;										// actual door state
	BYTE doorcommand [MAX_DOOR];
	BYTE i;
	BYTE msi_state = 0;
	WORD door_zone_state;

	calls_off = 0;										// enable calls
	doordelay = 0;										// set door delay for group control to 0
	door_zone_state = door_zone;						/* save old state of door zone switches	*/
	for (i = 0; i < MAX_DOOR; i++)
	{
		doorcommand [i] = DOOR_OFF;						// no door command
		dooropenerrorfloor [i] = 0xFF;					// reset level of last open error
	}
	send_hse_state ();
	ct = timer + (p.triptime SEC);						// wait max. trip time for stop
	doornew = 0;
	doorold = 0;
	msi_state = 0;
	landingcalldir = 0;									// clear old landing call direction
	if (calltype == LANDINGCALL_UP)						// landing call up to process
		calldir = DIR_UP;								// set new call direction
	else if (calltype == LANDINGCALL_DN)				// landing call down to process
		calldir = DIR_DN;								// set new call direction
	fault_signal = 0;									// reset fault signal
	out_of_order = 0;									// reset out of order indication

	while (1)
	{
		check_ilu_ild ();
		check_speed();
		get_call (landingcalldir, target_floor, (~doornew) & 0x0F, 1);
		if (callfloor == target_floor)					// call for target floor
		{
			if ((callpriority == C_FIREALARM) && (target_floor != firealarmfloor))
			{											// disable door opening in fire alarm mode
				doornew = 0;							// if lift is not in fire alarm floor
				doorold = 0;
			}
			doornew |= calldoors;						// open all called doors
			doornew &= p.doorpos [target_floor];		// only open existing doors
			if (!landingcalldir)
			{
				if (calltype == LANDINGCALL_UP)			// landing call up to process
					landingcalldir = LANDINGCALL_UP;	// set new landing call direction
				else if (calltype == LANDINGCALL_DN)	// landing call down to process
					landingcalldir = LANDINGCALL_DN;	// set new landing call direction
				else if (calltype == LANDINGCALL)		// landing call down to process
					landingcalldir = LANDINGCALL;		// set new landing call direction
			}
		}
		if((door_zone & POS_SGM) && !msi_state)
			{
				msi_state = 1;
				msi_trans_check++;
				if(msi_trans_check >= 3)
					{
						if(msi_trans_check > msi_trans_num)
							{
								if(!(general_error & G_MSI_NOTOFF))
									{
										general_error |= G_MSI_NOTOFF;				// set trip time exceeded error 
										write_errorhist (E_MSI_NOTOFF, HSE_ID, C_MSI_NOTOFF, 0);
									}
							}
						msi_trans_check = 0;
						msi_trans_num = 0;
					}
			}
		set_state ();									// set new state
		if ((hse_state == H_STANDSTILL) ||				// lift is in standstill state
			((hse_state == H_ARRIVAL) &&					// or lift is still in arrival state
			(p.adv_door_op) &&								// and advanced door opening enabled
			(door_zone & POS_SGM) &&						// and Lift is in door zone
			(dz_input) && (!adv_door_op_dly) &&				// and door zone relay input is set
			(level == target_floor) &&						// and Lift is in target floor
			(nice5000_Rstore_feedspd < p.arrival_speed) &&
			(triperrorcount < MAX_TRIP_ERROR)))
		{			
//应急电源撤离时先开门，然后再运行至指定楼层
			if(emp_power && p.emp_power_door)
				{
					calldoors = p.doorpos [level] & p.emp_power_door;
					doornew |= calldoors; 					// open all called doors
					doornew &= p.doorpos [level];		// only open existing doors
				}
//应急电源撤离时先开门，然后再运行至指定楼层
			
//增加SHE 贯通门功能 2016-10-26
			if(p.cooperate_sel1 & COOP1_THROUGH_DOOR)
				{
					if(doornew == (DOOR1 | DOOR2))
						{
							she_calldoor_double = doornew;
							doornew &= p.she_doornumber;
							she_calldir = calldir;
						}
					else
						{
							she_calldoor_double = 0;
							she_calldir = 0;
						}
				}
//增加SHE 贯通门功能 2016-10-26 	

			if (doornew != doorold)						// still doors to open
			{
				if ((!door_off) &&
				((!firedoormode) || (p.fire_dooropenmode == FIREDOOR_AUTO)))
				{
					for (i = 0; i < MAX_DOOR; i++)		// search all existing doors
					{
						if (bit_select (doornew, i) && !bit_select (doorold, i))
						{
							doorcommand [i] = OPEN_DOOR;// door to open
							bit_reset (doorstate, i);	// mark door as not closed
							gongoutputenable = 1;
						}
						else							// all requested doors already open
							doorcommand [i] = DOOR_OFF;	// door don't care
					}
					door_command (doorcommand [0], doorcommand [1]);
//在开门时刻清除按钮指示灯			2016-12-08		
					if((p.custumer1 & CUSTUMER1_SHE_MODE) && (p.doorpos[level] == (DOOR1 | DOOR2)) &&
							(she_calldoor_double != (DOOR1 | DOOR2)))
						{							
							set_out (CAR_CALL, level + 1, 0, doornew, 0, O_CANA);		//消除指示灯
							if(calldir == DIR_UP)
								i = HALL_CALL_UP; 					
							else if(calldir == DIR_DN)
								i = HALL_CALL_DOWN;
							else
								i = HALL_CALL_UP;
							set_out (HALL_CALL, i, level + 1, doornew, 0, O_CANB);
						}						
//在开门时刻清除按钮指示灯			2016-12-08		
				}
				doorold = doornew;						// set door state
			}
		}
		cycle ();
		special_trip ();								// handle special trips		

		if (hse_state != H_ARRIVAL)						// state changed
		{
			if (p.clear_cc_last_floor &&				// clear all car calls in last landing
			((level == p.top_floor) || (level == p.bot_floor)))
				clearcalls (CARCALL);
			return;
		}
		if(p.simulation == SIMULATION_AUTO)
			{
				if ((door_zone & POS_SGM) != (door_zone_state & POS_SGM))					/* state of door zone switches changed	*/
					{
						ct = timer + (p.triptime SEC);					/* retrigger trip time					*/
						door_zone_state = door_zone;				/* save old state of door zone switches	*/
					}
			}
		if (ct < timer)									// time out, lift did not stop
		{
			emergency_stop ();							// send emergency stop
			general_error |= G_TRIPTIME;				// set trip time exceeded error
			write_errorhist (E_TRIP_TIME, HSE_ID, C_TRIP_TIME, 0);
			hse_state = H_WAIT_STOP;
			return;
		}

		fan_on ();										// cabin fan on
	}
}


