
#ifndef _RTC_H_
#define _RTC_H_

#define IIC_ADDR_RTC				0x68			// RTCÐ¾Æ¬µÄI2CµØÖ·

struct Time
{
	BYTE second;
	BYTE minute;
	BYTE hour;
	BYTE day;
	BYTE month;
	BYTE weekday;
	WORD year;
};

extern struct Time t_set;
extern struct Time t;
extern BYTE rtc_iic_doing;

BYTE InitRtc (void);
BYTE GetRtc (void);
BYTE SetRtc (void);


#endif

