
#include "typedef.h"
#include "parameter.h"
#include "state.h"
#include "call.h"
#include "can.h"


/****************************************************************************************************/
/* error state																						*/
/****************************************************************************************************/
void wait_for_stop_state (void)
{
	send_hse_state ();
	clear_groupcall ();									/* clear all variables for group control	*/
	ct = timer + 10 SEC;								/* wait 2 seconds 					*/
	while (1)
	{
		cycle ();
		if (drive_state == D_STOPPED)					/* Lift is stopped						*/
		{
			set_state ();								/* set new state						*/
			return;
		}
		if (ct < timer)									/* time out							*/
		{
			hse_state = H_SEARCH;
			return;
		}
	}
}


