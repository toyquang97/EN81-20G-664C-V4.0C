
#include "typedef.h"
#include "Lpc1700.h"
#include "parameter.h"
#include "state.h"
#include "io.h"
#include "can.h"
#include "call.h"
#include "rtc.h"
#include "text.h"
#include "version.h"
#include "history.h"
#include "eeprom.h"
#include "bootloader.h"
#include "nice5000.h"
#include "stdio.h"
#include "string.h"


BYTE state_msi;
BYTE state_sgm;
BYTE msi_trans_state = 0;
DWORD msi_trans_num = 0;
DWORD msi_trans_check = 0;
BYTE state_ulsi = SWITCH_OFF;
BYTE state_run = LED_OFF;
BYTE state_psl = 0;
BYTE psl_chk_on = 0;
BYTE psl_chk_off = 0;
BYTE psl_oncofirm = 0;
BYTE psl_offcofirm = 0;
DWORD input_psl = 0;

BYTE state_cdsl = 0;
BYTE cdsl_chk_on = 0;
BYTE cdsl_chk_off = 0;
BYTE cdsl_oncofirm = 0;
BYTE cdsl_offcofirm = 0;
DWORD input_cdsl = 0;

BYTE state_sdsl = 0;
BYTE sdsl_chk_on = 0;
BYTE sdsl_chk_off = 0;
BYTE sdsl_oncofirm = 0;
BYTE sdsl_offcofirm = 0;
DWORD input_sdsl = 0;

/****************************************************
* 函数名: SetLed
* 功能: 点亮或熄灭LED
* 入参: index LED索引; state LED状态;
* 出参: 无
* 返回值: 无
****************************************************/
void SetLed (BYTE index, BYTE state)
{
	if (state)
	{
		switch (index)
		{
			case LED_RUNNING:
				state_run = LED_ON;
				RunningLedOn();
				break;
		}
	}
	else
	{
		switch (index)
		{
			case LED_RUNNING:
				state_run = LED_OFF;
				RunningLedOff();
				break;
		}
	}
}

/****************************************************
* 函数名: SetSwitch
* 功能: 开关继电器
* 入参: index 继电器索引; state 继电器状态;
* 出参: 无
* 返回值: 无
****************************************************/
void SetSwitch (BYTE index, BYTE state)
{
	if (state)
		{
			switch (index)
				{
					case SWITCH_ULSI:
						state_ulsi = SWITCH_ON;
						UlsiOn();
						break;

					case SWITCH_MAIN_RELAY:
						OPEN_MAIN_RELAY();
						break;
				}
		}
	else
		{
			switch (index)
				{
					case SWITCH_ULSI:
						state_ulsi = SWITCH_OFF;
						UlsiOff();
						break;
						
					case SWITCH_MAIN_RELAY:
						CLOSE_MAIN_RELAY();
						break;
				}
		}
}

/****************************************************
* 函数名: fan_on
* 功能: 打开风扇
* 入参: 无
* 出参: 无
* 返回值: 无
****************************************************/
void fan_on (void){
	if((p.fan_mode & FAN_FIRE) &&	((callpriority == C_FIREMAN) || (callpriority == C_FIREALARM)))
		{ 			
			fantimer = timer;					/* start or re-trigger fan timer		*/
		}
	else if (p.fan_mode & FAN_AUTOMATIC)											/* automativ cabin fan mode		*/
	{
		if (!fan_state)										/* cabin fan is not on				*/
		{
			fan_state = 1;									/* switch fan on					*/
			if (!fan_switch_status)
				set_out (SPECIAL_FUNC, FAN_1, 0, 0, fan_state, (O_CANA|O_CANB|O_HSE));
		}
	if(((callpriority == C_EVACUATION) && (p.evac_fire_carlightmode & 0x01))
				|| ((callpriority == C_FIREALARM) && (p.evac_fire_carlightmode & 0x02)))
		fantimer = timer + (p.carlight_fan_time_fire SEC); 		/* start car light off timer	*/
	else
		fantimer = timer + (p.fan_time SEC);					/* start or re-trigger fan timer		*/
	}
}

/****************************************************
* 函数名: handheld_key_read
* 功能: 手持操作器按键转换
* 入参: value 键码
* 出参: 无
* 返回值: 键值
****************************************************/
WORD handheld_key_read (WORD value)
{
	switch (value)
	{
		case (KEY_ESC):
		case (0x18):
			return (KEY_ESC);
		case (KEY_UP):
		case (0x2D):
			return (KEY_UP);
		case ('*'):
		case (0x2B):
			return ('*');
		case ('#'):
		case (0x0D):
			return ('#');
		default:
			if ((value >= '0') && (value <= '9'))
				return (value);
	}
	return (0);
}


/****************************************************************************************************/
/*	Read a key on keypad																			*/
/****************************************************************************************************/
WORD get_key (BYTE mode)
{
	static WORD key_old = 0;
	static WORD key = 0;
	static BYTE keytimer = 0;
	WORD help [2] = {0, 0};
	WORD i;

	if (handheld_key)									/* key from handheld terminal					*/
		help [0] = help [1] = handheld_key;
	if (help [0])
	{
		if (passwordtimer)								/* if access enabled							*/
			passwordtimer = PASSWORDTIME;					/* restart password timer						*/
	}
	if (mode && help [0] && (help [0] != '#'))
	{
		key_old = help [0];
		return (help [0]);
	}
	if (((help [0] == '*') || (help [0] == '#')) && (key_old == help [0]))
		keytimer++;
	else
		keytimer = 0;
	key_old = help [0];
	if (keytimer == 10)								/* '*' or '#'  pressed for 1 second				*/
	{
		help [0] = 0;
		if (key_old == '*')
			key = KEY_UP;								/* use this as cursor up key					*/
		else if (key_old == '#')
			key = KEY_ESC;								/* use this as ESC key							*/
	}
	else if (keytimer > 10)
	{
		help [0] = 0;
		key = 0;
		keytimer = 11;
	}
	if (help [0])
		i = 0;
	else
		i = key;
	key = help [0];
	return (i);
}

/****************************************************
* 函数名: ClrWdt
* 功能: 刷看门狗
* 入参: 无
* 出参: 无
* 返回值: 无
****************************************************/
void ClrWdt (void)
{
	static BYTE wdtstate = 0;

	if (wdtstate)
		WdtOn();
	else
		WdtOff();
	wdtstate ^= 1;
}

const DWORD NUM_TBL[] =
{	// 00				01					02					03					04					05					06					07					08					09					0A					0B					0C					0D					0E					0F
	NUM_agd,	NUM_agD,	NUM_aGd,	NUM_aGD,	NUM_Agd,	NUM_AgD,	NUM_AGd,	NUM_AGD,	NUM_ERR,	NUM_ERR,	NUM_ERR,	NUM_ERR,	NUM_ERR,	NUM_ERR,	NUM_UP,	NUM_DOWN,	// 00
	NUM_bcef,	NUM_bceF,	NUM_bcEf,	NUM_bcEF,	NUM_bCef,	NUM_bCeF,	NUM_bCEf,	NUM_bCEF,	NUM_Bcef,	NUM_BceF,	NUM_BcEf,	NUM_BcEF,	NUM_BCef,	NUM_BCeF,	NUM_BCEf,	NUM_BCEF,	// 01
	NUM_NOP,	NUM_ERR,	NUM_ERR,	NUM_ERR,	NUM_ERR,	NUM_ERR,	NUM_ERR,	NUM_ERR,	NUM_ERR,	NUM_ERR,	NUM_ERR,	NUM_ERR,	NUM_ERR,	NUM_aGd,	NUM_ERR,	NUM_ERR,	// 02
	NUM_0,		NUM_1,		NUM_2,		NUM_3,		NUM_4,		NUM_5,		NUM_6,		NUM_7,		NUM_8,		NUM_9,		NUM_ERR,	NUM_ERR,	NUM_ERR,	NUM_ERR,	NUM_ERR,	NUM_aGd,	// 03
	NUM_ERR,	NUM_A,		NUM_b,		NUM_C,		NUM_d,		NUM_E,		NUM_F,		NUM_G,		NUM_H,		NUM_i,		NUM_J,		NUM_ERR,	NUM_L,		NUM_ERR,	NUM_n,		NUM_O,		// 04
	NUM_P,		NUM_ERR,	NUM_r,		NUM_S,		NUM_T,		NUM_U,		NUM_ERR,	NUM_ERR,	NUM_ERR,	NUM_y,		NUM_ERR,	NUM_ERR,	NUM_ERR,	NUM_ERR,	NUM_ERR,	NUM_agD,	// 05
	NUM_ERR,	NUM_A,		NUM_b,		NUM_C,		NUM_d,		NUM_E,		NUM_F,		NUM_G,		NUM_H,		NUM_i,		NUM_J,		NUM_ERR,	NUM_L,		NUM_ERR,	NUM_n,		NUM_o,		// 06
	NUM_P,		NUM_ERR,	NUM_r,		NUM_S,		NUM_t,		NUM_u,		NUM_ERR,	NUM_ERR,	NUM_ERR,	NUM_y,		NUM_ERR,	NUM_ERR,	NUM_ERR,	NUM_ERR,	NUM_ERR,	NUM_ERR,	// 07
};

//数码管扫描程序
void DispDigitLED(void)
{
	static BYTE disp_cnt = 0;
	DWORD outpin;

	outpin = NUM_TBL[led_buff[disp_cnt]];
	if (led_buff[disp_cnt + 1] == '.')
		outpin |= NUM_DOT;
	switch(disp_cnt)
	{
		case 0:
			LD1_L;
			LD4_H;
			break;
		case 2:
			LD4_L;
			LD3_H;
			break;
		case 4:
			LD3_L;
			LD2_H;
			break;
		case 6:
			LD2_L;
			LD1_H;
			break;
	}
	FIO1CLR = NUM_ALL;
	FIO1SET = outpin;
	disp_cnt = (disp_cnt + 2) % LED_BUFF_LEN;
}

struct DigitMenu * pMenu;
WORD boardkey = 0;
DWORD para_dword_pass;
float para_float_pass;
//BYTE dm_call_target = 1;
BYTE dm_get_answer = 0;
WORD dm_zero = 0;
BYTE ver_buff[10];
BYTE hht_pcb_ciu = 1;
BYTE hht_pcb_liop = 1;
WORD hht_menu = 0;
//BYTE temp_dat = 0;


// 按键扫描函数
void BoardKeyProc(void){
	static WORD key_press_cnt = 0;
	static WORD oldkeystatus = 0;
	WORD keystatus = 0;

	if (ENT_KEY_IN)
		keystatus |= SM_KEY_ENT;
	if (MENU_KEY_IN)
		keystatus |= SM_KEY_MENU;
	if (UP_KEY_IN)
		keystatus |= SM_KEY_UP;

	if (keystatus)
	{
		if (keystatus != oldkeystatus)
		{
			key_press_cnt = 1;
		}
		else
		{
			++ key_press_cnt;
			if (key_press_cnt == KEY_TIMER_600ms)
				boardkey = keystatus | LM_KEY_FLAG;
			else if (key_press_cnt == KEY_TIMER_900ms)
			{
				boardkey = keystatus | LM_KEY_FLAG;
				key_press_cnt = KEY_TIMER_600ms;
			}
		}
	}
	else if (oldkeystatus)
	{
		if (key_press_cnt >= KEY_TIMER_30ms && key_press_cnt < KEY_TIMER_600ms)
			boardkey = oldkeystatus;
		key_press_cnt = 0;
	}
	oldkeystatus = keystatus;
}

const struct DigitMenu C_LED_MENU[] = {
//		菜单名称			当前菜单号				上级菜单号		下级菜单号			兄弟级菜单号				参数指针					参数大小								菜单属性
//		menuname			menuself					menuparent		menuchild				menubrother				menuptr						size										menuattr
	{	"- P 1 - ",	DM_P1,					DM_NULL,		DM_STATE,			DM_P2,					NULL,						0,										D_MENU		},
	{	"P 1.0 1 ",	DM_STATE,				DM_P1,			DM_NULL,			DM_LEVEL,				&hse_state,			sizeof(hse_state),		D_PAR_STATE	},	// 电梯状态
	{	"P 1.0 2 ",	DM_LEVEL,				DM_P1,			DM_NULL,			DM_LSSTATE,				&level,					sizeof(level),				D_PAR_LVL	},	// 当前楼层
	{	"P 1.0 4 ",	DM_LSSTATE,			DM_P1,			DM_NULL,			DM_D1STATE,			&ls_state,			sizeof(ls_state),			D_PAR_DR	},	// 光幕状态
	{	"P 1.0 6 ",	DM_D1STATE,			DM_P1,			DM_NULL,			DM_LOADSTATE,				&door_state[0],	sizeof(door_state[0]),D_PAR_DRSTATE},	// 门1状态
	{	"P 1.0 8 ",	DM_LOADSTATE,	DM_P1,			DM_NULL,			DM_SPEC,				&load,					sizeof(load),					D_PAR_LOAD},	// 称重信息
	{	"P 1.0 9 ",	DM_SPEC,				DM_P1,			DM_NULL,			DM_STATE,				NULL,						0,										D_PAR_SPEC},	// 特殊状态

	{	"- P 2 - ",	DM_P2,					DM_NULL,		DM_TRIPCNT,		DM_P3,					NULL,						0,										D_MENU		},
	{	"P 2.0 1 ",	DM_TRIPCNT,			DM_P2,			DM_NULL,			DM_OPTIME,			&tripcount,			sizeof(tripcount),		D_PAR			},	// 运行次数
	{	"P 2.0 2 ",	DM_OPTIME,			DM_P2,			DM_NULL,			DM_TRIPERR,			&op_time,				sizeof(op_time),			D_PAR_OPTIME	},	// 运行时间
	{	"P 2.0 3 ",	DM_TRIPERR,			DM_P2,			DM_NULL,			DM_MOTO_ANGLE,&triperrorcount,sizeof(triperrorcount),	D_PAR		},	// 报警次数
	{	"P 2.0 4 ",	DM_MOTO_ANGLE,DM_P2,			DM_NULL,			DM_TRIPCNT,			&niv.FH_04,			sizeof(niv.FH_04),		D_PAR		},	// 曳引机角度

	{	"- P 3 - ",	DM_P3,					DM_NULL,		DM_VERMCU,		DM_P4,					NULL,			0,													D_MENU		},
	{	"P 3.0 1 ",	DM_VERMCU,			DM_P3,			DM_NULL,			DM_VERCDU,			(void*)version_hse,	0,								D_PAR_VER	},	// MCU软件版本
	{	"P 3.0 2 ",	DM_VERCDU,			DM_P3,			DM_NULL,			DM_PCUNUM,			(void*)ver_buff,sizeof(ver_buff),			D_PAR_VER	},	// CDU软件版本
	{	"P 3.0 3 ",	DM_PCUNUM,			DM_P3,			DM_NULL,			DM_VERPCU,			&hht_pcb_ciu,		sizeof(hht_pcb_ciu),	D_PAR			},	// 需要查看的 CIU number
	{	"P 3.0 4 ",	DM_VERPCU,			DM_P3,			DM_NULL,			DM_LIOPNUM,		(void*)ver_buff,sizeof(ver_buff),			D_PAR_VER	},	// CIU软件版本
	{	"P 3.0 5 ",	DM_LIOPNUM,		DM_P3,			DM_NULL,			DM_VERLIOP,			&hht_pcb_liop,	sizeof(hht_pcb_liop),	D_PAR			},	// 需要查看的 LIOP number
	{	"P 3.0 6 ",	DM_VERLIOP,			DM_P3,			DM_NULL,			DM_VERDRV,			(void*)ver_buff,sizeof(ver_buff),			D_PAR_VER	},	// LIOP软件版本
	{	"P 3.0 7 ",	DM_VERDRV,			DM_P3,			DM_NULL,			DM_VERMCU,			(void*)version_drv,	0,								D_PAR_VER	},	// 驱动模块软件版本

	{	"- P 4 - ",	DM_P4,					DM_NULL,		DM_TARGET,		DM_P5,					NULL,							0,									D_MENU		},
	{	"P 4.0 1 ",	DM_TARGET,			DM_P4,			DM_NULL,			DM_IDSET,				&dm_zero,				sizeof(dm_zero),			D_PAR		},	// 设置目标楼层
	{	"P 4.0 2 ",	DM_IDSET,				DM_P4,			DM_NULL,			DM_TARGET,			&dm_zero,				sizeof(dm_zero),			D_PAR	},			// 设置外呼ID

	{	"- P 5 - ",	DM_P5,					DM_NULL,		DM_TESTLOAD,	DM_P6,					NULL,						0,										D_MENU		},
	{	"P 5.0 1 ",	DM_TESTLOAD,		DM_P5,			DM_NULL,			DM_DIR,					&bTestLoad,			sizeof(bTestLoad),		D_PAR			},	// 测试载重
	{	"P 5.0 3 ",	DM_DIR,					DM_P5,			DM_NULL,			DM_RESET,				&niv.F0_05,			sizeof(niv.F0_05),		D_PAR			},	// 设置运行方向
	{	"P 5.0 4 ",	DM_RESET,				DM_P5,			DM_NULL,			DM_EMOP_ENILS,	&bForceResetTrip,	sizeof(bForceResetTrip),D_PAR	},	// 执行复位运行
	{	"P 5.0 5 ",	DM_EMOP_ENILS,	DM_P5,			DM_NULL,			DM_TUNE,				&bTest_IlsSwitch,	sizeof(bTest_IlsSwitch),D_PAR	},	// 测试蹲底
	{	"P 5.0 6 ",	DM_TUNE,				DM_P5,			DM_NULL,			DM_BREAK,			&p.motor_tuned,		sizeof(p.motor_tuned),	D_PAR	},	// 主机自整定
	{	"P 5.0 7 ",	DM_BREAK,			DM_P5,			DM_NULL,			DM_FXFUN,			&bBreak_check_enable,	sizeof(bBreak_check_enable),	D_PAR	},	// 取消抱闸反馈
	{	"P 5.0 8 ",	DM_FXFUN,			DM_P5,			DM_NULL,			DM_MAX_SPEED,	&bFx_check_enable,		sizeof(bFx_check_enable),		D_PAR	},	// 使能/取消封星功能
	{	"P 5.0 9 ",	DM_MAX_SPEED,	DM_P5,			DM_NULL,			DM_TEMPERA,		&niv.F0_03,			sizeof(niv.F0_03),		D_PAR			},	// 设置运行最大速度
	{	"P 5.1 0 ",	DM_TEMPERA,		DM_P5,			DM_NULL,			DM_SIL_TEST,		&bTemp_check_enable,	sizeof(bTemp_check_enable),		D_PAR			},	// 控制柜温度检测使能
	{	"P 5.1 1 ",	DM_SIL_TEST,		DM_P5,			DM_NULL,			DM_RESET_UCMP,&bTest_SIL,			sizeof(bTest_SIL),		D_PAR			},	// 测试限位开关功能以及安装距离
	{	"P 5.1 2 ",	DM_RESET_UCMP,DM_P5,				DM_NULL,			DM_BRAKE_TEST,		&ucmp_state,			sizeof(ucmp_state),		D_PAR			},	// 安全回路的连接模式
	{	"P 5.1 3 ",	DM_BRAKE_TEST,DM_P5,				DM_NULL,			DM_BRAKE_VALUE,		&bTest_brake,			sizeof(bTest_brake),		D_PAR			},	// 安全回路的连接模式
	{	"P 5.1 4 ",	DM_BRAKE_VALUE,DM_P5,			DM_NULL,			DM_TEST_UCMP,		&brake_value,			sizeof(brake_value),		D_PAR			},	// 安全回路的连接模式
	{	"P 5.1 6 ",	DM_TEST_UCMP,	DM_P5,			DM_NULL,			DM_TESTLOAD,		&bTestUCMP,			sizeof(bTestUCMP),		D_PAR			},	// 安全回路的连接模式


	{	"- P 6 - ",	DM_P6,					DM_NULL,		DM_TEACH,			DM_P7,					NULL,						0,										D_MENU		},
	{	"P 6.0 2 ",	DM_TEACH,			DM_P6,			DM_NULL,			DM_F2_11,				&teach_in,				sizeof(teach_in),	D_PAR	},	// 井道自学习
	{	"P 6.0 3 ",	DM_F2_11,				DM_P6,			DM_NULL,			DM_F2_12,				&niv.F2_11,				sizeof(niv.F2_11),			D_PAR	},	// 测试载重
	{	"P 6.0 4 ",	DM_F2_12,				DM_P6,			DM_NULL,			DM_F2_13,				&niv.F2_12,				sizeof(niv.F2_12),			D_PAR	},	// 设置打滑功能
	{	"P 6.0 5 ",	DM_F2_13,				DM_P6,			DM_NULL,			DM_F2_14,				&niv.F2_13,				sizeof(niv.F2_13),			D_PAR	},	// 设置运行方向
	{	"P 6.0 6 ",	DM_F2_14,				DM_P6,			DM_NULL,			DM_F2_15,				&niv.F2_14,				sizeof(niv.F2_14),			D_PAR	},	// 测试载重
	{	"P 6.0 7 ",	DM_F2_15,				DM_P6,			DM_NULL,			DM_TEACH,			&niv.F2_15,				sizeof(niv.F2_15),			D_PAR	},	// 设置打滑功能

	{ "- P 7 - ", DM_P7,					DM_NULL,			DM_ERR_LIST,	DM_P1,				NULL, 						0,											D_MENU},	// 故障记录
	{	"P 7.n n ",	DM_ERR_LIST,		DM_P7,				DM_ERR_DET1,	DM_NULL,			NULL, 						0,											D_MENU},	// 
	{	"7.n n.1 ",	DM_ERR_DET1,		DM_ERR_LIST,	DM_NULL,		DM_ERR_DET2,		&para_dword_pass,sizeof(para_dword_pass),D_PAR_ERRTIMES},	// 记录详细的1errorcode
	{	"7.n n.2 ",	DM_ERR_DET2,		DM_ERR_LIST,	DM_NULL,		DM_ERR_DET3,		&para_dword_pass,sizeof(para_dword_pass),D_PAR},	// 记录详细的2:year
	{	"7.n n.3 ",	DM_ERR_DET3,		DM_ERR_LIST,	DM_NULL,		DM_ERR_DET4,		&para_dword_pass,sizeof(para_dword_pass),D_PAR_MONTH	},	// 记录详细的3:month,day
	{	"7.n n.4 ",	DM_ERR_DET4,		DM_ERR_LIST,	DM_NULL,		DM_ERR_DET5,		&para_dword_pass,sizeof(para_dword_pass),D_PAR_MONTH	},	// 记录详细的4:hour,minute
	{	"7.n n.5 ",	DM_ERR_DET5,		DM_ERR_LIST,	DM_NULL,		DM_ERR_DET6,		&para_dword_pass,sizeof(para_dword_pass),D_PAR},	// 记录详细的5:second
	{	"7.n n.6 ",	DM_ERR_DET6,		DM_ERR_LIST,	DM_NULL,		DM_ERR_DET1,		&para_dword_pass,sizeof(para_dword_pass),D_PAR},	// 记录详细的6: error_count

	{	"- P 8 - ",	DM_P8,					DM_NULL,		DM_YEAR_TMR,	DM_P1,					NULL,						0,											 	D_MENU},
	{	"P 8.0 1 ",	DM_YEAR_TMR,		DM_P8,			DM_NULL,			DM_MON_TMR,		&t.year,				sizeof(t.year),				D_PAR_YEAR},	// 年
	{	"P 8.0 2 ",	DM_MON_TMR,		DM_P8,			DM_NULL,			DM_DAY_TMR,		&t.month,				sizeof(t.month),					D_PAR	 },	// 月
	{	"P 8.0 3 ",	DM_DAY_TMR,		DM_P8,			DM_NULL,			DM_HOUR_TMR,	&t.day,					sizeof(t.day),						D_PAR	 },	// 日
	{	"P 8.0 4 ",	DM_HOUR_TMR,	DM_P8,			DM_NULL,			DM_MIN_TMR,		&t.hour,				sizeof(t.hour),						D_PAR	 },	// 时
	{	"P 8.0 5 ",	DM_MIN_TMR,		DM_P8,			DM_NULL,			DM_SEC_TMR,		&t.minute,			sizeof(t.minute),					D_PAR	 },	// 分
	{	"P 8.0 6 ",	DM_SEC_TMR,		DM_P8,			DM_NULL,			DM_YEAR_TMR,		&t.second,			sizeof(t.second),					D_PAR	 },	// 秒

};

// 显示参数内容
void RenderLedDisplay(void){
	SBYTE buff[10];
	DWORD value = 0, value2 = 0;
	BYTE len = 0;
	BYTE dot = 0;
	BYTE i;
	BYTE j;
	BYTE pos = LED_BUFF_LEN - 2;
	BYTE * ptr;

	switch(pMenu->size)
	{
		case 1:
			value = *((BYTE *)pMenu->menuptr);
			if((pMenu->menuattr == D_PAR_DRIO) && (p.doornumber > 1))
				value2 = *(((BYTE *)pMenu->menuptr) + pMenu->size);
			break;
		case 2:
			value = *((WORD *)pMenu->menuptr);
			if((pMenu->menuattr == D_PAR_DRSTATE) && (p.doornumber > 1))
				value2 = door_state[1];
			break;
		case 4:
			value = *((DWORD *)pMenu->menuptr);
			break;
	}
	memset(led_buff, 0x00, LED_BUFF_LEN);

	switch(pMenu->menuattr)
	{
		case D_PAR_LVL:
			if (value == 0xff)
				strcpy(buff, "----");
			else
				sprintf(buff, "%d", value+1);
			break;
			
		case D_PAR_DR:
			for (i=0; i<p.doornumber; ++i)
			{
				if (value & (0x01 << i))
					led_buff[pos - 2*i] = '1';		// '1'表示门已关闭
				else
					led_buff[pos - 2*i] = '0';		// '0'表示门未关闭
			}
			return;
			
		case D_PAR_SAF:
			if (value & SC3)
				led_buff[6] = '1';					// '1'表示安全回路闭合
			else
				led_buff[6] = '0';					// '0'表示安全回路断开
			if (value & SC2)
				led_buff[4] = '1';					// '1'表示安全回路闭合
			else
				led_buff[4] = '0';					// '0'表示安全回路断开
			if (value & SC1)
				led_buff[2] = '1';					// '1'表示安全回路闭合
			else
				led_buff[2] = '0';					// '0'表示安全回路断开
			return;
			
		case D_PAR_SENSOR:
			led_buff[6] = 0;
			value = 0;
			if (door_zone & POS_ILU)
				value |= 0x01;
			if (door_zone & POS_ILD)
				value |= 0x02;
			led_buff[6] = '0' + value;				
			return;

		case D_PAR_DRSTATE:
			i =  (BYTE)(value2 & 0x0000000F);
			if(i < 10)
				i += '0';
			else if(i < 16)
				i += 'A';
			led_buff[4] = i;
			i =  (BYTE)(value & 0x0000000F);
			if(i < 10)
				i += '0';
			else if(i < 16)
				i += 'A';
			led_buff[6] = i;
			return;
			
		case D_PAR_DRIO:
			if ((value == NOT_AVAILABLE) && (value2 == NOT_AVAILABLE))
				strcpy(buff, "----");
			else
			{
				led_buff[6] = '0';
				led_buff[4] = '0';
				if (value & DOOR_OP)
					led_buff[6] = '1';		
				else if (value & DOOR_CL)
					led_buff[6] = '2';		
				else if (value & DOOR_REV)
					led_buff[6] = '3';		
				if (value2 & DOOR_OP)
					led_buff[4] = '1';		
				if (value2 & DOOR_CL)
					led_buff[4] = '2';		
				if (value2 & DOOR_REV)
					led_buff[4] = '3';		
				return;
			}
			break;

		case D_PAR_LOAD:			
			if(load & OVERLOAD)
				led_buff[6] = '8';
			else if(load & FULL_LOAD)
				led_buff[6] = '4';
			else if(load & HALF_LOAD)
				led_buff[6] = '2';
			else				
				led_buff[6] = '0';
			return;
			
		case D_PAR_SPEC:
			sprintf(buff, "%2d", para_dword_pass);
			break;
			
		case D_PAR_MONTH:
			i = (para_dword_pass >> 8) & 0xff;
			j = para_dword_pass & 0xff;
			sprintf((char*)led_buff, "%d %d.%d %d ", i/10, i%10, j/10, j%10);
			return;
			
		case D_PAR_VER:
			ptr = (BYTE *)pMenu->menuptr;
			i = 0;
			j = 0;
			len = 0;
			while(*ptr)
			{
				if (NUM_TBL[*ptr] != NUM_ERR)
				{
					led_buff[i] = *ptr;
					len = 1;
				}
				if (*(ptr+1) == '.')
				{
					led_buff[i+1] = '.';
					len = 1;
				}
				if (len)
				{
					i += 2;
					len = 0;
					if (i >= LED_BUFF_LEN)
						break;
				}
				++ ptr;
				++ j;
				if (j > SW_SIZE)
					break;
			}
			return;
			
		case D_PAR_ERRTIMES:
			if (value)
				sprintf(buff, "E%03d", value);
			else
				strcpy(buff, "----");
			break;
			
		case D_PAR_HEX:
			sprintf(buff, "%02X", value);
			break;

//显示电梯状态
		case D_PAR_STATE:			
			value2 = 0;
			if(value == H_DRIVE_ERROR)
				{
					if(driveerror & D_ERRORSTATE)
						{
							if(nice5000_Rfault_errcode)
								value2 = nice5000_Rfault_errcode;
						}
					else if(driveerror)
						{
							if(driveerror & D_RUNCONT_NOTOFF)
								value2 = E_RUNCONTACT_NOTOFF;
							else if(driveerror & D_RUNCONT_NOTON)
								value2 = E_RUNCONTACT_NOTON;
							else if(driveerror & D_BRKCONT_NOTOFF)
								value2 = E_BRKCONTACT_NOTOFF;
							else if(driveerror & D_BRKCONT_NOTON)
								value2 = E_BRKCONTACT_NOTON;
							else if(driveerror & D_FXCONT_NOTOFF)
								value2 = E_FXCONTACT_NOTOFF;
							else if(driveerror & D_FXCONT_NOTON)
								value2 = E_FXCONTACT_NOTON;
							
							else if(driveerror & D_BRAKE_NOTOFF)
								value2 = E_BRAKE_NOTOFF;
							else if(driveerror & D_BRAKE_NOTON)
								value2 = E_BRAKE_NOTON;
							else if(driveerror & D_NICE_MOVE_NOTOFF)
								value2 = E_NICE_MOVE_NOTOFF;
							else if(driveerror & D_NICE_MOVE_NOTON)
								value2 = E_NICE_MOVE_NOTON;									
								
							else if(driveerror & D_NICE_RUN_NOTOFF)
								value2 = E_NICE_RUN_NOTOFF;
							else if(driveerror & D_NICE_RUN_NOTON)
								value2 = E_NICE_RUN_NOTON;
							else if(driveerror & D_NICE_BRAKE_NOTOFF)
								value2 = E_NICE_BRAKE_NOTOFF;
							else if(driveerror & D_NICE_BRAKE_NOTON)
								value2 = E_NICE_BRAKE_NOTON;
							
							else if(driveerror & D_NO_SC)
								{
									if(!ucmp_state)
										value2 = E_SC_VOLTAGE_MOVE;
									else										
										value2 = E_UCMP_LOCK;
								}
							else if(driveerror & D_NICE_DIR_NOTON)
								value2 = E_NICE_DIR_NOTON;
						}
				}
			else if(value == H_ERROR)
				{
					if(poserror & P_KH5_OFF)						
						value2 = E_KH5_OFF;
					else if(poserror & P_KH5_ON)						
						value2 = E_KH5_ON;
					else if(poserror & P_DIVING_ERR)						
						value2 = E_DIVING_ERR;
					else if(poserror & (P_ILO_ON | P_ILU_ON	))					
						value2 = E_ILS_ERROR;					
					else if(poserror & P_VO_VU_ERROR)						
						value2 = E_VO_VU_ERROR;

					else if(general_error & G_MSI_NOTOFF)						
						value2 = E_MSI_NOTOFF;
					else if(general_error & G_LIGHTSCREEN1)						
						value2 = E_DOOR_LS_1;
					else if(general_error & G_LIGHTSCREEN2)						
						value2 = E_DOOR_LS_2;
					else if(general_error & G_TRIPTIME)						
						value2 = E_TRIP_TIME;
					else if(general_error & G_DL_SHORTED)						
						value2 = E_DL_SHORTED_CAR;
					else if(general_error & G_CLOSEDOOR1)						
						value2 = E_DOOR_CLOSING_1;
					else if(general_error & G_CLOSEDOOR2)						
						value2 = E_DOOR_CLOSING_2;
					else if(general_error & G_OPENDOOR1)						
						value2 = E_DOOR_OPENING_1;
					else if(general_error & G_CLOSEDOOR2)						
						value2 = E_DOOR_OPENING_2;
				} 
			else if(value == H_ERROR_RELEV)
				{
					if (temperature & OVERTEMP1)
						value2 = E_MOTOR_TEMP1;
					else if(temperature & OVERTEMP2)
						value2 = E_MOTOR_TEMP2;
					else if(temperature & ROOMTEMP_MIN)
						value2 = E_ROOM_TEMP_MIN;
					else if(temperature & ROOMTEMP_MAX)
						value2 = E_ROOM_TEMP_MAX;
					else if(!carlight)
						value2 = E_LIGHT_VOLTAGE;
					else if(limit_switch_error)
						value2 = E_SC_VOLTAGE_LIMIT;
					else if(steel_broke)
						value2 = E_STEEL_BROKE;
				}
			else if(value == H_HB_ERROR)
				{
					if (hb_error & B_DRV)
						value2 = E_HEARTBEAT_DRV;
					else if (hb_error & B_FVE)
						value2 = E_HEARTBEAT_CDU;
					else if (exe_hb_error & 0x03)
						value2 = E_HEARTBEAT_EXE;
					else
						value2 = E_HEARTBEAT;
				}
			else if(ucmp_state)
				value2 = E_UCMP_LOCK;
			else if(brake_value == BRAKE_NO_GOOD)
				value2 = E_BRAKE_CHECK;

			else
				value2 = 0;

			if(value2)
				{
					if(value2 > 1000)
						{
							i = 0;
							while (errortext [i].number != ENDSTRING) 		/* search text structure					*/
								{
									if (errortext [i].number == value2)
										{
											value = errortext[i].err_code;
											break;
										}
									i++;
								} 			
						}
					else
						value = value2;
					sprintf(buff, "E%03d", value);
				}
			else	
				{
					if(sec_cnt % 3)
						sprintf(buff, "%d", value);
					else
						{
							if(move_dooropen_flag)
								{//DOP1/DOP2
									led_buff[0] = 'd';
									led_buff[2] = 'O';
									led_buff[4] = 'P';
									led_buff[6] = '0' + move_dooropen_flag;
									return;
								}
							else if(door_zone & (POS_ILU | POS_ILD))
								{//ULIL
									led_buff[0] = 0;
									led_buff[4] = 'I';
									led_buff[6] = 'L';
									if(door_zone & POS_ILU)
										led_buff[2] = 'U';
									else										
										led_buff[2] = 'L';
									return;
								}
							else if(bFunc_flag & FUNC_SDD1_SDU1_ON)
								{//U1D1
									led_buff[0] = 'd';
									led_buff[2] = '1';
									led_buff[4] = 'U';
									led_buff[6] = '1' + move_dooropen_flag;
									return;
								}
							else if(door_short_conn)
								{//SH1/SH2									
									led_buff[0] = 0;
									led_buff[2] = 'S';
									led_buff[4] = 'H';
									led_buff[6] = '0' + door_short_conn;
									return;
								}
							else if(car_attendance)
								{//ATTE									
									led_buff[0] = 'A';
									led_buff[2] = 'T';
									led_buff[4] = 'T';
									led_buff[6] = 'E';
									return;
								}
							else if(firemode | firealarmmode)
								{//FIRE									
									led_buff[0] = 'F';
									led_buff[2] = 'I';
									led_buff[4] = 'R';
									led_buff[6] = 'E';
									return;
								}
							else if(landings_off | landings_off_key)
								{//HOFF									
									led_buff[0] = 'H';
									led_buff[2] = 'O';
									led_buff[4] = 'F';
									led_buff[6] = 'F';
									return;
								}
							else if(overload | fullload)
								{//OVER/FULL
									if(overload)
										{											
											led_buff[0] = 'O';
											led_buff[2] = 'U';
											led_buff[4] = 'E';
											led_buff[6] = 'R';
										}
									else if(fullload)
										{											
											led_buff[0] = 'F';
											led_buff[2] = 'U';
											led_buff[4] = 'L';
											led_buff[6] = 'L';
										}
									return;
								}
							else if(doorstopstate)
								{//STOP									
									led_buff[0] = 'S';
									led_buff[2] = 'T';
									led_buff[4] = 'O';
									led_buff[6] = 'P';
									return;
								}
							else if(car_switched_off | remote_off)
								{//LOFF
									led_buff[0] = 'L';
									led_buff[2] = 'O';
									led_buff[4] = 'F';
									led_buff[6] = 'F';
									return;
								}
							else if((callpriority == C_ADVANCED) ||	(timer1_trip | timer_trip))
								{//CLOC
									led_buff[0] = 'C';
									led_buff[2] = 'L';
									led_buff[4] = 'O';
									led_buff[6] = 'C';									
									return;
								}
							else if(bTestUCMP)
								{//UCP
									led_buff[0] = 0;
									led_buff[2] = 'U';
									led_buff[4] = 'C';
									led_buff[6] = 'P';									
									return;
								}
							else if(bTest_brake)
								{									
									led_buff[0] = 0;
									led_buff[2] = 'b';
									led_buff[4] = 'C';
									led_buff[6] = 'H';									
									return;
								}
							else if(pitbox_reset_active & PIO_1)
								{
									led_buff[0] = 0;
									led_buff[2] = 'P';
									led_buff[4] = 'i';
									led_buff[6] = 't';									
									return;
								}
							else
								{
									if(direction_ind & 0x30)
										{
											if(direction_ind & 0x10)
												led_buff[6] = 0x0E;
											else
												led_buff[6] = 0x0F;
										}
									if(level >= 9)
										{
											led_buff[0] = '0' + (level + 1) / 10;
											led_buff[2] = '0' + ((level + 1) % 10);
											led_buff[4] = 'F';
										}
									else
										{
											led_buff[0] = '0' + ((level + 1) % 10);													
											led_buff[2] = 'F';
											led_buff[4] = 0;
										}
									return;
								}
						}
				}
			break;

		default:
			if (pMenu->menuattr == D_PAR_YEAR)
				value += 2000;
			else if (pMenu->menuattr == D_PAR_OPTIME)
				value /= 3600;
			sprintf(buff, "%d", value);
			break;
	}

	len = strlen(buff);
	if (len > 4)
	{
		dot = len - 4;
		len = 4;
	}
	while(len)
	{
		led_buff[pos] = buff[len - 1];
		if (dot)
		{
			led_buff[pos + 1] = '.';
			-- dot;
		}
		-- len;
		if (pos >= 2)
			pos -= 2;
		else
			break;
	}
}

// 数码管菜单主函数
void ConductLedDisplay(void){
	static WORD menu_num = DM_STATE;		//当前菜单名
	static BYTE menu_first = 1;	
	static BYTE cont_first = 1;
	static BYTE menu_sel = 1;	
	static BYTE edit_pos = 0;
	static DWORD last_value;
	static BYTE flash_flag = 0;
	static BYTE flash_cnt = 0;
	static BYTE menu_size = 0;
	static BYTE menu_pos = 0;
	static DWORD flash_timer = 0;
	static DWORD longkey_detect_timer = 0;
	static BYTE buff[10];
	static BYTE keytimer_set = 0;
	static BYTE teach_flag = 0;
	WORD i, address = 0;
	DWORD current_value = 0;
	BYTE buffer[10];
	static BYTE ucmp_flag = 0;
	static BYTE brake_flag = 0;

// 同时按下主板上的 ENT MENU UP 三个键,紧急电动开关拔至 ON 的位置
// 保持 10 秒将自动执行 set_parameter 函数,但是不保存参数
//	if ((ENT_KEY_IN) && (MENU_KEY_IN) && (UP_KEY_IN) && (ERO_BTN_IN))

	if ((ENT_KEY_IN) && (MENU_KEY_IN) && (UP_KEY_IN) && (em_op == INSP_ON))
		{
			if (!longkey_detect_timer)
				longkey_detect_timer = timer + 10 SEC;
			else
				{
					if (longkey_detect_timer < timer)
						{
							set_parameter();
							strcpy((char*)buff, "7777");
							flash_cnt = 0xff;
							menu_sel = 0xff;
							longkey_detect_timer = 0;
						}
				}
		}
	else
		longkey_detect_timer = 0;

	if((!menu_keytimer) && (!teach_flag))
		{
			if(keytimer_set)
				{
					menu_num = DM_STATE;
					menu_first = 1;
					menu_sel = 1;
					keytimer_set = 0;
				}
		}

	if (menu_first)
		{
			i = 0;
			while(C_LED_MENU[i].menuself != menu_num)
				{
					++ i;
					if(i >= sizeof(C_LED_MENU)/sizeof(struct DigitMenu))
						break;
				}
			if(i < sizeof(C_LED_MENU)/sizeof(struct DigitMenu))
				{
					pMenu = (struct DigitMenu *)&C_LED_MENU[i];
					menu_first = 0;
					cont_first = 1;
				}
			else
				{
					menu_first = 1;
					return;		//菜单不匹配，退出
				}
		}

	if (!menu_sel)			// 选择菜单
		{
			if (cont_first)
				{
					strcpy((char *)led_buff, pMenu->menuname);
					if (pMenu->menuself == DM_ERR_LIST)
						{
							led_buff[4] = '0' + (menu_pos + 1) / 10;
							led_buff[6] = '0' + (menu_pos + 1) % 10;
						}
					else if (pMenu->menuparent == DM_ERR_LIST)
						{
							led_buff[2] = '0' + (menu_pos + 1) / 10;
							led_buff[4] = '0' + (menu_pos + 1) % 10;
						}
					cont_first = 0;
				}
			switch(boardkey)
			{
				case SM_KEY_MENU:
					if (pMenu->menuparent != DM_NULL)
						{
							menu_num = pMenu->menuparent;
							menu_first = 1;
							flash_flag = 0;
						}
					break;
					
				case SM_KEY_ENT:
					flash_flag = 0;
					if (pMenu->menuself == DM_VERCDU ||
							pMenu->menuself == DM_VERPCU ||
							pMenu->menuself == DM_VERLIOP)
						{
							menu_sel = 0xfe;
							flash_cnt = 0xff;
						}
					else if (pMenu->menuchild != DM_NULL)
						{
							if (pMenu->menuchild == DM_ERR_LIST)
								{
									menu_pos = 0;
									menu_size = GetBriefHist();
									if (!menu_size)
										{
											strcpy((char*)buff, "----");
											flash_cnt = 0xff;
											menu_sel = 0xff;
											break;
										}
								}
							menu_num = pMenu->menuchild;
							menu_first = 1;
						}
					else
						{
							menu_sel = 1;
							cont_first = 1;
						}
					break;
					
				case SM_KEY_UP:
				case LM_KEY_UP:
					if (pMenu->menuself == DM_ERR_LIST)
						{
							if (menu_size >= 2)
								{
									++ menu_pos;
									if (menu_pos == menu_size)
										menu_pos = 0;
									cont_first = 1;
								}
						}
					else if (pMenu->menubrother != DM_NULL)
						{
							menu_num = pMenu->menubrother;
							menu_first = 1;
							if (flash_flag)
								flash_timer = mstimer + FLASH_TIMER_500ms;
						}
					break;
			}
			if ((flash_flag) && (flash_timer < mstimer))
				{
					flash_flag = 0;
					if (pMenu->menuself == DM_VERCDU ||
							pMenu->menuself == DM_VERPCU ||
							pMenu->menuself == DM_VERLIOP)
						{
							menu_sel = 0xfe;
							flash_cnt = 0xff;
						}
					else
						{
							menu_sel = 1;
							cont_first = 1;
						}
				}
		}
	else if (menu_sel == 1)			// 显示参数内容
	{
		switch (menu_num)
		{
			case DM_SENSOR:
				current_value = (DWORD)door_zone;
				break;

			case DM_SPEC:
				current_value = D_NORMAL;
				if (firemode)
					current_value = D_FIRE;
				else if (callpriority == C_FIREALARM)
					current_value = D_FIREALARM;
				else if (earthquake)
					current_value = D_EARTHQUAKE;
				else if ((callpriority == C_EMERGENCY) || (calltype == EMERGENCY_CALL))
					current_value = D_EMERGENCY;
				else if ((callpriority == C_ADVANCED) || (calltype == ADVANCED_CALL))
					current_value = D_ADVANCED;
				else if ((callpriority == C_SPECIAL) || (calltype == SPECIAL_CALL))
					current_value = D_SPECIAL;
				else if (car_switched_off)
					current_value = D_LIFT_OFF;
				else if (intermediate_door)
					current_value = D_INTERM_DOOR;
				else if (remote_off)
					current_value = D_REMOTE_OFF;
				else if (landings_off || landings_off_key)
					current_value = D_LANDINGS_OFF;
				else if (fullload)
					current_value = D_FULL_LOAD;
				else if (auto_call)
					current_value = D_AUTOCALL;
				else if (parkcall)
					current_value = D_PARKTRIP;
				else if (timer2_trip)
					current_value = D_TIMER2;
				else if (timer1_trip || timer_trip)
					current_value = D_TIMER1;
				else if (car_attendance)
					current_value = D_ATTENDANCE;
				else if (up_peak_time || up_peak_input)
					current_value = D_UP_PEAK;
				else if (dn_peak_time || dn_peak_input)
					current_value = D_DN_PEAK;
				else if(callpriority == C_EVACUATION) 			
					current_value = D_EVACTION;
				else if(ucmp_state)
					current_value = D_UCMPLOCK;
				else if(emp_power)
					current_value = D_EMPPOWER;
				else if (door_zone & POS_ILU)
					current_value = D_ILO;
				else if (door_zone & POS_ILD)
					current_value = D_ILU;						
				else if(temperature & (OVERTEMP1 | OVERTEMP2 | ROOMTEMP_MIN | ROOMTEMP_MAX))
					current_value = D_OVERTEMP;
				else if(door_short_conn)
					current_value = D_DOOR_CONNECT;
				else if(brake_value == BRAKE_NO_GOOD)
					current_value = D_BRAKE_CHECK;
				para_dword_pass = current_value;
				break;

			case DM_ERR_DET1:
			case DM_ERR_DET2:
			case DM_ERR_DET3:
			case DM_ERR_DET4:
			case DM_ERR_DET5:
			case DM_ERR_DET6:
				current_value = brief_err_info[menu_pos][(menu_num &0x000f ) - 1];
				if (menu_num == DM_ERR_DET2)
					current_value += 2000;
				para_dword_pass = current_value;
				break;
				
			case DM_TARGET:
				current_value = 1;		//都是从 1 楼开始
				para_dword_pass = current_value;
				break;
				
			case DM_FXFUN:
				current_value = p.fx_active;		
				bFx_check_enable = p.fx_active;
				para_dword_pass = current_value;
				break;
				
			case DM_TEMPERA:
				if(p.lift_func_sel1 & LIFTFUN1_CHECK_ROOM_TEMP)
					{
						current_value = 1;		
						bTemp_check_enable = 1;
					}
				else
					{
						current_value = 0;		
						bTemp_check_enable = 0;
					}
				para_dword_pass = current_value;
				break;
				
			case DM_IDSET:
				current_value = 0;		//都是从 0 楼开始
				para_dword_pass = current_value;
				break;
				
			case DM_RESET_UCMP:
				para_dword_pass = ucmp_state;
				break;

			case DM_STATE:
				if(sec_cnt % 3)			///1.5s闪烁		
					current_value = *((BYTE *)pMenu->menuptr);
				else
					{
						current_value = 0;		//切换显示
					}
				break;

			default:
				switch(pMenu->size)
				{
					case 1:
						current_value = *((BYTE *)pMenu->menuptr);
						break;
					case 2:
						current_value = *((WORD *)pMenu->menuptr);
						break;
					case 4:
						current_value = *((DWORD *)pMenu->menuptr);
						break;
				}
				break;
		}
		if (cont_first)
		{
			last_value = ~current_value;
			cont_first = 0;
		}
		if ((last_value != current_value) || (ucmp_flag != ucmp_state) || (brake_flag != brake_value))
		{
			last_value = current_value;
			if(ucmp_flag != ucmp_state)
				ucmp_flag = ucmp_state;
			if(brake_flag != brake_value)
				brake_flag = brake_value;
			RenderLedDisplay();
		}
		switch(boardkey)
		{
			case SM_KEY_MENU:
				menu_sel = 0;
				cont_first = 1;				
				teach_flag = 0;
				break;
			case SM_KEY_UP:
			case LM_KEY_UP:
				if (pMenu->menubrother != DM_NULL)
				{
					menu_num = pMenu->menubrother;
					menu_first = 1;
					menu_sel = 0;
					flash_flag = 1;
					teach_flag = 0;
					flash_timer = mstimer + FLASH_TIMER_500ms;
				}
				break;
			case SM_KEY_ENT:
				switch (pMenu->menuself)
					{
						case DM_TARGET:
						case DM_IDSET:
						case DM_LIOPNUM:
						case DM_PCUNUM:
						case DM_TESTLOAD:
						case DM_DIR:
						case DM_RESET:
						case DM_TUNE:
						case DM_BREAK:			
						case DM_F2_11:		
						case DM_F2_12:			
						case DM_F2_13:			
						case DM_F2_14:			
						case DM_F2_15:			
						case DM_EMOP_ENILS:
						case DM_TEACH:
						case DM_FXFUN:		
						case DM_MAX_SPEED:
						case DM_TEMPERA:
						case DM_SIL_TEST:
						case DM_RESET_UCMP:
						case DM_BRAKE_TEST:
						case DM_TEST_UCMP:
							para_dword_pass = current_value;
							last_value = ~para_dword_pass;
							menu_sel = 2;		
							edit_pos = 0;							
							teach_flag = 0;
							break;
					}
				break;
		}
	}
	else if (menu_sel == 2)						// 十进制编辑数据
	{
		if (last_value != para_dword_pass)
		{
			last_value = para_dword_pass;
			sprintf((char*)buff, "%04d", para_dword_pass);
			memset(led_buff, 0x00, LED_BUFF_LEN);
			led_buff[0] = buff[0];
			led_buff[2] = buff[1];
			led_buff[4] = buff[2];
			led_buff[6] = buff[3];
			flash_timer = mstimer + FLASH_TIMER_1S;
			flash_flag = 0;
		}
		else if (flash_timer <= mstimer)
		{
			flash_timer = mstimer + FLASH_TIMER_1S;
			flash_flag = 0;
			switch (edit_pos)
			{
				case 3:
					led_buff[0] = buff[0];
					break;
				case 2:
					led_buff[2] = buff[1];
					break;
				case 1:
					led_buff[4] = buff[2];
					break;
				case 0:
					led_buff[6] = buff[3];
					break;
			}
		}
		else if (flash_timer <= mstimer + FLASH_TIMER_500ms)
		{
			if (!flash_flag)
			{
				switch (edit_pos)
				{
					case 3:
						led_buff[0] = 0;
						break;
					case 2:
						led_buff[2] = 0;
						break;
					case 1:
						led_buff[4] = 0;
						break;
					case 0:
						led_buff[6] = 0;
						break;
				}
				flash_flag = 1;
			}
		}

		switch(boardkey)
		{
			case SM_KEY_MENU:
				menu_sel = 1;
				cont_first = 1;
				flash_flag = 0;
				break;
			case SM_KEY_UP:
			case LM_KEY_UP:
				switch (edit_pos)
				{
					case 3:
						para_dword_pass = (para_dword_pass + 1000ul) % 10000ul;
						break;
					case 2:
						para_dword_pass = (para_dword_pass + 100ul) % 10000ul;
						break;
					case 1:
						para_dword_pass = (para_dword_pass + 10ul) % 10000ul;
						break;
					case 0:
						if((pMenu->menuself == DM_DIR) || (pMenu->menuself == DM_TESTLOAD)
							|| (pMenu->menuself == DM_RESET) || (pMenu->menuself == DM_TUNE)
							|| (pMenu->menuself == DM_BREAK) || (pMenu->menuself == DM_FXFUN)
							|| (pMenu->menuself == DM_EMOP_ENILS) || (pMenu->menuself == DM_TEACH) 
							|| (pMenu->menuself == DM_TEMPERA) || (pMenu->menuself == DM_SIL_TEST)
							|| (pMenu->menuself == DM_RESET_UCMP) || (pMenu->menuself == DM_BRAKE_TEST)
							|| (pMenu->menuself == DM_TEST_UCMP))
							{
								para_dword_pass = (para_dword_pass + 1ul) % 2;
							}						
						else
							{
								para_dword_pass = (para_dword_pass + 1ul) % 10000ul;
							}
						break;
				}
				break;
			case LM_KEY_MENU:
				para_dword_pass = 0;
				break;
			case SM_KEY_ENT:
				edit_pos = (edit_pos + 1) % 4;
				last_value = ~para_dword_pass;
				break;
			case LM_KEY_ENT:			//参数编辑确认
				flash_flag = 0;
				if (pMenu->menuself == DM_TARGET)
					{
						current_value = para_dword_pass - 1;
						if ((current_value >= p.bot_floor) && (current_value <= p.top_floor) && (calls_enabled & CARCALL))
							{
								calltab [current_value].calltype |= CARCALL;
								calltab [current_value].cc_door |= p.doorpos [current_value];
							}
						else
							{
								strcpy((char*)buff, "----");
								flash_cnt = 0xff;
								menu_sel = 0xff;
								break;
							}
					}
				else if (pMenu->menuself == DM_PCUNUM || pMenu->menuself == DM_LIOPNUM)
					{
						if ((pMenu->menuself == DM_PCUNUM && para_dword_pass > p.tse_number)
								|| (pMenu->menuself == DM_LIOPNUM && para_dword_pass > p.ese_number)
								|| (para_dword_pass == 0))
							{
								strcpy((char*)buff, "----");
								flash_cnt = 0xff;
								menu_sel = 0xff;
								break;
							}
						else
							{
								*((BYTE *)pMenu->menuptr) = (BYTE)para_dword_pass;
							}
					}
				else if (pMenu->menuself == DM_IDSET)
					{
						if (para_dword_pass == 1)
							{
								buffer[0] = SET_NODE_ID2;
								send_message(LSS, LSS_REQ_ID, 1, buffer, CAN_B);
							}
						else if(para_dword_pass == 2)
							{
								buffer[0] = DISP_NODE_ID;
								send_message(LSS, LSS_REQ_ID, 1, buffer, CAN_B);
							}
						else if(para_dword_pass == 3)
							{								
								buffer[0] = ABORT_NODE_ID;
								send_message(LSS, LSS_REQ_ID, 1, buffer, CAN_B);
							}						
						else
							{
								strcpy((char*)buff, "----");
								flash_cnt = 0xff;
								menu_sel = 0xff;
								break;
							}
					}
				else if (pMenu->menuself == DM_DIR)
					{
						if (para_dword_pass)
							{
								niv.F0_05 = 1;
								GetAddrByReadPara(&niv.F0_05, &address);
								send_nice5000_parameter(address, niv.F0_05);
							}
						else
							{
								niv.F0_05 = 0;
								GetAddrByReadPara(&niv.F0_05, &address);
								send_nice5000_parameter(address, niv.F0_05);
							}
					}
				else if (pMenu->menuself == DM_MAX_SPEED)
					{
						niv.F0_03 = (WORD)para_dword_pass;
						GetAddrByReadPara(&niv.F0_03, &address);
						send_nice5000_parameter(address, niv.F0_03);
					}				
				else if (pMenu->menuself == DM_TEACH)
					{	
						teach_in = para_dword_pass;
						teach_flag = teach_in;
						memset(led_buff, 0x00, LED_BUFF_LEN);
					}
				else if (pMenu->menuself == DM_BREAK)
					{
						bBreak_check_enable = para_dword_pass;
						brake_check = bBreak_check_enable;
						brake_state = bBreak_check_enable;
						p.hse_inpar [17][IO_ENABLE] = bBreak_check_enable;
					}
				else if ((pMenu->menuself == DM_FXFUN) || (pMenu->menuself == DM_TEMPERA))
					{
						if(pMenu->menuself == DM_FXFUN)
							{
								bFx_check_enable = para_dword_pass;
								p.fx_active = bFx_check_enable;
							}
						else if(pMenu->menuself == DM_TEMPERA)
							{
								bTemp_check_enable = para_dword_pass;
								if(bTemp_check_enable)
									p.lift_func_sel1 |= LIFTFUN1_CHECK_ROOM_TEMP;
							}
						savepara = 1;
					}
				else if (pMenu->menuself == DM_SIL_TEST)
					{
						bTest_SIL = para_dword_pass;
						p.hse_inpar [INPUT_ILO][IO_BASIC_FUNC] = POS_IN;
						p.hse_inpar [INPUT_ILO][IO_SUB_FUNC] = POS_ILU;
						p.hse_inpar [INPUT_ILO][IO_STATE] = POLARITY_BIT;
						p.hse_inpar [INPUT_ILO][IO_ENABLE] = bTest_SIL;
						switchexist |= p.hse_inpar [INPUT_ILO][IO_SUB_FUNC];
						p.hse_inpar [INPUT_ILU][IO_BASIC_FUNC] = POS_IN;
						p.hse_inpar [INPUT_ILU][IO_SUB_FUNC] = POS_ILD;
						p.hse_inpar [INPUT_ILU][IO_STATE] = POLARITY_BIT;
						p.hse_inpar [INPUT_ILU][IO_ENABLE] = bTest_SIL;
						switchexist |= p.hse_inpar [INPUT_ILU][IO_SUB_FUNC];
					}
				else if(pMenu->menuself == DM_RESET_UCMP)
					{
						if(!para_dword_pass)
							ucmp_state = para_dword_pass;		//只允许清0，不允许设置1
					}				
				else if(pMenu->menuself == DM_BRAKE_TEST)
					{
						if((drive_state == D_STOPPED) && (!driveerror) && (para_dword_pass))
							{
								bTest_brake = para_dword_pass;
								brake_check_forced = bTest_brake;
							}
						else						
							{
								bTest_brake = 0;
								brake_check_forced = 0;
							}
					}
				else
					{
						switch(pMenu->size)
						{
							case 1:
								*((BYTE *)pMenu->menuptr) = (BYTE)para_dword_pass;
								break;
							case 2:
								*((WORD *)pMenu->menuptr) = (WORD)para_dword_pass;
								break;
							case 4:
								*((DWORD *)pMenu->menuptr) = para_dword_pass;
								break;
						}
						switch(pMenu->menuself)
							{
								case DM_F2_11:
								case DM_F2_12:
								case DM_F2_13:
								case DM_F2_14:
								case DM_F2_15:
									GetAddrByReadPara(pMenu->menuptr, &address);
									send_nice5000_parameter(address, para_dword_pass);
									break;
							}
					}
				menu_sel = 1;
				cont_first = 1;
				break;
		}
	}
	else if (menu_sel == 0xff)				// 闪烁显示指定内容后返回菜单
	{
		if (flash_cnt == 0xff)				// 初次显示
		{
			memset(led_buff, 0x00, LED_BUFF_LEN);
			led_buff[0] = buff[0];
			led_buff[2] = buff[1];
			led_buff[4] = buff[2];
			led_buff[6] = buff[3];
			flash_timer = timer + 5;
			++ flash_cnt;
		}
		if (flash_timer < timer)
		{
			++ flash_cnt;
			if (flash_cnt >= 6)
			{
				if (buff[0] == '7' &&
					buff[1] == '7' &&
					buff[2] == '7' &&
					buff[3] == '7')
				{
					menu_num = DM_STATE;
					menu_first = 1;
					menu_sel = 1;
					flash_flag = 0;
					flash_cnt = 0;	
					savepara = 1;
				}
				else
				{
					menu_sel = 0;
					cont_first = 1;
				}
			}
			else
			{
				if (flash_cnt % 2)
					memset(led_buff, 0x00, LED_BUFF_LEN);
				else
				{
					led_buff[0] = buff[0];
					led_buff[2] = buff[1];
					led_buff[4] = buff[2];
					led_buff[6] = buff[3];
				}
				flash_timer = timer + 5;
			}
		}
	}
	else if (menu_sel == 0xfe)				// 等待数据1
	{
		if (flash_cnt == 0xff)				// 初次显示
		{
			memset(led_buff, 0x00, LED_BUFF_LEN);
			flash_cnt = 0;
			led_buff[0+1] = '.';
			flash_timer = timer + 5;
			dm_get_answer = 0;
			if (pMenu->menuself == DM_VERCDU)
				hht_menu = M_FVE_INFO;
			else if (pMenu->menuself == DM_VERPCU)
				hht_menu = M_TSE_INFO;
			else if (pMenu->menuself == DM_VERLIOP)
				hht_menu = M_ESE_INFO;
		}

		if (((dm_get_answer)&&(flash_cnt==3)) || (boardkey == SM_KEY_MENU))
		{
			if ((dm_get_answer)&&(flash_cnt==3))
			{
				menu_sel = 1;
				cont_first = 1;
			}
			else if (boardkey == SM_KEY_MENU)
			{
				menu_sel = 0;
				cont_first = 1;
			}
			hht_menu = M_STATUS1;
		}
		else if (flash_timer < timer)
		{
			led_buff[flash_cnt*2 + 1] = 0x00;
			flash_cnt = (flash_cnt + 1) % 4;
			led_buff[flash_cnt*2 + 1] = '.';
			flash_timer = timer + 5;
		}
	}
	if(teach_flag)
		{
			if((!teach_in_text) || (teach_in_text == T_T_TOP))
				{//不存在故障，显示当前楼层 							
					sprintf((char*)buff, "%2d", level + 1);
					led_buff[2] = ' ';
					led_buff[4] = buff[0];
					led_buff[6] = buff[1];
				}
			else
				{
					sprintf((char*)buff, "%2d", teach_in_text);
					led_buff[2] = 'T';
					led_buff[4] = buff[0];
					led_buff[6] = buff[1];
				}
		}
	
	if(boardkey)
		{
			menu_keytimer = 600;
			keytimer_set = 1;
		}
	boardkey = 0;
}

