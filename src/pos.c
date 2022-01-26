
#include "typedef.h"
#include "lpc1700.h"
#include "parameter.h"
#include "nice5000.h"
#include "state.h"
#include "io.h"
#include "history.h"
#include "Text.h"
#include "Can.h"
#include "Drive.h"


/****************************************************
* 函数名: EINT3_IRQHandler
* 功能: IO中断服务程序,处理MSI状态变化
* 入参: 无
* 出参: 无
* 返回值: 无
****************************************************/
void EINT3_IRQHandler (void){

	if (IO0IntStatr & (0x01ul << 30))
		{
			state_msi = 1;
			IO0IntClr = (0x01ul << 30);
		}
		else if (IO0IntStatf & (0x01ul << 30))
		{
			state_msi = 0;
			IO0IntClr = (0x01ul << 30);
		}
	if (IO2IntStatr & (0x01ul << 9))
	{
		psl_chk_on = 0;
		psl_chk_off = 0;
		IO2IntClr = (0x01ul << 9);
	}
	if (IO2IntStatr & (0x01ul << 12))
	{
		cdsl_chk_on = 0;
		cdsl_chk_off = 0;
		IO2IntClr = (0x01ul << 12);
	}
	if (IO2IntStatr & (0x01ul << 13))
	{
		sdsl_chk_on = 0;
		sdsl_chk_off = 0;
		IO2IntClr = (0x01ul << 13);
	}
}


void check_ilu_ild(void)
{
	if ((door_zone & POS_ILU) && (direction == DIR_UP))
		{
			if (!(poserror & P_ILO_ON))
				{
					poserror |= P_ILO_ON;
					write_errorhist (E_ILS_ERROR, HSE_ID, C_ILS_ON, W_ILS_ON);
				}
		}
	if ((door_zone & POS_ILD) && (direction == DIR_DN))
		{
			if (!(poserror & P_ILU_ON))
				{
					poserror |= P_ILU_ON;
					write_errorhist (E_ILS_ERROR, HSE_ID, C_ILS_ON, W_ILS_ON);
				}
		}	
}

/****************************************************************************************************/
/* Calculate max. target floor																*/
/****************************************************************************************************/
BYTE get_max_target (BYTE startdir)
{
	if (startdir == DIR_UP)							/* up direction							*/
		return (p.top_floor);						/* max. target floor is highest floor			*/
	else											/* down direction						*/
		return (p.bot_floor);						/* max. target floor is lowest floor			*/
}

void check_UCMP(void){
	
	if((doorstate != ALL_DOORS_CLOSED) && //!(safety_circuit & (SC2 | SC3)))
			((!(safety_circuit & (SC2 | SC3)) && !(p.relevel | p.adv_door_op)) || 
			(!(safety_circuit & SC2) && (p.relevel | p.adv_door_op))))
		{//开着门检测
			if(!(door_zone & POS_SGM) &&  //门区发生变化
					(((drive_state == D_STOPPED) && (nice5000_Rstore_feedspd > 50)) ||
					((drive_state != D_STOPPED) && (nice5000_Rstore_feedspd > p.ucmp_speed))))
				{//当前轿厢离开门区并且存在编码器计数
					emergency_stop ();
					if(!ucmp_state)
						{
							ucmp_state = 1;				
							write_errorhist (E_UCMP_LOCK, HSE_ID, C_UCMP_LOCK, W_UCMP_LOCK);
						}
				}
		}
}

//检测电梯的当前速度
void check_speed(void){

	if((p.lift_func_sel1 & LIFTFUN1_CHECK_OVERSPEED) &&
			(p.simulation != SIMULATION_AUTO) && (drive_state != D_STOPPED))
		{
			if(niv.F0_03 >= p.max_speed)				
				max_speed = p.max_speed;
			else
				max_speed = niv.F0_03;

			if((direction & DIR_UP) && (force_switch & FORCE_SDU1) && (door_zone & POS_SGM))
				{//电梯上行至顶层
					if(nice5000_Rstore_feedspd >= max_speed)
						{//接近门区的速度过大 						
							emergency_stop ();
							if(!(bFunc_flag & FUNC_OVERSPEEP_ACTIVE))
								{
									bFunc_flag |= FUNC_OVERSPEEP_ACTIVE;			
									write_errorhist (E_NICE_OVER_SPEED, HSE_ID, C_OVERSPEED, W_OVERSPEED);
								}
						}
				}
			else if(((direction & DIR_DN) && (force_switch & FORCE_SDD1) && (door_zone & POS_SGM)))
				{//电梯下行至底层
					if(nice5000_Rstore_feedspd >= max_speed)
						{//接近门区的速度过大 						
							emergency_stop ();
							if(!(bFunc_flag & FUNC_OVERSPEEP_ACTIVE))
								{
									bFunc_flag |= FUNC_OVERSPEEP_ACTIVE;			
									write_errorhist (E_NICE_OVER_SPEED, HSE_ID, C_OVERSPEED, W_OVERSPEED);
								}
						}
				}
		}
}

extern const WORD * c_level_pos[];

BYTE get_target_floor_ups(BYTE dir){
	BYTE i, startdir = DIR_NULL;
	BYTE diving_fg = 0;
	WORD * ptr;
	DWORD levelpos;	

	level %= (p.top_floor + 1);
	target_floor = level;							/* go to next floor						*/

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
					if((level <= p.top_floor) && (level >= p.bot_floor))
						target_floor = level;
					else
						target_floor = p.bot_floor;
					startdir = DIR_UP;
				}
			else if(door_zone & POS_ILU)
				{//碰到上限位					
					if((level <= p.top_floor) && (level >= p.bot_floor))
						target_floor = level;
					else
						target_floor = p.top_floor;
					startdir = DIR_DN;
				}
			else if (force_switch & FORCE_SDD1)
				{
					if(!(switchexist & POS_ILD))
						{//限位开关不使能							
							if((level <= p.top_floor) && (level >= p.bot_floor))
								target_floor = level + 1;
							else
								target_floor = p.bot_floor + 1;
							startdir = dir;
						}
					else
						{
							if((level <= p.top_floor) && (level >= p.bot_floor))
								target_floor = level;
							else
								target_floor = p.bot_floor;
							if((door_zone & POS_ILD))		//碰到限位开关
								startdir = DIR_UP;	
							else
								startdir = dir;							
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
							startdir = dir;
						}
					else 
						{//碰到限位开关
							if((level <= p.top_floor) && (level > p.bot_floor))
								target_floor = level;
							else
								target_floor = p.top_floor;
							if(door_zone & POS_ILU)
								startdir = DIR_DN;
							else
								startdir = dir;									
						}
				}
			else
				startdir = dir;
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
	target_floor %= (p.top_floor + 1);
	return startdir;

}


