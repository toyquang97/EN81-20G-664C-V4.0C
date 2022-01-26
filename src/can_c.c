
#include "typedef.h"
#include "lpc1700.h"
#include "parameter.h"
#include "canopen.h"
#include "can.h"
#include "mcp2515.h"
#include "state.h"
#include "call.h"
#include "pos.h"
#include "dict.h"
#include "io.h"
#include "history.h"


void init_can_c(void)
{
	mcp2515_can_init();
}

//***************************************************
// 函数名: ReadCanC
// 功能: 处理CAN C上的一条消息
// 入参: 无
// 出参: 无
// 返回值: 无
//***************************************************
void ReadCanC(void)
{
	BYTE i, j;

	switch (rxc[roc][0])								// function code of identifier
	{
		case (PDO_OUT):
			if (!rxc [roc][IO_STATE + 2])					// state is off
			{
				switch (rxc [roc][IO_BASIC_FUNC + 2])
				{
					case (HALL_CALL):
						i = rxc [roc][IO_FLOOR + 2];			// read floor number
						if (i && (i != 0xFF))
						{
							i --;
							j = rxc [roc][IO_DOOR + 2]; 		// read doors
							switch (rxc [roc][IO_SUB_FUNC + 2])
							{
								case (HALL_CALL_UP):
									calltab [i].lcu_door &= ~j; 	// clear all open doors
									if (!calltab [i].lcu_door)		// if all doors open clear call
										calltab [i].calltype &= ~LANDINGCALL_UP;
									break;
								case (HALL_CALL_DOWN):
									calltab [i].lcd_door &= ~j; 	// clear all open doors
									if (!calltab [i].lcd_door)		// if all doors open clear call
										calltab [i].calltype &= ~LANDINGCALL_DN;
									break;
								case (HALL_CALL_NO_DIR):
									calltab [i].lc_door &= ~j;		// clear all open doors
									if (!calltab [i].lc_door)			// if all doors open clear call
										calltab [i].calltype &= ~LANDINGCALL;
									break;
							}
						}
						break;
						
					case (HALL_CALL_SPECIAL):					// special hall call
					case (HALL_CALL_ADVANCED):				// advanced hall call
					case (HALL_CALL_EMERGENCY): 			// emergency hall call
						i = rxc [roc][IO_SUB_FUNC + 2]; 		// read floor number
						if (i && (i != 0xFF))
						{
							i--;
							j = rxc [roc][IO_DOOR + 2]; 		// read doors
							switch (rxc [roc][IO_BASIC_FUNC + 2])
							{
								case (HALL_CALL_SPECIAL):			// special hall call
									calltab [i].sc_door &= ~j;		// clear all open doors
									if (!calltab [i].sc_door)			// if all doors open clear call
										calltab [i].calltype &= ~SPECIAL_CALL;
									break;
								case (HALL_CALL_ADVANCED):			// advanced hall call
									calltab [i].ac_door &= ~j;		// clear all open doors
									if (!calltab [i].ac_door)			// if all doors open clear call
										calltab [i].calltype &= ~ADVANCED_CALL;
									break;
								case (HALL_CALL_EMERGENCY): 		// emergency hall call
									calltab [i].ec_door &= ~j;		// clear all open doors
									if (!calltab [i].ec_door)			// if all doors open clear call
										calltab [i].calltype &= ~EMERGENCY_CALL;
									break;
							}
						}
						break;
				}
				set_out (rxc[roc][IO_BASIC_FUNC+2], rxc[roc][IO_SUB_FUNC+2], rxc[roc][IO_FLOOR+2], rxc[roc][IO_DOOR+2], 0, O_CANA|O_CANB|O_HSE);
			}
			break;

		case (PDO_IN):
			for (i = 0; i < MAX_IO_TYPE; i++) 					// read input function
				virt_in [i] = rxc [roc][i + 2]; 			// write to virtual input mapping
			if(virt_in[IO_ENABLE] == ENABLE)
				handle_input (LIFT1, 1);			// handle input
			break;
	}

	if (roc >= (RXCSW_SIZE - 1))
		roc = 0;
	else
		roc ++;
	rcc --;
}

//**************************************************************************************************
// check transmit buffer
//**************************************************************************************************
BYTE check_txc (void)
{
	if (!tec)											// TX not enabled
		return(0);
	if (tcc >= TXCSW_SIZE)								// TX software buffer overrun
	{
		if (!(canerrorstate & TXC_SW_OVERRUN))
		{
			canerrorstate |= TXC_SW_OVERRUN;			// set CAN error state
			canc_error_count += 5;
			write_errorhist (E_CAN_TX_SW_C, HSE_ID, 0, W_TXC_OVERRUN);
		}
		return(0);										// buffer overrun
	}
	return(1);											// buffer empty
}

//***************************************************
// 函数名: WriteCanC
// 功能: 往CAN C上发送一条消息
// 入参: 
// 出参: 无
// 返回值: 无
//***************************************************
void WriteCanC(void)
{
	BYTE len, i, tmpbuff[10];

	if (tic >= (TXCSW_SIZE - 1))
		tic = 0;
	else
		tic ++;
	if (trc)
	{
		CLRENA0 = ISE_EINT1;
		len = txc[toc][0] & 0x0f;
		tmpbuff[0] = (txc[toc][0] & 0xf0) | (txc[toc][1] >> 3);
		tmpbuff[1] = (txc[toc][1] << 5);
		for (i=0; i<len; ++i)
			tmpbuff[2 + i] = txc[toc][2 + i];
		mcp2515_send(MCP_TXB0SIDH, len, tmpbuff);
		mcp2515_modify_reg(MCP_TXB0CTRL, MCP_TXB_TXREQ_M, MCP_TXB_TXREQ_M);

		trc = 0;
		if (toc >= (TXCSW_SIZE - 1))
			toc = 0;
		else
			toc ++;
		SETENA0 = ISE_EINT1;
	}
	else
		tcc ++;	
}

//**************************************************************************************************
// Send heartbeat on CAN bus C
//**************************************************************************************************
void send_heartbeat_c(void){
	BYTE tmpbuff[10];

	if (check_txc ())
	{
		tmpbuff[0] = HEARTBEAT | ((p.groupnumber + 1) >> 3);
		tmpbuff[1] = (p.groupnumber + 1) << 5;
		*((DWORD *)&tmpbuff[2]) = ((DWORD)next_calltype << 24) | OPERATIONAL;
		*((DWORD *)&tmpbuff[6]) = ((DWORD)next_calltime << 16) | ((DWORD)next_calldoors << 8) | ((DWORD)next_callfloor + 1);
		
		CLRENA0 = ISE_EINT1;
		mcp2515_send(MCP_TXB1SIDH, 8, tmpbuff);
		mcp2515_modify_reg(MCP_TXB1CTRL, 
		MCP_TXB_TXREQ_M | MCP_TXB_TXBUFE_M, 
		MCP_TXB_TXREQ_M | MCP_TXB_TXBUFE_M);		//使用高优先级
		SETENA0 = ISE_EINT1;
	}
}

//**************************************************************************************************
// Set outputs on CAN bus C (group bus)
//**************************************************************************************************
void set_out_c (BYTE func, BYTE sub_func, BYTE floor, BYTE doors, BYTE state)
{
	if (check_txc ())									// transmit buffer empty
	{
		txc [tic][0] = PDO_OUT | 6;						// write Function code and data length
		txc [tic][1] = (1 << p.groupnumber);			// write node ID
		txc [tic][IO_BASIC_FUNC + 2] = func;				// write output function
		txc [tic][IO_SUB_FUNC + 2] = sub_func;			// write output sub function
		txc [tic][IO_LIFT + 2] = (1 << p.groupnumber);
		txc [tic][IO_FLOOR + 2] = floor;					// write floor number
		txc [tic][IO_DOOR + 2] = doors;					// write door numbers
		txc [tic][IO_STATE + 2] = state;					// write state
		WriteCanC ();									// transmit message and set FIFO pointer
	}
}

void set_hallcall_c(void)
{
	BYTE i;

	if (check_txc ())
	{
		txc [tic][0] = PDO_IN | MAX_IO_TYPE;						// write Function code and data length
		txc [tic][1] = (1 << p.groupnumber);				// write node ID
		for (i=0; i<MAX_IO_TYPE; ++i)
			txc [tic][i + 2] = virt_in[i];
		WriteCanC ();									// transmit message and set FIFO pointer
	}
}

//**************************************************************************************************
// clear CAN C RX buffer
//**************************************************************************************************
void clear_rxc_buffer (void){
	BYTE i, j;
	for (i = 0; i < RXCSW_SIZE; i++)
		{
			for (j = 0; j < 10; j++)
				rxc [i][j] = 0;
		}
	rcc = 0;													// RX message counter
	roc = 0;												// pointer for RX FIFO read
	ric = 0;												// pointer for RX FIFO write
}
//**************************************************************************************************
// clear CAN C TX buffer
//**************************************************************************************************
void clear_txc_buffer (void){
	BYTE i, j;
	for (i = 0; i < TXCSW_SIZE; i++)
		{
			for (j = 0; j < 10; j++)
				txc [i][j] = 0;
		}
	tcc = 0;												// TX message counter
	toc = 0;												// pointer for TX FIFO read
	tic = 0;												// pointer for TX FIFO write
	trc = 1;											// TX ready
	tec = 1;
}

void gse (void)
{
	static BYTE gse_state = GSE_WATCH;
	static DWORD gse_timer;

	switch (gse_state)
	{
		case (GSE_WATCH):
			if (canerrorstate & CANC_MASK)
				{
					gse_state = GSE_BUS_OFF;
					gse_timer = timer + 2 SEC;
					tec = 0;
				}
			else
				tec = 1;
			break;
			
		case (GSE_BUS_OFF):
			if (gse_timer < timer)
			{
				rcc = 0;
				roc = 0;
				ric = 0;
				canc_busoff_flag = 0;
				clear_txc_buffer ();
				clear_rxc_buffer ();
				init_can_c ();
				gse_timer = timer + 5 SEC;
				gse_state = GSE_WAIT_CAN;
			}
			break;
			
		case (GSE_WAIT_CAN):
			if (gse_timer < timer)
			{
				if (!canc_busoff_flag)
				{
					canerrorstate &= ~CANC_MASK;
					tec = 1;
					gse_state = GSE_WATCH;
				}
				else
				{
					gse_state = GSE_BUS_OFF;
					gse_timer = timer + 1 SEC;
				}
			}
			break;
			
		default:
			gse_state = GSE_WATCH;
			break;
	}
}

