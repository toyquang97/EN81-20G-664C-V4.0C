
#include "typedef.h"
#include "parameter.h"
#include "state.h"
#include "call.h"
#include "can.h"
#include "drive.h"
#include "io.h"
#include "history.h"
#include "nice5000.h"

/****************************************************************************************************/
/* Emergency operation state																*/
/****************************************************************************************************/
void em_op_state (void)
{
	DWORD limittimer = 0;
	DWORD touchtimer = 0;
	BYTE singlebtnpressed = 0;
	BYTE doublebtnpressed = 0;
	BYTE em_op_dir = 0;
	BYTE doorclosecommand;
	BYTE dooropencommand;
 	BYTE limitenable = 0;
	BYTE limitact = 0;
	BYTE i = 0;
	WORD insp_state = 0;
	BYTE ils_active = 0;	
	BYTE ils_action = 0;

	if (em_op & 0x06)
		emop_btn_stick = 1;
	doorclosecommand = 1;
	dooropencommand = 0;
	ct = timer;
	touchtimer = mstimer + 2;
	send_hse_state ();
	calls_off = 1;										/* disable calls						*/
	clearcalls (ALL_CALLS);								/* clear all calls						*/
	target_floor = 0xFF;								/* no target floor								*/
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
	limit_switch_error = 0; 							/* reset error marker					*/
	
#if	SRU_OLD
//进入检修状态，关闭SRU
	if (state_ulsi != SWITCH_OFF)
		ulsi_off_timer = timer + 1 SEC;
	if(!bTestUCMP)
		SetSwitch(SWITCH_ULSI, SWITCH_OFF);
//进入检修状态，关闭SR3
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
	
	em_op &= INSP_ON;
	sc_limit_fg = 0;	
	she_calldoor_double = 0;
	if((general_error & G_DL_SHORTED) || dl_shorted)
		{
			general_error &= ~G_DL_SHORTED;
			dl_shorted = 0;
		}	


	while (1)
	{
		cycle ();
		set_state ();									/* set new state						*/
		if (hse_state != H_EM_OP)						/* state changed						*/
		{
			if (drive_state != D_STOPPED)				/* drive not in stopped state			*/
			{
				emergency_stop ();					/* stop drive							*/
				hse_state = H_WAIT_STOP;				/* wait for stop						*/
			}
			if ((p.setup_ready) && (p.cooperate_sel1 & COOP1_INSP_RETURN))
				insp_return = 1;
			return;								/* go to new state					*/
		}
		switch (em_op & 0x07)							/* set max. speed depennding from direction		*/
		{
			case (3):									/* up direction						*/
				em_op_dir = DIR_UP;						/* set ils_active direction				*/
				if(bTest_IlsSwitch)					
					ils_active = 0;
				else if (switchexist & POS_ILU)
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
				break;

			case (5):									/* down direction						*/
				em_op_dir = DIR_DN;						/* set inspection direction				*/
				if(bTest_IlsSwitch)					
					ils_active = 0;
				else if (switchexist & POS_ILD)
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
				em_op_dir = 0;
				break;
		}
		
//增加门旁路装置的机械到位开关的检测
		if(p.cooperate_sel2 & COOP2_CL_LIMIT)
			{
				if(door_short_conn && (door_closeswitch_limit != ALL_DOORS_CLOSED))
					ils_active = 1;
			}
//增加门旁路装置的机械到位开关的检测

		
		switch (em_op & 0x07)							/* handle pushes								*/
		{
			case (3):									/* up direction								*/
			case (5):									/* down direction								*/
				doublebtnpressed = 0;
				if (!singlebtnpressed)
				{
					singlebtnpressed = 1;
					touchtimer = timer + 10;
				}
				if ((!doorclosecommand) && (!dooropencommand))
				{
					door_command (CLOSE_DOOR, CLOSE_DOOR);
					doorclosecommand = 1;					/* set marker for door command				*/
				}
				if (drive_state == D_STOPPED)			/* drive in stopped state						*/
				{
					if (((!p.setup_ready) ||	(bTestUCMP)	||		/* if setup not ready ignore door state			*/
					(doorstate == ALL_DOORS_CLOSED)) &&		/* all doors closed					*/
					(!emop_btn_stick) &&
					(!limitenable) &&
					(!limitact) &&
					(!ils_active) &&
					(touchtimer < timer) &&
					((nice5000_Rcmd_data2 & NICE5000_RCMD2_STATE) == insp_state) &&
					((safety_circuit & SC3) || (bTestUCMP))) /* and sefety circuit closed					*/
					{
						start_drive (em_op_dir);		/* start drive							*/
						limittimer = timer + INSP_LIMIT_TIMER;
						limitenable = 1;
						move_dooropen_flag = 0;
					}
					else
					{
						limitenable = 0;
						if((doorstate & DOOR1) == 0)
							move_dooropen_flag = 1;
						else if((doorstate & DOOR2) == 0)
							move_dooropen_flag = 2;
					}
					ils_action = 0;
				}
				else									/* drive in moving state						*/
				{
					if (direction != em_op_dir)			/* drive is moving downwards					*/
					{
						limitenable = 0;
						stop_drive ();							/* stop drive									*/
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
							em_op &= ~(INSP_UP | INSP_DOWN);
						}
				}
				break;

			default:
				singlebtnpressed = 0;
				if (drive_state != D_STOPPED)			/* drive not in stopped state					*/
				{
					limitenable = 0;
					stop_drive ();							/* stop drive ("soft stop")						*/
				}
				else if (doorclosecommand)
				{
					if (doorstate != ALL_DOORS_CLOSED)		/* if door is open stop door			*/
						door_command (STOP_DOOR, STOP_DOOR);
					doorclosecommand = 0;					/* reset marker for door command				*/
				}
				else
				{
					if (((em_op & 0x06) == 0x06) && (door_zone & POS_SGM))
					{
						if (!doublebtnpressed)
						{
							doublebtnpressed = 1;
							dooropencommand = 0;
							touchtimer = timer + 30;
						}
						else if (touchtimer < timer)
						{
							if (!dooropencommand)
							{
								dooropencommand = 1;
								if (p.doornumber == 2)
									door_command (OPEN_DOOR, OPEN_DOOR);
								else
									door_command (OPEN_DOOR, DOOR_OFF);
							}
						}
					}
					else
					{
						doublebtnpressed = 0;
						dooropencommand = 0;
					}
				}
				break;
		}	

#if	SRU_OLD
//增加 UCMP 现场的验收测试
		if(bTestUCMP)
			{
				if((state_ulsi == SWITCH_ON) && (drive_state == D_STOPPED) && (!(safety_circuit & SC3))
							&& (ulsi_off_timer < timer))// && (dz_input))
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

void motorturn_state (void){
	DWORD touchtimer = 0;
	BYTE btnpressed = 0;
	BYTE em_op_dir = 0;
	BYTE doorcommand;
	BYTE finished = 0;

	if (drive_state != D_STOPPED)
	{
		emergency_stop ();
		hse_state = H_WAIT_STOP;
		return;
	}
	nice5000_Tcmd_data2 = (nice5000_Tcmd_data2 & (~(NICE5000_TCMD2_STATE|NICE5000_TCMD2_TARGET))) 
							| (TCMD2_STATE_MOTOR_TURN | MOTOR_TURN_01);
	doorcommand = 1;
	ct = timer;
	send_hse_state ();
	calls_off = 1;										/* disable calls						*/
	clearcalls (ALL_CALLS); 							/* clear all calls						*/
	target_floor = 0xFF;								/* no target floor								*/
	fault_signal = 0;									/* reset fault signal					*/
	out_of_order = 1;									/* set out of order indication			*/
	
	em_op &= 0x01;
	she_calldoor_double = 0;
	
	while (1)
	{
		cycle ();
		set_state ();									/* set new state						*/
		if (hse_state != H_IV_TEACH_IN)						/* state changed						*/
		{
			if (!finished)
			{
				if (drive_state != D_STOPPED)				/* drive not in stopped state			*/
				{
					emergency_stop ();					/* stop drive							*/
					hse_state = H_WAIT_STOP;				/* wait for stop						*/
				}
				return;
			}
			else
			{
				if (drive_state == D_STOPPED)
					return;
			}
		}
		switch (em_op & 0x07)							/* handle pushes								*/
		{
			case (3):									/* up direction 							*/
			case (5):									/* down direction								*/
				if (!btnpressed)
				{
					btnpressed = 1;
					touchtimer = timer + 3;
				}
				if (!doorcommand)						/* no door command							*/
				{
					door_command (CLOSE_DOOR, CLOSE_DOOR);
					doorcommand = 1;					/* set marker for door command				*/
				}
				if (drive_state == D_STOPPED)			/* drive in stopped state						*/
				{
					if ((!emop_btn_stick) &&
					(touchtimer < timer) &&
					((nice5000_Rcmd_data2 & NICE5000_RCMD2_STATE) == RCMD2_STATE_MOTOR_TURN) &&
					(safety_circuit & SC3)) 			/* and sefety circuit closed					*/
					{
						if ((em_op & 0x07) == 3)		/* up direction pressed 					*/
							em_op_dir = DIR_UP; 	/* set up direction 						*/
						else							/* down direction pressed						*/
							em_op_dir = DIR_DN; 	/* set down direction							*/
						start_drive (em_op_dir);		/* start drive							*/
					}
				}
				else if ((drive_state == D_WAIT_STOP) && (btnpressed))
				{
					p.motor_tuned = 1;
					savepara = 1;
					finished = 1;
					init_error |= B_DRV;
				}
				break;

			default:
				btnpressed = 0;
				if (drive_state != D_STOPPED)			/* drive not in stopped state					*/
				{
					emergency_stop ();
				}
				else if (doorcommand)
				{
					if (doorstate != ALL_DOORS_CLOSED)		/* if door is open stop door			*/
						door_command (STOP_DOOR, STOP_DOOR);
					doorcommand = 0;					/* reset marker for door command				*/
				}
				break;
		}		
	}
}

