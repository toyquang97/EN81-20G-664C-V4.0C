
#include "typedef.h"
#include "string.h"
#include "parameter.h"
#include "call.h"
#include "can.h"
#include "eeprom.h"
#include "rtc.h"
#include "history.h"
#include "text.h"
#include "nice5000.h"

struct ErrorCounter errorcounter;
struct ErrorCountDisp count [COUNTSIZE];
struct Hist	errstatus;
WORD brief_err_info[MAX_HIST][BRIEF_INFO_LENGTH];

/****************************************************************************************************/
/* write error history																		*/
/****************************************************************************************************/
void write_errorhist (WORD errortype, WORD node_id, BYTE errorcount, BYTE warningcount)
{
	BYTE i;
	BYTE j;

	if (!power24v)
		return;						// 24v电源断电也不记载故障记录
	if (((errortype & 0xFF00) == E_IV_ERROR) && (!errstatus.idorpwr.power24v))
		return;						// 报驱动故障时如果24v电源断电也不记载故障记录, 主要是强减开关粘连错误
	if (!errortype)
		errortype = 0xFFFF;

	if (save_hist)
		save_all = 1;
	else
		save_hist = 1;

	if (triperrorcount < (MAX_TRIP_ERROR - errorcount))
		triperrorcount += errorcount;
	else
		triperrorcount = 100;
	if (errorwarning < (ERROR_WARNINGLIMIT - warningcount))
		errorwarning += warningcount;
	else
		errorwarning = 100;
	i = errorhist.histpointer;
	if (i < (MAX_HIST - 1))
		errorhist.histpointer++;
	else
		errorhist.histpointer = 0;

	errorhist.hist [i].triperrorcount = triperrorcount;
	if ((errortype & 0xFF00) == E_IV_ERROR)
	{
		memcpy(&errorhist.hist[i], &errstatus, sizeof (struct Hist));
		errorhist.hist [i].errorcode = errortype;
		errorhist.hist [i].idorpwr.node_id = node_id;
		errorhist.hist [i].err_maskcode = nice5000_Rfault_errsub;		
		errorhist.hist [i].triperrorcount = triperrorcount;

		errorhist.hist [i].nice5000_Rfault_setspd = nice5000_Rfault_setspd;
		errorhist.hist [i].nice5000_Rfault_feedspd = nice5000_Rfault_feedspd;
		errorhist.hist [i].nice5000_Rfault_dcvoltage = nice5000_Rfault_dcvoltage;
		errorhist.hist [i].nice5000_Rfault_outvoltage = nice5000_Rfault_outvoltage;
		errorhist.hist [i].nice5000_Rfault_outcurrent = nice5000_Rfault_outcurrent;
		errorhist.hist [i].nice5000_Rfault_outfreq = nice5000_Rfault_outfreq;
		errorhist.hist [i].nice5000_Rfault_outtorque = nice5000_Rfault_outtorque;
		errorhist.hist [i].nice5000_Rfault_tkcurrent = nice5000_Rfault_tkcurrent;
		errorhist.hist [i].nice5000_Rfault_outpwr = nice5000_Rfault_outpwr;
	}
	else
	{
		errorhist.hist [i].errorcode = errortype;
		errorhist.hist [i].idorpwr.node_id = node_id;
		errorhist.hist [i].t.second = t.second;
		errorhist.hist [i].t.minute = t.minute;
		errorhist.hist [i].t.hour = t.hour;
		errorhist.hist [i].t.day = t.day;
		errorhist.hist [i].t.month = t.month;
		errorhist.hist [i].t.weekday = t.weekday;
		errorhist.hist [i].t.year = t.year;
		errorhist.hist [i].hse_state = hse_text;
		errorhist.hist [i].direction = direction;
		errorhist.hist [i].drive_state = drive_state + 1;
		errorhist.hist [i].level = level;
		errorhist.hist [i].out = out;
		errorhist.hist [i].in1 = (in ^ in_polarity);
		errorhist.hist [i].in2 = (in ^ in_polarity) >> 8;
		errorhist.hist [i].in3 = (in ^ in_polarity) >> 16;
		errorhist.hist [i].calls_disabled = calls_enabled & CARCALL;
		errorhist.hist [i].landingcalls_disabled = calls_enabled & (LANDINGCALL_UP | LANDINGCALL_DN | LANDINGCALL);
		errorhist.hist [i].start_floor = start_floor;
		errorhist.hist [i].target_floor = target_floor;
		errorhist.hist [i].safety_circuit = safety_circuit;
		errorhist.hist [i].door_zone = door_zone;
		errorhist.hist [i].doorstate = doorstate;
		for (j = 0; j < MAX_DOOR; j++)
		{
			if (door_io [j] == NOT_AVAILABLE)
				errorhist.hist [i].door_io [j] = door_io [j];
			else
			{
				errorhist.hist [i].door_io [j] = door_io [j] & 0x07;
				if ((1 << j) & ls_state)
					errorhist.hist [i].door_io [j] |= 0x08;
			}
		}
		errorhist.hist [i].load = load;
		errorhist.hist [i].carlight = carlight;
		errorhist.hist [i].temperature = temperature;

		errorhist.hist [i].nice5000_Rfault_setspd = nice5000_Rstore_setspd;
		errorhist.hist [i].nice5000_Rfault_feedspd = nice5000_Rstore_feedspd;
		errorhist.hist [i].nice5000_Rfault_dcvoltage = nice5000_Rstore_dcvoltage;
		errorhist.hist [i].nice5000_Rfault_outvoltage = nice5000_Rstore_outvoltage;
		errorhist.hist [i].nice5000_Rfault_outcurrent = nice5000_Rstore_outcurrent;
		errorhist.hist [i].nice5000_Rfault_outfreq = nice5000_Rstore_outfreq;
		errorhist.hist [i].nice5000_Rfault_outtorque = nice5000_Rstore_outtorque;
		errorhist.hist [i].nice5000_Rfault_tkcurrent = nice5000_Rstore_tkcurrent;
		errorhist.hist [i].nice5000_Rfault_outpwr = nice5000_Rstore_outpwr;
		errorhist.hist [i].nice5000_errorpos = nice5000_Rstore_position;

		errorhist.hist [i].nice5000_startpos = nice5000_startpos;
		errorhist.hist [i].err_maskcode = 0;

	}

	if (node_id == HSE_ID)
		transmit_emergency (errortype);
	j = 0;
	for (i = 0; i < MAX_PCB_ERROR; i++)
	{
		if (errortext_bg [i].number == errortype)
		{
			switch (node_id)
			{
				case (HSE_ID):
					wptr_errcounter = &errorcounter.error_hse[i];
					++ (*wptr_errcounter);
			  		break;
				case (FVE_ID):
					wptr_errcounter = &errorcounter.error_fve[i];
					++ (*wptr_errcounter);
			  		break;
				case (DRV_ID):
				case (DCU_ID):
				case (EXE_ID):
					wptr_errcounter = &errorcounter.error_drv[i];
					++ (*wptr_errcounter);
			  		break;
				default:
					if ((node_id >= (128 + ESE_ID)) && (node_id < (128 + ESE_ID + MAX_ESE)))
			  		{							/* Error from ESE								*/
			  			wptr_errcounter = &errorcounter.error_ese[node_id - 128 - ESE_ID][i];
						++ (*wptr_errcounter);
					}
					else if ((node_id >= TSE_ID) && (node_id < (TSE_ID + MAX_TSE)))
					{
						wptr_errcounter = &errorcounter.error_tse[node_id - TSE_ID][i];
						++ (*wptr_errcounter);
					}
		  		break;
			}
			j = 1;
			break;
		}
	}
	if (!j)
	{
		i = 0;
		while (errortext [i].number != ENDSTRING)			/* search text structure						*/
		{
			if (errortext [i].number == errortype)
			{
				wptr_errcounter = &errorcounter.error_gen[i];
				++ (*wptr_errcounter);
				break;
			}
			i++;
		}
	}
}

void RecordErrorStatus(void)
{
	BYTE j;

	errstatus.t.second = t.second;
	errstatus.t.minute = t.minute;
	errstatus.t.hour = t.hour;
	errstatus.t.day = t.day;
	errstatus.t.month = t.month;
	errstatus.t.weekday = t.weekday;
	errstatus.t.year = t.year;
	errstatus.hse_state = hse_text;
	errstatus.direction = direction;
	errstatus.drive_state = drive_state + 1;
	errstatus.level = level;
	errstatus.out = out;
	errstatus.in1 = (in ^ in_polarity);
	errstatus.in2 = (in ^ in_polarity) >> 8;
	errstatus.in3 = (in ^ in_polarity) >> 16;
	errstatus.calls_disabled = calls_enabled & CARCALL;
	errstatus.landingcalls_disabled = calls_enabled & (LANDINGCALL_UP | LANDINGCALL_DN | LANDINGCALL);
	errstatus.start_floor = start_floor;
	errstatus.target_floor = target_floor;
	errstatus.safety_circuit = safety_circuit;
	errstatus.door_zone = door_zone;
	errstatus.doorstate = doorstate;
	for (j = 0; j < MAX_DOOR; j++)
	{
		if (door_io [j] == NOT_AVAILABLE)
			errstatus.door_io [j] = door_io [j];
		else
		{
			errstatus.door_io [j] = door_io [j] & 0x07;
			if ((1 << j) & ls_state)
				errstatus.door_io [j] |= 0x08;
		}
	}
	errstatus.load = load;
	errstatus.carlight = carlight;
	errstatus.temperature = temperature;

	errstatus.nice5000_errorpos = nice5000_Rstore_position;

	errstatus.nice5000_startpos = nice5000_startpos;
	errstatus.idorpwr.power24v = power24v;
}

BYTE GetBriefHist(void){
	BYTE i, j, k, m, a;
	WORD errcode = 0;
	BYTE result = 0;

	j = errorhist.histpointer;
	m = 0;
	for (i=0; i<MAX_HIST; ++i)
	{
		a = 0;
		k = (j + MAX_HIST - i - 1) % MAX_HIST;
		if (errorhist.hist[k].errorcode)
		{
			errcode = errorhist.hist[k].errorcode;
			while (errortext [a].number != ENDSTRING) 		/* search text structure					*/
			{
				if (errortext [a].number == errcode)
					{
						result = 1;
						break;
					}
				a++;
			}
			if(result)
				brief_err_info[i][0] = errortext [a].err_code;
			brief_err_info[i][1] = errorhist.hist[k].t.year;
			brief_err_info[i][2] = (errorhist.hist[k].t.month << 8) | errorhist.hist[k].t.day;
			brief_err_info[i][3] = (errorhist.hist[k].t.hour << 8) | errorhist.hist[k].t.minute;
			brief_err_info[i][4] = errorhist.hist[k].t.second;
			brief_err_info[i][5] = errorhist.hist[k].triperrorcount;
			++ m;
		}
		else
			break;
	}
	return m;
}

