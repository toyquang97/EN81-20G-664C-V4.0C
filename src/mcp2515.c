
#include "typedef.h"
#include "lpc1700.h"
#include "mcp2515.h"
#include "parameter.h"
#include "state.h"
#include "history.h"
#include "call.h"
#include "nice5000.h"


/****************************************************
* 函数名: WriteSpiByte
* 功能: SPI发送一个字节
* 入参: ch 待发送的数据
* 出参: 无
* 返回值: 无
****************************************************/
void WriteSpiByte (BYTE ch)
{
	SSP0DR = ch;
	while (SSP0SR & SSP_SR_BSY);
	ch = SSP0DR;
}

/****************************************************
* 函数名: ReadSpiByte
* 功能: SPI读取一个字节
* 入参: 无
* 出参: 无
* 返回值: 读到的数据
****************************************************/
BYTE ReadSpiByte (void)
{
	SSP0DR = 0xFF;
	while (SSP0SR & SSP_SR_BSY);
	return (BYTE)SSP0DR;
}

// reset MCP2515
void mcp2515_reset(void)
{
	MCP_CS_OFF();
	delay(10);
	MCP_CS_ON();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	WriteSpiByte(MCP_RESET);
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	MCP_CS_OFF();
	delay(50);
}

// set MCP2515 configurate BYTE
void mcp2515_config_rate(void)
{
	MCP_CS_ON();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	WriteSpiByte(MCP_WRITE);
	WriteSpiByte(MCP_CNF3);
	WriteSpiByte(MCP_12M_125K_CF3);
	WriteSpiByte(MCP_12M_125K_CF2);
	WriteSpiByte(MCP_12M_125K_CF1);
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	MCP_CS_OFF();
	delay(5);
}

// get MCP2515 state
BYTE mcp2515_read_status(void)
{
	BYTE status;

	MCP_CS_ON();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	WriteSpiByte(MCP_READ_STATUS);
	status = ReadSpiByte();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	MCP_CS_OFF();
	return status;
}
void mcp2515_read_regs(BYTE addr, BYTE * buff, BYTE size)
{
	MCP_CS_ON();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	WriteSpiByte(MCP_READ);
	WriteSpiByte(addr);
	while(size --)
	{
		*buff ++ = ReadSpiByte();
	}
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	MCP_CS_OFF();
}

void mcp2515_write_regs(BYTE addr, BYTE * buff, BYTE size)
{
	MCP_CS_ON();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	WriteSpiByte(MCP_WRITE);
	WriteSpiByte(addr);
	while(size --)
	{
		WriteSpiByte(*buff ++);
	}
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	MCP_CS_OFF();
}

// modify MCP2515 register
void mcp2515_modify_reg(BYTE addr, BYTE mask, BYTE data)
{
	MCP_CS_ON();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	WriteSpiByte(MCP_BITMOD);
	WriteSpiByte(addr);
	WriteSpiByte(mask);
	WriteSpiByte(data);
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	MCP_CS_OFF();
}

void mcp2515_recv(BYTE addr, BYTE * len, BYTE * buffer)
{
	BYTE size;

	MCP_CS_ON();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	WriteSpiByte(MCP_READ);
	WriteSpiByte(addr);
	*buffer ++ = ReadSpiByte();
	*buffer ++ = ReadSpiByte();
	ReadSpiByte();
	ReadSpiByte();
	size = ReadSpiByte() & 0x0f;
	*len = size;
	while(size --)
		*buffer ++ = ReadSpiByte();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	MCP_CS_OFF();
}

void mcp2515_send(BYTE addr, BYTE len, BYTE * buffer)
{
	MCP_CS_ON();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	WriteSpiByte(MCP_WRITE);
	WriteSpiByte(addr);
	WriteSpiByte(*buffer ++);
	WriteSpiByte(*buffer ++);
	WriteSpiByte(0x00);
	WriteSpiByte(0x00);
	WriteSpiByte(len);
	while(len --)
		WriteSpiByte(*buffer ++);
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	MCP_CS_OFF();
}

const DWORD c_mcp2515_buffers_init[][2] =
{
	{MCP_RXM0SIDH, 0x00},
	{MCP_RXM1SIDH, 0x00},
	{MCP_RXF0SIDH, 0x00},
	{MCP_RXF1SIDH, 0x00},
	{MCP_RXF2SIDH, 0x00},
	{MCP_RXF3SIDH, 0x00},
	{MCP_RXF4SIDH, 0x00},
	{MCP_RXF5SIDH, 0x00},
};

// Init MCP2515
void mcp2515_can_init(void){
	BYTE i;

	mcp2515_reset();
	mcp2515_modify_reg(MCP_CANCTRL, MODE_MASK, MODE_CONFIG);		//进入配置模式
	for (i=0; i<sizeof(c_mcp2515_buffers_init)/(2*sizeof(DWORD)); ++i)
	{
		MCP_CS_ON();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		WriteSpiByte(MCP_WRITE);
		WriteSpiByte(c_mcp2515_buffers_init[i][0]);				//发送地址
		WriteSpiByte(c_mcp2515_buffers_init[i][1]);				//发送4字节数据
		WriteSpiByte(c_mcp2515_buffers_init[i][1] >> 8);
		WriteSpiByte(c_mcp2515_buffers_init[i][1] >> 16);
		WriteSpiByte(c_mcp2515_buffers_init[i][1] >> 24);
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		MCP_CS_OFF();
	}
	mcp2515_config_rate();			//设置波特率,默认为125K
	mcp2515_modify_reg(MCP_CANINTE, 0xff, MCP_INT_MASK);				//使能中断
	mcp2515_modify_reg(MCP_CANCTRL, MODE_MASK, MODE_NORMAL);	//返回正常模式
}

// Interrupt routine
void EINT1_IRQHandler(void)
{
	BYTE intf, eflg, len, i, tmpbuff[10];

	CLRPEND0 = ISE_EINT1;
	EXTINT = (0x01 << 1);
	
	mcp2515_read_regs(MCP_CANINTF, &intf, 1);
	if (intf & RX0IE)
	{
		if (rcc == RXCSW_SIZE)
		{
			if (!(canerrorstate & RXC_SW_OVERRUN))
			{
				canerrorstate |= RXC_SW_OVERRUN;
				canc_error_count += 5;
				write_errorhist (E_CAN_OV_SW_C, HSE_ID, 0, W_RXC_SW_OVERRUN);
			}
		}
		else
		{
			mcp2515_recv(MCP_RXB0SIDH, &len, tmpbuff);
			rxc[ric][0] = tmpbuff[0] & 0xf0;
			rxc[ric][1] = ((tmpbuff[0] & 0x0f) << 3) | (tmpbuff[1] >> 5);
			for (i=0; i<len; ++i)
				rxc[ric][2 + i] = tmpbuff[2 + i];
			if (rxc[ric][0] == HEARTBEAT)
			{
				i = rxc[ric][1];
				if ((i >= HSE_ID) && (i < (HSE_ID + MAX_GR)))
				{
					groupcall [i - 1].type = rxc[ric][5];
					groupcall [i - 1].floor = rxc[ric][6] - 1;
					groupcall [i - 1].doors = rxc[ric][7];
					groupcall [i - 1].time  = *(WORD *)&rxc[ric][8];
					hse_hb |= (1 << (i - 1));
				}
			}
			else
			{
				if (ric == (RXCSW_SIZE -1)) 
					ric = 0;
				else
					ric ++;
				rcc++;
			}
		}
		intf &= ~RX0IE;
	}
	if (intf & RX1IE)
	{
		if (rcc == RXCSW_SIZE)
		{
			if (!(canerrorstate & RXC_SW_OVERRUN))
			{
				canerrorstate |= RXC_SW_OVERRUN;
				canc_error_count += 5;
				write_errorhist (E_CAN_OV_SW_C, HSE_ID, 0, W_RXC_SW_OVERRUN);
			}
		}
		else
		{
			mcp2515_recv(MCP_RXB1SIDH, &len, tmpbuff);
			rxc[ric][0] = tmpbuff[0] & 0xf0;
			rxc[ric][1] = ((tmpbuff[0] & 0x0f) << 3) | (tmpbuff[1] >> 5);
			for (i=0; i<len; ++i)
				rxc[ric][2 + i] = tmpbuff[2 + i];

			if (rxc[ric][0] == HEARTBEAT)
			{
				i = rxc[ric][1];
				if ((i >= HSE_ID) && (i < (HSE_ID + MAX_GR)))
				{
					groupcall [i - 1].type = rxc[ric][5];
					groupcall [i - 1].floor = rxc[ric][6] - 1;
					groupcall [i - 1].doors = rxc[ric][7];
					groupcall [i - 1].time  = *(WORD *)&rxc[ric][8];
					hse_hb |= (1 << (i - 1));
				}
			}
			else
			{
				if (ric == (RXCSW_SIZE - 1)) 
					ric = 0;
				else
					ric ++;
				rcc++;
			}
		}
		intf &= ~RX1IE;
	}
	if (intf & TX0IE)
	{
		if (tcc)
		{
			len = txc[toc][0] & 0x0f;
			tmpbuff[0] = (txc[toc][0] & 0xf0) | (txc[toc][1] >> 3);
			tmpbuff[1] = (txc[toc][1] << 5);
			for (i=0; i<len; ++i)
				tmpbuff[2 + i] = txc[toc][2 + i];
			mcp2515_send(MCP_TXB0SIDH, len, tmpbuff);
			mcp2515_modify_reg(MCP_TXB0CTRL, MCP_TXB_TXREQ_M, MCP_TXB_TXREQ_M);
			-- tcc;
			if (toc == (TXCSW_SIZE - 1))
				toc = 0;
			else
				toc++;
		}
		else
		{
			trc = 1;
		}
		intf &= ~TX0IE;
	}
	if (intf & (ERRIE | MERRE))
	{
		mcp2515_read_regs(MCP_EFLG, &eflg, 1);
		if (eflg & (RX1OV | RX0OV))
		{
			if (!(canerrorstate & RXC_HW_OVERRUN))
			{
				canerrorstate |= RXC_HW_OVERRUN;
				canc_error_count ++;
				write_errorhist (E_CAN_OVERRUN_C, HSE_ID, 0, W_RXC_HW_OVERRUN);
			}
		}
		if (eflg & TXBO)
		{
			canc_busoff_flag = 1;
			if (!(canerrorstate & CANC_BUSOFF))
			{
				canerrorstate |= CANC_BUSOFF;
				canc_error_count += 5;
				write_errorhist (E_BUS_OFF_C, HSE_ID, 0, W_BUSC_OFF);
			}
		}
		
		intf &= ~(ERRIE|MERRE);
	}
	mcp2515_write_regs(MCP_CANINTF, &intf, 1);		//清除中断标志
}

