
#include "typedef.h"
#include "lpc1700.h"
#include "parameter.h"
#include "load.h"

#define C_LD_BF_SIZE		100

BYTE ldtimes = 0;
WORD ldoffset = 0;
WORD ldbuff[C_LD_BF_SIZE];
WORD ldvalue = 0xffff;
BYTE ldstate[4];

/****************************************************
* ������: GetLoadValue
* ����: ��ȡ����A/Dֵ����ǳ���״̬
* ���: ��
* ����: ��
* ����ֵ: ��
****************************************************/
void GetLoadValue(void)
{
	static DWORD sum;
	WORD value;
	BYTE i = 0;
	AD0CR = (1ul << 0)						// AD0.0
			| (1ul << 8)					// 12MHz
			| (0ul << 16)					// burst=0 �������ת��
			| (1ul << 21)					// PDN=1 ��������ģʽ
			| (1ul << 24)					// ����ֱ������ģʽ
			| (0ul << 27);					// 
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	ldstate[2] = ldstate[1];
	ldstate[1] = ldstate[0];
	while ((ADSTAT & (1ul << 0)) == 0)
	{
		__nop();
		__nop();
		__nop();
		++ i;
		if (i == 0xff)
			return;
	}
	value = (ADDR0 >> 4) & 0xfff;

	if (ldtimes < C_LD_BF_SIZE)
	{
		ldbuff[ldoffset++] = value;
		if (ldoffset == C_LD_BF_SIZE)
			ldoffset = 0;

		++ ldtimes;
		if (ldtimes == C_LD_BF_SIZE)
		{
			sum = 0;
			for (i=0; i<C_LD_BF_SIZE; ++i)
				sum += ldbuff[i];
		}
	}
	else
	{
		sum -= ldbuff[ldoffset];
		sum += value;
		ldbuff[ldoffset++] = value;
		if (ldoffset == C_LD_BF_SIZE)
			ldoffset = 0;
		ldvalue = sum / C_LD_BF_SIZE;

		if (p.adlimit[OVERLOADLIMIT] > p.adlimit[ZEROLOADLIMIT])
		{
			if (p.adlimit[OVERLOADLIMIT])
			{
				if (ldvalue > p.adlimit[OVERLOADLIMIT])
					ldstate[0] |= OVERLOAD;
				else
					ldstate[0] &= ~OVERLOAD;
			}
			if (p.adlimit[FULLLOADLIMIT])
			{
				if (ldvalue > p.adlimit[FULLLOADLIMIT])
					ldstate[0] |= FULL_LOAD;
				else
					ldstate[0] &= ~FULL_LOAD;
			}
			if (p.adlimit[HALFLOADLIMIT])
			{
				if (ldvalue > p.adlimit[HALFLOADLIMIT])
					ldstate[0] |= HALF_LOAD;
				else
					ldstate[0] &= ~HALF_LOAD;
			}
			if (p.adlimit[ZEROLOADLIMIT])
			{
				if (ldvalue < p.adlimit[ZEROLOADLIMIT])
					ldstate[0] |= ZERO_LOAD;
				else
					ldstate[0] &= ~ZERO_LOAD;
			}
		}
		else if (p.adlimit[OVERLOADLIMIT] < p.adlimit[ZEROLOADLIMIT])
		{
			if (p.adlimit[OVERLOADLIMIT])
			{
				if (ldvalue < p.adlimit[OVERLOADLIMIT])
					ldstate[0] |= OVERLOAD;
				else
					ldstate[0] &= ~OVERLOAD;
			}
			if (p.adlimit[FULLLOADLIMIT])
			{
				if (ldvalue < p.adlimit[FULLLOADLIMIT])
					ldstate[0] |= FULL_LOAD;
				else
					ldstate[0] &= ~FULL_LOAD;
			}
			if (p.adlimit[HALFLOADLIMIT])
			{
				if (ldvalue < p.adlimit[HALFLOADLIMIT])
					ldstate[0] |= HALF_LOAD;
				else
					ldstate[0] &= ~HALF_LOAD;
			}
			if (p.adlimit[ZEROLOADLIMIT])
			{
				if (ldvalue > p.adlimit[ZEROLOADLIMIT])
					ldstate[0] |= ZERO_LOAD;
				else
					ldstate[0] &= ~ZERO_LOAD;
			}
		}

		ldstate[3] = ldstate[0] ^ ldstate[1] ^ ldstate[2];
		load &= ldstate[3];
		load |= (ldstate[0] & (~ldstate[3]));		
	}
}

