
#include "typedef.h"
#include "parameter.h"
#include "state.h"
#include "call.h"
#include "can.h"
#include "eeprom.h"
#include "history.h"
#include "io.h"
#include "Drive.h"

/****************************************************************************************************/
/* Safety circuit error state																		*/
/****************************************************************************************************/
void sc_error_state (void)
{
	DWORD errortimer;
	DWORD closetimer = 0;
	BYTE errordelay = 3;
	BYTE wait_time_over = 0;
	BYTE opendoorflag = 0;
	BYTE closedoorflag = 0;

	errortimer = timer + (p.errortime SEC);			/* start error delay timer						*/
	cycle ();
	ct = timer + 2;									/* wait 0,2 seconds							*/
	emergency_stop ();
	send_hse_state ();
	clear_groupcall ();								/* clear all variables for group control		*/
	she_calldoor_double = 0;
	while (1)
	{
		if (ct < timer)
		{
			set_state ();
			ct = timer + 1 SEC;							/* wait again 1 seconds							*/
			if (hse_state != H_SC_ERROR)								/* state changed									*/
			{
				return;
			}
			if (!errordelay)								/* set error delayed							*/
			{
				if ((safety_circuit_1) && (!(safety_circuit & SC1)) && sc_limit_fg)
				{
					if (door_zone & (POS_SDU1 | POS_SDD1))
					{
						if (!limit_switch_error)
							write_errorhist (E_SC_VOLTAGE_LIMIT, HSE_ID, 0, 0);
						limit_switch_error = 1;					/* set error marker								*/
					}
					else
						write_errorhist (E_SC_VOLTAGE, HSE_ID, 0, 0);
					safety_circuit_1 = 0;						/* mark safety circuit as opened				*/
					sc_limit_fg = 0;
				}
			}
			else
				errordelay--;
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
				if ((door_zone & POS_SGM) && (level != 0xFF) && (!(em_op & 0x01)) && (!(insp & (INSP_TOPCAR_ACTIVE | INSP_PITBOX_ACTIVE))))
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
		fan_on();
	}
}


