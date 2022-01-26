
#include "typedef.h"
#include "parameter.h"
#include "nice5000.h"
#include "state.h"
#include "call.h"
#include "can.h"
#include "io.h"
#include "drive.h"
#include "text.h"

void set_shaft_switch(void){
	BYTE i, j, state;
	WORD value = 0;
	DWORD set_timer = 0;
	
	set_timer = mstimer + 12;		//等待60ms
	p.hse_inpar [INPUT_SDU1][IO_BASIC_FUNC] = FORCE_SWITCH;
	p.hse_inpar [INPUT_SDU1][IO_SUB_FUNC] = FORCE_SDU1;
	p.hse_inpar [INPUT_SDU1][IO_STATE] = POLARITY_BIT;
	p.hse_inpar [INPUT_SDU1][IO_ENABLE] = ENABLE;
	p.hse_inpar [INPUT_SDD1][IO_BASIC_FUNC] = FORCE_SWITCH;
	p.hse_inpar [INPUT_SDD1][IO_SUB_FUNC] = FORCE_SDD1;
	p.hse_inpar [INPUT_SDD1][IO_STATE] = POLARITY_BIT;
	p.hse_inpar [INPUT_SDD1][IO_ENABLE] = ENABLE;
	p.F5_X3 = NICE5000_INPUT_SDU1 + NICE5000_INPUT_LOGIC;
	p.F5_X4 = NICE5000_INPUT_SDD1 + NICE5000_INPUT_LOGIC;
	send_nice5000_parameter(186, p.F5_X3);
	while(set_timer > mstimer)
		{
			cycle ();
		}
	set_timer = mstimer + 12;		//等待60ms
	send_nice5000_parameter(187, p.F5_X4);
	while(set_timer > mstimer)
		{
			cycle ();
		}

//强减2
	value = niv.F0_04;
	if(value > 1750)
		{
			p.hse_inpar [INPUT_SDU2][IO_BASIC_FUNC] = FORCE_SWITCH;
			p.hse_inpar [INPUT_SDU2][IO_SUB_FUNC] = FORCE_SDU2;
			p.hse_inpar [INPUT_SDU2][IO_STATE] = POLARITY_BIT;
			p.hse_inpar [INPUT_SDU2][IO_ENABLE] = ENABLE;
			p.hse_inpar [INPUT_SDD2][IO_BASIC_FUNC] = FORCE_SWITCH;
			p.hse_inpar [INPUT_SDD2][IO_SUB_FUNC] = FORCE_SDD2;
			p.hse_inpar [INPUT_SDD2][IO_STATE] = POLARITY_BIT;
			p.hse_inpar [INPUT_SDD2][IO_ENABLE] = ENABLE;
			p.F5_X5 = NICE5000_INPUT_SDU2 + NICE5000_INPUT_LOGIC;
			p.F5_X6 = NICE5000_INPUT_SDD2 + NICE5000_INPUT_LOGIC;
			set_timer = mstimer + 12; 	//等待60ms
			send_nice5000_parameter(188, p.F5_X5);
			while(set_timer > mstimer)
				{
					cycle ();
				}
			set_timer = mstimer + 12; 	//等待60ms
			send_nice5000_parameter(189, p.F5_X6);
			while(set_timer > mstimer)
				{
					cycle ();
				}
		}
//强减3 
	if(value > 2500)
		{
			p.hse_inpar [INPUT_SDU3][IO_BASIC_FUNC] = FORCE_SWITCH;
			p.hse_inpar [INPUT_SDU3][IO_SUB_FUNC] = FORCE_SDU3;
			p.hse_inpar [INPUT_SDU3][IO_STATE] = POLARITY_BIT;
			p.hse_inpar [INPUT_SDU3][IO_ENABLE] = ENABLE;
			p.F5_X7 = NICE5000_INPUT_SDU3 + NICE5000_INPUT_LOGIC;
			p.hse_inpar [INPUT_SDD3][IO_BASIC_FUNC] = FORCE_SWITCH;
			p.hse_inpar [INPUT_SDD3][IO_SUB_FUNC] = FORCE_SDD3;
			p.hse_inpar [INPUT_SDD3][IO_STATE] = POLARITY_BIT;
			p.hse_inpar [INPUT_SDD3][IO_ENABLE] = ENABLE;
			p.F5_X8 = NICE5000_INPUT_SDD3 + NICE5000_INPUT_LOGIC;
			
			set_timer = mstimer + 12; 	//等待60ms
			send_nice5000_parameter(190, p.F5_X7);
			while(set_timer > mstimer)
				{
					cycle ();
				}
			set_timer = mstimer + 12; 	//等待60ms
			send_nice5000_parameter(191, p.F5_X8);
			while(set_timer > mstimer)
				{
					cycle ();
				}
		}
//限位开关	
	p.hse_inpar [INPUT_ILO][IO_BASIC_FUNC] = POS_IN;
	p.hse_inpar [INPUT_ILO][IO_SUB_FUNC] = POS_ILU;
	p.hse_inpar [INPUT_ILO][IO_STATE] = POLARITY_BIT;
	p.hse_inpar [INPUT_ILO][IO_ENABLE] = ENABLE;
	switchexist |= p.hse_inpar [INPUT_ILO][IO_SUB_FUNC];
	p.hse_inpar [INPUT_ILU][IO_BASIC_FUNC] = POS_IN;
	p.hse_inpar [INPUT_ILU][IO_SUB_FUNC] = POS_ILD;
	p.hse_inpar [INPUT_ILU][IO_STATE] = POLARITY_BIT;
	p.hse_inpar [INPUT_ILU][IO_ENABLE] = ENABLE;
	switchexist |= p.hse_inpar [INPUT_ILU][IO_SUB_FUNC];
	
	for (i = 0; i < MAX_HSE_IN + MAX_HSE_EX_IN; i++)
		{//读取输入状态
			if (i < MAX_HSE_IN)
				{
					if (p.hse_inpar [i][IO_BASIC_FUNC] && (p.hse_inpar [i][IO_ENABLE] == ENABLE))
						{
							for (j = 0; j < 5; j++)
								virt_in [j] = p.hse_inpar [i][j];
							state = bit_select (in_polarity, i) ^ bit_select (in, i);
							virt_in [IO_STATE] = state;
							handle_input (LIFT1, 1);					/* handle input 				*/
						}
				}	
			else
				{
					if (p.hse_ex_inpar [i - MAX_HSE_IN][IO_BASIC_FUNC] && (p.hse_ex_inpar [i - MAX_HSE_IN][IO_ENABLE] == ENABLE))
						{
							for (j = 0; j < 5; j++)
								virt_in [j] = p.hse_ex_inpar [i-MAX_HSE_IN][j];
							state = bit_select (in_polarity, i) ^ bit_select (in, i);
							virt_in [IO_STATE] = state;
							handle_input (LIFT1, 1);					/* handle input 				*/
						}
				}
		}
	set_timer = mstimer + 12; 	//等待60ms
	while(set_timer > mstimer)
		{
			cycle ();
		}
}

/****************************************************************************************************/
/* Teach in state																					*/
/****************************************************************************************************/
void teach_in_state (void)
{
	BYTE start = 0;
	BYTE sgm_state;
	BYTE floorset = 0;
	BYTE oldlevel;

	fault_signal = 0;
	out_of_order = 1;
	if(!p.simulation)
		set_shaft_switch();
	ct = timer + 2;
	while(ct > timer)
		{
			cycle ();
			set_state ();
		} 
	if (teach_in & TI_SHAFT)
	{
		if (((force_switch & (FORCE_SDU1 | FORCE_SDD1)) != FORCE_SDD1)
			|| ((force_switch & (FORCE_SDU1 | FORCE_SDD1)) == (FORCE_SDD1 | FORCE_SDU1))
			|| ((!state_msi) && (p.top_floor-p.bot_floor >= 2))
			|| ((state_msi) && (p.top_floor-p.bot_floor < 2)))		///只有2层的项目，必须脱开平层才可学习井道
		{
	  	teach_in = 0;
			teach_in_text = T_T_POSERROR;
			set_state ();
			return;
		}
	}
	send_hse_state ();
	calls_off = 1;
	clearcalls (ALL_CALLS);
	if (doorstate != ALL_DOORS_CLOSED)
	{
		door_command (CLOSE_DOOR, CLOSE_DOOR);
		ct = timer + 45 SEC;
		teach_in_text = T_T_DOORCL;
	}	
	
	while ((doorstate != ALL_DOORS_CLOSED) || (!floorset))
	{
		if ((!nice5000_Tstore_send) && (!floorset))
		{
			send_nice5000_parameter(NICE5000_RSTATE_CURRFLOOR, 1);
			niv.F4_01 = 1;
			floorset = 1;
		}
		if ((ct < timer) && (doorstate != ALL_DOORS_CLOSED))
		{
			teach_in = 0;
			teach_in_text = T_T_DOORERROR;
		}
		cycle ();
		set_state ();
		if (hse_state != H_TEACH_IN)
		{
			if (teach_in)
				teach_in_text = T_T_GEN_ERROR;
			teach_in = 0;
			return;
		}
	}

	p.setup_ready = 0;
	nice5000_Tcmd_data2 = (nice5000_Tcmd_data2 & (~(NICE5000_TCMD2_STATE|NICE5000_TCMD2_TARGET)))
							| TCMD2_STATE_SHAFT_TEACHIN;
	sgm_state = state_msi;
	ct = timer + p.triptime SEC;
	oldlevel = ~level;

	while (1)
	{
		cycle ();
		set_state ();
		if (level != oldlevel)
		{
			oldlevel = level;
			level_display(level, O_CANA | O_CANB);
		}
		if (hse_state != H_TEACH_IN)
		{
			if (drive_state != D_STOPPED)
			{
				emergency_stop ();
	  		hse_state = H_WAIT_STOP;
			}
			if (teach_in)
				teach_in_text = T_T_GEN_ERROR;
			teach_in = 0;
			return;
		}

		if (!(safety_circuit & SC3))
		{
			teach_in = 0;
			teach_in_text = T_T_SL_OFF;
		}
		else if ((ct < timer) && (start))
		{
			teach_in = 0;
			teach_in_text = T_T_TIME;
		}
		if ((!start) && ((nice5000_Rcmd_data2 & NICE5000_RCMD2_STATE) == TCMD2_STATE_SHAFT_TEACHIN))
		{
			start_drive (DIR_UP);
			direction = DIR_UP;						/* set direction							*/
			direction_ind = direction | (direction << 4);		/* when travelling use travel direction		*/
			teach_in_text = T_T_TOP;
			start = 1;
		}
		else if (start)
		{
			if (sgm_state != state_msi)
			{
				sgm_state = state_msi;
				ct = timer + p.triptime SEC;
			}
			if (drive_state == D_STOPPED)
			{
				p.setup_ready = 1;
				init_error |= B_DRV;
				bForceResetTrip = 0;
				p.canbus_check = 0;
				teach_in = 0;
				teach_in_text = T_T_READY;
				set_state ();
				return;
			}
		}
		else
			ct = timer + p.triptime SEC;
	}
}
