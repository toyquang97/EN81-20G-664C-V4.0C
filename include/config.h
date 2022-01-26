
#ifndef _CONFIG_H_
#define _CONFIG_H_

#define FOSC				12000000ul				// 晶振频率12MHz
#define FCCLK			(FOSC * 8)				// 主时钟频率96MHz
#define FPCLK				(FCCLK / 4)				//外设时钟频率24MHz

#define FCCO				(FCCLK * 3)				// PLL频率
#define PLL_NVALUE		1
#define PLL_MVALUE		((FCCO / FOSC) * (PLL_NVALUE) / 2)

#define UART0_BPS		9600
#define BRP_125K		0xAB800Bul		//TESG1 = 12  TESG2 = 1  BRP = 11  SJW = 1  采样1次，采样点 87.5%
	//bit0-bit9: BRP = 0x0B + 1 = 12						
	//bit10-bit13: Not Used
	//bit14-bit15: SJW = 0x02 + 1 = 3
	//bit16-bit19: TESG1 = 0x0B + 1 = 12
	//bit20-bit22: TESG2 = 0x02 + 1 = 3
	//bit23: SAM = 1 采样3次
	//bit24-bit31: Not Used

#define BRP_50K			0xAB801Dul		//TESG1 = 12  TESG2 = 1  BRP = 29  SJW = 1  采样3次，采样点 87.5%
	//bit0-bit9: BRP = 0x1D + 1 = 30						
	//bit10-bit13: Not Used
	//bit14-bit15: SJW = 0x02 + 1 = 3
	//bit16-bit19: TESG1 = 0x0B + 1 = 12
	//bit20-bit22: TESG2 = 0x02 + 1 = 3
	//bit23: SAM = 1 采样3次
	//bit24-bit31: Not Used

#define IIC_100K			100000ul
#define IIC_300K			300000ul

#define	SRU_NEW		1
#define	SRU_OLD		0

#define SYS_TICK_8ms		0xbb7fful
#define SYS_TICK_12ms		0x1193fful
#define SYS_TICK_15ms		0x15f900ul

#endif

