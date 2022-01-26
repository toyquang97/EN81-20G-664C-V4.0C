
#ifndef _TEXT_H_
#define _TEXT_H_

#include "parameter.h"

/****************************************************************************************************/
/* Standard text						 															*/
/****************************************************************************************************/
#define	T_WAIT				2
#define	T_TOO_SMALL			3
#define	T_TOO_BIG			4
#define	T_SUCCESS			5
#define	T_NO_SUCCESS		6
#define	T_MOVE				7
#define	T_MOVEERROR1		8
#define	T_MOVEERROR2		9
#define	T_MOVEERROR3		10
#define	T_MOVEERROR4		11
#define	T_MOVEERROR5		12
#define	T_MOVEERROR6		13
#define	T_T_DOORCL			14
#define	T_T_NO_ACCESS		15
#define	T_T_SL_OFF			16
#define	T_T_POSERROR		17
#define	T_T_DOORERROR		18
#define	T_T_TIME			19
#define	T_T_TOP				20
#define	T_T_GEN_ERROR		22
#define	T_T_READY			32
#define	T_HIST_END			36
#define	T_ESC				37
#define	T_SURE				47
#define	T_CONTINUE			48
#define	T_BACK				49
#define	T_CANCEL			50
#define	T_SIMUPARA			51
#define	T_INPARA			52
#define	T_OUTPARA			53
#define	T_SET_CALL			54
#define	T_SET_QUIT			55
#define	T_CLEAR_INPUT		56
#define	T_CLEAR_OUTPUT		57
#define	T_TESTTRIP			59
#define	T_GROUP				60
#define	T_CORRECT_TARGET	62
#define	T_CORRECT_LEVEL		63
#define	T_CORRECT_CALL		64
#define	T_PASSWORD			66
#define	T_PASSWORD_WRONG	67
#define	T_CAR_CALL			70
#define	T_CALL_READY		71
#define	T_CALLS_DISABLED	72
#define	T_ADJUST_LOAD		73
#define	T_NO_RESPONSE		75
#define	T_ERRCNT_START		76
#define	T_DENY_ACCESS		77
#define	T_DISP_ID			78
#define	T_ABORT_ID			79
#define	T_DIGIT_LOAD_NA		80
#define	T_RENDER_LOAD		81
#define T_TRIPLMT_NOTAVAILABLE	82
#define T_TRIPLMT_START	83
#define T_TRIPLMT_CANCELLED	84
#define	T_ACK_OVERTIMER		85
#define	T_BRAKE_CHECK1			86
#define	T_BRAKE_CHECK2			87
#define	T_BRAKE_CHECK3			88
#define	T_BRAKE_CHECK4			89
#define	T_BRAKE_CHECK5			90
#define	T_BACKUP_INVALID		91
#define	T_SELECT_RDID				92
#define	T_SELECT_COPDISPLAY	93
#define	T_SELECT_LIOPDISPLAY	94

#define	T_UCMP_NOT_FINISH	95

#define	T_TSE_NOTALLOCATE	100
#define	T_ESE_NOTALLOCATE	101
#define	T_ESE_FULLED		102
#define	T_EE_ERROR		104
#define	T_CHKSUM_ERROR	105
#define	T_OVERWRITE_WARNNG		106
#define	T_OK_AND_RESET			107
#define	T_CANCEL_AND_RESET		108
#define	T_EXE_NOTALLOCATE	109
#define	T_ACCESS_NOTALLOCATE	110

#define	T_STATUS1			201
#define	T_STATUS2			202
#define	T_STATUS3			203
#define	T_STATUS3_1		204
#define	T_STATUS4			205
#define	T_STATUS5			206
#define	T_STATUS6			207
#define	T_STATUS7			208
#define	T_STATUS8			209

/****************************************************************************************************/
/* Menu text						 																*/
/****************************************************************************************************/
#define	M_MAIN				1
#define	M_ERROR				2
#define	M_PARAMETER			3
#define	M_HARDWARE			4
#define	M_INTERNAL			5
#define	M_SAVEPARA			6
#define	M_SET_TIME			7
#define	M_MOVE				8
#define	M_TSE				9
#define	M_ESE				10
#define	M_SHOW_HIST			11
#define	M_CL_HIST			12
#define	M_HIST_DETAIL		13
#define	M_IO_HSE			14
#define	M_IO_FVE			15
#define	M_PARA_BACKUP		17
#define	M_IN_HSE			18
#define	M_OUT_HSE			19
#define	M_IN_FVE			20
#define	M_OUT_FVE			21
#define	M_IN_TSE			22
#define	M_OUT_TSE			23
#define	M_IN_ESE			24
#define	M_OUT_ESE			25
#define	M_EXE				26
#define	M_IN_EXE			27
#define	M_OUT_EXE			28
#define	M_EXE_IN_LIST		29
#define	M_EXE_OUT_LIST		30
#define	M_EXE_INFO		31

#define	M_LEVEL_KEY			32
#define	M_DOORPARA			33
#define	M_STATISTIC			34
#define	M_CLR_TRIPCOUNT		35
#define	M_CLR_HOURCOUNT		36
#define	M_POS				37
#define	M_DOORTEST			38
#define	M_SPECIAL			39
#define	M_BASIC_SAVE		40
#define	M_SIMU				41
#define	M_PASSWORD			42
#define	M_CHANGE_PW			43
#define	M_GROUP				44
#define	M_DOORFUNCTION		45
#define	M_TSE_INFO			46
#define	M_ESE_INFO			47
#define	M_FVE_INFO			48
#define	M_HSE_INFO			49
#define	M_SET_LOAD			50

#define	M_CAR_CALLS			53
#define	M_LEVELPOS			54
#define	M_DEBUGDISPLAY		55
#define	M_CLR_FLOORCNT		56
#define	M_SHOW_COUNT		57
#define	M_CLEAR_COUNT		58
#define	M_TEACH_IN_SHAFT	59
#define	M_GROUPSYNC			61
#define	M_EM_PAR			62
#define	M_IC_TEST			63
#define	M_LCU_ID			64
#define	M_DISP_ID			65
#define	M_SET_ID_1			66
#define	M_DRIVEPARA			67
#define	M_RUNPARA			68
#define	M_TEACH_IN			69
#define	M_MOTOR_TUNE		70
#define	M_SHAFT_SENSORS		71
#define	M_MONITOR			72
#define	M_IN_OUTPUTS		73
#define	M_TSE_IN_LIST		74
#define	M_TSE_OUT_LIST		75
#define	M_ESE_IN_LIST		76
#define	M_ESE_OUT_LIST		77
#define	M_SET_ID_2			78
#define	M_CONFIG_SENSORS		79
#define	M_LOAD_IV_FR_HHT	87
#define	M_ACCESS					88
#define	M_ACCESS_IN_LIST		89
#define	M_ACCESS_OUT_LIST		90
#define	M_ACCESS_INFO		91
#define	M_IN_ACCESS			92
#define	M_OUT_ACCESS			93

#define	M_GEN_PARA			501
#define	M_DOOR1PARA			502
#define	M_DOOR2PARA			503
#define	M_DRIVE_COMMON		504
#define	M_TIMERPARA			505
#define	M_INTERNPARA		506
#define	M_SLOWPOS			507
#define	M_LEVEL_HEIGHT		508
#define	M_SGMPOS			509
#define	M_SGOUPOS			510
#define	M_DOORPOS			511
#define	M_SIGN				512
#define	M_HALLDISPLAY		513
#define	M_TESTTRIP			514
#define	M_FORCED_STOP		515
#define	M_REMOTE_OFF		516
#define	M_FAN				517
#define	M_MISUSE			518
#define	M_PARKDOOR			519
#define	M_DOORSWITCH		520
#define	M_DOORRELAY			521
#define	M_CAN_DISTURB		522
#define	M_COLLECTDIR		523
#define	M_SELECTIVE_CC		524
#define	M_SELECTIVE_LC		525
#define	M_STARTPARA		526

#define	M_DOOR_NORMAL		527
#define	M_DOOR_TIMER_1		528
#define	M_DOOR_TIMER_2		529
#define	M_DOOR_SPECIAL		530
#define	M_DOOR_ADV			531
#define	M_DOOR_EMERG		532
#define	M_DOOR_FIRE			533
#define	M_DOOR_N_HALL		534
#define	M_DOOR_T1_HALL		535
#define	M_DOOR_T2_HALL		536
#define	M_DOOR_AUTOCALL		537
#define	M_SPECIAL_TRIP		538
#define	M_ADV_TRIP			539
#define	M_EMERG_TRIP		540
#define	M_FIREMAN_TRIP		541
#define	M_TIMER_TRIP		542
#define	M_DIS_ENABLE		543
#define	M_FIRE_CONTROL		544
#define	M_FIRE_FLOOR		545
#define	M_DOOR_ALARM		546
#define	M_GROUPCONTROL		547
#define	M_ATTENDANCE		548
#define	M_EVACUATION		549
#define	M_EMPTY_LOAD		550
#define	M_HALF_LOAD			551
#define	M_FULL_LOAD		552
#define	M_OVER_LOAD			553

#define	M_COOPERATE		554
#define	M_CHECK				555
#define	M_DOUBLE_DOOR	556
#define	M_SPECTION_MODE	557

#define	M_BASIC				558
#define	M_MUSIC_SETTING	559

#define	M_STATUS1			560
#define	M_STATUS2			561
#define	M_STATUS3			562
#define	M_STATUS4			564
#define	M_STATUS5			565
#define	M_STATUS_INTERNAL	566
#define	M_PARASET			568
#define	M_BRAKE_CHECK		569
#define	M_TIME				570
#define	M_FLOORCOUNT		571
#define	M_TRIPLIMIT			572
#define	M_STARTLIMIT		573
#define	M_STATUS_LIMIT	574
#define	M_RELEPARA				575	
#define	M_STATUS6		576
#define	M_STATUS7			577
#define	M_STATUS8			578

#define	M_CHANGE_PW0		590
#define	M_CHANGE_PW1		591
#define	M_CHANGE_PW2		592
#define	M_CHANGE_PW3		593

/****************************************************************************************************/
/* Parameter input display			 																*/
/* Don't use the same numbers like for the menu text!												*/
/****************************************************************************************************/
#define	M_PARAINPUT			600
#define	M_NUMBERPARA		600
#define	M_LISTPARA			601
#define	M_FLOORPARA			602
#define	M_ERRORPARA			603
#define	M_BITPARA			604
#define	M_BITPOSPARA		605
#define	M_YES_NOPARA		606
#define	M_NODEIDPARA		607
#define	M_DOORNUMBERPARA	608
#define	M_LIFTPARA			611
#define	M_SIGNPARA			612
#define	M_IOPARA			613
#define	M_CHANGE_IOPARA		614
#define	M_SIGNALPARA		615
#define	M_LIFTBITPARA		616
#define	M_POSHEIGHTPARA		617
#define	M_1BIT_DECIMAL		620
#define	M_2BIT_DECIMAL		621
#define	M_3BIT_DECIMAL		622
#define	M_CHANGE_FX			627
#define	M_SS_PARA			628
#define	M_RESET_MCU			629
#define	M_FLOOR_LEVEL	630

#define	M_UCMP_TEST	640

//NICE5000 Menu
#define	M_NICE_F0		0x1000
#define	M_NICE_F1		0x1100
#define	M_NICE_F2		0x1200
#define	M_NICE_F3		0x1300
#define	M_NICE_F4		0x1400
#define	M_NICE_F5		0x1500
#define	M_NICE_F6		0x1600
#define	M_NICE_FH		0x1700
#define	M_NICE_FA		0x1800
#define	M_NICE_FF		0x1900
#define	M_NICE_FR		0x1A00


/****************************************************************************************************/
/* General definitions					 															*/
/****************************************************************************************************/
#define	ENDSTRING			0xFFFE

/****************************************************************************************************/
/* HSE state text						 															*/
/****************************************************************************************************/
#define	HT_WAIT_STOP		0x01
#define	HT_INIT				0x02
#define	HT_SEARCH			0x03
#define	HT_STANDSTILL		0x04
#define	HT_MOVING			0x05
#define	HT_ARRIVAL			0x06
#define	HT_ERROR			0x07
#define	HT_CAN_ERROR		0x08
#define	HT_INSP				0x09
#define	HT_EM_OP			0x0A
#define	HT_RESTART_INH		0x0B
#define	HT_TERM				0x0C
#define	HT_HB_ERROR			0x0D
#define	HT_START			0x0E
#define	HT_DIVING			0x0F
#define	HT_TEACH_IN			0x10
#define	HT_SC_ERROR			0x11
#define	HT_DOORERROR		0x12
#define	HT_DOOROPEN			0x13
#define	HT_RESET			0x14
#define	HT_NO_SETUP			0x15
#define HT_DRIVE_ERROR	 	0x16
#define	HT_HB_DRV			0x17
#define	HT_DRV_SC			0x18
#define	HT_DRV_RUNCON			0x19
#define	HT_DRV_BRAKE		0x1A
#define	HT_DRV_MOVE			0x1B
#define	HT_HB_FVE			0x1C
#define	HT_VO_VU			0x1D
#define	HT_OVERTEMP			0x1E
#define	HT_BRAKESTATE		0x1F
#define	HT_SC2				0x20
#define	HT_SC3				0x21
#define	HT_KH5_ON			0x22
#define	HT_KH5_OFF			0x23
#define	HT_CARLIGHT			0x24
#define	HT_OVERLOAD			0x25
#define	HT_RELEVEL			0x26
#define	HT_NO_CAN_A			0x27
#define	HT_TRIP_TIME		0x28
#define	HT_DOORTEST			0x29
#define	HT_SC_LIMIT			0x2A
#define	HT_EVACUATION		0x2B
#define	HT_DIVING_ERR		0x2C
#define	HT_WARNING			0x2D
#define	HT_EE_ERROR			0x2E
#define	HT_IV_TEACH_IN		0x2F
#define	HT_DL_SHORTED		0x30
#define	HT_ILO_ON			0x31
#define	HT_ILU_ON			0x32
#define	HT_LIMITCALL			0x33
#define	HT_DRV_BRKCON		0x34
#define	HT_DRV_FXCON			0x35
#define	HT_SAFETY_BEAM	0x36
#define	HT_MSI_NOTOFF	0x37
#define	HT_DRV_NICE_RUNSIG		0x38
#define	HT_DRV_NICE_BRAKESIG	0x39
#define	HT_DRV_NICE_DIRSIG		0x3A
#define	HT_HB_EXE						0x3B
#define	HT_DOOR_SHORT				0x3C
#define	HT_ACCESS				0x3D
#define	HT_STEEL_BROKE				0x3E

/****************************************************************************************************/
/* Definition if basic function is used for input or output	(or both)								*/
/****************************************************************************************************/
#define	INPUT				0x01
#define	OUTPUT				0x02

/****************************************************************************************************/
/* Definition for special display of inputs and outputs												*/
/****************************************************************************************************/
#define	S_STANDARD			0x00				/* display only function text						*/
#define	S_HALL_CALL			0x01				/* display function text,direction and floor number	*/
#define	S_FUNC_SUB			0x02				/* display function text and sub function number	*/
#define	S_FUNC_SUBTEXT		0x03				/* display function text and sub function text		*/
#define	S_SUBTEXT			0x04				/* display sub function text						*/
#define	S_SUBTABLE			0x05				/* display sub function text from table				*/
#define	S_SUBTEXT_FLOOR		0x06				/* display sub function text and floor number		*/
#define	S_FUNC_ARROW		0x07				/* display function text and sub function as arrow	*/
#define	S_FUNC_NUMBER		0x08				/* display function text and sub function number	*/

/****************************************************************************************************/
/* Definition for status display: call mode															*/
/****************************************************************************************************/
#define D_NORMAL			0xFF				/* normal mode; display operation time counter		*/
#define D_FIRE				0x00				/* display fireman mode								*/
#define D_FIREALARM			0x01				/* display fire alarm								*/
#define D_EMERGENCY			0x02				/* display emergency mode							*/
#define D_ADVANCED			0x03				/* display advanced mode							*/
#define D_SPECIAL			0x04				/* display special mode								*/
#define D_LIFT_OFF			0x05				/* display lift off									*/
#define D_INTERM_DOOR		0x06				/* display intermediate door open					*/
#define D_REMOTE_OFF		0x07				/* display remote off								*/
#define D_LANDINGS_OFF		0x08				/* display landings off								*/
#define D_FULL_LOAD			0x09				/* display full load								*/
#define D_PARKTRIP			0x0A				/* display parking trip								*/
#define D_TIMER2			0x0B				/* display timer 2 trip								*/
#define D_TIMER1			0x0C				/* display timer 1 trip								*/
#define D_EARTHQUAKE		0x0D				/* display earthquake								*/
#define D_ATTENDANCE		0x0E				/* display car attendance mode						*/
#define D_AUTOCALL			0x0F				/* display auto-call mode ("Sabbat control")		*/
#define D_UP_PEAK			0x10				/* display up peak traffic							*/
#define D_DN_PEAK			0x11				/* display down peak traffic						*/
#define	D_BYPASS				0x12
#define	D_EVACTION			0x13
#define	D_UCMPLOCK		0x14
#define	D_EMPPOWER		0x15
#define	D_ILO					0x16
#define	D_ILU					0x17
#define	D_OVERTEMP		0x18
#define	D_FORCE_STOP   0x19
#define	D_DOOR_CONNECT   0x1A
#define	D_DOOR_STOP   0x1B
#define	D_BRAKE_CHECK   0x1C
#define	D_VIP_CARCALL   0x1D
#define	D_FIRE_RECALL   0x1E
#define	D_SDD1_SDU1_ON   0x1F
#define	D_PIT_RESET   	0x20

/****************************************************************************************************/
/* Definition for object index																		*/
/****************************************************************************************************/
#define	NO_OBJECT			0					/* variable should not be transmitted on CAN bus	*/
#define	DOUBLE_OBJECT		0					/* object already in dictionary on other place		*/
#define	OBJECT_BIT0			0xFE00		//用于未变量的操作
#define	OBJECT_BIT1			0xFE01		//用于未变量的操作
#define	OBJECT_BIT2			0xFE02
#define	OBJECT_BIT3			0xFE03
#define	OBJECT_BIT4			0xFE04
#define	OBJECT_BIT5			0xFE05
#define	OBJECT_BIT6			0xFE06
#define	OBJECT_BIT7			0xFE07
#define	OBJECT_BIT8			0xFE08
#define	OBJECT_BIT9			0xFE09
#define	OBJECT_BIT10		0xFE0A
#define	OBJECT_BIT11		0xFE0B
#define	OBJECT_BIT12		0xFE0C
#define	OBJECT_BIT13		0xFE0D
#define	OBJECT_BIT14		0xFE0E
#define	OBJECT_BIT15		0xFE0F


struct _iopara
{
	WORD menu;
	BYTE * pointer;
	BYTE size;
	BYTE dir;
	BYTE array;
	WORD checkmenu;
	BYTE start;
	BYTE stop;
};
extern const struct _iopara iopara [];


struct _textlist
{
	BYTE const * text [NO_OF_LANGUAGE];
};
extern const struct _textlist statetext [];
extern const struct _textlist dirtext [];
extern const struct _textlist fanmode [];
extern const struct _textlist paradetail [];
extern const struct _textlist signaldetail [];
extern const struct _textlist drivedetail [];
extern const struct _textlist hallcalldetail [];
extern const struct _textlist carcalldetail [];
extern const struct _textlist arrowtext [];
extern const struct _textlist polarity [];
extern const struct _textlist enable_io [];
extern const struct _textlist parkmode [];
extern const struct _textlist openallow [];
extern const struct _textlist no_yes [];
extern const struct _textlist postype [];
extern const struct _textlist weekday [];
extern const struct _textlist month [];
extern const struct _textlist set_speed [];
extern const struct _textlist drivestate [];
extern const struct _textlist calls [];
extern const struct _textlist off_on [];
extern const struct _textlist drivetype [];
extern const struct _textlist relaymode [];
extern const struct _textlist valve [];
extern const struct _textlist lcmode [];
extern const struct _textlist callmode [];
extern const struct _textlist edge [];
extern const struct _textlist firedoor [];
extern const struct _textlist door_firefloor [];
extern const struct _textlist normal_special [];
extern const struct _textlist rampmode [];
extern const struct _textlist ramptiming [];
extern const struct _textlist dis_enable [];
extern const struct _textlist controldir [];
extern const struct _textlist single_double [];
extern const struct _textlist loadtype [];
extern const struct _textlist arrowtype [];


struct _errortext
{
	WORD number;
	BYTE err_code;
	const BYTE * text [NO_OF_LANGUAGE];
	BYTE show_id;
};
extern const struct _errortext errortext [];

struct _errordetail
{
	BYTE  const * text [NO_OF_LANGUAGE];
	WORD  paratype;
	void  *pointer;
	WORD  size;
	const struct _textlist  * table;
};
extern const struct _errordetail errordetail [];
extern const struct _errortext errortext_bg [];

struct _iotype
{
	const BYTE * text [NO_OF_LANGUAGE];
	WORD number;
	BYTE io;
	WORD special;
	void * pointer;
	BYTE filter;
	const struct _textlist * detail;
	BYTE sign [NO_OF_LANGUAGE];
};
extern const struct _iotype iotype [];

struct _paratext
{
	BYTE const * text [NO_OF_LANGUAGE];
	WORD menu;
	WORD paratype;
	void * pointer;			
	BYTE size;
	BYTE array;
	void * minpointer;			
	void * tablepointer;			
	DWORD min;
	DWORD max;
	DWORD basic;
	WORD index;
	BYTE subindex;
	BYTE object;
	BYTE datatype;
	BYTE access;        
	BYTE right;
};
extern const struct _paratext paratext [];

struct _standardtext
{
	BYTE const * text [NO_OF_LANGUAGE];
	WORD number;
};
extern const struct _standardtext standardtext [];
extern const struct _standardtext doortext [];

struct _menutext
{
	BYTE const *text [NO_OF_LANGUAGE];
	WORD number;
	WORD submenu;
	BYTE right;
};
extern const struct _menutext menutext [];

struct _specialtext
{
	BYTE const *text [NO_OF_LANGUAGE];
	WORD number;
	BYTE filter;
	BYTE sign [NO_OF_LANGUAGE];
};
extern const struct _specialtext special_func [];
extern const struct _specialtext special_out [];
extern const struct _specialtext drive_out [];
extern const struct _specialtext contact_in [];
extern const struct _specialtext pos_ind [];
extern const struct _specialtext insp_dir_in[];
extern const struct _specialtext em_op_dir [];
extern const struct _specialtext access_op_dir [];
extern const struct _specialtext door_in [];
extern const struct _specialtext door_out [];
extern const struct _specialtext sc_text [];
extern const struct _specialtext load_text [];
extern const struct _specialtext temp_text [];
extern const struct _specialtext press_text [];
extern const struct _specialtext brake_in [];
extern const struct _specialtext pos_in [];
extern const struct _specialtext force_switch_in [];
extern const struct _specialtext door_function[];
extern const struct _specialtext fire_in[];
extern const struct _specialtext fire_out[];
extern const struct _specialtext call_type_in[];
extern const struct _specialtext call_type_out[];
extern const struct _specialtext light_type[];
extern const struct _specialtext gong_type[];

#endif

