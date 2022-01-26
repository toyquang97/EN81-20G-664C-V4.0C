
#include "typedef.h"
#include "lpc1700.h"
#include "parameter.h"
#include "temperature.h"

#define	C_AD_BF_SIZE		10

WORD advalue;
BYTE adtimes = 0;
WORD adoffset = 0;
WORD adbuff[C_AD_BF_SIZE];

/****************************************************
* 函数名: GetCurrentTemperature
* 功能: 读取机房当前温度
* 入参: 无
* 出参: 无
* 返回值: 当前温度状态
****************************************************/
BYTE GetCurrentTemperature(void)
{
	WORD value, min, max;
	DWORD sum;
	BYTE i = 0;

	AD0CR = (1ul << 1)						// AD0.1
			| (1ul << 8)					// 12MHz
			| (0ul << 16)					// burst=0 软件控制转换
			| (1ul << 21)					// PDN=1 正常工作模式
			| (1ul << 24)					// 设置直接启动模式
			| (0ul << 27);					// 
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	while ((ADSTAT & (1ul << 1)) == 0)
	{
		__nop();
		__nop();
		__nop();
		++ i;
		if (i == 0xff)
			return MMT_UNKNOW;
	}

	value = (ADDR1 >> 4) & 0xfff;
	adbuff[adoffset++] = value;
	if (adoffset == C_AD_BF_SIZE)
		adoffset = 0;
	if (adtimes < C_AD_BF_SIZE)
	{
		++ adtimes;
		return MMT_OK;
	}
	else
	{
		sum = 0;
		min = 0xffff;
		max = 0;
		for (i=0; i<C_AD_BF_SIZE; ++i)
		{
			sum += adbuff[i];
			if (adbuff[i] > max)
				max = adbuff[i];
			if (adbuff[i] < min)
				min = adbuff[i];
		}
		sum -= min;
		sum -= max;
		advalue = sum / (C_AD_BF_SIZE-2);

		if (advalue >= MMT_AD_L_LIMIT)
			return MMT_L;
		else if (advalue <= MMT_AD_H_LIMIT)
			return MMT_H;
		else
			return MMT_OK;
	}
}


