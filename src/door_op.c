
#include "typedef.h"
#include "parameter.h"
#include "state.h"
#include "call.h"
#include "can.h"
#include "drive.h"
#include "eeprom.h"
#include "history.h"
#include "io.h"
#include "Drive.h"

//**************************************************************************************************
// Door is open and lift is not in normal state
//**************************************************************************************************
void door_open_state (void)
{
	DWORD errortimer;
	BYTE wait_time_over = 0;

	errortimer = timer + (p.errortime SEC);						// start error delay timer
	send_hse_state ();
	clear_groupcall ();											// clear all variables for group control
	she_calldoor_double = 0;

	if(!p.ucmp_enable)
		{			
			ct = timer + 2 SEC;
		}
	else
		{
			emergency_stop ();
			ct = timer;
		}
	while (1)
	{
		cycle ();
		set_state ();											// set new state
		if (hse_state != H_DOOROPEN)							// state changed
		{
			if (drive_state != D_STOPPED)						// drive not in stopped state
			{
				emergency_stop ();								// stop drive
				hse_state = H_WAIT_STOP;						// wait for stop
			}
			return;												// go to new state
		}
		if (ct < timer)											// time out for next door command
		{
			if((!p.ucmp_enable) || (!ucmp_state))
				door_command (CLOSE_DOOR, CLOSE_DOOR);
			ct = timer + 30 SEC;								// restart timer
		}
		if (!wait_time_over)									// error wait time not over
		{
			if (errortimer < timer)								// time over
			{
				calls_off = 1;									// disable calls
				clearcalls (ALL_CALLS);							// clear all calls
				wait_time_over = 1;
				fault_signal = 1;								// set fault signal
				out_of_order = 1;								// set out of order indication
			}
		}

		fan_on ();
	}
}

