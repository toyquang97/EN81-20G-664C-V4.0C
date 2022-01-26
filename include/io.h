
#ifndef _IO_H_
#define _IO_H_

#include "lpc1700.h"
#include "config.h"

#define	PIO_0		(0x01ul << 0)
#define	PIO_1		(0x01ul << 1)
#define	PIO_2		(0x01ul << 2)
#define	PIO_3		(0x01ul << 3)
#define	PIO_4		(0x01ul << 4)
#define	PIO_5		(0x01ul << 5)
#define	PIO_6		(0x01ul << 6)
#define	PIO_7		(0x01ul << 7)
#define	PIO_8		(0x01ul << 8)
#define	PIO_9		(0x01ul << 9)
#define	PIO_10		(0x01ul << 10)
#define	PIO_11		(0x01ul << 11)
#define	PIO_12		(0x01ul << 12)
#define	PIO_13		(0x01ul << 13)
#define	PIO_14		(0x01ul << 14)
#define	PIO_15		(0x01ul << 15)
#define	PIO_16		(0x01ul << 16)
#define	PIO_17		(0x01ul << 17)
#define	PIO_18		(0x01ul << 18)
#define	PIO_19		(0x01ul << 19)
#define	PIO_20		(0x01ul << 20)
#define	PIO_21		(0x01ul << 21)
#define	PIO_22		(0x01ul << 22)
#define	PIO_23		(0x01ul << 23)
#define	PIO_24		(0x01ul << 24)
#define	PIO_25		(0x01ul << 25)
#define	PIO_26		(0x01ul << 26)
#define	PIO_27		(0x01ul << 27)
#define	PIO_28		(0x01ul << 28)
#define	PIO_29		(0x01ul << 29)
#define	PIO_30		(0x01ul << 30)
#define	PIO_31		(0x01ul << 31)

#define LED_RUNNING		0x00
#define LED_ON			0x01
#define LED_OFF			0x00

//运行指示灯
#define RunningLedOn()	(FIO4SET = 1ul << 29)
#define RunningLedOff()	(FIO4CLR = 1ul << 29)

#define WdtOn()			(FIO1SET = 1ul << 10)
#define WdtOff()		(FIO1CLR = 1ul << 10)

#define UlsiOn()		(FIO1SET = 1ul << 16)
#define UlsiOff()		(FIO1CLR = 1ul << 16)
#define	OPEN_MAIN_RELAY()	(FIO1SET = 1ul << 22)
#define CLOSE_MAIN_RELAY() (FIO1CLR = 1ul << 22)

#define SWITCH_ULSI					0x00
#define SWITCH_MAIN_RELAY		0x01

#define SWITCH_ON					0x01
#define SWITCH_OFF					0x00

extern BYTE state_msi;
extern BYTE state_sgm;
extern BYTE msi_trans_state;
extern DWORD msi_trans_num;
extern DWORD msi_trans_check;
extern BYTE state_ulsi;
extern BYTE state_run;
extern WORD boardkey;

extern BYTE state_psl;
extern BYTE psl_chk_on;
extern BYTE psl_chk_off;
extern BYTE psl_oncofirm;
extern BYTE psl_offcofirm;
extern DWORD input_psl;

extern BYTE state_cdsl;
extern BYTE cdsl_chk_on;
extern BYTE cdsl_chk_off;
extern BYTE cdsl_oncofirm;
extern BYTE cdsl_offcofirm;
extern DWORD input_cdsl;

extern BYTE state_sdsl;
extern BYTE sdsl_chk_on;
extern BYTE sdsl_chk_off;
extern BYTE sdsl_oncofirm;
extern BYTE sdsl_offcofirm;
extern DWORD input_sdsl;

void SetLed (BYTE index, BYTE state);
void SetSwitch (BYTE index, BYTE state);
void fan_on (void);

#define READ_INPUTS		((FIO0PIN >> 5) & 0x03) | \
						(((FIO0PIN >> 10) & 0x03) << 2) | \
						(((FIO0PIN >> 16) & 0x01) << 4) | \
						(((FIO0PIN >> 19) & 0x0f) << 5) | \
						(((FIO0PIN >> 25) & 0x03) << 9) | \
						(((FIO0PIN >> 29) & 0x01) << 11) | \
						((FIO2PIN & 0x3f) << 12) |\
						((FIO1PIN & 0x03) << MAX_HSE_IN) |\
						(((FIO1PIN >> 4) & 0x01) << (MAX_HSE_IN+2)) |\
						(((FIO2PIN >> 6) & 0x01) << (MAX_HSE_IN+MAX_HSE_EX_IN))			//ZONE input
#define DOORZONE_INPUT	(0x01ul << (MAX_HSE_IN+MAX_HSE_EX_IN))


#define MSI_IN			((FIO0PIN >> 30) & 0x01)
#define	PWR_IN			((FIO1PIN >> 21) & 0x01)

#define SET_OUT_1()		(FIO1SET = 1ul << 22)
#define CLR_OUT_1()		(FIO1CLR = 1ul << 22)

#define SET_OUT_2()		(FIO1SET = 1ul << 19)
#define CLR_OUT_2()		(FIO1CLR = 1ul << 19)

#define SET_OUT_3()		(FIO1SET = 1ul << 18)
#define CLR_OUT_3()		(FIO1CLR = 1ul << 18)

#define SET_OUT_4()		(FIO1SET = 1ul << 17)
#define CLR_OUT_4()		(FIO1CLR = 1ul << 17)

#define SET_OUT_5()		(FIO1SET = 1ul << 8)
#define CLR_OUT_5()		(FIO1CLR = 1ul << 8)

#define SET_OUT_6()		(FIO0SET = 1ul << 4)
#define CLR_OUT_6()		(FIO0CLR = 1ul << 4)

// 交流脉冲检测
#define	AC_CHK_TMS		5
#define	AC_ON_CONFIRM	6
#define	AC_OFF_CONFIRM	6
#define	AC_PENDING_TMS	3
#define	PSL_IN			((FIO2PIN >> 9) & 0x01)
#define	CDSL_IN			((FIO2PIN >> 12) & 0x01)
#define	SDSL_IN			((FIO2PIN >> 13) & 0x01)

#define	INPUT_SDU1		5
#define	INPUT_SDD1		6
#define	INPUT_SDU2		7
#define	INPUT_SDD2		8
#define	INPUT_SDU3		10
#define	INPUT_SDD3		11
#define	IO_SC1			18
#define	IO_SC2			19
#define	IO_SC3			20
#define	INPUT_ILO		12
#define	INPUT_ILU		4

//**************************************************************************************************
//	Key definition
//**************************************************************************************************
#define KEY_ESC		0x001B			// simulated by pressing '#' more than 1 sec
#define KEY_UP		0x0430			// simulated by pressing '*' more than 1 sec

#define SM_KEY_ENT			0x0001
#define SM_KEY_MENU			0x0002
#define SM_KEY_UP			0x0004
#define	SM_KEY_ENT_MENU		0x0008
#define	SM_KEY_ENT_UP		0x0010
#define	SM_KEY_MENU_UP		0x0020
#define	SM_KEY_ALL			0x0040
#define	LM_KEY_FLAG			0x8000
#define LM_KEY_ENT			0x8001
#define LM_KEY_MENU			0x8002
#define LM_KEY_UP			0x8004
#define	LM_KEY_ENT_MENU		0x8008
#define	LM_KEY_ENT_UP		0x8010
#define	LM_KEY_MENU_UP		0x8020
#define	LM_KEY_ALL			0x8040


#define LD1_H	FIO1SET = PIO_14
#define LD1_L	FIO1CLR = PIO_14

#define LD2_H	FIO1SET = PIO_15
#define LD2_L	FIO1CLR = PIO_15

#define LD3_H	FIO1SET = PIO_20
#define LD3_L	FIO1CLR = PIO_20

#define LD4_H	FIO1SET = PIO_23
#define LD4_L	FIO1CLR = PIO_23

#define SEG_A  (0x01ul << 24)
#define SEG_B  (0x01ul << 25)
#define SEG_C  (0x01ul << 26)
#define SEG_D  (0x01ul << 27)
#define SEG_E  (0x01ul << 28)
#define SEG_F  (0x01ul << 29)
#define SEG_G  (0x01ul << 30)
#define SEG_DOT  (0x01ul << 31)
#define SEG__  0x00ul


#define NUM_0  SEG_A+SEG_B+SEG_C+SEG_D+SEG_E+SEG_F+SEG__
#define NUM_1  SEG__+SEG_B+SEG_C+SEG__+SEG__+SEG__+SEG__
#define NUM_2  SEG_A+SEG_B+SEG__+SEG_D+SEG_E+SEG__+SEG_G
#define NUM_3  SEG_A+SEG_B+SEG_C+SEG_D+SEG__+SEG__+SEG_G
#define NUM_4  SEG__+SEG_B+SEG_C+SEG__+SEG__+SEG_F+SEG_G
#define NUM_5  SEG_A+SEG__+SEG_C+SEG_D+SEG__+SEG_F+SEG_G
#define NUM_6  SEG_A+SEG__+SEG_C+SEG_D+SEG_E+SEG_F+SEG_G
#define NUM_7  SEG_A+SEG_B+SEG_C+SEG__+SEG__+SEG__+SEG__
#define NUM_8  SEG_A+SEG_B+SEG_C+SEG_D+SEG_E+SEG_F+SEG_G
#define NUM_9  SEG_A+SEG_B+SEG_C+SEG_D+SEG__+SEG_F+SEG_G

#define NUM_A  SEG_A+SEG_B+SEG_C+SEG__+SEG_E+SEG_F+SEG_G
#define NUM_b  SEG__+SEG__+SEG_C+SEG_D+SEG_E+SEG_F+SEG_G
#define NUM_C  SEG_A+SEG__+SEG__+SEG_D+SEG_E+SEG_F+SEG__
#define NUM_d  SEG__+SEG_B+SEG_C+SEG_D+SEG_E+SEG__+SEG_G
#define NUM_E  SEG_A+SEG__+SEG__+SEG_D+SEG_E+SEG_F+SEG_G
#define NUM_F  SEG_A+SEG__+SEG__+SEG__+SEG_E+SEG_F+SEG_G
#define NUM_G  SEG_A+SEG__+SEG_C+SEG_D+SEG_E+SEG_F+SEG__

#define NUM_H  SEG__+SEG_B+SEG_C+SEG__+SEG_E+SEG_F+SEG_G
#define NUM_i  SEG__+SEG__+SEG__+SEG__+SEG_E+SEG_F+SEG__
#define NUM_J  SEG_A+SEG_B+SEG_C+SEG_D+SEG_E+SEG__+SEG__
#define NUM_L  SEG__+SEG__+SEG__+SEG_D+SEG_E+SEG_F+SEG__
#define NUM_n  SEG__+SEG__+SEG_C+SEG__+SEG_E+SEG__+SEG_G
#define NUM_O  SEG_A+SEG_B+SEG_C+SEG_D+SEG_E+SEG_F+SEG__
#define NUM_o  SEG__+SEG__+SEG_C+SEG_D+SEG_E+SEG__+SEG_G
#define NUM_P  SEG_A+SEG_B+SEG__+SEG__+SEG_E+SEG_F+SEG_G
#define NUM_r  SEG__+SEG__+SEG__+SEG__+SEG_E+SEG__+SEG_G
#define NUM_S  SEG_A+SEG__+SEG_C+SEG_D+SEG__+SEG_F+SEG_G
#define NUM_T  SEG_A+SEG__+SEG__+SEG__+SEG_E+SEG_F+SEG__
#define NUM_t  SEG__+SEG__+SEG__+SEG_D+SEG_E+SEG_F+SEG_G
#define NUM_U  SEG__+SEG_B+SEG_C+SEG_D+SEG_E+SEG_F+SEG__
#define NUM_u  SEG__+SEG__+SEG_C+SEG_D+SEG_E+SEG__+SEG__
#define NUM_y  SEG__+SEG_B+SEG_C+SEG_D+SEG__+SEG_F+SEG_G
//#define NUM_UP  SEG__+SEG__+SEG_C+SEG_D+SEG_E+SEG__+SEG__
//#define NUM_DOWN  SEG_A+SEG_B+SEG__+SEG__+SEG__+SEG_F+SEG__
#define NUM_UP  SEG__+SEG_B+SEG__+SEG__+SEG__+SEG_F+SEG_G
#define NUM_DOWN  SEG__+SEG__+SEG_C+SEG__+SEG_E+SEG__+SEG_G
#define NUM_NOP  0x00 //  22
#define NUM_ERR  SEG__+SEG__+SEG__+SEG__+SEG__+SEG__+SEG_G
#define NUM_DOT (0x01ul<<31)
#define NUM_ALL  SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G|SEG_DOT

#define	NUM_agd		SEG__
#define	NUM_agD		SEG_D
#define	NUM_aGd		SEG_G
#define	NUM_aGD		SEG_G+SEG_D
#define	NUM_Agd		SEG_A
#define	NUM_AgD		SEG_A+SEG_D
#define	NUM_AGd		SEG_A+SEG_G
#define	NUM_AGD		SEG_A+SEG_G+SEG_D
#define	NUM_BFG		SEG_B+SEG_F+SEG_G
#define	NUM_CDE		SEG_G+SEG_F+SEG_G

#define	NUM_bcef	SEG__
#define	NUM_bceF	SEG_F
#define	NUM_bcEf	SEG_E
#define	NUM_bcEF	SEG_E+SEG_F
#define	NUM_bCef	SEG_C
#define	NUM_bCeF	SEG_C+SEG_F
#define	NUM_bCEf	SEG_C+SEG_E
#define	NUM_bCEF	SEG_C+SEG_E+SEG_E
#define	NUM_Bcef	SEG_B
#define	NUM_BceF	SEG_B+SEG_F
#define	NUM_BcEf	SEG_B+SEG_E
#define	NUM_BcEF	SEG_B+SEG_E+SEG_F
#define	NUM_BCef	SEG_B+SEG_C
#define	NUM_BCeF	SEG_B+SEG_C+SEG_F
#define	NUM_BCEf	SEG_B+SEG_C+SEG_E
#define	NUM_BCEF	SEG_B+SEG_C+SEG_E+SEG_E

#define	EXT_A		(0x01 << 2)
#define	EXT_G		(0x01 << 1)
#define	EXT_D		(0x01 << 0)

#define	EXT_B		(0x10 | (0x01 << 3))
#define	EXT_C		(0x10 | (0x01 << 2))
#define	EXT_E		(0x10 | (0x01 << 1))
#define	EXT_F		(0x10 | (0x01 << 0))

#define ENT_KEY_IN			(!(FIO3PIN & PIO_25))
#define MENU_KEY_IN			(!(FIO4PIN & PIO_28))
#define UP_KEY_IN			(!(FIO3PIN & PIO_26))
#define	KEY_TIMER_30ms		6
#define	KEY_TIMER_600ms		120
#define	KEY_TIMER_900ms		180
#define	FLASH_TIMER_500ms	100
#define	FLASH_TIMER_1S		200

#define	ERO_BTN_IN			(FIO0PIN & PIO_5)


#define	DM_P1			0x1000
	#define DM_STATE				0x1010
	#define DM_LEVEL				0x1020
	#define DM_DOOR				0x1030	
	#define DM_LSSTATE			0x1040
	#define DM_SENSOR 		0x1050
	#define DM_D1STATE			0x1060
	#define DM_D1IO 				0x1070
	#define DM_LOADSTATE 	0x1080
	#define DM_SPEC 				0x1090	
	
#define	DM_P2			0x2000
	#define DM_TRIPCNT			0x2010
	#define DM_OPTIME 		0x2020
	#define DM_TRIPERR			0x2030
	#define DM_MOTO_ANGLE	0x2040

#define	DM_P3			0x3000
	#define DM_VERMCU			0x3010
	#define	DM_VERCDU			0x3020
	#define	DM_PCUNUM			0x3030
	#define	DM_VERPCU			0x3040
	#define	DM_LIOPNUM		0x3050
	#define	DM_VERLIOP			0x3060
	#define	DM_VERDRV			0x3070
	
#define	DM_P4			0x4000
	#define	DM_TARGET			0x4010
	#define	DM_IDSET				0x4020

#define	DM_P5			0x5000
	#define	DM_TESTLOAD		0x5010
	#define DM_DIR 				0x5030	
	#define DM_RESET 			0x5040
	#define DM_EMOP_ENILS	0x5050
	#define DM_TUNE 			0x5060
	#define DM_BREAK			0x5070
	#define	DM_FXFUN			0x5080
	#define DM_MAX_SPEED	0x5090
	#define DM_TEMPERA		0x50A0
	#define DM_SIL_TEST			0x50B0
	#define DM_RESET_UCMP 0x50C0
	#define DM_BRAKE_TEST 0x50D0
	#define DM_BRAKE_VALUE 0x50E0
	#define DM_TEST_UCMP 	0x5100
	
#define	DM_P6			0x6000	
	#define DM_TEACH			0x6020
	#define	DM_F2_11				0x6030
	#define DM_F2_12				0x6040
	#define DM_F2_13				0x6050
	#define DM_F2_14				0x6060
	#define DM_F2_15				0x6070

#define	DM_P7			0x7000
	#define	DM_ERR_LIST		0x7010
	#define DM_ERR_DET1 		0x7011
	#define DM_ERR_DET2 		0x7012
	#define DM_ERR_DET3 		0x7013
	#define DM_ERR_DET4 		0x7014
	#define DM_ERR_DET5 		0x7015
	#define DM_ERR_DET6 		0x7016
	
#define	DM_P8			0x8000
	#define DM_YEAR_TMR		0x8010
	#define DM_MON_TMR		0x8020
	#define DM_DAY_TMR		0x8030
	#define DM_HOUR_TMR		0x8040
	#define DM_MIN_TMR		0x8050
	#define DM_SEC_TMR		0x8060
		

#define	DM_NULL			0xFFFF

#define	D_MENU				0x00
#define	D_PAR				0x01			//普通参数
#define	D_PAR_LVL			0x02		//显示楼层
#define	D_PAR_DR			0x03			//门到位
#define	D_PAR_SAF			0x04		//安全回路
#define	D_PAR_SENSOR		0x05		//位置传感器
#define	D_PAR_TMR			0x06
#define	D_PAR_YEAR			0x07
#define	D_PAR_OPTIME		0x08
#define	D_PAR_DRSTATE	0x09		//门输出状态
#define	D_PAR_SPEC			0x0A
#define	D_PAR_VER			0x0B
#define	D_PAR_DRIO			0x0C		//门输入状态
#define	D_PAR_POS			0x0D
#define	D_PAR_DFP			0x0E
#define	D_PAR_MONTH		0x0F
#define	D_PAR_HEX			0x10
#define	D_PAR_ERRTIMES	0x11
#define	D_PAR_LOAD			0x12
#define	D_PAR_STATE		0x13

struct DigitMenu
{
	const SBYTE * menuname;			// 菜单名称
	WORD menuself;					// 当前菜单号
	WORD menuparent;				// 上级菜单号
	WORD menuchild;					// 下级菜单号
	WORD menubrother;				// 兄弟级菜单号
	void * menuptr;					// 当前菜单指向的参数指针
	BYTE size;						// 当前菜单指向的参数大小
	BYTE menuattr;					// 菜单属性
};

extern BYTE ver_buff[];
extern BYTE hht_pcb_ciu;
extern BYTE hht_pcb_liop;
extern WORD hht_menu;
extern BYTE dm_get_answer;


WORD handheld_key_read (WORD value);
WORD get_key (BYTE mode);
void ClrWdt (void);
void DispDigitLED(void);
void BoardKeyProc(void);
void ConductLedDisplay(void);
BYTE card_identification(BYTE* buf);
void card_read_call(BYTE* buf, BYTE type, BYTE bus);

#endif

