
#include "typedef.h"
#include "parameter.h"
#include "state.h"
#include "drive.h"
#include "call.h"
#include "can.h"
#include "io.h"
#include "nice5000.h"

/****************************************************************************************************/
/* drive error state																				*/
/****************************************************************************************************/
void drive_error_state (void)
{
	DWORD errortimer;
	DWORD closetimer = 0;
	BYTE wait_time_over = 0;
	BYTE opendoorflag = 0;
	BYTE closedoorflag = 0;

	nice5000_Tcmd_data1 &= ~(NICE5000_TCMD1_RUN | NICE5000_TCMD1_DOWN | NICE5000_TCMD1_UP
													| NICE5000_TCMD1_BRAKE | NICE5000_TCMD1_SPECIALDEC);
	errortimer = timer + (p.errortime SEC);				/* start error delay timer						*/
	direction = 0;
	send_hse_state ();
	clear_groupcall ();									/* clear all variables for group control		*/
	ct = timer + 2 SEC;									/* wait 2 seconds								*/
	she_calldoor_double = 0;
	while (1)
	{
		cycle ();
		if (ct < timer)									/* wait time over								*/
		{
			if (( triperrorcount < MAX_TRIP_ERROR)
				|| (insp & (INSP_TOPCAR_ACTIVE | INSP_PITBOX_ACTIVE))						/* or Inspection is on							*/
				|| (em_op & INSP_ON)						/* or Emergency recall operation				*/
				|| (term_active))						/* or Terminal mode 							*/
			{
				if (!(nice5000_Rcmd_data5 & NICE_TCMD5_ERRCODE))
				{
					driveerror &= ~D_ERRORSTATE;
					if ((nice5000_Rfault_errcode == NICE_ERR_45) && (p.setup_ready))
						nice5000_poserr_flag = 1;
				}
				if ((driveerror & D_BRAKE_NOTON) && ((insp & (INSP_TOPCAR_ACTIVE | INSP_PITBOX_ACTIVE)) || (em_op & INSP_ON)))
					driveerror &= ~D_BRAKE_NOTON;
				if ((driveerror & D_BRKCONT_NOTON) && ((insp & (INSP_TOPCAR_ACTIVE | INSP_PITBOX_ACTIVE)) || (em_op & INSP_ON)))
					driveerror &= ~D_BRKCONT_NOTON;

				if (driveerror & D_RUNCONT_NOTON)
					driveerror &= ~D_RUNCONT_NOTON;
				if (driveerror & D_NO_SC)
					driveerror &= ~D_NO_SC;
 				if(driveerror & D_NICE_DIR_NOTON)
					driveerror &= ~D_NICE_DIR_NOTON;

				if (driveerror & D_NICE_RUN_NOTON)
					driveerror &= ~D_NICE_RUN_NOTON;
				if (driveerror & D_NICE_BRAKE_NOTON)
					driveerror &= ~D_NICE_BRAKE_NOTON;
				if (driveerror & D_NICE_MOVE_NOTON)
					driveerror &= ~D_NICE_MOVE_NOTON;

				if (driveerror & D_BRAKE_NOTOFF)
				{
					if ((!brake_check) || (!brake_state))
						driveerror &= ~D_BRAKE_NOTOFF;
				}
				if (driveerror & D_RUNCONT_NOTOFF)
				{
					if (!(contactor_state & contactor_check & D_RUNCONT_SUP))
						driveerror &= ~D_RUNCONT_NOTOFF;
				}
				if (driveerror & D_BRKCONT_NOTOFF)
				{
					if (!(contactor_state & contactor_check & D_BRKCONT_SUP))
						driveerror &= ~D_BRKCONT_NOTOFF;
				}
				if (driveerror & D_NICE_RUN_NOTOFF)
				{
					if (!(nice5000_Rcmd_data1 & NICE5000_RCMD1_RUN))
						driveerror &= ~D_NICE_RUN_NOTOFF;
				}
				if (driveerror & D_NICE_BRAKE_NOTOFF)
				{
					if (!(nice5000_Rcmd_data1 & NICE5000_RCMD1_BRAKE))
						driveerror &= ~D_NICE_BRAKE_NOTOFF;
				}
				if (driveerror & D_NICE_MOVE_NOTOFF)
				{
					if (nice5000_Rcmd_data1 & NICE5000_RCMD1_ZEROSPD)
						driveerror &= ~D_NICE_MOVE_NOTOFF;
				}
				if (driveerror & D_FXCONT_NOTOFF)
				{
					if ((!(contactor_state & contactor_check & D_FXCONT_SUP)) || 	(!p.fx_active))
						driveerror &= ~D_FXCONT_NOTOFF;
				}
				if (((driveerror & D_FXCONT_NOTON) && ((insp & (INSP_TOPCAR_ACTIVE | INSP_PITBOX_ACTIVE)) || (em_op & INSP_ON))) ||		//检修状态认为干预消除
						(!p.fx_active))
					driveerror &= ~D_FXCONT_NOTON;
			}
			ct = timer + 2 SEC; 						/* wait 2 seconds for next reset				*/
			if ((nice5000_Rcmd_data5 & NICE_TCMD5_ERRCODE) && IsNiceFaultChked())
			{
				nice5000_Tcmd_data5 = nice5000_Rcmd_data5 & NICE_TCMD5_ERRCODE;
				nice5000_errrsttime = 5;			
				ct = timer + 6 SEC;
			}
		}

		set_state ();								/* check next state									*/
		if (hse_state != H_DRIVE_ERROR)				/* state changed									*/
		{
			nice5000_Tcmd_data5 = 0;
			return;
		}

		if (!wait_time_over)						/* error wait time not over							*/
		{
			if (errortimer < timer)					/* time over										*/
			{
				calls_off = 1;						/* disable calls									*/
				clearcalls (ALL_CALLS);				/* clear all calls									*/
				wait_time_over = 1;
				fault_signal = 1;					/* set fault signal									*/
				out_of_order = 1;					/* set out of order indication						*/
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


