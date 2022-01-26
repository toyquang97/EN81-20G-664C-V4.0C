
#ifndef _TEMPERATURE_H_
#define _TEMPERATURE_H_

#define MMT_AD_H_LIMIT	653		// 55+1C
#define MMT_AD_L_LIMIT	2933	// -10-1C

#define MMT_UNKNOW		3
#define MMT_H	2
#define MMT_L	1
#define MMT_OK	0

extern WORD advalue;

BYTE GetCurrentTemperature(void);

#endif

