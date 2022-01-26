
#include "typedef.h"
#include "parameter.h"
#include "state.h"
#include "call.h"
#include "can.h"

/****************************************************************************************************/
/* setup not ready state																			*/
/****************************************************************************************************/
void nosetup_state (void)
{
	send_hse_state ();
	calls_off = 1;									/* disable calls								*/
	clearcalls (ALL_CALLS);							/* clear all calls										*/
	cycle ();
	ct = timer + 2 SEC;								/* wait 2 seconds 								*/
	fault_signal = 0;								/* reset fault signal							*/
	out_of_order = 1;								/* set out of order indication					*/
	while (1)
	{
		if (ct < timer)
		{
			set_state ();
			ct = timer + 2 SEC;						/* wait again 2 seconds							*/
			if (hse_state != H_NO_SETUP)		 	/* state changed							 		*/
				return;
		}
		cycle ();
	}
}


