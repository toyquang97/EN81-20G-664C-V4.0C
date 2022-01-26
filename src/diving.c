
#include "typedef.h"
#include "parameter.h"
#include "nice5000.h"
#include "state.h"
#include "can.h"
#include "call.h"
#include "pos.h"
#include "drive.h"
#include "io.h"
#include "history.h"

WORD* const c_level_pos[] =
//const WORD* c_level_pos[] =
{
	&tiv.F4_10,
	&tiv.F4_11,
	&tiv.F4_12,
	&tiv.F4_13,
	&tiv.F4_14,
	&tiv.F4_15,
	&tiv.F4_16,
	&tiv.F4_17,
	&tiv.F4_18,
	&tiv.F4_19,
	&tiv.F4_20,
	&tiv.F4_21,
	&tiv.F4_22,
	&tiv.F4_23,
	&tiv.F4_24,
	&tiv.F4_25,
	&tiv.F4_26,
	&tiv.F4_27,
	&tiv.F4_28,
	&tiv.F4_29,
	&tiv.F4_30,
	&tiv.F4_31,
	&tiv.F4_32,
	&tiv.F4_33,
	&tiv.F4_34,
	&tiv.F4_35,
	&tiv.F4_36,
	&tiv.F4_37,
	&tiv.F4_38,
	&tiv.F4_39,
	&tiv.F4_40,
	&tiv.F4_41,
	&tiv.F4_42,
	&tiv.F4_43,
	&tiv.F4_44,
	&tiv.F4_45,
	&tiv.F4_46,
	&tiv.F4_47,
	&tiv.F4_48,
	&tiv.F4_49,
	&tiv.F4_50,
	&tiv.F4_51,
	&tiv.F4_52,
	&tiv.F4_53,
	&tiv.F4_54,
	&tiv.F4_55,
	&tiv.F4_56,
	&tiv.F4_57,
	&tiv.F4_58,
	&tiv.F4_59,
	&tiv.F4_60,
	&tiv.F4_61,
	&tiv.F4_62,
	&tiv.F4_63,
	&tiv.F4_64,
	&tiv.F4_65,
	&tiv.F4_66,
	&tiv.F4_67,
	&tiv.F4_68,
	&tiv.F4_69,
	&tiv.F4_70,
	&tiv.F4_71,
	&tiv.F4_72,
	&tiv.F4_73,
	&tiv.F4_74,
	&tiv.F4_75,
	&tiv.F4_76,
	&tiv.F4_77,
	&tiv.F4_78,
	&tiv.F4_79,
	&tiv.F4_80,
	&tiv.F4_81,
	&tiv.F4_82,
	&tiv.F4_83,
	&tiv.F4_84,
	&tiv.F4_85,
	&tiv.F4_86,
	&tiv.F4_87,
	&tiv.F4_88,
	&tiv.F4_89,
	&tiv.F4_90,
	&tiv.F4_91,
	&tiv.F4_92,
	&tiv.F4_93,
	&tiv.F4_94,
	&tiv.F4_95,
	&tiv.F4_96,
	&tiv.F4_97,
	&tiv.F4_98,
	&tiv.F4_99,
	&tiv.F4_100,
	&tiv.F4_101,
	&tiv.F4_102,
	&tiv.F4_103,
	&tiv.F4_104,
	&tiv.F4_105,
	&tiv.F4_106,
	&tiv.F4_107,
	&tiv.F4_108,
	&tiv.F4_109,
	&tiv.F4_110,
	&tiv.F4_111,
	&tiv.F4_112,
	&tiv.F4_113,
	&tiv.F4_114,
	&tiv.F4_115,
	&tiv.F4_116,
	&tiv.F4_117,
	&tiv.F4_118,
	&tiv.F4_119,
};

/****************************************************************************************************/
/* Trip to next floor																		*/
/****************************************************************************************************/
void diving_state (void)
{
	BYTE i;
	BYTE startdir;
	WORD * ptr;
	WORD door_zone_state;
	DWORD levelpos;
	DWORD delaytimer = 0;
	BYTE diving_fg = 0;

	++ diving_fail_cnt;
	if (diving_fail_cnt > MAX_DIVING_FAIL)		// diving 4次不成功
	{
		if (!(poserror & P_DIVING_ERR))
		{
			poserror |= P_DIVING_ERR;
			write_errorhist (E_DIVING_ERR, HSE_ID, C_DIVING_ERROR, 0);
		}
		cycle ();
		set_state ();
		return;
	}

	calls_off = 1;		//在divind 状态不允许呼梯
	
#if	SRU_OLD
	if (state_ulsi != SWITCH_OFF)
		ulsi_off_timer = timer + 1 SEC;
	SetSwitch(SWITCH_ULSI, SWITCH_OFF);
#endif
	
	ct = timer + 2 SEC;							/* wait 2 seconds							*/
	
	level %= (p.top_floor + 1);
	nice5000_Tcmd_data2 = (nice5000_Tcmd_data2 & (~(NICE5000_TCMD2_STATE|NICE5000_TCMD2_TARGET))) | TCMD2_STATE_DIVING;// | (level + 1);
	while ((ct > timer)
		|| (!start_enabled ())
		|| ((nice5000_Rcmd_data2 & NICE5000_RCMD2_STATE) != TCMD2_STATE_DIVING))			/* check if start is enabled					*/
	{
		cycle ();
		set_state ();								/* set new state							*/
		if (hse_state != H_DIVING)					/* state changed							*/
			return;							/* go to new state						*/
	}
	send_hse_state ();
	level %= (p.top_floor + 1);
	target_floor = level;							/* go to next floor						*/
	start_floor = level;
	insp_return = 0;			//返平层时，取消检修返回操作

	if (level != 0xFF)
		{
			levelpos = 100000ul;
			for (i=0; i<level; ++i)
				{
					ptr = (WORD *)c_level_pos[i * 2];
					levelpos += *(ptr + 1);
					levelpos += ((DWORD)(*ptr) << 16);
					levelpos += tiv.F4_04;
				}
			if(door_zone & POS_ILD)
				{//碰到下限位					
					if((level < p.top_floor) && (level >= p.bot_floor))
						target_floor = level + 1;
					else
						target_floor = p.bot_floor + 1;
					startdir = DIR_UP;
				}
			else if(door_zone & POS_ILU)
				{//碰到上限位					
					if((level <= p.top_floor) && (level > p.bot_floor))
						target_floor = level - 1;
					else
						target_floor = p.top_floor - 1;
					startdir = DIR_DN;
				}
			else if (force_switch & FORCE_SDD1)
				{
					if(!(switchexist & POS_ILD))
						{//限位开关不使能							
							if((level < p.top_floor) && (level >= p.bot_floor))
								target_floor = level + 1;
							else
								target_floor = p.bot_floor + 1;
							startdir = DIR_UP;
						}
					else
						{
							if((door_zone & POS_ILD))
								{//碰到限位开关
									if((level < p.top_floor) && (level >= p.bot_floor))
										target_floor = level + 1;
									else
										target_floor = p.bot_floor + 1;
									startdir = DIR_UP;	
								}
							else
								{
									if(level == p.bot_floor)
										{//电梯已经处于1楼,上行
											target_floor = p.bot_floor + 1;
											startdir = DIR_UP;							
										}
									else
										{
											target_floor = level;
											startdir = DIR_UP;							
										}
								}
						}
				}
			else if (force_switch & FORCE_SDU1)
				{
					if(!(switchexist & POS_ILU))
						{//限位开关不使能 						
							if((level <= p.top_floor) && (level > p.bot_floor))
								target_floor = level - 1;
							else
								target_floor = p.top_floor - 1;
							startdir = DIR_DN;
						}
					else 
						{//碰到限位开关
							if(door_zone & POS_ILU)
								{
									if((level <= p.top_floor) && (level > p.bot_floor))
										target_floor = level - 1;
									else
										target_floor = p.top_floor - 1;
									startdir = DIR_DN;
								}
							else
								{
									if(level == p.top_floor)
										{//电梯已经处于顶楼,上行
											target_floor = p.top_floor - 1;
											startdir = DIR_DN;							
										}
									else if(level > p.bot_floor)
										{
											target_floor = level - 1;
											startdir = DIR_DN;							
										}
									else
										{
											target_floor = level;
											startdir = DIR_DN;											
										}
								}									
						}
				}
			else
				{
					if (levelpos > dCurrentPulse)
						startdir = DIR_UP;
					else
						startdir = DIR_DN;					
					target_floor = level; 						/* go to next floor 	*/
				}
			if (startdir == DIR_UP) 						/* up direction 						*/
				{
					for (i = target_floor; i <= p.top_floor; i++)
						{
							if (p.doorpos [i])						/* Lift has shaft door in this floor			*/
								{
									target_floor = i;
									diving_fg = 1;
									break;
								}
						}
					if(!diving_fg)
						target_floor = p.top_floor;
				}
			else											/* down direction 						*/
				{
					for (i = 0; i <= (target_floor - p.bot_floor); i++)
						{
							if (p.doorpos [target_floor - i]) 		/* Lift has shaft door in this floor			*/
								{
									target_floor -= i;
									diving_fg = 1;
									break;
								}
						}
					if(!diving_fg)
						target_floor = p.bot_floor;
				}
		}	
	else
		{
			if (force_switch & FORCE_SDD1)
				{
					if((!(switchexist & POS_ILD)) || (door_zone & POS_ILD))
						{//碰到下限位，往上
							target_floor = p.bot_floor + 1;
							startdir = DIR_UP;
						}
					else
						{
							target_floor = p.bot_floor;
							startdir = DIR_DN;					
						}
				}
			else if (force_switch & FORCE_SDU1)
				{
					if((!(switchexist & POS_ILU)) || (door_zone & POS_ILU))
						{//碰到上限位，往下
							target_floor = p.top_floor - 1;
							startdir = DIR_DN;
						}
					else
						{
							target_floor = p.top_floor;
							startdir = DIR_UP;					
						}
				}
			else
				{
					target_floor = p.top_floor;
					startdir = DIR_UP;
				}
		}	

	diving_fg = 0;
	target_floor %= (p.top_floor + 1);
	nice5000_Tcmd_data2 = (nice5000_Tcmd_data2 & (~(NICE5000_TCMD2_STATE | NICE5000_TCMD2_TARGET))) | TCMD2_STATE_DIVING | (target_floor + 1);

	calls_off = 0;		//允许呼梯
	door_zone_state = door_zone;
	ct = timer + (p.triptime SEC);
	diving = 1;
	max_target = get_max_target (startdir);
	next_floor = target_floor;
	door_command (CLOSE_DOOR, CLOSE_DOOR);
	start_drive (startdir);
//增加返平层蜂鸣器鸣叫功能
	if(p.cooperate_sel1 & COOP1_DIVING_GONG)
		set_out(SPEAKER_BUZ, BUZZER_LEVELING, 0, 0, 1, O_CANA | O_CANB | O_HSE);
//增加返平层蜂鸣器鸣叫功能	
	while (1)
	{
		check_ilu_ild();
		cycle ();
		set_state ();									/* set new state							*/
		if (hse_state != H_DIVING)
		{
			diving = 0;
			if(p.cooperate_sel1 & COOP1_DIVING_GONG)
				set_out(SPEAKER_BUZ, BUZZER_LEVELING, 0, 0, 0, O_CANA | O_CANB | O_HSE);
			return;
		}
		if ((door_zone & POS_SGM) != (door_zone_state & POS_SGM))					/* state of door zone switches changed	*/
		{
			ct = timer + (p.triptime SEC);					/* retrigger trip time					*/
			door_zone_state = door_zone;				/* save old state of door zone switches	*/
		}
		if ((ct < timer)	&& (!(p.cooperate_sel1 & COOP1_DIVING_NOTIME)))	
		{	/* time out, lift did not stop			*/
			emergency_stop ();						/* send emergency stop				*/
			general_error |= G_TRIPTIME;				/* set trip time exceeded error			*/
			write_errorhist (E_TRIP_TIME, HSE_ID, C_TRIP_TIME, 0);
			hse_state = H_WAIT_STOP;
			return;
		}

		if (drive_state == D_STOPPED)
			{
				if (!diving_fg)
					{
						diving_fg = 1;
						delaytimer = timer + 3 SEC;
					}
				else if (delaytimer < timer)
					{
						if ((adt == DOOR_NORMAL) && p.doorpos[level] && (door_zone & POS_SGM))
							{
								dooropenpush = DOOR1 | DOOR2;
								handle_dooropenpush();
								dooropenpush = 0;
							}
						diving = 0;
						set_state ();
						return;
					}			
			}		
	}
}


