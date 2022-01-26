
#include "typedef.h"
#include "parameter.h"
#include "state.h"
#include "call.h"
#include "can.h"
#include "drive.h"


/****************************************************************************************************/
/* terminal function																				*/
/****************************************************************************************************/
void doortest_state (void)
{
	BYTE doortest_state = 0;

	send_hse_state ();
	calls_off = 1;											/* disable calls								*/
	clearcalls (ALL_CALLS);									/* clear all calls										*/
	fault_signal = 0;										/* reset fault signal							*/
	out_of_order = 1;										/* set out of order indication					*/
	she_calldoor_double = 0;
	while (1)
	{
		cycle ();
		set_state ();										/* set new state								*/
		if (hse_state != H_DOORTEST)						/* state changed										*/
		{
			if (drive_state != D_STOPPED)					/* drive not in stopped state					*/
			{
				emergency_stop ();						/* stop drive									*/
				hse_state = H_WAIT_STOP;					/* wait for stop								*/
			}
			return;										/* go to new state								*/
		}
		if (doortest_command != doortest_state)				/* new door command								*/
		{
			doortest_state = doortest_command;
			switch (doortest_state)
			{
				case (DR1_OPEN):							/* open door 1									*/
					door_command (OPEN_DOOR, DOOR_OFF);
					break;

				case (DR1_CLOSE):						/* close door 1									*/
					door_command (CLOSE_DOOR, DOOR_OFF);
					break;

				case (DR1_CLOSE_L):						/* close door 1 with force limit				*/
					door_command (CLOSE_DOOR_L, DOOR_OFF);
					break;

				case (DR2_OPEN):							/* open door 2									*/
					door_command (DOOR_OFF, OPEN_DOOR);
					break;

				case (DR2_CLOSE):						/* close door 2									*/
					door_command (DOOR_OFF, CLOSE_DOOR);
					break;

				case (DR2_CLOSE_L):						/* close door 2 with force limit				*/
					door_command (DOOR_OFF, CLOSE_DOOR_L);
					break;

				case (DR_STOP):							/* all doors stop								*/
					door_command (STOP_DOOR, STOP_DOOR);
					break;

				case (DR_OPEN):							/* open all doors								*/
					door_command (OPEN_DOOR, OPEN_DOOR);
					break;

				case (DR_CLOSE):							/* close all doors								*/
					door_command (CLOSE_DOOR, CLOSE_DOOR);
					break;
			}
		}
	}
}


