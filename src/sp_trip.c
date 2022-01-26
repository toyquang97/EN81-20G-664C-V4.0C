
#include "typedef.h"
#include "parameter.h"
#include "eeprom.h"
#include "state.h"
#include "call.h"
#include "rtc.h"

/****************************************************************************************************/
/* Search for any call																		*/
/****************************************************************************************************/
BYTE new_call (BYTE type)
{
	BYTE i;

	for (i = p.bot_floor; i <= p.top_floor; i++)
	{
		if (calltab [i].calltype & type)					/* call for this floor						*/
			return (1);
	}
	return (0);
}

/****************************************************************************************************/
/* Look for special trips and set calls														*/
/****************************************************************************************************/
void special_trip (void)
{
	BYTE i;
	BYTE parkfloor;

	if (callpriority != C_STANDARD)					/* only in standard call priority				*/
		return;
	if (intermediate_door)							/* intermediate door is open				*/
	{
		parkingtimer = timer + (p.parkingtime SEC);		/* restart parking trip timer				*/
		if (new_call (ALL_CALLS))					/* not all calls cleared						*/
			clearcalls (ALL_CALLS);
		return;
	}
	if (car_switched_off || earthquake)				/* switched off from car or earthquake input	*/
	{
		parkingtimer = timer + (p.parkingtime SEC);		/* restart parking trip timer				*/
		if (new_call (ALL_CALLS))					/* not all calls cleared						*/
			clearcalls (ALL_CALLS);
		return;
	}
	if (remote_off)								/* remote switch off						*/
	{
		parkingtimer = timer + (p.parkingtime SEC);		/* restart parking trip timer				*/
		if (remote_clearcalls)						/* calls to clear							*/
		{
			clearcalls (ALL_CALLS);
			remote_clearcalls = 0;
		}
		if (remote_call != 0xFF)						/* lift was not in remote call floor			*/
		{
			if ((level != remote_call) ||				/* lift is not in remote call floor			*/
	  		((doorstate & remote_door & p.doorpos [remote_call]) && (!door_off)))
			{
				if ((!new_call (0xFF)) &&			/* no other call							*/
				(hse_state != H_ARRIVAL))			/* not in arrival state						*/
				{
					i = remote_call;
					calltab [i].calltype |= CARCALL;	/* set new car call						*/
					calltab [i].cc_door = remote_door & p.doorpos [i];
				}
			}
			else
			{
				if (hse_state == H_STANDSTILL)		/* lift is in remote call floor				*/
					remote_call = 0xFF;
			}
		}
		else if ((level != p.remote_off_floor) &&			/* lift is not in remote switch off floor		*/
		(hse_state != H_ARRIVAL))					/* not in arrival state						*/
		{
			if (!new_call (0xFF))					/* no other call							*/
			{
				i = p.remote_off_floor;
				calltab [i].calltype |= CARCALL;		/* set new car call						*/
				calltab [i].cc_door = p.remote_off_door & p.doorpos [i];
			}
		}
		else										/* lift is in remote switch off floor			*/
		{
			if (!remote_switched_off && (hse_state == H_STANDSTILL))
			{
				if (!new_call (ALL_CALLS))			/* no other call exists						*/
					remote_switched_off = 1;		/* lift is remote switched off now			*/
			}
		}
		return;
	}
	if (auto_call)									/* auto-call mode ("Sabbat control")		*/
	{
		if ((hse_state == H_STANDSTILL) &&			/* lift is in standstill state					*/
		(!number_of_calls (1, CARCALL)))		/* no new call							*/
		{
			if (doorstate == ALL_DOORS_CLOSED)	/* all doors closed						*/
			{
				if (p.doortable [DOOR_AUTOCALL][level] & p.doorpos [level])
				{
					calltab [level].calltype |= CARCALL;	/* set new car call					*/
					calltab [level].cc_door = p.doortable [DOOR_AUTOCALL][level] & p.doorpos [level];
					return;
				}
			}
			if (auto_call_dir == DIR_UP)				/* actual direction is up					*/
			{
				if (p.autocall_dir != HALL_CALL_DOWN)
				{
					for (i = level + 1; i <= p.top_floor; i++)
					{
						if (p.doortable [DOOR_AUTOCALL][i] & p.doorpos [i])
						{
							calltab [i].calltype |= CARCALL;	/* set new car call					*/
							calltab [i].cc_door = p.doortable [DOOR_AUTOCALL][i] & p.doorpos [i];
							return;
						}
					}
				}
				auto_call_dir = DIR_DN;
				if (p.autocall_dir == HALL_CALL_UP)
				{
					for (i=p.bot_floor; i<=p.top_floor; ++i)
					{
						if (p.doortable [DOOR_AUTOCALL][i] & p.doorpos [i])
						{
							calltab [i].calltype |= CARCALL;	/* set new car call 				*/
							calltab [i].cc_door = p.doortable [DOOR_AUTOCALL][i] & p.doorpos [i];
							return;
						}
					}
				}
			}
			else									/* actual direction is down					*/
			{
				if (p.autocall_dir != HALL_CALL_UP)
				{
					for (i = 1; i <= (level - p.bot_floor); i++)
					{
						if (p.doortable [DOOR_AUTOCALL][level - i] & p.doorpos [level - i])
						{							/* set new car call						*/
							calltab [level - i].calltype |= CARCALL;
							calltab [level - i].cc_door = p.doortable [DOOR_AUTOCALL][level - i] & p.doorpos [level - i];
							return;
						}
					}
				}
				auto_call_dir = DIR_UP;
				if (p.autocall_dir == HALL_CALL_DOWN)
				{
					for (i=p.top_floor; i>=p.bot_floor; --i)
					{
						if (p.doortable [DOOR_AUTOCALL][i] & p.doorpos [i])
						{
							calltab [i].calltype |= CARCALL;	/* set new car call 				*/
							calltab [i].cc_door = p.doortable [DOOR_AUTOCALL][i] & p.doorpos [i];
							return;
						}
						if (i==p.bot_floor)
							break;
					}
				}
			}
		}
		return;
	}

	if (testtrip)									/* test trips on (call simulation)			*/
	{
		if (start_test_trip &&						/* time to start test trip					*/
		(calls_enabled & CARCALL)	&&				/* and calls not disabled					*/
		(handle_calls  & CARCALL)	&&				/* and calls should be handled					*/
		(!calltype) && (groupcalldelay < mstimer))		/* no new call							*/
		{
			if ((((test_endtime > test_starttime) && (t.hour >= test_starttime && t.hour < test_endtime))
				|| ((test_endtime < test_starttime) && (t.hour >= test_starttime || t.hour < test_endtime))
				|| (test_starttime == test_endtime))
				&& (testtimer <= timer)
				&& ((testtimes) || (!test_times)))
			{
				i = random(MAX_FLOOR);
				if (i != level)							/* not actual floor						*/
				{
					if (p.doortable [adt][i] & p.doorpos [i])
					{
						calltab [i].calltype |= CARCALL;	/* set new car call						*/
						calltab [i].cc_door = p.doortable [adt][i] & p.doorpos [i];

						if (testtimes)
						{
							-- testtimes;
							if ((!testtimes) && test_times)
							{
								testtrip = 0;
							}
						}
					}
				}
			}
		}
		return;
	}
	if ((p.parkingtime) &&							/* park trip enabled						*/
      ((parkingtimer < timer)) ||					/* park time over								*/
	   (triperrorcount >= MAX_TRIP_ERROR))			/* if error count exceed limit start park trip	*/
	{
		if (timer2_trip)								/* park floor depending on call state		*/
			parkfloor = p.timer2_parkfloor;
		else if (timer1_trip || timer_trip)
			parkfloor = p.timer1_parkfloor;
		else
			parkfloor = p.parkfloor;
		if ((level != parkfloor) &&					/* lift is not in park floor					*/
		((hse_state == H_STANDSTILL) ||			/* lift in standstill state or					*/
		(hse_state == H_ERROR_RELEV)))			/* lift in error state with relevelling active	*/
		{
			if ((parkfloor >= p.bot_floor) && (parkfloor <= p.top_floor))
			{
				calltab [parkfloor].calltype |= CARCALL;		/* write call to call table				*/
				i = p.doortable [adt][parkfloor] & p.doorpos [parkfloor] & p.parkdoor [parkfloor];
				if ((p.parkdoormode [0] & DOOR1) != P_DOOR_OPEN)	/* parking with closed door 1			*/
					i &= 0xFE;							/* clear door 1						*/
				if ((p.parkdoormode [1] & DOOR1) != P_DOOR_OPEN)	/* parking with closed door 2			*/
					i &= 0xFD;							/* clear door 2						*/
				calltab [parkfloor].cc_door |= i;
				parkcall = parkfloor + 1;
			}
			parkingtimer = timer + (p.parkingtime SEC);		/* restart parking trip timer			*/
		}
	}
}


