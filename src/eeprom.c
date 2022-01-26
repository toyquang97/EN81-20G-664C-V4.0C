
#include "typedef.h"
#include "lpc1700.h"
#include "lpc1700pincfg.h"
#include "string.h"
#include "parameter.h"
#include "eeprom.h"
#include "test.h"
#include "io.h"
#include "i2c.h"
#include "rtc.h"

BYTE iic_busy = 0;
BYTE iic_write_error = 0;
BYTE iic_read_error = 0;
DWORD eeprom_count = 0;			// write counter for EEPROM	
struct EE_WRITE_QUEUE ee_write_queue;


void SysTick_Handler (void)
{
	DWORD addr, dly;
	BYTE * ptr;

	addr = STCTRL;
	if (ee_write_queue.count == 0)
	{
		STCTRL = 0x04;
		iic_busy = 0;
	}
	else if ((!rtc_iic_doing) && (!check_para_doing))
	{
		if (eeprom_count >= ee_write_queue.rec[ee_write_queue.read_rec].size)
		{
			eeprom_count = 0x00;
			-- ee_write_queue.count;
			++ ee_write_queue.read_rec;
			if (ee_write_queue.read_rec == EE_MAX_WRITERECORD)
				ee_write_queue.read_rec = 0;
		}
		else
		{
			addr = ee_write_queue.rec[ee_write_queue.read_rec].address + eeprom_count;
			ptr = ee_write_queue.rec[ee_write_queue.read_rec].pointer;

			I2C0CONCLR = 0x2c;
			I2C0CONSET = 0x60;
			dly = IIC_RSP_DLY;
			while (dly --)
			{
				if ((I2C0STAT & 0xf8) == 0x08)
					break;
			}
			if (dly == 0)
			{
				I2C0CONSET = 0x10;
				I2C0CONCLR = 0x28;
				iic_write_error = 0x01;
				return;
			}
			I2C0CONCLR = 0x28;
			I2C0DAT = IIC_ADDR_EEPROM;
			dly = IIC_RSP_DLY;
			while (dly --)
			{
				if ((I2C0STAT & 0xf8) == 0x18)
					break;
			}
			if (dly == 0)
			{
				I2C0CONSET = 0x10;
				I2C0CONCLR = 0x28;
				iic_write_error = 0x01;
				return;
			}
			I2C0DAT = addr >> 8;
			I2C0CONCLR = 0x28;
			dly = IIC_RSP_DLY;
			while (dly --)
			{
				if ((I2C0STAT & 0xf8) == 0x28)
					break;
			}
			if (dly == 0)
			{
				I2C0CONSET = 0x10;
				I2C0CONCLR = 0x28;
				iic_write_error = 0x01;
				return;
			}
			I2C0DAT = addr & 0xff;
			I2C0CONCLR = 0x28;
			dly = IIC_RSP_DLY;
			while (dly --)
			{
				if ((I2C0STAT & 0xf8) == 0x28)
					break;
			}
			if (dly == 0)
			{
				I2C0CONSET = 0x10;
				I2C0CONCLR = 0x28;
				iic_write_error = 0x01;
				return;
			}

			do
			{
				I2C0DAT = *ptr ++;
				I2C0CONCLR = 0x28;
				dly = IIC_RSP_DLY;
				while (dly --)
				{
					if ((I2C0STAT & 0xf8) == 0x28)
						break;
				}
				if (dly == 0)
				{
					I2C0CONSET = 0x10;
					I2C0CONCLR = 0x28;
					iic_write_error = 0x01;
					return;
				}				
				++ eeprom_count;
				if (eeprom_count >= ee_write_queue.rec[ee_write_queue.read_rec].size)
					break;
				++ addr;
			}
			while (addr % EE_BLOCK_SIZE);
			I2C0CONSET = 0x10;
			I2C0CONCLR = 0x28;
			ee_write_queue.rec[ee_write_queue.read_rec].pointer = ptr;
		}
	}
}


/****************************************************
* 函数名: ReadEEData
* 功能: 向EEPROM读取size个字节
* 入参: address EEPROM地址; size 读数据字节数
* 出参: pointer 目标缓冲区
* 返回值: 0 成功；非0: 失败
****************************************************/
BYTE ReadEEData (void * pointer, WORD address, WORD size)
{
	volatile DWORD dly;
	BYTE * ptr = pointer;

	if (size)
	{
		iic_busy = 1;

		I2C0CONCLR = 0x2c;
		I2C0CONSET = 0x60;
		dly = IIC_RSP_DLY;
		while (dly --)
		{
			if ((I2C0STAT & 0xf8) == 0x08)
				break;
		}
		if (dly == 0)
		{
			I2C0CONSET = 0x10;
			I2C0CONCLR = 0x28;
			iic_busy = 0;
			iic_read_error = 1;
			return 0x01;
		}
		I2C0CONCLR = 0x28;
		I2C0DAT = IIC_ADDR_EEPROM;
		dly = IIC_RSP_DLY;
		while (dly --)
		{
			if ((I2C0STAT & 0xf8) == 0x18)
				break;
		}
		if (dly == 0)
		{
			I2C0CONSET = 0x10;
			I2C0CONCLR = 0x28;
			iic_busy = 0;
			iic_read_error = 1;
			return 0x01;
		}
		I2C0DAT = address >> 8;
		I2C0CONCLR = 0x28;
		dly = IIC_RSP_DLY;
		while (dly --)
		{
			if ((I2C0STAT & 0xf8) == 0x28)
				break;
		}
		if (dly == 0)
		{
			I2C0CONSET = 0x10;
			I2C0CONCLR = 0x28;
			iic_busy = 0;
			iic_read_error = 1;
			return 0x02;
		}
		I2C0DAT = address & 0xff;
		I2C0CONCLR = 0x28;
		dly = IIC_RSP_DLY;
		while (dly --)
		{
			if ((I2C0STAT & 0xf8) == 0x28)
				break;
		}
		if (dly == 0)
		{
			I2C0CONSET = 0x10;
			I2C0CONCLR = 0x28;
			iic_busy = 0;
			iic_read_error = 1;
			return 0x03;
		}

		I2C0CONCLR = 0x2c;
		I2C0CONSET = 0x60;
		dly = IIC_RSP_DLY;
		while (dly --)
		{
			if ((I2C0STAT & 0xf8) == 0x08)
				break;
		}
		if (dly == 0)
		{
			I2C0CONSET = 0x10;
			I2C0CONCLR = 0x28;
			iic_busy = 0;
			iic_read_error = 1;
			return 0x04;
		}
		I2C0CONCLR = 0x28;
		I2C0DAT = 0xA1;
		dly = IIC_RSP_DLY;
		while (dly --)
		{
			if ((I2C0STAT & 0xf8) == 0x40)
				break;
		}
		if (dly == 0)
		{
			I2C0CONSET = 0x10;
			I2C0CONCLR = 0x28;
			iic_busy = 0;
			iic_read_error = 1;
			return 0x05;
		}
		if (size > 1)
		{
			I2C0CONSET = 0x04;
		}
		else
		{
			I2C0CONCLR = 0x04;
		}
		I2C0CONCLR = 0x28;
		while (size --)
		{
			dly = IIC_RSP_DLY;
			while (dly --)
			{
				if ((I2C0STAT & 0xf8) == 0x50)
					break;
				if ((I2C0STAT & 0xf8) == 0x58)
					break;
			}
			if (dly == 0)
			{
				I2C0CONSET = 0x10;
				I2C0CONCLR = 0x28;
				iic_busy = 0;
				iic_read_error = 1;
				return 0x06;
			}
	
			*ptr ++ = I2C0DAT;
			if (size == 0)
				break;
			else if (size > 1)
			{
				I2C0CONSET = 0x04;
				I2C0CONCLR = 0x28;
			}
			else
			{
				I2C0CONCLR = 0x2c;
			}
		}
		I2C0CONSET = 0x10;
		I2C0CONCLR = 0x28;
		iic_busy = 0;
	}
	return 0x00;
}

//***************************************************
// 函数名: WriteEEData
// 功能: 向EEPROM写入size个字节
// 入参: address EEPROM地址; size 读数据字节数
// 出参: pointer 目标缓冲区
// 返回值: 0 成功;1 写缓冲区满
//***************************************************
BYTE WriteEEData (void * pointer, WORD address, WORD size)
{
	if (ee_write_queue.count < EE_MAX_WRITERECORD)
	{
		DisableIrq();
		ee_write_queue.rec[ee_write_queue.write_rec].pointer = (BYTE *)pointer;
		ee_write_queue.rec[ee_write_queue.write_rec].address = address;
		ee_write_queue.rec[ee_write_queue.write_rec].size = size;
		++ ee_write_queue.write_rec;
		if (ee_write_queue.write_rec == EE_MAX_WRITERECORD)
			ee_write_queue.write_rec = 0x00;
		++ ee_write_queue.count;
		if (!iic_busy)
		{
			iic_busy = 0x01;
			STCTRL = 0x07;					// 下次tick中断开始写
			STCURR = 0x00;
		}
		EnableIrq();
		return 0x00;
	}
	else
	{
		return 0x01;
	}
}

#ifdef TEST_EEPROM
BYTE testram[EE_1K_SIZE];
BYTE backram[EE_1K_SIZE];

//***************************************************
// 函数名: EraseEeprom
// 功能: 擦除eeprom
// 入参: 无
// 出参: 无
// 返回值: 0 成功;1 失败
//***************************************************
BYTE EraseEeprom (void)
{
	WORD testcounter, tempcounter;

	testcounter = 0;
	while (testcounter < EE_CAP_KB)
	{
		memset (testram, 0xff, EE_1K_SIZE);
		WriteEEData (testram, testcounter * EE_1K_SIZE, EE_1K_SIZE);
		++ testcounter;
		while(iic_busy)
			ClrWdt();
	}
	testcounter = 0;
	while (testcounter < EE_CAP_KB)
	{
		memset (testram, 0x00, EE_1K_SIZE);
		ReadEEData (testram, testcounter * EE_1K_SIZE, EE_1K_SIZE);
		++ testcounter;
		for (tempcounter=0;tempcounter<EE_1K_SIZE;++tempcounter)
		{
			if (testram[tempcounter] != 0xff)
				break;
		}
		if (tempcounter != EE_1K_SIZE)
			break;
	}
	return (testcounter != EE_CAP_KB);
}

//***************************************************
// 函数名: TestEeprom
// 功能: 测试 EEPROM
// 入参: 无
// 出参: 无
// 返回值: 0 成功;1 失败
//***************************************************
BYTE TestEeprom (void)
{
	WORD testcounter, tempcounter;

	testcounter = 0;
	while (testcounter < EE_CAP_KB)
	{
		ReadEEData(backram, testcounter * EE_1K_SIZE, EE_1K_SIZE);

		memset (testram, 0x55, EE_1K_SIZE);
		WriteEEData (testram, testcounter * EE_1K_SIZE, EE_1K_SIZE);
		while(iic_busy)
			ClrWdt();
		memset (testram, 0x00, EE_1K_SIZE);
		ReadEEData (testram, testcounter * EE_1K_SIZE, EE_1K_SIZE);
		for (tempcounter=0;tempcounter<EE_1K_SIZE;++tempcounter)
		{
			if (testram[tempcounter] != 0x55)
				break;
		}
		if (tempcounter != EE_1K_SIZE)
		{
			WriteEEData(backram, testcounter * EE_1K_SIZE, EE_1K_SIZE);
			while(iic_busy)
				ClrWdt();
			break;
		}

		memset (testram, 0xaa, EE_1K_SIZE);
		WriteEEData (testram, testcounter * EE_1K_SIZE, EE_1K_SIZE);
		while(iic_busy)
			ClrWdt();
		memset (testram, 0x00, EE_1K_SIZE);
		ReadEEData (testram, testcounter * EE_1K_SIZE, EE_1K_SIZE);
		for (tempcounter=0;tempcounter<EE_1K_SIZE;++tempcounter)
		{
			if (testram[tempcounter] != 0xaa)
				break;
		}
		if (tempcounter != EE_1K_SIZE)
		{
			WriteEEData(backram, testcounter * EE_1K_SIZE, EE_1K_SIZE);
			while(iic_busy)
				ClrWdt();
			break;
		}

		for (tempcounter = 0; tempcounter < EE_1K_SIZE; ++ tempcounter)
		{
			testram[tempcounter] = tempcounter & 0xff;
		}
		WriteEEData (testram, testcounter * EE_1K_SIZE, EE_1K_SIZE);
		while(iic_busy)
			ClrWdt();
		memset (testram, 0x00, EE_1K_SIZE);
		ReadEEData (testram, testcounter * EE_1K_SIZE, EE_1K_SIZE);
		for (tempcounter=0;tempcounter<EE_1K_SIZE;++tempcounter)
		{
			if (testram[tempcounter] != (tempcounter&0xff))
				break;
		}
		if (tempcounter != EE_1K_SIZE)
		{
			WriteEEData(backram, testcounter * EE_1K_SIZE, EE_1K_SIZE);
			while(iic_busy)
				ClrWdt();
			break;
		}

		for (tempcounter = 0; tempcounter < EE_1K_SIZE; ++ tempcounter)
		{
			testram[tempcounter] = 0xff - (tempcounter & 0xff);
		}
		WriteEEData (testram, testcounter * EE_1K_SIZE, EE_1K_SIZE);
		while(iic_busy)
			ClrWdt();
		memset (testram, 0x00, EE_1K_SIZE);
		ReadEEData (testram, testcounter * EE_1K_SIZE, EE_1K_SIZE);
		for (tempcounter=0;tempcounter<EE_1K_SIZE;++tempcounter)
		{
			if (testram[tempcounter] != (0xff - (tempcounter&0xff)))
				break;
		}
		if (tempcounter != EE_1K_SIZE)
		{
			WriteEEData(backram, testcounter * EE_1K_SIZE, EE_1K_SIZE);
			while(iic_busy)
				ClrWdt();
			break;
		}

		WriteEEData(backram, testcounter * EE_1K_SIZE, EE_1K_SIZE);
		while(iic_busy)
			ClrWdt();
		ReadEEData(testram, testcounter * EE_1K_SIZE, EE_1K_SIZE);
		for (tempcounter=0;tempcounter<EE_1K_SIZE;++tempcounter)
		{
			if (testram[tempcounter] != backram[tempcounter])
				break;
		}
		if (tempcounter != EE_1K_SIZE)
			break;

		++ testcounter;
		ClrWdt();
	}
	if (testcounter == EE_CAP_KB)
	{
		for (testcounter = 0; testcounter < EE_CAP_KB; ++ testcounter)
		{
			ReadEEData(&backram[testcounter], testcounter * EE_1K_SIZE, 1);
			WriteEEData(&testcounter, testcounter * EE_1K_SIZE, 1);
			while(iic_busy)
				ClrWdt();
		}
		for (testcounter = 0; testcounter < EE_CAP_KB; ++ testcounter)
		{
			ReadEEData(&testram[testcounter], testcounter * EE_1K_SIZE, 1);
			WriteEEData(&backram[testcounter], testcounter * EE_1K_SIZE, 1);
			while(iic_busy)
				ClrWdt();
		}
		for (testcounter = 0; testcounter < EE_CAP_KB; ++ testcounter)
		{
			if (testcounter != testram[testcounter])
				break;
		}
	}

	return (testcounter != EE_CAP_KB);
}

#endif

//***************************************************
// 函数名: WriteParaData
// 功能: 写入size个字节参数
// 入参: address 地址; size 写数据字节数
// 出参: pointer 目标缓冲区
// 返回值: 0 成功<并不确保写成功>; 1 写缓冲区满
//***************************************************
BYTE WriteParaData (void * pointer, WORD address, WORD size)
{
	if (WriteEEData(pointer, address, size))
		return 0x01;
	if (address < EE_CHECKSUM)
	{
		write_checksum = 1;
	}

	return 0x00;
}


