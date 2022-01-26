
#include "typedef.h"
#include "parameter.h"
#include "state.h"
#include "call.h"
#include "can.h"
#include "io.h"


/****************************************************************************************************/
/* heartbeat error state																			*/
/****************************************************************************************************/
void hb_error_state (void)
{
	DWORD errortimer;
	DWORD closetimer = 0;
	BYTE wait_time_over = 0;
	BYTE opendoorflag = 0;
	BYTE closedoorflag = 0;

	errortimer = timer + (p.errortime SEC);			/* start error delay timer						*/
	cycle ();
	ct = timer + 2 SEC;								/* wait 2 seconds								*/
	send_hse_state ();
	clear_groupcall ();								/* clear all variables for group control		*/
	she_calldoor_double = 0;
	while (1)
	{
		if (ct < timer)
		{
			search_state ();
			set_state ();
			ct = timer + 20;								/* wait again 2 seconds							*/
			if (hse_state != H_HB_ERROR)								/* state changed									*/
			{
				return;
			}
		}
		cycle ();
		if (!wait_time_over)							/* error wait time not over						*/
		{
			if (errortimer < timer)						/* time over									*/
			{
				calls_off = 1;								/* disable calls								*/
				clearcalls (ALL_CALLS);								/* clear all calls										*/
				wait_time_over = 1;
				fault_signal = 1;							/* set fault signal							*/
				out_of_order = 1;							/* set out of order indication					*/
				if ((door_zone & POS_SGM) && (level != 0xFF) && (!(em_op & INSP_ON)) && (!(insp & (INSP_TOPCAR_ACTIVE | INSP_PITBOX_ACTIVE))))
				{
					opendoorflag = 1;
					closetimer = timer + 20 SEC;
					dooropenpush = DOOR1 | DOOR2;
					handle_dooropenpush();
					dooropenpush = 0;
				}
			}
		}
		if ((door_zone & POS_SGM) && (level != 0xff))
		{
			if (handle_dooropenpush())
			{
				closedoorflag = 0;
				closetimer = timer + 20 SEC;
			}
		}
		if ((opendoorflag) && (closetimer < timer))
		{
			if (!closedoorflag)
			{
				closedoorflag = 1;
				door_command (CLOSE_DOOR, CLOSE_DOOR);
			}
		}
		fan_on ();
	}
}


