
#include "typedef.h"
#include "parameter.h"
#include "nice5000.h"
#include "text.h"
#include "history.h"
#include "state.h"
#include "version.h"
#include "load.h"


//**************************************************************************************************
// Standard text
//**************************************************************************************************
const struct _standardtext standardtext[] = {
	{	"-----          0",		"-----          0",		T_STATUS1		},
	{	"-----      00:00",		"-----      00:00",		T_STATUS1		},
	{	"F?? D  SS:??????",		"F?? D  SS:??????",		T_STATUS1		},
	{	"          SL:???",		"          SL:???",		T_STATUS1		},

	{	"   Car calls",			"   Car calls",			T_STATUS2		},
	{	"-----",				"-----",				T_STATUS2		},
	{	"  Landing calls",		"   Landing calls",		T_STATUS2		},
	{	"-----",				"-----",				T_STATUS2		},

	{	" Door 1  Door 2",		" Door 1  Door 2",		T_STATUS3		},
	{	"",						"",						T_STATUS3		},
	{	"Time=   Time=",		"Time=   Time=",		T_STATUS3		},

	{	"Timers",				"Timers",				T_STATUS4		},
	{	"DT=",					"DT=",					T_STATUS4		},
	{	"PT=",					"PT=",					T_STATUS4		},
	{	"LT=     RMT:",			"LT=     RMT:",			T_STATUS4		},

	{	"Floor =         ",		"Floor =         ",		T_STATUS5		},
	{	"Pos =         cm",		"Pos =         cm",		T_STATUS5		},
	{	"Pul =           ",		"Pul =           ",		T_STATUS5		},
	{ "OutCur = 			A", 	"OutCur = 			A", 	T_STATUS5 	},

	{	"SetSpd=      m/s",		"SetSpd=      m/s",		T_STATUS6		},
	{	"FedSpd=      m/s",		"FedSpd=      m/s",		T_STATUS6		},
	{	"DC vol =       V",		"DC vol =       V",		T_STATUS6		},
	{	"Out vol =      V",		"Out vol =      V",		T_STATUS6		},

	{ "OutPwr = 		 kW", 	"OutPwr = 		 kW", 	T_STATUS7 	},
	{	"OutFrq =      Hz",		"OutFrq =      Hz",		T_STATUS7		},
	{	"OutTor =       N",		"OutTor =       %",		T_STATUS7		},
	{	"TorCur =       A",		"TorCur =       A",		T_STATUS7		},
	
	{	"Please wait ...",		"Please wait ...",		T_WAIT			},
	{	"Value too small",		"Value too small",		T_TOO_SMALL		},
	{	"Value too big",		"Value too big",		T_TOO_BIG		},
	{	"..successful",			"..successful",			T_SUCCESS		},
	{	"..failed",				"..failed",				T_NO_SUCCESS	},
	{ "\5:\5 SL:0 \6:\6 <VI>", "\5:\5 SL:0 \6:\6 <VI>", T_MOVE			},
	{	"F??    SS:??????",		"F??    SS:??????",		T_MOVE			},
	{	"",						"",						T_MOVE			},
	{	"ENT:Stop ESC:Ext",		"ENT:Stop ESC:Ext",		T_MOVE			},
	{	"Safety line off",		"Safety line off",		T_MOVEERROR1	},
	{	"CAN-Error Drive",		"CAN-Error Drive",		T_MOVEERROR2	},
	{	"Brake error",			"Brake error",			T_MOVEERROR3	},
	{	"Timeout Stopping",		"Timeout Stopping",		T_MOVEERROR4	},
	{	"Contactor error",		"Contactor error",		T_MOVEERROR5	},
	{	"Drive error",			"Drive error",			T_MOVEERROR6	},
	{	"Closing door..",		"Closing door..",		T_T_DOORCL		},
	{	"Cancel with ESC",		"Cancel with ESC",		T_T_DOORCL		},
	{	"Door error",			"Door error",			T_T_DOORERROR	},
	{	"Check shaftsensor",	"Check shaftsensor",	T_T_POSERROR	},
	{	"state!",				"state!",				T_T_POSERROR	},
	{	"Teach in not",			"Teach in not",			T_T_NO_ACCESS	},
	{	"possible,check",		"possible,check",		T_T_NO_ACCESS	},
	{	"lift state!",			"lift state!",			T_T_NO_ACCESS	},
	{	"Teach in trip to",		"Teach in trip to",		T_T_TOP			},
	{	"top floor",			"top floor",			T_T_TOP			},
	{	"Cancel with ESC",		"Cancel with ESC",		T_T_TOP			},
	{	"Trip Time Out",		"Trip Time Out",		T_T_TIME		},
	{	"State changed.",		"State changed.",		T_T_GEN_ERROR	},
	{	"New state:",			"New state:",			T_T_GEN_ERROR	},
	{	"Teach in trip",		"Teach in trip",		T_T_READY		},
	{	"successfull.",			"successfull.",			T_T_READY		},
	{	"Save parameters",		"Save parameters",		T_T_READY		},
	{	"with <ENT>",			"with <ENT>",			T_T_READY		},
	{	"SL off",				"SL off",				T_T_SL_OFF		},
	{	"Group lifts",			"Group lifts",			T_GROUP			},
	{	"1......8",				"1......8",				T_GROUP			},
	{	"Stack end",			"Stack end",			T_HIST_END		},
	{	"--- ESC ---",			"-- ESC --",			T_ESC			},
	{	"Are you sure?",		"Are you sure?",		T_SURE			},
	{	"Continue:    ENT",		"Continue:    ENT",		T_SURE			},
	{	"Cancel:      ESC",		"Cancel:      ESC",		T_SURE			},
	{	"-- Continue --",		"-- Continue --",		T_CONTINUE		},
	{	"---- Back ----",		"-- Back --",			T_BACK			},
	{	"--- Cancel ---",		"-- Cancel -",			T_CANCEL		},
	{	"Attention! Some",		"Attention! Some",		T_SIMUPARA		},
	{	"setup parameters",		"setup parameters",		T_SIMUPARA		},
	{	"will be changed!",		"will be changed!",		T_SIMUPARA		},
	{	"Cont.:#/Cancel:*",		"Cont.:#/Cancel:*",		T_SIMUPARA		},
	{	"Attention! Input",		"Attention! Input",		T_INPARA		},
	{	"already used!",		"already used!",		T_INPARA		},
	{	"Continue:    ENT",		"Continue:    ENT",		T_INPARA		},
	{	"Cancel:      ESC",		"Cancel:      ESC",		T_INPARA		},
	{	"Attention!Output",		"Attention!Output",		T_OUTPARA		},
	{	"already used!",		"already used!",		T_OUTPARA		},
	{	"Continue:    ENT",		"Continue:    ENT",		T_OUTPARA		},
	{	"Cancel:      ESC",		"Cancel:      ESC",		T_OUTPARA		},
	{	"Set call",				"Set call",				T_SET_CALL		},
	{	"automatic?",			"automatic?",			T_SET_CALL		},
	{	"Yes ENT / No ESC",		"Yes ENT / No ESC",		T_SET_CALL		},
	{	"Set also call",		"Set also call",		T_SET_QUIT		},
	{	"ackknowledgement",		"ackknowledgement",		T_SET_QUIT		},
	{	"Yes ENT / No ESC",		"Yes ENT / No ESC",		T_SET_QUIT		},
	{	"Clear also",			"Clear also",			T_CLEAR_INPUT	},
	{	"input ?",				"input ?",				T_CLEAR_INPUT	},
	{	"Yes ENT / No ESC",		"Yes ENT / No ESC",		T_CLEAR_INPUT	},
	{	"Clear also",			"Clear also",			T_CLEAR_OUTPUT	},
	{	"output ?",				"output ?",				T_CLEAR_OUTPUT	},
	{	"Yes ENT / No ESC",		"Yes ENT / No ESC",		T_CLEAR_OUTPUT	},
	{	"Testruns",				"Testruns",				T_TESTTRIP		},
	{	"Target floor",			"Target floor",			T_CORRECT_TARGET},
	{	"Actual floor",			"Actual floor",			T_CORRECT_TARGET},
	{	"Adjusted       0",		"Adjusted       0",		T_CORRECT_LEVEL	},
	{	"Doing          0",		"Doing          0",		T_CORRECT_LEVEL	},
	{	"",						"",						T_CORRECT_LEVEL	},
	{	"Act.Diff       0",		"Act.Diff       0",		T_CORRECT_LEVEL	},
	{	"Call activated",		"Call activated",		T_CORRECT_CALL	},

	{	"Enter password:",		"Enter password:",		T_PASSWORD		},
	{	"Wrong password!",		"Wrong password!",		T_PASSWORD_WRONG},
	{	"Access denied!",		"Access denied!",		T_PASSWORD_WRONG},
	{	"Actual floor",			"Actual floor",			T_CAR_CALL		},
	{	"Call floor",			"Call floor",			T_CAR_CALL		},
	{	"Call entered",			"Call entered",			T_CALL_READY	},
	{	"Calls disabled",		"Calls disabled",		T_CALLS_DISABLED},

	{	"Old volt:",			"Old volt:",			T_RENDER_LOAD	},
	{	"New volt:",			"New:",					T_RENDER_LOAD	},
	{	"",						"",						T_RENDER_LOAD	},
	{	"ENT:Update",			"ENT:Update",			T_RENDER_LOAD	},

	{	"Count start:",			"Count start:",			T_ERRCNT_START	},

	{	"Insufficient",			"Insufficient",			T_DENY_ACCESS	},
	{	"privileges to",		"privileges to",		T_DENY_ACCESS	},
	{	"access!",				"access!",				T_DENY_ACCESS	},

	{	"Press ENT to",			"Press ENT to",			T_DISP_ID		},
	{	"exit!",				"exit!",				T_DISP_ID		},
	{	"User aborted!",		"User aborted!",		T_ABORT_ID		},

	{	"No response!",			"No response!",			T_ACK_OVERTIMER	},

	{	"Load weighting",		"Load weighting",		T_DIGIT_LOAD_NA	},
	{	"can't be adjusted",	"can't be adjusted",	T_DIGIT_LOAD_NA	},
	{	"in digit type!",		"in digit type!",		T_DIGIT_LOAD_NA	},

	{	"Limit function",		"Limit function",		T_TRIPLMT_START	},
	{	"is working!",			"is working!",			T_TRIPLMT_START	},

	{	"Reading eeprom",		"Reading eeprom",		T_EE_ERROR		},
	{	"error, wait for",		"error, wait for",		T_EE_ERROR		},
	{	"reset ...",			"reset ...",			T_EE_ERROR		},
	{	"Checksum error,",		"Checksum error,",		T_CHKSUM_ERROR	},
	{	"wait for reset..",		"wait for reset..",		T_CHKSUM_ERROR	},

	{	"Attention pls!",		"Attention pls!",		T_OVERWRITE_WARNNG		},
	{	"All parameters",		"All parameters",		T_OVERWRITE_WARNNG		},
	{	"may be changed!",		"may be changed!",		T_OVERWRITE_WARNNG		},
	{	"Yes ENT / No ESC",		"Yes ENT / No ESC",		T_OVERWRITE_WARNNG		},

	{	"Completed! wait",		"Completed! wait",		T_OK_AND_RESET			},
	{	"for reset ...",		"for reset ...",		T_OK_AND_RESET			},
	{	"Cancelled! wait",		"Cancelled! wait",		T_CANCEL_AND_RESET		},
	{	"for reset ...",		"for reset ...",		T_CANCEL_AND_RESET		},

	{	"no TSE allocated",		"no TSE allocated",		T_TSE_NOTALLOCATE		},
	{	"no ESE allocated",		"no ESE allocated",		T_ESE_NOTALLOCATE		},
	{ "no EXE allocated", 	"no EXE allocated", 	T_EXE_NOTALLOCATE 	},		

	{	"Not available in",		"Not available in",		T_TRIPLMT_NOTAVAILABLE	},
	{	"test run mode!",		"test run mode!",		T_TRIPLMT_NOTAVAILABLE	},

	{	"Trip limit is",		"Trip limit is",		T_TRIPLMT_CANCELLED		},
	{	"cancelled!",			"cancelled!",			T_TRIPLMT_CANCELLED		},
	
	{ "Please wait ...", 		"Please wait ...", 		T_BRAKE_CHECK1},
	{ "until brake",				"until brake",				T_BRAKE_CHECK1 	},
	{ "check finish!",			"check finish!",				T_BRAKE_CHECK1 	},
	{ "ENT to exit", 			"ENT to exit", 			T_BRAKE_CHECK1},

	{ "Brake check is", 		"Brake check is", 		T_BRAKE_CHECK2},
	{ "OK!",								"OK!",								T_BRAKE_CHECK2 	},
	{ "Press ENT to", 			"Press ENT to", 			T_BRAKE_CHECK2},
	{ "exit!",							"exit!",							T_BRAKE_CHECK2 	},
		
	{ "Brake check is", 		"Brake check is", 		T_BRAKE_CHECK3},
	{ "not good!",					"not good!",					T_BRAKE_CHECK3 	},
	{ "Press ENT to", 			"Press ENT to", 			T_BRAKE_CHECK3},
	{ "exit!",							"exit!",							T_BRAKE_CHECK3 	},
	
	{ "Safety line off", 		"Safety line off", 		T_BRAKE_CHECK4},
	{ "Press ENT to", 			"Press ENT to", 			T_BRAKE_CHECK4},
	{ "exit!",							"exit!",							T_BRAKE_CHECK4},
	
	{ "Driver not run",			"Driver not run",			T_BRAKE_CHECK5},
	{ "Press ENT to", 			"Press ENT to", 			T_BRAKE_CHECK5},
	{ "exit!",							"exit!",							T_BRAKE_CHECK5},
	
	{ "Lift is moving", 	"Lift is moving", 			T_BACKUP_INVALID 		},
	{ "Cancel with ESC",		"Cancel with ESC",		T_BACKUP_INVALID 		},

	{ "Select RFID num.", 	"Select RFID num.", 	T_SELECT_RDID		},
	{ "Select COP num.", 		"Select COP num.", 		T_SELECT_COPDISPLAY 	},
	{ "Select LIOP num.", 	"Select LIOP num.", 	T_SELECT_LIOPDISPLAY 	},

	{ "The ucmp test", 	"The ucmp test", 	T_UCMP_NOT_FINISH		},
	{ "not finish,", 		"not finish,", 	T_UCMP_NOT_FINISH 	},
	{ "please wait or", "please wait or", 	T_UCMP_NOT_FINISH 	},
	{ "ESC exit", 			"ESC exit", 	T_UCMP_NOT_FINISH 	},

	{	"",						"",						ENDSTRING		},
};


//**************************************************************************************************
// Menu text
//**************************************************************************************************
const struct _menutext menutext[] = {
	{	"P0",				"General paramet.",		M_MAIN,			M_GEN_PARA,		RIGHT_MAINTAIN	},
	{	"P1",				"Timer parameter",		M_MAIN,			M_TIMERPARA ,	RIGHT_MAINTAIN	},
	{	"P2",				"Drive parameter",		M_MAIN,			M_DRIVE_COMMON,	RIGHT_MAINTAIN	},
	{	"P3",				"Hardware test",		M_MAIN,			M_HARDWARE,		RIGHT_MAINTAIN	},
	{	"P4",				"Errors",				M_MAIN,			M_ERROR,		RIGHT_MAINTAIN	},
	{	"P5",				"Set clock",			M_MAIN,			M_TIME,			RIGHT_MAINTAIN	},
	{	"P6",				"Config LCU ID",		M_MAIN,			M_LCU_ID,		RIGHT_MAINTAIN	},
	{	"P7",				"Load weighing",		M_MAIN,			M_SET_LOAD,		RIGHT_MAINTAIN	},
	{	"P8",				"Floor name",			M_MAIN,			M_SIGN,			RIGHT_MAINTAIN	},
	{	"P9",				"Relevel paramet.",		M_MAIN,			M_RELEPARA,		RIGHT_MAINTAIN	},
	{	"PA",				"Internal param.",		M_MAIN,			M_INTERNPARA,	RIGHT_MAINTAIN	},
	{	"PB",				"Basic setting",		M_MAIN,			M_BASIC,		RIGHT_MAINTAIN	},
	{ "PC", 			"Check function",		M_MAIN, 		M_CHECK,		RIGHT_MAINTAIN	},
	{	"PD",				"Door parameter",		M_MAIN,			M_DOORPARA,		RIGHT_MAINTAIN	},
	{ "PM", 			"Spection mode", 	M_MAIN, 		M_SPECTION_MODE, 	RIGHT_MAINTAIN	},
	{	"PP",				"In-/outputs",			M_MAIN,			M_IN_OUTPUTS,	RIGHT_MAINTAIN	},
	{	"PR",				"Run control",			M_MAIN,			M_RUNPARA,		RIGHT_MAINTAIN	},
	{	"PT",				"Teach in trip",		M_MAIN,			M_TEACH_IN,		RIGHT_MAINTAIN	},
	{	"PU",				"Monitoring para.",		M_MAIN,			M_MONITOR,		RIGHT_MAINTAIN	},
	{ "PW", 			"Change password",		M_MAIN, 		M_CHANGE_PW,	RIGHT_MAINTAIN	},

	{	"F0",				"F0",					M_MAIN,			M_NICE_F0,		RIGHT_MAINTAIN	},
	{	"F1",				"F1",					M_MAIN,			M_NICE_F1,		RIGHT_MAINTAIN	},
	{	"F2",				"F2",					M_MAIN,			M_NICE_F2,		RIGHT_MAINTAIN	},
	{	"F3",				"F3",					M_MAIN,			M_NICE_F3,		RIGHT_MAINTAIN	},
	{	"F4",				"F4",					M_MAIN,			M_NICE_F4,		RIGHT_MAINTAIN	},
	{	"F5",				"F5",					M_MAIN,			M_NICE_F5,		RIGHT_MAINTAIN	},
	{	"FA",				"FA",					M_MAIN,			M_NICE_FA,		RIGHT_MAINTAIN	},
	{	"FH",				"FH",					M_MAIN,			M_NICE_FH,		RIGHT_MAINTAIN	},
	{	"FF",				"FF",					M_MAIN,			M_NICE_FF,		RIGHT_MAINTAIN	},
	{	"FR",				"FR",					M_MAIN, 		M_NICE_FR,		RIGHT_MAINTAIN	},
	{	"-- ESC --",		"-- ESC --",			M_MAIN,			M_STATUS1,		RIGHT_MAINTAIN	},

	{	"P3-00",			"MCU in-/outputs",		M_HARDWARE,		M_HSE_INFO,		RIGHT_MAINTAIN	},
	{	"P3-01",			"CDU in-/outputs",		M_HARDWARE,		M_FVE_INFO,		RIGHT_MAINTAIN	},
	{	"P3-02",			"PCU in-/outputs",		M_HARDWARE,		M_TSE,			RIGHT_MAINTAIN	},
	{	"P3-03",			"LCU in-/outputs",		M_HARDWARE, 	M_ESE,			RIGHT_MAINTAIN	},
	{	"P3-04",			"Group test",					M_HARDWARE, 	M_GROUP,		RIGHT_MAINTAIN	},
	{ "P3-05",			"EXU in-/outputs", 		M_HARDWARE, 	M_EXE,		RIGHT_MAINTAIN	},
	{	"-- ESC --",		"-- ESC --",			M_HARDWARE,		M_MAIN,			RIGHT_MAINTAIN	},

	{ "PT-00",			"Motor tune", 	M_TEACH_IN, 	M_MOTOR_TUNE, RIGHT_INSTALL },
	{	"PT-01",			"Shaft sensors",M_TEACH_IN,		M_SHAFT_SENSORS,RIGHT_INSTALL	},
	{	"-- ESC --",		"-- ESC --",			M_TEACH_IN,		M_MAIN,			RIGHT_MAINTAIN	},

	{	"P4-00",			"Error stack",			M_ERROR,		M_SHOW_HIST,	RIGHT_MAINTAIN	},
	{	"P4-01",			"Error count",			M_ERROR,		M_SHOW_COUNT,	RIGHT_MAINTAIN	},
	{	"P4-02",			"Clear stack",			M_ERROR,		M_CL_HIST,		RIGHT_INSTALL	},
	{	"P4-03",			"Clear count",			M_ERROR,		M_CLEAR_COUNT,	RIGHT_INSTALL	},
	{	"-- ESC --",		"-- ESC --",			M_ERROR,		M_MAIN,			RIGHT_MAINTAIN	},

	{	"P6-00",			"Display LCU ID",		M_LCU_ID,		M_DISP_ID,		RIGHT_INSTALL	},
	{	"P6-01",			"Set ID by turn",		M_LCU_ID,		M_SET_ID_1,		RIGHT_INSTALL	},
	{	"P6-02",			"Set ID by turn 2",	M_LCU_ID,		M_SET_ID_2,		RIGHT_INSTALL	},
	{	"-- ESC --",		"-- ESC --",			M_LCU_ID,		M_MAIN,			RIGHT_MAINTAIN	},

	{	"P7-00",			"Empty load:0%",		M_SET_LOAD,		M_EMPTY_LOAD,	RIGHT_INSTALL	},
	{	"P7-01",			"Half load:50%",		M_SET_LOAD,		M_HALF_LOAD,	RIGHT_INSTALL	},
	{	"P7-02",			"Full load:95%",		M_SET_LOAD,		M_FULL_LOAD,	RIGHT_INSTALL	},
	{	"P7-03",			"Over load:102%",		M_SET_LOAD,		M_OVER_LOAD,	RIGHT_INSTALL	},
	{	"P7-04",			"Save values",			M_SET_LOAD,		M_SAVEPARA,		RIGHT_INSTALL	},
	{	"-- ESC --",		"-- ESC --",			M_SET_LOAD,		M_MAIN,			RIGHT_MAINTAIN	},
	
	{	"PD0",				"Door1 parameter",		M_DOORPARA,		M_DOOR1PARA,	RIGHT_MAINTAIN	},
	{	"PD1",				"Door2 parameter",		M_DOORPARA,		M_DOOR2PARA,	RIGHT_MAINTAIN	},
	{	"PD2",				"Shaft doors",			M_DOORPARA,		M_DOORPOS, 		RIGHT_MAINTAIN	},
	{	"PD3",				"Door park mode",		M_DOORPARA,		M_PARKDOOR,		RIGHT_MAINTAIN	},
	{	"PD4",				"Normal run,CC",		M_DOORPARA,		M_DOOR_NORMAL,	RIGHT_MAINTAIN	},
	{	"PD5",				"Normal run,LC",		M_DOORPARA,		M_DOOR_N_HALL,	RIGHT_MAINTAIN	},
	{	"PD6",				"Auto-call",			M_DOORPARA,		M_DOOR_AUTOCALL,RIGHT_MAINTAIN	},
	{	"PD7",				"Clock run 1,CC",		M_DOORPARA,		M_DOOR_TIMER_1,	RIGHT_MAINTAIN	},
	{	"PD8",				"Clock run 1,LC",		M_DOORPARA,		M_DOOR_T1_HALL,	RIGHT_MAINTAIN	},
	{	"PD9",				"Clock run 2,CC",		M_DOORPARA,		M_DOOR_TIMER_2,	RIGHT_MAINTAIN	},
	{	"PD10",				"Clock run 2,LC",		M_DOORPARA,		M_DOOR_T2_HALL,	RIGHT_MAINTAIN	},
	{	"PD11",				"Special run",			M_DOORPARA,		M_DOOR_SPECIAL,	RIGHT_MAINTAIN	},
	{	"PD12",				"VIP run",				M_DOORPARA,		M_DOOR_ADV,		RIGHT_MAINTAIN	},
	{	"PD13",				"Emergency run",		M_DOORPARA,		M_DOOR_EMERG,	RIGHT_MAINTAIN	},
	{	"PD14",				"Fire evac. run",		M_DOORPARA,		M_DOOR_ALARM,	RIGHT_MAINTAIN	},
	{	"PD15",				"Firemen run",			M_DOORPARA,		M_DOOR_FIRE,	RIGHT_MAINTAIN	},
	{ "PD16", 			"LP/CP door Set",	M_DOORPARA, 	M_DOUBLE_DOOR,	RIGHT_MAINTAIN	},
	{	"-- ESC --",		"-- ESC --",			M_DOORPARA,		M_MAIN,			RIGHT_MAINTAIN	},

	{	"PP-00",			"MCU inputs",			M_IN_OUTPUTS,	M_IN_HSE,		RIGHT_INSTALL	},
	{	"PP-01",			"MCU outputs",			M_IN_OUTPUTS,	M_OUT_HSE,		RIGHT_INSTALL	},
	{	"PP-10",			"CDU inputs",			M_IN_OUTPUTS,	M_IN_FVE,		RIGHT_INSTALL	},
	{	"PP-11",			"CDU outputs",			M_IN_OUTPUTS,	M_OUT_FVE,		RIGHT_INSTALL	},
	{	"PP-20",			"PCU inputs",			M_IN_OUTPUTS,	M_TSE_IN_LIST,	RIGHT_INSTALL	},
	{	"PP-21",			"PCU outputs",			M_IN_OUTPUTS,	M_TSE_OUT_LIST,	RIGHT_INSTALL	},
	{	"PP-30",			"LCU inputs",			M_IN_OUTPUTS,	M_ESE_IN_LIST,	RIGHT_INSTALL	},
	{	"PP-31",			"LCU outputs",			M_IN_OUTPUTS,	M_ESE_OUT_LIST,	RIGHT_INSTALL	},
	{ "PP-40",			"EXU inputs", 		M_IN_OUTPUTS, M_EXE_IN_LIST,	RIGHT_INSTALL },
	{ "PP-41",			"EXU outputs",			M_IN_OUTPUTS, M_EXE_OUT_LIST, RIGHT_INSTALL },
	{	"-- ESC --",		"-- ESC --",			M_IN_OUTPUTS,	M_MAIN,			RIGHT_MAINTAIN	},

	{	"PR0",				"Special run adj.",		M_RUNPARA,		M_SPECIAL_TRIP,	RIGHT_MAINTAIN	},
	{	"PR1",				"VIP run adj.",			M_RUNPARA,		M_ADV_TRIP,		RIGHT_MAINTAIN	},
	{	"PR2",				"Emerg. run adj.",		M_RUNPARA,		M_EMERG_TRIP,	RIGHT_MAINTAIN	},
	{	"PR3",				"Evacuation run",		M_RUNPARA,		M_EVACUATION,	RIGHT_MAINTAIN	},
	{	"PR4",				"Fire evac.floors",		M_RUNPARA,		M_FIRE_FLOOR,	RIGHT_MAINTAIN	},
	{	"PR6",				"Firemen run adj.",		M_RUNPARA,		M_FIREMAN_TRIP,	RIGHT_MAINTAIN	},
	{	"PR7",				"Compulsory stop",		M_RUNPARA,		M_FORCED_STOP,	RIGHT_MAINTAIN	},
	{	"PR8",				"Remote off",			M_RUNPARA,		M_REMOTE_OFF,	RIGHT_MAINTAIN	},
	{	"PR9",				"Clock run",			M_RUNPARA,		M_TIMER_TRIP,	RIGHT_MAINTAIN	},
	{	"PR10",				"Control calls",		M_RUNPARA,		M_DIS_ENABLE,	RIGHT_MAINTAIN	},
	{	"PR11",				"Car attendance",		M_RUNPARA,		M_ATTENDANCE,	RIGHT_MAINTAIN	},
	{	"PR12",				"Cooperate func.",		M_RUNPARA,		M_COOPERATE,		RIGHT_MAINTAIN	},
	{	"PR14",				"Anti nuisance",		M_RUNPARA,		M_MISUSE,		RIGHT_MAINTAIN	},
	{	"PR15",				"Peak control",			M_RUNPARA,		M_GROUPCONTROL,	RIGHT_MAINTAIN	},
	{	"PR16",				"Trip limit",			M_RUNPARA,		M_TRIPLIMIT,	RIGHT_MAINTAIN	},
	{	"PR17",				"Test runs",			M_RUNPARA,		M_TESTTRIP,		RIGHT_MAINTAIN	},
	{	"PR18",				"Travel commands",		M_RUNPARA,		M_MOVE,			RIGHT_INSTALL	},
	{	"-- ESC --",		"-- ESC --",			M_RUNPARA,		M_MAIN,			RIGHT_MAINTAIN	},

	{	"PU-00",			"Floorcount",			M_MONITOR,		M_FLOORCOUNT,	RIGHT_MAINTAIN	},
	{	"PU-01",			"Clear trip count",		M_MONITOR,		M_CLR_TRIPCOUNT,RIGHT_INSTALL	},
	{	"PU-02",			"Clear op.hours",		M_MONITOR,		M_CLR_HOURCOUNT,RIGHT_INSTALL	},
	{	"PU-03",			"Clear floorcount",		M_MONITOR,		M_CLR_FLOORCNT,	RIGHT_INSTALL	},
	{ "PU-04",			"Floor height", 	M_MONITOR, 	M_LEVEL_HEIGHT,	RIGHT_INSTALL },
	{ "PU-05",			"Can disturb", 	M_MONITOR,	M_CAN_DISTURB, RIGHT_INSTALL },
	{ "PU-07",			"UCMP test",	M_MONITOR,	M_UCMP_TEST, RIGHT_INSTALL },
	{	"-- ESC --",		"-- ESC --",			M_MONITOR,		M_MAIN,			RIGHT_MAINTAIN	},
	
	{	"PW-00",			"Maintain",				M_CHANGE_PW,	M_CHANGE_PW0,	RIGHT_MAINTAIN	},
	{	"PW-01",			"Install",				M_CHANGE_PW,	M_CHANGE_PW1,	RIGHT_INSTALL	},
	{	"PW-02",			"Help debug",			M_CHANGE_PW,	M_CHANGE_PW2,	RIGHT_HELP		},
	{ "PW-03",			"Administrator",	M_CHANGE_PW,	M_CHANGE_PW3, RIGHT_USER},
	{	"-- ESC --",		"-- ESC --",		M_CHANGE_PW,	M_MAIN,			RIGHT_MAINTAIN	},

	{	"Door limt switch",	"Door limt switch",		0,				M_DOORSWITCH,	RIGHT_MAINTAIN	},
	{	"Door relay state",	"Door relay state",		0,				M_DOORRELAY,	RIGHT_MAINTAIN	},

	{	"..successfull",	"..successfull",		M_SET_TIME,		M_MAIN,			RIGHT_MAINTAIN	},

	{	"",					"",						ENDSTRING,		0,				RIGHT_MAINTAIN	},
};

//**************************************************************************************************
// HSE state text
//**************************************************************************************************
const struct _textlist statetext[] = {
	{	"Emer.Stop",	"Emer.Stop",	},			// State text  0x01 HT_WAIT_STOP
	{	"Init",				"Init",			},			// State text  0x02 HT_INIT
	{	"Search",			"Search",		},			// State text  0x03 HT_SEARCH
	{	"Standstil",	"Standstil",	},			// State text  0x04 HT_STANDSTILL
	{	"Moving",			"Moving",		},			// State text  0x05 HT_MOVING
	{	"Arrival",		"Arrival",		},			// State text  0x06 HT_ARRIVAL
	{	"Error",			"Error",		},			// State text  0x07 HT_ERROR
	{	"CAN-Error",	"CAN-Error",	},			// State text  0x08 HT_CAN_ERROR
	{	"Inspect.",		"Inspect.",		},			// State text  0x09 HT_INSP
	{	"Em.recall",	"Em.recall",	},			// State text  0x0A HT_EM_OP
	{	"!Repair!",		"!Repair!",		},			// State text  0x0B HT_RESTART_INH
	{	"Terminal",		"Terminal",		},			// State text  0x0C HT_TERM
	{	"PCB fault",	"PCB fault",	},			// State text  0x0D HT_HB_ERROR
	{	"Starting",		"Starting",		},			// State text  0x0E HT_START
	{	"Leveling",		"Leveling",		},			// State text  0x0F HT_DIVING
	{	"Teach in",		"Teach in",		},			// State text  0x10 HT_TEACH_IN
	{	"SL error",		"SL error",		},			// State text  0x11 HT_SC_ERROR
	{	"Door err.",	"Door err.",	},			// State text  0x12 HT_DOORERROR
	{	"Door open",	"Door open",	},			// State text  0x13 HT_DOOROPEN
	{	"Refer.Run",	"Refer.Run",	},			// State text  0x14 HT_RESET
	{	"Setup",			"Setup",		},			// State text  0x15 HT_NO_SETUP
	{	"Driveerr.",	"Driveerr.",	},			// State text  0x16 HT_DRIVE_ERROR
	{	"SPI-DRV",		"SPI-DRV"		},			// State text  0x17 HT_HB_DRV
	{	"SL ontrip",	"SL ontrip",	},			// State text  0x18 HT_DRV_SC
	{	"Run.Cont.",	"Run.Cont.",	},			// State text  0x19 HT_DRV_RUNCON
	{	"Brakeerr.",	"Brakeerr.",	},			// State text  0x1A HT_DRV_BRAKE
	{	"Drivetrip",	"Drivetrip",	},			// State text  0x1B HT_DRV_MOVE
	{	"CAN-CDU",		"CAN-CDU",		},			// State text  0x1C HT_HB_FVE
	{	"Err.U/LCM",	"Err.U/LCM",	},			// State text  0x1D HT_VO_VU
	{	"Overtemp.",	"Overtemp.",	},			// State text  0x1E HT_OVERTEMP
	{	"Brakechk.",	"Brakechk.",	},			// State text  0x1F HT_BRAKESTATE
	{	"SCcardoor",	"SCcardoor",	},			// State text  0x20 HT_SC2
	{	"SL shdoor",	"SL shdoor",	},			// State text  0x21 HT_SC3
	{	"Zon.n.off",	"Zon.n.off",	},			// State text  0x22 HT_KH5_ON
	{	"Zon.n.on",		"Zon.n.on",		},			// State text  0x23 HT_KH5_OFF
	{	"Car light",	"Car light",	},			// State text  0x24 HT_CARLIGHT
	{	"Overload",		"Overload",		},			// State text  0x25 HT_OVERLOAD
	{	"Relevel.",		"Relevel.",		},			// State text  0x26 HT_RELEVEL
	{	"No CAN-A",		"No CAN-A",		},			// State text  0x27 HT_NO_CAN_A
	{	"Trip time",	"Trip time",	},			// State text  0x28 HT_TRIP_TIME
	{	"Doortest",		"Doortest",		},			// State text  0x29 HT_DOORTEST
	{	"Limit sw.",	"Limit sw.",	},			// State text  0x2A HT_SC_LIMIT
	{	"UPS evac.",	"UPS evac.",	},			// State text  0x2B HT_EVACUATION
	{	"Diving err",	"Diving err",	},			// State text  0x2C HT_DIVING_ERR
	{	"!Mainten.",	"!Mainten.",	},			// State text  0x2D HT_WARNING
	{	"Checksum",		"Checksum",		},			// State text  0x2E HT_EE_ERROR
	{	"IV-teach",		"IV-teach",		},			// State text  0x2F HT_IV_TEACH_IN
	{	"DL short",		"DL short",		},			// state text  0x30 HT_DL_SHORTED
	{	"UIL on",			"UIL on",		},			// state text  0x31 HT_ILO_ON
	{	"LIL on",			"LIL on",		},			// state text  0x32 HT_ILU_ON
	{ "Limitcall",	"Limitcall"		},			// State text  0x33 HT_LIMITCALL
	{ "BRK.Cont.",	"BRK.Cont."		},			// State text  0x34 HT_DRV_BRKCON
	{ "FX.Cont.",		"FX.Cont."		},			// State text  0x35 HT_DRV_FXCON
	{ "Safe.beam",	"Safe.beam"		},			// State text  0x36 HT_SAFETY_BEAM
	{ "MSI notoff",	"MSI notoff"		},	// State text  0x37 HT_MSI_NOTOFF
	{ "RunSig.err", "RunSig.err"		},	// State text  0x38 HT_DRV_NICE_RUNSIG
	{ "BrkSig.err", "BrkSig.err"		},	// State text  0x39 HT_DRV_NICE_BRAKESIG
	{ "DirSig.err", "DirSig.err"		},	// State text  0x3A HT_DRV_NICE_DIRSIG
	{ "CAN-EXE",		"CAN-EXE",		},		// State text  0x3B HT_HB_EXE
	{ "DoorCon.",		"DoorCon.",		},		// State text  0x3C DoorCon.
	{ "Access op.", "Access op.", 	},		// State text  0x3D ACCESS
	{ "SteelBroke", "SteelBroke", 	},		// State text  0x3E STEEL BROKE
	{	"",				""				},			// Endstring
};

//**************************************************************************************************
// error text
//**************************************************************************************************
const struct _errortext errortext[MAX_GEN_ERROR] = {
	{	E_SC_VOLTAGE,				101,		"SL off",					"SL off",					0	},
	{	E_SC_VOLTAGE_MOVE,		102,		"SL off trip",		"SL off trip",		0	},
	{	E_SC_VOLTAGE_DOOR,		103,		"SL door off",		"SL door off",		0	},
	{	E_SC_VOLTAGE_LIMIT,		104,		"SL limit",				"SL limit",				0	},
	{	E_MOTOR_TEMP1,				105,		"M. overtemp",		"M. overtemp",		0	},
	{	E_MOTOR_TEMP2,				106,		"M. overtemp",		"M. overtemp",		0	},
	{	E_ROOM_TEMP_MIN,			107,		"Room temp",			"Room temp",			0	},
	{	E_ROOM_TEMP_MAX,		108,		"Room temp",			"Room temp",			0	},
	{	E_INTERNAL_SW,				109,		"Int.SW err.",		"Int.SW err.",		1	},
	{	E_SW_START,						110,		"Lif.Boot up",		"Lif.Boot up",		0	},
	{	E_BG_START,						111,		"Restart",				"Restart",				1	},
	{	E_GROUPNUMBER,				112,		"Mult. error",		"Mult. error",		0	},
	{ E_ILS_ERROR,					113,		"ILS error", 			"ILS error", 			0 },
	{ E_KH5_ON, 						114,		"SRU ZoneON", 		"SRU ZoneON", 		0 },
	{ E_KH5_OFF,						115,		"SRU ZoneOff",		"SRU ZoneOff",		0 },
	{ E_RELEV_SHORT,				116,		"Relev.short",		"Relev.short",		0 },
	{ E_RELEV_LONG, 				117,		"Relev.long" ,		"Relev.long", 		0 },
	{ E_LIGHT_VOLTAGE,			118,		"CarLightoff",		"CarLightoff",		0 },
	{ E_TRIP_TIME,						119,		"TripTimeout",		"TripTimeout",		0 },
	{ E_LIGHT_SCREEN, 			120,		"Callmisuse", 		"Callmisuse", 		0 },
	{ E_ALARM_CALL, 				121,		"Alarm call", 		"Alarm call", 		0 },
	{ E_VO_VU_ERROR,				122,		"Error ULCM",			"Error ULCM",			0 },
	{ E_DIVING_ERR, 				123,		"Diving err.", 		"Diving err.", 		0 },
	{ E_BASIC_SETTING,			124,		"Bas.setting",		"Bas setting",		0 },
	{ E_EMOP_BTN_STICK, 		125,		"EMOP. stick",		"EMOP. stick",		0 },
	{ E_DL_SHORTED_CAR, 		126,		"DL short", 			"DL short", 			0 },
	{ E_RTCLOWPOWER,			127,		"Battery low",		"Battery low",		0 },
	{ E_INSP_STICK, 					128,		"Ins.overtime",		"Ins.overtime",		0 },
	{ E_RESET_TRIP, 					129,		"Refer.Run",			"Refer.Run",			0 },
	{ E_IV_ERROR, 					0,			"Drive error",		"Drive error",		0 },
	{ E_HEARTBEAT,					130,		"No connect.",		"No connect.",		1 },
	{ E_HEARTBEAT_CDU,			131,		"No CDU conn.", 	"No CDU conn.", 	0 },
	{ E_CAN_TSE,						132,		"No COP conn.",		"No COP conn.",		0 },
	{ E_MSI_NOTOFF,					133,		"MSI not off",		"MSI not off",		0 },
	{ E_HEARTBEAT_DRV,			134,		"No DRV conn.",		"No conn.DRV",		0 },
	{ E_UCMP_LOCK,					135,		"UCMP lock",			"UCMP lock",			0 },
	{ E_HEARTBEAT_EXE,			136,		"No connect.",		"No connect.",		0 },
	{ E_BRAKE_CHECK,				137,		"Bke.CK.err",			"Bke.CK.err",			0 },
	
	{ E_DOOR_CLOSING_1, 		140,		"D1 CL err.", 		"D1 CL err.", 		0 },
	{ E_DOOR_OPENING_1, 		141,		"D1 OP err.", 		"D1 OP err.", 		0 },
	{ E_DOOR_REV_1, 				142,		"D1 ROP err.", 		"D1 ROP err.", 		0 },
	{ E_DOOR_OP_SW_1, 			143,		"D1 open.sw", 		"D1 open.sw", 		0 },
	{ E_DOOR_CL_SW_1, 			144,		"D1 cls. sw", 		"D1 cls. sw", 		0 },
	{ E_DOOR_NUDGING_1, 		145,		"D1 nudging", 		"D1 nudging",			0 },
	{ E_DOOR_LS_1,					146,		"D1 screen",			"D1 screen",			0 },
	{ E_DOOR_CLOSING_2, 		150,		"D2 CL err.",			"D2 CL err.", 		0 },
	{ E_DOOR_OPENING_2, 		151,		"D2 OP err.",		 	"D2 OP err.",		 	0 },
	{ E_DOOR_REV_2, 				152,		"D2 ROP err.", 		"D2 ROP err.",		0 },
	{ E_DOOR_OP_SW_2, 			153,		"D2 open.sw", 		"D2 open.sw", 		0 },
	{ E_DOOR_CL_SW_2, 			154,		"D2 cls. sw", 		"D2 cls. sw", 		0 },
	{ E_DOOR_NUDGING_2, 		155,		"D2 nudging", 		"D2 nudging",			0 },
	{ E_DOOR_LS_2,					156,		"D2 screen", 			"D2 screen", 			0 },
	
	{	E_RUNCONTACT_NOTON,	160,		"RunCont.err",	"RunCont.err",	0	},
	{	E_RUNCONTACT_NOTOFF,161,		"RunCont.err",	"RunCont.err",	0	},
	{	E_BRKCONTACT_NOTON,	162,		"BrkCont.err",	"BrkCont.err",	0	},
	{	E_BRKCONTACT_NOTOFF,163,		"BrkCont.err",	"BrkCont.err",	0	},
	{	E_FXCONTACT_NOTON,	164,		"FXCont. err",	"FXCont. err",	0	},
	{	E_FXCONTACT_NOTOFF,	165,		"FXCont. err",	"FXCont. err",	0	},
	{	E_BRAKE_NOTON,				166,		"Brk not op.",	"Brk not op.",	0	},
	{	E_BRAKE_NOTOFF,			167,		"Brk not cl.",	"Brk not cl.",	0	},
	{	E_NICE_RUN_NOTOFF, 	168,		"RunSig. err",	"RunSig. err",	0	},
	{	E_NICE_RUN_NOTON, 		169,		"RunSig. err",	"RunSig. err",	0	},
	{	E_NICE_BRAKE_NOTOFF, 170,		"BrkSig. err",	"BrkSig. err",	0	},
	{	E_NICE_BRAKE_NOTON, 	171,		"BrkSig. err",	"BrkSig. err",	0	},
	{	E_NICE_MOVE_NOTOFF, 	172,		"MovSig. err",	"MovSig. err",	0	},
	{	E_NICE_MOVE_NOTON, 	173,		"MovSig. err",	"MovSig. err",	0	},
	{ E_NICE_DIR_NOTON, 		174,		"DirSig. err", 	"DirSig. err", 	0 },
	{ E_NICE_OVER_SPEED,		175,		"Over speed", 	"Over speed", 		0 },

	{ E_STEEL_BROKE,				180,		"Steel broke", 	"Steel broke", 		0 },

	{	E_CAN_OVERRUN_A,			200,		"CANA RX HW.",		"CANA RX HW.",		1	},
	{	E_CAN_OV_SW_A,				201,		"CANA RX SW.",		"CANA RX SW.",		1	},
	{	E_BUS_OFF_A,					202,		"CANA busoff",		"CANA busoff",		1	},
	{	E_CAN_TX_A,						203,		"CANA TX SW2",		"CANA TX SW2",		1	},
	{	E_CAN_TX_SW_A,				204,		"CANA TX SW1",		"CANA TX SW1",		1	},
	{	E_CAN_SX_SW_A,				205,		"CANA TX SW3",		"CANA TX SW3",		1	},
	{	E_CAN_PASSIVE_A,			206,		"CANA faulty",		"CANA faulty",		1	},

	{	E_CAN_OVERRUN_B,			210,		"CANB RX HW.",		"CANB RX err",		1	},
	{	E_CAN_OV_SW_B,				211,		"CANB RX SW.",		"CANB RX err",		1	},
	{	E_BUS_OFF_B,					212,		"CANB busoff",		"CANB busoff",		1	},
	{	E_CAN_TX_B,						213,		"CANB TX SW2",		"CANB TX err",		1	},
	{	E_CAN_TX_SW_B,				214,		"CANB TX SW1",		"CANB TX err",		1	},
	{	E_CAN_SX_SW_B,				215,		"CANB TX SW3",		"CANB TX err",		1	},
	{ E_CAN_PASSIVE_B,			216,		"CANB faulty",		"CANA TX err",		1 },

	{	E_CAN_OVERRUN_C,			220,		"CANC RX HW.",		"CANC RX HW.",		1	},
	{	E_CAN_OV_SW_C,				221,		"CANC RX SW.",		"CANC RX SW.",		1	},
	{	E_BUS_OFF_C,					222,		"CANC busoff",		"CANC busoff",		1	},
	{	E_CAN_TX_C,						223,		"CANC TX SW2",		"CANC TX SW2",		1	},
	{	E_CAN_TX_SW_C,				224,		"CANC TX SW1",		"CANC TX SW1",		1	},
	{	E_CAN_SX_SW_C,				225,		"CANC TX SW3",		"CANC TX SW3",		1	},
	{ E_CAN_PASSIVE_C,			226,		"CANC faulty",		"CANC faulty",		1 },

	{	ENDSTRING,						255,		"Unknown err",		"Unknown err",		1	},
};


//**************************************************************************************************
// PCb specific error text
//**************************************************************************************************
const struct _errortext errortext_bg[MAX_PCB_ERROR + 1] = {
	{	E_INTERNAL_SW			,	109,		"Int.SW err."		,"Int.SW err."		,1	},	// internal software error (watchdog reset)
	{	E_BG_START				,	111,		"Restart"			,"Restart"			,1	},	// anormal software restart
	{	E_CAN_OVERRUN_A	,	200,	"CANA RX HW."		,"CANA RX HW."		,1	},	// CAN bus data overrun in hardware buffer (message lost)
	{	E_CAN_PASSIVE_A			,	206,		"CANA faulty"		,"CANA faulty"		,1	},	// CAN controller entered error passive state
	{	E_BUS_OFF_A				,	202,	"CANA bus off"		,"CANA bus off"		,1	},	// CAN bus off state
	{	E_HEARTBEAT				,	130,		"No CAN connect."	,"No CAN connect."	,1	},	// CAN bus off state
	{ E_HEARTBEAT_CDU	, 131,		"No CDU connect." ,"No CDU connect."	,1	},	// CAN bus off state
	{	ENDSTRING					,	255,		"Unknown err"		,"Unknown err"		,1	},
};

const struct _errordetail errordetail[] = {
	{	"Error no.",	"Error no.",		M_ERRORPARA,	&hist.hist[0].errorcode,				sizeof(hist.hist[0].errorcode),					0											},
	{	"Module",		"Module",			M_NODEIDPARA,	&hist.hist[0].idorpwr.node_id,			sizeof(hist.hist[0].idorpwr.node_id),			0											},
	{	"State",		"State",			M_LISTPARA,		&hist.hist[0].hse_state,				sizeof(hist.hist[0].hse_state),					statetext									},
	{	"Dir.",			"Dir.",				M_LISTPARA,		&hist.hist[0].direction,				sizeof(hist.hist[0].direction),					dirtext										},
	{	"Drivest.",		"Drivest.",			M_LISTPARA,		&hist.hist[0].drive_state,				sizeof(hist.hist[0].drive_state),				drivestate									},
	{	"Start fl.",	"Start fl.",		M_FLOORPARA,	&hist.hist[0].start_floor,				sizeof(hist.hist[0].start_floor),				0 											},
	{	"Target fl.",	"Target fl.",		M_FLOORPARA,	&hist.hist[0].target_floor,				sizeof(hist.hist[0].target_floor),				0 											},
	{	"Actual fl.",	"Actual fl.",		M_FLOORPARA,	&hist.hist[0].level,					sizeof(hist.hist[0].level),						0											},

	{	"Set speed",	"Set speed",		M_3BIT_DECIMAL,	&hist.hist[0].nice5000_Rfault_setspd,	sizeof(hist.hist[0].nice5000_Rfault_setspd),	0 											},
	{	"Act.speed",	"Act.speed",		M_3BIT_DECIMAL,	&hist.hist[0].nice5000_Rfault_feedspd,	sizeof(hist.hist[0].nice5000_Rfault_feedspd),	0 											},
	{	"StartPos", 	"StartPos", 		M_NUMBERPARA,	&hist.hist[0].nice5000_startpos, 		sizeof(hist.hist[0].nice5000_startpos),			0											},
	{	"ErrorPos", 	"ErrorPos", 		M_NUMBERPARA,	&hist.hist[0].nice5000_errorpos, 		sizeof(hist.hist[0].nice5000_errorpos),			0											},
	{	"DC voltage",	"DC voltage",		M_1BIT_DECIMAL,	&hist.hist[0].nice5000_Rfault_dcvoltage,sizeof(hist.hist[0].nice5000_Rfault_dcvoltage),	0 											},
	{	"Out voltage",	"Out voltage",		M_1BIT_DECIMAL,	&hist.hist[0].nice5000_Rfault_feedspd,	sizeof(hist.hist[0].nice5000_Rfault_feedspd),	0 											},
	{	"Out current",	"Out current",		M_2BIT_DECIMAL,	&hist.hist[0].nice5000_Rfault_outcurrent,sizeof(hist.hist[0].nice5000_Rfault_outcurrent),0											},
	{	"Out freq.",	"Out freq.",		M_2BIT_DECIMAL,	&hist.hist[0].nice5000_Rfault_outfreq,	sizeof(hist.hist[0].nice5000_Rfault_outfreq),	0 											},
	{	"Out torque",	"Out torque",		M_1BIT_DECIMAL,	&hist.hist[0].nice5000_Rfault_outtorque,sizeof(hist.hist[0].nice5000_Rfault_outtorque),	0											},
	{	"torque cur.",	"torque cur.",		M_2BIT_DECIMAL,	&hist.hist[0].nice5000_Rfault_tkcurrent,sizeof(hist.hist[0].nice5000_Rfault_tkcurrent),	0 											},
	{	"Out power",	"Out power",		M_2BIT_DECIMAL,	&hist.hist[0].nice5000_Rfault_outpwr,	sizeof(hist.hist[0].nice5000_Rfault_outpwr),	0 											},

	{	"Calls",		"Calls",			M_YES_NOPARA,	&hist.hist[0].calls_disabled,			sizeof(hist.hist[0].calls_disabled),			calls 										},
	{	"LC",			"LC",				M_YES_NOPARA,	&hist.hist[0].landingcalls_disabled,	sizeof(hist.hist[0].landingcalls_disabled),		calls 										},
	{	"SL 1-3",		"SL 1-3",			M_BITPOSPARA,	&hist.hist[0].safety_circuit,			sizeof(hist.hist[0].safety_circuit),			(const struct _textlist *)3					},
	{	"SS",			"SS",				M_SS_PARA,		&hist.hist[0].door_zone,				sizeof(hist.hist[0].door_zone), 				0											},
	{	"O. 1-6",		"O. 1-6",			M_BITPOSPARA,	&hist.hist[0].out,						sizeof(hist.hist[0].out),						(const struct _textlist *)MAX_HSE_OUT+MAX_HSE_EX_OUT		},
	{	"I. 1-8",		"I. 1-8",			M_BITPOSPARA,	&hist.hist[0].in1,						sizeof(hist.hist[0].in1),						(const struct _textlist *)8					},
	{	"I.9-16",		"I.9-16",			M_BITPOSPARA,	&hist.hist[0].in2,						sizeof(hist.hist[0].in2),						(const struct _textlist *)8					},
	{	"I.17-24",		"I.17-24",			M_BITPOSPARA,	&hist.hist[0].in3,						sizeof(hist.hist[0].in3),						(const struct _textlist *)8					},
	{	"Door 1-max",	"Door 1-max",		M_DOORNUMBERPARA,&hist.hist[0].doorstate,				sizeof(hist.hist[0].doorstate),					(const struct _textlist *)&p.doornumber		},
	{	"Dr1 O|C|R|L",	"Dr1 O|C|R|L",		M_BITPOSPARA,	&hist.hist[0].door_io[0],				sizeof(hist.hist[0].door_io[0]),				(const struct _textlist *)4					},
	{	"Dr2 O|C|R|L",	"Dr2 O|C|R|L",		M_BITPOSPARA,	&hist.hist[0].door_io[1],				sizeof(hist.hist[0].door_io[1]),				(const struct _textlist *)4					},
	{	"Load Z|H|F|O",	"Load Z|H|F|O",		M_BITPOSPARA,	&hist.hist[0].load,						sizeof(hist.hist[0].load),						(const struct _textlist *)4					},
	{	"Carlight",		"Carlight",			M_BITPARA,		&hist.hist[0].carlight,					sizeof(hist.hist[0].carlight),					off_on										},
	{	"O1|O2|R\6|R\5","O1|O2|R\6|R\5",	M_BITPOSPARA,	&hist.hist[0].temperature,				sizeof(hist.hist[0].temperature),				(const struct _textlist *)4					},
	{	"Err count",	"Err count",		M_NUMBERPARA,	&hist.hist[0].triperrorcount,			sizeof(hist.hist[0].triperrorcount),			0											},
	{ "Err subcode",	"Err subcode",		M_NUMBERPARA, &hist.hist[0].err_maskcode, 		sizeof(hist.hist[0].err_maskcode),			0 										},

	{	"-- ESC --",	"-- ESC --",		M_SHOW_HIST,	0,										0,												0											},
};


const struct _iopara iopara[] = {
//		menu						pointer													size														dir			array	checkmenu			start	stop	
	{	M_IN_HSE,		(BYTE *)&p.hse_inpar,					MAX_HSE_IN + MAX_HSE_EX_IN,		INPUT,		0,	0,					0,		0				},
	{	M_OUT_HSE,	(BYTE *)&p.hse_outpar,				MAX_HSE_OUT + MAX_HSE_EX_OUT,	OUTPUT,		0,	0,					0,		0				},
	{	M_IN_FVE,		(BYTE *)&p.fve_inpar,					MAX_FVE_IN + MAX_FVE_EX_IN,		INPUT,		0,	0,					0,		0				},
	{	M_OUT_FVE,	(BYTE *)&p.fve_outpar,				MAX_FVE_OUT,										OUTPUT,		0,	0,					0,		0				},
	{	M_IN_TSE,		(BYTE *)&p.tse_inpar,					MAX_TSE_IN,											INPUT,		1,	M_OUT_TSE,	0,		MAX_TSE_OUT - 1	},
	{	M_OUT_TSE,	(BYTE *)&p.tse_outpar,				MAX_TSE_OUT,										OUTPUT,		1,	M_IN_TSE,		0,		MAX_TSE_IN - 1	},
	{	M_IN_ESE,		(BYTE *)&p.ese_inpar,					MAX_ESE_IN,											INPUT,		1,	M_OUT_ESE,	0,		MAX_ESE_OUT - 1	},
	{	M_OUT_ESE,	(BYTE *)&p.ese_outpar,				MAX_ESE_OUT,										OUTPUT,		1,	M_IN_ESE,		0,		MAX_ESE_IN - 1	},
	{ M_IN_EXE, 	(BYTE *)&exe_para.exe_inpar, 	MAX_EXE_IN, 										INPUT,		1,	M_OUT_EXE,	0,		MAX_EXE_OUT - 1 },
	{ M_OUT_EXE,	(BYTE *)&exe_para.exe_outpar,	MAX_EXE_OUT,										OUTPUT, 	1,	M_IN_EXE, 	0,		MAX_EXE_IN - 1	},

	{	ENDSTRING,	0,						0,				0,			0,	0,			0,	0,				},
};

//**************************************************************************************************
// Table for all possible inputs and outputs
// Text english,		Text german,		inout/output function,	input/output,	short display,	bitwise,	textlist sub,	filter,		text list for each parameter detail
// Filter,what details to display (bitwise): ESC | Confirm | Polarity | Door | Floor | Lift | Sub | Basic
// BYTE  const *text[NO_OF_LANGUAGE];		WORD  number;			BYTE  io;		BYTE special;	void  *pointer;BYTE filter[2];struct _textlist  *detail;
//**************************************************************************************************
const struct _iotype iotype[] = {
//		text[0] 					text[1] 					number											io								special 			pointer 						filter		detial					sign[0] 	sign[1]
	{	"--",						"--",						0,										INPUT | OUTPUT,	S_STANDARD,		0,									0xE1,	paradetail,			'-',	'-',	},
	{	"Land.call",	"Land.call",	HALL_CALL,				INPUT | OUTPUT,	S_HALL_CALL,	(void *)dirtext,		0xFB,	hallcalldetail,	'a',	'a',	},
	{	"Special LC",	"Special LC",	HALL_CALL_SPECIAL,		INPUT | OUTPUT,	S_FUNC_SUB,		(void *)dirtext,		0xF3,	carcalldetail,	's',	's',	},
	{	"VIP LC",		"VIP LC",		HALL_CALL_ADVANCED,		INPUT | OUTPUT,	S_FUNC_SUB,		(void *)dirtext,		0xF3,	carcalldetail,	'v',	'v',	},
	{	"Emerg.LC",		"Emerg.LC",		HALL_CALL_EMERGENCY,	INPUT | OUTPUT,	S_FUNC_SUB,		(void *)dirtext,		0xF3,	carcalldetail,	'n',	'n',	},
	{	"Car call",		"Car call",		CAR_CALL,				INPUT | OUTPUT,	S_FUNC_SUB,		0,						0xF3,	carcalldetail,	'i',	'i',	},
	{	"Spec.funct.",	"Spec.funct.",	SPECIAL_FUNC,			INPUT,			S_SUBTABLE,		(void *)special_func,	0xE3,	paradetail,		'-',	'-',	},
	{	"Spec.out.",	"Spec.out.",	SPECIAL_FUNC,			OUTPUT,			S_SUBTABLE,		(void *)special_out,	0xE3,	paradetail,		'-',	'-',	},
	{	"Disable CC",	"Disable CC",	DISABLE_CARCALL,		INPUT,			S_FUNC_SUB,		0,						0xF3,	carcalldetail,	'm',	'm',	},
	{	"Enable CC",	"Enable CC",	ENABLE_CARCALL,			INPUT,			S_FUNC_SUB,		0,						0xF3,	carcalldetail,	'f',	'f',	},
	{	"Disable LC",	"Disable LC",	DISABLE_HALLCALL,		INPUT,			S_HALL_CALL,	(void *)dirtext,		0xF9,	hallcalldetail,	'm',	'm',	},
	{	"Enable LC",	"Enable LC",	ENABLE_HALLCALL,		INPUT,			S_HALL_CALL,	(void *)dirtext,		0xFB,	hallcalldetail,	'f',	'f',	},
	{	"Gong",			"Gong",			ARRIVAL_INDICATION,		OUTPUT,			S_FUNC_ARROW,	(void *)arrowtext,		0xFB,	hallcalldetail,	'g',	'g',	},
	{	"Inspect.",		"Inspect.",		INSPECT_IN,				INPUT,			S_SUBTABLE,		(void *)insp_dir_in,		0xE3,	paradetail,		'-',	'-',	},
	{	"Door test",	"Door test",	DOORTEST,				INPUT,			S_STANDARD,		0,						0xE1,	paradetail,		't',	't',	},
	{	"Pos.",			"Pos.",			POS_IN,					INPUT | OUTPUT,	S_SUBTABLE,		(void *)pos_in,			0xE3,	paradetail,		'-',	'-',	},
	{	"Car light",	"Car light",	CARLIGHT,				INPUT | OUTPUT,	S_STANDARD,		0,						0xE1,	paradetail,		'l',	'l',	},
	{	"Load",			"Load",			LOAD_IN,				INPUT,			S_SUBTABLE,		(void *)load_text,		0xE3,	paradetail,		'y',	'y',	},
	{	"Door input",	"Door input",	DOOR_IO,				INPUT,			S_SUBTABLE,		(void *)door_in,		0xF3,	paradetail,		'-',	'-',	},
	{	"Door output",	"Door output",	DOOR_IO,				OUTPUT,			S_SUBTABLE,		(void *)door_out,		0xF3,	paradetail,		'-',	'-',	},
	{	"Light screen",	"Light screen",	DOOR_LS,				INPUT,			S_STANDARD,		0,						0xF1,	paradetail,		'l',	'l',	},
	{	"Safety line",	"Safety line",	SAFETY_CIRCUIT,			INPUT,			S_SUBTABLE,		(void *)sc_text,		0xE3,	paradetail,		'-',	'-',	},
	{	"Emerg.light",	"Emerg.light",	EMERGENCY_LIGHT,		OUTPUT,			S_STANDARD,		0,						0xE1,	paradetail,		'l',	'l',	},
	{	"Alarm call",	"Alarm call",	ALARM_CALL,				INPUT,			S_STANDARD,		0,						0xE1,	paradetail,		'l',	'l',	},
	{	"Em.op.",		"Em.op.",		EM_OP_IN,				INPUT,			S_SUBTABLE,		(void *)em_op_dir,		0xE3,	paradetail,		'-',	'-',	},
	{	"Cont.sup",		"Cont.sup",		CONT_CHECK,				INPUT,			S_SUBTABLE,		(void *)contact_in,		0xE3,	drivedetail,	'-',	'-',	},
	{	"Drive",		"Drive",		DRIVE_OUT,				OUTPUT,			S_SUBTABLE,		(void *)drive_out,		0xE3,	drivedetail,	'-',	'-',	},
	{	"Brake sup.",	"Brake sup.",	BRAKE_CHECK,			INPUT,			S_SUBTABLE,		(void *)brake_in,		0xE3,	paradetail,		'-',	'-',	},
	{	"Temp.",		"Temp.",		TEMPERATURE,			INPUT,			S_SUBTABLE,		(void *)temp_text,		0xE3,	paradetail,		'-',	'-',	},
	{	"Conn.IO",		"Conn.IO",		CONNECTED_IO,			INPUT | OUTPUT,	S_FUNC_NUMBER,	0,						0xE9,	signaldetail,	'o',	'o',	},
	{	"Force switch",	"Force switch",	FORCE_SWITCH,	INPUT | OUTPUT,	S_SUBTABLE,		(void *)force_switch_in,0xE3,	paradetail,		'p',	'p',	},
	{ "Door funct.", "Door funct.", DOOR_FUNCTION, INPUT, S_SUBTABLE, 	(void *)door_function ,0xE3, paradetail, 	'd',	'd',	},

	{	"Buzzer",				"Buzzer",				SPEAKER_BUZ,					OUTPUT,					S_SUBTABLE,		(void *)gong_type,	0xE3,		paradetail,	'g',	'g',	},
	{	"Light func.",	"Light func.",	LIGHT_FUNC,						OUTPUT,					S_SUBTABLE,		(void *)light_type,		0xE3,	paradetail,			'l',	'l',	},

	{	"",				"",				ENDSTRING,				INPUT | OUTPUT,	0,				0,						0xE1,	paradetail,		'-',	'-',	},
};

const struct _specialtext insp_dir_in[] = {
//	{	"On",			"On",			INSP_ON,				0xE3,'i',	'i'	},
//	{	"Up",			"Up",			INSP_UP,				0xE3,'u',	'u'	},
//	{	"Down",			"Down",			INSP_DOWN,				0xE3,'d',	'd'	},
//	{	"Slow",			"Slow",			INSP_FAST,				0xE3,'f',	'f'	},

	{	"Top car on",		"Top car on",		INSP_TOPCAR_ON,				0xE3,'i',	'i'	},
	{	"Top car up",		"Top car up",		INSP_TOPCAR_UP,				0xE3,'u',	'u'	},
	{	"Top car dn",		"Top car dn",		INSP_TOPCAR_DOWN,		0xE3,'d',	'd'	},
	
	{ "Pit box on",		"Pit box on",		INSP_PITBOX_ON,				0xE3,'i', 'i' },
	{ "Pit box up",		"Pit box up",		INSP_PITBOX_UP,				0xE3,'u', 'u' },
	{ "Pit box dn",		"Pit box dn",		INSP_PITBOX_DOWN,			0xE3,'d', 'd' },
	
	{	"" ,					"",						ENDSTRING,									0xE3,'-',	'-'	},
};

const struct _specialtext access_op_dir[] = {
	{	"Access on",		"Access on",		INSP_ACCESS_ON,				0xE3,'a',	'a'	},
	{	"Access up",		"Access up",		INSP_ACCESS_UP,				0xFB,'u',	'u'	},
	{	"Access dn",		"Access dn",		INSP_ACCESS_DOWN,		0xFB,'d',	'd'	},
	{	"" ,			"",				ENDSTRING,				0xE3,'-',	'-'	},
};

const struct _specialtext em_op_dir[] = {
	{	"On",			"On",			INSP_ON,				0xE3,'r',	'r'	},
	{	"Up",			"Up",			INSP_UP,				0xE3,'u',	'u'	},
	{	"Down",			"Down",			INSP_DOWN,				0xE3,'d',	'd'	},
	{	"Fast",			"Fast",			INSP_FAST,				0xE3,'f',	'f'	},
	{	"" ,			"",				ENDSTRING,				0xE3,'-',	'-'	},
};


const struct _specialtext pos_in[] = {
	{	"MSI",			"MSI",			POS_SGM,				0xE3,'p',	'p'	},
	{	"USI",			"USI",			POS_SGU,				0xE3,'p',	'p'	},
	{	"LSI",			"LSI",			POS_SGD,				0xE3,'p',	'p'	},
	{	"UIL",			"UIL",			POS_ILU,				0xE3,'p',	'p' },
	{	"LIL",			"LIL",			POS_ILD,					0xE3,'p',	'p' },
	{ "LPLS",			"LPLS",			POS_LPLS,				0xE3,'p', 'p' },
	{ "UPLS",			"UPLS",			POS_UPLS,				0xE3,'p', 'p' },
	{	"" ,			"",				ENDSTRING,				0xE3,'-',	'-'	},
};

const struct _specialtext force_switch_in[] = {
	{	"SDU1",			"SDU1",			FORCE_SDU1,				0xE3,'p',	'p'	},
	{	"SDU2",			"SDU2",			FORCE_SDU2,				0xE3,'p',	'p'	},
	{	"SDU3",			"SDU3",			FORCE_SDU3,				0xE3,'p',	'p'	},
	{	"SDD1",			"SDD1",			FORCE_SDD1,				0xE3,'p',	'p' },
	{	"SDD2",			"SDD2",			FORCE_SDD2,				0xE3,'p',	'p' },
	{	"SDD3",			"SDD3",			FORCE_SDD3,				0xE3,'p',	'p' },
	{	"" ,			"",				ENDSTRING,				0xE3,'-',	'-'	},
};

const struct _specialtext door_function[] = {
	{	"Cardoor conn.",	"Cardoor conn.",	CARDOOR_CONNECT,		0xE3,	'd',	'd'	},
	{	"SHdoor conn.",		"SHdoor conn.",		SHAFTDOOR_CONNECT,	0xE3,	'd',	'd'	},
	{ "DPM signal", 		"DPM signal", 		DOOR_POSIOTION_MONITOR,	0xE3, 'd',	'd' },
	{ "Photo sensor", 	"Photo sensor", 	PHOTO_SENSOR,				0xFB, 's', 's' },
	{	"" ,			"",					ENDSTRING,				0xE3,	'-',	'-'	},
};


const struct _specialtext special_func[] = {
	{ "Car fan",		"Car fan",			FAN_1,					0xE3, 'j',	'j' },
	{ "Special run",	"Special run",		KEY_SPECIAL,			0xE3, 's',	's' },
	{ "VIP run",		"VIP run",			KEY_ADVANCED, 		0xE3, 'v',	'v' },
	{ "Emerg. run", "Emerg. run", 	KEY_EMERGENCY,			0xE3, 'n',	'n' },
	{ "Lift off", 	"Lift off", 		KEY_OFF,				0xE3, 'x',	'x' },
	{ "Lift off disp","Lift off disp",	OUT_OF_ORDER, 		0xE3, 'x',	'x' },
	{ "Door op.push", "Door op.push", 	DOOR_OPEN,				0xF3, 'o',	'o' },
	{ "Door cl.push", "Door cl.push", 	DOOR_CLOSE, 			0xF3, 'z',	'z' },
	{ "Dr.stop push", "Dr.stop push", 	DOOR_STOP,				0xF3, 'z',	'z' },
	{ "Firemen run",	"Firemen run",		FIRE_SERVICE_ENABLE,	0xE3, 'f',	'f' },
	{ "Fire call",	"Fire call",		FIRE_SERVICE, 		0xFB, 'f',	'f' },
	{ "Fire evac.", "Fire evac.", 	FIRE_EVACUATION,		0xE3, 'f',	'f' },
	{ "Fire alarm", "Fire alarm", 	FIRE_ALARM, 			0xEB, 'f',	'f' },
	{ "Fire return",	"Fire return",		FIRE_ALARM_RETURN,		0xE3, 'f',	'f' },
	{ "Fire bypass",	"Fire bypass",		FIRE_BYPASS,		0xE3, 'f',	'f' },
	{ "Clear calls",	"Clear calls",		CLEAR_CALLS,			0xE3, 'x',	'x' },
	{ "LC off", 		"LC off", 			HALL_CALL_DISABLE,		0xE3, 's',	's' },
	{ "Remote off", "Remote off", 	REMOTE_OFF, 			0xFB, 'x',	'x' },
	{ "Interm.door",	"Interm.door",		INTERMEDIATE_DOOR,		0xE3, 't',	't' },
	{ "Clock run 1",	"Clock run 1",		TIMER1_TRIP,			0xE3, 'u',	'u' },
	{ "Clock run 2",	"Clock run 2",		TIMER2_TRIP,			0xE3, 'u',	'u' },
	{ "EMP.evac.",	"EMP evac.",		EMP_POWER,					0xFB, 'e',	'e' },
	{ "UPS evac.",	"UPS evac.",		UPS_EVACUATION, 		0xE3, 'e',	'e' },
	{ "Evac.active",	"Evac.active",		EVACUATION_ACTIVE, 			0xFB, 'e',	'e' },
	{ "Evac.enable",	"Evac.enable",		EVACUATION_READY, 	0xE3, 'e',	'e' },
	{ "Evac.normal",	"Evac.normal",		EVACUATION_NORMAL,		0xE3, 'e',	'e' },
	{ "Earthquake", "Earthquake", 	EARTHQUAKE, 			0xE3, 'f',	'f' },
	{ "Car attend.",	"Car attend.",		CAR_ATTENDANCE, 		0xE3, 's',	's' },
	{ "Call dir.up",	"Call dir.up",		CALLDIR_UP, 			0xE3, 's',	's' },
	{ "Call dir.down","Call dir.down",	CALLDIR_DN, 			0xE3, 's',	's' },
	{ "Call bypass",	"Call bypass",		CALL_BYPASS,			0xE3, 's',	's' },
	{ "LC bypass",	"LC bypass",		HALLCALL_BYPASS,		0xE3, 's',	's' },
	{ "Auto-call",	"Auto-call",		AUTO_CALL,				0xE3, 's',	's' },
	{ "Up peak",		"Up peak",			UP_PEAK_TRAFFIC,		0xE3, 's',	's' },
	{ "Dn peak",		"Dn peak",			DN_PEAK_TRAFFIC,		0xE3, 's',	's' },
	{ "Light switch", "Light switch", 	LIGHT_SWITCH, 		0xE3, 'l',	'l' },
	{ "Fan switch", "Fan switch", 	FAN_SWITCH, 			0xE3, 'f',	'f' },
	{ "Pitbox reset", "Pitbox reset", 	RESET_PITBOX, 			0xE3, 'r',	'r' },
//	{ "Close limit", "Close limit", 	DOOR_CLOSE_LIMIT, 		0xF3, 'r',	'r' },
	{ "" ,			"", 				ENDSTRING,				0xE3, '-',	'-' },

};

const struct _specialtext special_out[] = {
	{	"Car fan",		"Car fan",			FAN_1,					0xE3,	'j',	'j'	},
	{	"Special mode",	"Special mode",		SPECIAL_MODE,			0xE3,	'k',	'k'	},
	{	"Special run",	"Special run",		KEY_SPECIAL,			0xE3,	'k',	'k'	},
	{	"VIP run",		"VIP run",			KEY_ADVANCED,			0xE3,	'k',	'k'	},
	{	"Emerg. run",	"Emerg. run",		KEY_EMERGENCY,			0xE3,	'k',	'k'	},
	{	"UPS evac.",	"UPS evac.",		UPS_EVACUATION,			0xE3,	'e',	'e'	},
	{	"UPS ready",	"UPS ready",		UPS_EVAC_READY,		0xE3,	'e',	'e'	},
	{	"Fire state",	"Fire state",		FIRE_STATE,				0xE3,	'k',	'k'	},
	{	"Fire service",	"Fire service",		FIRE_SERVICE_ENABLE,	0xE3,	'k',	'k'	},
	{	"Fire call",	"Fire call",		FIRE_SERVICE,			0xE3,	'k',	'k' },
	{	"Fire.floor",	"Fire.floor",		FIREFLOOR_IND,			0xE3,	'k',	'k' },
	{	"Fire alarm",	"Fire alarm",		FIRE_ALARM,				0xE3,	'k',	'k'	},
	{	"Evac.floor",	"Evac.floor",		FIRE_ALARM_FLOOR,		0xE3,	'k',	'k'	},
	{	"Out of work",	"Out of work",		OUT_OF_ORDER,			0xE3,	'k',	'k'	},
	{	"Fault signal",	"Fault signal",		FAULT_SIGNAL,			0xE3,	'k',	'k'	},
	{	"Buzzer",			"Buzzer",			BUZZER,					0xE3,	'k',	'k'	},
	{	"Remote off",	"Remote off",		REMOTE_OFF_STATE,		0xE3,	'k',	'k'	},
	{	"Switched off",	"Switched off",		REMOTE_OFF,				0xE3,	'k',	'k'	},
	{	"Overload",		"Overload",			OVER_LOAD_STATE,		0xE3,	'k',	'k'	},
	{	"Full load",	"Full load",		FULL_LOAD_STATE,		0xE3,	'k',	'k'	},
	{	"Occupied",		"Occupied",			OCCUPIED,				0xE3,	'k',	'k'	},
	{	"Door op.push",	"Door op.push",		DOOR_OPEN,				0xE3,	'o',	'o'	},
	{	"Door cl.push",	"Door cl.push",		DOOR_CLOSE,				0xE3,	'z',	'z'	},
	{	"Dr.stop push",	"Dr.stop push",		DOOR_STOP,				0xE3,	'z',	'z'	},
	{	"Door closing",	"Door closing",		DOOR_IS_CLOSING,		0xE3,	'k',	'k'	},
	{	"Evac.ready",	"Evac.ready",		EVACUATION_READY,		0xE3,	'e',	'e'	},
	{	"LC bypass",	"LC bypass",		HALLCALL_BYPASS,		0xE3,	's',	's'	},
	{	"AntiFaint", 	"AntiFaint",		ANTI_FAINT_OUTPUT,		0xE3,	'k',	'k' },
	{ "Safety beam", "Safety beam", SAFETY_BEAM,			0xE3, 's',	's' },
	{ "UCMP lock",  "UCMP lock", 		UCMP_LOCK,			0xE3, 'u',	'u' },
	{ "Door alarm",	"Door alarm",		DOOR_CONN_ALARM,			0xE3, 'd',	'd' },
	{ "Steel brake",	"Steel brake",	STEEL_BRAKE,			0xE3, 't',	't' },
	{ "Pitbox reset", "Pitbox reset", RESET_PITBOX, 			0xE3, 'r',	'r' },
	{ "Alarm filter", "Alarm filter", ALARM_FILTER, 			0xE3, 'f',	'f' },
	{	"",				"",					ENDSTRING,				0xFB,	'-',	'-'	},
};

const struct _specialtext light_type[] = {
	{ "Fire.light", 	"Fire.light", 	FIRE_LIGHT, 				0xE3, 'l',	'l' },
	{ "Access.light",	"Access.light",	ACCESS_LIGHT,			0xE3, 'l',	'l' },
	{ "CDoor.light", 	"CDoor.light", 	CDOOR_LIGHT, 		0xE3, 'l',	'l' },
	{ "HDoor.light", 	"HDoor.light", 	HDOOR_LIGHT, 		0xE3, 'l',	'l' },
	{ "Hall lan.up", 	"Hall lan.up", 	HALL_LANTERN_UP, 0xFB, 'w',	'w',},
	{ "Hall lan.dn", 	"Hall lan.dn", 	HALL_LANTERN_DN,	0xFB, 'w',	'w',},
	{ "Dir.up",				"Dir.up",				DIRECTION_IND_UP,	0xE3, 'k',	'k',},
	{ "Dir.dn", 			"Dir.dn", 			DIRECTION_IND_DN, 0xE3, 'k',	'k',},

	{ "" ,						"", 						ENDSTRING,				0xE3, '-',	'-' },
};

const struct _specialtext gong_type[] = {
	{ "Up",					"Up",					BUZZER_UP,				0xE3, 'g',	'g' },
	{ "Down", 			"Down", 			BUZZER_DN, 			0xE3, 'g',	'g' },
	{ "Fire alarm",	"Fire alarm",	BUZZER_FIRE,			0xE3, 'b',	'b' },
	{ "Leveling", 	"Leveling", 	BUZZER_LEVELING,	0xE3, 'b',	'b' },
	{ "Normal fun", "Normal fun", BUZZER_NORMAL,	0xE3, 'b',	'b' },

	{ "" ,						"", 						ENDSTRING,				0xE3, '-',	'-' },
};

const struct _specialtext load_text[] = {
	{	"Zero load",	"Zero load",		ZERO_LOAD,				0xE3,	'y',	'y'	},
	{	"Half load",	"Half load",		HALF_LOAD,				0xE3,	'y',	'y'	},
	{	"Full load",	"Full load",		FULL_LOAD,				0xE3,	'y',	'y'	},
	{	"Overload",		"Overload",			OVERLOAD,				0xE3,	'y',	'y'	},
	{	"",				"",					ENDSTRING,				0xE3,	'-',	'-'	},
};

const struct _specialtext temp_text[] = {
	{	"Overtemp.1",	"Overtemp.1",		OVERTEMP1,				0xE3,	'q',	'q'	},
	{	"Overtemp.2",	"Overtemp.2",		OVERTEMP2,				0xE3,	'q',	'q'	},
	{	"Roomtemp.min",	"Roomtemp.min",		ROOMTEMP_MIN,			0xE3,	'q',	'q'	},
	{	"Roomtemp.max",	"Roomtemp.max",		ROOMTEMP_MAX,			0xE3,	'q',	'q'	},
	{	"",				"",					ENDSTRING,				0xE3,	'-',	'-'	},
};

const struct _specialtext contact_in[] = {
	{	"Run",			"Run",				D_RUNCONT_SUP,			0xE3,	'w',	'w'	},
	{	"FX",			"FX",				D_FXCONT_SUP,			0xE3,	'f',	'f'	},
	{	"Brake",		"Brake",			D_BRKCONT_SUP,			0xE3,	'b',	'b'	},
	{	"",				"",					ENDSTRING,				0xE3,	'-',	'-'	},
};

const struct _specialtext drive_out[] = {
	{	"Running cont",	"Running cont",		D_MAIN,					0xE3,	'l',	'l'	},
	{	"Brake cont.",	"Brake cont.",		D_BRAKE,				0xE3,	'b',	'b'	},
	{	"",				"",					ENDSTRING,				0xFB,	'-',	'-'	},
};

const struct _specialtext door_in[] = {
	{	"Door opened",	"Door opened",		DOOR_OP,				0xF3,	'b',	'b'	},
	{	"Door closed",	"Door closed",		DOOR_CL,				0xF3,	'b',	'b'	},
	{	"Door reverse",	"Door reverse",		DOOR_REV,				0xF3,	'r',	'r'	},
	{ "Door C.limit", "Door C.limit", 	DOOR_CL_LIMIT,	0xF3, 'b',	'b' },
	{	"",				"",					ENDSTRING,				0xF3,	'-',	'-'	},
};

const struct _specialtext door_out[] = {
	{	"Door opening",	"Door opening",		DOOR_OP,				0xF3,	'o',	'o'	},
	{	"Door closing",	"Door closing",		DOOR_CL,				0xF3,	'c',	'c'	},
	{	"Door nudging",	"Door nudging",		DOOR_REV,				0xF3,	'z',	'z'	},
	{	"RetiringRamp",	"RetiringRamp",		DOOR_RAMP,				0xF3,	'r',	'r'	},
	{ "Close.slow", 	"Close.slow", 		DOOR_CL_SLOW,		0xF3, 'c',	'c' },
	{	"",				"",					ENDSTRING,				0xF3,	'-',	'-'	},
};

const struct _specialtext brake_in[] = {
	{	"Brake 1",		"Brake 1",			BRAKE1,					0xE3,	'r',	'r'	},
	{	"Brake 2",		"Brake 2",			BRAKE2,					0xE3,	'r',	'r'	},
	{	"Brake 3",		"Brake 3",			BRAKE3,					0xE3,	'r',	'r'	},
	{	"",				"",					ENDSTRING,				0xE3,	'-',	'-'	},
};

const struct _textlist dirtext[] = {
	{	"upwards",		"upwards",		},
	{	"downwards",	"downwards",	},
	{	"up+down",		"up+down",		},
	{	"",				""				},
};

const struct _textlist fanmode[] = {
	{	"manual",		"manual"		},
	{	"autom.",		"autom."		},
	{	"",				""				},
};

const struct _textlist paradetail[] = {
	{	"Fct",			"Fct",			},
	{	"Sub",			"Sub",			},
	{	"Enable",		"Enable",		},
	{	"Floor",		"Floor",		},
	{	"Door",			"Door",			},
	{	"Logic",		"Logic",		},
	{	"- Confirm -",	"- Confirm -",	},
	{	"- ESC -",		"- ESC -",		},
};

const struct _textlist signaldetail[] = {
	{	"Fct",			"Fct",			},
	{	"Sub",			"Sub",			},
	{	"Enable",		"Enable",		},
	{	"Signal no.",	"Signal no.",	},
	{	"Door",			"Door",			},
	{	"Logic",		"Logic",		},
	{	"- Confirm -",	"- Confirm -",	},
	{	"- ESC -",		"- ESC -",		},
};

const struct _textlist drivedetail[] = {
	{	"Fct",			"Fct",			},
	{	"Sub",			"Sub",			},
	{	"Enable",		"Enable",		},
	{	"Dir.",			"Dir.",			},
	{	"Door",			"Door",			},
	{	"Logic",		"Logic",		},
	{	"- Confirm -",	"- Confirm -",	},
	{	"- ESC -",		"- ESC -",		},
};

const struct _textlist hallcalldetail[] = {
	{	"Fct",			"Fct",			},
	{	"Dir.",			"Dir.",			},
	{	"Enable",		"Enable",		},
	{	"Floor",		"Floor",		},
	{	"Door",			"Door",			},
	{	"Logic",		"Logic",		},
	{	"- Confirm -",	"- Confirm -",	},
	{	"- ESC -",		"- ESC -",		},
};

const struct _textlist carcalldetail[] = {
	{	"Fct",			"Fct",			},
	{	"Floor",		"Floor",		},
	{	"Enable",		"Enable",		},
	{	"",				"",				},
	{	"Door",			"Door",			},
	{	"Logic",		"Logic",		},
	{	"- Confirm -",	"- Confirm -",	},
	{	"- ESC -",		"- ESC -",		},
};

const struct _textlist enable_io[] = {
	{	"no",			"no"			},
	{	"yes",			"yes"			},
};

const struct _textlist polarity[] = {
	{	"normal",		"normal"		},
	{	"inverse",		"invers"		},
};

const struct _textlist arrowtext[] = {
	{	"Up",			"Up"			},
	{	"Down",		"Down"			},
	{	"",				""				},
};

const struct _specialtext sc_text[] = {
	{	"SL1(Primary)",	"SL1(Primary)",		SC1,			0xE3,	's',	's'	},
	{ "SL2(1stdoor)", "SL2(1stdoor)", 	SC2,			0xE3, 's',	's' },
	{ "SL3(2nddoor)", "SL3(2nddoor)", 	SC3,			0xE3, 's',	's' },
	{	"",				"",					ENDSTRING,		0xE3,	'-',	'-'	},
};

const struct _textlist controltype[] = {
	{	"Collecting",	"Collecting",	},
	{	"Single call","Single call",	},
	{	"",				""				},
};

//**************************************************************************************************
// Parameter text
//		text[ENGLISH],text[DEUTSCH],menu,paratype,pointer,size,array,minpointer,tablepointer,min,max,default
//**************************************************************************************************
const struct _paratext paratext[] = {
//		text[0]				text[1]					menu							paratype						*pointer										size										array	minpointer	tablepointer		min	max				basic	index							subindex	object		datatype				access		right
	{	"PW-00",		"Code",				M_CHANGE_PW0,	M_NUMBERPARA,		&password_maintain,		sizeof(password_maintain),	0,		0,			0,					0,	0xFFFFFFFE,	0,	PASSWORD_MAINTAIN,	0,		D_VAR,	D_UNSIGNED32,	D_RW,RIGHT_MAINTAIN	},
	{ "PW-01",		"Code", 			M_CHANGE_PW1, M_NUMBERPARA, 	&password_install,			sizeof(password_install), 	0,		0,			0,					0,	0xFFFFFFFE, 0,	PASSWORD_INSTALL,	0,		D_VAR,	D_UNSIGNED32,	D_RW,RIGHT_INSTALL },
	{ "PW-02",		"Code", 			M_CHANGE_PW2, M_NUMBERPARA, 	&password_help, 				sizeof(password_help),			0,		0,			0,					0,	0xFFFFFFFE, 0,	PASSWORD_HELP,			0,		D_VAR,	D_UNSIGNED32,	D_RW,RIGHT_HELP		},
	{	"PW-03",		"Code",				M_CHANGE_PW3,	M_NUMBERPARA,		&password_admin,			sizeof(password_admin),			0,		0,			0,					0,	0xFFFFFFFE,	0,	PASSWORD_ADMIN,		0,		D_VAR,	D_UNSIGNED32,	D_RW,RIGHT_USER},

	{	"P0-00",		"Bottom floor",		M_GEN_PARA,		M_FLOORPARA,		&p.bot_floor,				sizeof(p.bot_floor),				0,	0,	0,	0,MAX_FLOOR-2,	0,BOT_FLOOR,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_INSTALL	},
	{	"P0-01",		"Top floor",		M_GEN_PARA,		M_FLOORPARA,		&p.top_floor,					sizeof(p.top_floor),				0,	0,	0,	1,MAX_FLOOR-1,	7,TOP_FLOOR,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_INSTALL	},
	{	"P0-02",		"Car door num.",	M_GEN_PARA,		M_NUMBERPARA,		&p.doornumber,					sizeof(p.doornumber),					0,	0,	0,	1,MAX_DOOR,	1,DOORNUMBER,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_INSTALL	},
	{	"P0-03",		"PCU numbers",		M_GEN_PARA,		M_NUMBERPARA,		&p.tse_number,					sizeof(p.tse_number),					0,	0,	0,	1,MAX_TSE,	1,PCUNUMBERS,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_INSTALL	},
	{	"P0-04",		"PCU ext.nums",		M_GEN_PARA,		M_NUMBERPARA,		&p.tse_extnums,					sizeof(p.tse_extnums),					0,	0,	0,	0,MAX_TSE_EXT,	0,IOU_EXT_NUMS,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_INSTALL	},
	{	"P0-05",		"LCU numbers",		M_GEN_PARA,		M_NUMBERPARA,		&p.ese_number,					sizeof(p.ese_number),					0,	0,	0,	1,MAX_ESE,	8,LCUNUMBERS,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_INSTALL	},
	{	"P0-06",		"Group size",		M_GEN_PARA,		M_NUMBERPARA,		&p.groupsize,					sizeof(p.groupsize),					0,	0,	0,	1,MAX_GR,	1,GROUPSIZE,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_INSTALL	},
	{	"P0-07",		"Group number",		M_GEN_PARA,		M_NUMBERPARA,		&p.groupnumber,					sizeof(p.groupnumber),					0,	0,	&p.groupsize,	0,	0,	0,GROUPNUMBER,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_INSTALL	},
	{	"P0-08",		"Main floor",		M_GEN_PARA,		M_FLOORPARA,		&p.main_floor,					sizeof(p.main_floor),					0,	&p.bot_floor,	&p.top_floor,0xFFFFFFFF,	0,	1,MAIN_FLOOR,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{	"P0-09",		"Park floor",		M_GEN_PARA,		M_FLOORPARA,		&p.parkfloor,					sizeof(p.parkfloor),					0,	&p.bot_floor,	&p.top_floor,0xFFFFFFFF,	0,	1,PARKFLOOR,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{	"P0-10",		"Setup ready",		M_GEN_PARA,		M_NUMBERPARA,		&p.setup_ready,					sizeof(p.setup_ready),					0,	0,(void *)no_yes,	0,	1,	0,SETUP_READY,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_INSTALL	},
	{	"P0-11",		"Motor turned",		M_GEN_PARA,		M_NUMBERPARA,		&p.motor_tuned,					sizeof(p.motor_tuned),					0,	0,(void *)no_yes,	0,	1,	0,IV_READY,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_INSTALL	},
	{	"P0-12",		"Call type",		M_GEN_PARA,		M_NUMBERPARA,		&p.controltype,					sizeof(p.controltype),					0,	0,(void *)controltype,	0,	1,	0,CONTROLTYPE,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_INSTALL	},
	{	"P0-13",		"LC mode",			M_GEN_PARA,		M_NUMBERPARA,		&p.lcmode_actfloor,				sizeof(p.lcmode_actfloor),				0,	0,(void *)lcmode,	0,2,	0,LCMODE_ACTFLOOR,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN },
	{	"P0-14",		"Fan mode",			M_GEN_PARA,			M_NUMBERPARA,		&p.fan_mode,					sizeof(p.fan_mode),						0,	0,(void *)fanmode,	0,	255,1,FAN_MODE,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{	"P0-15",		"Load type",		M_GEN_PARA,		M_NUMBERPARA,		&p.loadtype,					sizeof(p.loadtype),						0,	0,(void *)loadtype,	0,5,	0,LOADTYPE,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN },
	{	"P0-16",		"Arrow type",		M_GEN_PARA, 	M_NUMBERPARA,		&p.arrowtype,					sizeof(p.arrowtype),					0,	0,(void *)arrowtype,	0,2,	0,ARROW_MODE,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_INSTALL	},
	{	"P0-18",		"FX active",		M_GEN_PARA, M_NUMBERPARA,		&p.fx_active,					sizeof(p.fx_active),					0,	0,	0,				0,		1,		1,		FX_ACTIVE, 			0,	D_VAR,	D_UNSIGNED8,	D_RW,	RIGHT_INSTALL },
	{	"P0-20",		"Vane length",		M_GEN_PARA,	M_NUMBERPARA,		&p.doorzoneswitchsize, 			sizeof(p.doorzoneswitchsize),			0,	0,	0,	50,	1000,268,DOORZONESWITCHSIZE,0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN	},
	{ "P0-22",		"EXE numbers",		M_GEN_PARA,		M_NUMBERPARA, 	&p.exe_number,					sizeof(p.exe_number), 				0,	0,	0,	0,MAX_EXE,	1,EXENUMBERS, 0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_INSTALL	},
	{ "P0-23",		"Max speed",		M_GEN_PARA,		M_NUMBERPARA, 	&p.max_speed,					sizeof(p.max_speed), 				0,	0,	0,	200,1000,	250,MAX_SPEED, 0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_INSTALL	},
	{ "P0-40",		"UCM speed",		M_GEN_PARA,		M_NUMBERPARA, 	&p.ucmp_speed,					sizeof(p.ucmp_speed), 				0,	0,	0,	200,1000,	300,NO_OBJECT, 0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_INSTALL	},


	{	"P1-00",		"Parktime[s]",		M_TIMERPARA,	M_NUMBERPARA,		&p.parkingtime,					sizeof(p.parkingtime),					0,	0,	0,	0,3600,300,PARKINGTIME,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN	},
	{	"P1-01",		"Triptime[s]",		M_TIMERPARA,	M_NUMBERPARA,		&p.triptime,					sizeof(p.triptime),						0,	0,	0,	0,100,45,TRIPTIME,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_INSTALL	},
	{	"P1-02",		"Carlight[s]",		M_TIMERPARA,	M_NUMBERPARA,		&p.carlighttimer,				sizeof(p.carlighttimer),				0,	0,	0,	0,3600,60,CARLIGHTTIMER,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN	},
	{	"P1-03",		"Waitt.CC[s]",		M_TIMERPARA,	M_NUMBERPARA,		&p.doorstaytime_cc,				sizeof(p.doorstaytime_cc),				0,	0,	0,	1,30,	3,DOORSTAYTIME_CC,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN	},
	{	"P1-04",		"Waitt.LC[s]",		M_TIMERPARA,	M_NUMBERPARA,		&p.doorstaytime_lc,				sizeof(p.doorstaytime_lc),				0,	0,	0,	1,30,	5,DOORSTAYTIME_LC,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN	},
	{	"P1-05",		"Waitt.nC[s]",		M_TIMERPARA,	M_NUMBERPARA,		&p.doorstaytime_nc,				sizeof(p.doorstaytime_nc),				0,	0,	0,	1,30,	10,DOORSTAYTIME_NC,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN	},
	{	"P1-06",		"Wt.CC Main[s]",	M_TIMERPARA,	M_NUMBERPARA,		&p.doorstaytime_main_cc,		sizeof(p.doorstaytime_main_cc),			0,	0,	0,	1,30,	3,DOORSTAYTIME_CC_MAIN,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN	},
	{	"P1-07",		"Wt.LC Main[s]",	M_TIMERPARA,	M_NUMBERPARA,		&p.doorstaytime_main_lc,		sizeof(p.doorstaytime_main_lc),			0,	0,	0,	1,30,	5,DOORSTAYTIME_LC_MAIN,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN	},
	{	"P1-08",		"Wt.nC Main[s]",	M_TIMERPARA,	M_NUMBERPARA,		&p.doorstaytime_main_nc,		sizeof(p.doorstaytime_main_nc),			0,	0,	0,	1,30,	10,DOORSTAYTIME_NC_MAIN,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN	},
	{	"P1-09",		"Doorstop[s]",		M_TIMERPARA,	M_NUMBERPARA,		&p.doorstoptime,				sizeof(p.doorstoptime),					0,	0,	0,	1,600,	300,DOORSTOPTIME,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN	},
	{	"P1-10",		"Errdelay[s]",		M_TIMERPARA,	M_NUMBERPARA,		&p.errortime,					sizeof(p.errortime),					0,	0,	0,	0,30,	10,ERRORTIME,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_INSTALL	},
	{	"P1-11",		"Faint-Def[s]",		M_TIMERPARA,	M_NUMBERPARA,		&p.antifainttime,				sizeof(p.antifainttime),				0,	0,	0,	0,255,	0,ANTI_FAINT,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_INSTALL	},
	{ "P1-12",		"RstStop.dly",		M_TIMERPARA, M_NUMBERPARA, 	&p.rsttrip_stop_dly,			sizeof(p.rsttrip_stop_dly), 			0,	0,	0,	100,	50000,1000,RSTTRIP_STOP_DLY,0, D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN	},
	{ "P1-13",		"Fan times[s]",		M_TIMERPARA,	M_NUMBERPARA, 	&p.fan_time,					sizeof(p.fan_time), 					0,	0,	0,	0,300,60,FAN_TIME,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN	},
	{ "P1-16",		"UpsStop.dly", 	M_TIMERPARA,	M_NUMBERPARA, 		&p.upstrip_stop_delay,					sizeof(p.upstrip_stop_delay), 0,	0,	0,	0,50000,500,NO_OBJECT,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN	},

	{	"P2-00",		"Wt.Start[ms]",		M_DRIVE_COMMON,	M_NUMBERPARA,		&p.starttime,					sizeof(p.starttime),					0,	0,	0,				0,		25000,	300,	STARTTIME,			0,	D_VAR,	D_UNSIGNED16,	D_RW,	RIGHT_INSTALL },
	{	"P2-01",		"Contact.[ms]", 	M_DRIVE_COMMON, M_NUMBERPARA,		&p.wait_for_contactor,			sizeof(p.wait_for_contactor),			0,	0,	0,				0,		25000,	1000,	WAIT_FOR_CONTACTOR,	0,	D_VAR,	D_UNSIGNED16,	D_RW,	RIGHT_INSTALL },
	{	"P2-02",		"Ready=1[ms]",		M_DRIVE_COMMON, M_NUMBERPARA,		&p.max_ready_on,				sizeof(p.max_ready_on), 				0,	0,	0,				0,		25000,	2000,	MAX_READY_ON,		0,	D_VAR,	D_UNSIGNED16,	D_RW,	RIGHT_INSTALL },
	{	"P2-03",		"Brakedel[ms]",		M_DRIVE_COMMON,	M_NUMBERPARA,		&p.brake_on_delay,				sizeof(p.brake_on_delay),				0,	0,	0,				0,		25000,	600,	BRAKE_ON_DELAY,		0,	D_VAR,	D_UNSIGNED16,	D_RW,	RIGHT_INSTALL },
	{	"P2-04",		"Brake=1[ms]",		M_DRIVE_COMMON, M_NUMBERPARA,		&p.max_brake_on,				sizeof(p.max_brake_on), 				0,	0,	0,				0,		25000,	2000,	MAX_BRAKE_ON,		0,	D_VAR,	D_UNSIGNED16,	D_RW,	RIGHT_INSTALL },
	{	"P2-05",		"Mov. =1[ms]",		M_DRIVE_COMMON, M_NUMBERPARA,		&p.max_start,					sizeof(p.max_start),					0,	0,	0,				0,		25000,	1500,	MAX_START, 			0,	D_VAR,	D_UNSIGNED16,	D_RW,	RIGHT_INSTALL },
	{	"P2-06",		"Braket.[ms]",		M_DRIVE_COMMON, M_NUMBERPARA,		&p.wait_for_brake,				sizeof(p.wait_for_brake),				0,	0,	0,				0,		25000,	1000,	WAIT_FOR_BRAKE, 	0,	D_VAR,	D_UNSIGNED16,	D_RW,	RIGHT_INSTALL },
	{	"P2-07",		"Mov. =0[ms]",		M_DRIVE_COMMON, M_NUMBERPARA,		&p.max_stop,					sizeof(p.max_stop), 					0,	0,	0,				0,		25000,	2000,	MAX_STOP,			0,	D_VAR,	D_UNSIGNED16,	D_RW,	RIGHT_INSTALL },
	{	"P2-08",		"Brakeoff[ms]",		M_DRIVE_COMMON,	M_NUMBERPARA,		&p.brake_off_delay,				sizeof(p.brake_off_delay),				0,	0,	0,				0,		25000,	600,	BRAKE_OFF_DELAY,	0,	D_VAR,	D_UNSIGNED16,	D_RW,	RIGHT_INSTALL },
	{	"P2-09",		"Brake=0[ms]",		M_DRIVE_COMMON,	M_NUMBERPARA,		&p.max_brake_off,				sizeof(p.max_brake_off),				0,	0,	0,				0,		25000,	5000,	MAX_BRAKE_OFF,		0,	D_VAR,	D_UNSIGNED16,	D_RW,	RIGHT_INSTALL },
	{	"P2-10",		"Runoff[ms]", 		M_DRIVE_COMMON, M_NUMBERPARA,		&p.run_off_delay, 				sizeof(p.run_off_delay),				0,	0,	0,				0,		25000,	300,	RUN_OFF_DELAY,		0,	D_VAR,	D_UNSIGNED16,	D_RW,	RIGHT_INSTALL },
	{	"P2-11",		"Readyoff[ms]", 	M_DRIVE_COMMON, M_NUMBERPARA,		&p.ready_off_delay, 			sizeof(p.ready_off_delay),				0,	0,	0,				0,		25000,	300,	READY_OFF_DELAY,	0,	D_VAR,	D_UNSIGNED16,	D_RW,	RIGHT_INSTALL },
	{	"P2-12",		"Ready=0[ms]",		M_DRIVE_COMMON, M_NUMBERPARA,		&p.max_ready_off,				sizeof(p.max_ready_off),				0,	0,	0,				0,		25000,	5000,	MAX_READY_OFF, 		0,	D_VAR,	D_UNSIGNED16,	D_RW,	RIGHT_INSTALL },
	{	"P2-14",		"FX ontime",		M_DRIVE_COMMON, M_NUMBERPARA,		&p.fx_ontime,					sizeof(p.fx_ontime),					0,	0,	0,				500,	25000,	1000,	FX_ONTIME, 			0,	D_VAR,	D_UNSIGNED16,	D_RW,	RIGHT_INSTALL },
	{	"P2-15",		"FX offtime",		M_DRIVE_COMMON, M_NUMBERPARA,		&p.fx_offtime,					sizeof(p.fx_offtime),					0,	0,	0,				500,	25000,	1000,	FX_OFFTIME,			0,	D_VAR,	D_UNSIGNED16,	D_RW,	RIGHT_INSTALL },
	{	"P2-16",		"BrkFdbck[ms]", 	M_DRIVE_COMMON, M_NUMBERPARA,		&p.brake_feedback_dly,			sizeof(p.brake_feedback_dly),			0,	0,	0,				0,		25000,	600,	BRAKE_FEEDBACK_DLY, 0,	D_VAR,	D_UNSIGNED16,	D_RW,	RIGHT_INSTALL },
	{	"P2-17",		"BrakeRel[ms]", 	M_DRIVE_COMMON, M_NUMBERPARA,		&p.brake_off_sig_dly,			sizeof(p.brake_off_sig_dly),			0,	0,	0,				0,		25000,	800,	BRAKE_RELEASE_DLY,	0,	D_VAR,	D_UNSIGNED16,	D_RW,	RIGHT_INSTALL },

	{	"P5-00",		"Day",				M_TIME,			M_NUMBERPARA,		&t_set.day,						sizeof(t_set.day),						0,	0,	0,				1,		31,		1,		SETDATE_DAY,		0,	D_VAR,	D_UNSIGNED8,	D_RW,	RIGHT_MAINTAIN },
	{	"P5-01",		"Month",			M_TIME,			M_NUMBERPARA,		&t_set.month,					sizeof(t_set.month),					0,	0,	(void *)month,	1,		12,		1,		SETDATE_MONTH,		0,	D_VAR,	D_UNSIGNED8,	D_RW,	RIGHT_MAINTAIN },
	{	"P5-02",		"Year",				M_TIME,			M_NUMBERPARA,		&t_set.year,					sizeof(t_set.year),						0,	0,	0,				2000,	2099,	2013,	SETDATE_YEAR,		0,	D_VAR,	D_UNSIGNED16,	D_RW,	RIGHT_MAINTAIN },
	{	"P5-03",		"Hour",				M_TIME,			M_NUMBERPARA,		&t_set.hour,					sizeof(t_set.hour),						0,	0,	0,				0,		23,		0,		SETTIME_HOUR,		0,	D_VAR,	D_UNSIGNED8,	D_RW,	RIGHT_MAINTAIN },
	{	"P5-04",		"Minute",			M_TIME,			M_NUMBERPARA,		&t_set.minute,					sizeof(t_set.minute),					0,	0,	0,				0,		59,		0,		SETTIME_MINUTE,		0,	D_VAR,	D_UNSIGNED8,	D_RW,	RIGHT_MAINTAIN },
	{	"P5-05",		"Second",			M_TIME,			M_NUMBERPARA,		&t_set.second,					sizeof(t_set.second),					0,	0,	0,				0,		59,		0,		SETTIME_SECOND,		0,	D_VAR,	D_UNSIGNED8,	D_RW,	RIGHT_MAINTAIN },
	{	"P5-06",		"Weekday",		M_TIME,			M_NUMBERPARA,		&t_set.weekday,					sizeof(t_set.weekday),					0,	0,	(void *)weekday,1,		7,		1,		SETDATE_WEEK,		0,	D_VAR,	D_UNSIGNED8,	D_RW,	RIGHT_MAINTAIN },
	{	"P5-07",		"Start clock",M_TIME,			M_NUMBERPARA,		&settimeflag,					sizeof(settimeflag),					0,	0,	0,				0,		1,		0,		START_TIME,			0,	D_VAR,	D_UNSIGNED8,	D_RW,	RIGHT_MAINTAIN },

	{	"P8-",			"Floor",			M_SIGN,			M_SIGNPARA,			&p.sign[0],						sizeof(p.sign[0]),						1,	0,	0,	0,	0xFFFF,0x30,FLOORSIGN,MAX_FLOOR,D_ARRAY,D_UNSIGNED16,D_RW,RIGHT_MAINTAIN	},

	{	"P9-00",		"Adv.door op.",		M_RELEPARA,		M_NUMBERPARA,		&p.adv_door_op,					sizeof(p.adv_door_op),					0,	0,(void *)no_yes,	0,	1,	0,ADV_DOOR_OP,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_INSTALL	},
	{	"P9-01",		"v Adv.door",		M_RELEPARA,		M_NUMBERPARA,		&p.arrival_speed,				sizeof(p.arrival_speed),				0,	0,	0,	0,300,100,ARRIVAL_SPEED,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_INSTALL	},
	{	"P9-02",		"Pos.Adv.door",		M_RELEPARA,		M_NUMBERPARA,		&p.pos_dooropen,				sizeof(p.pos_dooropen),					0,	0,	0,	0,350,100,POS_DOOROPEN,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_INSTALL	},
	{	"P9-03",		"Releveling",		M_RELEPARA,		M_NUMBERPARA,		&p.relevel,						sizeof(p.relevel),						0,	0,(void *)no_yes,	0,	1,	0,RELEVEL,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_INSTALL	},
	{	"P9-04",		"Rel.at Call",		M_RELEPARA,		M_NUMBERPARA,		&p.relevel_trip,				sizeof(p.relevel_trip),					0,	0,(void *)no_yes,	0,	1,	0,RELEVEL_TRIP,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_INSTALL	},
	{ "P9-05",		"UCMP enable", 	M_RELEPARA, M_NUMBERPARA, 		&p.ucmp_enable,					sizeof(p.ucmp_enable), 0,	0,	0,	0,	1,	0,UCMP_CHECK_MODE , 	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_INSTALL	},
	{ "P9-07",		"Door height", 	M_RELEPARA, M_NUMBERPARA, 		&p.ucmp_door_height,					sizeof(p.ucmp_door_height), 0,	0,	0,	0,	5000,	2000,NO_OBJECT, 	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_INSTALL	},
	{ "P9-08",		"Car Apron.L", 	M_RELEPARA, M_NUMBERPARA, 		&p.ucmp_car_apron,					sizeof(p.ucmp_car_apron), 0,	0,	0,	0,	2000,	750,NO_OBJECT , 	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_INSTALL	},
	{ "P9-09",		"Hall apron.L", 	M_RELEPARA, M_NUMBERPARA, 		&p.ucmp_hall_apron,					sizeof(p.ucmp_hall_apron), 0,	0,	0,	0,2000,	350,NO_OBJECT , 	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_INSTALL	},
	{ "P9-10",		"Car distance", 	M_RELEPARA, M_NUMBERPARA, 		&p.ucmp_car_distance,					sizeof(p.ucmp_car_distance), 0,	0,	0,	0,	300,	30,NO_OBJECT , 	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_INSTALL	},
	{ "P9-11",		"Test dir.", 		M_RELEPARA, M_NUMBERPARA, 		&p.ucmp_test_dir,					sizeof(p.ucmp_test_dir), 0,	0,	0,	0,	2,	0,NO_OBJECT , 	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_INSTALL	},
	{ "P9-12",		"Test speed", 	M_RELEPARA, M_2BIT_DECIMAL, 		&p.ucmp_test_speed,					sizeof(p.ucmp_test_speed), 0,	0,	0,	0,	630,	100,NO_OBJECT , 	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_INSTALL	},

	{	"PA-00",		"Language",			M_INTERNPARA,	M_NUMBERPARA,		&language,						sizeof(language),						0,	0,	0,	0,	1,0,LANGUAGE,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_INSTALL	},
	{	"PA-01",		"2% load",			M_INTERNPARA,	M_NUMBERPARA,		&p.adlimit[0],					sizeof(p.adlimit[0]),					0,	0,	0,	0,	0xFFFF,	0,AD_EMPTY_LOAD,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_INSTALL	},
	{	"PA-02",		"50% load",			M_INTERNPARA,	M_NUMBERPARA,		&p.adlimit[1],					sizeof(p.adlimit[1]),					0,	0,	0,	0,	0xFFFF,	0,AD_HALF_LOAD,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_INSTALL	},
	{	"PA-03",		"95% load",			M_INTERNPARA,	M_NUMBERPARA,		&p.adlimit[2],					sizeof(p.adlimit[2]),					0,	0,	0,	0,	0xFFFF,	0,AD_FULL_LOAD,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_INSTALL	},
	{	"PA-04",		"102% load",		M_INTERNPARA,	M_NUMBERPARA,		&p.adlimit[3],					sizeof(p.adlimit[3]),					0,	0,	0,	0,	0xFFFF,	0,AD_OVER_LOAD,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_INSTALL	},
	{	"PA-07",		"Lift func1.",	M_INTERNPARA,	M_NUMBERPARA,		&p.lift_func_sel1, 			sizeof(p.lift_func_sel1),				0,	0,	0,	0,	255,	1,LIFT_FUNCTION_SEL1,0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{	"PA-12",		"LCD B.light",		M_INTERNPARA,	M_NUMBERPARA,	&p.lcd_backlight, 			sizeof(p.lcd_backlight),			0,	0,	0,	0,	255,	0,	BACKLIGHT_MODE,0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{ "PA-14",		"Landing push",		M_INTERNPARA, M_NUMBERPARA, 	&p.landingcall_push, 					sizeof(p.landingcall_push),						0,	0,	0,	0,	0xFF,	0,	LANDING_PUSH_MODE,0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN },
	{ "PA-15",		"Ups.run mode", 	M_INTERNPARA, M_NUMBERPARA, 	&p.ups_direction_mode,					sizeof(p.ups_direction_mode), 					0,	0,	0,	0,	0xFF,	0,	UPS_DIRECTION_MODE,0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN },
	{ "PA-16",		"Canbus check",		M_INTERNPARA, M_NUMBERPARA, 	&p.canbus_check, 					sizeof(p.canbus_check),						0,	0,	0,	0,	2,	0,	CANBUS_CHECK_MODE,0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN },
	{ "PA-27",		"Emp. PR. door",	M_INTERNPARA, M_NUMBERPARA, 	&p.emp_power_door, 					sizeof(p.emp_power_door),	0,	0,	0,	0,	3,	0,EMPPOWER_DOOR , 	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_INSTALL	},
	{ "PA-31",		"Door connect", 	M_INTERNPARA, M_NUMBERPARA, 	&p.door_connect,	sizeof(p.door_connect), 0,	0,	0,	0,	1,	0,DOOR_CONNECT_MODE ,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_INSTALL	},
	{ "PA-32",		"SHE door OP.", 	M_INTERNPARA, M_NUMBERPARA, 	&p.she_doornumber,	sizeof(p.she_doornumber), 0,	0,	0,	0,	1,	0,NO_OBJECT , 0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_INSTALL	},
	{ "PA-40",		"Brake auto", 	M_INTERNPARA, M_NUMBERPARA, 	&p.brake_check_auto,	sizeof(p.brake_check_auto), 0,	0,	0,	0,	1,	0,NO_OBJECT , 0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_INSTALL	},
	{ "PA-41",		"Brake cycle", 	M_INTERNPARA, M_NUMBERPARA, 	&p.brake_check_cycle,	sizeof(p.brake_check_cycle), 0,	0,	0,	0,	31,	0,NO_OBJECT , 0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_INSTALL	},
	{ "PA-42",		"Brake time", 	M_INTERNPARA, M_NUMBERPARA, 	&p.brake_check_time,	sizeof(p.brake_check_time), 0,	0,	0,	0,	2359,	0,NO_OBJECT , 0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_INSTALL	},

	{	"PB-01",		"Top floor",			M_BASIC,		M_FLOORPARA,			&p.top_floor,					sizeof(p.top_floor),					0,	0,	0,	1,MAX_FLOOR-1,	3,TOP_FLOOR,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_INSTALL	},
	{	"PB-02",		"Car door num.",	M_BASIC,		M_NUMBERPARA,		&p.doornumber,					sizeof(p.doornumber),					0,	0,	0,	1,MAX_DOOR,	1,DOORNUMBER,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_INSTALL	},
	{	"PB-03",		"PCU numbers",		M_BASIC,		M_NUMBERPARA,		&p.tse_number,					sizeof(p.tse_number),					0,	0,	0,	1,MAX_TSE,	1,PCUNUMBERS,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_INSTALL	},
	{	"PB-04",		"LCU numbers",		M_BASIC,		M_NUMBERPARA,		&p.ese_number,					sizeof(p.ese_number),					0,	0,	0,	1,MAX_ESE,	1,LCUNUMBERS,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_INSTALL	},
	{ "PB-06",		"FX active",			M_BASIC, 		M_NUMBERPARA, 	&p.fx_active, 				sizeof(p.fx_active),					0,	0,	0,				0,		1,		1,		FX_ACTIVE,			0,	D_VAR,	D_UNSIGNED8,	D_RW, RIGHT_INSTALL },
	{ "PB-07",		"Load type",			M_BASIC, 		M_NUMBERPARA, 	&p.loadtype,					sizeof(p.loadtype), 					0,	0,(void *)loadtype, 0,4,	0,LOADTYPE, 0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN },
	{	"PB-09",		"Save parameter",	M_BASIC,		M_NUMBERPARA,		&savepara,						sizeof(savepara),						0,	0,	0,	0,	1,	0,SAVEPARA,	0,	D_VAR,	D_UNSIGNED8,D_WRITE,RIGHT_INSTALL 	},

	{	"PC-00",		"Chk DL short",		M_CHECK,	M_NUMBERPARA,		&check_dl_short,				sizeof(check_dl_short),				0,	0,(void *)no_yes,	0,	1,1,CHK_DOORLOCK_SHORT,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_INSTALL	},
	{ "PC-01",		"Lock doors", 		M_CHECK, M_NUMBERPARA, 	&door_off,					sizeof(door_off), 					0,	0,(void *)no_yes, 0,	1,	0,DOOROFF,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_HELP 	},
	{ "PC-02",		"Disable LC", 		M_CHECK, M_NUMBERPARA, 	&landings_off,					sizeof(landings_off), 				0,	0,(void *)no_yes, 0,	1,	0,LANDINGS_OFF, 0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_HELP 	},
	{ "PC-03",		"Test load",			M_CHECK, M_NUMBERPARA, 	&bTestLoad, 					sizeof(bTestLoad),						0,	0,	0,	0,	1,	0,NO_OBJECT , 	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_INSTALL	},
	{ "PC-04",		"Reset MCU",			M_CHECK, M_NUMBERPARA, 	&reset_mcu, 					sizeof(reset_mcu),						0,	0,	0,	0,	1,	0,	CMD_RESET_MCU,0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN },
	{ "PC-05",		"Test ILSwitch",	M_CHECK, M_NUMBERPARA, 	&bTest_IlsSwitch, 					sizeof(bTest_IlsSwitch),	0,	0,	0,	0,	1,	0,NO_OBJECT , 	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_INSTALL	},
	{ "PC-06",		"Test UCMP",			M_CHECK, M_NUMBERPARA,		&bTestUCMP, 				sizeof(bTestUCMP), 0, 0,	0,	0,	1,	0,NO_OBJECT , 	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_INSTALL	},
	{ "PC-07",		"Reset UCMP", 		M_CHECK, M_NUMBERPARA,		&ucmp_state,				sizeof(ucmp_state), 0, 0, 0,	0,	0,	0,NO_OBJECT , 	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_INSTALL	},
	{ "PC-08",		"Check all DS", 	M_CHECK, M_NUMBERPARA,		&bTestDL, 			sizeof(bTestDL), 0, 0, 0, 0,	1,	0,NO_OBJECT , 	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_INSTALL	},
	{ "PC-09",		"Check brake", 		M_CHECK, M_NUMBERPARA,		&bTest_brake, 	sizeof(bTest_brake), 0, 0, 0, 0,	1,	0,NO_OBJECT , 	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_INSTALL	},
	{ "PC-10",		"Brake value",		M_CHECK, M_NUMBERPARA,		&brake_value, 	sizeof(brake_value), 0, 0, 0, 0,	3,	0,NO_OBJECT , 	0,	D_VAR,	D_UNSIGNED8,D_READ,RIGHT_INSTALL	},
	{ "PC-13",		"Reset trip",			M_CHECK, M_NUMBERPARA,		&bForceResetTrip, 				sizeof(bForceResetTrip), 0, 0,	0,	0,	1,	0,NO_OBJECT , 	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_INSTALL	},

	{	"PD0-01",		"Open park.",		M_DOOR1PARA,	M_NUMBERPARA,		&p.parkdoormode[0],				sizeof(p.parkdoormode[0]),				0,	0,(void *)openallow,	0,	200,	1,PARKDOORMODE,MAX_DOOR,D_ARRAY,D_UNSIGNED8,D_RW,RIGHT_INSTALL	},
	{	"PD0-02",		"Lightscr.[s]",		M_DOOR1PARA,	M_NUMBERPARA,		&p.lstime[0],					sizeof(p.lstime[0]),					0,	0,	0,	0,10,	3,LSTIME,MAX_DOOR,D_ARRAY,D_UNSIGNED8,D_RW,RIGHT_INSTALL	},
	{	"PD0-03",		"Rev.open[s]",		M_DOOR1PARA,	M_NUMBERPARA,		&p.revtime[0],					sizeof(p.revtime[0]),					0,	0,	0,	0,10,	6,REVTIME,MAX_DOOR,D_ARRAY,D_UNSIGNED8,D_RW,RIGHT_INSTALL	},
	{	"PD0-04",		"Nudging[*2s]",		M_DOOR1PARA,	M_NUMBERPARA,		&p.nudgingtime[0],				sizeof(p.nudgingtime[0]),				0,	0,	0,	0,240,	0,NUDGINGTIME,MAX_DOOR,D_ARRAY,D_UNSIGNED8,D_RW,RIGHT_INSTALL	},
	{	"PD0-05",		"Opentime[s]",		M_DOOR1PARA,	M_NUMBERPARA,		&p.dooropentime[0],				sizeof(p.dooropentime[0]),				0,	0,	0,	0,60,30,DOOROPENTIME,MAX_DOOR,D_ARRAY,D_UNSIGNED8,D_RW,RIGHT_INSTALL	},
	{	"PD0-06",		"Closetime[s]",		M_DOOR1PARA,	M_NUMBERPARA,		&p.doorclosetime[0],			sizeof(p.doorclosetime[0]),				0,	0,	0,	0,60,30,DOORCLOSETIME,MAX_DOOR,D_ARRAY,D_UNSIGNED8,D_RW,RIGHT_INSTALL	},
	{	"PD0-07",		"Open attempts",	M_DOOR1PARA,	M_NUMBERPARA,		&p.dooropenlimit[0],			sizeof(p.dooropenlimit[0]),				0,	0,	0,	0,30,10,DOOROPENLIMIT,MAX_DOOR,D_ARRAY,D_UNSIGNED8,D_RW,RIGHT_INSTALL	},
	{	"PD0-08",		"Closeattempts",	M_DOOR1PARA,	M_NUMBERPARA,		&p.doorcloselimit[0],			sizeof(p.doorcloselimit[0]),			0,	0,	0,	0,30,10,DOORCLOSELIMIT,MAX_DOOR,D_ARRAY,D_UNSIGNED8,D_RW,RIGHT_INSTALL	},
	{	"PD0-09",		"Switcht.[ms]",		M_DOOR1PARA,	M_NUMBERPARA,		&p.doorswitchtime[0],			sizeof(p.doorswitchtime[0]),			0,	0,	0,	0,1500,100,DOORSWITCHTIME,MAX_DOOR,D_ARRAY,D_UNSIGNED16,D_RW,RIGHT_INSTALL	},
	{	"PD0-10",		"Cls.delay[s]",		M_DOOR1PARA,	M_NUMBERPARA,		&p.doordelaytime[0],			sizeof(p.doordelaytime[0]),				0,	0,	0,	0,60,	0,DOORDELAYTIME,MAX_DOOR,D_ARRAY,D_UNSIGNED8,D_RW,RIGHT_INSTALL	},
	{	"PD0-11",		"Relays",			M_DOOR1PARA,	M_NUMBERPARA,		&p.doorrelaymode[0],			sizeof(p.doorrelaymode[0]),				0,	0,(void *)relaymode,	0,	3,	3,DOORRELAYMODE,MAX_DOOR,D_ARRAY,D_UNSIGNED8,D_RW,RIGHT_INSTALL	},
	{	"PD0-12",		"MaxCl.Rel[s]",		M_DOOR1PARA,	M_NUMBERPARA,		&p.doorrelaytime_cl[0],			sizeof(p.doorrelaytime_cl[0]),			0,	0,	0,	0,10000,	0,DOORRELAYTIME_CL,MAX_DOOR,D_ARRAY,D_UNSIGNED16,D_RW,RIGHT_INSTALL	},
	{	"PD0-13",		"MaxOp.Rel[s]",		M_DOOR1PARA,	M_NUMBERPARA,		&p.doorrelaytime_op[0],			sizeof(p.doorrelaytime_op[0]),			0,	0,	0,	0,10000,	0,DOORRELAYTIME_OP,MAX_DOOR,D_ARRAY,D_UNSIGNED16,D_RW,RIGHT_INSTALL	},
	{	"PD0-14",		"Max.LS [s]",		M_DOOR1PARA,	M_NUMBERPARA,		&p.max_ls_on[0],				sizeof(p.max_ls_on[0]),					0,	0,	0,	0,10000,600,MAX_LS_ON,MAX_DOOR,D_ARRAY,D_UNSIGNED16,D_RW,RIGHT_INSTALL	},

	{	"PD1-01",		"Open park.",		M_DOOR2PARA,	M_NUMBERPARA,		&p.parkdoormode[1],				sizeof(p.parkdoormode[1]),				0,	0,(void *)openallow,	0,	200,	1,DOUBLE_OBJECT,	0,	0,	0,	0,RIGHT_INSTALL	},
	{	"PD1-02",		"Lightscr.[s]",		M_DOOR2PARA,	M_NUMBERPARA,		&p.lstime[1],					sizeof(p.lstime[1]),					0,	0,	0,	0,120,	3,DOUBLE_OBJECT,	0,	0,	0,	0,RIGHT_INSTALL	},
	{	"PD1-03",		"Revers. [s]",		M_DOOR2PARA,	M_NUMBERPARA,		&p.revtime[1],					sizeof(p.revtime[1]),					0,	0,	0,	0,120,6,DOUBLE_OBJECT,	0,	0,	0,	0,RIGHT_INSTALL	},
	{	"PD1-04",		"Nudging[*2s]",		M_DOOR2PARA,	M_NUMBERPARA,		&p.nudgingtime[1],				sizeof(p.nudgingtime[1]),				0,	0,	0,	0,240,	0,DOUBLE_OBJECT,	0,	0,	0,	0,RIGHT_INSTALL	},
	{	"PD1-05",		"Opentime[s]",		M_DOOR2PARA,	M_NUMBERPARA,		&p.dooropentime[1],				sizeof(p.dooropentime[1]),				0,	0,	0,	0,120,30,DOUBLE_OBJECT,	0,	0,	0,	0,RIGHT_INSTALL	},
	{	"PD1-06",		"Closetime[s]",		M_DOOR2PARA,	M_NUMBERPARA,		&p.doorclosetime[1],			sizeof(p.doorclosetime[1]),				0,	0,	0,	0,120,30,DOUBLE_OBJECT,	0,	0,	0,	0,RIGHT_INSTALL	},
	{	"PD1-07",		"Open attempts",	M_DOOR2PARA,	M_NUMBERPARA,		&p.dooropenlimit[1],			sizeof(p.dooropenlimit[1]),				0,	0,	0,	0,100,4,DOUBLE_OBJECT,	0,	0,	0,	0,RIGHT_INSTALL	},
	{	"PD1-08",		"Closeattempts",	M_DOOR2PARA,	M_NUMBERPARA,		&p.doorcloselimit[1],			sizeof(p.doorcloselimit[1]),			0,	0,	0,	0,100,4,DOUBLE_OBJECT,	0,	0,	0,	0,RIGHT_INSTALL	},
	{	"PD1-09",		"Switcht.[ms]",		M_DOOR2PARA,	M_NUMBERPARA,		&p.doorswitchtime[1],			sizeof(p.doorswitchtime[1]),			0,	0,	0,	0,1500,100,DOUBLE_OBJECT,	0,	0,	0,	0,RIGHT_INSTALL	},
	{	"PD1-10",		"Cls.delay[s]",		M_DOOR2PARA,	M_NUMBERPARA,		&p.doordelaytime[1],			sizeof(p.doordelaytime[1]),				0,	0,	0,	0,120,	0,DOUBLE_OBJECT,	0,	0,	0,	0,RIGHT_INSTALL	},
	{	"PD1-11",		"Relays",			M_DOOR2PARA,	M_NUMBERPARA,		&p.doorrelaymode[1],			sizeof(p.doorrelaymode[1]),				0,	0,(void *)relaymode,	0,	3,	0,DOUBLE_OBJECT,	0,	0,	0,	0,RIGHT_INSTALL	},
	{	"PD1-12",		"MaxCl.Rel[s]",		M_DOOR2PARA,	M_NUMBERPARA,		&p.doorrelaytime_cl[1],			sizeof(p.doorrelaytime_cl[1]),			0,	0,	0,	0,10000,	0,DOUBLE_OBJECT,	0,	0,	0,	0,RIGHT_INSTALL	},
	{	"PD1-13",		"MaxOp.Rel[s]",		M_DOOR2PARA,	M_NUMBERPARA,		&p.doorrelaytime_op[1],			sizeof(p.doorrelaytime_op[1]),			0,	0,	0,	0,10000,	0,DOUBLE_OBJECT,	0,	0,	0,	0,RIGHT_INSTALL	},
	{	"PD1-14",		"Max.LS [s]",		M_DOOR2PARA,	M_NUMBERPARA,		&p.max_ls_on[1],				sizeof(p.max_ls_on[1]),					0,	0,	0,	0,10000,600,DOUBLE_OBJECT,	0,	0,	0,	0,RIGHT_INSTALL	},

	{	"PD2-",			"Floor",			M_DOORPOS,		M_DOORNUMBERPARA,	&p.doorpos[0],					sizeof(p.doorpos[0]),					1,	0,	0,	0,	3,	1,DOORPOS,MAX_FLOOR,D_ARRAY,D_UNSIGNED8,D_RW,RIGHT_INSTALL	},
	{	"PD3-",			"Floor",			M_PARKDOOR,		M_DOORNUMBERPARA,	&p.parkdoor[0],					sizeof(p.parkdoor[0]),					1,	0,	0,	0,	3,	1,PARKDOOR,MAX_FLOOR,D_ARRAY,D_UNSIGNED8,D_RW,RIGHT_INSTALL	},
	{	"PD4-",			"Floor",			M_DOOR_NORMAL,	M_DOORNUMBERPARA,	&p.doortable[DOOR_NORMAL][0],	sizeof(p.doortable[DOOR_NORMAL][0]),	1,	0,	0,	0,	3,	1,DOORTABLE_NORMAL,MAX_FLOOR,D_ARRAY,D_UNSIGNED8,D_RW,RIGHT_INSTALL	},
	{	"PD5-",			"Floor",			M_DOOR_N_HALL,	M_DOORNUMBERPARA,	&p.doortable[DOOR_N_HALL][0],	sizeof(p.doortable[DOOR_N_HALL][0]),	1,	0,	0,	0,	3,	1,DOORTABLE_N_HALL,MAX_FLOOR,D_ARRAY,D_UNSIGNED8,D_RW,RIGHT_INSTALL	},
	{	"PD6-",			"Floor",			M_DOOR_AUTOCALL,M_DOORNUMBERPARA,	&p.doortable[DOOR_AUTOCALL][0],	sizeof(p.doortable[DOOR_AUTOCALL][0]),	1,	0,	0,	0,	3,	1,DOORTABLE_AUTOCALL,MAX_FLOOR,D_ARRAY,D_UNSIGNED8,D_RW,RIGHT_INSTALL	},
	{	"PD7-",			"Floor",			M_DOOR_TIMER_1,	M_DOORNUMBERPARA,	&p.doortable[DOOR_TIMER_1][0],	sizeof(p.doortable[DOOR_TIMER_1][0]),	1,	0,	0,	0,	3,	1,DOORTABLE_TIMER_1,MAX_FLOOR,D_ARRAY,D_UNSIGNED8,D_RW,RIGHT_INSTALL	},
	{	"PD8-",			"Floor",			M_DOOR_T1_HALL,	M_DOORNUMBERPARA,	&p.doortable[DOOR_T1_HALL][0],	sizeof(p.doortable[DOOR_T1_HALL][0]),	1,	0,	0,	0,	3,	1,DOORTABLE_T1_HALL,MAX_FLOOR,D_ARRAY,D_UNSIGNED8,D_RW,RIGHT_INSTALL	},
	{	"PD9-",			"Floor",			M_DOOR_TIMER_2,	M_DOORNUMBERPARA,	&p.doortable[DOOR_TIMER_2][0],	sizeof(p.doortable[DOOR_TIMER_2][0]),	1,	0,	0,	0,	3,	1,DOORTABLE_TIMER_2,MAX_FLOOR,D_ARRAY,D_UNSIGNED8,D_RW,RIGHT_INSTALL	},
	{	"PD10-",		"Floor",			M_DOOR_T2_HALL,	M_DOORNUMBERPARA,	&p.doortable[DOOR_T2_HALL][0],	sizeof(p.doortable[DOOR_T2_HALL][0]),	1,	0,	0,	0,	3,	1,DOORTABLE_T2_HALL,MAX_FLOOR,D_ARRAY,D_UNSIGNED8,D_RW,RIGHT_INSTALL	},
	{	"PD11-",		"Floor",			M_DOOR_SPECIAL,	M_DOORNUMBERPARA,	&p.doortable[DOOR_SPECIAL][0],	sizeof(p.doortable[DOOR_SPECIAL][0]),	1,	0,	0,	0,	3,	1,DOORTABLE_SPECIAL,MAX_FLOOR,D_ARRAY,D_UNSIGNED8,D_RW,RIGHT_INSTALL	},
	{	"PD12-",		"Floor",			M_DOOR_ADV,		M_DOORNUMBERPARA,	&p.doortable[DOOR_ADV][0],		sizeof(p.doortable[DOOR_ADV][0]),		1,	0,	0,	0,	3,	1,DOORTABLE_ADV,MAX_FLOOR,D_ARRAY,D_UNSIGNED8,D_RW,RIGHT_INSTALL	},
	{	"PD13-",		"Floor",			M_DOOR_EMERG,	M_DOORNUMBERPARA,	&p.doortable[DOOR_EMERG][0],	sizeof(p.doortable[DOOR_EMERG][0]),		1,	0,	0,	0,	3,	1,DOORTABLE_EMERG,MAX_FLOOR,D_ARRAY,D_UNSIGNED8,D_RW,RIGHT_INSTALL	},
	{	"PD14-",		"Floor",			M_DOOR_ALARM,	M_DOORNUMBERPARA,	&p.doortable[DOOR_ALARM][0],	sizeof(p.doortable[DOOR_ALARM][0]),		1,	0,	0,	0,	3,	1,DOORTABLE_ALARM,MAX_FLOOR,D_ARRAY,D_UNSIGNED8,D_RW,RIGHT_INSTALL	},
	{	"PD15-",		"Floor",			M_DOOR_FIRE,	M_DOORNUMBERPARA,	&p.doortable[DOOR_FIREMAN][0],	sizeof(p.doortable[DOOR_FIREMAN][0]),	1,	0,	0,	0,	3,	1,DOORTABLE_FIREMAN,MAX_FLOOR,D_ARRAY,D_UNSIGNED8,D_RW,RIGHT_INSTALL	},

	{ "PD16-00", 	"Liop/Cop door", 	M_DOUBLE_DOOR,	M_NUMBERPARA, 	&p.liop_cop_door, 			sizeof(p.liop_cop_door),			0,	0,	0,	0,	3,	0,DOUBLE_OBJECT,	0,	D_VAR,	D_UNSIGNED8,	D_RW,RIGHT_INSTALL },
	{ "PD16-01", 	"OP./CL. door", 	M_DOUBLE_DOOR,	M_NUMBERPARA, 	&p.open_close_door, 			sizeof(p.open_close_door),			0,	0,	0,	0,	3,	0,DOUBLE_OBJECT,	0,D_VAR,	D_UNSIGNED8,	D_RW,RIGHT_INSTALL },
	{ "PD16-02", 	"Save door",	M_DOUBLE_DOOR,	M_NUMBERPARA, 	&save_double_door,	sizeof(save_double_door), 0,	0,	0,	0,1,0,DOUBLE_OBJECT,	0,	D_VAR,	D_UNSIGNED8,	D_RW,RIGHT_INSTALL },

	{ "PM-00",	"User mode",		M_SPECTION_MODE,	M_NUMBERPARA, 	&p.custumer1, 			sizeof(p.custumer1),			0,	0,	0,	0,	2,	0,NO_OBJECT,	0,	D_VAR,	D_UNSIGNED8,	D_RW,RIGHT_INSTALL },

	{	"PR0-00",		"Finish trip",		M_SPECIAL_TRIP,	M_NUMBERPARA,		&p.last_trip_special,			sizeof(p.last_trip_special),			0,	0,(void *)no_yes,	0,	1,1,LAST_TRIP_SPECIAL,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{	"PR0-01",		"CC Prior.[s]",		M_SPECIAL_TRIP,	M_NUMBERPARA,		&p.time_special,				sizeof(p.time_special),					0,	0,	0,	0,30,0,TIME_SPECIAL,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN	},
	{	"PR0-02",		"CC if active",		M_SPECIAL_TRIP,	M_NUMBERPARA,		&p.call_active_special,			sizeof(p.call_active_special),			0,	0,(void *)no_yes,	0,	1,	0,CALL_ACTIVE_SPECIAL,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{	"PR0-03",		"CC enable[s]",		M_SPECIAL_TRIP,	M_NUMBERPARA,		&p.calltime_special,			sizeof(p.calltime_special),				0,	0,	0,	0,30,5,CALLTIME_SPECIAL,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{	"PR0-04",		"->normal[s]",		M_SPECIAL_TRIP,	M_NUMBERPARA,		&p.time_back_special,			sizeof(p.time_back_special),			0,	0,	0,	0,30,5,TIME_BACK_SPECIAL,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{	"PR0-05",		"Land.CC canc.",	M_SPECIAL_TRIP,	M_NUMBERPARA,		&p.carcall_special_lc,			sizeof(p.carcall_special_lc),			0,	0,(void *)no_yes,	0,	1,1,CARCALL_SPECIAL_LC,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{	"PR0-06",		"LC disable",		M_SPECIAL_TRIP,	M_NUMBERPARA,		&p.hallcall_special,			sizeof(p.hallcall_special),				0,	0,(void *)no_yes,	0,	1,1,HALLCALL_SPECIAL,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{	"PR0-07",		"CarCCcanc.",		M_SPECIAL_TRIP,	M_NUMBERPARA,		&p.carcall_special,				sizeof(p.carcall_special),				0,	0,(void *)edge,	0,	3,3,CARCALL_SPECIAL,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{	"PR0-08",		"Max. CC no.",		M_SPECIAL_TRIP,	M_NUMBERPARA,		&p.max_cc[C_SPECIAL],			sizeof(p.max_cc[C_SPECIAL]),			0,	0,	&p.top_floor,	0,	0,	0,DOUBLE_OBJECT,	0,	0,	0,	0,RIGHT_MAINTAIN	},
	{	"PR0-09",		"All.norm.CC",		M_SPECIAL_TRIP,	M_NUMBERPARA,		&p.normal_cc_special,			sizeof(p.normal_cc_special),			0,	0,(void *)no_yes,	0,	1,0,NORMAL_CC_SPECIAL,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},

	{	"PR1-00",		"Finish trip",		M_ADV_TRIP,		M_NUMBERPARA,		&p.last_trip_advanced,			sizeof(p.last_trip_advanced),			0,	0,(void *)no_yes,	0,	1,	0,LAST_TRIP_ADVANCED,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{	"PR1-01",		"CC Prior[s]",		M_ADV_TRIP,		M_NUMBERPARA,		&p.time_advanced,				sizeof(p.time_advanced),				0,	0,	0,	0,30,30,TIME_ADVANCED,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN	},
	{	"PR1-02",		"CC if active",		M_ADV_TRIP,		M_NUMBERPARA,		&p.call_active_advanced,		sizeof(p.call_active_advanced),			0,	0,(void *)no_yes,	0,	1,	0,CALL_ACTIVE_ADVANCED,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{	"PR1-03",		"CC enabl.[s]",		M_ADV_TRIP,		M_NUMBERPARA,		&p.calltime_advanced,			sizeof(p.calltime_advanced),			0,	0,	0,	0,30,5,CALLTIME_ADVANCED,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{	"PR1-04",		"->normal[s]",		M_ADV_TRIP,		M_NUMBERPARA,		&p.time_back_advanced,			sizeof(p.time_back_advanced),			0,	0,	0,	0,30,5,TIME_BACK_ADVANCED,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{	"PR1-05",		"Land.CC canc.",	M_ADV_TRIP,		M_NUMBERPARA,		&p.carcall_advanced_lc,			sizeof(p.carcall_advanced_lc),			0,	0,(void *)no_yes,	0,	1,1,CARCALL_ADVANCED_LC,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{	"PR1-06",		"LC disable",		M_ADV_TRIP,		M_NUMBERPARA,		&p.hallcall_advanced,			sizeof(p.hallcall_advanced),			0,	0,(void *)no_yes,	0,	1,	1,HALLCALL_ADVANCED,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{	"PR1-07",		"CarCCcanc.",		M_ADV_TRIP,		M_NUMBERPARA,		&p.carcall_advanced,			sizeof(p.carcall_advanced),				0,	0,(void *)edge,	0,	3,3,CARCALL_ADVANCED,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{	"PR1-08",		"Max. CC no.",		M_ADV_TRIP,		M_NUMBERPARA,		&p.max_cc[C_ADVANCED],			sizeof(p.max_cc[C_ADVANCED]),			0,	0,	&p.top_floor,	0,	0,	0,DOUBLE_OBJECT,	0,	0,	0,	0,RIGHT_MAINTAIN	},
	{	"PR1-09",		"All.norm.CC",		M_ADV_TRIP,		M_NUMBERPARA,		&p.normal_cc_advanced,			sizeof(p.normal_cc_advanced),			0,	0,(void *)no_yes,	0,	1,	0,NORMAL_CC_ADVANCED,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},

	{	"PR2-00",		"Finish trip",		M_EMERG_TRIP,	M_NUMBERPARA,		&p.last_trip_emergency,			sizeof(p.last_trip_emergency),			0,	0,(void *)no_yes,	0,	1,	0,LAST_TRIP_EMERGENCY,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{	"PR2-01",		"CC Prior[s]",		M_EMERG_TRIP,	M_NUMBERPARA,		&p.time_emergency,				sizeof(p.time_emergency),				0,	0,	0,	0,30,30,TIME_EMERGENCY,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN	},
	{	"PR2-02",		"CC if active",		M_EMERG_TRIP,	M_NUMBERPARA,		&p.call_active_emergency,		sizeof(p.call_active_emergency),		0,	0,(void *)no_yes,	0,	1,	0,CALL_ACTIVE_EMERGENCY,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{	"PR2-03",		"CC enabl.[s]",		M_EMERG_TRIP,	M_NUMBERPARA,		&p.calltime_emergency,			sizeof(p.calltime_emergency),			0,	0,	0,	0,30,5,CALLTIME_EMERGENCY,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{	"PR2-04",		"->normal[s]",		M_EMERG_TRIP,	M_NUMBERPARA,		&p.time_back_emergency,			sizeof(p.time_back_emergency),			0,	0,	0,	0,30,5,TIME_BACK_EMERGENCY,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{	"PR2-05",		"Land.CC canc.",	M_EMERG_TRIP,	M_NUMBERPARA,		&p.carcall_emergency_lc,		sizeof(p.carcall_emergency_lc),			0,	0,(void *)no_yes,	0,	1,1,CARCALL_EMERGENCY_LC,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{	"PR2-06",		"LC disable",		M_EMERG_TRIP,	M_NUMBERPARA,		&p.hallcall_emergency,			sizeof(p.hallcall_emergency),			0,	0,(void *)no_yes,	0,	1,	1,HALLCALL_EMERGENCY,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{	"PR2-07",		"CarCCcanc.",		M_EMERG_TRIP,	M_NUMBERPARA,		&p.carcall_emergency,			sizeof(p.carcall_emergency),			0,	0,(void *)edge,	0,	3,3,CARCALL_EMERGENCY,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{	"PR2-08",		"Max. CC no.",		M_EMERG_TRIP,	M_NUMBERPARA,		&p.max_cc[C_EMERGENCY],			sizeof(p.max_cc[C_EMERGENCY]),			0,	0,	&p.top_floor,	0,	0,	0,DOUBLE_OBJECT,	0,	0,	0,	0,RIGHT_MAINTAIN	},
	{	"PR2-09",		"All.norm.CC",		M_EMERG_TRIP,	M_NUMBERPARA,		&p.normal_cc_emergency,			sizeof(p.normal_cc_emergency),			0,	0,(void *)no_yes,	0,	1,	0,NORMAL_CC_EMERGENCY,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{	"PR2-10",		"Forc.door cl.",	M_EMERG_TRIP,	M_NUMBERPARA,		&p.forced_close_emerg,			sizeof(p.forced_close_emerg),			0,	0,(void *)no_yes,	0,	1,1,FORCED_CLOSE_EMERG,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},

	{	"PR3-00",		"Min.delay[s]",		M_EVACUATION,	M_NUMBERPARA,		&p.evacuationdelay,				sizeof(p.evacuationdelay),				0,	0,	0,	0,600,0,EVACUATIONDELAY,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN	},
	{	"PR3-01",		"Max.delay[s]",		M_EVACUATION,	M_NUMBERPARA,		&p.evacuationtime,				sizeof(p.evacuationtime),				0,	0,	0,	0,600,	0,EVACUATIONTIME,	0,	D_VAR,	D_UNSIGNED32,D_RW,RIGHT_MAINTAIN	},
	{ "PR3-02",		"Evac.door mode", M_EVACUATION, M_NUMBERPARA, 	&p.evac_door, 			sizeof(p.evac_door),				0,	0,	0,	0,	1,1,EVAC_DOOR,0, D_VAR, D_UNSIGNED8,D_RW,RIGHT_MAINTAIN },
	{ "PR3-03",		"Evac.lightmode", M_EVACUATION, M_NUMBERPARA,	&p.evac_fire_carlightmode,			sizeof(p.evac_fire_carlightmode), 			0,	0,	0,	0,	3,1,EVAC_CARLIGHT_MODE,0, D_VAR, D_UNSIGNED8,D_RW,RIGHT_MAINTAIN },
	{	"PR3-04",		"Auto return",		M_EVACUATION,	M_NUMBERPARA,		&p.evac_autoreturn,				sizeof(p.evac_autoreturn),				0,	0,(void *)no_yes,	0,	1,1,EVAC_AUTORETURN,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{ "PR3-05", 	"Ligthfan T.",M_EVACUATION, M_NUMBERPARA, 	&p.carlight_fan_time_fire, 	sizeof(p.carlight_fan_time_fire),	0,	0,(void *)no_yes, 0,	1000,1,EVAC_CARLIGHT_TIME,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN },
	{ "PR3-06", 	"No move[s]",M_EVACUATION, M_NUMBERPARA, 	&p.evaction_waittime, 	sizeof(p.evaction_waittime),	0,	0,(void *)no_yes, 0,	255,1,NO_OBJECT,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN },

	{	"PR4-",			"F.Evac.fl.",		M_FIRE_FLOOR,	M_FLOORPARA,		&p.fire_floor[0],				sizeof(p.fire_floor[0]),				1,	&p.bot_floor,	&p.top_floor,0xFFFFFFFF,	0,	1,FIRE_FLOOR,MAX_FLOOR,D_ARRAY,D_UNSIGNED8,D_RW,RIGHT_MAINTAIN },

	{	"PR6-00",		"Door cl.",			M_FIREMAN_TRIP,	M_NUMBERPARA,		&p.fire_doorclosemode,			sizeof(p.fire_doorclosemode),			0,	0,(void *)firedoor,	0,	4,3,FIRE_DOORCLOSEMODE,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{	"PR6-01",		"Door op.",			M_FIREMAN_TRIP,	M_NUMBERPARA,		&p.fire_dooropenmode,			sizeof(p.fire_dooropenmode),			0,	0,(void *)firedoor,	0,	3,3,FIRE_DOOROPENMODE,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{ "PR6-02", 	"F.alarm fun.", M_FIREMAN_TRIP, M_NUMBERPARA, 	&p.firealarm_function,			sizeof(p.firealarm_function), 		0,	0,(void *)no_yes, 0,	255,	1,FORCED_CLOSE_FIRE,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN },
	{	"PR6-04",		"Fireman fun1",M_FIREMAN_TRIP,	M_NUMBERPARA,		&p.fireman_function1,				sizeof(p.fireman_function1),				0,	0,(void *)door_firefloor,	0,	255,	0,NO_OBJECT,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},

	{	"PR7-00",		"Compuls.stop",		M_FORCED_STOP,	M_NUMBERPARA,		&p.forced_stop,					sizeof(p.forced_stop),					0,	0,0,	0,	3,	0,FORCED_STOP,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{	"PR7-01",		"Floor",			M_FORCED_STOP,	M_FLOORPARA,		&p.forced_stop_floor,			sizeof(p.forced_stop_floor),			0,	&p.bot_floor,	&p.top_floor,0xFFFFFFFF,	0,	1,FORCED_STOP_FLOOR,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{	"PR7-02",		"Doors",			M_FORCED_STOP,	M_NUMBERPARA,		&p.forced_stop_door,			sizeof(p.forced_stop_door),				0,	0,	0,	0,	3,	1,FORCED_STOP_DOOR,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{	"PR7-03",		"Direct.",			M_FORCED_STOP,	M_NUMBERPARA,		&p.forced_stop_dir,				sizeof(p.forced_stop_dir),				0,	0,(void *)dirtext,	1,	3,3,FORCED_STOP_DIR,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{ "PR7-04", 	"Starttime[h]", 	M_FORCED_STOP, M_NUMBERPARA, 	&p.forced_starttime, 			sizeof(p.forced_starttime),				0,	0,	0,	0,23, 0,FORCE_STARTHOUR,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN },
	{ "PR7-05", 	"Stop time[h]",	M_FORCED_STOP, M_NUMBERPARA,			&p.forced_stoptime,			sizeof(p.forced_stoptime), 			0,	0,	0,	0,23, 0,FORCE_STOPHOUR,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN },

	{	"PR8-00",		"Floor",			M_REMOTE_OFF,	M_FLOORPARA,		&p.remote_off_floor,			sizeof(p.remote_off_floor),				0,	&p.bot_floor,	&p.top_floor,0xFFFFFFFF,	0,	0,REMOTE_OFF_FLOOR,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{	"PR8-01",		"Doors",			M_REMOTE_OFF,	M_DOORNUMBERPARA	,	&p.remote_off_door,			sizeof(p.remote_off_door),				0,	0,	0,	0,	3,	0,REMOTE_OFF_DOOR,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{	"PR8-02",		"Func.sel",		M_REMOTE_OFF,	M_NUMBERPARA,		&p.remote_off_func,			sizeof(p.remote_off_func),				0,	0,0,	0,	7,	1,REMOTE_OFF_LIGHT,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{	"PR8-03",		"Starttime[h]",		M_REMOTE_OFF,	M_NUMBERPARA,		&p.remote_starttime,			sizeof(p.remote_starttime),			0,	0,0,	0,	23,	0,NO_OBJECT,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{	"PR8-04",		"Stoptime[h]",		M_REMOTE_OFF,	M_NUMBERPARA,		&p.remote_stoptime,			sizeof(p.remote_stoptime),			0,	0,0,	0,	23,	0,NO_OBJECT,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},

	{	"PR9-00",		"ClearCall",		M_TIMER_TRIP,	M_NUMBERPARA,		&p.timer_clearcall,				sizeof(p.timer_clearcall),				0,	0,(void *)edge,	0,	3,	0,TIMER_CLEARCALL,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{	"PR9-01",		"CR1 Start[h]",		M_TIMER_TRIP,	M_NUMBERPARA,		&p.timertrip_start,				sizeof(p.timertrip_start),				0,	0,	0,	0,23,	0,TIMERTRIP_START,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{	"PR9-02",		"CR1 Stop[h]",		M_TIMER_TRIP,	M_NUMBERPARA,		&p.timertrip_stop,				sizeof(p.timertrip_stop),				0,	0,	0,	0,23,	0,TIMERTRIP_STOP,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{	"PR9-03",		"CR1 Parkfloor",	M_TIMER_TRIP,	M_FLOORPARA,		&p.timer1_parkfloor,			sizeof(p.timer1_parkfloor),				0,	&p.bot_floor,	&p.top_floor,0xFFFFFFFF,	0,	1,TIMER1_PARKFLOOR,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{	"PR9-04",		"CR2 Parkfloor",	M_TIMER_TRIP,	M_FLOORPARA,		&p.timer2_parkfloor,			sizeof(p.timer1_parkfloor),				0,	&p.bot_floor,	&p.top_floor,0xFFFFFFFF,	0,	1,TIMER2_PARKFLOOR,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},

	{	"PR10-00",	"En_Disable",			M_DIS_ENABLE,	M_NUMBERPARA,		&p.call_disable_enable,	sizeof(p.call_disable_enable),		0,	0,(void *)normal_special,	0,	255,	0,DISABLE_SPECIAL,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},

	{	"PR11-00",		"Attan func.",		M_ATTENDANCE,	M_NUMBERPARA,		&p.attend_carcall,				sizeof(p.attend_carcall),				0,	0,(void *)no_yes,	0,	255,	0,ATTEND_CARCALL,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{ "PR11-01",		"Att_Hall.alam",	M_ATTENDANCE, M_NUMBERPARA, 	&p.attend_normal_timer, 		sizeof(p.attend_normal_timer), 0, 0,	0,	0,	10,	0,ATT_HALL_ALARM ,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_INSTALL	},

	{ "PR12-00",		"ATC.Stop dir.",	M_COOPERATE,		M_NUMBERPARA, 	&p.autocall_dir,				sizeof(p.autocall_dir), 				0,	0,(void *)dirtext,	1,	3,3,AUTOCALL_DIR, 0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN },
	{ "PR12-01",		"Coop. sel1.",	M_COOPERATE, M_NUMBERPARA,				&p.cooperate_sel1,					sizeof(p.cooperate_sel1), 					0,	0,	0,	0,	0xFF, 0,	COOPERATE_FUNC1,0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN },
	{ "PR12-02",		"Coop. sel2.", 	M_COOPERATE, M_NUMBERPARA,				&p.cooperate_sel2,					sizeof(p.cooperate_sel2), 					0,	0,	0,	0,	0xFF, 0,	COOPERATE_FUNC2,0, D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN },
	{ "PR12-03",		"Arr.gong S.[h]", 	M_COOPERATE, M_NUMBERPARA,	&p.arrivegong_starttime,					sizeof(p.arrivegong_starttime), 					0,	0,	0,	0,	23, 0,	NO_OBJECT,0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN },
	{ "PR12-04",		"Arr.gong E.[h]", 	M_COOPERATE, M_NUMBERPARA,	&p.arrivegong_stoptime, 				sizeof(p.arrivegong_stoptime),					0,	0,	0,	0,	23, 0,	NO_OBJECT,0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN },

	{	"PR14-00",		"CC undo",			M_MISUSE,		M_NUMBERPARA,		&p.cc_cancel,					sizeof(p.cc_cancel),					0,	0,(void *)single_double,	0,2,	2,CC_CANCEL,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{	"PR14-01",		"LC dir. dep.",		M_MISUSE,		M_NUMBERPARA,		&p.dir_pending,					sizeof(p.dir_pending),					0,	0,(void *)no_yes,	0,	1,1,DIR_PENDING,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{	"PR14-02",		"Max. CC no.",		M_MISUSE,		M_NUMBERPARA,		&p.max_cc[C_STANDARD],			sizeof(p.max_cc[C_STANDARD]),			0,	0,	&p.top_floor,	0,	0,	0,MAX_CC,MAX_PRIORITY,D_ARRAY,D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{	"PR14-03",		"Max.CC empty",		M_MISUSE,		M_NUMBERPARA,		&p.max_cc_empty,				sizeof(p.max_cc_empty),					0,	0,	&p.top_floor,	0,	0,	0,MAX_CC_EMPTY,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{	"PR14-04",		"Check LS",			M_MISUSE,		M_NUMBERPARA,		&p.cc_no_ls,					sizeof(p.cc_no_ls),						0,	0,	&p.top_floor,	0,	0,	0,CC_NO_LS,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{	"PR14-05",		"Counter call",		M_MISUSE,		M_NUMBERPARA,		&p.clear_other_lc_if_cc,		sizeof(p.clear_other_lc_if_cc),			0,	0,(void *)no_yes,	0,	1,	0,CLEAR_OTHER_LC_IF_CC,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{	"PR14-06",		"Clr.last fl.",		M_MISUSE,		M_NUMBERPARA,		&p.clear_cc_last_floor,			sizeof(p.clear_cc_last_floor),			0,	0,(void *)no_yes,	0,	1,	0,CLEAR_CC_LAST_FLOOR,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{	"PR14-07",		"Clr.opp.dir.",		M_MISUSE,		M_NUMBERPARA,		&p.clear_cc_opp_dir,			sizeof(p.clear_cc_opp_dir),				0,	0,(void *)no_yes,	0,	1,	0,CLEAR_CC_OPP_DIR,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},

	{	"PR15-00",		"Doorclose[s]",		M_GROUPCONTROL,	M_NUMBERPARA,		&p.max_door_close_group,		sizeof(p.max_door_close_group),			0,	0,	0,	0,240,20,MAX_DOOR_CLOSE_GROUP,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{	"PR15-01",		"\5 peak on[h1]",	M_GROUPCONTROL,	M_NUMBERPARA,		&p.up_peak_start_h1,				sizeof(p.up_peak_start_h1),				0,	0,	0,	0,23,	0,UP_PEAK_START,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{ "PR15-02",		"\5 peak on[m1]",	M_GROUPCONTROL, M_NUMBERPARA, 	&p.up_peak_start_m1,				sizeof(p.up_peak_start_m1), 			0,	0,	0,	0,59, 0,NO_OBJECT,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN },
	{	"PR15-03",		"\5 peakoff[h1]",	M_GROUPCONTROL,	M_NUMBERPARA,		&p.up_peak_stop_h1,				sizeof(p.up_peak_stop_h1),					0,	0,	0,	0,23,	0,UP_PEAK_STOP,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{ "PR15-04",		"\5 peakoff[m1]", M_GROUPCONTROL, M_NUMBERPARA, 	&p.up_peak_stop_m1, 			sizeof(p.up_peak_stop_m1),					0,	0,	0,	0,59, 0,NO_OBJECT, 0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN },
	{	"PR15-05",		"\5 peak on[h2]",	M_GROUPCONTROL,	M_NUMBERPARA,		&p.up_peak_start_h2,				sizeof(p.up_peak_start_h2),				0,	0,	0,	0,23,	0,UP_PEAK_START,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{ "PR15-06",		"\5 peak on[m2]",	M_GROUPCONTROL, M_NUMBERPARA, 	&p.up_peak_start_m2,				sizeof(p.up_peak_start_m2), 			0,	0,	0,	0,59, 0,NO_OBJECT,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN },
	{	"PR15-07",		"\5 peakoff[h2]",	M_GROUPCONTROL,	M_NUMBERPARA,		&p.up_peak_stop_h2,				sizeof(p.up_peak_stop_h2),					0,	0,	0,	0,23,	0,UP_PEAK_STOP,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{ "PR15-08",		"\5 peakoff[m2]", M_GROUPCONTROL, M_NUMBERPARA, 	&p.up_peak_stop_m2, 			sizeof(p.up_peak_stop_m2),					0,	0,	0,	0,59, 0,NO_OBJECT, 0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN },
	{	"PR15-09",		"\5 peak on[h3]",	M_GROUPCONTROL,	M_NUMBERPARA,		&p.up_peak_start_h3,				sizeof(p.up_peak_start_h3),				0,	0,	0,	0,23,	0,UP_PEAK_START,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{ "PR15-10",		"\5 peak on[m3]",	M_GROUPCONTROL, M_NUMBERPARA, 	&p.up_peak_start_m3,				sizeof(p.up_peak_start_m3), 			0,	0,	0,	0,59, 0,NO_OBJECT,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN },
	{	"PR15-11",		"\5 peakoff[h3]",	M_GROUPCONTROL,	M_NUMBERPARA,		&p.up_peak_stop_h3,				sizeof(p.up_peak_stop_h3),					0,	0,	0,	0,23,	0,UP_PEAK_STOP,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{ "PR15-12",		"\5 peakoff[m3]", M_GROUPCONTROL, M_NUMBERPARA, 	&p.up_peak_stop_m3, 			sizeof(p.up_peak_stop_m3),					0,	0,	0,	0,59, 0,NO_OBJECT, 0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN },
	{	"PR15-13",		"\6 peak on[h1]",	M_GROUPCONTROL,	M_NUMBERPARA,		&p.dn_peak_start_h1,				sizeof(p.dn_peak_start_h1),				0,	0,	0,	0,23,	0,DN_PEAK_START,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{ "PR15-14",		"\6 peak on[m1]", M_GROUPCONTROL, M_NUMBERPARA, 	&p.dn_peak_start_m1,				sizeof(p.dn_peak_start_m1), 			0,	0,	0,	0,59, 0,NO_OBJECT,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN },
	{	"PR15-15",		"\6 peakoff[h1]",	M_GROUPCONTROL,	M_NUMBERPARA,		&p.dn_peak_stop_h1,				sizeof(p.dn_peak_stop_h1),					0,	0,	0,	0,23,	0,DN_PEAK_STOP,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{ "PR15-16",		"\6 peakoff[m1]",	M_GROUPCONTROL, M_NUMBERPARA, 	&p.dn_peak_stop_m1, 			sizeof(p.dn_peak_stop_m1),					0,	0,	0,	0,59, 0,NO_OBJECT, 0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN },
	{ "PR15-17",		"\6 peak on[h2]", M_GROUPCONTROL, M_NUMBERPARA, 	&p.dn_peak_start_h2,				sizeof(p.dn_peak_start_h2), 			0,	0,	0,	0,23, 0,DN_PEAK_START,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN },
	{ "PR15-18",		"\6 peak on[m2]", M_GROUPCONTROL, M_NUMBERPARA, 	&p.dn_peak_start_m2,				sizeof(p.dn_peak_start_m2), 			0,	0,	0,	0,59, 0,NO_OBJECT,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN },
	{ "PR15-19",		"\6 peakoff[h2]", M_GROUPCONTROL, M_NUMBERPARA, 	&p.dn_peak_stop_h2, 			sizeof(p.dn_peak_stop_h2),					0,	0,	0,	0,23, 0,DN_PEAK_STOP, 0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN },
	{ "PR15-20",		"\6 peakoff[m2]", M_GROUPCONTROL, M_NUMBERPARA, 	&p.dn_peak_stop_m2, 			sizeof(p.dn_peak_stop_m2),					0,	0,	0,	0,59, 0,NO_OBJECT, 0, D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN },
	{ "PR15-21",		"\6 peak on[h3]", M_GROUPCONTROL, M_NUMBERPARA, 	&p.dn_peak_start_h3,				sizeof(p.dn_peak_start_h3), 			0,	0,	0,	0,23, 0,DN_PEAK_START,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN },
	{ "PR15-22",		"\6 peak on[m3]", M_GROUPCONTROL, M_NUMBERPARA, 	&p.dn_peak_start_m3,				sizeof(p.dn_peak_start_m3), 			0,	0,	0,	0,59, 0,NO_OBJECT,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN },
	{ "PR15-23",		"\6 peakoff[h3]", M_GROUPCONTROL, M_NUMBERPARA, 	&p.dn_peak_stop_h3, 			sizeof(p.dn_peak_stop_h3),					0,	0,	0,	0,23, 0,DN_PEAK_STOP, 0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN },
	{ "PR15-24",		"\6 peakoff[m3]", M_GROUPCONTROL, M_NUMBERPARA, 	&p.dn_peak_stop_m3, 			sizeof(p.dn_peak_stop_m3),					0,	0,	0,	0,59, 0,NO_OBJECT, 0, D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN },
	{	"PR15-25",		"\5 Park floor",	M_GROUPCONTROL,	M_NUMBERPARA,		&p.up_peak_parkfloor,			sizeof(p.up_peak_parkfloor),			0,	&p.bot_floor,	&p.top_floor,0xFFFFFFFF,	0,	1,UP_PEAK_PARKFLOOR,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},
	{	"PR15-26",		"\6 Park floor",	M_GROUPCONTROL,	M_NUMBERPARA,		&p.dn_peak_parkfloor,			sizeof(p.dn_peak_parkfloor),			0,	&p.bot_floor,	&p.top_floor,0xFFFFFFFF,	0,	1,DN_PEAK_PARKFLOOR,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN	},

	{	"PR16-00",		"End year:",		M_TRIPLIMIT,	M_NUMBERPARA,		&p.t_limit.year,				sizeof(p.t_limit.year),					0,	0,	0,	0,9999,	0,LIMIT_YEARS,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN },
	{	"PR16-01",		"End month:",		M_TRIPLIMIT,	M_NUMBERPARA,		&p.t_limit.month,				sizeof(p.t_limit.month),				0,	0,	0,	0,12,	0,LIMIT_MONTHS,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN },
	{	"PR16-02",		"End day:",			M_TRIPLIMIT,	M_NUMBERPARA,		&p.t_limit.day,					sizeof(p.t_limit.day),					0,	0,	0,	0,31,	0,LIMIT_DAYS,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN },
	{	"PR16-03",		"End hour:",		M_TRIPLIMIT,	M_NUMBERPARA,		&p.t_limit.hour,				sizeof(p.t_limit.hour),					0,	0,	0,	0,23,	0,LIMIT_HOURS,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN },
	{	"PR16-04",		"Times:",			M_TRIPLIMIT,	M_NUMBERPARA,		&p.limittimes,					sizeof(p.limittimes),					0,	0,	0,	0,500000,	0,LIMIT_COUNT,	0,	D_VAR,	D_UNSIGNED32,D_RW,RIGHT_MAINTAIN },
	{	"PR16-05",		"Start counting",	M_TRIPLIMIT,	M_NUMBERPARA,		&start_limit,					sizeof(start_limit),					0,	0,	0,	0,	1,	0,NO_OBJECT,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_MAINTAIN },

	{	"PR17-00",		"Test runs",		M_TESTTRIP,		M_NUMBERPARA,		&testtrip,						sizeof(testtrip),						0,	0,(void *)no_yes,	0,	1,	0,TESTTRIP,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_ADMIN	},
	{	"PR17-04",		"Start[h]",			M_TESTTRIP,		M_NUMBERPARA,		&test_starttime,		sizeof(test_starttime),				0,	0,	0,	0,23,	0,TEST_STARTTIME,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_ADMIN	},
	{	"PR17-05",		"End[h]",			M_TESTTRIP,		M_NUMBERPARA,		&test_endtime,				sizeof(test_endtime),					0,	0,	0,	0,23,	0,TEST_ENDTIME,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_ADMIN	},
	{	"PR17-07",		"Times",			M_TESTTRIP,		M_NUMBERPARA,		&test_times,					sizeof(test_times),					0,	0,	0,	0,250,	0,TEST_TIMES,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_ADMIN	},
	{	"PR17-08",		"Interval[s]",		M_TESTTRIP,		M_NUMBERPARA,	&test_interval,			sizeof(test_interval),				0,	0,	0,	0,180,	0,TEST_INTERVAL,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_ADMIN	},
	{	"PR17-10",		"Simulation",		M_TESTTRIP,		M_NUMBERPARA,		&p.simulation,					sizeof(p.simulation),					0,	0,(void *)no_yes,	0,	2,	0,NO_OBJECT,	0,	0,	0,	0,RIGHT_ADMIN	},
	{ "PR17-11",		"Simu.mult", 	M_TESTTRIP, 	M_NUMBERPARA, 	&p.simu_mult,					sizeof(p.simu_mult), 				0,	0,(void *)no_yes, 0,	250,	0,NO_OBJECT,	0,	0,	0,	0,RIGHT_ADMIN },

//F0 parameter
	{	"F0-00",		"F0-00",			M_NICE_F0,		M_NUMBERPARA,		&niv.F0_00,		sizeof(niv.F0_00),		0,	0,	0,	0,		2,			1,		1 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN},
	{	"F0-01",		"F0-01",			M_NICE_F0,		M_NUMBERPARA,		&niv.F0_01,		sizeof(niv.F0_01),		0,	0,	0,	0,		1,			1,		2 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN},
	{	"F0-02",		"F0-02",			M_NICE_F0,		M_3BIT_DECIMAL,		&niv.F0_02,		sizeof(niv.F0_02),		0,	0,	0,	50,		10000,	50,		3 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN },
	{	"F0-03",		"F0-03",			M_NICE_F0,		M_3BIT_DECIMAL,		&niv.F0_03,		sizeof(niv.F0_03),		0,	0,	0,	100,	4000,		1600,	4 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN },
	{	"F0-04",		"F0-04",			M_NICE_F0,		M_3BIT_DECIMAL,		&niv.F0_04,		sizeof(niv.F0_04),		0,	0,	0,	100,	4000,		1600,	5 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN },
	{	"F0-05",		"F0-05",			M_NICE_F0,		M_NUMBERPARA,		&niv.F0_05,		sizeof(niv.F0_05),		0,	0,	0,	0,		1,			0,		6 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN},
	{	"F0-07",		"F0-07",			M_NICE_F0,		M_1BIT_DECIMAL,		&niv.F0_07,		sizeof(niv.F0_07),		0,	0,	0,	20,		160,		80,		7 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN},

//F1 parameter
	{	"F1-01",		"F1-01",			M_NICE_F1,		M_1BIT_DECIMAL,		&niv.F1_01,		sizeof(niv.F1_01),		0,	0,	0,	1,		3000,		55,		9 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN},
	{	"F1-02",		"F1-02",			M_NICE_F1,		M_NUMBERPARA,		&niv.F1_02,		sizeof(niv.F1_02),		0,	0,	0,	50,		600,		380,	10 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN },
	{	"F1-03",		"F1-03",			M_NICE_F1,		M_2BIT_DECIMAL,		&niv.F1_03,		sizeof(niv.F1_03),		0,	0,	0,	0,		65500,	2500,	11 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN},
	{	"F1-04",		"F1-04",			M_NICE_F1,		M_2BIT_DECIMAL,		&niv.F1_04,		sizeof(niv.F1_04),		0,	0,	0,	0,		9900,		50,		12 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN},
	{	"F1-05",		"F1-05",			M_NICE_F1,		M_NUMBERPARA,		&niv.F1_05,		sizeof(niv.F1_05),		0,	0,	0,	0,		3000,		1460,	13 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN},
	{	"F1-06",		"F1-06",			M_NICE_F1,		M_3BIT_DECIMAL,		&niv.F1_06,		sizeof(niv.F1_06),		0,	0,	0,	0,		65000,	0,		14 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN},
	{	"F1-07",		"F1-07",			M_NICE_F1,		M_3BIT_DECIMAL,		&niv.F1_07,		sizeof(niv.F1_07),		0,	0,	0,	0,		65000,	0,		15 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN},
	{	"F1-08",		"F1-08",			M_NICE_F1,		M_2BIT_DECIMAL,		&niv.F1_08,		sizeof(niv.F1_08),		0,	0,	0,	0,		65000,	234,	16 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN},
	{	"F1-09",		"F1-09",			M_NICE_F1,		M_1BIT_DECIMAL,		&niv.F1_09,		sizeof(niv.F1_09),		0,	0,	0,	0,		30000,	666,	17 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN},
	{	"F1-10",		"F1-10",			M_NICE_F1,		M_2BIT_DECIMAL,		&niv.F1_10,		sizeof(niv.F1_10),		0,	0,	0,	1,		65000,	1070,	18 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN},
	{	"F1-11",		"F1-11",			M_NICE_F1,		M_NUMBERPARA,		&niv.F1_11,		sizeof(niv.F1_11),		0,	0,	0,	0,		3,			0,		19 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN},
	{	"F1-12",		"F1-12",			M_NICE_F1,		M_NUMBERPARA,		&niv.F1_12,		sizeof(niv.F1_12),		0,	0,	0,	0,		65535,	0,		20 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN},
	{	"F1-13",		"F1-13",			M_NICE_F1,		M_NUMBERPARA,		&niv.F1_13,		sizeof(niv.F1_13),		0,	0,	0,	30,		150,		60,		21 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN},
	{	"F1-15",		"F1-15",			M_NICE_F1,		M_2BIT_DECIMAL,		&niv.F1_15,		sizeof(niv.F1_15),		0,	0,	0,	0,		65000,	300,	22 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN},
	{	"F1-16",		"F1-16",			M_NICE_F1,		M_2BIT_DECIMAL,		&niv.F1_16,		sizeof(niv.F1_16),		0,	0,	0,	0,		65000,	300,	23 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN},
	{	"F1-17",		"F1-17",			M_NICE_F1,		M_NUMBERPARA,		&niv.F1_17,		sizeof(niv.F1_17),		0,	0,	0,	0,		65535,	0,		24 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN},
	{	"F1-25",		"F1-25",			M_NICE_F1,		M_NUMBERPARA,		&niv.F1_25,		sizeof(niv.F1_25),		0,	0,	0,	0,		1,			1,		25 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN},

//F2 parameter
	{	"F2-00",		"F2-00",			M_NICE_F2,		M_NUMBERPARA,		&niv.F2_00,		sizeof(niv.F2_00),		0,	0,	0,	1,		100,		40,		26 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN},
	{	"F2-01",		"F2-01",			M_NICE_F2,		M_2BIT_DECIMAL,		&niv.F2_01,		sizeof(niv.F2_01),		0,	0,	0,	1,		1000,		60,		27 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN},
	{	"F2-02",		"F2-02",			M_NICE_F2,		M_2BIT_DECIMAL,		&niv.F2_02,		sizeof(niv.F2_02),		0,	0,	0,	0,		9900,		200,	29 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN},
	{	"F2-03",		"F2-03",			M_NICE_F2,		M_NUMBERPARA,		&niv.F2_03,		sizeof(niv.F2_03),		0,	0,	0,	1,		100,		35,		30 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN},
	{	"F2-04",		"F2-04",			M_NICE_F2,		M_2BIT_DECIMAL,		&niv.F2_04,		sizeof(niv.F2_04),		0,	0,	0,	1,		1000,		80,		28 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN},
	{	"F2-05",		"F2-05",			M_NICE_F2,		M_2BIT_DECIMAL,		&niv.F2_05,		sizeof(niv.F2_05),		0,	0,	0,	0,		9900,		500,	32 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN},
	{	"F2-06",		"F2-06",			M_NICE_F2,		M_NUMBERPARA,		&niv.F2_06,		sizeof(niv.F2_06),		0,	0,	0,	10,		1000,		60,		33 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN},
	{	"F2-07",		"F2-07",			M_NICE_F2,		M_NUMBERPARA,		&niv.F2_07,		sizeof(niv.F2_07),		0,	0,	0,	10,		1000,		30,		34 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN},
	{	"F2-08",		"F2-08",			M_NICE_F2,		M_1BIT_DECIMAL,		&niv.F2_08,		sizeof(niv.F2_08),		0,	0,	0,	0,		2000,		1500,	35 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN},
	{	"F2-09",		"F2-09",			M_NICE_F2,		M_NUMBERPARA,		&niv.F2_09,		sizeof(niv.F2_09),		0,	0,	0,	10,		1000,		60,		44 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN},
	{	"F2-10",		"F2-10",			M_NICE_F2,		M_NUMBERPARA,		&niv.F2_10,		sizeof(niv.F2_10),		0,	0,	0,	10,		1000,		30,		45 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN},
	{	"F2-11",		"F2-11",			M_NICE_F2,		M_NUMBERPARA,		&niv.F2_11,		sizeof(niv.F2_11),		0,	0,	0,	0,		1,			0,		36 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN},
	{	"F2-12",		"F2-12",			M_NICE_F2,		M_NUMBERPARA,		&niv.F2_12,		sizeof(niv.F2_12),		0,	0,	0,	1,		100,		35,		37 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN},
	{	"F2-13",		"F2-13",			M_NICE_F2,		M_2BIT_DECIMAL,		&niv.F2_13,		sizeof(niv.F2_13),		0,	0,	0,	1,		1000,		80,		38 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN},
	{	"F2-14",		"F2-14",			M_NICE_F2,		M_NUMBERPARA,		&niv.F2_14,		sizeof(niv.F2_14),		0,	0,	0,	10,		1000,		60,		39 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN},
	{	"F2-15",		"F2-15",			M_NICE_F2,		M_NUMBERPARA,		&niv.F2_15,		sizeof(niv.F2_15),		0,	0,	0,	10,		1000,		30,		40 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN},
	{	"F2-16",		"F2-16",			M_NICE_F2,		M_NUMBERPARA,		&niv.F2_16,		sizeof(niv.F2_16),		0,	0,	0,	10,		1000,		60,		41 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN},
	{	"F2-17",		"F2-17",			M_NICE_F2,		M_NUMBERPARA,		&niv.F2_17,		sizeof(niv.F2_17),		0,	0,	0,	10,		1000,		30,		42 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN},
	{	"F2-18",		"F2-18",			M_NICE_F2,		M_NUMBERPARA,		&niv.F2_18,		sizeof(niv.F2_18),		0,	0,	0,	0,		500,		0,		1060 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN},
	{	"F2-19",		"F2-19",			M_NICE_F2,		M_NUMBERPARA,		&niv.F2_19,		sizeof(niv.F2_19),		0,	0,	0,	0,		60000,	350,	1061 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN},
	{	"F2-20",		"F2-20",			M_NICE_F2,		M_2BIT_DECIMAL,		&niv.F2_20,		sizeof(niv.F2_20),		0,	0,	0,	0,		4000,		0,		1784 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN},//��һ��˵����δ�ἰ����Ҫ��ͨ
	{	"F2-22",		"F2-22",			M_NICE_F2,		M_3BIT_DECIMAL,		&niv.F2_22,		sizeof(niv.F2_22),		0,	0,	0,	0,		9999,		0,		1605 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,D_RW,RIGHT_MAINTAIN},
	{ "F2-23",		"F2-23",			M_NICE_F2,		M_NUMBERPARA, 	&niv.F2_23, 	sizeof(niv.F2_23),		0,	0,	0,	100,	2000, 	600,		1606 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,D_READ,RIGHT_MAINTAIN},	//Ԥ����һ���������Ϊֻ����

//F3 parameter
	{	"F3-00",		"F3-00",			M_NICE_F3,		M_3BIT_DECIMAL,		&niv.F3_00,		sizeof(niv.F3_00),		0,	0,	0,	0,		30,			0,		46 + NICE_INDEX,		0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F3-01",		"F3-01",			M_NICE_F3,		M_3BIT_DECIMAL,		&niv.F3_01,		sizeof(niv.F3_01),		0,	0,	0,	0,		1500,		150,	47 + NICE_INDEX,		0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F3-02",		"F3-02",			M_NICE_F3,		M_3BIT_DECIMAL,		&niv.F3_02,		sizeof(niv.F3_02),		0,	0,	0,	300,	1300,		600,	48 + NICE_INDEX,		0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F3-03",		"F3-03",			M_NICE_F3,		M_3BIT_DECIMAL,		&niv.F3_03,		sizeof(niv.F3_03),		0,	0,	0,	800,	6000,		2500,	49 + NICE_INDEX,		0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F3-04",		"F3-04",			M_NICE_F3,		M_3BIT_DECIMAL,		&niv.F3_04,		sizeof(niv.F3_04),		0,	0,	0,	800,	6000,		2500,	50 + NICE_INDEX,		0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F3-05",		"F3-05",			M_NICE_F3,		M_3BIT_DECIMAL,		&niv.F3_05,		sizeof(niv.F3_05),		0,	0,	0,	300,	1300,		600,	51 + NICE_INDEX,		0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F3-06",		"F3-06",			M_NICE_F3,		M_3BIT_DECIMAL,		&niv.F3_06,		sizeof(niv.F3_06),		0,	0,	0,	800,	6000,		2500,	52 + NICE_INDEX,		0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F3-07",		"F3-07",			M_NICE_F3,		M_3BIT_DECIMAL,		&niv.F3_07,		sizeof(niv.F3_07),		0,	0,	0,	800,	6000,		2500,	53 + NICE_INDEX,		0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F3-08",		"F3-08",			M_NICE_F3,		M_3BIT_DECIMAL,		&niv.F3_08,		sizeof(niv.F3_08),		0,	0,	0,	800,	1500,		900,	54 + NICE_INDEX,		0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F3-09",		"F3-09",			M_NICE_F3,		M_1BIT_DECIMAL,		&niv.F3_09,		sizeof(niv.F3_09),		0,	0,	0,	0,		500,		0,		55 + NICE_INDEX,		0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F3-10",		"F3-10",			M_NICE_F3,		M_3BIT_DECIMAL,		&niv.F3_10,		sizeof(niv.F3_10),		0,	0,	0,	40,		80,			40,		56 + NICE_INDEX,		0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F3-11",		"F3-11",			M_NICE_F3,		M_3BIT_DECIMAL,		&niv.F3_11,		sizeof(niv.F3_11),		0,	0,	0,	80,		750,		250,	57 + NICE_INDEX,		0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F3-12",		"F3-12",			M_NICE_F3,		M_3BIT_DECIMAL,		&niv.F3_12,		sizeof(niv.F3_12),		0,	0,	0,	80,		630,		100,	58 + NICE_INDEX,		0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F3-13",		"F3-13",			M_NICE_F3,		M_3BIT_DECIMAL,		&niv.F3_13,		sizeof(niv.F3_13),		0,	0,	0,	100,	4000,		500,	59 + NICE_INDEX,		0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F3-14",		"F3-14",			M_NICE_F3,		M_NUMBERPARA,		&niv.F3_14,		sizeof(niv.F3_14),		0,	0,	0,	0,		65535,	32,		1609 + NICE_INDEX,		0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F3-15",		"F3-15",			M_NICE_F3,		M_3BIT_DECIMAL,		&niv.F3_15,		sizeof(niv.F3_15),		0,	0,	0,	80,		500,		80,		649 + NICE_INDEX,		0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F3-16",		"F3-16",			M_NICE_F3,		M_3BIT_DECIMAL,		&niv.F3_16,		sizeof(niv.F3_16),		0,	0,	0,	500,	2000,		500,	648 + NICE_INDEX,		0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},

//F4 Parameter
	{	"F4-00",		"F4-00",			M_NICE_F4,		M_NUMBERPARA,		&niv.F4_00,		sizeof(niv.F4_00),		0,	0,	0,	0,	60,		30,		65 + NICE_INDEX,		0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-01",		"F4-01",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_01,		sizeof(tiv.F4_01),		0,	0,	0,	1,	55,		1,		66 + NICE_INDEX,		0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-02",		"F4-02",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_02,		sizeof(tiv.F4_02),		0,	0,	0,	0,	65535,	0,		67 + NICE_INDEX,		0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-03",		"F4-03",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_03,		sizeof(tiv.F4_03),		0,	0,	0,	0,	65535,	0,		68 + NICE_INDEX,		0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-04",		"F4-04",			M_NICE_F4,		M_NUMBERPARA,		&niv.F4_04,		sizeof(tiv.F4_04),		0,	0,	0,	0,	65535,	0,		69 + NICE_INDEX,		0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-05",		"F4-05",			M_NICE_F4,		M_NUMBERPARA,		&niv.F4_05,		sizeof(tiv.F4_05),		0,	0,	0,	0,	65535,	0,		70 + NICE_INDEX,		0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-06",		"F4-06",			M_NICE_F4,		M_NUMBERPARA,		&niv.F4_06,		sizeof(niv.F4_06),		0,	0,	0,	0,	80,		28,		71 + NICE_INDEX,		0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-07",		"F4-07",			M_NICE_F4,		M_NUMBERPARA,		&niv.F4_07,		sizeof(niv.F4_07),		0,	0,	0,	0,	60,		30,		72 + NICE_INDEX,		0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-08",		"F4-08",			M_NICE_F4,		M_NUMBERPARA,		&niv.F4_08,		sizeof(niv.F4_08),		0,	0,	0,	0,	65535,	0,		1063 + NICE_INDEX,		0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-09",		"F4-09",			M_NICE_F4,		M_NUMBERPARA,		&niv.F4_09,		sizeof(niv.F4_09),		0,	0,	0,	0,	65535,	0,		1064 + NICE_INDEX,		0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-10",		"F4-10",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_10,		sizeof(tiv.F4_10),		0,	0,	0,	0,	65535,	0,		73 + NICE_INDEX,		0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-11",		"F4-11",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_11,		sizeof(tiv.F4_11),		0,	0,	0,	0,	65535,	0,		74 + NICE_INDEX,		0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-12",		"F4-12",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_12,		sizeof(tiv.F4_12),		0,	0,	0,	0,	65535,	0,		75 + NICE_INDEX,		0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-13",		"F4-13",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_13,		sizeof(tiv.F4_13),		0,	0,	0,	0,	65535,	0,		76 + NICE_INDEX,		0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-14",		"F4-14",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_14,		sizeof(tiv.F4_14),		0,	0,	0,	0,	65535,	0,		77 + NICE_INDEX,		0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-15",		"F4-15",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_15,		sizeof(tiv.F4_15),		0,	0,	0,	0,	65535,	0,		78 + NICE_INDEX,		0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-16",		"F4-16",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_16,		sizeof(tiv.F4_16),		0,	0,	0,	0,	65535,	0,		79 + NICE_INDEX,		0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-17",		"F4-17",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_17,		sizeof(tiv.F4_17),		0,	0,	0,	0,	65535,	0,		80 + NICE_INDEX,		0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-18",		"F4-18",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_18,		sizeof(tiv.F4_18),		0,	0,	0,	0,	65535,	0,		81 + NICE_INDEX,		0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-19",		"F4-19",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_19,		sizeof(tiv.F4_19),		0,	0,	0,	0,	65535,	0,		82 + NICE_INDEX,		0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-20",		"F4-20",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_20,		sizeof(tiv.F4_20),		0,	0,	0,	0,	65535,	0,		83 + NICE_INDEX,		0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-21",		"F4-21",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_21,		sizeof(tiv.F4_21),		0,	0,	0,	0,	65535,	0,		84 + NICE_INDEX,		0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-22",		"F4-22",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_22,		sizeof(tiv.F4_22),		0,	0,	0,	0,	65535,	0,		85 + NICE_INDEX,		0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-23",		"F4-23",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_23,		sizeof(tiv.F4_23),		0,	0,	0,	0,	65535,	0,		86 + NICE_INDEX,		0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-24",		"F4-24",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_24,		sizeof(tiv.F4_24),		0,	0,	0,	0,	65535,	0,		87 + NICE_INDEX,		0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-25",		"F4-25",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_25,		sizeof(tiv.F4_25),		0,	0,	0,	0,	65535,	0,		88 + NICE_INDEX,		0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-26",		"F4-26",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_26,		sizeof(tiv.F4_26),		0,	0,	0,	0,	65535,	0,		89 + NICE_INDEX,		0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-27",		"F4-27",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_27,		sizeof(tiv.F4_27),		0,	0,	0,	0,	65535,	0,		90 + NICE_INDEX,		0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-28",		"F4-28",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_28,		sizeof(tiv.F4_28),		0,	0,	0,	0,	65535,	0,		91 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-29",		"F4-29",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_29,		sizeof(tiv.F4_29),		0,	0,	0,	0,	65535,	0,		92 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-30",		"F4-30",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_30,		sizeof(tiv.F4_30),		0,	0,	0,	0,	65535,	0,		93 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-31",		"F4-31",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_31,		sizeof(tiv.F4_31),		0,	0,	0,	0,	65535,	0,		94 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-32",		"F4-32",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_32,		sizeof(tiv.F4_32),		0,	0,	0,	0,	65535,	0,		95 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-33",		"F4-33",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_33,		sizeof(tiv.F4_33),		0,	0,	0,	0,	65535,	0,		96 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-34",		"F4-34",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_34,		sizeof(tiv.F4_34),		0,	0,	0,	0,	65535,	0,		97 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-35",		"F4-35",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_35,		sizeof(tiv.F4_35),		0,	0,	0,	0,	65535,	0,		98 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-36",		"F4-36",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_36,		sizeof(tiv.F4_36),		0,	0,	0,	0,	65535,	0,		99 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-37",		"F4-37",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_37,		sizeof(tiv.F4_37),		0,	0,	0,	0,	65535,	0,		100 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN },
	{	"F4-38",		"F4-38",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_38,		sizeof(tiv.F4_38),		0,	0,	0,	0,	65535,	0,		101 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-39",		"F4-39",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_39,		sizeof(tiv.F4_39),		0,	0,	0,	0,	65535,	0,		102 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-40",		"F4-40",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_40,		sizeof(tiv.F4_40),		0,	0,	0,	0,	65535,	0,		103 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-41",		"F4-41",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_41,		sizeof(tiv.F4_41),		0,	0,	0,	0,	65535,	0,		104 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-42",		"F4-42",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_42,		sizeof(tiv.F4_42),		0,	0,	0,	0,	65535,	0,		105 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-43",		"F4-43",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_43,		sizeof(tiv.F4_43),		0,	0,	0,	0,	65535,	0,		106 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-44",		"F4-44",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_44,		sizeof(tiv.F4_44),		0,	0,	0,	0,	65535,	0,		107 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-45",		"F4-45",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_45,		sizeof(tiv.F4_45),		0,	0,	0,	0,	65535,	0,		108 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-46",		"F4-46",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_46,		sizeof(tiv.F4_46),		0,	0,	0,	0,	65535,	0,		109 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-47",		"F4-47",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_47,		sizeof(tiv.F4_47),		0,	0,	0,	0,	65535,	0,		110 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-48",		"F4-48",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_48,		sizeof(tiv.F4_48),		0,	0,	0,	0,	65535,	0,		111 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-49",		"F4-49",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_49,		sizeof(tiv.F4_49),		0,	0,	0,	0,	65535,	0,		112 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-50",		"F4-50",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_50,		sizeof(tiv.F4_50),		0,	0,	0,	0,	65535,	0,		113 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-51",		"F4-51",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_51,		sizeof(tiv.F4_51),		0,	0,	0,	0,	65535,	0,		114 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-52",		"F4-52",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_52,		sizeof(tiv.F4_52),		0,	0,	0,	0,	65535,	0,		115 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-53",		"F4-53",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_53,		sizeof(tiv.F4_53),		0,	0,	0,	0,	65535,	0,		116 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-54",		"F4-54",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_54,		sizeof(tiv.F4_54),		0,	0,	0,	0,	65535,	0,		117 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-55",		"F4-55",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_55,		sizeof(tiv.F4_55),		0,	0,	0,	0,	65535,	0,		118 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-56",		"F4-56",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_56,		sizeof(tiv.F4_56),		0,	0,	0,	0,	65535,	0,		119 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-57",		"F4-57",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_57,		sizeof(tiv.F4_57),		0,	0,	0,	0,	65535,	0,		120 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-58",		"F4-58",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_58,		sizeof(tiv.F4_58),		0,	0,	0,	0,	65535,	0,		121 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-59",		"F4-59",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_59,		sizeof(tiv.F4_59),		0,	0,	0,	0,	65535,	0,		122 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-60",		"F4-60",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_60,		sizeof(tiv.F4_60),		0,	0,	0,	0,	65535,	0,		123 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-61",		"F4-61",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_61,		sizeof(tiv.F4_61),		0,	0,	0,	0,	65535,	0,		124 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-62",		"F4-62",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_62,		sizeof(tiv.F4_62),		0,	0,	0,	0,	65535,	0,		125 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-63",		"F4-63",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_63,		sizeof(tiv.F4_63),		0,	0,	0,	0,	65535,	0,		126 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-64",		"F4-64",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_64,		sizeof(tiv.F4_64),		0,	0,	0,	0,	65535,	0,		127 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-65",		"F4-65",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_65,		sizeof(tiv.F4_65),		0,	0,	0,	0,	65535,	0,		128 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-66",		"F4-66",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_66,		sizeof(tiv.F4_66),		0,	0,	0,	0,	65535,	0,		129 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-67",		"F4-67",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_67,		sizeof(tiv.F4_67),		0,	0,	0,	0,	65535,	0,		130 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-68",		"F4-68",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_68,		sizeof(tiv.F4_68),		0,	0,	0,	0,	65535,	0,		131 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-69",		"F4-69",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_69,		sizeof(tiv.F4_69),		0,	0,	0,	0,	65535,	0,		132 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-70",		"F4-70",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_70,		sizeof(tiv.F4_70),		0,	0,	0,	0,	65535,	0,		133 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-71",		"F4-71",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_71,		sizeof(tiv.F4_71),		0,	0,	0,	0,	65535,	0,		134 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-72",		"F4-72",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_72,		sizeof(tiv.F4_72),		0,	0,	0,	0,	65535,	0,		135 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-73",		"F4-73",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_73,		sizeof(tiv.F4_73),		0,	0,	0,	0,	65535,	0,		136 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-74",		"F4-74",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_74,		sizeof(tiv.F4_74),		0,	0,	0,	0,	65535,	0,		137 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-75",		"F4-75",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_75,		sizeof(tiv.F4_75),		0,	0,	0,	0,	65535,	0,		138 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-76",		"F4-76",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_76,		sizeof(tiv.F4_76),		0,	0,	0,	0,	65535,	0,		139 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-77",		"F4-77",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_77,		sizeof(tiv.F4_77),		0,	0,	0,	0,	65535,	0,		140 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-78",		"F4-78",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_78,		sizeof(tiv.F4_78),		0,	0,	0,	0,	65535,	0,		141 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-79",		"F4-79",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_79,		sizeof(tiv.F4_79),		0,	0,	0,	0,	65535,	0,		142 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-80",		"F4-80",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_80,		sizeof(tiv.F4_80),		0,	0,	0,	0,	65535,	0,		143 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-81",		"F4-81",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_81,		sizeof(tiv.F4_81),		0,	0,	0,	0,	65535,	0,		144 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-82",		"F4-82",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_82,		sizeof(tiv.F4_82),		0,	0,	0,	0,	65535,	0,		145 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-83",		"F4-83",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_83,		sizeof(tiv.F4_83),		0,	0,	0,	0,	65535,	0,		146 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-84",		"F4-84",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_84,		sizeof(tiv.F4_84),		0,	0,	0,	0,	65535,	0,		147 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-85",		"F4-85",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_85,		sizeof(tiv.F4_85),		0,	0,	0,	0,	65535,	0,		148 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-86",		"F4-86",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_86,		sizeof(tiv.F4_86),		0,	0,	0,	0,	65535,	0,		149 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-87",		"F4-87",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_87,		sizeof(tiv.F4_87),		0,	0,	0,	0,	65535,	0,		150 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-88",		"F4-88",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_88,		sizeof(tiv.F4_88),		0,	0,	0,	0,	65535,	0,		151 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-89",		"F4-89",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_89,		sizeof(tiv.F4_89),		0,	0,	0,	0,	65535,	0,		152 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-90",		"F4-90",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_90,		sizeof(tiv.F4_90),		0,	0,	0,	0,	65535,	0,		153 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-91",		"F4-91",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_91,		sizeof(tiv.F4_91),		0,	0,	0,	0,	65535,	0,		154 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-92",		"F4-92",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_92,		sizeof(tiv.F4_92),		0,	0,	0,	0,	65535,	0,		155 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-93",		"F4-93",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_93,		sizeof(tiv.F4_93),		0,	0,	0,	0,	65535,	0,		156 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-94",		"F4-94",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_94,		sizeof(tiv.F4_94),		0,	0,	0,	0,	65535,	0,		157 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-95",		"F4-95",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_95,		sizeof(tiv.F4_95),		0,	0,	0,	0,	65535,	0,		158 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-96",		"F4-96",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_96,		sizeof(tiv.F4_96),		0,	0,	0,	0,	65535,	0,		159 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-97",		"F4-97",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_97,		sizeof(tiv.F4_97),		0,	0,	0,	0,	65535,	0,		160 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-98",		"F4-98",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_98,		sizeof(tiv.F4_98),		0,	0,	0,	0,	65535,	0,		161 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-99",		"F4-99",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_99,		sizeof(tiv.F4_99),		0,	0,	0,	0,	65535,	0,		162 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-100",		"F4-100",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_100,	sizeof(tiv.F4_100),		0,	0,	0,	0,	65535,	0,		163 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-101",		"F4-101",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_101,	sizeof(tiv.F4_101),		0,	0,	0,	0,	65535,	0,		164 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-102",		"F4-102",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_102,	sizeof(tiv.F4_102),		0,	0,	0,	0,	65535,	0,		165 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-103",		"F4-103",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_103,	sizeof(tiv.F4_103),		0,	0,	0,	0,	65535,	0,		166 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-104",		"F4-104",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_104,	sizeof(tiv.F4_104),		0,	0,	0,	0,	65535,	0,		167 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-105",		"F4-105",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_105,	sizeof(tiv.F4_105),		0,	0,	0,	0,	65535,	0,		168 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-106",		"F4-106",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_106,	sizeof(tiv.F4_106),		0,	0,	0,	0,	65535,	0,		169 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-107",		"F4-107",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_107,	sizeof(tiv.F4_107),		0,	0,	0,	0,	65535,	0,		170 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-108",		"F4-108",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_108,	sizeof(tiv.F4_108),		0,	0,	0,	0,	65535,	0,		171 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-109",		"F4-109",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_109,	sizeof(tiv.F4_109),		0,	0,	0,	0,	65535,	0,		172 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-110",		"F4-110",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_110,	sizeof(tiv.F4_110),		0,	0,	0,	0,	65535,	0,		173 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-111",		"F4-111",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_111,	sizeof(tiv.F4_111),		0,	0,	0,	0,	65535,	0,		174 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-112",		"F4-112",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_112,	sizeof(tiv.F4_112),		0,	0,	0,	0,	65535,	0,		175 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-113",		"F4-113",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_113,	sizeof(tiv.F4_113),		0,	0,	0,	0,	65535,	0,		176 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-114",		"F4-114",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_114,	sizeof(tiv.F4_114),		0,	0,	0,	0,	65535,	0,		177 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-115",		"F4-115",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_115,	sizeof(tiv.F4_115),		0,	0,	0,	0,	65535,	0,		178 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-116",		"F4-116",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_116,	sizeof(tiv.F4_116),		0,	0,	0,	0,	65535,	0,		179 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-117",		"F4-117",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_117,	sizeof(tiv.F4_117),		0,	0,	0,	0,	65535,	0,		180 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-118",		"F4-118",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_118,	sizeof(tiv.F4_118),		0,	0,	0,	0,	65535,	0,		181 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F4-119",		"F4-119",			M_NICE_F4,		M_NUMBERPARA,		&tiv.F4_119,	sizeof(tiv.F4_119),		0,	0,	0,	0,	65535,	0,		182 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},

//F5 Parameter
	{	"F5-01",		"F5-01",			M_NICE_F5,		M_NUMBERPARA,		&p.F5_X1,		sizeof(p.F5_X1),		0,	0,	0,	0,	255,		1,		184 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F5-02",		"F5-02",			M_NICE_F5,		M_NUMBERPARA,		&p.F5_X2,		sizeof(p.F5_X2),		0,	0,	0,	0,	255,		2,		185 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F5-03",		"F5-03",			M_NICE_F5,		M_NUMBERPARA,		&p.F5_X3,		sizeof(p.F5_X3),		0,	0,	0,	0,	255,		16,		186	+ NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F5-04",		"F5-04",			M_NICE_F5,		M_NUMBERPARA,		&p.F5_X4,		sizeof(p.F5_X4),		0,	0,	0,	0,	255,		17,		187 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F5-05",		"F5-05",			M_NICE_F5,		M_NUMBERPARA,		&p.F5_X5,		sizeof(p.F5_X5),		0,	0,	0,	0,	255,		18,		188 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F5-06",		"F5-06",			M_NICE_F5,		M_NUMBERPARA,		&p.F5_X6,		sizeof(p.F5_X6),		0,	0,	0,	0,	255,		19,		189 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F5-07",		"F5-07",			M_NICE_F5,		M_NUMBERPARA,		&p.F5_X7,		sizeof(p.F5_X7),		0,	0,	0,	0,	255,		0,		190 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"F5-08",		"F5-08",			M_NICE_F5,		M_NUMBERPARA,		&p.F5_X8,		sizeof(p.F5_X8),		0,	0,	0,	0,	255,		0,		191 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},

//FA Parameter
	{	"FA-06",		"FA-06",			M_NICE_FA,		M_NUMBERPARA,		&niv.FA_06,		sizeof(niv.FA_06),		0,	0,	0,	0,	65535,		0,	323 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_READ,		RIGHT_MAINTAIN},
	{	"FA-07",		"FA-07",			M_NICE_FA,		M_NUMBERPARA,		&niv.FA_07,		sizeof(niv.FA_07),		0,	0,	0,	0,	100,		0,		324 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_READ,		RIGHT_MAINTAIN},
	{	"FA-08",		"FA-08",			M_NICE_FA,		M_NUMBERPARA,		&niv.FA_08,		sizeof(niv.FA_08),		0,	0,	0,	0,	0,			0,		1201 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_READ,		RIGHT_MAINTAIN},	//��һ��˵����δ�ἰ����Ҫ��ͨ
	{	"FA-09",		"FA-09",			M_NICE_FA,		M_NUMBERPARA,		&niv.FA_09,		sizeof(niv.FA_09),		0,	0,	0,	0,	65535,		3,		325 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FA-10",		"FA-10",			M_NICE_FA,		M_2BIT_DECIMAL,		&niv.FA_10,		sizeof(niv.FA_10),		0,	0,	0,	50,	1000,		100,	326 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FA-11",		"FA-11",			M_NICE_FA,		M_NUMBERPARA,		&niv.FA_11,		sizeof(niv.FA_11),		0,	0,	0,	50,	100,		80,		327 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FA-12",		"FA-12",			M_NICE_FA,		M_NUMBERPARA,		&niv.FA_12,		sizeof(niv.FA_12),		0,	0,	0,	0,	0xFFFF,		0,		328 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},	//��һ��˵����δ�ἰ����Ҫ��ͨ
	{	"FA-13",		"FA-13",			M_NICE_FA,		M_NUMBERPARA,		&niv.FA_13,		sizeof(niv.FA_13),		0,	0,	0,	0,	0,			0,		329 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},	//��һ��˵����δ�ἰ����Ҫ��ͨ
	{	"FA-14",		"FA-14",			M_NICE_FA,		M_NUMBERPARA,		&niv.FA_14,		sizeof(niv.FA_14),		0,	0,	0,	0,	0,			0,		1202 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FA-15",		"FA-15",			M_NICE_FA,		M_NUMBERPARA,		&niv.FA_15,		sizeof(niv.FA_15),		0,	0,	0,	0,	0xFFFF,		0,		1777 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},	//��һ��˵����δ�ἰ����Ҫ��ͨ
	{	"FA-16",		"FA-16",			M_NICE_FA,		M_NUMBERPARA,		&niv.FA_16, 	sizeof(niv.FA_16),		0,	0,	0,	0,	0xFFFF, 	0,		1778 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},	//��һ��˵����δ�ἰ����Ҫ��ͨ
	{	"FA-17",		"FA-17",			M_NICE_FA,		M_NUMBERPARA,		&niv.FA_17, 	sizeof(niv.FA_17),		0,	0,	0,	0,	0xFFFF, 	0,		1779 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},	//��һ��˵����δ�ἰ����Ҫ��ͨ
	{	"FA-18",		"FA-18",			M_NICE_FA,		M_NUMBERPARA,		&niv.FA_18, 	sizeof(niv.FA_18),		0,	0,	0,	0,	0xFFFF, 	0,		1780 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},	//��һ��˵����δ�ἰ����Ҫ��ͨ
	{	"FA-19",		"FA-19",			M_NICE_FA,		M_NUMBERPARA,		&niv.FA_19, 	sizeof(niv.FA_19),		0,	0,	0,	0,	0xFFFF, 	0,		1781 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},	//��һ��˵����δ�ἰ����Ҫ��ͨ
	{	"FA-20",		"FA-20",			M_NICE_FA,		M_NUMBERPARA,		&niv.FA_20, 	sizeof(niv.FA_20),		0,	0,	0,	0,	0xFFFF, 	0,		1782 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},	//��һ��˵����δ�ἰ����Ҫ��ͨ
	{	"FA-21",		"FA-21",			M_NICE_FA,		M_NUMBERPARA,		&niv.FA_21, 	sizeof(niv.FA_21),		0,	0,	0,	0,	0xFFFF, 	0,		1783 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},	//��һ��˵����δ�ἰ����Ҫ��ͨ
	{	"FA-22",		"FA-22",			M_NICE_FA,		M_NUMBERPARA,		&niv.FA_22, 	sizeof(niv.FA_22),		0,	0,	0,	0,	0xFFFF, 	0,		1062 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},	//��һ��˵����δ�ἰ����Ҫ��ͨ
	{	"FA-23",		"FA-23",			M_NICE_FA,		M_NUMBERPARA,		&niv.FA_23, 	sizeof(niv.FA_23),		0,	0,	0,	0,	0xFFFF, 	0,		1785 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},	//��һ��˵����δ�ἰ����Ҫ��ͨ
	{	"FA-24",		"FA-24",			M_NICE_FA,		M_NUMBERPARA,		&niv.FA_24, 	sizeof(niv.FA_24),		0,	0,	0,	0,	0xFFFF, 	0,		1786 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},	//��һ��˵����δ�ἰ����Ҫ��ͨ
	{	"FA-25",		"FA-25",			M_NICE_FA,		M_NUMBERPARA,		&niv.FA_25, 	sizeof(niv.FA_25),		0,	0,	0,	0,	0xFFFF, 	0,		1787 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},	//��һ��˵����δ�ἰ����Ҫ��ͨ
	{	"FA-26",		"FA-26",			M_NICE_FA,		M_NUMBERPARA,		&niv.FA_26, 	sizeof(niv.FA_26),		0,	0,	0,	0,	0xFFFF, 	0,		1788 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},	//��һ��˵����δ�ἰ����Ҫ��ͨ
	{	"FA-27",		"FA-27",			M_NICE_FA,		M_NUMBERPARA,		&niv.FA_27, 	sizeof(niv.FA_27),		0,	0,	0,	0,	0xFFFF, 	0,		1789 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},	//��һ��˵����δ�ἰ����Ҫ��ͨ
	{	"FA-28",		"FA-28",			M_NICE_FA,		M_NUMBERPARA,		&niv.FA_28, 	sizeof(niv.FA_28),		0,	0,	0,	0,	0xFFFF, 	0,		1790 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},	//��һ��˵����δ�ἰ����Ҫ��ͨ
	{	"FA-29",		"FA-29",			M_NICE_FA,		M_NUMBERPARA,		&niv.FA_29, 	sizeof(niv.FA_29),		0,	0,	0,	0,	0xFFFF, 	0,		1791 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},	//��һ��˵����δ�ἰ����Ҫ��ͨ
	{	"FA-30",		"FA-30",			M_NICE_FA,		M_NUMBERPARA,		&niv.FA_30, 	sizeof(niv.FA_30),		0,	0,	0,	0,	0xFFFF, 	0,		1792 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},	//��һ��˵����δ�ἰ����Ҫ��ͨ
	{ "FA-31",		"FA-31",			M_NICE_FA,		M_NUMBERPARA, 	&niv.FA_31, 	sizeof(niv.FA_31),		0,	0,	0,	0,	0xFFFF, 	0,		1793 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16, 	D_RW, 	RIGHT_MAINTAIN},	//��һ��˵����δ�ἰ����Ҫ��ͨ
	{ "FA-32",		"FA-32",			M_NICE_FA,		M_NUMBERPARA, 	&niv.FA_32, 	sizeof(niv.FA_32),		0,	0,	0,	0,	0xFFFF, 	0,		1794 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16, 	D_RW, 	RIGHT_MAINTAIN},	//��һ��˵����δ�ἰ����Ҫ��ͨ
	{ "FA-33",		"FA-33",			M_NICE_FA,		M_NUMBERPARA, 	&niv.FA_33, 	sizeof(niv.FA_33),		0,	0,	0,	0,	0xFFFF, 	0,		1795 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16, 	D_RW, 	RIGHT_MAINTAIN},	//��һ��˵����δ�ἰ����Ҫ��ͨ
	{ "FA-34",		"FA-34",			M_NICE_FA,		M_NUMBERPARA, 	&niv.FA_34, 	sizeof(niv.FA_34),		0,	0,	0,	0,	0xFFFF, 	0,		1796 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16, 	D_RW, 	RIGHT_MAINTAIN},	//��һ��˵����δ�ἰ����Ҫ��ͨ
	{ "FA-35",		"FA-35",			M_NICE_FA,		M_NUMBERPARA, 	&niv.FA_35, 	sizeof(niv.FA_35),		0,	0,	0,	0,	10, 	5,		1797 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16, 	D_RW, 	RIGHT_MAINTAIN},	//��һ��˵����δ�ἰ����Ҫ��ͨ
	{ "FA-36",		"FA-36",			M_NICE_FA,		M_NUMBERPARA, 	&niv.FA_36, 	sizeof(niv.FA_36),		0,	0,	0,	0,	150, 	100,	1798 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16, 	D_RW, 	RIGHT_MAINTAIN},	//��һ��˵����δ�ἰ����Ҫ��ͨ
	{ "FA-37",		"FA-37",			M_NICE_FA,		M_NUMBERPARA, 	&niv.FA_37, 	sizeof(niv.FA_37),		0,	0,	0,	0,	20, 	5,		1799 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16, 	D_RW, 	RIGHT_MAINTAIN},	//��һ��˵����δ�ἰ����Ҫ��ͨ
	{ "FA-38",		"FA-38",			M_NICE_FA,		M_NUMBERPARA, 	&niv.FA_38, 	sizeof(niv.FA_38),		0,	0,	0,	0,	20, 	5,		1800 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16, 	D_RW, 	RIGHT_MAINTAIN},	//��һ��˵����δ�ἰ����Ҫ��ͨ
	{ "FA-39",		"FA-39",			M_NICE_FA,		M_NUMBERPARA, 	&niv.FA_39, 	sizeof(niv.FA_39),		0,	0,	0,	0,	0xFFFF, 0,	1801 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16, 	D_RW, 	RIGHT_MAINTAIN},	//��һ��˵����δ�ἰ����Ҫ��ͨ
	{ "FA-40",		"FA-40",			M_NICE_FA,		M_NUMBERPARA, 	&niv.FA_40, 	sizeof(niv.FA_40),		0,	0,	0,	0,	0xFFFF, 0,	1802 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16, 	D_RW, 	RIGHT_MAINTAIN},	//��һ��˵����δ�ἰ����Ҫ��ͨ

//FH Parameter
	{	"FH-01",		"FH-01",			M_NICE_FH,		M_NUMBERPARA,		&niv.FH_01,		sizeof(niv.FH_01),		0,	0,	0,	0,	4,			0,		432 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FH-02",		"FH-02",			M_NICE_FH,		M_NUMBERPARA,		&niv.FH_02,		sizeof(niv.FH_02),		0,	0,	0,	0,	10000,		2048,	433 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FH-03",		"FH-03",			M_NICE_FH,		M_1BIT_DECIMAL,		&niv.FH_03,		sizeof(niv.FH_03),		0,	0,	0,	0,	100,		21,		434 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FH-04",		"FH-04",			M_NICE_FH,		M_1BIT_DECIMAL,		&niv.FH_04,		sizeof(niv.FH_04),		0,	0,	0,	0,	3599,		0,		438 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FH-05",		"FH-05",			M_NICE_FH,		M_1BIT_DECIMAL,		&niv.FH_05,		sizeof(niv.FH_05),		0,	0,	0,	0,	3599,		0,		439 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FH-06",		"FH-06",			M_NICE_FH,		M_NUMBERPARA,		&niv.FH_06,		sizeof(niv.FH_06),		0,	0,	0,	0,	15,			0,		437 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FH-07",		"FH-07",			M_NICE_FH,		M_NUMBERPARA,		&niv.FH_07,		sizeof(niv.FH_07),		0,	0,	0,	0,	0,			0,		440 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_READ,		RIGHT_MAINTAIN},//��һ��˵����δ�ἰ����Ҫ��ͨ
	{	"FH-08",		"FH-08",			M_NICE_FH,		M_NUMBERPARA,		&niv.FH_08,		sizeof(niv.FH_08),		0,	0,	0,	0,	65535,		0,		441 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FH-09",		"FH-09",			M_NICE_FH,		M_NUMBERPARA,		&niv.FH_09,		sizeof(niv.FH_09),		0,	0,	0,	0,	65535,		0,		442 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FH-10",		"FH-10",			M_NICE_FH,		M_NUMBERPARA,		&niv.FH_10,		sizeof(niv.FH_10),		0,	0,	0,	0,	65535,		0,		443 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FH-11",		"FH-11",			M_NICE_FH,		M_NUMBERPARA,		&niv.FH_11,		sizeof(niv.FH_11),		0,	0,	0,	0,	65535,		0,		444 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FH-12",		"FH-12",			M_NICE_FH,		M_NUMBERPARA,		&niv.FH_12,		sizeof(niv.FH_12),		0,	0,	0,	1,	10,			1,			445 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FH-17",		"FH-17",			M_NICE_FH,		M_2BIT_DECIMAL,		&niv.FH_17,		sizeof(niv.FH_17),		0,	0,	0,	0,	30000,		0,		446 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FH-18",		"FH-18",			M_NICE_FH,		M_2BIT_DECIMAL,		&niv.FH_18,		sizeof(niv.FH_18),		0,	0,	0,	0,	30000,		0,		447 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FH-19",		"FH-19",			M_NICE_FH,		M_2BIT_DECIMAL,		&niv.FH_19,		sizeof(niv.FH_19),		0,	0,	0,	0,	30000,		0,		448 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FH-20",		"FH-20",			M_NICE_FH,		M_2BIT_DECIMAL,		&niv.FH_20,		sizeof(niv.FH_20),		0,	0,	0,	0,	30000,		0,		449 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FH-21",		"FH-21",			M_NICE_FH,		M_2BIT_DECIMAL,		&niv.FH_21,		sizeof(niv.FH_21),		0,	0,	0,	0,	30000,		0,		450 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FH-22",		"FH-22",			M_NICE_FH,		M_2BIT_DECIMAL,		&niv.FH_22,		sizeof(niv.FH_22),		0,	0,	0,	0,	30000,		0,		451 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FH-23",		"FH-23",			M_NICE_FH,		M_2BIT_DECIMAL,		&niv.FH_23,		sizeof(niv.FH_23),		0,	0,	0,	0,	30000,		0,		452 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FH-24",		"FH-24",			M_NICE_FH,		M_2BIT_DECIMAL,		&niv.FH_24,		sizeof(niv.FH_24),		0,	0,	0,	0,	30000,		0,		453 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FH-25",		"FH-25",			M_NICE_FH,		M_2BIT_DECIMAL,		&niv.FH_25,		sizeof(niv.FH_25),		0,	0,	0,	0,	30000,		0,		454 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FH-26",		"FH-26",			M_NICE_FH,		M_2BIT_DECIMAL,		&niv.FH_26,		sizeof(niv.FH_26),		0,	0,	0,	0,	30000,		0,		455 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FH-27",		"FH-27",			M_NICE_FH,		M_2BIT_DECIMAL,		&niv.FH_27,		sizeof(niv.FH_27),		0,	0,	0,	0,	30000,		0,		456 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FH-28",		"FH-28",			M_NICE_FH,		M_2BIT_DECIMAL,		&niv.FH_28,		sizeof(niv.FH_28),		0,	0,	0,	0,	30000,		0,		457 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FH-29",		"FH-29",			M_NICE_FH,		M_NUMBERPARA,		&niv.FH_29,		sizeof(niv.FH_29),		0,	0,	0,	0,	200,		0,			458 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},

//FF Parameter	Ϊֻ������
	{	"FF-00",		"FF-00",			M_NICE_FF,		M_NUMBERPARA,		&niv.FF_00,		sizeof(niv.FF_00),		0,	0,	0,	0,	0xFFFF,		0,		548 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FF-01",		"FF-01",			M_NICE_FF,		M_NUMBERPARA,		&niv.FF_01,		sizeof(niv.FF_01),		0,	0,	0,	0,	0xFFFF,		0,		549 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FF-02",		"FF-02",			M_NICE_FF,		M_NUMBERPARA,		&niv.FF_02,		sizeof(niv.FF_02),		0,	0,	0,	0,	0xFFFF,		0,		550 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FF-03",		"FF-03",			M_NICE_FF,		M_NUMBERPARA,		&niv.FF_03,		sizeof(niv.FF_03),		0,	0,	0,	0,	0xFFFF,		0,		551 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FF-05",		"FF-05",			M_NICE_FF,		M_NUMBERPARA,		&niv.FF_05,		sizeof(niv.FF_05),		0,	0,	0,	0,	0xFFFF,		0,		552 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FF-06",		"FF-06",			M_NICE_FF,		M_NUMBERPARA,		&niv.FF_06,		sizeof(niv.FF_06),		0,	0,	0,	0,	0xFFFF,		0,		553 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FF-07",		"FF-07",			M_NICE_FF,		M_NUMBERPARA,		&niv.FF_07,		sizeof(niv.FF_07),		0,	0,	0,	0,	0xFFFF,		0,		554 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},//��һ��˵����δ�ἰ����Ҫ��ͨ
	{	"FF-08",		"FF-08",			M_NICE_FF,		M_NUMBERPARA,		&niv.FF_08,		sizeof(niv.FF_08),		0,	0,	0,	0,	0xFFFF,		0,		555 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FF-09",		"FF-09",			M_NICE_FF,		M_NUMBERPARA,		&niv.FF_09,		sizeof(niv.FF_09),		0,	0,	0,	0,	0xFFFF,		0,		556 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FF-10",		"FF-10",			M_NICE_FF,		M_NUMBERPARA,		&niv.FF_10,		sizeof(niv.FF_10),		0,	0,	0,	0,	0xFFFF,		0,		1937 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FF-11",		"FF-11",			M_NICE_FF,		M_NUMBERPARA,		&niv.FF_11,		sizeof(niv.FF_11),		0,	0,	0,	0,	0xFFFF,		0,		1938 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},

//	{	"FR-00",		"FR-00",			M_NICE_FR,		M_NUMBERPARA,		&landings_off,		sizeof(landings_off),		0,	0,	0,	0,	1,			0,		LANDINGS_OFF,	0,	D_VAR,	D_UNSIGNED8,		D_RW,		RIGHT_MAINTAIN},
	{	"FR-00",		"FR-00",			M_NICE_FR,		M_NUMBERPARA,		&niv.FR_00,		sizeof(niv.FR_00),		0,	0,	0,	0,	1,			0,			1181 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{ "FR-01",		"FR-01",			M_NICE_FR,		M_FLOOR_LEVEL,		&niv.FR_01, 	sizeof(niv.FR_01),		0,	0,	0,	0,	65535,		0,		1182 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16, 	D_RW, 	RIGHT_MAINTAIN},
	{	"FR-02",		"FR-02",			M_NICE_FR,		M_FLOOR_LEVEL,		&niv.FR_02,		sizeof(niv.FR_02),		0,	0,	0,	0,	65535,		0,		1183 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FR-03",		"FR-03",			M_NICE_FR,		M_FLOOR_LEVEL,		&niv.FR_03,		sizeof(niv.FR_03),		0,	0,	0,	0,	65535,		0,		1184 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FR-04",		"FR-04",			M_NICE_FR,		M_FLOOR_LEVEL,		&niv.FR_04,		sizeof(niv.FR_04),		0,	0,	0,	0,	65535,		0,		1185 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FR-05",		"FR-05",			M_NICE_FR,		M_FLOOR_LEVEL,		&niv.FR_05,		sizeof(niv.FR_05),		0,	0,	0,	0,	65535,		0,		1186 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FR-06",		"FR-06",			M_NICE_FR,		M_FLOOR_LEVEL,		&niv.FR_06,		sizeof(niv.FR_06),		0,	0,	0,	0,	65535,		0,		1187 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FR-07",		"FR-07",			M_NICE_FR,		M_FLOOR_LEVEL,		&niv.FR_07,		sizeof(niv.FR_07),		0,	0,	0,	0,	65535,		0,		1188 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FR-08",		"FR-08",			M_NICE_FR,		M_FLOOR_LEVEL,		&niv.FR_08,		sizeof(niv.FR_08),		0,	0,	0,	0,	65535,		0,		1189 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FR-09",		"FR-09",			M_NICE_FR,		M_FLOOR_LEVEL,		&niv.FR_09,		sizeof(niv.FR_09),		0,	0,	0,	0,	65535,		0,		1190 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FR-10",		"FR-10",			M_NICE_FR,		M_FLOOR_LEVEL,		&niv.FR_10,		sizeof(niv.FR_10),		0,	0,	0,	0,	65535,		0,		1191 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FR-11",		"FR-11",			M_NICE_FR,		M_FLOOR_LEVEL,		&niv.FR_11,		sizeof(niv.FR_11),		0,	0,	0,	0,	65535,		0,		1907 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FR-12",		"FR-12",			M_NICE_FR,		M_FLOOR_LEVEL,		&niv.FR_12,		sizeof(niv.FR_12),		0,	0,	0,	0,	65535,		0,		1908 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FR-13",		"FR-13",			M_NICE_FR,		M_FLOOR_LEVEL,		&niv.FR_13, 	sizeof(niv.FR_13),		0,	0,	0,	0,	65535,		0,		1909 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FR-14",		"FR-14",			M_NICE_FR,		M_FLOOR_LEVEL,		&niv.FR_14, 	sizeof(niv.FR_14),		0,	0,	0,	0,	65535,		0,		1910 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FR-15",		"FR-15",			M_NICE_FR,		M_FLOOR_LEVEL,		&niv.FR_15, 	sizeof(niv.FR_15),		0,	0,	0,	0,	65535,		0,		1911 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FR-16",		"FR-16",			M_NICE_FR,		M_FLOOR_LEVEL,		&niv.FR_16, 	sizeof(niv.FR_16),		0,	0,	0,	0,	65535, 		0,		1912 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FR-17",		"FR-17",			M_NICE_FR,		M_FLOOR_LEVEL,		&niv.FR_17,		sizeof(niv.FR_17),		0,	0,	0,	0,	65535,		0,		1913 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FR-18",		"FR-18",			M_NICE_FR,		M_FLOOR_LEVEL,		&niv.FR_18,		sizeof(niv.FR_18),		0,	0,	0,	0,	65535,		0,		1914 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FR-19",		"FR-19",			M_NICE_FR,		M_FLOOR_LEVEL,		&niv.FR_19,		sizeof(niv.FR_19),		0,	0,	0,	0,	65535,		0,		1915 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FR-20",		"FR-20",			M_NICE_FR,		M_FLOOR_LEVEL,		&niv.FR_20,		sizeof(niv.FR_20),		0,	0,	0,	0,	65535,		0,		1916 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FR-21",		"FR-21",			M_NICE_FR,		M_FLOOR_LEVEL,		&niv.FR_21,		sizeof(niv.FR_21),		0,	0,	0,	0,	65535,		0,		1917 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FR-22",		"FR-22",			M_NICE_FR,		M_FLOOR_LEVEL,		&niv.FR_22,		sizeof(niv.FR_22),		0,	0,	0,	0,	65535,		0,		1918 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FR-23",		"FR-23",			M_NICE_FR,		M_FLOOR_LEVEL,		&niv.FR_23,		sizeof(niv.FR_23),		0,	0,	0,	0,	65535,		0,		1919 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FR-24",		"FR-24",			M_NICE_FR,		M_FLOOR_LEVEL,		&niv.FR_24,		sizeof(niv.FR_24),		0,	0,	0,	0,	65535,		0,		1920 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FR-25",		"FR-25",			M_NICE_FR,		M_FLOOR_LEVEL,		&niv.FR_25,		sizeof(niv.FR_25),		0,	0,	0,	0,	65535,		0,		1921 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FR-26",		"FR-26",			M_NICE_FR,		M_FLOOR_LEVEL,		&niv.FR_26,		sizeof(niv.FR_26),		0,	0,	0,	0,	65535,		0,		1922 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FR-27",		"FR-27",			M_NICE_FR,		M_FLOOR_LEVEL,		&niv.FR_27,		sizeof(niv.FR_27),		0,	0,	0,	0,	65535,		0,		1923 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},
	{	"FR-28",		"FR-28",			M_NICE_FR,		M_FLOOR_LEVEL,		&niv.FR_28,		sizeof(niv.FR_28),		0,	0,	0,	0,	65535,		0,		1924 + NICE_INDEX,	0,	D_VAR,	D_UNSIGNED16,		D_RW,		RIGHT_MAINTAIN},

	{ "Floor",	 "Floor",			M_FLOORCOUNT,	M_NUMBERPARA,		&floorcount[0],					sizeof(floorcount[0]),					1,	0,	0,	0,	0xFFFFFFFE,	0,FLOORCOUNT,MAX_FLOOR,D_ARRAY,D_UNSIGNED32,D_READ,RIGHT_INSTALL	},
	{ "Floor"		,"Floor"			,M_LEVEL_HEIGHT ,M_MAIN	,&floor_higth[0] 				,sizeof (floor_higth[0])					,1		,0				,0						,0				,0xFFFFFFFE 		,0			,LEVEL					,MAX_FLOOR			,D_ARRAY	,D_UNSIGNED16 ,D_RW 	,RIGHT_INSTALL	},
	{ "CanA"		,"CanA"			,M_CAN_DISTURB,M_MAIN	,&can_disturb[0] 				,sizeof (can_disturb[0])		,0		,0	,0	,0	,0xFF ,0	,NO_OBJECT	,0xFF			,D_VAR	,D_UNSIGNED8 ,D_READ 	,RIGHT_INSTALL	},
	{ "CanB"		,"CanB"			,M_CAN_DISTURB,M_MAIN	,&can_disturb[1] 				,sizeof (can_disturb[1])		,0		,0	,0	,0	,0xFF ,0	,NO_OBJECT	,0xFF			,D_VAR	,D_UNSIGNED8 ,D_READ 	,RIGHT_INSTALL	},
	{ "CanC"		,"CanC"			,M_CAN_DISTURB,M_MAIN	,&can_disturb[2] 				,sizeof (can_disturb[2])		,0		,0	,0	,0	,0xFF ,0	,NO_OBJECT	,0xFF			,D_VAR	,D_UNSIGNED8 ,D_READ 	,RIGHT_INSTALL	},


	{  "Device type",	"Device type",		0,				0,					(void *)&device_type,			sizeof(device_type),					0,	0,	0,	0,	0xFFFFFFFF,	0,DEVICE_TYPE,	0,	D_VAR,	D_UNSIGNED32,D_READ,RIGHT_HELP		},
	{  "Virtual device","Virtual device",	0,				0,					(void *)virtual_device,			sizeof(virtual_device[0]),				0,	0,	0,	0,	0xFFFFFFFF,	0,VIRTUAL_DEVICE,7,D_ARRAY,D_UNSIGNED16,D_READ,RIGHT_HELP		},
	{  "Device name",	"Device name",		0,				0,					(void *)device_name,			127,									0,	0,	0,	0,	0,	0,DEVICE_NAME,	0,	D_VAR,	D_VIS_STRING,D_READ,RIGHT_HELP		},
	{  "HW-Version",	"HW-Version",		0,				0,					(void *)hardware_hse,			127,									0,	0,	0,	0,	0,	0,HW_VERSION,	0,	D_VAR,	D_VIS_STRING,D_READ,RIGHT_HELP		},
	{  "SW-Version",	"SW-Version",		0,				0,					(void *)version_hse,			127,									0,	0,	0,	0,	0,	0,SW_VERSION,	0,	D_VAR,	D_VIS_STRING,D_READ,RIGHT_HELP		},
	{  "Identity",		"Identity",			0,				0,					(void *)identity,				sizeof(identity[0]),					0,	0,	0,	0,	0xFFFFFFFF,	0,IDENTITY,4,D_ARRAY,D_IDENTITY,D_READ,RIGHT_HELP		},
	{  "Act.floor",		"Act.floor",		0,				M_FLOORPARA,		&level,							sizeof(level),							0,	0,	0,	0,MAX_FLOOR-1,	0,FLOOR,	0,	D_VAR,	D_UNSIGNED8,D_READ,RIGHT_HELP		},
	{  "Inputs HSE",	"Inputs HSE",		0,				0,					&in,							sizeof(in),								0,	0,	0,	0,	0xFFFF,	0,PORT_IN,	0,	D_VAR,	D_UNSIGNED16,D_READ,RIGHT_HELP		},
	{  "Outputs HSE",	"Outputs HSE",		0,				0,					&out,							sizeof(out),							0,	0,	0,	0,	0xFF,0xFF,PORT_OUT,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_HELP		},
	{  "Save parameter","Save parameter",	0,				0,					&savepara,						sizeof(savepara),						0,	0,	0,	0,	1,	0,SAVEPARA,	0,	D_VAR,	D_UNSIGNED8,D_WRITE,RIGHT_HELP		},
	{  "Current AD",	"Current AD",		0,				0,					&ldvalue,						sizeof(ldvalue),						0,	0,	0,	0,	0xFFFF,	0,LOAD_AD_VALUE,	0,	D_VAR,	D_UNSIGNED16,D_READ,RIGHT_INSTALL	},
	{  "DoorZone",		"DoorZone",			0,				0,					&dz_input,						sizeof(dz_input),						0,	0,	0,	0,	1,	0,DOORZONEIN,	0,	D_VAR,	D_UNSIGNED8,D_READ,RIGHT_INSTALL	},
	{  "TestSRU",		"TestSRU",			0,				0,					&bTestUCMP,						sizeof(bTestUCMP),						0,	0,	0,	0,	1,	0,TEST_SRU,	0,	D_VAR,	D_UNSIGNED8,D_RW,RIGHT_INSTALL	},

	{  "--- ESC ---",	"-- ESC --",		ENDSTRING,		0,					&dummy,							0,										0,	0,	0,	0,	1,	0,	0,	0,	0,	0,	0,RIGHT_MAINTAIN	},
};

const struct _textlist parkmode[] = {
	{	"opened",		"opened"		},
	{	"reclose",		"reclose"		},
	{	"",				""				},
};

const struct _textlist openallow[] = {
	{	"allow",		"allow"			},
	{	"forbid",		"forbid"		},
	{	"",				""				},
};

const struct _textlist no_yes[] = {
	{	"no",			"no"			},
	{	"yes",			"yes"			},
	{	"",				""				},
};

const struct _textlist postype[] = {
	{	"MSI only",		"MSI only"		},
	{	"MSI+ULSI",		"MSI+ULSI"		},
	{	"",				""				},
};

const struct _textlist loadtype[] = {
	{ "An.CDU", 	"An.CDU"		},
	{ "Dig.CDU",	"Dig.CDU" 		},
	{	"An.MCU",		"An.MCU"		},
	{	"Dig.MCU",	"Dig.MCU"		},
	{	"Others",		"Others"			},
	{ "None", 		"None"		},
	{	"",				""				},
};

const struct _textlist arrowtype[] = {
	{	"Scroll",		"Scroll"		},
	{	"Flash",		"Flash"			},
	{	"Static",		"Static"		},
	{	"",				""				},
};

const struct _textlist weekday[] = {
	{	"Mon",			"Mon",			},
	{	"Tue",			"Tue",			},
	{	"Wed",			"Wed",			},
	{	"Thu",			"Thu",			},
	{	"Fri",			"Fri",			},
	{	"Sat",			"Sat",			},
	{	"Sun",			"Sun",			},
	{	"",				""				},
};

const struct _textlist month[] = {
	{	"Jan",			"Jan",			},
	{	"Feb",			"Feb",			},
	{	"Mar",			"Mar",			},
	{	"Apr",			"Apr",			},
	{	"May",			"May",			},
	{	"Jun",			"Jun",			},
	{	"Jul",			"Jul",			},
	{	"Aug",			"Aug",			},
	{	"Sep",			"Sep",			},
	{	"Oct",			"Oct",			},
	{	"Nov",			"Nov",			},
	{	"Dec",			"Dec",			},
	{	"",				""				},
};

const struct _textlist drivestate[] = {
	{	"Stopped",		"Stopped",		},
	{	"Start",		"Start",		},
	{	"Op.Brake",		"Op.Brake",		},
	{	"Moving",		"Moving",		},
	{	"Slowing",		"Slowing",		},
	{	"Stopping",		"Stopping",		},
	{	"Cl.Brake",		"Cl.Brake",		},
	{	"Dir.off",		"Dir.off",		},
	{	"Sw.off",		"Sw.off",		},
	{	"",				""				},
};

const struct _textlist calls[] = {
	{	"disabled",		"disabled"		},
	{	"enabled",		"enabled"		},
	{	"",				""				},
};

const struct _textlist off_on[] = {
	{	"off",			"off"			},
	{	"on",			"on"			},
	{	"",				""				},
};


const struct _standardtext doortext[] = {
	{	"closed",		"closed",		0x00		},
	{	"opening",		"opening",		0x50		},
	{	"opened",		"opened",		0x30		},
	{	"closing",		"closing",		0x20		},
	{	"reopen",		"reopen",		0x60		},
	{	"error",		"error",		0xE0		},
	{	"stopped",		"stopped",		0x70		},
	{	"",				"",				ENDSTRING	},
};

const struct _textlist drivetype[] = {
	{	"NICE5000",		"NICE5000"		},
	{	"GTEC-SL3",		"GTEC-SL3"		},
	{	"ATV LIFT",		"ATV LIFT"		},
	{	"MONT51",		"MONT51"		},
	{	"CAN DCU",		"CAN DCU"		},
	{	"None",			"None"			},
	{	"",				""				},
};

const struct _textlist relaymode[] = {
	{	"both off",		"both off",		},
	{	"Open on",		"Open on",		},
	{	"Close on",		"Close on",		},
	{	"both on",		"both on",		},
	{	"",				""				},
};

const struct _textlist lcmode[] = {
	{	"reopen",		"reopen",		},
	{	"ignore",		"ignore",		},
	{	"oth.lift",		"oth.lift",		},
	{	"",				""				},
};

const struct _textlist callmode[] = {
	{	" Firemen",		" Firemen",		},
	{	"Fire ev.",		"Fire ev.",		},
	{	"Emer.run",		"Emer.run",		},
	{	" VIP run",		" VIP run",		},
	{	"Spec.run",		"Spec.run",		},
	{	"Lift off",		"Lift off",		},
	{	"Int.door",		"Int.door",		},
	{	"Rem. off",		"Rem. off",		},
	{	"Land.off",		"Land.off",		},
	{	"Car full",		"Car full",		},
	{	"Parktrip",		"Parktrip",		},
	{	"Cl.run 2",		"Cl.run 2",		},
	{	"Cl.run 1",		"Cl.run 1",		},
	{	"Earthqu.",		"Earthqu.",		},
	{	"Car att.",		"Car att.",		},
	{	"Autocall",		"Autocall",		},
	{	" Up peak",		" Up peak",		},
	{	"Downpeak",		"Downpeak",		},
	{	"  Bypass",		"  Bypass"		},
	{ "Evaction", 	"Evaction"		},
	{ "UCMPlock",		"UCMPlock",		},
	{ "EmpPower", 	"EmpPower", 	},
	{ "SW. UIL", 		"SW. UIL", 		},
	{ "SW. LIL",		"SW. LIL",		},
	{ "Overtemp",		"Overtemp",		},
	{ "Force.ST", 	"Force.ST", 	},
	{ "DoorCon.", 	"DoorCon.", 	},
	{ "DoorStop", 	"DoorStop", 	},
	{ "Bkeckerr", 	"Bkeckerr", 	},
	{ "VIP call", 	"VIP call", 	},
	{ "Firecall", 	"Firecall", 	},
	{ "SW. D1U1", 	"SW. D1U1", 	},
	{ "SW.PIT", 		"SW.PIT", 	},
	{	"",				""				},
};

const struct _textlist edge[] = {
	{	"no",			"no",			},
	{	"on",			"on",			},
	{	"off",			"off",			},
	{	"on+off",		"on+off",		},
	{	"",				""				},
};

const struct _textlist firedoor[] = {
	{	"auto",			"auto",			},
	{	"pulse",		"pulse",		},
	{	"stop",			"stop",			},
	{	"reverse",		"reverse",		},
	{	"",				""				},
};

const struct _textlist door_firefloor[] = {
	{	"auto",			"auto"			},
	{	"fire",			"fire."			},
	{	"",				""				},
};

const struct _textlist normal_special[] = {
	{	"Normal",		"Normal"		},
	{	"+Special",		"+Special"		},
	{	"",				""				},
};

const struct _textlist rampmode[] = {
	{	"never",		"never",		},
	{	"always",		"always",		},
	{	"betw.fl",		"betw.fl",		},
	{	"",				""				},
};

const struct _textlist ramptiming[] = {
	{	"with door",	"with door",	},
	{	"after door",	"after door",	},
	{	"",				""				},
};

const struct _textlist dis_enable[] = {
	{	"Disable",		"Disable"		},
	{	"Enable",		"Enable."		},
	{	"",				""				},
};

const struct _textlist controldir[] = {
	{	"-",			"-"				},
	{	"upwards",		"upwards"		},
	{	"downwards",	"downwards"		},
	{	"",				""				},
};

const struct _textlist single_double[] = {
	{	"no",			"no"			},
	{	"single",		"single"		},
	{	"double",		"double"		},
	{	"",				""				},
};


