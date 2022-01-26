
#include "typedef.h"
#include "parameter.h"
#include "nice5000.h"
#include "state.h"
#include "drive.h"


/****************************************************************************************************/
/* set or reset mains contactor																		*/
/****************************************************************************************************/
void set_contactor (BYTE type, BYTE state)
{
	BYTE i;

	for (i = 0; i < MAX_HSE_OUT; i++)						/* search output parameter list				*/
	{
		if (p.hse_outpar [i][IO_BASIC_FUNC] == DRIVE_OUT)	/* output is drive output					*/
		{
			if ((p.hse_outpar [i][IO_SUB_FUNC] == type) ||	/* type of mains contactor					*/
						(!type))									/* switch all drive outputs					*/
			{
				if (state)								/* switch contactor on						*/
				{
					bit_set (out, i);
				}
				else
				{
					bit_reset (out, i);
				}
			}
		}
	}
	for (i = 0; i < MAX_HSE_EX_OUT; i++)						/* search output parameter list				*/
	{
		if (p.hse_ex_outpar [i][IO_BASIC_FUNC] == DRIVE_OUT)	/* output is drive output					*/
		{
			if ((p.hse_ex_outpar [i][IO_SUB_FUNC] == type) ||	/* type of mains contactor					*/
						(!type))									/* switch all drive outputs					*/
			{
				if (state)								/* switch contactor on						*/
				{
					bit_set (out, (i+MAX_HSE_OUT));
				}
				else
				{
					bit_reset (out, (i+MAX_HSE_OUT));
				}
			}
		}
	}
}

/****************************************************************************************************/
/* start drive																						*/
/****************************************************************************************************/
void start_drive (BYTE drv_direction){
	if ((!driveerror) && (!(hb_error & B_DRV)))				/* no drive error and no heartbeat error	*/
		{
			direction = drv_direction;						/* set direction							*/
			gong_direction = drv_direction;				//设置到站钟方向
			drive_state = D_START_MOVING;				/* set new drive state						*/
			nice5000_startpos = nice5000_Rstore_position;

			check_sdd1_working_status = 1;
			check_sdu1_working_status = 1;
			check_sds_working_status = 1;
			
			nice5000_Tcmd_data1 &= ~(NICE5000_TCMD1_RUN | NICE5000_TCMD1_DOWN | NICE5000_TCMD1_UP
													| NICE5000_TCMD1_BRAKE | NICE5000_TCMD1_SPECIALDEC);
		}
}

/****************************************************************************************************/
/* stop drive immediately																			*/
/****************************************************************************************************/
void emergency_stop (void)
{
	nice5000_Tcmd_data1 &= ~(NICE5000_TCMD1_RUN | NICE5000_TCMD1_DOWN | NICE5000_TCMD1_UP
													| NICE5000_TCMD1_BRAKE | NICE5000_TCMD1_SPECIALDEC);
	drive_state = D_STOPPED;
	set_contactor (0, 0);								/* switch off all contactor					*/
}

/****************************************************************************************************/
/* stop drive in normal mode																		*/
/****************************************************************************************************/
void stop_drive (void)
{
	if ((drive_state != D_STOPPED)  &&					/* drive not in stopped state				*/
	(drive_state != D_STOPPING) &&						/* drive not in stopping state				*/
	(drive_state != D_OPERATION_DISABLED) &&			/* drive not in operation disabled state	*/
	(drive_state != D_SWITCHED_OFF) &&				/* drive not in switched off state			*/
	(drive_state != D_WAIT_STOP))						/* drive not in waiting for stop state		*/
	{
		nice5000_Tcmd_data1 |= NICE5000_TCMD1_SPECIALDEC;
	}
}

