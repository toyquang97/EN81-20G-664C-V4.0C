
#ifndef _HISTORY_H_
#define _HISTORY_H_

#include "rtc.h"

#define MAX_HIST				100					//最大错误记录条数
#define MAX_GEN_ERROR		110					// Number of different PCB independent errors
#define MAX_PCB_ERROR		7					// Number of different PCB dependent errors
#define ERRORCOUNTCHECK		0x5A25

union IDorPWR
{
	WORD node_id;
	WORD power24v;
};

struct Hist										// structure for error history
{
	WORD errorcode;								// actual error
	union IDorPWR idorpwr;
	struct Time t; 				  					// error time
	BYTE hse_state;								// HSE state
	BYTE direction;								// drive direction
	BYTE drive_state;								// drive state
	BYTE level;									// actual level
	BYTE out;									// output state HSE
	BYTE in1;									// input state HSE (debounced) Input 0..7
	BYTE in2;									// input state HSE (debounced) Input 8..15
	BYTE in3;									// input state HSE (debounced) Input 17..21
	WORD calls_disabled;  							// calls disabled
	WORD landingcalls_disabled;					// landingcalls disabled
	WORD door_zone;								// door zone state
	BYTE start_floor;								// start floor (normal trip)
	BYTE target_floor;								// target floor (normal trip)
	BYTE safety_circuit;							// state of safety circuit
	BYTE doorstate;								// actual door state
	BYTE door_io [MAX_DOOR];						// state of door inputs
	BYTE load;									// load measurement state
	BYTE carlight;									// car light state
	BYTE temperature;								// temperature state of motor and machine room
	BYTE triperrorcount;

	WORD err_maskcode;
	WORD nice5000_Rfault_setspd;
	WORD nice5000_Rfault_feedspd;
	WORD nice5000_Rfault_dcvoltage;
	WORD nice5000_Rfault_outvoltage;
	WORD nice5000_Rfault_outcurrent;
	WORD nice5000_Rfault_outfreq;
	WORD nice5000_Rfault_outtorque;
	WORD nice5000_Rfault_tkcurrent;
	WORD nice5000_Rfault_outpwr;
	DWORD nice5000_errorpos;
	DWORD nice5000_startpos;
//	BYTE triperrorcount;
};

struct ErrorHist									// structure for error history
{
	WORD check;
	BYTE histpointer;
	struct Hist hist[MAX_HIST];
//	WORD check;
};


struct ErrorCounter
{
	struct Time t; 				  					// error time
	WORD error_gen [MAX_GEN_ERROR];			// Generic error
	WORD error_tse [MAX_TSE][MAX_PCB_ERROR];	// Error from TSE
	WORD error_ese [MAX_ESE][MAX_PCB_ERROR];	// Error from ESE
	WORD error_hse [MAX_PCB_ERROR];			// Error from HSE
	WORD error_fve [MAX_PCB_ERROR];				// Error from FVE
	WORD error_drv [MAX_PCB_ERROR];				// Error from DRV
	WORD check;
};
struct ErrorCountDisp
{
	WORD count;									// Error count
	WORD number;								// Error number
	WORD pcb;									// Error PCB
};

#define COUNTSIZE	(MAX_GEN_ERROR + (MAX_TSE + MAX_ESE + 3) * MAX_PCB_ERROR)

#define	BRIEF_INFO_LENGTH		6

extern struct ErrorHist errorhist;
extern struct ErrorHist hist;
extern struct ErrorCounter errorcounter;
extern struct ErrorCountDisp count [COUNTSIZE];
extern WORD brief_err_info[MAX_HIST][BRIEF_INFO_LENGTH];

void write_errorhist (WORD errortype, WORD node_id, BYTE errorcount, BYTE warningcount);
void RecordErrorStatus(void);
BYTE GetBriefHist(void);

#endif

