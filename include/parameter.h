
#ifndef _PARAMETER_H_
#define _PARAMETER_H_

#include "config.h"
#include "canopen.h"
#include "rtc.h"

//模拟运行使用常量
#define	SIMULATION_AUTO		2
#define	SIMULATION_DRIVE		1

//**************************************************************************************************
//	lift specific definitions
//**************************************************************************************************
#define MAX_GR				8
#define MAX_FLOOR			55
#define MAX_DOOR			2///3///3///2
#define MAX_TSE				3
#define MAX_TSE_EXT		3
#define MAX_ESE				110
#define MAX_EXE				2//8
#define MAX_ACCESS			4
#define MAX_VIRTUAL_GROUP	16

#define MAX_HSE_IN			21
#define MAX_HSE_OUT			4
#define MAX_FVE_IN			18
#define MAX_FVE_OUT			10
#define MAX_ASE_IN			8
#define MAX_ASE_OUT			8
#define MAX_TSE_IN			70
#define MAX_TSE_OUT			70
#define MAX_EXE_IN			8
#define MAX_EXE_OUT			8
#define MAX_ESE_IN			8
#define MAX_ESE_OUT			8
#define MAX_ACCESS_IN			8
#define MAX_ACCESS_OUT			4
#define MAX_ESE_PER_F_D		3					//每层每个门最多对应的ese个数
#define MAX_ESE_DW			((MAX_ESE+31)/32)
#define	MAX_IO_TYPE		7

#define	MAX_HSE_EX_IN		3
#define	MAX_HSE_EX_OUT		2
#define	MAX_FVE_EX_IN			1

//**************************************************************************************************
// Bitwise order of nodes
//**************************************************************************************************
#define B_FVE				0x01				// LSB for FVE
#define B_DRV				0x02				// Bit for drive
#define B_PSE				0x04				// Bit for PSE
#define B_DSE				0x08				// Bit for PSE
#define B_ESE				0x40				// Bit for ESE
#define B_CAN				0x80				// Bit for no CAN

//**************************************************************************************************
// Variables for door table
//**************************************************************************************************
#define DOOR_NORMAL			0					// door table for normal trip, car calls
#define DOOR_TIMER_1		1					// door table for timer trip 1,	car calls
#define DOOR_TIMER_2		2					// door table for timer trip 2,	car calls
#define DOOR_SPECIAL		3					// door table for special trip
#define DOOR_ADV			4					// door table for advanced trip
#define DOOR_EMERG			5					// door table for emergency trip
#define DOOR_FIREMAN		6					// door table for fireman trip
#define DOOR_ALARM			7					// door table for fire alarm
#define DOOR_N_HALL			8					// door table for normal trip, hall calls
#define DOOR_T1_HALL		9					// door table for timer trip 1, hall calls
#define DOOR_T2_HALL		10					// door table for timer trip 2, hall calls
#define DOOR_AUTOCALL		11					// door table for auto call
#define MAX_DOORTABLE		12					// maximum number of door tables

//**************************************************************************************************
// Call priority
//**************************************************************************************************
#define C_STANDARD			0					// Standard priority
#define C_SPECIAL			1					// Special call priority
#define C_ADVANCED			2					// Advanced call priority
#define C_EMERGENCY			3					// Emergency priority
#define C_EVACUATION		4					// Evacuation priority
#define C_FIREALARM			5					// Fire alarm
#define C_FIREMAN			6					// Fireman priority
#define MAX_PRIORITY		7					// Maximum number of priorities

//**************************************************************************************************
// Definitions for input and output variables
//**************************************************************************************************
#define IO_BASIC_FUNC		0					// basic function of input / output
#define IO_SUB_FUNC			1					// sub function of input / output
#define IO_LIFT				2					// lift number of input / output
#define IO_FLOOR			3					// floor number of input / output
#define IO_DOOR				4					// door number of input / output
#define IO_STATE			5					// state of input / output
#define IO_ENABLE				6					// acknowledgement marker for calls
#define IO_ACK				7					// acknowledgement marker for 

#define	ID_CARD_ACTIVE		0xF0
#define	ID_CARD_TYPE			0xF1
#define	ID_FLOOR_EN1			0xF2
#define	ID_FLOOR_EN2			0xF3
#define	ID_LIFT_ATTEN			0xF4
#define	ID_LIFT_OFF				0xF5
#define	ID_CARD_NORMAL		0xF6
#define	ID_LIFT_MANAGER		0xF7
#define	ID_CARD_TIME1			0xF8
#define	ID_CARD_TIME2			0xF9

//**************************************************************************************************
// Definition, if outputs are for CAN-A, CAN-B or HSE
//**************************************************************************************************
#define O_CANA				0x01				// output is for CAN A (control bus)
#define O_CANB				0x02				// output is for CAN B (shaft bus)
#define O_CANC				0x04
#define O_HSE				0x08				// output is for HSE
#define O_ALL				0x0f				// output is for HSE, CAN A and CAN B
#define O_CANA1				0x11				// output is for CAN A + safety circuit inputs

//**************************************************************************************************
// Language
//**************************************************************************************************
#define NO_OF_LANGUAGE		2					// number of languages

#define ENGLISH				0
#define CHINESE				1

//**************************************************************************************************
// Door state in parking mode (parameter parkdoormode)
//**************************************************************************************************
#define P_DOOR_OPEN			0					// parking with open doors
#define P_DOOR_CLOSED		1					// parking with closed doors

//**************************************************************************************************
// Landing call mode in actual floor (parameter lcmode_actfloor)
//**************************************************************************************************
#define LCM_REOPEN			0					// handle landing calls when door is closing
#define LCM_NO_REOPEN		1					// handle landing calls when door is open
#define LCM_NO_CALL			2					// never handle landing calls in actual floor

//**************************************************************************************************
// Type of positioning system (parameter postype)
//**************************************************************************************************
#define P_ENC_HSE			0					// position encoder connected to HSE
#define P_MOTOR_HSE			1					// motor encoder connected to HSE

//**************************************************************************************************
// Control type
//**************************************************************************************************
#define COLLECTING			0					// collecting control
#define SINGLE_CALL			1					// single call control

//**************************************************************************************************
// Door close and door open mode in fireman mode
//**************************************************************************************************
#define FIREDOOR_AUTO		0					// automatic door movement
#define FIREDOOR_PULSE		1					// door movem. with door push pulse
#define FIREDOOR_STOP		2					// door movem. with door push,stop if released
#define FIREDOOR_REVERSE	3					// door mov.with door push,reverse if released

//**************************************************************************************************
// Car car cancelling (undo)
//**************************************************************************************************
#define SINGLE				1					// car call cancel by repeated single pressing
#define DOUBLE				2					// car call cancel by quick double pressing

//**************************************************************************************************
// Load limit (analog value)
//**************************************************************************************************
#define C_LOADTYPE_CNT		4
#define ZEROLOADLIMIT		0					// variable contains zero load signalling limit
#define HALFLOADLIMIT		1					// variable contains half load signalling limit
#define FULLLOADLIMIT		2					// variable contains full load signalling limit
#define OVERLOADLIMIT		3					// variable contains overload signalling limit

//Config double door
#define	CONFIG_SHAFT_DOOR	1
#define	CONFIG_CAR_DOOR		2
#define	CONFIG_OPEN_DOOR		1
#define	CONFIG_CLOSE_DOOR	2

#define	CARD_TYPE_NORMAL		0
#define	CARD_TYPE_MANAGER	1
#define	CARD_TYPE_FLOOR		2
#define	CARD_TYPE_VIPCALL		3
#define	CARD_TYPE_USER			4
#define	CARD_TYPE_TIMER		5
#define	CARD_TYPE_ATTEND		6
#define	CARD_TYPE_LIFTOFF		7

//变量 bFunc_flag 的位定义
#define	FUNC_MSI_CHANGE				0x01		//门区信号变化
#define	FUNC_DOORSTOP_TIMEOUT	0x02		//延时关门按钮时间到
#define	FUNC_ARRIVEGONG_TIMEIN	0x04		//到站钟夜间时间到
#define	FUNC_FIRE_DOORCLOSE			0x10		//蜂鸣器鸣叫的时间到
#define	FUNC_OVERSPEEP_ACTIVE		0x40	//端站超速标志
#define	FUNC_CANA_INIT					0x80	//CANA 初始化标志
#define	FUNC_CANA_ERROR				0x100	//CANA 错误标志
#define	FUNC_SDD1_SDU1_ON				0x1000	///2个一级强减信号生效标志
#define	FUNC_PITBOX_RESET_HOLD		0x2000	

//变量 p.fan_mode 的位定义
#define	FAN_AUTOMATIC		0x01		//自动模式
#define	FAN_FIRE					0x02		//消防状态下风扇不工作

//变量 p.ups_direction_mode 的位定义
#define	UPS_DIR_SEL		0x01		//选择变频器的方向
#define	UPS_NO_TIME	0x02		//救援时不检测运行时间
#define	UPS_ALARM		0x04		//救援时轿厢蜂鸣器鸣叫
#define	UPS_RETURN		0x08		//救援的退出选择(0: 开门停电退出，1: 关门停电退出)

//变量 p.cooperate_sel1 的位定义
#define	COOP1_ARRIVE_NIGHTGONG	0x01
#define	COOP1_DIVING_NOTIME			0x02
#define	COOP1_DIVING_GONG				0x04
#define	COOP1_INSP_RETURN				0x10
#define	COOP1_THROUGH_DOOR			0x20

//变量 p.cooperate_sel2 的位定义
#define	COOP2_CL_LIMIT	0x80


//变量 p.landingcall_push 的位定义
#define	PUSH_NOT_OPEN	0x01
#define	PUSH_ALARM			0x02
#define	PUSH_STOP_SHIP	0x04

//变量 p.forced_stop 的位定义
#define	FORCED_TIME	0x01
#define	FORCED_INPUT	0x02

//变量 p.remote_off_func 的位定义
#define	REMOTE_LIGHT		0x01
#define	REMOTE_CALL		0x02
#define	REMOTE_TIME		0x04

//变量 p.attend_carcall 的位定义
#define	ATTEND_HALLCALL_CARENABLE	0x01
#define	ATTEND_CANCEL_CALL					0x02
#define	ATTEND_DOOR_MODE					0x10
#define	ATTEND_SPEEKER							0x20

//变量 p.custumer1 的位定义
#define	CUSTUMER1_SHE_MODE				0x02

//变量 p.normal_cc_advanced 的位定义
#define	VIPRUN_CARCALL_ENABLE		0x01

//变量 p.firealarm_function 的位定义
#define	FIREALARM_FORCE_CLOSE			0x01
#define	FIREALARM_DOORPARK_MODE	0x02
#define	FIREALARM_DYNAMIC				0x04
#define	FIREALARM_ACTIVE_DETECT		0x08
#define	FIREALARM_RESET_MODE			0x10
#define	FIREALARM_CROSS_FIREFLOOR	0x20
#define	FIREALARM_DISP_LIGHT			0x40

//变量 p.fireman_function1 的位定义
#define	FIREMAN1_DOOR_MAINFLOOR		0x01
#define	FIREMAN1_PHASE1_DOORSPEED		0x02
#define	FIREMAN1_PHASE2_DOORSPEED		0x04
#define	FIREMAN1_REPEAT_FIRECALL			0x08
#define	FIREMAN1_RETURN_MAINFLOOR	0x10
#define	FIREMAN1_AUTO_FIREMODE			0x20

//变量 p.lift_func_sel1 的位定义
#define	LIFTFUN1_DOORSHORT_REVEL			0x01
#define	LIFTFUN1_PRESS_CLOSEBUTTOM			0x02
#define	LIFTFUN1_CHECK_ROOM_TEMP			0x08
#define	LIFTFUN1_INSP_LOCK							0x10
#define	LIFTFUN1_BYPASSLC_DOORZONE			0x20
#define	LIFTFUN1_CHECK_OVERSPEED			0x80

//变量 p.call_disable_enable 的位定义
#define	CALL_DISEN_DISCALL		0x01
#define	CALL_DISEN_ENCALL			0x02
#define	CALL_DISEN_AUTO_CC		0x04
#define	CALL_DISEN_AUTO_LC		0x08
#define	CALL_DISEN_PRIORITY		0x10

//变量 ls_state 的位定义
#define	LS_STATE_DOOR1		0x01
#define	LS_STATE_DOOR2		0x02
#define	LS_STATE_DOOR3		0x04

struct Parameter	
{
	BYTE ucmp_enable;
	BYTE ucmp_test_dir;			
	BYTE emp_power_door;		///0: 不开门  1: 开门1  2: 开门2   3: 开门1，门2
	BYTE custumer1;				//用户功能的选择模式			
	BYTE bot_floor;	// bottom floor (0 - 62)
	BYTE top_floor;	// top floor (1 - 63)

	BYTE back3;			//字对齐，增加 2byte, 一共 6byte
	BYTE back4;
	BYTE back5;
	BYTE arrivegong_stoptime;
	BYTE she_doornumber;
	BYTE exe_number;
	
	BYTE groupsize;								// number of lifts in group (1 - 8)
	BYTE groupnumber;							// number of this lift in group (0 - 7)
	WORD wait_for_contactor;					// contactor input delay time in ms (open/close)
	WORD wait_for_brake;						// brake input delay time in ms (open/close)
	WORD run_off_delay;						// delay time in ms from brake feedback to withdraw run sig
	WORD triptime;								// trip time according to EN-81 in S
	WORD parkingtime;							// parking trip time in S; 0 = no parking trip
	BYTE parkfloor;								// floor for parking trip (0 - top floor)
	BYTE max_door_close_group;					// max. waiting time for door in group
	BYTE fire_floor[MAX_FLOOR];					// order for floors in case of fire
	BYTE doornumber;								// number of car doors (1 - 3)
	BYTE lstime[MAX_DOOR];						// light screen reclose time is S 
	BYTE revtime [MAX_DOOR];					// reopen by reverse contact reclose time in S
	BYTE nudgingtime [MAX_DOOR];				// closing time if ligth screen is on in 2S
	BYTE rev_door [MAX_DOOR];					// door has revolving door (=1) or not (=0)
	BYTE dooropentime [MAX_DOOR];				// max. door open time in S
	BYTE doorclosetime [MAX_DOOR];				// max. door close time in S
	BYTE attend_normal_timer;						
	BYTE back15;						

	WORD doorswitchtime [MAX_DOOR];				// delay time by changing of door direction, ms
	BYTE doordelaytime [MAX_DOOR];				// door closing delay time
	BYTE doorrelaymode [MAX_DOOR];				// door relay state when door is opened/closed
												// Bit0 open relay/Bit1 close relay: 1=stay on
	WORD doorstaytime_cc;						// door open stay time if only car call
	WORD doorstaytime_lc;						// door open stay time if also hall call
	WORD doorstaytime_nc;						// door open stay time if no new call
	BYTE fve_inpar [MAX_FVE_IN][MAX_IO_TYPE];				// free programmable inputs of FVE
	BYTE fve_outpar [MAX_FVE_OUT][MAX_IO_TYPE];			// free programmable outputs of FVE
	BYTE hse_inpar [MAX_HSE_IN + 1][MAX_IO_TYPE];			// free programmable inputs of HSE
	BYTE hse_outpar [MAX_HSE_OUT][MAX_IO_TYPE];			// free programmable outputs of HSE
	BYTE tse_inpar [MAX_TSE][MAX_TSE_IN][MAX_IO_TYPE];	// free programmable inputs of TSE
	BYTE tse_outpar [MAX_TSE][MAX_TSE_OUT][MAX_IO_TYPE];	// free programmable outputs of TSE
	BYTE ese_inpar [MAX_ESE][MAX_ESE_IN][MAX_IO_TYPE];	// free programmable inputs of ESE
	BYTE ese_outpar [MAX_ESE][MAX_ESE_OUT][MAX_IO_TYPE];	// free programmable outputs of ESE
	BYTE tse_number;
	BYTE tse_extnums;
	BYTE ese_number;
	BYTE doorpos [MAX_FLOOR];					// shaft door positions

	WORD sign [MAX_FLOOR];						// floor names (for level display)
	BYTE up_peak_start_m1;							// start time for up peak traffic
	BYTE up_peak_stop_m1;							// stop  time for up peak traffic
	BYTE dn_peak_start_m1;							// start time for down peak traffic
	BYTE dn_peak_stop_m1;							// stop  time for down peak traffic
	BYTE up_peak_start_h2;							// start time for up peak traffic
	BYTE up_peak_start_m2;							// start time for up peak traffic
	BYTE up_peak_stop_h2;							// stop  time for up peak traffic
	BYTE up_peak_stop_m2;							// stop  time for up peak traffic
	BYTE dn_peak_start_h2;							// start time for down peak traffic
	BYTE dn_peak_start_m2;							// start time for down peak traffic
	BYTE dn_peak_stop_h2;							// stop  time for down peak traffic
	BYTE dn_peak_stop_m2;							// stop  time for down peak traffic
	BYTE up_peak_start_h3;							// start time for up peak traffic
	BYTE up_peak_start_m3;							// start time for up peak traffic
	BYTE up_peak_stop_h3;							// stop  time for up peak traffic
	BYTE up_peak_stop_m3;							// stop  time for up peak traffic
	BYTE dn_peak_start_h3;							// start time for down peak traffic
	BYTE dn_peak_start_m3;							// start time for down peak traffic
	BYTE dn_peak_stop_h3;							// stop  time for down peak traffic
	BYTE dn_peak_stop_m3;							// stop  time for down peak traffic
	DWORD level [MAX_FLOOR - 6];					// floor positions (absolute)
	WORD ucmp_door_height;						//轿厢门高度
	WORD ucmp_car_apron;						//轿厢护脚板长度
	WORD arrival_speed;							// speed limit for advanced door opening
	BYTE adv_door_op;							// advanced door opening: yes=1, no=0
	BYTE relevel;								// re-levelling enabled: yes=1, no=0
	BYTE relevel_trip;							// re-levelling if new call : yes=1, no=0
	BYTE canbus_check;	
	BYTE lcmode_actfloor;						// mode if landing call pressed at actual floor

	BYTE back32;
 	WORD carlighttimer;							// automatic switch off car light (0=disabled)
	WORD errortime;								// error delay (clear calls...)
	BYTE forced_stop;							// forced stop: yes / no
	BYTE forced_stop_floor;						// floor for forced stop
	BYTE forced_stop_dir;						// direction for forced stop
	BYTE forced_stop_door;						// door for foced stop
	BYTE controltype;							// call collecting / single call lift
	BYTE remote_off_floor;						// floor for remote switch off
	BYTE remote_off_door;						// doors for remote switch off
	BYTE remote_off_func;						// car light state when remote switch off
	BYTE fan_mode;								// 风扇工作模式设定

	BYTE remote_starttime;
	WORD fan_time;								// cabin fan: time delay
	BYTE autocall_dir;							// stop direction for auto call function
	BYTE dir_pending;							// cancel hall calls direction pending
	BYTE remote_stoptime;					// clear calls when remote switch off
	BYTE cc_no_ls;								// max.number of car calls without light screen
	BYTE clear_other_lc_if_cc;					// clear landing call in other dir. if car call
	BYTE main_floor;							// main floor
	BYTE open_close_door;
	
	BYTE back9;
	WORD doorrelaytime_op [MAX_DOOR];			// max. door open relay on time
	WORD max_ls_on [MAX_DOOR];					// max. time light screen interrupted to error
	WORD brake_on_delay;						// delay time to open the brake
	WORD brake_off_delay;						// delay time to release the brake
	WORD ready_off_delay;						// delay time to release running contactor
	WORD max_start;								// max. waiting time to drive speed > 0
	WORD max_ready_on;							// max. waiting time for ready signal
	WORD max_brake_on;							// max. waiting time for brake signal
	WORD max_stop;								// max. waiting time to drive speed = 0
	WORD max_brake_off;							// max. waiting time for brake signal off
	WORD max_ready_off;							// max. waiting time for ready signal off
	WORD starttime;								// start delay (debouncing safety circuit door)
	BYTE doortable [MAX_DOORTABLE][MAX_FLOOR];	// enabled door in different states
	BYTE last_trip_special;						// finish last trip when special call ??
	BYTE last_trip_advanced;					// finish last trip when advanced call ??
	BYTE last_trip_emergency;					// finish last trip when emergency call ??

	BYTE back10;
	WORD time_special;							// time lift stays in special mode
	WORD time_advanced;							// time lift stays in advanced mode
	WORD time_emergency;						// time lift stays in emergency mode
	BYTE hallcall_special;						// disable hall calls in special mode
	BYTE hallcall_advanced;						// disable hall calls in advanced mode
	BYTE hallcall_emergency;					// disable hall calls in emergency mode
	BYTE carcall_special;						// clear car calls in special mode
	BYTE carcall_advanced;						// clear car calls in advanced mode
	BYTE carcall_emergency;						// clear car calls in emergency mode
	BYTE carcall_special_lc;					// clear car calls by special landing
	BYTE carcall_advanced_lc;					// clear car calls advanced landing
	BYTE carcall_emergency_lc;					// clear car calls by emergency landing
	BYTE max_cc [MAX_PRIORITY];					// max.number of car calls at the same time
	BYTE forced_close_emerg;					// forced door closing in emergency mode
	BYTE fire_doorclosemode;					// door closing mode in fireman mode 
	BYTE fire_dooropenmode;						// door open mode in fireman mode
	BYTE back20;

	BYTE fireman_function1;
			//	bit0: 1-->消防基站的门属于消防门    									0-->消防基站的门自动打开 
			//bit1: 1-->消防第 I 阶段，低速关门 										0-->消防第 I 阶段，正常关门
			//bit2: 1-->消防第 II 阶段，低速关门 									0-->消防第 II 阶段，正常关门
			//bit3: 1-->消防员信号消除后，电梯自动重返消防基站   			0-->不自动，需要人为呼梯
			//bit4: 1-->达到消防基站才可以退出消防状态(bit3=0)   			0-->不需要到达消防基站就可以退出
			//bit5: 1-->fire call 信号自动进入消防员(也可以自动退出)		0-->需要消防员开关
	BYTE timer_clearcall;						// clear calls if timer trip
	BYTE timertrip_start;						// time to start timer trip
	BYTE timertrip_stop;						// time to stop timer trip
	BYTE timer1_parkfloor;						// parking floor for timer 1 trip
	BYTE timer2_parkfloor;						// parking floor for timer 2 trip
	WORD max_speed;
	BYTE call_disable_enable; 			//higher priority of enable or disable calls
	BYTE back6;
	BYTE calltime_special;						// time calls enabled in special mode
	BYTE calltime_advanced;						// time calls enabled in advanced mode
	BYTE calltime_emergency;					// time calls enabled in emergency mode
	BYTE time_back_special;						// delay time back to normal from special mode
	BYTE time_back_advanced;					// delay time back to normal from advanced mode
	BYTE time_back_emergency;					// delay time back to normal from emergency mode
	BYTE call_active_special;					// calls enabled only if special key is on
	BYTE call_active_advanced;					// calls enabled only if advanced key is on
	BYTE call_active_emergency;					// calls enabled only if emergency key is on
	BYTE back18;				
	BYTE normal_cc_special;						// normal car calls enabled in special mode
	BYTE normal_cc_advanced;					// normal car calls enabled in advanced mode
	BYTE normal_cc_emergency;					// normal car calls enabled in emergency mode
	BYTE firealarm_function;						// light screen active in case of fire
	WORD insp_stop_dly;
	WORD back22;
	BYTE back19;
	BYTE parkdoor [MAX_FLOOR];					// park state of door in different floors
	BYTE parkdoormode [MAX_DOOR];				// bit0: general park state of every door
																			//bit7: =0 开门按钮打开对应的门    1: 只打开未关闭的门
	WORD doorrelaytime_cl [MAX_DOOR];			// max. door close relay on time
	BYTE rampmode [MAX_DOOR];					// mode for retiring ramp on / off
	BYTE ramptiming [MAX_DOOR];					// retiring ramp off before or after car door
	WORD doorstoptime;							// time for door stop push
	WORD dooropendelay [MAX_DOOR];				// door open delay after retiring ramp off
	WORD ucmp_car_distance;					//厅门地坎到轿厢门地坎的距离
	WORD rampdelay [MAX_DOOR];					// retiring ramp on delay after revolving door
	WORD doorstaytime_main_cc;					// door stay time if only car call (main floor)
	WORD doorstaytime_main_lc;					// door stay time if also hall call (main floor)
	WORD doorstaytime_main_nc;					// door stay time if no new call (main floor)
	BYTE attend_carcall;						// car attendance:auto-car call if hall call
	BYTE cc_cancel;								// cancel car call by pressing again
	BYTE clear_cc_last_floor;					// clear all car calls in last landing
	BYTE clear_cc_opp_dir;						// ignore car call opposite to actual direction
	BYTE max_cc_empty;							// max. number of car calls for empty car
	BYTE up_peak_start_h1;							// start time for up peak traffic
	BYTE up_peak_stop_h1;							// stop  time for up peak traffic
	BYTE dn_peak_start_h1;							// start time for down peak traffic
	BYTE dn_peak_stop_h1;							// stop  time for down peak traffic
	BYTE up_peak_parkfloor;						// park floor in up peak traffic
	BYTE dn_peak_parkfloor;						// park floor in down peak traffic

	BYTE cooperate_sel1;		//电梯功能选择 
	BYTE arrivegong_starttime;
	BYTE cooperate_sel2;		//电梯功能选择 
		//bit7: 1-->启用轿门关门到位的机械开关(只对门旁路装置有效)			0-->采用标准模式
	WORD evacuationtime;						// max. waiting time for evacuation enable	
	BYTE evaction_waittime;
	BYTE back30;									
	WORD evacuationdelay;						// min. waiting time in evacuation mode
	WORD upstrip_stop_delay;
	BYTE evac_autoreturn;						// automatic return from evacuation
	BYTE dooropenlimit [MAX_DOOR];				// door open limit in case of door open error
	BYTE doorcloselimit [MAX_DOOR];				// door close limit in case of door close error
	
	BYTE back1;
	WORD ucmp_test_speed;		

	WORD pos_dooropen; 							// position for advanced door opening
	BYTE setup_ready;							// setup (teach in) done: yes=1, no=0
	BYTE motor_tuned;
	
	BYTE simu_mult;
	BYTE forced_starttime;
	BYTE forced_stoptime;
	
	BYTE back31;		
	WORD brake_check_time;		//抱闸力自动检测的时间(千位百位: 小时     十位个位: 分钟)
	BYTE back21;		
	BYTE lcd_backlight;		//LCD外呼的背光控制	
	BYTE loadtype;
	BYTE arrowtype;
	BYTE back29;

	BYTE back12;			//语音报站的语言选择
	WORD adlimit[C_LOADTYPE_CNT];			// 模拟量称重记录的AD值
	BYTE back24;	

	BYTE brake_check_cycle;				//抱闸力自动检测的周期(以天为单位:1-31天)
	WORD ucmp_speed;		
	WORD back26;
	WORD back27;
	BYTE antifainttime;
	BYTE liop_cop_door;

	BYTE brake_check_auto;		//抱闸力自动检测使能标志
	BYTE back2;
	DWORD limittimes;
	struct Time t_limit;
	BYTE limitenable;
	BYTE landingcall_push;		//
	WORD F5_X1;
	WORD F5_X2;
	WORD F5_X3;
	WORD F5_X4;
	WORD F5_X5;
	WORD F5_X6;
	WORD F5_X7;
	WORD F5_X8;
	BYTE fx_active;							// 是否使能封星功能

	BYTE back8;		
	WORD fx_offtime;							// 断开运行接触器至闭合封星之间的延迟时间
	WORD fx_ontime;							// 断开封星至闭合运行接触器之间的延迟时间

	BYTE back23;			
	BYTE door_connect;				//门锁的连接方式(1: 新方式(层门在前，轿门在后); 0: 旧方式(轿门在前，层门在后))
	WORD brake_feedback_dly;
	WORD brake_off_sig_dly;
	WORD doorzoneswitchsize;
	WORD ucmp_hall_apron;			//厅门护脚板长度
	BYTE simulation;
	BYTE back13;
	WORD rsttrip_stop_dly;
	BYTE lift_func_sel1;			//电梯功能选择
	BYTE back17;
	BYTE evac_door;								//发电机撤离完成门保持状态(0: 关门  1: 开门)
	BYTE evac_fire_carlightmode;		//发电机撤离以及火警撤离结束轿厢灯模式: 0-->2种方式都开灯 1-->发电机撤离关灯 2-->火警撤离关灯 3-->2种方式都关灯
	BYTE ups_direction_mode;			//UPS 功能参数
	BYTE back28;	
	BYTE fve_ex_inpar [3][MAX_IO_TYPE];
	BYTE hse_ex_inpar [5][MAX_IO_TYPE];			//Mcu 扩展板的输入(一共3个，预留2个)
	BYTE hse_ex_outpar [4][MAX_IO_TYPE];			//Mcu 扩展板的输出(一共2个，预留2个)

	WORD carlight_fan_time_fire; 						// automatic switch off car light (0=disabled)

	BYTE reserve[98];

};

//增加扩展参数
struct exu_parameter{
	BYTE exe_inpar [MAX_EXE][MAX_EXE_IN][MAX_IO_TYPE];	// free programmable inputs of TSE
	BYTE exe_outpar [MAX_EXE][MAX_EXE_OUT][MAX_IO_TYPE];	// free programmable outputs of TSE

};

//**************************************************************************************************
//	Structure for all variables for initialization
//*************************************************************************************************
struct _initpara
{
	void * pointer;								// pointer to parameter value
	BYTE size;									// parameter size
	WORD index;
	BYTE subindex_start;
	WORD subindex_stop;
	BYTE bg;									// parameter asigned to nodes
};

//**************************************************************************************************
//	Structure for all variables for state search
//*************************************************************************************************
struct _statevar
{
	void * pointer;								// pointer to parameter value
	BYTE size;									// parameter size
	WORD index;
	BYTE subindex_start;
	BYTE subindex_stop;
	BYTE bg;									// parameter asigned to nodes
};

//**************************************************************************************************
//	State variables for special outputs
//*************************************************************************************************
#define RIGHT_ADMIN				0x00
#define RIGHT_HELP				0x01
#define RIGHT_INSTALL			0x02
#define RIGHT_MAINTAIN			0x03
#define RIGHT_USER				0xff
extern DWORD password_admin;
extern DWORD password_help;
extern DWORD password_install;
extern DWORD password_maintain;
extern BYTE password_right;

#define C_TEST_ONCE		0
#define C_TEST_PERDAY		1
#define C_TEST_PERWEEK	2

extern DWORD tripcount;							// 行程计数
extern DWORD op_time;							// 工作时间

extern BYTE testtrip;							// 测试运行
extern BYTE testtimes;
extern DWORD testtimer;
extern BYTE start_test_trip;				// start test trip (simulate call)
extern BYTE test_starttime;
extern BYTE test_endtime;
extern BYTE test_times;
extern BYTE test_interval;
//extern BYTE simulation;

extern BYTE iv_para_valid;
extern BYTE dummy;

extern BYTE language;							// 语言
extern DWORD para_checksum;
extern BYTE write_checksum;


//*************************************************************************
// CAN variables CAN A
//*************************************************************************
#define RXASW_SIZE				50
#define TXASW_SIZE				100
#define SXASW_SIZE				100

extern BYTE rca; 	     				// RX message counter
extern BYTE roa;		    			// pointer for RX FIFO read
extern BYTE ria;		    			// pointer for RX FIFO write
extern BYTE tca; 	     				// TX message counter
extern BYTE toa;		    			// pointer for TX FIFO read
extern BYTE tia;		    			// pointer for TX FIFO write
extern BYTE tra;						// TX ready
extern BYTE tea;						// TX enabled
extern BYTE sca;
extern BYTE sia;
extern BYTE soa;
extern BYTE sra;
extern BYTE rxa[RXASW_SIZE][10];		// FIFO RX message buffer
extern BYTE txa[TXASW_SIZE][10];		// FIFO RX message buffer
extern BYTE sxa[SXASW_SIZE][10];		// FIFO SX message buffer
extern BYTE txa_fve_error;
extern DWORD cana_busoff_checktime;

//*************************************************************************
// CAN variables CAN B
//*************************************************************************
#define RXBSW_SIZE				50
#define TXBSW_SIZE				50
#define SXBSW_SIZE				50

extern BYTE rcb; 	     				// RX message counter
extern BYTE rob;		    			// pointer for FIFO read
extern BYTE rib;		    			// pointer for FIFO write
extern BYTE tcb; 	     				// TX message counter
extern BYTE tob;		    			// pointer for TX FIFO read
extern BYTE tib;		    			// pointer for TX FIFO write
extern BYTE trb;						// TX ready
extern BYTE teb;						// TX enabled
extern BYTE scb;
extern BYTE sib;
extern BYTE sob;
extern BYTE srb;
extern BYTE rxb[RXBSW_SIZE][10];		// FIFO RX message buffer
extern BYTE txb[TXBSW_SIZE][10];		// FIFO TX message buffer
extern BYTE sxb[SXBSW_SIZE][10];		// FIFO TX message buffer
extern DWORD canb_busoff_checktime;

//*************************************************************************
// CAN variables CAN C
//*************************************************************************
#define RXCSW_SIZE				50
#define TXCSW_SIZE				50
#define SXCSW_SIZE				50

extern BYTE rcc; 	     						// RX message counter
extern BYTE roc;		    					// pointer for FIFO read
extern BYTE ric;		    					// pointer for FIFO write
extern BYTE tcc; 	     						// TX message counter
extern BYTE toc;		    					// pointer for TX FIFO read
extern BYTE tic;		    					// pointer for TX FIFO write
extern BYTE trc;								// TX ready
extern BYTE tec;								// TX enabled
extern BYTE canc_busoff_flag;						// CANC bussoff flag
extern BYTE rxc[RXCSW_SIZE][10];				// FIFO RX message buffer
extern BYTE txc[TXCSW_SIZE][10];				// FIFO TX message buffer
extern BYTE txc_error;							// transmit error counter CAN C
extern BYTE canctxerror;						// CAN bus transmit not possible CAN C


//*************************************************************************
//	Variablen for free programmable IO
//*************************************************************************
extern const DWORD HSE_MARIN[];

extern BYTE out;					// output state
extern DWORD in;					// Variable to read physical inputs
extern DWORD input[];
extern DWORD oldin;
extern DWORD in_polarity;			// inputs inverted
extern DWORD oldin_polarity;
extern BYTE out_polarity;			// inputs not inverted
extern BYTE virt_in [];				// virtual input (last changed)

//*************************************************************************
//	Error variables
//*************************************************************************
#define MAX_TRIP_ERROR		100		// Maximum trip error count
#define ERROR_WARNINGLIMIT	100		// Error warning limit
#define MAX_DOOR_ERROR		4		// Maximum door error count
#define	MAX_DROPERR_TRYCNT	4
#define	BRAKE_NO_GOOD	2
#define	BRAKE_GOOD	1
#define	TRAVEL_TEST_VALUE		6//6//7

extern BYTE triperrorcount;			// Trip error count
extern BYTE errorwarning;			// Error warning: many errors
extern DWORD driveerror;			// drive error
extern BYTE poserror;				// error positioning system
extern WORD general_error;			// general error
extern BYTE doorcloseerrorcount	[];
extern BYTE dooropenerrorcount  [];
extern BYTE dooropenerrorfloor  [];
extern BYTE nextfloortrycount;
extern BYTE save_hist;				// history still to save
extern BYTE save_all;
extern WORD * wptr_errcounter;

//*************************************************************************
//	SDO and LSS variables
//*************************************************************************
#define SDO_TIMER			5		// 5s time out for SDO segment transfer

extern BYTE lss_res;				// response to LSS message
extern WORD sdo_id;					// response to SDO message
extern WORD hse_sdo_id;				// response to SDO message from HSE (Sync.)
extern WORD sdo_index;				// save SDO index
extern BYTE sdo_subindex;			// save SDO sub-index
extern WORD sdo_pcb;				// save PCB for SDO
extern BYTE sdo_data [];			// data of SDO message
extern BYTE tse_sdo_data [];		// data of SDO message from TSE
extern BYTE ese_sdo_data [];		// data of SDO message from ESE
extern BYTE hse_sdo_data [];		// data of SDO message from HSE
extern DWORD sdo_timer;				// timer for SDO segment transfer
extern WORD sdo_size;				// size of SDO object
extern BYTE * sdo_address;			// address of SDO object
extern WORD sdo_pos;				// last transferred BYTE inside SDO object
extern BYTE sdo_toggle;				// toggle BYTE for SDO transfer
extern BYTE sdo_string;				// marker if data type is string
extern BYTE sdo_master;				// SDO transfer from normal software
extern BYTE tse_sdo_master;			// TSE SDO transfer from normal software
extern BYTE ese_sdo_master;			// ESE SDO transfer from normal software
extern BYTE sdo_interrupt;			// SDO transfer from interrupt
extern BYTE handle_ese;				// handle ESE boards

extern	BYTE start_limit;
extern	BYTE reset_mcu;

//*************************************************************************
//	Variables for LCD display
//*************************************************************************
#define MAX_LCD_ROW			4
#define MAX_LCD_COL			16
#define HANDHELDTIME			8
#define HANDHELDDLY			5
//*************************************************************************
//	CANopen specific variables
//*************************************************************************
extern DWORD const device_type;
extern WORD const virtual_device [7];

extern WORD handheld_key;			// key from handheld terminal
extern BYTE lcd_display [MAX_LCD_ROW][MAX_LCD_COL];
extern BYTE lcd_old [MAX_LCD_ROW][MAX_LCD_COL];
extern BYTE cursortype;				// cursor type
extern BYTE cursortype_old;			// old cursor type
extern BYTE cursorpos [];			// cursor position
extern BYTE cursorpos_old [];		// old cursor position
extern BYTE lcdram;					// content of user character generator RAM
extern BYTE handheld_active;		// handheld terminal active
extern BYTE handheldtimer;			// handheld supervision time
extern BYTE handheld_dly;
extern BYTE handheld_bak;

//*************************************************************************
//	Variables for special inputs
//*************************************************************************
extern BYTE dooropenpush;			// door open push
extern BYTE doorclosepush;			// door close push
extern BYTE doorstoppush;			// door stop push
extern BYTE doorstopstate;			// door stop state
extern BYTE special_key;			// key switch special trip
extern BYTE advanced_key;			// key switch advanced trip
extern BYTE emergency_key;			// key switch emergency trip
extern BYTE intermediate_door;		// contact from intermediate door
extern BYTE car_attendance;			// car attendance
extern BYTE door_attendance;		// door mode while car attendance
extern BYTE dir_attendance;			// direction changed while car attendance
extern BYTE call_bypass;			// bypass a floor while car attendance
extern BYTE hallcall_bypass;		// bypass a hall call while car attendance
extern BYTE fire_bypass;			//bypass firealaem
extern BYTE doorstoppush_sheopen_flag;		//开门标志
extern BYTE doorstoppush_sheclose_flag;		//关门门标志
extern BYTE she_photonsensor;
extern BYTE she_calldoor_double;
extern BYTE she_calldir;		//呼梯时贯通门的处理
extern BYTE she_doorstoppush;
extern BYTE she_doorstoppush_count;

//*************************************************************************
//	Variables for evacuation
//*************************************************************************
extern BYTE ups_power;				// UPS power
extern BYTE ups_brake;				// state changed to UPS brake opening
extern BYTE ups_brake_new;			// UPS evacuation start count
extern BYTE ups_startcount;			// UPS evacuation start count
extern BYTE evacuation;				// Evacuation on / off
extern BYTE evacuation_enable;		// Evacuation enable
extern BYTE evacuation_ready;		// Evacuation ready
extern BYTE evacuation_normal;		// Evacuation enable, but back to normal
extern DWORD evacuationtimer;		// Max. waiting time in evacuation mode
extern DWORD evacuationdelay;		// Min. time delay in evacuation mode
extern BYTE evacuationfloor;		// evacuation floor
extern BYTE evacuationdoor;			// doors to open in evacuation floor

extern BYTE emp_power;
extern BYTE emp_floor;
extern BYTE emp_doors;

//*************************************************************************
//	Variables for fireman mode
//*************************************************************************
extern BYTE firemode;				// fireman mode
extern BYTE firedoormode;			// door mode in fireman mode
extern BYTE firekey;				// fireman key in car
extern BYTE auto_fire;				// fireman key in car
extern BYTE firecall;				// fireman call (fire key in fireman floor)
extern BYTE firefloor;				// fireman floor
extern BYTE firedoors;				// doors in fireman floor
extern BYTE firecall_exist;			// lift has fireman call key
extern BYTE firealarm;				// fire alarm on
extern BYTE firealarmfloor;			// evacuation floor in case of fire alarm
extern BYTE firealarmmode;			// fire alarm mode
extern BYTE firealarmreturn;		// input to return from fire alarm mode
extern DWORD fireinput [];			// fire alarm inputs in floor

//*************************************************************************
//	Variables for calls
//*************************************************************************
extern struct CallTab calltab [];		// call table
extern struct GroupCall groupcall [];	// calls of other lifts

extern BYTE calldir;					// direction for call processing
extern BYTE startdir;
extern BYTE calldir_old;				// old call direction
extern BYTE callfloor;  				// floor for next call
extern BYTE calltype;				// call type next call
extern BYTE calldoors;     			// doors for next call
extern BYTE lastcalltype;			// last call type
extern BYTE lastcallfloor; 			// last call floor
extern BYTE lastcalldoors; 			// last call doors
extern BYTE next_callfloor;			// floor for possible next call (wait for group)
extern BYTE next_calltype;			// call type possible next call	(wait for group)
extern BYTE next_calldoors;			// doors for possible next call (wait for group)
extern WORD next_calltime;			// time to process next call (wait for
extern BYTE landingcalldir;			// variable to save direction of last land.call
extern BYTE calls_enabled;			// enabled calls
extern BYTE handle_calls;			// calls to handle
extern BYTE calls_off;				// calls disabled (error, inspection...)
extern BYTE carcallcounter;			// counter for car calls
extern BYTE callpriority;			// actual call priority
extern DWORD callprioritytimer;		// timer for call priority
extern DWORD callenabledtimer;		// timer for calls enabled in priority mode
extern DWORD groupcalldelay;		// delay timer for group control
extern BYTE calltype_old;			// last call type
extern BYTE callfloor_old;			// last call floor
extern BYTE calldoors_old;			// last call doors
extern BYTE landings_off;			// landing calls off (from menu)
extern BYTE landings_off_key;		// landing calls off (from key)
extern BYTE priorstate [3][MAX_FLOOR];
extern BYTE parkcall;				// floor for parking trip
extern BYTE doordelay;				// group delay if problems with door

//*************************************************************************
//	Variables for up and down peak traffic
//*************************************************************************
extern BYTE up_peak_time;			// time  for up   peak traffic
extern BYTE dn_peak_time;			// time  for down peak traffic
extern BYTE up_peak_input;			// input for up   peak traffic
extern BYTE dn_peak_input;			// input for down peak traffic

//*************************************************************************
//	Variables for TSE boards
//*************************************************************************
extern BYTE tse_init_error;			// TSE board not init
extern BYTE tse_start_init;			// TSE board init since last HSE start
extern BYTE tse_enabled;			// list of enabled TSE board
extern BYTE tse_hb_error;			// heartbeat error TSE
extern BYTE tse_hb_error_old;		// heartbeat error TSE
extern BYTE tse_hb_state;			// old heartbeat state TSE (for display)
extern BYTE tse_state_request;		// state request of TSE board necessary
extern BYTE tse_heartbeat;			// heartbeat message TSE
extern WORD tse_sdo_id;				// id of SDO producer

//*************************************************************************
//	Variables for EXE boards
//*************************************************************************
extern BYTE exe_init_error;			// TSE board not init
extern BYTE exe_start_init;		// TSE board init since last HSE start
extern BYTE exe_enabled;			// list of enabled TSE board
extern BYTE exe_hb_error;			// heartbeat error TSE
extern BYTE exe_hb_error_old;		// heartbeat error TSE
extern BYTE exe_hb_state;		// old heartbeat state TSE (for display)
extern BYTE exe_state_request;	// state request of TSE board necessary
extern BYTE exe_heartbeat;			// heartbeat message TSE
extern WORD exe_sdo_id;				// id of SDO producer
extern BYTE exe_sdo_data [8];			// data of SDO message from HSE
extern BYTE exe_sdo_master;		// ESE SDO transfer from normal software
extern BYTE txa_exe_error;					// transmit error counter for TSE
extern DWORD exetimer;						// timer for error warning if TSE fault


//*************************************************************************
//	Variables for ESE boards
//*************************************************************************
extern DWORD ese_state_request[];	// ESE state request necessary
extern DWORD ese_init_error [];		// ESE board not init
extern DWORD ese_start_init [];		// ESE board init since last HSE start
extern DWORD ese_enabled [];		// list of enabled ESE board
extern DWORD ese_hb_error [];		// heartbeat error ESE
extern DWORD ese_hb_error_old [];	// heartbeat error ESE
extern DWORD ese_hb_state [];		// old heartbeat state ESE (for display)
extern DWORD ese_heartbeat [];		// heartbeat message ESE
extern WORD ese_sdo_id;				// id of SDO producer

//*************************************************************************
//	Variables for drive
//*************************************************************************
extern BYTE brake_check;			// Brake check enabled
extern BYTE brake_state;			// Brake state
extern BYTE drive_fault;			// Drive fault input
extern BYTE contactor_check;		// contactor check enabled
extern BYTE contactor_state;		// Contactor state

//*************************************************************************
//	Variables for door table
//*************************************************************************
extern BYTE adt;					// actual door table
extern BYTE adt_hall;				// actual door table for hall calls

extern BYTE save_double_door;

//*************************************************************************
//	Statistic variables
//*************************************************************************
extern BYTE save_op_time;			// write operation time to EEPROM
extern WORD floorcount [MAX_FLOOR];	// floor counter (trips to this floor)
extern BYTE floortried [MAX_FLOOR];

//*************************************************************************
// State variables
//*************************************************************************
#define ALL_DOORS_CLOSED	0x07	// all doors closed in door state
#define EXISTING_DOORS		0x07	// filter for door bits door 1 - 3
#define TXERRORLIMIT		100//20//5		// set tx error after 5 heartbeat messages
#define NOT_AVAILABLE		0x80	// state not available  更改为2S
#define MAX_DIVING_FAIL		4
#define	INIT_POSITION			100000

extern BYTE hse_state;				// HSE state
extern BYTE hse_text;				// Text for HSE state
extern BYTE drive_state;			// Drive state
extern BYTE rtc_lowpower;
extern BYTE level;					// Actual floor
extern BYTE start_floor;			// Start floor
extern BYTE target_floor;			// Actual target floor
extern BYTE next_floor;				// Next possible target floor
extern BYTE max_target;				// Maximum target floor
extern BYTE direction;				// Actual drive direction
extern BYTE gong_direction;
extern BYTE insp;					// inspection switches
extern BYTE insp_dir;						// inspection switches
extern BYTE em_op;					// emergency recall operation switches
extern BYTE access_op;					// Access operation switches
extern BYTE access_floor;						// emergency recall operation switches
extern BYTE safety_circuit;			// safety circuit state
extern BYTE safety_circuit_1;		// marker for safety circuit 1 state
extern BYTE limit_switch_error;		// marker that final limit switch was off
extern WORD door_zone;				// door zone state
extern BYTE door_zone_old;				// door zone state
extern BYTE force_switch;
extern WORD door_state [];			// Door state
extern BYTE door_io [];				// Door input state
extern BYTE ls_state;				// Light screen state
extern BYTE doorstate;				// internal door state (closed/not closed)
extern BYTE gongoutputenable;			// gong output enable
extern BYTE doornew;				// doors still to open in arrival state
extern BYTE adv_door_op_dly;
extern BYTE load;					// load measurement state
extern BYTE overload;				// car is overloaded
extern BYTE fullload;				// car is full
extern BYTE zeroload;				// car is empty
extern BYTE temperature;			// temperature state of motor and machine room
extern BYTE mrt_state;				// machine room temperature state
extern BYTE carlight;				// car light state
extern BYTE term_active;			// terminal mode (lift controlled from pushes)
extern BYTE doortest_active;		// doortest mode (door controlled from pushes)
extern BYTE doortest_command;		// doortest command
extern BYTE key_doorcommand;		// doortest command from keys
extern BYTE teach_in;				// teach in mode
extern BYTE teach_in_text;			// teach in text
extern DWORD canerrorstate;			// CAN bus error CAN A (bus off, overrun...)
extern BYTE canatxerror;			// CAN bus transmit not possible CAN A
extern BYTE canbtxerror;			// CAN bus transmit not possible CAN B
extern BYTE txb_ese_error;				// transmit error counter CAN B
extern BYTE txa_tse_error;			// transmit error counter for TSE
extern BYTE switchexist;			// shows if door zone switch exists or not
extern BYTE doortimer [];			//
extern BYTE fan_state;				// state of cabin fan (on/off)
extern BYTE numberkey;				// direct access with xxx* enabled (= 0)
extern BYTE savepara;				// command to save all parameter
extern BYTE relevelnumber;			// number of not successful re-levelling
extern BYTE relevelflag;
extern BYTE dl_shorted;
extern BYTE insp_return;
extern BYTE relev_active;
extern BYTE relev_new;
extern BYTE diving_fail_cnt;			// diving failed counts
extern BYTE iic_busy;
extern BYTE ucmp_state;
extern BYTE cte_mode_send;
extern BYTE check_dl_short;
extern BYTE door_off;
extern BYTE sc_limit_fg;
extern BYTE dl_short_test_finish;

//*************************************************************************
//	State variables for special outputs
//*************************************************************************
extern BYTE fault_signal;
extern BYTE out_of_order;
extern BYTE out_of_order_input;
extern BYTE out_of_order_menu;
extern BYTE direction_ind;
extern BYTE hall_lantern;
extern BYTE occupied;
extern BYTE carlight_switched_off;
extern BYTE connected_io;
extern BYTE overload_ind;
extern BYTE fullload_ind;
extern BYTE dooropenpush_ind;
extern BYTE doorclosepush_ind;
extern BYTE doorstop_ind;
extern BYTE special_ind;
extern BYTE advanced_ind;
extern BYTE emergency_ind;
extern BYTE switched_off_ind;
extern BYTE remote_off_ind;
extern BYTE firecall_ind;
extern BYTE firefloor_ind;
extern BYTE firekey_ind;
extern BYTE specialmode;
extern BYTE buzzer;
extern BYTE door_closing;
extern BYTE firealarmfloor_ind;
extern BYTE dooroutputstate [];
extern BYTE hallcall_bypass_ind;

//*************************************************************************
//	Variables for special trips and special functions
//*************************************************************************
extern BYTE remote_switched_off;	// remote switch off state
extern BYTE remote_off;				// remote switch off input
extern BYTE remote_call;			// remote switch off floor
extern BYTE remote_door;			// remote switch off doors
extern BYTE remote_clearcalls;		// clear calls if remote switch off
extern BYTE car_switched_off;		// switched off from car
extern BYTE earthquake;				// earthquake (same function like switch off)
extern BYTE timer_trip;				// timer trip (real timer)
extern BYTE timer1_trip;			// timer 1 trip (input)
extern BYTE timer2_trip;			// timer 2 trip (input)
extern BYTE enable_carcall [];		//
extern BYTE disable_carcall [];		//
extern BYTE enable_hallcall [];		//
extern BYTE disable_hallcall [];	//
extern BYTE auto_call;				// auto-call function ("Sabbat-control")
extern BYTE auto_call_dir;			// direction for auto-call function

//**************************************************************************
// variables to check heartbeat of all boards
//**************************************************************************
extern BYTE heartbeat;				// heartbeat message received
extern BYTE init_error;				// boards not init
extern BYTE hb_error;				// mandatory boards without heartbeat
extern BYTE start_init;				// init since last start of HSE
extern BYTE hse_hb;					// heartbeat of other HSE in group
extern BYTE hse_hb_state;			// actual state of other HSE

//**************************************************************************
//	Timer variables
//**************************************************************************
#define SEC					*10		// change timer counts to seconds
#define TSE_TIME			(30 SEC)// error warning time if TSE fault
#define ESE_TIME			(30 SEC)// error warning time if ESE fault
#define TEMPERATURE_TIME	5		// 500ms for machine room temperature detect
#define LOAD_TIME			2		// 10ms for LOAD
#define	ADV_DR_OPEN_DLY		5		// at least 25ms delay from zone input to door open

#define PASSWORDTIME		1800	// Access for 15 minutes
#define LCD_BACKLIGHTTIME	120		// LCD backlight stays on for 60 seconds
#define	INSP_LIMIT_TIMER		3000	// inspection run or emergency recall limit time: 5 minutes

extern DWORD timer;					// general timer count variable
extern DWORD fxtimer;
extern DWORD realtimer;				// timer count variable (used in interrupt)
extern DWORD ulsi_off_timer;
extern DWORD mstimer;
extern DWORD ct;					// general timer for all state functions
extern DWORD parkingtimer;			// timer for parking trip
extern DWORD doorstoptimer;			// timer for door stop function
extern DWORD esetimer;				// timer for error warning if ESE fault
extern DWORD tsetimer;				// timer for error warning if TSE fault
extern DWORD fantimer;				// timer for cabin fan
extern BYTE outtimer;				// timer variable for setting output state
extern WORD brake_feedback_timer;
extern WORD passwordtimer;			// password timer
extern DWORD readload_timer;
extern DWORD stick_dect_timer;
extern DWORD canbus_check_timer;
extern BYTE error_recode_timer;

//**************************************************************************
//	Variables for monitoring
//**************************************************************************
#define MONITORING_ON			0x01
#define MONITORING_SEND			0x02

extern BYTE monitoring_state;		// state for the monitoring's state
extern BYTE monitoring_delay_time;	// every time sending after delay time

//**************************************************************************
// Variables for actual time
//**************************************************************************
extern BYTE time_new;				// Time changed
extern BYTE second_toggle;
extern BYTE menu_toggle_sec;
extern BYTE send_heartbeat;
extern BYTE sec_cnt;
extern BYTE send_level;
extern BYTE send_level_insp;
extern BYTE level_insp_fg;
extern BYTE settimeflag;
extern DWORD error_timer;

//**************************************************************************
//	String variables for software version of other boards
//**************************************************************************
extern BYTE version_fve [];			// software version number of FVE
extern BYTE hardware_fve [];			// hardware version number of FVE


//**************************************************************************
//	Variables for insp key press
//**************************************************************************
extern BYTE para_valid;
extern BYTE oldtse_arrowtype;
extern BYTE oldese_arrowtype;
extern BYTE oldtse_extnums;
extern BYTE emop_btn_stick;

//**************************************************************************
//	Variables for id teach in
//**************************************************************************
#define TSE_WATCH				0						// watch state
#define TSE_INIT_START			1						// start init
#define TSE_INIT_REQ			2						// send init request
#define TSE_INIT_SEG			3						// wait for init response (segmented transfer)
#define TSE_INIT_RES			4						// wait for init response
#define TSE_INIT_OP				5						// start operational mode
#define TSE_STATE_START			6						// start state search
#define TSE_STATE_REQ			7						// request state
#define TSE_STATE_SEG			8						// wait for state response (segmented transfer)
#define TSE_STATE_RES			9						// wait for state response
#define TSE_SET_OUT				10						// set outputs
#define TSE_HBINI_TIMER			(8*2)
#define TSE_HBCHK_TIMER			(15*2)

#define ESE_WATCH				0						// watch state
#define ESE_INIT_START			1						// start init
#define ESE_INIT_REQ			2						// send init request
#define ESE_INIT_SEG			3						// wait for init response (segmented transfer)
#define ESE_INIT_RES			4						// wait for init response
#define ESE_INIT_OP				5						// start operational mode
#define ESE_STATE_START			6						// start state search
#define ESE_STATE_REQ			7						// request state
#define ESE_STATE_SEG			8						// wait for state response (segmented transfer)
#define ESE_STATE_RES			9						// wait for all input states
#define ESE_BUS_OFF				10						// bus off state
#define ESE_WAIT_CAN			11						// wait for CAN
#define ESE_SET_OUT				12						// set outputs
#define ESE_SLAVE				13						// HSE is not master
#define ESE_HBINI_TIMER			(8*2)
#define ESE_HBCHK_TIMER			(15*2)		//运行之后 5S 检测外呼心跳

#define GSE_WATCH				0
#define GSE_BUS_OFF				10
#define GSE_WAIT_CAN			11

#define EXE_WATCH				0						// watch state
#define EXE_INIT_START			1						// start init
#define EXE_INIT_REQ			2						// send init request
#define EXE_INIT_SEG			3						// wait for init response (segmented transfer)
#define EXE_INIT_RES			4						// wait for init response
#define EXE_INIT_OP				5						// start operational mode
#define EXE_STATE_START			6						// start state search
#define EXE_STATE_REQ			7						// request state
#define EXE_STATE_SEG			8						// wait for state response (segmented transfer)
#define EXE_STATE_RES			9						// wait for state response
#define EXE_SET_OUT				10						// set outputs
#define EXE_HBINI_TIMER			(8*2)
#define EXE_HBCHK_TIMER			(5*2)

#define ACCESS_WATCH				0						// watch state
#define ACCESS_INIT_START			1						// start init
#define ACCESS_INIT_REQ			2						// send init request
#define ACCESS_INIT_SEG			3						// wait for init response (segmented transfer)
#define ACCESS_INIT_RES			4						// wait for init response
#define ACCESS_INIT_OP				5						// start operational mode
#define ACCESS_STATE_START			6						// start state search
#define ACCESS_STATE_REQ			7						// request state
#define ACCESS_STATE_SEG			8						// wait for state response (segmented transfer)
#define ACCESS_STATE_RES			9						// wait for state response
#define ACCESS_SET_OUT				10						// set outputs
#define ACCESS_HBINI_TIMER			(8*2)
#define ACESS_HBCHK_TIMER			(5*2)
extern BYTE tse_hb_timer;
extern BYTE tse_state;

extern BYTE ese_hb_timer;
extern BYTE ese_state;

extern BYTE exe_hb_timer;
extern BYTE exe_state;

extern DWORD tripcount_limit;
extern BYTE limitstatus;
extern BYTE bTestUCMP;
extern BYTE bTestLoad;
extern BYTE bForceResetTrip;
extern BYTE bTest_IlsSwitch;
extern BYTE bBreak_check_enable;
extern BYTE bFx_check_enable;			///0: 取消封星功能		1: 检测封星功能
extern BYTE bTemp_check_enable;			///0: 取消控制柜温度检测		1: 检测控制柜温度
extern BYTE bTestDL;
extern BYTE bTest_SIL;	
extern BYTE	forced_stop;
extern BYTE bTest_brake;					///1: 测试抱闸力
extern BYTE brake_value;					//测试抱闸力的结论: 1: 合格    2: 不合格
extern BYTE brake_value_old;
extern BYTE brake_check_forced;
extern BYTE brake_check_data;
extern BYTE brake_check_return;
extern BYTE brake_check_noactive;
extern BYTE brake_check_count;

#define	MAX_PARA_CHECKTIMES		10
#define	EE_READCHECK_SIZE			16
extern BYTE check_times;
extern BYTE check_para_doing;
extern BYTE para_checkbuff[];
#define LED_BUFF_LEN			8
extern BYTE led_buff[];

extern BYTE light_switch_status;
extern BYTE fan_switch_status;
extern BYTE dz_input;
extern BYTE power24v;
extern BYTE diving;
extern BYTE bResetTripDoing;
extern BYTE fve_hardware_ver_after_v13;
extern BYTE correct_level;

extern DWORD dTmrCnt;
extern DWORD dCurrentPulse;
extern DWORD dPosHcnt, dPosLcnt;

extern BYTE carlight_flag;
extern BYTE alarmfloor_flag;
extern BYTE landingcall_push;
extern BYTE landingcall_actfloor;
extern BYTE door_close_open;
extern BYTE cl_op_fg;
extern WORD floor_higth[MAX_FLOOR];	//楼高
extern BYTE floor_record_fg[MAX_FLOOR];
extern WORD menu_keytimer;
extern BYTE send_pos_fg;
extern BYTE car_alarm;
extern BYTE door_short_conn;

//判断强减所用变量
#define	C_SDS_UNWORK_CHECKTIMES		5
extern BYTE check_sdd1_working_status;
extern BYTE check_sdu1_working_status;
extern BYTE check_sds_working_status;
extern BYTE vu_not_on_recovery;
extern BYTE vo_not_on_recovery;
extern BYTE vovu_not_off_recovery;
extern WORD sdd1_unwork_times;
extern WORD sdu1_unwork_times;
extern WORD sds_unwork_times;

extern BYTE small_key_timer;
extern BYTE bSmallkey_enable;
extern BYTE move_dooropen_flag;

extern	WORD ldvalue;
extern	DWORD pos;//INIT_POSITION;				// Actual lift position
extern	BYTE simu_step;

extern	BYTE can_disturb[3];
extern	DWORD cana_error_count;
extern	DWORD canb_error_count;
extern	DWORD canc_error_count;
extern	BYTE can_error_timer;
extern	DWORD canb_heartbeat_time;
#define	CANB_HEARTBEAT_NOTREAD		72000		///2 小时记录一次CANB的心跳

extern	DWORD bFunc_flag;
extern	BYTE steel_broke;
extern	BYTE pitbox_reset_active;
extern	BYTE alarm_filter;
extern	BYTE door_closeswitch_limit;

extern BYTE evacuation_key;
extern DWORD evaction_waittime_nomove;
extern BYTE bot_floor_old;
extern BYTE top_floor_old;
extern WORD max_speed;

extern BYTE ucmp_test_finish;
extern BYTE ucmp_test_floor;
extern WORD ucmp_test_distance;

extern struct Parameter p;
extern struct exu_parameter exe_para;

SWORD DisableIrq (void);
SWORD EnableIrq (void);


void menu(void);
void cycle (void);
BYTE new_call(BYTE type);
void special_trip (void);
void handle_input (BYTE liftnumber, BYTE active);
void tse (void);
void exe (void);
void ese (void);
void gse (void);
BYTE start_enabled (void);
BYTE handle_dooropenpush (void);
void handle_doorstoppush (void);
BYTE set_park_state (void);
void fire_door (BYTE doors, BYTE command);
void set_door (BYTE doors, BYTE command);
BYTE relevelling (BYTE state, BYTE call);
void hse_out (BYTE basic, BYTE sub, BYTE floor, BYTE doors, BYTE state);
BYTE forced_closing (void);

void init_node (BYTE node);
BYTE send_parameter (BYTE *pointer, WORD subindex, DWORD value);
BYTE send_write_request (WORD menunumber, WORD paranumber, WORD pcb, BYTE bus, BYTE invers);
void send_iodata (BYTE *para, WORD pcb, BYTE bus);
void send_iopolarity (WORD menunumber, WORD paranumber, WORD pcb, BYTE para, BYTE bus, BYTE invers);
void send_ioenable (WORD menunumber, WORD paranumber, WORD pcb, BYTE para, BYTE bus, BYTE invers);
void write_io (BYTE *buffer, BYTE *para, BYTE size, BYTE io);
WORD random (WORD max);
void trim(BYTE * str);
DWORD GetMin(DWORD a, DWORD b);
DWORD GetMax(DWORD a, DWORD b);

extern	void set_parameter (void);
extern	void load_SetParameter(void);
extern	void read_hse_inputs (void);
extern	void config_double_door(void);
extern	void set_shaft_switch(void);
extern	void floor_name_change(WORD row);
extern	void LoadDefaultPara (void);
extern	BYTE send_hht_parameter (BYTE *pointer, WORD index, DWORD value);
extern	void config_botfloor_change(void);

#endif

