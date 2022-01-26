
#include "typedef.h"
#include "lpc1700.h"
#include "parameter.h"
#include "i2c.h"

BYTE iic_read(BYTE address, BYTE * data, BYTE count)
{
	volatile DWORD dly;

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
		return 0x01;
	}
	I2C0DAT = (address << 1) + 0x01;
	I2C0CONCLR = 0x28;
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
		return 0x01;
	}
	if (count > 1)
	{
		I2C0CONSET = 0x04;
	}
	else
	{
		I2C0CONCLR = 0x04;
	}
	I2C0CONCLR = 0x28;
	while (count --)
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
			return 0x01;
		}

		*data ++ = I2C0DAT;
		if (count == 0)
			break;
		else if (count > 1)
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
	return 0x00;
}

BYTE iic_write(BYTE address, BYTE * data, BYTE count)
{
	volatile DWORD dly;

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
		return 0x01;
	}

	I2C0DAT = address << 1;
	I2C0CONCLR = 0x28;
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
		return 0x01;
	}

	while (count --)
	{
		I2C0DAT = *data ++;
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
			return 0x01;
		}
	}
	I2C0CONSET = 0x10;
	I2C0CONCLR = 0x28;
	return 0x00;
}


