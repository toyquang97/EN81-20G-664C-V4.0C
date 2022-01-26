
#include "typedef.h"
#include "parameter.h"
#include "nice5000.h"
#include "state.h"
#include "call.h"
#include "can.h"
#include "drive.h"
#include "io.h"
#include "history.h"


/****************************************************************************************************/
/* Inspection state																		*/
/****************************************************************************************************/
void inspection_state (void)
{
	DWORD limittimer = 0;
	DWORD touchtimer = 0;
	BYTE dir = 0;
	BYTE doorcommand = 1;
	BYTE limitenable = 0;
	BYTE limitact = 0;
	BYTE btnpressed = 0;
	BYTE i = 0;
	BYTE ils_active = 0;
	BYTE ils_action = 0;
	WORD insp_state = 0;
	BYTE insp_active = 0;

	ct = timer;
	touchtimer = mstimer + 2;
	send_hse_state ();
	calls_off = 1;										/* disable calls						*/
	clearcalls (ALL_CALLS);								/* clear all calls						*/
	triperrorcount = 0;									/* reset error counter					*/
	for (i = 0; i < p.doornumber; ++i)
	{
		dooropenerrorcount [i] = 0;
		doorcloseerrorcount [i] = 0;
	}
	errorwarning = 0;									/* reset error warning					*/
	fault_signal = 0;									/* reset fault signal					*/
	out_of_order = 1;									/* set out of order indication			*/
	diving_fail_cnt = 0;	
	if(poserror & P_DIVING_ERR)
		poserror &= ~P_DIVING_ERR;
	
#if	SRU_OLD
	if (state_ulsi != SWITCH_OFF)
		ulsi_off_timer = timer + 1 SEC;
	SetSwitch(SWITCH_ULSI, SWITCH_OFF);
#endif

	set_out (SPECIAL_FUNC, DOOR_OPEN, 0, EXISTING_DOORS, 0, O_CANA);
	while(touchtimer > mstimer)
		{
			ClrWdt();
		}
	set_out (SPECIAL_FUNC, DOOR_CLOSE, 0, EXISTING_DOORS, 0, O_CANA);
	touchtimer = 0;

	if(ups_power)		
		{
			nice5000_Tcmd_data2 = (nice5000_Tcmd_data2 & (~(NICE5000_TCMD2_STATE|NICE5000_TCMD2_TARGET))) | TCMD2_STATE_EVACUATION;
			insp_state = TCMD2_STATE_EVACUATION;
		}
	else
		{
			nice5000_Tcmd_data2 = (nice5000_Tcmd_data2 & (~(NICE5000_TCMD2_STATE|NICE5000_TCMD2_TARGET))) | TCMD2_STATE_INSPECTION;
			insp_state = TCMD2_STATE_INSPECTION;
		}

	insp &= (INSP_TOPCAR_ACTIVE | INSP_PITBOX_ACTIVE);
	sc_limit_fg = 0;
	she_calldoor_double = 0;
	
	if(general_error & G_DL_SHORTED)
		{
			general_error &= ~G_DL_SHORTED;
//			dl_shorted = 0;
		}

	while (1)
	{		
		nice5000_Tcmd_data2 = (nice5000_Tcmd_data2 & (~(NICE5000_TCMD2_STATE|NICE5000_TCMD2_TARGET))) | insp_state;
		cycle ();
		set_state ();									/* set new state						*/
		if (hse_state != H_INSP)							/* state changed						*/
		{
			if (drive_state != D_STOPPED)				/* drive not in stopped state		保证轿顶安全，状态切换务必急停	*/
			{
				emergency_stop ();							/* stop drive									*/
				hse_state = H_WAIT_STOP;
			}
			if ((p.setup_ready) && (p.cooperate_sel1 & COOP1_INSP_RETURN) && (!pitbox_reset_active))
				insp_return = 1;
			return;								/* go to new state					*/
		}
		
		if(insp)
			insp_active = (insp_dir | INSP_ON);
		else
			insp_active = 0;

		switch (insp_active & 0x07)							/* set max. speed depennding from direction		*/
		{
			case (3):									/* up direction						*/
				dir = DIR_UP;						/* set ils_active direction				*/
				if(!(p.lift_func_sel1 & LIFTFUN1_INSP_LOCK))
					{
						if (switchexist & POS_ILU)
							{
								if (door_zone & POS_ILU)
									ils_active = 1;
								else
									ils_active = 0;
							}
						else if ((p.setup_ready) && (level == p.top_floor) && (state_msi))
							ils_active = 1;
						else
							ils_active = 0;
					}
				else
					{
						if(door_zone & POS_SDU1)
							ils_active = 1;
						else
							ils_active = 0;
					}
				break;

			case (5):									/* down direction						*/
				dir = DIR_DN;						/* set inspection direction				*/
				if (switchexist & POS_ILD)
				{
					if (door_zone & POS_ILD)
						ils_active = 1;
					else
						ils_active = 0;
				}
				else if ((p.setup_ready) && (level == p.bot_floor) && (state_msi))
					ils_active = 1;
				else
					ils_active = 0;
				break;

			default:
				dir = 0;
				break;
		}

//增加门旁路装置的机械到位开关的检测
		if(p.cooperate_sel2 & COOP2_CL_LIMIT)
			{
				if(door_short_conn && (door_closeswitch_limit != ALL_DOORS_CLOSED))
					ils_active = 1;
			}
//增加门旁路装置的机械到位开关的检测

		switch (insp_active & 0x07)							/* handle pushes						*/
		{
			case (3):									/* up direction						*/
			case (5):									/* down direction						*/
				if (!btnpressed)
				{
					btnpressed = 1;
					touchtimer = timer + 3;
				}
				if (!doorcommand)						/* no door command					*/
				{
					door_command (CLOSE_DOOR, CLOSE_DOOR);
					doorcommand = 1;					/* set marker for door command		*/
				}
				if (drive_state == D_STOPPED)			/* drive in stopped state				*/
				{
					if (p.motor_tuned)
					{
						if (((!p.setup_ready) ||				/* if setup not ready ignore door state	*/
						(doorstate == ALL_DOORS_CLOSED)) &&	/* all doors closed				*/
						((nice5000_Rcmd_data2 & NICE5000_RCMD2_STATE) == insp_state) &&
						(touchtimer < timer) &&
						(safety_circuit & SC3))				/* and sefety circuit closed			*/
						{
							if ((!limitenable) && (!limitact) && (!ils_active) && dir)
							{
								start_drive (dir);					/* start drive						*/
								limittimer = timer + INSP_LIMIT_TIMER;
								limitenable = 1;
								move_dooropen_flag = 0;
							}
						}
						else
						{
							limitenable = 0;
							if((doorstate & DOOR1) == 0)
								move_dooropen_flag = 1;
							else if((doorstate & DOOR2) == 0)
								move_dooropen_flag = 2;
						}
					}
					ils_action = 0;
				}
				else									/* drive in moving state				*/
				{
					if (direction != dir)
					{
						stop_drive ();
						limitenable = 0;
					}
					else if ((ils_active) && (!ils_action))
					{
						stop_drive ();
						limitenable = 0;
						ils_action = 1;
					}

					if ((limitenable) && (!limitact))
					{
						if (limittimer < timer)
						{
							limitact = 1;
							stop_drive ();
							write_errorhist (E_INSP_STICK, HSE_ID, 0, 0);
						}
					}
					if(bFunc_flag & FUNC_CANA_ERROR)
						{
							if(drive_state != D_STOPPED)
								stop_drive ();							
							insp_active &= ~(INSP_UP | INSP_DOWN);
						}
				}
				break;

			default:
				btnpressed = 0;
				limitact = 0;
				if (drive_state != D_STOPPED)			/* drive not in stopped state			*/
				{
					if((drive_state == D_START_MOVING) || (drive_state == D_READY_TO_SWITCH_ON))
						emergency_stop ();
					else
						stop_drive ();
					limitenable = 0;
				}
				else if (doorcommand)
				{
					if (doorstate != ALL_DOORS_CLOSED)	/* if door is open stop door		*/
						door_command (STOP_DOOR, STOP_DOOR);
					doorcommand = 0;					/* reset marker for door command		*/
				}
				break;
		}

#if	SRU_OLD
//增加 UCMP 现场的验收测试	
		if(bTestUCMP)
			{
				if((state_ulsi == SWITCH_ON) && (drive_state == D_STOPPED) && (!(safety_circuit & SC3))
							&& (ulsi_off_timer < timer) && (dz_input))
					{
						if(!ucmp_state)
							{
								ucmp_state = 1; 			
								write_errorhist (E_UCMP_LOCK, HSE_ID, C_UCMP_LOCK, W_UCMP_LOCK);
							} 					
						bTestUCMP = 0;		//复位测试模式						
						if (state_ulsi != SWITCH_OFF)
							ulsi_off_timer = timer + 1 SEC;
						SetSwitch(SWITCH_ULSI, SWITCH_OFF);
					}
			}
//增加 UCMP 现场的验收测试
#endif

		fan_on();
	}
}

