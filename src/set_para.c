
#include "typedef.h"
#include "stdio.h"
#include "string.h"
#include "parameter.h"
#include "state.h"
#include "can.h"
#include "text.h"
#include "lcd.h"
#include "io.h"
#include "history.h"
#include "version.h"
#include "eeprom.h"
#include "nice5000.h"


//*************************************************************************************
// set all other parameter
//*************************************************************************************
void set_parameter (void)
{
	WORD i, j, k;

// P0 General parameter
	p.bot_floor = 0;
	p.main_floor = p.bot_floor;
	p.parkfloor = p.main_floor;
	bot_floor_old = p.bot_floor;
	top_floor_old = p.top_floor;	
	p.setup_ready = 0;										// setup not ready
	p.motor_tuned = 0;
	p.groupsize = 1;
	p.groupnumber = p.groupsize;
	p.controltype = COLLECTING;
	if (p.groupsize == 1)
		p.lcmode_actfloor = LCM_REOPEN;
	else
		p.lcmode_actfloor = LCM_NO_REOPEN;
	if ((p.top_floor - p.bot_floor) < 14)
		p.tse_extnums = 0;
	else if ((p.top_floor - p.bot_floor) < (14 + 16))
		p.tse_extnums = 1;
	else if ((p.top_floor - p.bot_floor) < (14 + 32))
		p.tse_extnums = 2;
	else
		p.tse_extnums = 3;
	p.arrowtype = ARROW_SCROLL;
	p.doorzoneswitchsize = 225;			// Ĭ�Ͽ��峤��225mm(ʵ��Լ300mm)
	p.exe_number = 2;				//Ĭ���������1����չ�壬���ڿ��ƹ�
	p.fan_mode = FAN_AUTOMATIC;
	p.max_speed = 400;
	p.ucmp_speed = 300;

// P1 Timer parameter
	p.parkingtime = 300;
	p.triptime = 45;
	p.carlighttimer = 300;				//����ƿ���ʱ���60����Ϊ300
	p.doorstaytime_cc = 3;
	p.doorstaytime_lc = 5;
	p.doorstaytime_nc = 10;
	p.doorstaytime_main_cc = 3;
	p.doorstaytime_main_lc = 5;
	p.doorstaytime_main_nc = 10;
	p.doorstoptime = 300;
	p.errortime = 10;
	p.antifainttime = 0;	
	p.rsttrip_stop_dly = 500;
	p.fan_time = 60;
	p.upstrip_stop_delay = 1000;

// P2 Drive parameter
	p.starttime = 300;
	p.wait_for_contactor = 1000;
	p.max_ready_on = 2000;
	p.brake_on_delay = 600;
	p.max_brake_on = 2000;
	p.max_start = 1500;
	p.wait_for_brake = 1500;				//Ԥ���±�բ�����ʱ��ʱ
	p.max_stop = 2000;
	p.brake_off_delay = 600;
	p.max_brake_off = 5000;
	p.run_off_delay = 300;
	p.ready_off_delay = 300;
	p.max_ready_off = 5000;
	p.fx_offtime = 1000;
	p.fx_ontime = 1000;
	p.brake_feedback_dly = 600;
	p.brake_off_sig_dly = 800;

// P8 Floor name
	for (i = p.bot_floor; i <= p.top_floor; i++)		// Floor name
	{
		p.sign [i] = 0x30 + ((i - p.bot_floor + 1) % 10);
		if ((i - p.bot_floor + 1) > 9)
			p.sign [i] |= ((0x30 + ((i - p.bot_floor + 1) / 10)) << 8);
	}
	
// P9 Relevel param.
	p.adv_door_op = 0;
	p.arrival_speed = 100;
	p.pos_dooropen = 100;
	p.relevel = 0;
	p.relevel_trip = 0;
	p.ucmp_enable = 0;			//�������ò����� UCM ����
	p.ucmp_door_height = 2000;
	p.ucmp_car_apron = 750;
	p.ucmp_hall_apron = 350;
	p.ucmp_car_distance = 30;
	p.ucmp_test_dir = 0;
	p.ucmp_test_speed = 0;

// PA Internal param.	
	language = ENGLISH;				//ʹ�����ֲ���
	p.lift_func_sel1 = 139;//LIFTFUN1_DOORSHORT_REVEL | LIFTFUN1_PRESS_CLOSEBUTTOM 	| LIFTFUN1_CHECK_ROOM_TEMP | LIFTFUN1_CHECK_OVERSPEED;
	p.lcd_backlight = 66;	//Ĭ�ϵȴ�2min������

	p.evac_door = 1;			//Ĭ�Ϸ����������Ͽ���
	p.landingcall_push = 0;
	p.ups_direction_mode = UPS_DIR_SEL;		//Ĭ��ARD��Ԯʱ�������巽��
	p.canbus_check = 2;		//Ĭ������ʱ����� canbus ����
	p.emp_power_door = 3;	//Ĭ��Ӧ����Դ����ʱĬ�Ͽ���2����
	p.door_connect = 0;
	p.she_doornumber = 0;
	p.brake_check_auto = 0;
	p.brake_check_cycle = 0;	
	p.brake_check_time = 0;
	
// PD0-PD1 Door parameter
	for (i = 0; i < MAX_DOOR; i++)								/* door settings							*/
	{
		p.parkdoormode [i] = P_DOOR_CLOSED;
		p.lstime [i] = 3;
		p.revtime[i] = 6;
		p.nudgingtime[i] = 0;
		p.dooropentime [i] = 30;
		p.doorclosetime [i] = 30;
		p.dooropenlimit [i] = 4;
		p.doorcloselimit [i] = 4;
		p.doorswitchtime [i] = 100;
		p.doordelaytime [i] = 0;
		p.doorrelaymode[i] = 3;
		p.doorrelaytime_cl [i] = 0;
		p.doorrelaytime_op [i] = 0;
		p.max_ls_on [i] = 600;

		p.rampmode [i] = 0; 	// ??
		p.ramptiming [i] = 0;	// ??
		p.rampdelay [i] = 0;	// ??
		p.dooropendelay [i] = 0;
		p.rev_door [i] = 0;
	}

// PD2~PD15
	for (i = 0; i < MAX_FLOOR; i++)
	{
		p.doorpos [i] = 0x01;								// ����Ĭ�϶���ֻ��1����
		for (j = 0; j < MAX_DOORTABLE; j++) 			// PD4~PD15 Door release
			p.doortable [j][i] = HSE_MARIN[p.doornumber];
		p.parkdoor [i] = p.doorpos [i]; 				// PD3- Door park mode
		p.fire_floor [i] = i;				// PR4- Fire evac.floors
	}

//PD16
	p.liop_cop_door = CONFIG_CAR_DOOR;//CONFIG_SHAFT_DOOR | CONFIG_CAR_DOOR;
	p.open_close_door = CONFIG_OPEN_DOOR | CONFIG_CLOSE_DOOR;

//PM
	p.custumer1 = 0;	

// PR Run control
// PR0 special run adj.
	p.last_trip_special = 1;		// finish
	p.time_special = 0;
	p.call_active_special = 0;
	p.calltime_special = 5;
	p.time_back_special = 5;
	p.carcall_special_lc = 1;
	p.hallcall_special = 1;
	p.carcall_special = 3;
	p.max_cc [C_SPECIAL] = 0;
	p.normal_cc_special = 0;
	
// PR1 VIP run adj.
	p.last_trip_advanced = 0;
	p.time_advanced = 30;
	p.call_active_advanced = 0;
	p.calltime_advanced = 5;
	p.time_back_advanced = 5;
	p.carcall_advanced_lc = 1;
	p.hallcall_advanced = 1;
	p.carcall_advanced = 3;
	p.max_cc [C_ADVANCED] = 0;
	p.normal_cc_advanced = 0;
	
// PR2 emerg. run adj.
	p.last_trip_emergency = 0;
	p.time_emergency = 30;
	p.call_active_emergency = 0;
	p.calltime_emergency = 5;
	p.time_back_emergency = 5;
	p.carcall_emergency_lc = 1;
	p.hallcall_emergency = 1;
	p.carcall_emergency = 3;
	p.max_cc [C_EMERGENCY] = 0;
	p.normal_cc_emergency = 0;
	p.forced_close_emerg = 1;
	
// PR3 evacuation run
	p.evacuationdelay = 0;
	p.evacuationtime = 0;		//Ĭ��ֵ��Ϊ0s���������źŴ�����һ��
	p.evac_autoreturn = 1;
	p.evac_fire_carlightmode = 3;		//Ĭ�Ϲص�
	p.carlight_fan_time_fire = 5;		//Ĭ�Ϲص�ʱ��Ϊ5s	

// PR6 firemen run adj.
	p.fire_doorclosemode = 3;
	p.fire_dooropenmode = 3;
	p.firealarm_function = 61;//FIREALARM_FORCE_CLOSE | FIREALARM_DYNAMIC | FIREALARM_ACTIVE_DETECT | FIREALARM_RESET_MODE | FIREALARM_CROSS_FIREFLOOR;
	p.fireman_function1 = 57;//FIREMAN1_DOOR_MAINFLOOR | FIREMAN1_REPEAT_FIRECALL | FIREMAN1_RETURN_MAINFLOOR | FIREMAN1_AUTO_FIREMODE;

// PR7 compulsory stop
	p.forced_stop = 0;
	p.forced_stop_floor = p.main_floor;
	p.forced_stop_door = p.doorpos [p.forced_stop_floor];
	p.forced_stop_dir = HALL_CALL_UP | HALL_CALL_DOWN;
	p.forced_starttime = 0;
	p.forced_stoptime = 0;
	
// PR8 remote off
	p.remote_off_floor = p.bot_floor;
	p.remote_off_door = 0;		//���ֹ���״̬
	p.remote_off_func = REMOTE_LIGHT;
	p.remote_starttime = 0;
	p.remote_stoptime = 0;
	
// PR9 clock run
	p.timer_clearcall = 0;
	p.timertrip_start = 0;
	p.timertrip_stop = 0;
	p.timer1_parkfloor = p.parkfloor;
	p.timer2_parkfloor = p.parkfloor;
	
// PR10 control calls
	p.call_disable_enable = 28;//CALL_DISEN_AUTO_CC | CALL_DISEN_AUTO_LC | CALL_DISEN_PRIORITY;
	
// PR11 car attendance
	p.attend_carcall = 34;// ATTEND_CANCEL_CALL | ATTEND_SPEEKER;
	p.attend_normal_timer = 5;
	
// PR12 auto-call func.
	p.autocall_dir = HALL_CALL_UP | HALL_CALL_DOWN;
	p.cooperate_sel1 = 16;//COOP1_INSP_RETURN
	p.cooperate_sel2 = COOP2_CL_LIMIT;
	p.arrivegong_starttime = 22;		//ҹ����22:00--8:00ȡ����վ��
	p.arrivegong_stoptime = 8;

// PR14 anti nuisance
	p.cc_cancel = DOUBLE;
	p.dir_pending = 1;
	p.max_cc [C_STANDARD] = 0;
	p.max_cc_empty = 0;
	p.cc_no_ls = 0;
	p.clear_other_lc_if_cc = 0;
	p.clear_cc_last_floor = 0;
	p.clear_cc_opp_dir = 0;
	
// PR15 Peak control
	p.max_door_close_group = 20;
	p.up_peak_start_h1 = 0;
	p.up_peak_start_m1 = 0;
	p.up_peak_stop_h1 = 0;
	p.up_peak_stop_m1 = 0;	
	p.dn_peak_start_h1 = 0;
	p.dn_peak_start_m1 = 0;
	p.dn_peak_stop_h1 = 0;
	p.dn_peak_stop_m1 = 0;	
	p.up_peak_start_h2 = 0;
	p.up_peak_start_m2 = 0;
	p.up_peak_stop_h2 = 0;
	p.up_peak_stop_m2 = 0;	
	p.dn_peak_start_h2 = 0;
	p.dn_peak_start_m2 = 0;
	p.dn_peak_stop_h2 = 0;
	p.dn_peak_stop_m2 = 0;	
	p.up_peak_start_h3 = 0;
	p.up_peak_start_m3 = 0;
	p.up_peak_stop_h3 = 0;
	p.up_peak_stop_m3 = 0;	
	p.dn_peak_start_h3 = 0;
	p.dn_peak_start_m3 = 0;
	p.dn_peak_stop_h3 = 0;
	p.dn_peak_stop_m3 = 0;	
	p.up_peak_parkfloor = p.main_floor;
	p.dn_peak_parkfloor = p.main_floor;

//PR17 Test Run
	p.simulation = 0; 									// simulation off
	p.simu_mult = 0;

//PW 
	password_admin = 0;
	password_help = 0;
	password_install = 0;
	password_maintain = 0;

// MCU in-/outputs
	for (i = 0; i < MAX_HSE_IN; i++)					// clear all HSE input parameter
	{
		for (j = 0; j < MAX_IO_TYPE; j++)
			p.hse_inpar[i][j] = 0;
		p.hse_inpar[i][IO_LIFT] = LIFT1;
		p.hse_inpar[i][IO_ENABLE] = ENABLE;
	}
	for (i = 0; i < MAX_HSE_EX_IN; i++)					// clear all HSE input parameter
	{
		for (j = 0; j < MAX_IO_TYPE; j++)
			p.hse_ex_inpar[i][j] = 0;
		p.hse_ex_inpar[i][IO_LIFT] = LIFT1;
		p.hse_ex_inpar[i][IO_ENABLE] = ENABLE;
	}

	p.hse_inpar [0][IO_BASIC_FUNC] = EM_OP_IN;
	p.hse_inpar [0][IO_SUB_FUNC] = INSP_ON;
	p.hse_inpar [0][IO_ENABLE] = DISABLE;
	p.hse_inpar [1][IO_BASIC_FUNC] = EM_OP_IN;
	p.hse_inpar [1][IO_SUB_FUNC] = INSP_UP;
	p.hse_inpar [1][IO_ENABLE] = DISABLE;
	p.hse_inpar [2][IO_BASIC_FUNC] = EM_OP_IN;
	p.hse_inpar [2][IO_SUB_FUNC] = INSP_DOWN;
	p.hse_inpar [2][IO_ENABLE] = DISABLE;

	p.hse_inpar [INPUT_ILU][IO_BASIC_FUNC] = POS_IN;
	p.hse_inpar [INPUT_ILU][IO_SUB_FUNC] = POS_ILD;
	p.hse_inpar [INPUT_ILU][IO_STATE] = POLARITY_BIT;
	p.hse_inpar [INPUT_ILU][IO_ENABLE] = DISABLE;

	p.hse_inpar [INPUT_SDU1][IO_BASIC_FUNC] = FORCE_SWITCH;
	p.hse_inpar [INPUT_SDU1][IO_SUB_FUNC] = FORCE_SDU1;
	p.hse_inpar [INPUT_SDU1][IO_STATE] = POLARITY_BIT;
	p.hse_inpar [INPUT_SDU1][IO_ENABLE] = DISABLE;
	p.hse_inpar [INPUT_SDD1][IO_BASIC_FUNC] = FORCE_SWITCH;
	p.hse_inpar [INPUT_SDD1][IO_SUB_FUNC] = FORCE_SDD1;
	p.hse_inpar [INPUT_SDD1][IO_STATE] = POLARITY_BIT;
	p.hse_inpar [INPUT_SDD1][IO_ENABLE] = DISABLE;

	if(p.loadtype == DIGIT_LOAD_MCU)
		{
			p.hse_inpar [INPUT_SDU2][IO_BASIC_FUNC] = LOAD_IN;
			p.hse_inpar [INPUT_SDU2][IO_SUB_FUNC] = ZERO_LOAD;
			p.hse_inpar [INPUT_SDU2][IO_ENABLE] = ENABLE;
		}
	else
		{
			p.hse_inpar [INPUT_SDU2][IO_BASIC_FUNC] = FORCE_SWITCH;
			p.hse_inpar [INPUT_SDU2][IO_SUB_FUNC] = FORCE_SDU2;
			p.hse_inpar [INPUT_SDU2][IO_STATE] = POLARITY_BIT;
			p.hse_inpar [INPUT_SDU2][IO_ENABLE] = DISABLE;
		}
	p.hse_inpar [INPUT_SDD2][IO_BASIC_FUNC] = FORCE_SWITCH;
	p.hse_inpar [INPUT_SDD2][IO_SUB_FUNC] = FORCE_SDD2;
	p.hse_inpar [INPUT_SDD2][IO_STATE] = POLARITY_BIT;
	p.hse_inpar [INPUT_SDD2][IO_ENABLE] = DISABLE;

	p.hse_inpar [9][IO_BASIC_FUNC] = TEMPERATURE;
	p.hse_inpar [9][IO_SUB_FUNC] = OVERTEMP1;

	if(p.loadtype == DIGIT_LOAD_MCU)
		{
			p.hse_inpar [INPUT_SDD3][IO_BASIC_FUNC] = LOAD_IN;
			p.hse_inpar [INPUT_SDD3][IO_SUB_FUNC] = FULL_LOAD;
			p.hse_inpar [INPUT_SDD3][IO_ENABLE] = ENABLE;
		}
	else
		{
			p.hse_inpar [INPUT_SDD3][IO_BASIC_FUNC] = FORCE_SWITCH;
			p.hse_inpar [INPUT_SDD3][IO_SUB_FUNC] = FORCE_SDD3;
			p.hse_inpar [INPUT_SDD3][IO_STATE] = POLARITY_BIT;
			p.hse_inpar [INPUT_SDD3][IO_ENABLE] = DISABLE;
		}
	
	p.hse_inpar [INPUT_ILO][IO_BASIC_FUNC] = POS_IN;
	p.hse_inpar [INPUT_ILO][IO_SUB_FUNC] = POS_ILU;
	p.hse_inpar [INPUT_ILO][IO_STATE] = POLARITY_BIT;
	p.hse_inpar [INPUT_ILO][IO_ENABLE] = DISABLE;

// F5 parameter
	p.F5_X1 = NICE5000_INPUT_MSI;
	p.F5_X2 = 0;
	p.F5_X3 = 0;//NICE5000_INPUT_SDU1 + NICE5000_INPUT_LOGIC;
	p.F5_X4 = 0;//NICE5000_INPUT_SDD1 + NICE5000_INPUT_LOGIC;
	p.F5_X5 = 0;
	p.F5_X6 = 0;
	p.F5_X7 = 0;
	p.F5_X8 = 0;

	p.hse_inpar [13][IO_BASIC_FUNC] = CONT_CHECK;
	p.hse_inpar [13][IO_SUB_FUNC] = D_FXCONT_SUP;
	p.hse_inpar [13][IO_STATE] = POLARITY_BIT;

	p.hse_inpar [14][IO_BASIC_FUNC] = SPECIAL_FUNC;
	p.hse_inpar [14][IO_SUB_FUNC] = UPS_EVACUATION;

	p.hse_inpar [15][IO_BASIC_FUNC] = CONT_CHECK;
	p.hse_inpar [15][IO_SUB_FUNC] = D_RUNCONT_SUP;
	p.hse_inpar [15][IO_STATE] = POLARITY_BIT;
	p.hse_inpar [16][IO_BASIC_FUNC] = CONT_CHECK;
	p.hse_inpar [16][IO_SUB_FUNC] = D_BRKCONT_SUP;
	p.hse_inpar [16][IO_STATE] = POLARITY_BIT;
	contactor_check = D_RUNCONT_SUP | D_FXCONT_SUP | D_BRKCONT_SUP;

	p.hse_inpar [17][IO_BASIC_FUNC] = BRAKE_CHECK;
	p.hse_inpar [17][IO_SUB_FUNC] = BRAKE1;
	p.hse_inpar [17][IO_ENABLE] = DISABLE;
	brake_check = 0;

	p.hse_inpar [IO_SC1][IO_BASIC_FUNC] = SAFETY_CIRCUIT;
	p.hse_inpar [IO_SC1][IO_SUB_FUNC] = SC1;
	p.hse_inpar [IO_SC2][IO_BASIC_FUNC] = SAFETY_CIRCUIT;
	p.hse_inpar [IO_SC2][IO_SUB_FUNC] = SC2;
	p.hse_inpar [IO_SC3][IO_BASIC_FUNC] = SAFETY_CIRCUIT;
	p.hse_inpar [IO_SC3][IO_SUB_FUNC] = SC3;

	p.hse_ex_inpar [2][IO_BASIC_FUNC] = SPECIAL_FUNC;
	p.hse_ex_inpar [2][IO_SUB_FUNC] = FIRE_EVACUATION;
	p.hse_ex_inpar [2][IO_STATE] = POLARITY_BIT;

	for (i = 0; i < MAX_HSE_IN; i++)
	{
		if (p.hse_inpar [i][IO_STATE] & POLARITY_BIT)
			bit_set (in_polarity, i);
		else
			bit_reset (in_polarity, i);
	}
	for (i = 0; i < MAX_HSE_EX_IN; i++)
	{
		if (p.hse_ex_inpar [i][IO_STATE] & POLARITY_BIT)
			bit_set (in_polarity, (i + MAX_HSE_IN));
		else
			bit_reset (in_polarity, (i + MAX_HSE_IN));
	}
	for (i = 0; i < MAX_HSE_OUT; i++) 						// clear all HSE output parameter
	{
		for (j = 0; j < MAX_IO_TYPE; j++)
			p.hse_outpar [i][j] = 0;
		p.hse_outpar [i][IO_LIFT] = LIFT1;
		p.hse_outpar [i][IO_ENABLE] = ENABLE;
	}
	for (i = 0; i < MAX_HSE_EX_OUT; i++)
	{
		for (j = 0; j < MAX_IO_TYPE; j++)
			p.hse_ex_outpar [i][j] = 0;
		p.hse_ex_outpar [i][IO_LIFT] = LIFT1;
		p.hse_ex_outpar [i][IO_ENABLE] = ENABLE;
	}
	p.hse_outpar [0][IO_BASIC_FUNC] = DRIVE_OUT;
	p.hse_outpar [0][IO_SUB_FUNC] = D_MAIN;
	p.hse_outpar [1][IO_BASIC_FUNC] = DRIVE_OUT;
	p.hse_outpar [1][IO_SUB_FUNC] = D_BRAKE;

	p.hse_outpar [3][IO_BASIC_FUNC] = SPECIAL_FUNC;
	p.hse_outpar [3][IO_SUB_FUNC] = UCMP_LOCK;

	p.hse_ex_outpar [0][IO_BASIC_FUNC] = SPECIAL_FUNC;
	p.hse_ex_outpar [0][IO_SUB_FUNC] = FIRE_ALARM_FLOOR;
	p.hse_ex_outpar [0][IO_STATE] = POLARITY_BIT;

	p.hse_ex_outpar [1][IO_BASIC_FUNC] = SPECIAL_FUNC;
	p.hse_ex_outpar [1][IO_SUB_FUNC] = UPS_EVAC_READY;

	for (i = 0; i < MAX_HSE_OUT; i++)
	{
		if (p.hse_outpar [i][IO_STATE] & POLARITY_BIT)
			bit_set (out_polarity, i);
		else
			bit_reset (out_polarity, i);
	}
	for (i = 0; i < MAX_HSE_EX_OUT; i++)
	{
		if (p.hse_ex_outpar [i][IO_STATE] & POLARITY_BIT)
			bit_set (out_polarity, (i + MAX_HSE_OUT));
		else
			bit_reset (out_polarity, (i + MAX_HSE_OUT));
	}

// CDU in-/outputs
	for (i = 0; i < MAX_FVE_IN; i++)							// clear all FVE input parameter
	{
		for (j = 0; j < MAX_IO_TYPE; j++)
			p.fve_inpar[i][j] = 0;
		p.fve_inpar[i][IO_LIFT] = LIFT1;
		p.fve_inpar [i][IO_ENABLE] = ENABLE;
	}
	for (i = 0; i < MAX_FVE_EX_IN; i++)							// clear all FVE input parameter
	{
		for (j = 0; j < MAX_IO_TYPE; j++)
			p.fve_ex_inpar[i][j] = 0;
		p.fve_ex_inpar[i][IO_LIFT] = LIFT1;
		p.fve_ex_inpar [i][IO_ENABLE] = ENABLE;
	}
	for (i = 0; i < MAX_FVE_OUT; i++)							// clear all FVE output parameter
	{
		for (j = 0; j < MAX_IO_TYPE; j++)
			p.fve_outpar [i][j] = 0;
		p.fve_outpar [i][IO_LIFT] = LIFT1;
		p.fve_outpar [i][IO_ENABLE] = ENABLE;
	}

	p.fve_inpar [0][IO_BASIC_FUNC] = DOORTEST;
	p.fve_inpar [0][IO_STATE] = POLARITY_BIT;
	p.fve_inpar [1][IO_BASIC_FUNC] = INSPECT_IN;
	p.fve_inpar [1][IO_SUB_FUNC] = INSP_TOPCAR_UP;
	p.fve_inpar [1][IO_STATE] = POLARITY_BIT;
	p.fve_inpar [2][IO_BASIC_FUNC] = INSPECT_IN;
	p.fve_inpar [2][IO_SUB_FUNC] = INSP_TOPCAR_DOWN;
	p.fve_inpar [2][IO_STATE] = POLARITY_BIT;

	p.fve_inpar [3][IO_BASIC_FUNC] = LOAD_IN;
	p.fve_inpar [3][IO_SUB_FUNC] = OVERLOAD;
	p.fve_inpar [3][IO_STATE] = POLARITY_BIT;
	p.fve_inpar [3][IO_ENABLE] = DISABLE;

	p.fve_inpar [4][IO_BASIC_FUNC] = INSPECT_IN;
	p.fve_inpar [4][IO_SUB_FUNC] = INSP_TOPCAR_ON;
	p.fve_inpar [4][IO_STATE] = POLARITY_BIT;

	p.fve_inpar [5][IO_BASIC_FUNC] = DOOR_IO;
	p.fve_inpar [5][IO_SUB_FUNC] = DOOR_CL_LIMIT;
	p.fve_inpar [5][IO_DOOR] = DOOR1;
	p.fve_inpar [6][IO_BASIC_FUNC] = DOOR_IO;
	p.fve_inpar [6][IO_SUB_FUNC] = DOOR_CL_LIMIT;
	p.fve_inpar [6][IO_DOOR] = DOOR2;

	p.fve_inpar [7][IO_BASIC_FUNC] = POS_IN;
	p.fve_inpar [7][IO_SUB_FUNC] = POS_SGD;
	p.fve_inpar [7][IO_ENABLE] = DISABLE;
	p.fve_inpar [8][IO_BASIC_FUNC] = POS_IN;
	p.fve_inpar [8][IO_SUB_FUNC] = POS_SGU;
	p.fve_inpar [8][IO_ENABLE] = DISABLE;

	p.fve_inpar[10][IO_BASIC_FUNC] = DOOR_IO;
	p.fve_inpar[10][IO_SUB_FUNC] = DOOR_OP;
	p.fve_inpar[10][IO_DOOR] = DOOR1;
	p.fve_inpar[11][IO_BASIC_FUNC] = DOOR_IO;
	p.fve_inpar[11][IO_SUB_FUNC] = DOOR_CL;
	p.fve_inpar[11][IO_DOOR] = DOOR1;

	p.fve_inpar[12][IO_BASIC_FUNC] = DOOR_IO;
	p.fve_inpar[12][IO_SUB_FUNC] = DOOR_OP;
	p.fve_inpar[12][IO_DOOR] = DOOR2;
	p.fve_inpar[13][IO_BASIC_FUNC] = DOOR_IO;
	p.fve_inpar[13][IO_SUB_FUNC] = DOOR_CL;
	p.fve_inpar[13][IO_DOOR] = DOOR2;
	p.fve_inpar[14][IO_BASIC_FUNC] = DOOR_LS;
	p.fve_inpar[14][IO_DOOR] = DOOR1;
	p.fve_inpar[15][IO_BASIC_FUNC] = DOOR_LS;
	p.fve_inpar[15][IO_DOOR] = DOOR2;
	if (p.doornumber < MAX_DOOR)
		{
			p.fve_inpar[12][IO_ENABLE] = DISABLE;
			p.fve_inpar[13][IO_ENABLE] = DISABLE;
			p.fve_inpar[15][IO_ENABLE] = DISABLE;
		}

	p.fve_inpar [16][IO_BASIC_FUNC] = LOAD_IN;
	p.fve_inpar [16][IO_SUB_FUNC] = ZERO_LOAD;
	p.fve_inpar [16][IO_STATE] = POLARITY_BIT;
	p.fve_inpar [16][IO_ENABLE] = DISABLE;

	p.fve_inpar [17][IO_BASIC_FUNC] = CARLIGHT;
	p.fve_inpar [17][IO_STATE] = POLARITY_BIT;

	p.fve_ex_inpar [0][IO_BASIC_FUNC] = LOAD_IN;
	p.fve_ex_inpar [0][IO_SUB_FUNC] = FULL_LOAD;
	p.fve_ex_inpar [0][IO_STATE] = POLARITY_BIT;
	p.fve_ex_inpar [0][IO_ENABLE] = DISABLE;

	p.fve_outpar [0][IO_BASIC_FUNC] = SPECIAL_FUNC;
	p.fve_outpar [0][IO_SUB_FUNC] = DOOR_CONN_ALARM;

	p.fve_outpar [1][IO_BASIC_FUNC] = ARRIVAL_INDICATION;
	p.fve_outpar [1][IO_SUB_FUNC] = DIR_UP;
	if(p.doornumber > 1)
		p.fve_outpar [1][IO_DOOR] = DOOR1 | DOOR2;
	else
		p.fve_outpar [1][IO_DOOR] = DOOR1;
	p.fve_outpar [2][IO_BASIC_FUNC] = ARRIVAL_INDICATION;
	p.fve_outpar [2][IO_SUB_FUNC] = DIR_DN;
	if(p.doornumber > 1)
		p.fve_outpar [2][IO_DOOR] = DOOR1 | DOOR2;
	else
		p.fve_outpar [2][IO_DOOR] = DOOR1;
	p.fve_outpar [3][IO_BASIC_FUNC] = DOOR_IO;
	p.fve_outpar [3][IO_SUB_FUNC] = DOOR_CL;
	p.fve_outpar [3][IO_DOOR] = DOOR1;
	p.fve_outpar [4][IO_BASIC_FUNC] = DOOR_IO;
	p.fve_outpar [4][IO_SUB_FUNC] = DOOR_OP;
	p.fve_outpar [4][IO_DOOR] = DOOR1;
	if (p.doornumber > 1)
		{
			p.fve_outpar [5][IO_BASIC_FUNC] = DOOR_IO;
			p.fve_outpar [5][IO_SUB_FUNC] = DOOR_CL;
			p.fve_outpar [5][IO_DOOR] = DOOR2;
			p.fve_outpar [6][IO_BASIC_FUNC] = DOOR_IO;
			p.fve_outpar [6][IO_SUB_FUNC] = DOOR_OP;
			p.fve_outpar [6][IO_DOOR] = DOOR2;
		}
	p.fve_outpar [7][IO_BASIC_FUNC] = CARLIGHT;
	p.fve_outpar [7][IO_STATE] = POLARITY_BIT;
	p.fve_outpar [8][IO_BASIC_FUNC] = SPECIAL_FUNC;
	p.fve_outpar [8][IO_SUB_FUNC] = FAN_1;
	p.fve_outpar [8][IO_STATE] = POLARITY_BIT;
	p.fve_outpar [9][IO_BASIC_FUNC]	= EMERGENCY_LIGHT;
	p.fve_outpar [9][IO_STATE] = POLARITY_BIT;

// PCU in-/outputs
	ClrWdt();
	memset(p.tse_inpar, 0x00, sizeof(p.tse_inpar));
	memset(p.tse_outpar, 0x00, sizeof(p.tse_outpar));
	for (i = 0; i < MAX_TSE; i++)
	{
		for (j = 0; j < MAX_TSE_IN; j++)
		{
			for (k = 0; k < 6; k++)
				p.tse_inpar [i][j][k]= 0;
			p.tse_inpar[i][j][IO_LIFT] = LIFT1;
			p.tse_inpar [i][j][IO_ENABLE] = ENABLE;
		}
	}
	for (i = 0; i < MAX_TSE; i++)
	{
		for (j = 0; j < MAX_TSE_OUT; j++)
		{
			for (k = 0; k < 6; k++)
				p.tse_outpar [i][j][k]= 0;
			p.tse_outpar[i][j][IO_LIFT] = LIFT1;
			p.tse_outpar [i][j][IO_ENABLE] = ENABLE;
		}
	}
	
	for (i = 0; i < MAX_TSE; i++)
	{
		p.tse_inpar [i][0][IO_BASIC_FUNC] = SPECIAL_FUNC;
		p.tse_inpar [i][0][IO_SUB_FUNC] = DOOR_CLOSE;
		p.tse_inpar [i][0][IO_DOOR] = HSE_MARIN[p.doornumber];
		p.tse_outpar [i][0][IO_BASIC_FUNC] = SPECIAL_FUNC;
		p.tse_outpar [i][0][IO_SUB_FUNC] = DOOR_CLOSE;
		p.tse_outpar [i][0][IO_DOOR] = HSE_MARIN[p.doornumber];
		p.tse_inpar [i][1][IO_BASIC_FUNC] = SPECIAL_FUNC;
		p.tse_inpar [i][1][IO_SUB_FUNC] = DOOR_OPEN;
		p.tse_inpar [i][1][IO_DOOR] = HSE_MARIN[p.doornumber];
		p.tse_outpar [i][1][IO_BASIC_FUNC] = SPECIAL_FUNC;
		p.tse_outpar [i][1][IO_SUB_FUNC] = DOOR_OPEN;
		p.tse_outpar [i][1][IO_DOOR] = HSE_MARIN[p.doornumber];

		for (j = 2; j <= 14; ++j)
		{
			p.tse_inpar[i][j][IO_BASIC_FUNC] = CAR_CALL;
			p.tse_inpar[i][j][IO_SUB_FUNC] = j - 2 + p.bot_floor + 1;
			p.tse_inpar[i][j][IO_DOOR] = p.doorpos [j - 2 + p.bot_floor];
			p.tse_outpar[i][j][IO_BASIC_FUNC] = CAR_CALL;
			p.tse_outpar[i][j][IO_SUB_FUNC] = j - 2 + p.bot_floor + 1;
			p.tse_outpar[i][j][IO_DOOR] = p.doorpos [j - 2 + p.bot_floor];
		}

		p.tse_inpar[i][15][IO_BASIC_FUNC] = SPECIAL_FUNC;
		p.tse_inpar[i][15][IO_SUB_FUNC] = FIRE_SERVICE_ENABLE;
		p.tse_inpar[i][16][IO_BASIC_FUNC] = SPECIAL_FUNC;
		p.tse_inpar[i][16][IO_SUB_FUNC] = CALLDIR_UP;
		p.tse_inpar[i][17][IO_BASIC_FUNC] = SPECIAL_FUNC;
		p.tse_inpar[i][17][IO_SUB_FUNC] = CALL_BYPASS;
		p.tse_inpar[i][18][IO_BASIC_FUNC] = SPECIAL_FUNC;
		p.tse_inpar[i][18][IO_SUB_FUNC] = CALLDIR_DN;
		p.tse_inpar[i][19][IO_BASIC_FUNC] = SPECIAL_FUNC;
		p.tse_inpar[i][19][IO_SUB_FUNC] = FAN_SWITCH;
		p.tse_inpar[i][20][IO_BASIC_FUNC] = SPECIAL_FUNC;	//SPECIAL_FUNC;
		p.tse_inpar[i][20][IO_SUB_FUNC] = CAR_ATTENDANCE;
		p.tse_inpar[i][21][IO_BASIC_FUNC] = SPECIAL_FUNC;
		p.tse_inpar[i][21][IO_SUB_FUNC] = LIGHT_SWITCH;
		
		for (j = 0; j < p.tse_extnums; ++j)
		{
			for (k = 0; k < 16; ++k)
			{
				if ((j*16 + k + 13) <= (p.top_floor - p.bot_floor))
				{
					p.tse_inpar[i][22+j*16+k][IO_BASIC_FUNC] = CAR_CALL;
					p.tse_inpar[i][22+j*16+k][IO_SUB_FUNC] = 13 + j*16 + k + p.bot_floor + 1;
					p.tse_inpar[i][22+j*16+k][IO_DOOR] = p.doorpos [13 + j*16 + k + p.bot_floor];
					p.tse_outpar[i][22+j*16+k][IO_BASIC_FUNC] = CAR_CALL;
					p.tse_outpar[i][22+j*16+k][IO_SUB_FUNC] = 13 + j*16 + k + p.bot_floor + 1;
					p.tse_outpar[i][22+j*16+k][IO_DOOR] = p.doorpos [13 + j*16 + k + p.bot_floor];
				}
				else
					break;
			}
		}
	}
	tse_enabled = HSE_MARIN[p.tse_number];

// LCU in-/outputs
	memset(p.ese_inpar, 0x00, sizeof(p.ese_inpar));
	memset(p.ese_outpar, 0x00, sizeof(p.ese_outpar));
	for (i = 0; i < MAX_ESE_DW; i++)
		ese_enabled [i] = 0;
	for (i = 0; i < MAX_ESE; i++)
	{
		for (j = 0; j < MAX_ESE_IN; ++j)
		{
			p.ese_inpar[i][j][IO_LIFT] = LIFT1;
			p.ese_inpar[i][j][IO_ENABLE] = ENABLE;
		}
		for (j = 0; j < MAX_ESE_OUT; ++j)
		{
			p.ese_outpar[i][j][IO_LIFT] = LIFT1;
			p.ese_outpar[i][j][IO_ENABLE] = ENABLE;
		}
	}
	for (i = 0; i < p.ese_number; ++ i)
	{
		p.ese_inpar[i][0][IO_BASIC_FUNC] = HALL_CALL;
		p.ese_inpar[i][1][IO_BASIC_FUNC] = HALL_CALL;

		p.ese_outpar[i][0][IO_BASIC_FUNC] = HALL_CALL;				//����ǹ̶���
		p.ese_outpar[i][1][IO_BASIC_FUNC] = HALL_CALL;
		p.ese_outpar[i][2][IO_BASIC_FUNC] = LIGHT_FUNC;
		p.ese_outpar[i][2][IO_SUB_FUNC] = HALL_LANTERN_UP;
		p.ese_outpar[i][3][IO_BASIC_FUNC] = LIGHT_FUNC;
		p.ese_outpar[i][3][IO_SUB_FUNC] = HALL_LANTERN_DN;

		if (p.controltype == SINGLE_CALL)
		{//��ѡģʽ
			p.ese_inpar[i][0][IO_SUB_FUNC] = HALL_CALL_NO_DIR;
			p.ese_inpar[i][1][IO_SUB_FUNC] = HALL_CALL_NO_DIR;

			p.ese_outpar[i][0][IO_SUB_FUNC] = HALL_CALL_NO_DIR;
			p.ese_outpar[i][1][IO_SUB_FUNC] = HALL_CALL_NO_DIR;
		}
		else
		{//��ѡģʽ
			p.ese_inpar[i][0][IO_SUB_FUNC] = HALL_CALL_UP;
			p.ese_inpar[i][1][IO_SUB_FUNC] = HALL_CALL_DOWN;

			p.ese_outpar[i][0][IO_SUB_FUNC] = HALL_CALL_UP;
			p.ese_outpar[i][1][IO_SUB_FUNC] = HALL_CALL_DOWN;
		}
		p.ese_inpar[i][0][IO_FLOOR] = i + p.bot_floor + 1;
		p.ese_inpar[i][1][IO_FLOOR] = i + p.bot_floor + 1;

		p.ese_outpar[i][0][IO_FLOOR] = i + p.bot_floor + 1;
		p.ese_outpar[i][1][IO_FLOOR] = i + p.bot_floor + 1;
		p.ese_outpar[i][2][IO_FLOOR] = i + p.bot_floor + 1;
		p.ese_outpar[i][3][IO_FLOOR] = i + p.bot_floor + 1;
		
		p.ese_inpar[i][0][IO_DOOR] = p.doorpos [i + p.bot_floor];
		p.ese_inpar[i][1][IO_DOOR] = p.doorpos [i + p.bot_floor];

		p.ese_outpar[i][0][IO_DOOR] = p.doorpos [i + p.bot_floor];
		p.ese_outpar[i][1][IO_DOOR] = p.doorpos [i + p.bot_floor];
		p.ese_outpar[i][2][IO_DOOR] = p.doorpos [i + p.bot_floor];
		p.ese_outpar[i][3][IO_DOOR] = p.doorpos [i + p.bot_floor];
		
		p.ese_inpar[i][0][IO_STATE] = POLARITY_BIT;
		p.ese_inpar[i][1][IO_STATE] = POLARITY_BIT;

		if (i == p.bot_floor)
		{
			p.ese_inpar[i][2][IO_BASIC_FUNC] = SPECIAL_FUNC;
			p.ese_inpar[i][2][IO_SUB_FUNC] = FIRE_SERVICE;
			p.ese_inpar[i][2][IO_FLOOR] = p.bot_floor + 1;
			p.ese_inpar[i][2][IO_DOOR] = p.doorpos[p.bot_floor];
			p.ese_inpar[i][2][IO_STATE] = POLARITY_BIT;

			p.ese_inpar[i][3][IO_BASIC_FUNC] = SPECIAL_FUNC;
			p.ese_inpar[i][3][IO_SUB_FUNC] = REMOTE_OFF;//KEY_OFF;
			p.ese_inpar[i][3][IO_FLOOR] = p.bot_floor + 1;
			p.ese_inpar[i][3][IO_DOOR] = p.doorpos[p.bot_floor];
			p.ese_inpar[i][3][IO_STATE] = POLARITY_BIT;
		}
	}
	for (i = 0; i < p.ese_number/32; i++)
		ese_enabled [i] = 0xffffffff;
	ese_enabled[p.ese_number/32] = HSE_MARIN[p.ese_number%32];

//������չ���������� EXE
	ClrWdt();
	memset(exe_para.exe_inpar, 0x00, sizeof(exe_para.exe_inpar));
	memset(exe_para.exe_outpar, 0x00, sizeof(exe_para.exe_outpar));
	for (i = 0; i < MAX_EXE; i++)
		{
			for (j = 0; j < MAX_EXE_IN; j++)
				{
					for (k = 0; k < 6; k++)
						exe_para.exe_inpar [i][j][k]= 0;
					exe_para.exe_inpar[i][j][IO_LIFT] = LIFT1;
					exe_para.exe_inpar [i][j][IO_ENABLE] = DISABLE;
				}
		}
	for (i = 0; i < MAX_EXE; i++)
		{
			for (j = 0; j < MAX_EXE_OUT; j++)
				{
					for (k = 0; k < 6; k++)
						exe_para.exe_outpar [i][j][k]= 0;
					exe_para.exe_outpar[i][j][IO_LIFT] = LIFT1;
					exe_para.exe_outpar [i][j][IO_ENABLE] = DISABLE;
				}
		}
//EXE1	
	exe_para.exe_inpar [0][0][IO_BASIC_FUNC] = EM_OP_IN;
	exe_para.exe_inpar [0][0][IO_SUB_FUNC] = INSP_ON;
	exe_para.exe_inpar [0][0][IO_ENABLE] = ENABLE;
	
	exe_para.exe_inpar [0][1][IO_BASIC_FUNC] = EM_OP_IN;
	exe_para.exe_inpar [0][1][IO_SUB_FUNC] = INSP_UP;
	exe_para.exe_inpar [0][1][IO_ENABLE] = ENABLE;
	
	exe_para.exe_inpar [0][2][IO_BASIC_FUNC] = EM_OP_IN;
	exe_para.exe_inpar [0][2][IO_SUB_FUNC] = INSP_DOWN;
	exe_para.exe_inpar [0][2][IO_ENABLE] = ENABLE;
	
	exe_para.exe_inpar [0][3][IO_BASIC_FUNC] = DOOR_FUNCTION;
	exe_para.exe_inpar [0][3][IO_SUB_FUNC] = CARDOOR_CONNECT;
	exe_para.exe_inpar [0][3][IO_ENABLE] = ENABLE;
	
	exe_para.exe_inpar [0][4][IO_BASIC_FUNC] = DOOR_FUNCTION;
	exe_para.exe_inpar [0][4][IO_SUB_FUNC] = SHAFTDOOR_CONNECT;
	exe_para.exe_inpar [0][4][IO_ENABLE] = ENABLE;
	
	exe_para.exe_outpar [0][0][IO_BASIC_FUNC] = SPECIAL_FUNC;
	exe_para.exe_outpar [0][0][IO_SUB_FUNC] = ALARM_FILTER;
	exe_para.exe_outpar [0][0][IO_ENABLE] = ENABLE;
	
//EXE2
	exe_para.exe_inpar [1][0][IO_BASIC_FUNC] = INSPECT_IN;
	exe_para.exe_inpar [1][0][IO_SUB_FUNC] = INSP_PITBOX_ON;
	exe_para.exe_inpar [1][0][IO_ENABLE] = ENABLE;
	
	exe_para.exe_inpar [1][1][IO_BASIC_FUNC] = INSPECT_IN;
	exe_para.exe_inpar [1][1][IO_SUB_FUNC] = INSP_PITBOX_UP;
	exe_para.exe_inpar [1][1][IO_ENABLE] = ENABLE;
	
	exe_para.exe_inpar [1][2][IO_BASIC_FUNC] = INSPECT_IN;
	exe_para.exe_inpar [1][2][IO_SUB_FUNC] = INSP_PITBOX_DOWN;
	exe_para.exe_inpar [1][2][IO_ENABLE] = ENABLE;
	
	exe_para.exe_inpar [1][5][IO_BASIC_FUNC] = SPECIAL_FUNC;
	exe_para.exe_inpar [1][5][IO_SUB_FUNC] = RESET_PITBOX;
	exe_para.exe_inpar [1][5][IO_ENABLE] = ENABLE;

	exe_para.exe_outpar [1][0][IO_BASIC_FUNC] = SPECIAL_FUNC;
	exe_para.exe_outpar [1][0][IO_SUB_FUNC] = RESET_PITBOX;
	exe_para.exe_outpar [1][0][IO_ENABLE] = ENABLE;

	exe_enabled = HSE_MARIN[p.exe_number % 33];

	force_switch = 0;
	oldin = (~in) & HSE_MARIN[MAX_HSE_IN + MAX_HSE_EX_IN];
	init_error |= B_DRV;
	nice5000_basicsetting_init = 1;
}

void config_double_door(void){
	BYTE i, j, k = 0, m;

//�����������ű�
	for (i = p.bot_floor; i <= p.top_floor; ++i)
		{
			for (j = 0; j < MAX_DOORTABLE; j++) 			// PD4~PD15 Door release
				p.doortable [j][i] = p.doorpos [i];
			p.parkdoor [i] = p.doorpos [i]; 				// PD3- Door park mode
		}
	
//ʵ�ʵĲ���
	for (i = p.bot_floor; i <= p.top_floor; ++i)
		{//���ŵ�����: ����忪2����, ��Ҫ��¥����Ҫ������ȷ		
			for (j = 0; j < p.doornumber; ++j)
				{
					if (p.doorpos[i] & (1 << j))
						{
							if(k < p.ese_number)
								{
									for(m = 0; m < MAX_ESE_IN; m++)
										{ 				
											if(p.ese_inpar [k][m][IO_BASIC_FUNC] == HALL_CALL)
												{
													p.ese_inpar [k][m][IO_FLOOR] = i + 1; 														
													p.ese_outpar [k][m][IO_FLOOR] = i + 1;															
													if(p.liop_cop_door & CONFIG_SHAFT_DOOR)
														{
															p.ese_inpar [k][m][IO_DOOR] = p.doorpos[i];
															p.ese_outpar [k][m][IO_DOOR] = p.doorpos[i];
														}
													else
														{
															p.ese_inpar [k][m][IO_DOOR] = p.doorpos[i] & (1 << j);
															p.ese_outpar [k][m][IO_DOOR] = p.doorpos[i] & (1 << j);
														}
												}
											if((p.ese_inpar [k][m][IO_BASIC_FUNC] == LIGHT_FUNC) &&
													(p.ese_inpar[k][m][IO_SUB_FUNC] & (HALL_LANTERN_DN | HALL_LANTERN_UP)))
												{
													p.ese_inpar [k][m][IO_FLOOR] = i + 1; 														
													p.ese_outpar [k][m][IO_FLOOR] = i + 1;			
												}											
										}
								}
							k++;
						}
				}
		}

//���ý���
	for(j = 0; j < p.tse_number; j++)
		{
			k = 0;
			for(m = 0; m < MAX_TSE_IN; m++)
				{
					if(p.tse_inpar [j][m][IO_BASIC_FUNC] == CAR_CALL)
						{
							if(k <= p.top_floor)
								{
									if(p.liop_cop_door & CONFIG_CAR_DOOR)
										{
											p.tse_inpar[j][m][IO_DOOR] = p.doorpos[k];
											p.tse_outpar[j][m][IO_DOOR] = p.doorpos[k];
										}
									else
										{												
											p.tse_inpar [j][m][IO_DOOR] = p.doorpos[k] & (1 << j);
											p.tse_outpar [j][m][IO_DOOR] = p.doorpos[k] & (1 << j);
										}
								}
							k++;
						}
					else if((p.tse_inpar [j][m][IO_BASIC_FUNC] == SPECIAL_FUNC) &&
									((p.tse_inpar [j][m][IO_SUB_FUNC] == DOOR_CLOSE) 	|| (p.tse_inpar [j][m][IO_SUB_FUNC] == DOOR_OPEN)))
						{//����������
							if(((p.open_close_door & CONFIG_CLOSE_DOOR) && (p.tse_inpar [j][m][IO_SUB_FUNC] == DOOR_CLOSE))
									|| ((p.open_close_door & CONFIG_OPEN_DOOR) && (p.tse_inpar [j][m][IO_SUB_FUNC] == DOOR_OPEN)))
								{
									p.tse_inpar[j][m][IO_DOOR] = HSE_MARIN[p.doornumber];
									p.tse_outpar[j][m][IO_DOOR] = HSE_MARIN[p.doornumber];
								}
							else
								{
									p.tse_inpar[j][m][IO_DOOR] = 1 << j;
									p.tse_outpar[j][m][IO_DOOR] = 1 << j;
								}
						}
				}
		}
}

void floor_name_change(WORD row){
	BYTE ten, unit, i, j;
	BYTE buf[2];
	
	ten = (BYTE)(p.sign[row] >> 8);
	unit = (BYTE)p.sign[row];

	if((ten == '-') && (unit >= '0') && (unit <= '9'))
		{//����Ϊ��¥��
			j = unit - '0';
			for(i = row; i < row + j; i++)
				{
					buf[0] = '-';
					buf[1] = j + row - i + '0';
					p.sign[i] = ((WORD)buf[0] << 8) | buf[1];
				}
			for(row = i; row < p.top_floor + 1; row++)
				{
					ten = row - i + 1;
					buf[0] = '0' + ten / 10;
					if(ten < 10)					
						buf[0] = 0;
					else
						buf[0] = '0' + ten / 10;
					buf[1] = '0' + (ten % 10);		
					p.sign[row] = ((WORD)buf[0] << 8) | buf[1];
				}
		}
	else if(((!ten ) || (ten == ' ') || ((ten > '0') && (ten <= '9'))) && 
					((unit >= '0') && (unit <= '9')))
		{//¥��˳����			
			if((!ten) || (ten == ' '))				
				j = unit - '0';
			else
				j = unit - '0' + (ten - '0') * 10;
			for(i = row; i < p.top_floor + 1; i++, j++)
				{
					if(j < 10)					
						buf[0] = 0;
					else
						buf[0] = '0' + j / 10;
					buf[1] = '0' + (j % 10);					
					p.sign[i] = ((WORD)buf[0] << 8) | buf[1];
				}
		}
	else
		{//����¥���򲻱��
		}

}

void config_botfloor_change(void){
	BYTE i, j, k;
	
	p.main_floor = p.bot_floor;
	p.parkfloor = p.main_floor;
	bot_floor_old = p.bot_floor;
	p.remote_off_floor = p.bot_floor;
	p.forced_stop_floor = p.main_floor;
	p.up_peak_parkfloor = p.main_floor;
	p.dn_peak_parkfloor = p.main_floor;
	p.timer1_parkfloor = p.parkfloor;
	p.timer2_parkfloor = p.parkfloor;

//TSE
	for (i = 0; i < MAX_TSE; i++)
		{
			for (j = 2; j <= 14; ++j)
				{
					p.tse_inpar[i][j][IO_BASIC_FUNC] = CAR_CALL;
					p.tse_inpar[i][j][IO_SUB_FUNC] = j - 2 + p.bot_floor + 1;
					p.tse_inpar[i][j][IO_DOOR] = p.doorpos [j - 2 + p.bot_floor];
					p.tse_outpar[i][j][IO_BASIC_FUNC] = CAR_CALL;
					p.tse_outpar[i][j][IO_SUB_FUNC] = j - 2 + p.bot_floor + 1;
					p.tse_outpar[i][j][IO_DOOR] = p.doorpos [j - 2 + p.bot_floor];
				}
			for (j = 0; j < p.tse_extnums; ++j)
				{
					for (k = 0; k < 16; ++k)
						{
							if ((j*16 + k + 13) <= (p.top_floor - p.bot_floor))
								{
									p.tse_inpar[i][22+j*16+k][IO_BASIC_FUNC] = CAR_CALL;
									p.tse_inpar[i][22+j*16+k][IO_SUB_FUNC] = 13 + j*16 + k + p.bot_floor + 1;
									p.tse_inpar[i][22+j*16+k][IO_DOOR] = p.doorpos [13 + j*16 + k + p.bot_floor];
									p.tse_outpar[i][22+j*16+k][IO_BASIC_FUNC] = CAR_CALL;
									p.tse_outpar[i][22+j*16+k][IO_SUB_FUNC] = 13 + j*16 + k + p.bot_floor + 1;
									p.tse_outpar[i][22+j*16+k][IO_DOOR] = p.doorpos [13 + j*16 + k + p.bot_floor];
								}
							else
								break;
						}
				}
		}

//ESE
	for (i = 0; i < p.ese_number; ++ i)
		{
			p.ese_inpar[i][0][IO_FLOOR] = i + p.bot_floor + 1;
			p.ese_inpar[i][1][IO_FLOOR] = i + p.bot_floor + 1;

			p.ese_outpar[i][0][IO_FLOOR] = i + p.bot_floor + 1;
			p.ese_outpar[i][1][IO_FLOOR] = i + p.bot_floor + 1;
			p.ese_outpar[i][2][IO_FLOOR] = i + p.bot_floor + 1;
			p.ese_outpar[i][3][IO_FLOOR] = i + p.bot_floor + 1;
			
			p.ese_inpar[i][0][IO_DOOR] = p.doorpos [i + p.bot_floor];
			p.ese_inpar[i][1][IO_DOOR] = p.doorpos [i + p.bot_floor];

			p.ese_outpar[i][0][IO_DOOR] = p.doorpos [i + p.bot_floor];
			p.ese_outpar[i][1][IO_DOOR] = p.doorpos [i + p.bot_floor];
			p.ese_outpar[i][2][IO_DOOR] = p.doorpos [i + p.bot_floor];
			p.ese_outpar[i][3][IO_DOOR] = p.doorpos [i + p.bot_floor];
			
			p.ese_inpar[i][0][IO_STATE] = POLARITY_BIT;
			p.ese_inpar[i][1][IO_STATE] = POLARITY_BIT;

			if (i == p.bot_floor)
				{
					p.ese_inpar[i][2][IO_BASIC_FUNC] = SPECIAL_FUNC;
					p.ese_inpar[i][2][IO_SUB_FUNC] = FIRE_SERVICE;
					p.ese_inpar[i][2][IO_FLOOR] = p.main_floor + 1;
					p.ese_inpar[i][2][IO_DOOR] = p.doorpos[p.main_floor];
					p.ese_inpar[i][2][IO_STATE] = POLARITY_BIT;

					p.ese_inpar[i][3][IO_BASIC_FUNC] = SPECIAL_FUNC;
					p.ese_inpar[i][3][IO_SUB_FUNC] = REMOTE_OFF;//KEY_OFF;
					p.ese_inpar[i][3][IO_STATE] = POLARITY_BIT;
				}
		}
}

