
#include "typedef.h"
#include "lpc1700.h"
#include "parameter.h"
#include "rtc.h"
#include "i2c.h"
#include "string.h"
#include "io.h"


struct Time t_set;
struct Time t;
BYTE rtc_read_mode = 1;
BYTE rtc_iic_doing = 0;


BYTE InitRtc (void)
{
	BYTE timerbuf[7];
	BYTE ret;

	timerbuf [0] = 0x07;
	timerbuf [1] = 0x00;
	rtc_read_mode = 1;
	iic_write(IIC_ADDR_RTC, timerbuf, 2);

	timerbuf [0] = 0x08;
	ret = iic_write(IIC_ADDR_RTC, timerbuf, 1);
	if (!ret)
	{
		ret = iic_read(IIC_ADDR_RTC, timerbuf, 6);
		if (timerbuf[0] != 0xAA ||
			timerbuf[1] != 0xAA ||
			timerbuf[2] != 0xAA ||
			timerbuf[3] != 0x55 ||
			timerbuf[4] != 0x55 ||
			timerbuf[5] != 0x55)
		{
			rtc_lowpower = 1;
			timerbuf [0] = 0x08;
			timerbuf [1] = 0xAA;
			timerbuf [2] = 0xAA;
			timerbuf [3] = 0xAA;
			timerbuf [4] = 0x55;
			timerbuf [5] = 0x55;
			timerbuf [6] = 0x55;
			iic_write(IIC_ADDR_RTC, timerbuf, 7);
		}
	}
	return ret;
}

BYTE GetRtc (void)
{
	BYTE timerbuf[7];
	BYTE pointer = 0x00;
	BYTE size;
	BYTE ret;

	if (rtc_read_mode)
	{
		rtc_read_mode = 0;
		size = 7;
	}
	else
	{
		if (t.second != 59)
			size = 1;
		else if (t.minute != 59)
			size = 2;
		else if (t.hour != 23)
			size = 3;
		else
			size = 7;
		if (t.second > 59 || t.minute > 59 || t.hour > 23 || t.day > 31 || t.month > 12)
			size = 7;
	}
	ret = iic_write(IIC_ADDR_RTC, &pointer, 1);			//Ð´Ö¸Õë¼Ä´æÆ÷
	if (!ret)
	{
		ret = iic_read(IIC_ADDR_RTC, timerbuf, size);
		if (!ret)
		{
			t.second = (timerbuf[0] & 0x0F);
			t.second += ((timerbuf[0] >> 4) * 10);
			if (size > 1)
			{
				t.minute = (timerbuf[1] & 0x0F);
				t.minute += ((timerbuf[1] >> 4) * 10);
				if (size > 2)
				{
					t.hour = (timerbuf[2] & 0x0F);
					t.hour += ((timerbuf[2] >> 4) * 10);
					if (size > 3)
					{
						t.day = (timerbuf[4] & 0x0F);
						t.day += ((timerbuf[4] >> 4) * 10);
						t.month = (timerbuf[5] & 0x0F);
						t.month += ((timerbuf[5] >> 4) * 10);
						t.year = (timerbuf[6] & 0x0F);
						t.year += ((timerbuf[6] >> 4) * 10);
						t.weekday = (timerbuf[3] & 0x07);
					}
				}
			}
		}
	}
	if (ret)
		rtc_read_mode = 1;
	return ret;
}

BYTE SetRtc (void)
{
	BYTE timerbuf[8];
	BYTE ret;

	rtc_read_mode = 1;
	timerbuf[7] = (t_set.year-2000) / 10;
	timerbuf[7] = timerbuf[7] << 4;
	timerbuf[7] = timerbuf[7]|((t_set.year-2000) % 10);

	timerbuf[6] = t_set.month / 10;
	timerbuf[6] = timerbuf[6] << 4;
	timerbuf[6] = timerbuf[6]|(t_set.month % 10);

	timerbuf[5] = t_set.day / 10;
	timerbuf[5] = timerbuf[5] << 4;
	timerbuf[5] = timerbuf[5]|(t_set.day % 10);

	timerbuf[4] = t_set.weekday;

	timerbuf[3] = t_set.hour / 10;
	timerbuf[3] = timerbuf[3] << 4;
	timerbuf[3] = timerbuf[3]|(t_set.hour % 10);

	timerbuf[2] = t_set.minute / 10;
	timerbuf[2] = timerbuf[2] << 4;
	timerbuf[2] = timerbuf[2]|(t_set.minute % 10);

	timerbuf[1] = t_set.second / 10;
	timerbuf[1] = timerbuf[1] << 4;
	timerbuf[1] = timerbuf[1]|(t_set.second % 10);

	timerbuf[0] = 0x00;
	ret = iic_write(IIC_ADDR_RTC, timerbuf, 8);
	return ret;
}

