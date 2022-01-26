
#include "typedef.h"
#include "parameter.h"
#include "state.h"
#include "call.h"
#include "can.h"
#include "pos.h"
#include "eeprom.h"
#include "history.h"
#include "io.h"

/****************************************************************************************************/
/* Definitions for door open state															*/
/****************************************************************************************************/
#define	D_CLOSED		0						/* door is closed							*/
#define	D_STAYTIME		1						/* minimum open stay time				*/
#define	D_CLOSING		2						/* door is closing							*/

/****************************************************************************************************/
/* Standstill in error mode																	*/
/****************************************************************************************************/
void error_relev (void)
{
	DWORD errortimer;
	BYTE doorstaystate = D_STAYTIME;
	BYTE overtempstate = 0;
	BYTE carlightstate = 1;
	BYTE rel_active = 2;
	BYTE wait_time_over = 0;

	errortimer = timer + (p.errortime SEC);				/* start error delay timer					*/
	target_floor = level;
	send_hse_state ();								/* send new state on CAN bus				*/
	ct = timer + (p.doorstaytime_lc SEC);				/* Timer for door open stay time with land.call	*/
	clearcalls (ALL_CALLS);							/* clear all calls							*/
	she_calldoor_double = 0;
	while (1)
	{
		cycle ();									/* do all cyclic functions					*/
		set_state ();								/* set new state							*/
		if (hse_state != H_ERROR_RELEV)			/* state changed							*/
		{
			return;							/* go to new state						*/
		}
		if (!wait_time_over)							/* error wait time not over					*/
		{
			if (errortimer < timer)					/* time over								*/
			{
				wait_time_over = 1;
				fault_signal = 1;					/* set fault signal							*/
				out_of_order = 1;					/* set out of order indication				*/
			}
		}

		if ((doorstate == ALL_DOORS_CLOSED) &&		/* all doors closed						*/
		(doorstaystate != D_CLOSED))				/* and door was opened					*/
			doorstaystate = D_CLOSED;				/* door stay mode: door is closed			*/

		switch (doorstaystate)
		{
			case (D_STAYTIME):					/* door is open, wait open stay time		*/
				if (ct < timer)						/* door open stay time is over				*/
				{								/* to be done if necessary close doors		*/
					if (new_call (CARCALL))			/* new car call							*/
					{
						set_door (~doorstate, CLOSE_DOOR);	/* close all not already closed doors*/
						doorstaystate = D_CLOSING;	/* door open mode: wait until door is closed	*/
						ct = timer + 60 SEC;		/* max. waiting time until door is closed	*/
					}
					else if (!set_park_state ())		/* parking with closed doors				*/
					{
						doorstaystate = D_CLOSING;	/* door open mode: wait until door is closed	*/
						ct = timer + 60 SEC;		/* max. waiting time until door is closed	*/
					}
				}
				else if ((door_state [0] == DOOR_OPENING) ||
				(door_state [1] == DOOR_OPENING))				/* door is opening				*/
					ct = timer + (p.doorstaytime_lc SEC);		/* restart door open stay timer		*/
				break;

			case (D_CLOSING):					/* door is closing							*/
				if (ct < timer)						/* max. waiting time until door is closed over	*/
				{								/* send new door command for all doors	*/
					door_command (CLOSE_DOOR, CLOSE_DOOR);
					ct = timer + 60 SEC;			/* restart waiting time						*/
				}
				break;

			case (D_CLOSED):						/* all doors are closed					*/
				if (doorstate != ALL_DOORS_CLOSED)		/* not all doors closed			*/
				{
					doorstaystate = D_STAYTIME;		/* door open mode: minimum open stay time	*/
					ct	= timer +(p.doorstaytime_lc SEC);		/* Timer for door open stay time with land.call	*/
				}
				else if ((!rel_active)					/* lift is not re-levelling					*/
				&& (new_call (CARCALL))			/* and new call							*/
				&& (start_enabled ()))				/* and start not disabled (evacuation, ...)	*/
				{
					hse_state = H_START;			/* start a new trip						*/
					return;					/* go to new state						*/
				}
				break;
		}
		if (((!carlight) && carlightstate)				/* car light is on and was off before			*/
			|| ((temperature & (OVERTEMP1 | OVERTEMP2 | ROOMTEMP_MIN | ROOMTEMP_MAX)) && (!overtempstate)))
		{										/* now overtemperature and not before		*/
			doornew = p.doorpos [level];				/* open all existing doors					*/
			set_door (doornew, OPEN_DOOR);		/* send door command					*/
			doorstate &= ~doornew;				/* mark doors as open					*/
			doorstaystate = D_STAYTIME;
			ct = timer + (p.doorstaytime_lc SEC);		/* Timer for door open stay time with car call	*/
		}
		if (temperature & (OVERTEMP1 | OVERTEMP2 | ROOMTEMP_MIN | ROOMTEMP_MAX))
			overtempstate = 1;
		else
			overtempstate = 0;
		carlightstate = carlight;						/* save actual car light state				*/
		if (handle_dooropenpush ())					/* handle door open push					*/
		{
			doorstaystate = D_STAYTIME;
			ct	= timer + (p.doorstaytime_lc SEC);		/* Timer for door open stay time with car call	*/
		}
		rel_active = relevelling (rel_active, NO_CALL);	/* re-levelling function					*/
		fan_on ();
	}
}


