
#include "typedef.h"
#include "string.h"
#include "parameter.h"
#include "nice5000.h"
#include "state.h"
#include "test.h"
#include "text.h"
#include "lcd.h"
#include "history.h"
#include "eeprom.h"
#include "version.h"
#include "io.h"
#include "Can.h"

extern const WORD * c_level_pos[];

/****************************************************************************************************/
/* Check for valid IO function																		*/
/****************************************************************************************************/
BYTE io_para_valid (BYTE io_func, BYTE dir)
{
	WORD number = 1;
	
	while (iotype [number].number != ENDSTRING)
	{
		if ((dir & iotype [number].io) &&			/* basic funtion is for input / output			*/
		(io_func == iotype [number].number))		/* valid basic function							*/
			return (1);
		number++;
	}
	return (0);
}

void AddChksum(void * pointer, WORD size, DWORD * sum)
{
	BYTE * ptr = (BYTE *)pointer;

	while(size--)
	{
		*sum += *ptr ++;
	}
}

//***************************************************
// ������: GetChecksum
// ����: ���������У��<�μ�eeprom.h>
// ���: ��
// ����: ��
// ����ֵ: У���
//***************************************************
DWORD GetChecksum(void * pointer, WORD size)
{
	DWORD addsum = 0;
	AddChksum(pointer, size, &addsum);
	return (addsum) ;
}

//***************************************************
// ������: VerifyChecksum
// ����: У�� EEPROM �������Ƿ���Ч
// ���: ��
// ����: ��
// ����ֵ: 0 ��Ч;1 ��Ч
//***************************************************
BYTE VerifyChecksum (void)
{
	DWORD readsum;
	BYTE ret;

	DisableIrq();
	ret = ReadEEData(&p, EE_PARAMETER, sizeof(p));
	ret |= ReadEEData(&password_admin, EE_PASSWORD0, sizeof(password_admin));
	ret |= ReadEEData(&password_help, EE_PASSWORD1, sizeof(password_help));
	ret |= ReadEEData(&password_install, EE_PASSWORD2, sizeof(password_install));
	ret |= ReadEEData(&password_maintain, EE_PASSWORD3, sizeof(password_maintain));
	ret |= ReadEEData(&tripcount, EE_TRIPCOUNT, sizeof(tripcount));
	ret |= ReadEEData(&tripcount_limit, EE_TRIPCOUNT_LIMIT, sizeof(tripcount_limit));
	ret |= ReadEEData(&op_time, EE_OPERTIME, sizeof(op_time));
	ret |= ReadEEData(&language, EE_LANGUAGE, sizeof(language));
	ret |= ReadEEData(&exe_para, EE_EXE_PARA, sizeof(exe_para));
	ret |= ReadEEData (floorcount, EE_FLOORCOUNT, sizeof (floorcount));
	ret |= ReadEEData(&readsum, EE_CHECKSUM, sizeof(readsum));
	EnableIrq();
	if (readsum == GetChecksum(&p, sizeof(p)))
	{
		relev_active = p.relevel;
		para_checksum = readsum;
		return 0x00;
	}

//У��ͳ���
	ClrWdt();
	memset(&p, 0x00, sizeof(p));
	memset(&errorhist, 0x00, sizeof(errorhist));
	errorhist.check = ERRORCOUNTCHECK;
	memset(&errorcounter, 0x00, sizeof(errorcounter));
	errorcounter.check = ERRORCOUNTCHECK;
	memset(floorcount, 0x00, sizeof(floorcount));

	password_admin = 0;
	password_help = 0;
	password_install = 0;
	password_maintain = 0;
	tripcount = 0;
	op_time = 0;
	language = ENGLISH;

	ClrWdt();
	p.bot_floor = 0;
	p.top_floor = 7;
	p.doornumber = 1;
	p.tse_number = 1;
	p.ese_number = 8;
	p.groupsize = 1;
	p.groupnumber = 0;
	p.fx_active = 0;
	set_parameter();
	ClrWdt();

	return 0x01;
}

void set_floorhigth(void){
	BYTE i;
	
	if(p.simulation == SIMULATION_AUTO)
		{
			floor_higth[0] = 0;
			floor_higth[1] = 950;
			for(i = 2; i < MAX_FLOOR - 1; i++)
				floor_higth[i] = floor_higth[i - 1] + 350;		//�趨ÿ��3.5m
			floor_higth[MAX_FLOOR - 1] = floor_higth[MAX_FLOOR - 2] + 850;
		}
	else if(p.simulation == SIMULATION_DRIVE)
		{
			floor_higth[0] = 0;
			floor_higth[1] = 1000;
			for(i = 2; i < MAX_FLOOR - 1; i++)
				floor_higth[i] = floor_higth[i - 1] + 400;		//�趨ÿ��3.5m
			floor_higth[MAX_FLOOR - 1] = floor_higth[MAX_FLOOR - 2] + 1000;
		}
}

void LoadDefaultPara (void){
	BYTE i, j, k;

// P0 General parameter
	p.bot_floor = 0;
	p.top_floor = MAX_FLOOR - 1;
	p.groupsize = 1;
	p.groupnumber = 0x00;
	p.doornumber = 1;
	p.ese_number = MAX_FLOOR;
	p.fx_active = 0;
	p.main_floor = p.bot_floor;
	p.parkfloor = p.bot_floor;
	p.controltype = COLLECTING;
	p.setup_ready = 1;
	p.motor_tuned = 1;
	p.lcmode_actfloor = LCM_REOPEN;
	if (p.top_floor - p.bot_floor < 14)
		p.tse_extnums = 0;
	else if ((p.top_floor - p.bot_floor) < (14 + 16))
		p.tse_extnums = 1;
	else if ((p.top_floor - p.bot_floor) < (14 + 32))
		p.tse_extnums = 2;
	else
		p.tse_extnums = 3;
	p.loadtype = ANALOG_ON_CDU;
	p.arrowtype = ARROW_SCROLL;
	p.doorzoneswitchsize = 225; 		// Ĭ�Ͽ��峤��268mm(ʵ��Լ300mm)
	p.exe_number = 0;				//Ĭ���������1����չ�壬���ڿ��ƹ�
	
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
	p.rsttrip_stop_dly = 1000;
	p.fan_time = 60;
	p.upstrip_stop_delay = 500;

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

//P8 Floor name
	for (i = p.bot_floor; i <= p.top_floor; i++)
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

// PA Internal param. 
	language = ENGLISH; 			//ʹ�����ֲ���
	p.lift_func_sel1 = 0;
	p.evac_door = 1;			//Ĭ�Ϸ����������Ͽ���
	p.landingcall_push = 0;
	p.ups_direction_mode =  UPS_DIR_SEL; 	//Ĭ��ARD��Ԯʱ�������巽��
	p.canbus_check = 1; 	//Ĭ������ʱ����� canbus ����
	p.emp_power_door = 3; //Ĭ��Ӧ����Դ����ʱĬ�Ͽ���2����
	p.custumer1 = 0;
	
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
			p.ramptiming [i] = 0; // ??
			p.rampdelay [i] = 0;	// ??
			p.dooropendelay [i] = 0;
			p.rev_door [i] = 0;
		}

// PD2~PD15
	for (i = 0; i < MAX_FLOOR; i++)
	{
		p.doorpos [i] = 0x01; 							// ����Ĭ�϶���ֻ��1����
		for (j = 0; j < MAX_DOORTABLE; j++) 			// PD4~PD15 Door release
			p.doortable [j][i] = HSE_MARIN[p.doornumber];
		p.parkdoor [i] = p.doorpos [i]; 				// PD3- Door park mode
		p.fire_floor [i] = i; 			// PR4- Fire evac.floors
	}

//PD16
	p.liop_cop_door = CONFIG_CAR_DOOR;//CONFIG_SHAFT_DOOR | CONFIG_CAR_DOOR;
	p.open_close_door = CONFIG_OPEN_DOOR | CONFIG_CLOSE_DOOR;

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
	p.evacuationtime = 0; 	//Ĭ��ֵ��Ϊ0s���������źŴ�����һ��
	p.evac_autoreturn = 1;
	p.evac_fire_carlightmode = 3; 	//Ĭ�Ϲص�
	p.carlight_fan_time_fire = 5; 	//Ĭ�Ϲص�ʱ��Ϊ5s
	
// PR6 firemen run adj.
	p.fire_doorclosemode = 3;
	p.fire_dooropenmode = 3;
	p.firealarm_function = 0x39;//FIREALARM_FORCE_CLOSE | FIREALARM_ACTIVE_DETECT | FIREALARM_RESET_MODE;
	p.fireman_function1 = 0x0F;//FIREMAN1_DOOR_MAINFLOOR | FIREMAN1_REPEAT_FIRECALL | FIREMAN1_AUTO_FIREMODE;
	
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
	
// PR9 clock run
	p.timer_clearcall = 0;
	p.timertrip_start = 0;
	p.timertrip_stop = 0;
	p.timer1_parkfloor = p.parkfloor;
	p.timer2_parkfloor = p.parkfloor;
	
// PR10 control calls
	p.call_disable_enable = 28;//CALL_DISEN_AUTO_CC | CALL_DISEN_AUTO_LC | CALL_DISEN_PRIORITY;
	
// PR11 car attendance
	p.attend_carcall = 0;
	p.attend_normal_timer = 10;
	
// PR12 auto-call func.
	p.autocall_dir = HALL_CALL_UP | HALL_CALL_DOWN;
	p.cooperate_sel1 = 16;//COOP1_INSP_RETURN
	p.cooperate_sel2 = 0;
	p.arrivegong_starttime = 22;		//ҹ����22:00--8:00ȡ����վ��
	p.arrivegong_stoptime = 8;

// PR13 car fam
	p.fan_mode = FAN_AUTOMATIC;
	p.fan_time = 60;
	
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
	p.up_peak_stop_h1 = 0;
	p.dn_peak_start_h1 = 0;
	p.dn_peak_stop_h1 = 0;
	p.up_peak_parkfloor = p.main_floor;
	p.dn_peak_parkfloor = p.main_floor;

//PR17
	p.simu_mult = 100;

// P3-00 MCU in-/outputs
	for (i = 0; i < MAX_HSE_IN; i++)
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
	p.hse_inpar [INPUT_SDD1][IO_BASIC_FUNC] = FORCE_SWITCH;
	p.hse_inpar [INPUT_SDD1][IO_SUB_FUNC] = FORCE_SDD1;
	p.hse_inpar [INPUT_SDD1][IO_STATE] = POLARITY_BIT;

	p.hse_inpar [INPUT_SDU2][IO_BASIC_FUNC] = FORCE_SWITCH;
	p.hse_inpar [INPUT_SDU2][IO_SUB_FUNC] = FORCE_SDU2;
	p.hse_inpar [INPUT_SDU2][IO_STATE] = POLARITY_BIT;
	p.hse_inpar [INPUT_SDU2][IO_ENABLE] = DISABLE;
	p.hse_inpar [INPUT_SDD2][IO_BASIC_FUNC] = FORCE_SWITCH;
	p.hse_inpar [INPUT_SDD2][IO_SUB_FUNC] = FORCE_SDD2;
	p.hse_inpar [INPUT_SDD2][IO_STATE] = POLARITY_BIT;
	p.hse_inpar [INPUT_SDD2][IO_ENABLE] = DISABLE;

	p.hse_inpar [9][IO_BASIC_FUNC] = TEMPERATURE;
	p.hse_inpar [9][IO_SUB_FUNC] = OVERTEMP1;
	if(p.simulation == SIMULATION_AUTO)
		p.hse_inpar [9][IO_STATE] = POLARITY_BIT;

	p.hse_inpar [INPUT_SDU3][IO_BASIC_FUNC] = FORCE_SWITCH;
	p.hse_inpar [INPUT_SDU3][IO_SUB_FUNC] = FORCE_SDU3;
	p.hse_inpar [INPUT_SDU3][IO_STATE] = POLARITY_BIT;
	p.hse_inpar [INPUT_SDU3][IO_ENABLE] = DISABLE;
	p.hse_inpar [INPUT_SDD3][IO_BASIC_FUNC] = FORCE_SWITCH;
	p.hse_inpar [INPUT_SDD3][IO_SUB_FUNC] = FORCE_SDD3;
	p.hse_inpar [INPUT_SDD3][IO_STATE] = POLARITY_BIT;
	p.hse_inpar [INPUT_SDD3][IO_ENABLE] = DISABLE;
	
	p.hse_inpar [INPUT_ILO][IO_BASIC_FUNC] = POS_IN;
	p.hse_inpar [INPUT_ILO][IO_SUB_FUNC] = POS_ILU;
	p.hse_inpar [INPUT_ILO][IO_STATE] = POLARITY_BIT;
	p.hse_inpar [INPUT_ILO][IO_ENABLE] = DISABLE;

	p.hse_inpar [13][IO_BASIC_FUNC] = CONT_CHECK;
	p.hse_inpar [13][IO_SUB_FUNC] = D_FXCONT_SUP;
	p.hse_inpar [13][IO_STATE] = POLARITY_BIT;
	p.hse_inpar [13][IO_ENABLE] = DISABLE;

	p.hse_inpar [14][IO_BASIC_FUNC] = SPECIAL_FUNC;		//SPECIAL_FUNC;
	p.hse_inpar [14][IO_SUB_FUNC] = UPS_EVACUATION;

	p.hse_inpar [15][IO_BASIC_FUNC] = CONT_CHECK;
	p.hse_inpar [15][IO_SUB_FUNC] = D_RUNCONT_SUP;
	p.hse_inpar [15][IO_STATE] = POLARITY_BIT;
	p.hse_inpar [16][IO_BASIC_FUNC] = CONT_CHECK;
	p.hse_inpar [16][IO_SUB_FUNC] = D_BRKCONT_SUP;
	p.hse_inpar [16][IO_STATE] = POLARITY_BIT;	
	p.hse_inpar [17][IO_BASIC_FUNC] = BRAKE_CHECK;
	p.hse_inpar [17][IO_SUB_FUNC] = BRAKE1;
	if(p.simulation == SIMULATION_AUTO)
		{
			p.hse_inpar [15][IO_ENABLE] = DISABLE;
			p.hse_inpar [16][IO_ENABLE] = DISABLE;
			p.hse_inpar [17][IO_ENABLE] = DISABLE;
			contactor_check = 0;
			brake_check = 0;
		}

	p.hse_inpar [IO_SC1][IO_BASIC_FUNC] = SAFETY_CIRCUIT;
	p.hse_inpar [IO_SC1][IO_SUB_FUNC] = SC1;
	p.hse_inpar [IO_SC2][IO_BASIC_FUNC] = SAFETY_CIRCUIT;
	p.hse_inpar [IO_SC2][IO_SUB_FUNC] = SC2;
	p.hse_inpar [IO_SC3][IO_BASIC_FUNC] = SAFETY_CIRCUIT;
	p.hse_inpar [IO_SC3][IO_SUB_FUNC] = SC3;
	if(p.simulation == SIMULATION_AUTO)
		{
			p.hse_inpar [IO_SC2][IO_STATE] = POLARITY_BIT;
			p.hse_inpar [IO_SC1][IO_STATE] = POLARITY_BIT;
			p.hse_inpar [IO_SC3][IO_STATE] = POLARITY_BIT;
		}

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
	if(p.simulation == SIMULATION_DRIVE)
		{//���� SDD1, SDU1 ���ܵ����	
			p.hse_outpar [1][IO_BASIC_FUNC] = DRIVE_OUT;
			p.hse_outpar [1][IO_SUB_FUNC] = D_BRAKE;
			
			p.hse_ex_outpar [0][IO_BASIC_FUNC] = POS_IN;
			p.hse_ex_outpar [0][IO_SUB_FUNC] = POS_UPLS;
			p.hse_ex_outpar [0][IO_STATE] = POLARITY_BIT;
			
			p.hse_ex_outpar [1][IO_BASIC_FUNC] = POS_IN;
			p.hse_ex_outpar [1][IO_SUB_FUNC] = POS_LPLS;
			p.hse_ex_outpar [0][IO_STATE] = POLARITY_BIT;
		}
	else if(p.simulation == SIMULATION_AUTO)
		{//���� SDD1, SDU1 ���ܵ����		
			p.hse_outpar [1][IO_BASIC_FUNC] = POS_IN;
			p.hse_outpar [1][IO_SUB_FUNC] = POS_UPLS;
			p.hse_outpar [2][IO_BASIC_FUNC] = POS_IN;
			p.hse_outpar [2][IO_SUB_FUNC] = POS_LPLS;
		}
//���� MSI ���ܵ����	
	p.hse_outpar [3][IO_BASIC_FUNC] = POS_IN;
	p.hse_outpar [3][IO_SUB_FUNC] = POS_SGM;

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

// P3-01 CDU in-/outputs
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

	p.fve_inpar [7][IO_BASIC_FUNC] = POS_IN;
	p.fve_inpar [7][IO_SUB_FUNC] = POS_SGD;
	p.fve_inpar [7][IO_ENABLE] = DISABLE;
	
	p.fve_inpar [8][IO_BASIC_FUNC] = POS_IN;
	p.fve_inpar [8][IO_SUB_FUNC] = POS_SGU;
	p.fve_inpar [8][IO_ENABLE] = DISABLE;

	p.fve_inpar[10][IO_BASIC_FUNC] = DOOR_IO;
	p.fve_inpar[10][IO_SUB_FUNC] = DOOR_OP;
	p.fve_inpar[10][IO_DOOR] = DOOR1;
	p.fve_inpar[10][IO_STATE] = POLARITY_BIT;
	
	p.fve_inpar[11][IO_BASIC_FUNC] = DOOR_IO;
	p.fve_inpar[11][IO_SUB_FUNC] = DOOR_CL;
	p.fve_inpar[11][IO_DOOR] = DOOR1;
	p.fve_inpar[11][IO_STATE] = POLARITY_BIT;

	p.fve_inpar[12][IO_BASIC_FUNC] = DOOR_IO;			//��2�Լ����й�Ļ�źŽ�ֹ
	p.fve_inpar[12][IO_SUB_FUNC] = DOOR_OP;
	p.fve_inpar[12][IO_DOOR] = DOOR2;
	p.fve_inpar[12][IO_STATE] = POLARITY_BIT;
	p.fve_inpar[12][IO_ENABLE] = DISABLE;
	
	p.fve_inpar[13][IO_BASIC_FUNC] = DOOR_IO;
	p.fve_inpar[13][IO_SUB_FUNC] = DOOR_CL;
	p.fve_inpar[13][IO_DOOR] = DOOR2;
	p.fve_inpar[13][IO_STATE] = POLARITY_BIT;
	p.fve_inpar[13][IO_ENABLE] = DISABLE;
	
	p.fve_inpar[14][IO_BASIC_FUNC] = DOOR_LS;
	p.fve_inpar[14][IO_DOOR] = DOOR1;
	p.fve_inpar[14][IO_STATE] = POLARITY_BIT;
	
	p.fve_inpar[15][IO_BASIC_FUNC] = DOOR_LS;
	p.fve_inpar[15][IO_DOOR] = DOOR2;
	p.fve_inpar[15][IO_STATE] = POLARITY_BIT;
	p.fve_inpar[15][IO_ENABLE] = DISABLE;

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

	p.fve_outpar [1][IO_BASIC_FUNC] = ARRIVAL_INDICATION;
	p.fve_outpar [1][IO_SUB_FUNC] = DIR_UP;
	p.fve_outpar [1][IO_DOOR] = DOOR1;
	p.fve_outpar [2][IO_BASIC_FUNC] = ARRIVAL_INDICATION;
	p.fve_outpar [2][IO_SUB_FUNC] = DIR_DN;
	p.fve_outpar [2][IO_DOOR] = DOOR1;
	p.fve_outpar [3][IO_BASIC_FUNC] = DOOR_IO;
	p.fve_outpar [3][IO_SUB_FUNC] = DOOR_CL;
	p.fve_outpar [3][IO_DOOR] = DOOR1;
	p.fve_outpar [4][IO_BASIC_FUNC] = DOOR_IO;
	p.fve_outpar [4][IO_SUB_FUNC] = DOOR_OP;
	p.fve_outpar [4][IO_DOOR] = DOOR1;
	
	p.fve_outpar [5][IO_BASIC_FUNC] = DOOR_IO;	//��2�����������ֹ
	p.fve_outpar [5][IO_SUB_FUNC] = DOOR_CL;
	p.fve_outpar [5][IO_DOOR] = DOOR2;
	p.fve_outpar [5][IO_ENABLE] = DISABLE;
	p.fve_outpar [6][IO_BASIC_FUNC] = DOOR_IO;
	p.fve_outpar [6][IO_SUB_FUNC] = DOOR_OP;
	p.fve_outpar [6][IO_DOOR] = DOOR2;
	p.fve_outpar [6][IO_ENABLE] = DISABLE;
	
	p.fve_outpar [7][IO_BASIC_FUNC] = CARLIGHT;
	p.fve_outpar [7][IO_STATE] = POLARITY_BIT;
	p.fve_outpar [8][IO_BASIC_FUNC] = SPECIAL_FUNC;
	p.fve_outpar [8][IO_SUB_FUNC] = FAN_1;
	p.fve_outpar [8][IO_STATE] = POLARITY_BIT;
	p.fve_outpar [9][IO_BASIC_FUNC]	= EMERGENCY_LIGHT;
	p.fve_outpar [9][IO_STATE] = POLARITY_BIT;

// P3-02 PCU in-/outputs
	ClrWdt();
	p.tse_number = 3;
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
	
	for (i = 0; i < p.tse_number; i++)
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
			p.tse_inpar[i][20][IO_BASIC_FUNC] = SPECIAL_FUNC;
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
	p.tse_number = 1;

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

		p.ese_inpar[i][0][IO_SUB_FUNC] = HALL_CALL_UP;
		p.ese_inpar[i][1][IO_SUB_FUNC] = HALL_CALL_DOWN;

		p.ese_outpar[i][0][IO_SUB_FUNC] = HALL_CALL_UP;
		p.ese_outpar[i][1][IO_SUB_FUNC] = HALL_CALL_DOWN;

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

		if (!i)
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

	exe_enabled = HSE_MARIN[p.exe_number];


	if(p.simulation == SIMULATION_AUTO)
		{
			set_floorhigth();
			iv_para_valid = 1;
			hb_error &= ~B_DRV;
			init_error &= ~B_DRV;
		}
	else if((p.simulation == SIMULATION_DRIVE) && (!p.setup_ready))
		set_floorhigth();
	password_admin = 0;
	password_help = 0;
	password_install = 0;
	password_maintain = 0;
}

//���п���,
void handle_drive(void){
	static BYTE wait = 0;
	static DWORD wt = 0;
	static DWORD dt = 0;
	BYTE nextfloor = level;
	
	switch(nice5000_running_step)
		{
			case RCMD2_STEP_IDLE:		//����״̬
				if(nice5000_Tcmd_data1 & NICE5000_TCMD1_RUN)
					{//���յ����������ָ��
						if(!wait)
							{
								wait = 1;
								dt = timer + 5;
								wt = timer + 5 SEC;		///5s�ȴ��߼���׼��ʱ��
							}
						else if((wait == 1) && (dt < timer))
							{//������������				
								if((nice5000_Tcmd_data4 & 0xF8) == 0xF8) 	//�߼����Ѿ�׼����
									{
										if(nice5000_Tcmd_data1 & (NICE5000_TCMD1_DOWN | NICE5000_TCMD1_UP))
											{
												nice5000_Rcmd_data1 |= NICE5000_RCMD1_RUN;
												dt = timer + 5;
												wt = timer + 5 SEC; 	///5s�ȴ��߼���׼��ʱ��
												wait = 2; 
											}
										else
											wait = 0;
									}
								else if(wt < timer)
									{//�ȴ�5s�߼��廹δ׼����
										wait = 0;								
									}
							}
						else if((wait == 2) && (dt < timer))
							{//���Ϳ�բ����
								if(nice5000_Rcmd_data1 & NICE5000_RCMD1_RUN)
									{
										nice5000_Rcmd_data1 |= NICE5000_RCMD1_BRAKE;
										wait = 0;
										nice5000_running_step = RCMD2_STEP_STARTING;
										dt = timer + 5;		//1500ms����ʱ��
										wt = timer + 5 SEC; 	///5s�ȴ��߼���׼��ʱ��
									}
								else if(wt < timer)
									wait = 0;
							}
					}
				else
					{//δ�յ���������,������ͣ���ڵ�ǰ¥��						
					}						
				break;

			case RCMD2_STEP_STARTING:		//���ڼ���
			case RCMD2_STEP_ACC_STARTING	:
			case RCMD2_STEP_ACC_DOING:				
			case RCMD2_STEP_ACC_ENDING:				
				if((nice5000_Rcmd_data1 & (NICE5000_RCMD1_RUN | NICE5000_RCMD1_BRAKE)
							== (NICE5000_RCMD1_RUN | NICE5000_RCMD1_BRAKE)))
					{
						if((dt < timer) && !wait)					
							nice5000_running_step = RCMD2_STEP_EVEN_SPEED;
					}
				else					
					{
						nice5000_running_step = RCMD2_STEP_IDLE;						
						nice5000_Rcmd_data1 &= ~(NICE5000_RCMD1_RUN | NICE5000_RCMD1_BRAKE);		//�رձ�բ
						wait = 0;
					}
				break;

			case RCMD2_STEP_EVEN_SPEED:			//��������				
				if((nice5000_Rcmd_data1 & (NICE5000_RCMD1_RUN | NICE5000_RCMD1_BRAKE))
						== (NICE5000_RCMD1_RUN | NICE5000_RCMD1_BRAKE))
					{
						if(level == target_floor)
							{
								if(!wait)
									{
										wait = 1;
										dt = timer + 5;			//����Ŀ��¥��ȴ� 2s �����
									}
								if((wait == 1) && (dt < timer))
									{
										wait = 0;
										nice5000_running_step = RCMD2_STEP_DEC_STARTING;
										dt = timer + 5;		//��ʼ����,���� arrive ״̬,Ȼ��ȴ� 1.5s����0�ٱ�־
									}								
							}	
					}
				else
					{
						nice5000_running_step = RCMD2_STEP_IDLE;						
						nice5000_Rcmd_data1 &= ~(NICE5000_RCMD1_RUN | NICE5000_RCMD1_BRAKE);		//�رձ�բ
						wait = 0;
					}
				break;

			case RCMD2_STEP_DEC_STARTING:				//����		
			case RCMD2_STEP_DEC_DOING:
			case RCMD2_STEP_DEC_ENDING:
				if((nice5000_Rcmd_data1 & (NICE5000_RCMD1_RUN | NICE5000_RCMD1_BRAKE))
						== (NICE5000_RCMD1_RUN | NICE5000_RCMD1_BRAKE))
					{
						if(!wait) 			
							{
								if((door_zone & POS_SGM) &&
										((level == target_floor) || (callpriority == C_FIREALARM) || (callpriority == C_FIREMAN)))
									{
										nice5000_Rcmd_data1 |= NICE5000_RCMD1_ZEROSPD;		//�������ٱ�־
										nice5000_running_step = RCMD2_STEP_0SPD_STOPPING;						
										wait = 0;
										dt = timer + 5;
										wt = timer + 5 SEC;
									}	
							}						
					}
				else
					{
						nice5000_running_step = RCMD2_STEP_IDLE;						
						nice5000_Rcmd_data1 &= ~(NICE5000_RCMD1_RUN | NICE5000_RCMD1_BRAKE);		//�رձ�բ
						wait = 0;
					}
				break;
				
			case RCMD2_STEP_0SPD_STOPPING:				
				if(nice5000_Rcmd_data1 & NICE5000_RCMD1_ZEROSPD)
					{
						if((!wait) && (dt < timer))
							{
								nice5000_Rcmd_data1 &= ~NICE5000_RCMD1_BRAKE;		//�رձ�բ
								wait = 1;
								dt = timer + 5;
							}
						else if((wait == 1) && (dt < timer))
							{
								wait = 2;
								nice5000_Rcmd_data1 &= ~NICE5000_RCMD1_RUN;
							}
						else if(wait == 2)
							{
								if(!(nice5000_Tcmd_data1 & (NICE5000_TCMD1_RUN | NICE5000_TCMD1_DOWN
											| NICE5000_TCMD1_UP | NICE5000_TCMD1_BRAKE)))
									{
										nice5000_running_step = RCMD2_STEP_IDLE; 		//����ͣ��
										nice5000_Rcmd_data1 &= ~(NICE5000_RCMD1_RUN | NICE5000_RCMD1_BRAKE
																			| NICE5000_RCMD1_DOWN | NICE5000_RCMD1_UP | NICE5000_RCMD1_ZEROSPD); 	//�������ٱ�־
										wait = 0;
									}
							}
					}
				else if(wt < timer)
					{
						nice5000_running_step = RCMD2_STEP_IDLE;						
						nice5000_Rcmd_data1 &= ~(NICE5000_RCMD1_RUN | NICE5000_RCMD1_BRAKE);		//�رձ�բ
						wait = 0;
					}
				break;
		}	
	if(direction)
		nextfloor = level + 1;
	
	nice5000_Rcmd_data2 = (nice5000_Rcmd_data2 & (~NICE5000_RCMD2_RUNNING_STEP)) | nice5000_running_step;
	nice5000_Rcmd_data3 = ((WORD)nextfloor << 8) | (level + 1);

}

void pos_control(void){
	static BYTE msi_output = 0;
	static BYTE sdd1_output = 0;
	static BYTE sdu1_output = 0;
	static BYTE floor = 0;
	BYTE i = 0;
	WORD * ptr = NULL;

	if(p.simulation == SIMULATION_AUTO)
		{//������
			switch(hse_state)
				{
					case H_START:
						nice5000_Rcmd_data2 = (nice5000_Rcmd_data2 & (~NICE5000_RCMD2_STATE)) | RCMD2_STATE_OTHER;
						break;
						
					case H_DIVING:
						nice5000_Rcmd_data2 = (nice5000_Rcmd_data2 & (~NICE5000_RCMD2_STATE)) | RCMD2_STATE_DIVING;
						break;

					default: break;
				}

			if (drive_state == D_READY_TO_SWITCH_ON ||
					drive_state == D_OPERATION_ENABLED ||
					drive_state == D_SLOW_DOWN)
				{
					switch (nice5000_running_step)
						{
							case RCMD2_STEP_STARTING			:
							case RCMD2_STEP_ACC_STARTING:
							case RCMD2_STEP_ACC_DOING		:
							case RCMD2_STEP_ACC_ENDING		:			
							case RCMD2_STEP_DEC_STARTING	:
							case RCMD2_STEP_DEC_DOING		:
							case RCMD2_STEP_DEC_ENDING		:						
								simu_step = 2;
								break;
								
							case RCMD2_STEP_EVEN_SPEED:
								simu_step = 10;
								break;

							default:
								simu_step = 0;
								break;
						}
					if (simu_step)
						{
							if (direction == DIR_UP)
								pos += simu_step;
							else if (direction == DIR_DN)
								pos -= simu_step;
						}
				}	
			else if(drive_state == D_STOPPED)
				{
					pos = INIT_POSITION + floor_higth[level];
				}
			if((p.bot_floor <= level) && (level <= p.top_floor))
				{//����MSI 
					if(!msi_output)
						{
							if((pos > floor_higth[level] - 30 + INIT_POSITION) && (pos < floor_higth[level] + 30 + INIT_POSITION))
								{//������
									set_out (POS_IN, POS_SGM, 0, 0, 1, O_HSE);
									msi_output = 1;
								}
						}
					else if((pos > floor_higth[level] + 50 + INIT_POSITION) || 
									(pos < floor_higth[level] - 50 + INIT_POSITION))
						{					
							msi_output = 0;
							set_out (POS_IN, POS_SGM, 0, 0, 0, O_HSE);
						}
				}
			if(!sdd1_output)
				{//SDD1����
					if(pos < floor_higth[p.bot_floor] + 150 + INIT_POSITION)
						{
							sdd1_output = 1;			
							set_out (POS_IN, POS_LPLS, 0, 0, 0, O_HSE);
						}
				}
			else if(pos > floor_higth[p.bot_floor] + 180 + INIT_POSITION)
				{
					sdd1_output = 0;
					set_out (POS_IN, POS_LPLS, 0, 0, 1, O_HSE);
				}
			if(!sdu1_output)
				{//SDU1����
					if(pos > floor_higth[p.top_floor] - 150 + INIT_POSITION)
						{
							sdu1_output = 1;			
							set_out (POS_IN, POS_UPLS, 0, 0, 0, O_HSE);
						}
				}
			else if(pos < floor_higth[p.top_floor] - 180 + INIT_POSITION)
				{
					sdu1_output = 0;
					set_out (POS_IN, POS_UPLS, 0, 0, 1, O_HSE);
				}
			if((!msi_output) && (level <= p.top_floor))
				{
					if(direction == DIR_UP)
						{
							if ((level < p.top_floor) &&		// Lift not in highest floor	
									(pos > (floor_higth [level + 1] - ((floor_higth [level + 1] - floor_higth [level]) / 3)) + INIT_POSITION))
								level++;
						}
					else if(direction == DIR_DN)
						{
							if(level > p.bot_floor)
								{// Lift not in lowest floor 
									if(pos < (floor_higth [level - 1] + ((floor_higth [level] - floor_higth [level - 1]) / 3)) + INIT_POSITION)
										level--;
								}
							else
								{//in lowest floor 
									
								}
						}
				}
		}
	else if(p.simulation == SIMULATION_DRIVE)
		{//ģ�⾮���ź�
			if ((!p.setup_ready) || (!p.motor_tuned) || (nice5000_Rcmd_data2 & NICE5000_RCMD2_SETUP_READY))
				{//δѧϰ����								
					pos = (((DWORD)nice5000_Rstore_pulseH) << 16) + nice5000_Rstore_pulseL;
					if(floor)
						{//����MSI
							if(!msi_output)
								{
									if((pos > (DWORD)(floor_higth[floor] - 30) * p.simu_mult + INIT_POSITION) &&
											(pos < ((DWORD)(floor_higth[floor] + 30) * p.simu_mult + INIT_POSITION)))
										{//������
											set_out (POS_IN, POS_SGM, 0, 0, 1, O_HSE);
											msi_output = 1;
										}
								}
							else if((pos > ((DWORD)(floor_higth[floor] + 50) * p.simu_mult + INIT_POSITION)) ||
											(pos < ((DWORD)(floor_higth[floor] - 50) * p.simu_mult + INIT_POSITION)))
								{
									msi_output = 0;
									set_out (POS_IN, POS_SGM, 0, 0, 0, O_HSE);
								}
						}
					else
						{
							if(!msi_output)
								{
									if((pos > INIT_POSITION - 30 * p.simu_mult) && (pos < INIT_POSITION + 30 * p.simu_mult))
										{//������
											set_out (POS_IN, POS_SGM, 0, 0, 1, O_HSE);
											msi_output = 1;
										}
								}
							else if((pos > 50 * p.simu_mult + INIT_POSITION) ||	(pos < INIT_POSITION - 50 * p.simu_mult))
								{ 				
									msi_output = 0;
									set_out (POS_IN, POS_SGM, 0, 0, 0, O_HSE);
								}
						}
					if(!sdd1_output)
						{//SDD1����
							if(pos < ((DWORD)(floor_higth[0] + 150) * p.simu_mult + INIT_POSITION))
								{
									sdd1_output = 1;			
									set_out (POS_IN, POS_LPLS, 0, 0, 0, O_HSE);
								}
						}
					else if(pos > ((DWORD)(floor_higth[0] + 180) * p.simu_mult + INIT_POSITION))
						{
							sdd1_output = 0;
							set_out (POS_IN, POS_LPLS, 0, 0, 1, O_HSE);
						}
					if(!sdu1_output)
						{//SDU1����
							if(pos > ((DWORD)(floor_higth[p.top_floor] - 150) * p.simu_mult + INIT_POSITION))
								{
									sdu1_output = 1;			
									set_out (POS_IN, POS_UPLS, 0, 0, 0, O_HSE);
								}
						}
					else if(pos < ((DWORD)(floor_higth[p.top_floor] - 180) * p.simu_mult + INIT_POSITION))
						{
							sdu1_output = 0;
							set_out (POS_IN, POS_UPLS, 0, 0, 1, O_HSE);
						}
					if((!msi_output) && (floor <= p.top_floor))
						{
							if(direction == DIR_UP)
								{
									if ((floor < p.top_floor) &&		// Lift not in highest floor	
											(pos > (floor_higth [floor + 1] - ((floor_higth [floor + 1] - floor_higth [floor]) / 3)) * p.simu_mult + INIT_POSITION))
										floor++;
								}
							else if(direction == DIR_DN)
								{
									if(floor > p.bot_floor)
										{// Lift not in lowest floor 
											if(pos < (floor_higth [floor - 1] + ((floor_higth [floor] - floor_higth [floor - 1]) / 3)) * p.simu_mult + INIT_POSITION)
												floor--;
										}
									else
										{//in lowest floor 
											
										}
								}
						}
				}
			else
				{//�Ѿ�ѧϰ�˾���//MSI				
					pos = 100000ul;							
					if(level <= p.top_floor)
						{
							for (i = 0; i < level; ++i)
								{
									ptr = (WORD *)c_level_pos[i * 2];
									pos += *(ptr + 1);
									pos += ((DWORD)(*ptr) << 16);
									pos += tiv.F4_04;
								}							
							if(!msi_output)
								{
									if((dCurrentPulse > pos - tiv.F4_04/2 + 200) && (dCurrentPulse < pos + tiv.F4_04/2 - 200))
										{//������
											set_out (POS_IN, POS_SGM, 0, 0, 1, O_HSE);
											msi_output = 1;
										}
								}
							else if((dCurrentPulse > pos + tiv.F4_04 + 500) || (dCurrentPulse < pos - tiv.F4_04 - 500))
								{ 				
									msi_output = 0;
									set_out (POS_IN, POS_SGM, 0, 0, 0, O_HSE);
								}
						}		
//SDD1					
					pos = 100000ul;				
					if(!sdd1_output)
						{//SDD1����
							if(dCurrentPulse < pos + 150 * p.simu_mult)
								{
									sdd1_output = 1;			
									set_out (POS_IN, POS_LPLS, 0, 0, 0, O_HSE);
								}
						}
					else if(dCurrentPulse > pos + 200 * p.simu_mult)
						{
							sdd1_output = 0;
							set_out (POS_IN, POS_LPLS, 0, 0, 1, O_HSE);
						}
//SDU1					
					pos = 100000ul;
					for (i=0; i<=p.top_floor; ++i)
						{
							ptr = (WORD *)c_level_pos[i * 2];
							pos += *(ptr + 1);
							pos += ((DWORD)(*ptr) << 16);
							pos += tiv.F4_04;
						} 						
					if(!sdu1_output)
						{//SDU1����
							if(dCurrentPulse > pos - 150 * p.simu_mult)
								{
									sdu1_output = 1;			
									set_out (POS_IN, POS_UPLS, 0, 0, 0, O_HSE);
								}
						}
					else if(dCurrentPulse < pos - 200 * p.simu_mult)
						{
							sdu1_output = 0;
							set_out (POS_IN, POS_UPLS, 0, 0, 1, O_HSE);
						}
				}
		}
}

