
#include "typedef.h"
#include "parameter.h"
#include "state.h"
#include "call.h"
#include "can.h"
#include "drive.h"
#include "history.h"
#include "nice5000.h"
#include "io.h"


/****************************************************************************************************/
/* terminal function																		*/
/****************************************************************************************************/
void terminal_state (void){
	BYTE doorcommand = 0;
	BYTE ucmp_test_flag = 0, ucmp_test_start = 0;
	DWORD wait_timer;
//	WORD speed_old = 0;
	WORD distance = 0;

	nice5000_Tcmd_data2 = (nice5000_Tcmd_data2 & (~(NICE5000_TCMD2_STATE|NICE5000_TCMD2_TARGET))) | TCMD2_STATE_INSPECTION;
	send_hse_state ();
	calls_off = 1;										/* disable calls						*/
	clearcalls (ALL_CALLS);								/* clear all calls						*/
	fault_signal = 0;									/* reset fault signal					*/
	out_of_order = 1;									/* set out of order indication			*/
	triperrorcount = 0;								/* reset error counter							*/

	wait_timer = timer + 1 SEC;
//	speed_old = niv.F3_11;
	if(bTestUCMP && p.ucmp_test_dir)
		{//UCMP 测试的时候需要设置响应的速度
			ucmp_test_flag = 1;
			if(p.ucmp_test_speed && (p.ucmp_test_speed < 630))
				{
//					send_nice5000_parameter(57, p.ucmp_test_speed);		//设置检修速度
				}			
			while (wait_timer > timer)
				{
					cycle ();
					set_state (); 							/* set new state								*/
					if (hse_state != H_TERM) 				/* other state								*/
						return;
				}
			if(p.ucmp_test_dir == DIR_UP)
				{
					term_active = 3;
					target_floor = level + 1;
				}
			else if(p.ucmp_test_dir == DIR_DN)
				{
					term_active = 5;
					if(level)
						target_floor = level - 1;
					else
						target_floor = level;
				}
			else
				term_active = 0;
			
		}
	
	while (1)
	{
		cycle ();
		set_state ();									/* set new state						*/
		if (hse_state != H_TERM)						/* state changed						*/
		{
			if (drive_state != D_STOPPED)				/* drive not in stopped state			*/
			{
				emergency_stop ();					/* stop drive							*/
				hse_state = H_WAIT_STOP;				/* wait for stop						*/
			}
			if(ucmp_test_start)
				{
//					if(!(safety_circuit & SC3) && !(door_zone & POS_SGM) && bTestUCMP)
					if(!(safety_circuit & SC3) && bTestUCMP)
						{//安全回路断开							
							if(!ucmp_state)
								{
									ucmp_state = 1; 
									write_errorhist (E_UCMP_LOCK, HSE_ID, C_UCMP_LOCK, W_UCMP_LOCK);
								} 		
//计算移动距离				
							if(nice5000_Rstore_position > distance)
								{
									ucmp_test_distance = nice5000_Rstore_position - distance + 50;
								}
							else
								{
									ucmp_test_distance = distance - nice5000_Rstore_position + 50;
								}
//计算移动距离				
							ucmp_test_finish = 1;
							ucmp_test_floor = level;
							bTestUCMP = 0;
							term_active = 0;
						}
				}
			return;								/* go to new state					*/
		}

		switch (term_active & 0x07)
		{
			case (3):									/* up direction						*/
				if (drive_state == D_STOPPED)			/* drive in stopped state				*/
				{
					if (!doorcommand)					/* no door command					*/
					{
						door_command (CLOSE_DOOR, CLOSE_DOOR);
						doorcommand = 1;
					}
					else if (((safety_circuit & SC3) || (ucmp_test_flag))
							&&((nice5000_Rcmd_data2 & NICE5000_RCMD2_STATE) == RCMD2_STATE_INSPECTION))
						{
							start_drive (DIR_UP);
							ucmp_test_start = ucmp_test_flag;
							ucmp_test_finish = 0;
							distance = nice5000_Rstore_position;
							ucmp_test_distance = 0;
						}
				}
				else									/* drive in moving state				*/
				{
					if (direction == DIR_DN)				/* drive is moving downwards			*/
						emergency_stop ();			/* stop drive							*/
				}
				if((door_zone & POS_ILU) && (door_zone & POS_SDU1) && (drive_state != D_STOPPED) )
					{
						stop_drive();
						doorcommand = 0;
					}
				break;

			case (5):									/* down direction						*/
				if (drive_state == D_STOPPED)			/* drive in stopped state				*/
				{
					if (!doorcommand)					/* no door command					*/
					{
						door_command (CLOSE_DOOR, CLOSE_DOOR);
						doorcommand = 1;
					}
					else if (((safety_circuit & SC3) || (ucmp_test_flag))
							&&((nice5000_Rcmd_data2 & NICE5000_RCMD2_STATE) == RCMD2_STATE_INSPECTION))
						{
							start_drive (DIR_DN);
							ucmp_test_start = ucmp_test_flag;
							ucmp_test_finish = 0;
							distance = nice5000_Rstore_position;
							ucmp_test_distance = 0;
						}
				}
				else									/* drive in moving state				*/
				{
					if (direction == DIR_UP)				/* drive is moving upwards			*/
						emergency_stop ();				/* stop drive					*/
				}
				if((door_zone & POS_ILD) && (door_zone & POS_SDD1) && (drive_state != D_STOPPED) )
					{
						stop_drive();
						doorcommand = 0;
					}
				break;

			case (7):
			case 6:
				stop_drive();
				doorcommand = 0;
				break;

			default:
				if (drive_state != D_STOPPED)			/* drive not in stopped state			*/
					emergency_stop ();				/* stop drive							*/
				doorcommand = 0;
				break;
		}

		if(ucmp_test_start)
			{
				if(!(safety_circuit & SC3) && !(door_zone & POS_SGM) && bTestUCMP)
					{//安全回路断开 						
						if(!ucmp_state)
							{
								ucmp_state = 1; 
								write_errorhist (E_UCMP_LOCK, HSE_ID, C_UCMP_LOCK, W_UCMP_LOCK);
							} 		
//计算移动距离				
						if(nice5000_Rstore_position > distance)
							{
								ucmp_test_distance = nice5000_Rstore_position - distance + 50;
							}
						else
							{
								ucmp_test_distance = distance - nice5000_Rstore_position + 50;
							}
//计算移动距离				
						ucmp_test_finish = 1;
						ucmp_test_floor = level;
						bTestUCMP = 0;
						term_active = 0;
					}
			}		
		fan_on ();
	}
}


