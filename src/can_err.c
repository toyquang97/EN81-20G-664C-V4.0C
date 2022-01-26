
#include "typedef.h"
#include "parameter.h"
#include "state.h"
#include "call.h"
#include "can.h"
#include "history.h"
#include "io.h"

//**************************************************************************************************
// clear CAN A RX buffer
//**************************************************************************************************
void clear_rxa_buffer (void)
{
	BYTE i, j;
	for (i = 0; i < RXASW_SIZE; i++)
	{
		for (j = 0; j < 10; j++)
			rxa [i][j] = 0;
	}
	rca = 0;													// RX message counter
	roa = 0;												// pointer for RX FIFO read
	ria = 0;												// pointer for RX FIFO write
}
//**************************************************************************************************
// clear CAN A SX buffer
//**************************************************************************************************
void clear_sxa_buffer (void)
{
	BYTE i, j;
	for (i = 0; i < SXASW_SIZE; i++)
	{
		for (j = 0; j < 10; j++)
			sxa [i][j] = 0;
	}
	sca = 0;													// RX message counter
	soa = 0;												// pointer for RX FIFO read
	sia = 0;												// pointer for RX FIFO write
}

//**************************************************************************************************
// clear CAN A TX buffer
//**************************************************************************************************
void clear_txa_buffer (void)
{
	BYTE i, j;
	for (i = 0; i < TXASW_SIZE; i++)
	{
		for (j = 0; j < 10; j++)
			txa [i][j] = 0;
	}
	tca = 0;												// TX message counter
	toa = 0;												// pointer for TX FIFO read
	tia = 0;												// pointer for TX FIFO write
	tra = 1;											// TX ready

	for (i = 0; i < SXASW_SIZE; i++)
	{
		for (j = 0; j < 10; j++)
			sxa [i][j] = 0;
	}
	sca = 0;												// TX message counter
	soa = 0;												// pointer for TX FIFO read
	sia = 0;												// pointer for TX FIFO write
	sra = 1;											// TX ready
}

//**************************************************************************************************
// can error state
//**************************************************************************************************
void can_error_state (void)
{
	DWORD errortimer;
	BYTE wait_time_over = 0;
	BYTE errrst = 0;

	errortimer = timer + (p.errortime SEC);			// start error delay timer
	cycle ();
	ct = timer + 2 SEC; 							// wait 1 second
//错误状态下清除双门功能   2016-12-08	
	she_calldoor_double = 0;
//错误状态下清除双门功能	 2016-12-08 

	while (1)
	{
		if (ct < timer)
		{
			if ((canerrorstate & (TXA_SW_OVERRUN | RXA_SW_OVERRUN | CANA_BUSOFF | SXA_SW_OVERRUN))
						|| (hb_error & B_CAN))
			{
				if (!errrst)
				{
					if(canerrorstate & CANA_BUSOFF)
						tea = 0;
					clear_txa_buffer ();
					clear_rxa_buffer ();
					clear_sxa_buffer ();
					canatxerror = 0;
					init_can_a ();
					cana_busoff_checktime = timer + 3 SEC;
					errrst = 1;
				}
				else
				{
					if (!(CAN1GSR & 0x80))
						canerrorstate &= ~CANA_BUSOFF;				// reset CAN A error
					else if (power24v)
						send_heartbeat_a();
					if (!(CAN1GSR & 0x02))
						canerrorstate &= ~RXA_HW_OVERRUN;
					if (rca < RXASW_SIZE)
						canerrorstate &= ~RXA_SW_OVERRUN;
					if (sca < SXASW_SIZE)
						canerrorstate &= ~SXA_SW_OVERRUN;
					if(canatxerror < TXERRORLIMIT)
						hb_error &= ~B_CAN;
					canerrorstate &= ~(TXA_SW_OVERRUN | CANA_PCB);

					errrst = 0;
				}
			}
			else											// no new CAN error
			{
				canatxerror = 0;
				tea = 1;									// enable CAN A
				hb_error &= ~B_CAN;							// reset heartbeat can errors (but not devices)
				heartbeat = 0xFF;
				search_state ();							// active state search
				set_state ();								// set new state
				if (hse_state != H_CAN_ERROR)							// state changed
				{
					return;
				}
			}
			ct = timer + 2 SEC;								// wait again 1 second
		}
		cycle ();
		if (!wait_time_over)							// error wait time not over
		{
			if (errortimer < timer)						// time over
			{
				calls_off = 1;								// disable calls
				clearcalls (ALL_CALLS);								// clear all calls
				wait_time_over = 1;
				fault_signal = 1;							// set fault signal
				out_of_order = 1;							// set out of order indication
			}
		}		
		fan_on ();
	}
}

