
#include "typedef.h"
#include "parameter.h"
#include "state.h"
#include "call.h"
#include "can.h"
#include "io.h"
#include "eeprom.h"
#include "history.h"

/****************************************************************************************************/
/* error state																						*/
/****************************************************************************************************/
void error_state (void)
{
	DWORD errortimer;
	DWORD closetimer = 0;
	DWORD doortimer = 0;
	BYTE wait_time_over = 0;
	BYTE save = 0;
	BYTE i;
	BYTE maxretry;
	BYTE retrycnt = 0;
	BYTE doorclosecmd = 0;
	BYTE dooropencmd = 0;
	BYTE d1cmd = DOOR_OFF;
	BYTE d2cmd = DOOR_OFF;
	BYTE opendoorflag = 0;
	BYTE closedoorflag = 0;
	DWORD dl_shorted_timer = 0;

	errortimer = timer + (p.errortime SEC);			/* start error delay timer						*/
	cycle ();
	ct = timer + 2 SEC;								/* wait 2 seconds								*/
	send_hse_state ();
	clear_groupcall ();								/* clear all variables for group control		*/
	she_calldoor_double = 0;

	if((dl_shorted) && (general_error & G_DL_SHORTED))
		dl_shorted_timer = timer + 3600 SEC;

	while (1)
	{
		if (ct < timer)
		{
			search_state ();
			set_state ();
			ct = timer + 2 SEC;							/* wait again 2 seconds							*/
			if (hse_state != H_ERROR)					/* state changed									*/
				return;
			if (triperrorcount < MAX_TRIP_ERROR)			/* error count did not reach limit				*/
			{
				if (poserror & P_VU_NOT_ON)
					{
						if ((level != p.bot_floor) || (door_zone & POS_SDD1))
							{
								poserror &= ~P_VU_NOT_ON;
								vu_not_on_recovery = 1;
							}
					}
				else if (poserror & P_VO_NOT_ON)
					{
						if ((level != p.top_floor) || (door_zone & POS_SDU1))
							{
								poserror &= ~P_VO_NOT_ON;
								vo_not_on_recovery = 1;
							}
					}
				else if (poserror & P_VO_VU_ERROR)
					{
						if ((force_switch & (FORCE_SDU1|FORCE_SDD1)) != (FORCE_SDU1|FORCE_SDD1))
						{
							poserror &= ~P_VO_VU_ERROR;
							vovu_not_off_recovery = 1;
						}
					}
					if (poserror & P_ILO_ON)
						poserror &= ~P_ILO_ON;
					if (poserror & P_ILU_ON)
						poserror &= ~P_ILU_ON;
					if (general_error & G_TRIPTIME)
						general_error &= ~G_TRIPTIME;
					if (general_error & G_MSI_NOTOFF)
						general_error &= ~G_MSI_NOTOFF;
					if(!(ls_state & LS_STATE_DOOR1))
						general_error &= ~G_LIGHTSCREEN1;
					if(!(ls_state & LS_STATE_DOOR2))
						general_error &= ~G_LIGHTSCREEN2;
				
					if ((general_error & (G_OPENDOOR1 | G_OPENDOOR2)) && (!(general_error & (~(G_OPENDOOR1 | G_OPENDOOR2)))) && (level != 0xff))
					{
						if (!dooropenerrorcount[0])
							general_error &= ~G_OPENDOOR1;
						if (!dooropenerrorcount[1])
							general_error &= ~G_OPENDOOR2;
						if ((dooropenerrorcount[0] | dooropenerrorcount[1]) && (general_error))
						{
							if ((general_error & (G_OPENDOOR1 | G_OPENDOOR2)) == (G_OPENDOOR1 | G_OPENDOOR2))
								maxretry = GetMin(p.dooropenlimit [0], p.dooropenlimit [1]);
							else if (general_error & G_OPENDOOR1)
								maxretry = p.dooropenlimit [0];
							else
								maxretry = p.dooropenlimit [1];
							if (retrycnt < maxretry)
							{
								if (!doorclosecmd)
								{
									doorclosecmd = 1;
									d1cmd = DOOR_OFF;
									d2cmd = DOOR_OFF;
									if (general_error & G_OPENDOOR1)
										d1cmd = CLOSE_DOOR;
									if (general_error & G_OPENDOOR2)
										d2cmd = CLOSE_DOOR;
									door_command (d1cmd, d2cmd);
									if ((general_error & (G_OPENDOOR1 | G_OPENDOOR2)) == (G_OPENDOOR1 | G_OPENDOOR2))
										doortimer = timer + 10ul * GetMax(p.doorclosetime[0], p.doorclosetime[1]);
									else if (general_error & G_OPENDOOR1)
										doortimer = p.doorclosetime[0];
									else
										doortimer = p.doorclosetime[1];
								}
								else
								{
									if ((!dooropencmd) && (door_zone & POS_SGM))
									{
										if (((general_error == G_OPENDOOR1) && (door_state[0] == DOOR_CLOSED))
											|| ((general_error == G_OPENDOOR2) && (door_state[1] == DOOR_CLOSED))
											|| ((general_error == (G_OPENDOOR1|G_OPENDOOR2)) && (door_state[0] == DOOR_CLOSED) && (door_state[1] == DOOR_CLOSED))
											|| (doortimer < timer))
										{
											dooropencmd = 1;
											d1cmd = DOOR_OFF;
											d2cmd = DOOR_OFF;
											if (general_error & G_OPENDOOR1)
												d1cmd = OPEN_DOOR;
											if (general_error & G_OPENDOOR2)
												d2cmd = OPEN_DOOR;
											door_command (d1cmd, d2cmd);
											if ((general_error & (G_OPENDOOR1 | G_OPENDOOR2)) == (G_OPENDOOR1 | G_OPENDOOR2))
												doortimer = timer + 10ul * GetMax(p.dooropentime[0], p.dooropentime[1]);
											else if (general_error & G_OPENDOOR1)
												doortimer = p.dooropentime[0];
											else
												doortimer = p.dooropentime[1];
										}
									}
									else if (doortimer < timer)
									{
										doorclosecmd = 0;
										dooropencmd = 0;
										++ retrycnt;
										if (retrycnt == maxretry)
										{
											if (d1cmd == OPEN_DOOR)
												d1cmd = CLOSE_DOOR;
											if (d2cmd == OPEN_DOOR)
												d2cmd = CLOSE_DOOR;
											door_command (d1cmd, d2cmd);
											if (level != 0xff)
												floortried[level] = 0x01;
										}
									}
								}
							}
							else if ((nextfloortrycount < MAX_DROPERR_TRYCNT) && (!poserror))
							{
								if (level != 0xff)
								{
									for (i=level+1; i<=p.top_floor; ++i)
									{
										if ((p.doorpos[i]) && (!floortried[i]))
										{
											door_command (CLOSE_DOOR, CLOSE_DOOR);
											calltab [i].calltype |= CARCALL;
											calltab [i].cc_door |= p.doorpos [i];
											++ nextfloortrycount;
											dooropenerrorcount [0] = 0;
											dooropenerrorcount [1] = 0;
											general_error &= ~(G_OPENDOOR1 | G_OPENDOOR2);
											set_state ();
											return;
										}
									}
									for (i=level-1; i>=p.bot_floor; --i)
									{
										if ((p.doorpos[i]) && (!floortried[i]))
										{
											door_command (CLOSE_DOOR, CLOSE_DOOR);
											calltab [i].calltype |= CARCALL;
											calltab [i].cc_door |= p.doorpos [i];
											++ nextfloortrycount;
											dooropenerrorcount [0] = 0;
											dooropenerrorcount [1] = 0;
											general_error &= ~(G_OPENDOOR1 | G_OPENDOOR2);
											set_state ();
											return;
										}
										if (i == p.bot_floor)
											break;
									}
								}
							}
						}
					}
					if ((general_error & (G_CLOSEDOOR1 | G_CLOSEDOOR2)) && (!(general_error & (~(G_CLOSEDOOR1 | G_CLOSEDOOR2)))))
					{
						if (!doorcloseerrorcount[0])
							general_error &= ~G_CLOSEDOOR1;
						if (!doorcloseerrorcount[1])
							general_error &= ~G_CLOSEDOOR2;
						if ((doorcloseerrorcount[0] | doorcloseerrorcount[1]) && (general_error))
						{
							if ((general_error & (G_CLOSEDOOR1 | G_CLOSEDOOR2)) == (G_CLOSEDOOR1 | G_CLOSEDOOR2))
								maxretry = GetMin(p.doorcloselimit [0], p.doorcloselimit [1]);
							else if (general_error & G_CLOSEDOOR1)
								maxretry = p.doorcloselimit [0];
							else
								maxretry = p.doorcloselimit [1];
							if (retrycnt < maxretry)
							{
								if ((!dooropencmd) && (door_zone & POS_SGM))
								{
									dooropencmd = 1;
									d1cmd = DOOR_OFF;
									d2cmd = DOOR_OFF;
									if (general_error & G_CLOSEDOOR1)
										d1cmd = OPEN_DOOR;
									if (general_error & G_CLOSEDOOR2)
										d2cmd = OPEN_DOOR;
									door_command (d1cmd, d2cmd);
									if ((general_error & (G_CLOSEDOOR1 | G_CLOSEDOOR2)) == (G_CLOSEDOOR1 | G_CLOSEDOOR2))
										doortimer = timer + 10ul * GetMax(p.dooropentime[0], p.dooropentime[1]);
									else if (general_error & G_CLOSEDOOR1)
										doortimer = p.dooropentime[0];
									else
										doortimer = p.dooropentime[1];
								}
								else
								{
									if (!doorclosecmd)
									{
										if (((general_error == G_CLOSEDOOR1) && (door_state[0] == DOOR_OPENED))
											|| ((general_error == G_CLOSEDOOR2) && (door_state[1] == DOOR_OPENED))
											|| ((general_error == (G_CLOSEDOOR1|G_CLOSEDOOR2)) && (door_state[0] == DOOR_OPENED) && (door_state[1] == DOOR_OPENED))
											|| (doortimer < timer))
										{
											doorclosecmd = 1;
											d1cmd = DOOR_OFF;
											d2cmd = DOOR_OFF;
											if (general_error & G_CLOSEDOOR1)
												d1cmd = CLOSE_DOOR;
											if (general_error & G_CLOSEDOOR2)
												d2cmd = CLOSE_DOOR;
											door_command (d1cmd, d2cmd);
											if ((general_error & (G_CLOSEDOOR1 | G_CLOSEDOOR2)) == (G_CLOSEDOOR1 | G_CLOSEDOOR2))
												doortimer = timer + 10ul * GetMax(p.doorclosetime[0], p.doorclosetime[1]);
											else if (general_error & G_CLOSEDOOR1)
												doortimer = p.doorclosetime[0];
											else
												doortimer = p.doorclosetime[1];
										}
									}
									else if (doortimer < timer)
									{
										doorclosecmd = 0;
										dooropencmd = 0;
										++ retrycnt;
										if ((retrycnt >= maxretry) && (door_zone & POS_SGM))
										{
											if (d1cmd == CLOSE_DOOR)
												d1cmd = OPEN_DOOR;
											if (d2cmd == CLOSE_DOOR)
												d2cmd = OPEN_DOOR;
											door_command (d1cmd, d2cmd);
										}
									}
								}
							}
							else
							{
								if ((!dooropencmd) && (door_zone & POS_SGM))
								{
									dooropencmd = 1;
									d1cmd = DOOR_OFF;
									d2cmd = DOOR_OFF;
									if (general_error & G_CLOSEDOOR1)
										d1cmd = OPEN_DOOR;
									if (general_error & G_CLOSEDOOR2)
										d2cmd = OPEN_DOOR;
									door_command (d1cmd, d2cmd);
								}
							}
						}
					}

					if (general_error & G_DL_SHORTED)
					{
						if ((!dooropencmd) && (level != 0xff) && (door_zone & POS_SGM))
						{
							dooropencmd = 1;
							d1cmd = DOOR_OFF;
							d2cmd = DOOR_OFF;
							if (p.doorpos [level] & DOOR1)
								d1cmd = OPEN_DOOR;
							if ((p.doorpos [level] & DOOR2) && (p.doornumber > 1))
								d2cmd = OPEN_DOOR;
							door_command (d1cmd, d2cmd);
						}
					if(dl_shorted_timer < timer)
						{
							if (!(safety_circuit & SC2))
								dl_shorted &= ~SC2;
							if (!(safety_circuit & SC3))
								dl_shorted &= ~SC3;
							if ((!dl_shorted) || (!check_dl_short))
								general_error &= ~G_DL_SHORTED;
						}
					}
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
				save = 1;								/* save important variables in EEPROM			*/
				fault_signal = 1;							/* set fault signal							*/
				out_of_order = 1;							/* set out of order indication					*/
				if ((door_zone & POS_SGM) && (level != 0xFF) && (!(em_op & INSP_ON)) && (!(insp & (INSP_TOPCAR_ACTIVE | INSP_PITBOX_ACTIVE))) &&
					(!(general_error & (G_OPENDOOR1 | G_OPENDOOR2 | G_CLOSEDOOR1 | G_CLOSEDOOR2 | G_DL_SHORTED | G_TRIPTIME) )))
				{
					opendoorflag = 1;
					closetimer = timer + 20 SEC;
					dooropenpush = DOOR1 | DOOR2;
					handle_dooropenpush();
					dooropenpush = 0;
				}
			}
		}
		if (save)										/* history still to save						*/
		{
			WriteParaData (&tripcount, EE_TRIPCOUNT, sizeof (tripcount));
			WriteParaData (&op_time, EE_OPERTIME, sizeof (op_time));
			WriteParaData (floorcount, EE_FLOORCOUNT, sizeof (floorcount));
			save = 0;
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


