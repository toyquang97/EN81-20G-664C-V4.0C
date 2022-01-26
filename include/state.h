
#ifndef _STATE_H_
#define _STATE_H_


/****************************************************************************************************/
/* HSE state																						*/
/****************************************************************************************************/
#define	H_WAIT_STOP			0x01
#define	H_INIT				0x02
#define	H_SEARCH			0x03
#define	H_STANDSTILL		0x04
#define	H_MOVING			0x05
#define	H_ARRIVAL			0x06
#define	H_ERROR				0x07
#define	H_CAN_ERROR		0x08
#define	H_INSP					0x09
#define	H_EM_OP				0x0A
#define	H_HOMING			0x0B
#define	H_TERM				0x0C
#define	H_HB_ERROR			0x0D
#define	H_START				0x0E
#define	H_DIVING			0x0F
#define	H_TEACH_IN			0x10
#define	H_SC_ERROR			0x11
#define	H_DOORERROR			0x12
#define	H_DOOROPEN			0x13
#define	H_RESET				0x14
#define	H_NO_SETUP			0x15
#define	H_DRIVE_ERROR	 	0x16
#define	H_ERROR_RELEV		0x17
#define	H_DOORTEST			0x18
#define	H_EVACUATION		0x19
#define	H_EE_ERROR		0x1A
#define	H_IV_TEACH_IN		0x1B
#define	H_ACCESS_OP		0x1C

//**************************************************************************************************
//	direction
//**************************************************************************************************
#define DIR_NULL				0
#define DIR_UP				1
#define DIR_DN				2

/****************************************************************************************************/
/*	Drive error state																				*/
/****************************************************************************************************/
#define D_EMERG_STOP			0xFFFFFFF0ul		/* stop lift immediately						*/
#define D_STOP_NEXT_FLOOR		0x0000000Ful		/* stop lift in next floor						*/

#define D_BRAKESTATE			0x00000002ul		/* bad brake state								*/

#define	D_BRKCONT_NOTON		0x00000040ul
#define	D_BRKCONT_NOTOFF		0x00000080ul
#define	D_FXCONT_NOTON		0x00000100ul
#define	D_FXCONT_NOTOFF		0x00000200ul
#define D_BRAKE_NOTON		0x00000400ul
#define D_BRAKE_NOTOFF		0x00000800ul
#define D_RUNCONT_NOTON		0x00001000ul
#define D_RUNCONT_NOTOFF		0x00002000ul
#define D_NO_SC				0x00004000ul
#define D_ERRORSTATE			0x00008000ul

#define D_NICE_RUN_NOTON		0x00010000ul
#define D_NICE_RUN_NOTOFF		0x00020000ul
#define D_NICE_BRAKE_NOTON	0x00040000ul
#define D_NICE_BRAKE_NOTOFF	0x00080000ul
#define	D_NICE_MOVE_NOTON	0x00100000ul
#define	D_NICE_MOVE_NOTOFF	0x00200000ul
#define D_NICE_DIR_NOTON		0x00400000ul

/****************************************************************************************************/
/*	Drive state																						*/
/****************************************************************************************************/
#define	D_START_MOVING			0x01				/* Start moving									*/
#define	D_READY_TO_SWITCH_ON		0x02				/* Drive ready to switch on						*/
#define	D_OPERATION_ENABLED		0x03				/* Drive is operational (moving)				*/
#define	D_SLOW_DOWN				0x04				/* Drive is slowing down						*/
#define	D_STOPPING				0x05				/* Drive is stopping							*/
#define	D_SPEED_ZERO			0x06				/* Drive reached speed = 0						*/
#define	D_OPERATION_DISABLED	0x07				/* Drive operation is disabled					*/
#define	D_SWITCHED_OFF			0x08				/* Drive is switched off						*/
#define	D_WAIT_STOP				0x09				/* Drive is waiting for brake and contactor off	*/
#define D_SWITCHED_ON			0x0A				/* Drive is switched on							*/
#define	D_STOPPED				0x00				/* Drive is stopped								*/

//**************************************************************************************************
//	CAN error state
//**************************************************************************************************
#define	CANA_MASK			0x000000fful
#define	CANB_MASK			0x0000ff00ul
#define	CANC_MASK			0x00ff0000ul

#define	TXA_SW_OVERRUN		0x00000001ul				// TX software buffer overrun
#define	TXA_HW_OVERRUN		0x00000002ul
#define	RXA_SW_OVERRUN		0x00000004ul				// RX software buffer overrun
#define	RXA_HW_OVERRUN		0x00000008ul				// RX message buffer overrun
#define	CANA_BUSOFF			0x00000010ul				// CAN bus off state
#define	CANA_PCB				0x00000020ul				// CAN error remote PCB
#define	SXA_SW_OVERRUN		0x00000040ul				// SXA software buffer overrun

#define	TXB_SW_OVERRUN		0x00000100ul				// TX software buffer overrun
#define	TXB_HW_OVERRUN		0x00000200ul
#define	RXB_SW_OVERRUN		0x00000400ul				// RX software buffer overrun
#define	RXB_HW_OVERRUN		0x00000800ul				// RX message buffer overrun
#define	CANB_BUSOFF			0x00001000ul				// CAN bus off state
#define	CANB_PCB				0x00002000ul				// CAN error remote PCB
#define	SXB_SW_OVERRUN		0x00004000ul				// SXB software buffer overrun

#define	TXC_SW_OVERRUN		0x00010000ul				// TX software buffer overrun
#define	TXC_HW_OVERRUN		0x00020000ul
#define	RXC_SW_OVERRUN		0x00040000ul				// RX software buffer overrun
#define	RXC_HW_OVERRUN		0x00080000ul				// RX message buffer overrun
#define	CANC_BUSOFF			0x00100000ul				// CAN bus off state
#define	CANC_PCB				0x00200000ul				// CAN error remote PCB
#define	SXC_SW_OVERRUN		0x00400000ul				// SXC software buffer overrun

/****************************************************************************************************/
/*	Positioning system error state																	*/
/****************************************************************************************************/
#define P_VO_VU_ERROR		0x0001					/* error on switch VO and / or VU				*/
#define P_DIVING_ERR			0x0002					/* SGM off, but positioning in level zone		*/
#define	P_ILO_ON			0x0004
#define	P_ILU_ON			0x0008
#define P_KH5_ON			0x0010					/* KH5 not released								*/
#define P_KH5_OFF			0x0020					/* KH5 not on									*/
#define P_VU_NOT_ON		0x0040			//检测不到强减信号
#define P_VO_NOT_ON		0x0080
#define P_MSI_ERROR		0x0100

/****************************************************************************************************/
/*	General errors																					*/
/****************************************************************************************************/
#define G_TRIPTIME			0x01					/* switch VU changed inside door zone			*/
#define G_OVERSPEED			0x02					/* overspeed									*/	
#define G_WRONG_DIR			0x04					/* wrong direction								*/	
#define G_DL_SHORTED			0x08

// 以下顺序不要随意更改，如果增加了门数(目前最大支持2个门)，这里有问题!
#define G_OPENDOOR1			0x10
#define G_OPENDOOR2			0x20
#define G_CLOSEDOOR1			0x40
#define G_CLOSEDOOR2			0x80

#define	G_LIGHTSCREEN1		0x0100
#define	G_LIGHTSCREEN2		0x0200
#define	G_MSI_NOTOFF			0x0400
#define	G_UCMP_LOCK			0x0800

/****************************************************************************************************/
/*	Definitions for trip error counter																*/
/****************************************************************************************************/
#define C_SC_MOVE			5						/* Safety circuit opened while moving			*/
#define C_NO_MOVE			5						/* No movement signal from drive				*/
#define C_MOVING_STOP			5

#define C_RUNCONTACT_NOTON		10
#define	C_RUNCONTACT_NOTOFF		100//10
#define C_BRKCONTACT_NOTON		10//100
#define	C_BRKCONTACT_NOTOFF		100//10
#define	C_BRAKE_NOTON			10//100
#define	C_BRAKE_NOTOFF			100

#define	C_WRONG_DIR			5//20						/* wrong travel direction						*/
#define	C_VO_VU_DOORZONE	10//20						/* VO or VU changed in doorzone					*/
#define	C_VO_VU_ON			10//20						/* VO and VU on at the same time				*/
#define	C_FAST_VO_VU		0//5						/* lift moving with V3 when VO/VU switched on	*/
#define	C_SGM_OFF			10//20						/* SGM off, but positioning in level			*/
#define	C_SGM_ON			20//100						/* SGM on, but positioning outside doorzone		*/
#define	C_KH5_ON			100						/* KH5 not released								*/
#define	C_KH5_OFF			10//100						/* KH5 not on									*/
#define	C_MAGNETCOUNT		20						/* count error (positioning with magnets)		*/
#define	C_TRIP_TIME			100						/* Trip time exceeded							*/
#define	C_PRESSURE			5						/* hydraulic pressure							*/
#define	C_SGMERR			20
#define	C_ILS_ON				0//20
#define	C_DIVING_ERROR		100
#define	C_RESET_TIP			0//20
#define	C_MSI_NOTOFF			100						/* Trip time exceeded							*/
#define	C_UCMP_LOCK			100						/* Trip time exceeded							*/
#define	C_BRAKE_CHECK		100						/* Trip time exceeded							*/

#define	C_NICE_ERROR			10
#define	C_NICE_RUN_NOTON		10
#define	C_NICE_RUN_NOTOFF		10
#define	C_NICE_BRAKE_NOTON	10
#define	C_NICE_BRAKE_NOTOFF	10
#define	C_NICE_MOVE_NOTON	10
#define	C_NICE_MOVE_NOTOFF	10
#define	C_NICE_MOVING_STOP	10
#define	C_FENGXING_NOTON		10
#define	C_FENGXING_NOTOFF		10//10
#define	C_OVERSPEED				5//20

#define	C_NICEERR_RETRY_3TIMES	40


/****************************************************************************************************/
/*	Definitions for error warning counter															*/
/****************************************************************************************************/
#define	W_HB_TSE			2						/* no heartbeat from TSE						*/
#define	W_HB_ESE			2						/* no heartbeat from ESE						*/
#define	W_ACCU				5						/* no mains power (power from accu)				*/
#define	W_CAN_TX_A			3						/* transmit on CAN A not possible				*/
#define	W_CAN_TX_B			3						/* transmit on CAN B not possible				*/
#define	W_CAN_TX_C			3						/* transmit on CAN B not possible				*/
#define	W_HB_FVE			5						/* no heartbeat from FVE						*/
#define	W_HB_PSE			5						/* no heartbeat from PSE						*/
#define	W_HB_ASE			5						/* no heartbeat from ASE						*/
#define	W_CARLIGHT			5						/* car light off								*/
#define	W_BUSA_OFF			5						/* CAN A bus off state							*/
#define	W_BUSB_OFF			5						/* CAN B bus off state							*/
#define	W_BUSC_OFF			5						/* CAN B bus off state							*/
#define	W_RXA_HW_OVERRUN	3						/* CAN A Rx hardware overrun					*/
#define	W_RXA_SW_OVERRUN	3						/* CAN A Rx software overrun					*/
#define	W_RXB_HW_OVERRUN	3						/* CAN B Rx hardware overrun					*/
#define	W_RXB_SW_OVERRUN	3						/* CAN B Rx software overrun					*/
#define	W_RXC_HW_OVERRUN	3						/* CAN C Rx hardware overrun					*/
#define	W_RXC_SW_OVERRUN	3						/* CAN C Rx software overrun					*/
#define	W_TXA_OVERRUN		3						/* CAN A Tx overrun								*/
#define	W_TXB_OVERRUN		3						/* CAN B Tx overrun								*/
#define	W_TXC_OVERRUN		3						/* CAN C Tx overrun								*/
#define	W_SC_MOVE			10						/* Safety circuit opened while moving			*/
#define	W_NO_MOVE			3						/* No movement signal from drive				*/
#define	W_NO_STOP			5						/* No stop in time								*/
#define W_RUNCONTACT_NOTON		10
#define W_RUNCONTACT_NOTOFF		10
#define W_BRKCONTACT_NOTON		10//40
#define	W_BRKCONTACT_NOTOFF		10
#define	W_BRAKE_NOTON			10//100
#define	W_BRAKE_NOTOFF			50//40
#define	W_BRAKE_WARNING		5						/* Brake warning state							*/
#define	W_MOTOR_TEMP		20						/* Motor overtemperature						*/
#define	W_OIL_TEMP			20						/* Oil overtemperature							*/
#define	W_PRESSURE			20						/* hydraulic pressure							*/
#define	W_VO_VU_DOORZONE	10						/* VO or VU changed in doorzone					*/
#define	W_VO_VU_ON			10						/* VO and VU on at the same time				*/
#define	W_FAST_VO_VU		10						/* lift moving with V3 when VO/VU switched on	*/
#define	W_SGM_OFF			10						/* SGM off, but positioning in level			*/
#define	W_SGM_ON			10						/* SGM on, but positioning outside doorzone		*/
#define	W_SC_SHAFTDOOR		10						/* safety circuit shaft door					*/
#define	W_INTERNAL_SW		5						/* restart by watchdog reset					*/
#define	W_BG_START			5						/* restart of a PCB								*/
#define	W_STOP_TO_EARLY		5						/* stopped too early (level zone not reached)	*/
#define	W_STOP_TO_LATE		5						/* stopped too late (level zone leaved)			*/
#define	W_WRONG_DIR			5						/* wrong travel direction						*/
#define	W_RELEV_SHORT		10						/* stopped too early while re-levelling			*/
#define	W_RELEV_LONG		10						/* stopped too late while re-levelling			*/
#define	W_LIGHT_SCREEN		40						/* Light screen defect or car call misuse		*/
#define	W_SGMERR			20
#define	W_ILS_ON				0//20
#define	W_RESET_TRIP		10
#define	W_UCMP_LOCK		100

#define	W_NICE_ERROR			10
#define	W_NICE_RUN_NOTON		10
#define	W_NICE_RUN_NOTOFF	10
#define	W_NICE_BRAKE_NOTON	10
#define	W_NICE_BRAKE_NOTOFF	10
#define	W_NICE_MOVE_NOTON	10
#define	W_NICE_MOVE_NOTOFF	10
#define	W_NICE_MOVING_STOP	10
#define	W_FENGXING_NOTON		10
#define	W_FENGXING_NOTOFF	10
#define	W_OVERSPEED		5
#define	W_STEEL_BROKE		20

/****************************************************************************************************/
/*	Definitions for teach in																		*/
/****************************************************************************************************/
#define	TI_SHAFT				0x01
#define	TI_MOTOR				0x02

/****************************************************************************************************/
/*	Doortest commands																				*/
/****************************************************************************************************/
#define	DR1_OPEN			1						/* open door 1									*/
#define	DR1_CLOSE			2						/* close door 1									*/
#define	DR1_CLOSE_L			3						/* close door 1	with limit force				*/
#define	DR2_OPEN			4						/* open door 2									*/
#define	DR2_CLOSE			5						/* close door 2									*/
#define	DR2_CLOSE_L			6						/* close door 2	with limit force				*/
#define	DR3_OPEN			7						/* open door 3									*/
#define	DR3_CLOSE			8						/* close door 3									*/
#define	DR3_CLOSE_L			9						/* close door 3	with limit force				*/
#define	DR_OPEN				10						/* open all doors								*/
#define	DR_CLOSE			11						/* close all doors								*/
#define	DR_STOP				12						/* stop all doors								*/


struct _outstate
{
	WORD function;
	BYTE subfunction;
	BYTE * value;
};
extern const struct _outstate outstate [];


void set_out_state (BYTE pos, BYTE bus);
BYTE request_io (WORD index, BYTE subindex, WORD pos);
BYTE search_nodestate (BYTE node);
void search_state (void);
void set_state (void);


void ini_state (void);
void inspection_state (void);
void em_op_state (void);
void error_state (void);
void can_error_state (void);
void drive_error_state (void);
void terminal_state (void);
void hb_error_state (void);
void sc_error_state (void);
void nosetup_state (void);
void reset_trip_state (void);
void door_open_state (void);
void moving_state (void);
void arrival_state (void);
void diving_state (void);
void standstill_state (void);
void wait_for_stop_state (void);
void start_trip_state (void);
void teach_in_state (void);
void error_relev (void);
void doortest_state (void);
void evacuation_state (void);
void motorturn_state (void);
void defaultstate (void);

#endif

