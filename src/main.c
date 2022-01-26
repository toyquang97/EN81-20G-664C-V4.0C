
#include "typedef.h"
#include "string.h"
#include "lpc1700.h"
#include "lpc1700pincfg.h"
#include "parameter.h"
#include "call.h"
#include "can.h"
#include "state.h"
#include "eeprom.h"
#include "rtc.h"
#include "text.h"
#include "test.h"
#include "history.h"
#include "io.h"
#include "temperature.h"
#include "pos.h"
#include "bootloader.h"
#include "mcp2515.h"
#include "nice5000.h"
#include "version.h"


/****************************************************
* Function name: DisableIrq
* Function: Close interrupt
* Entry: None
* Reference: None
* Return value: 0
****************************************************/
__asm SWORD DisableIrq (void)
{
	CPSID	I
	MOV		R0, #0
	BX		LR
}

/****************************************************
** Function name: EnableIrq
* Function: Open interrupt
* Entry: None
* Reference: None
* Return value: 0
****************************************************/
__asm SWORD EnableIrq (void)
{
	CPSIE	I
	MOV		R0, #0
	BX		LR
}

/****************************************************
* Function name: __WFI
* Function: Enter power saving mode
* Entry: None
* Reference: None
* Return value: 0
****************************************************/
__asm SWORD __WFI(void)
{
	WFI
	MOV 	R0, #0
	BX		LR
}

/****************************************************
*Function name: __GoToUpgrade
* Function: Enter bootloader CAN upgrade module
* Input parameters: addr bootloader upgrade address; mode 0 broadcast, non-zero broadcast; can 1 CANA, 2 CANB
* Reference: None
* Return value: 0
****************************************************/
__asm SWORD __GoToUpgrade(DWORD addr, DWORD mode, DWORD can)
{
	BX		R0
}


/****************************************************
* Function name: IsrSet
* Function: Set interrupt, priority
* Input parameter: channel channel number; prio priority
* Reference: None
* Return value: None
****************************************************/
void IsrSet (DWORD channel, DWORD prio)
{
	DWORD tmp1, tmp2, tmp3;

	if (channel >= 16)
	{
		tmp3 = channel - 16;
		tmp1 = tmp3 / 32;
		tmp2 = tmp3 % 32;
		((DWORD *)0xE000E100) [tmp1] = 1ul << tmp2;
		((BYTE *)0xE000E400) [tmp3] = prio;
	}
	else
	{
		switch (channel)
		{
			case MMI:
				SHCSR = SHCSR | (1 << 16);
				break;
			case BFI:
				SHCSR = SHCSR | (1 << 17);
				break;
			case UFI:
				SHCSR = SHCSR | (1 << 18);
				break;
			default:
				break;
		}
		if (channel >= MMI)
		{
			tmp3 = channel - MMI;
			((BYTE *)0xE000ED18) [tmp3] = prio;
		}
	}
}

/****************************************************
* Function name: delay
* Function: Delay ms without interruption
* Entry: None
* Reference: None
* Return value: None
****************************************************/
void delay(DWORD ms)
{
	volatile DWORD i;
	while(ms--)
	{
		for (i=0; i<19310; ++i);
	}
}

/****************************************************
* Function name: LpcCpuInit
* Function: Initialize LPC17XX
* Entry: None
* Reference: None
* Return value: None
****************************************************/
void LpcCpuInit (void)
{
	FLASHCFG = (0x05ul << 12) & (~(0x003f)) | 0x003a;		// FLASH ����ʹ��6��cpu clock
	PCLKSEL0 = 0x00000000;									// ����Ƶ��Ϊ��ʱ��1/4
	PCLKSEL1 = 0x00000000;
	CLKOUTCFG = 0;											// ���cpu clock

	if ((PLL0STAT >> 24) == 1)
	{
		PLL0CON = 1;
		PLL0FEED = 0xAA;
		PLL0FEED = 0x55;
	}
	PLL0CON = 0;											// �ر�pll0
	PLL0FEED = 0xAA;
	PLL0FEED = 0x55;
	while ((PLL0STAT & (3ul << 24)));				// �ȴ����

	SCS = (SCS & 0x04) | 0x20;								// ����������1~20MHz
	while ((SCS & (1ul << 6)) == 0);					// �ȴ����ȶ�

	CLKSRCSEL = 0x01;													// ѡ��������ΪPLL0ʱ��Դ

	PLL0CFG = (((PLL_NVALUE -1) << 16) | (PLL_MVALUE - 1));	// ����PLL0ϵ��
	PLL0FEED = 0xAA;
	PLL0FEED = 0x55;

	PLL0CON = 1;																// ��pll0,����δ����
	PLL0FEED = 0xAA;
	PLL0FEED = 0x55;

	CCLKCFG = (FCCO / FCCLK) - 1;							// ����PLL->CPU clock�ķ�Ƶϵ��
	while ((PLL0STAT & (1ul << 26)) == 0);					// �ȴ����໷����
	while (((PLL0STAT & 0x00007FFF) != (PLL_MVALUE - 1))	// ����M/Nϵ��
		&& (((PLL0STAT & 0x00FF0000) >> 16) != (PLL_NVALUE - 1)));

	PLL0CON = 3;
	PLL0FEED = 0xAA;
	PLL0FEED = 0x55;
	while ((PLL0STAT & (1ul << 25)) == 0);					// �ȴ�PLL���ӵ�CPU clock
}

/****************************************************
* ������: LpcPinInit
* ����: ��ʼ��I/O����
* ���: ��
* ����: ��
* ����ֵ: ��
****************************************************/
void LpcPinInit (void)
{
	PINSEL0 = (P0_15_FUNC << 30) |	
		(P0_11_FUNC << 22) |					
		(P0_10_FUNC << 20) |					
		(P0_9_FUNC << 18) | 				
		(P0_8_FUNC << 16) | 				
		(P0_7_FUNC << 14) | 				
		(P0_6_FUNC << 12) | 				
		(P0_5_FUNC << 10) | 				
		(P0_4_FUNC << 8) |					
		(P0_3_FUNC << 6) |					
		(P0_2_FUNC << 4) |					
		(P0_1_FUNC << 2) |					
		(P0_0_FUNC << 0); 			

	PINSEL1 = (P0_30_FUNC << 28) |		
		(P0_29_FUNC << 26) |		
		(P0_28_FUNC << 24) |		
		(P0_27_FUNC << 22) |		
		(P0_26_FUNC << 20) |		
		(P0_25_FUNC << 18) |		
		(P0_24_FUNC << 16) |		
		(P0_23_FUNC << 14) |		
		(P0_22_FUNC << 12) |		
		(P0_21_FUNC << 10) |		
		(P0_20_FUNC << 8) | 		
		(P0_19_FUNC << 6) | 		
		(P0_18_FUNC << 4) | 		
		(P0_17_FUNC << 2) | 		
		(P0_16_FUNC << 0);			

	PINSEL2 = (P1_15_FUNC << 30) |		
		(P1_14_FUNC << 28) |			
		(P1_10_FUNC << 20) |			
		(P1_9_FUNC << 18) | 		
		(P1_8_FUNC << 16) | 		
		(P1_4_FUNC << 8) |			
		(P1_1_FUNC << 2) |			
		(P1_0_FUNC << 0); 			

	PINSEL3 = (P1_31_FUNC << 30) |		
		(P1_30_FUNC << 28) |		
		(P1_29_FUNC << 26) |		
		(P1_28_FUNC << 24) |		
		(P1_27_FUNC << 22) |		
		(P1_26_FUNC << 20) |		
		(P1_25_FUNC << 18) |		
		(P1_24_FUNC << 16) |		
		(P1_23_FUNC << 14) |		
		(P1_22_FUNC << 12) |		
		(P1_21_FUNC << 10) |		
		(P1_20_FUNC << 8) | 		
		(P1_19_FUNC << 6) | 		
		(P1_18_FUNC << 4) | 		
		(P1_17_FUNC << 2) | 		
		(P1_16_FUNC << 0);			

	PINSEL4 = (P2_13_FUNC << 26) |	
		(P2_12_FUNC << 24) |		
		(P2_11_FUNC << 22) |		
		(P2_10_FUNC << 20) |		
		(P2_9_FUNC << 18) | 	
		(P2_8_FUNC << 16) | 	
		(P2_7_FUNC << 14) | 	
		(P2_6_FUNC << 12) | 	
		(P2_5_FUNC << 10) | 	
		(P2_4_FUNC << 8) |		
		(P2_3_FUNC << 6) |		
		(P2_2_FUNC << 4) |		
		(P2_1_FUNC << 2) |		
		(P2_0_FUNC << 0); 		

	PINSEL7 = (P3_26_FUNC << 20) |	
		(P3_25_FUNC << 18); 					

	PINSEL9 = (P4_29_FUNC << 26) |	
		(P4_28_FUNC << 24); 		

	PINSEL10 = P10_3_FUNC;					// TPIU��ֹ, P2.2~p2.6������Trace Function

	PINMODE0 = (P0_15_MODE << 30) |
		(P0_11_MODE << 22) |
		(P0_10_MODE << 20) |
		(P0_9_MODE << 18) |
		(P0_8_MODE << 16) |
		(P0_7_MODE << 14) |
		(P0_6_MODE << 12) |
		(P0_5_MODE << 10) |
		(P0_4_MODE << 8) |
		(P0_3_MODE << 6) |
		(P0_2_MODE << 4) |
		(P0_1_MODE << 2) |
		(P0_0_MODE << 0);
	PINMODE1 = (P0_26_MODE << 20) |
		(P0_25_MODE << 18) |
		(P0_24_MODE << 16) |
		(P0_23_MODE << 14) |
		(P0_22_MODE << 12) |
		(P0_21_MODE << 10) |
		(P0_20_MODE << 8) |
		(P0_19_MODE << 6) |
		(P0_18_MODE << 4) |
		(P0_17_MODE << 2) |
		(P0_16_MODE << 0);

	PINMODE2 = (P1_15_MODE << 30) |
		(P1_14_MODE << 28) |
		(P1_10_MODE << 20) |
		(P1_9_MODE << 18) |
		(P1_8_MODE << 16) |
		(P1_4_MODE << 8) |
		(P1_1_MODE << 2) |
		(P1_0_MODE << 0);
	PINMODE3 = (P1_31_MODE << 30) |
		(P1_30_MODE << 28) |
		(P1_29_MODE << 26) |
		(P1_28_MODE << 24) |
		(P1_27_MODE << 22) |
		(P1_26_MODE << 20) |
		(P1_25_MODE << 18) |
		(P1_24_MODE << 16) |
		(P1_23_MODE << 14) |
		(P1_22_MODE << 12) |
		(P1_21_MODE << 10) |
		(P1_20_MODE << 8) |
		(P1_19_MODE << 6) |
		(P1_18_MODE << 4) |
		(P1_17_MODE << 2) |
		(P1_16_MODE << 0);

	PINMODE4 = (P2_13_MODE << 26) |
		(P2_12_MODE << 24) |
		(P2_11_MODE << 22) |
		(P2_10_MODE << 20) |
		(P2_9_MODE << 18) |
		(P2_8_MODE << 16) |
		(P2_7_MODE << 14) |
		(P2_6_MODE << 12) |
		(P2_5_MODE << 10) |
		(P2_4_MODE << 8) |
		(P2_3_MODE << 6) |
		(P2_2_MODE << 4) |
		(P2_1_MODE << 2) |
		(P2_0_MODE << 0);

	PINMODE7 = (P3_26_MODE << 20) |
		(P3_25_MODE << 18);

	PINMODE9 = (P4_29_MODE << 26) |
		(P4_28_MODE << 24);

	FIO0MASK = 0x00;						// �����ο���I/O����
	FIO1MASK = 0x00;
	FIO2MASK = 0x00;
	FIO3MASK = 0x00;
	FIO4MASK = 0x00;

	I2CPADCFG = 0x00;

	FIO0DIR = PIO_9 | PIO_7 | PIO_4 | PIO_1;
	FIO0SET = PIO_4;
	FIO1DIR = ~((DWORD)(PIO_0 | PIO_1 | PIO_4 | PIO_21));
	FIO1SET = PIO_8 | PIO_9 | PIO_14 | PIO_15 | PIO_20 | PIO_23;		//GCAN, �����Ƭѡ
	FIO1CLR = PIO_16 | PIO_17 | PIO_18 | PIO_19 | PIO_22;
	FIO2DIR = 0x00000000ul;
	FIO3DIR = 0x00000000ul;
	FIO4DIR = PIO_29;

	MCP_CS_OFF();

	PCONP = (PCTIM0_FUNC << 1) | (PCTIM1_FUNC << 2) | (PCUART0_FUNC << 3) | (PCUART1_FUNC << 4) |
		(PWM1_FUNC << 6) | (PCI2C0_FUNC << 7) | (PCSPI_FUNC << 8) | (PCRTC_FUNC << 9) |
		(PCSSP1_FUNC << 10) | (PCAD_FUNC << 12) | (PCCAN1_FUNC << 13) | (PCCAN2_FUNC << 14) |
		(PCGPIO_FUNC << 15) | (PCRIT_FUNC << 16) | (PCMC_FUNC << 17) | (PCQEI_FUNC << 18) |
		(PCI2C1_FUNC << 19) | (PCSSP0_FUNC << 21) | (PCTIM2_FUNC << 22) | (PCTIM3_FUNC << 23) |
		(PCUART2_FUNC << 24) | (PCUART3_FUNC << 25) | (PCI2C2_FUNC << 26) | (PCI2S_FUNC << 27) |
		(PCGPDMA_FUNC << 29) | (PCENET_FUNC << 30) | (PCUSB_FUNC << 31);

	state_msi = MSI_IN;
	state_sgm = ~state_msi;

	IO0IntClr = (0x01ul << 30);
	IO0IntEnr = (0x01ul << 30);
	IO0IntEnf = (0x01ul << 30);

	IO2IntClr = (0x01ul << 9) | (0x01ul << 12) | (0x01ul << 13);
	IO2IntEnr = (0x01ul << 9) | (0x01ul << 12) | (0x01ul << 13);			//��ȫ��·ʹ���������ж�
	IsrSet(NVIC_EINT3, PRIO_ZERO);		// I/O�жϺ�EXT3�����ж�����

	EXTMODE = (0x01 << 1);		// �ⲿ�ж�1���ش���
	EXTPOLAR = (0x00 << 1); 	// �ⲿ�ж�1�½��ش���
	EXTINT = (0x01 << 1); 	// ����жϱ��
	IsrSet(NVIC_EINT1, PRIO_TWO);
}

/****************************************************
* ������: LpcSpiInit
* ����: ��ʼ��SPI�ӿ�,���ڷ���EEPROM��SD
* ���: ��
* ����: ��
* ����ֵ: ��
****************************************************/
void LpcSpiInit (void)
{
	volatile DWORD temp;

	SSP0CR0 = 0x0007;                    	// 8Bit, CPOL=0, CPHA=0
	SSP0CR1 = 0x0002;						// SSP1 enable, master
	SSP0CPSR = MCP_SPI_CLK;
	while( SSP0SR & SSP_SR_BSY );
	while( SSP0SR & SSP_SR_RNE)
		temp = SSP0DR;
	delay(10);

	init_error |= B_DRV;
	SSP1CR0 = 0x00cf;                    	// 16Bit, CPOL=0, CPHA=0
	SSP1CR1 = 0x0002;						// SSP1 enable, master
	SSP1CPSR = NICE5000_SPI_CLK;
	while( SSP1SR & SSP_SR_BSY);
	while( SSP1SR & SSP_SR_RNE )
		temp = SSP1DR;
	
	delay(10);
}

/****************************************************
* ������: LpcTimerInit
* ����: ��ʼ����ʱ��
* ���: ��
* ����: ��
* ����ֵ: ��
****************************************************/
void LpcTimerInit (void)
{
	T0TCR = 0x02;
	T0IR = 0x01;
	T0CTCR = 0x00;
	T0PR = 0x00;
	T0MR0 = FPCLK / 2;						// 0.5S
	T0MCR = 0x03;
	T0TCR = 0x01;
	IsrSet(NVIC_TIMER0, PRIO_THREE);

	T1TCR = 0x02;
	T1IR = 0x01;
	T1CTCR = 0x00;
	T1PR = 0x00;
	T1MR0 = FPCLK / 1000;					// 1mS
	T1MCR = 0x03;
	T1TCR = 0x01;
	IsrSet(NVIC_TIMER1, PRIO_THREE);

	T2TCR = 0x02;
	T2IR = 0x01;
	T2CTCR = 0x00;
	T2PR = 0x00;
	T2MR0 = FPCLK / 20;			// 50mS
	T2MCR = 0x03;
	T2TCR = 0x01;
	IsrSet(NVIC_TIMER2, PRIO_THREE);

	T3TCR = 0x02;
	T3IR = 0x01;
	T3CTCR = 0x00;
	T3PR = 0x00;
	T3MR0 = FPCLK / 500;		// 2mS
	T3MCR = 0x03;
	T3TCR = 0x01;
	
	IsrSet(NVIC_TIMER3, PRIO_ZERO);
}

/****************************************************
* ������: LpcUartInit
* ����: ��ʼ���������
* ���: ��
* ����: ��
* ����ֵ: ��
****************************************************/
void LpcUartInit (void)
{
	WORD div;

	U0LCR = 0x83;
	div = (FPCLK /16) / UART0_BPS;
	U0DLM = div >> 8;
	U0DLL = div & 0xff;
	U0LCR = 0x03;
	U0FCR = 0x87;
	U0IER = 0x01;
}

/****************************************************
* ������: LpcI2cInit
* ����: ��ʼ��I2C�ӿ�
* ���: ��
* ����: ��
* ����ֵ: ��
****************************************************/
void LpcI2cInit (void)
{
	RTCCIIR = 0x00;
	RTCAMR = 0xFF;
	RTC_AUXEN = 0x00;
	RTCCCR = 0x11;

	I2C0SCLH = (FPCLK/IIC_300K +1) / 2;
	I2C0SCLL = (FPCLK/IIC_300K) / 2;
	I2C0CONCLR = 0x2c;
	I2C0CONSET = 0x40;
	InitRtc();

	IsrSet(STI, PRIO_TWO);
	STCTRL = 0x04;
	STRELOAD = SYS_TICK_12ms;
	STCALIB = SYS_TICK_12ms;
	memset(&ee_write_queue, 0x00, sizeof(ee_write_queue));
	iic_busy = 0;
	eeprom_count = 0;
 }
 
/****************************************************
* ������: LpcQeiInit
* ����: ��ʼ�� QEI �ӿ�
* ���: ��
* ����: ��
* ����ֵ: ��
****************************************************/
void LpcCanInit (void)
{
	init_can_a();
	init_can_b();
	init_can_c();
	AFMR = 0x02ul;
	IsrSet(NVIC_CAN, PRIO_ONE);	
}

/****************************************************
* ������: InitVariables
* ����: ������ʼ��
* ���: ��
* ����: ��
* ����ֵ: ��
****************************************************/
//volatile DWORD compile;
void InitVariables (void)
{
	static volatile DWORD compile;
	BYTE i, j;

	compile = (WORD)version_fver;			
	compile = (WORD)version_chksum;

	for (i = 0; i < MAX_GR; i++)
		groupcall [i].floor = 0xFF;
	doorstate = ALL_DOORS_CLOSED;
	for (i = 0; i < MAX_FLOOR; i++)
	{
		priorstate [0][i] = 0;
		priorstate [1][i] = 0;
		priorstate [2][i] = 0;
		disable_carcall [i] = 0;
		enable_carcall [i] = 0;
		disable_hallcall [i] = 0;
		enable_hallcall [i] = 0;
		calltab [i].calltype = 0;
		calltab [i].cc_door = 0;
		calltab [i].lcu_door = 0;
		calltab [i].lcd_door = 0;
		calltab [i].lc_door = 0;
		calltab [i].sc_door = 0;
		calltab [i].ac_door = 0;
		calltab [i].ec_door = 0;
		floor_record_fg[i] = 0;
		floor_higth[i] = 0;
	}

//��� can �������Ĳ��ֲ�������
//CANA
	rca = 0;
	roa = 0;
	ria = 0;
	tca = 0;
	toa = 0;
	tia = 0;
	tra = 1;
	tea = 1;
	sca = 0;
	sia = 0;
	soa = 0;
	sra = 1;	
	for(i=0; i<RXASW_SIZE; i++)
		{
			for(j=0; j<10; j++)
				{
					rxa[i][j] = 0;
					txa[i][j] = 0;
					sxa[i][j] = 0;
				}
		}
//CANB
	rcb = 0;
	rob = 0;
	rib = 0;
	tcb = 0;
	tob = 0;
	tib = 0;
	trb = 1;
	teb = 1;
	scb = 0;
	sib = 0;
	sob = 0;
	srb = 1;	
	for(i=0; i<RXBSW_SIZE; i++)
		{
			for(j=0; j<10; j++)
				{
					rxb[i][j] = 0;
					txb[i][j] = 0;
					sxb[i][j] = 0;
				}
		}
//CANC
	rcc = 0;
	roc = 0;
	ric = 0;
	tcc = 0;
	toc = 0;
	tic = 0;
	trc = 1;
	tec = 1;
	for(i=0; i<RXCSW_SIZE; i++)
		{
			for(j=0; j<10; j++)
				{
					rxc[i][j] = 0;
					txc[i][j] = 0;
				}
		}

	DisableIrq();
	ReadEEData (&errorhist, EE_ERRHISTORY, sizeof(errorhist));
	ReadEEData (&errorcounter, EE_ERRCOUNTER, sizeof(errorcounter));
	if(((p.simulation == SIMULATION_AUTO) && p.simu_mult)
			|| ((p.simulation == SIMULATION_DRIVE) && p.simu_mult && (!p.setup_ready)))
		set_floorhigth();
	else
		ReadEEData(&floor_higth, EE_FLOOR_HIGH, sizeof(floor_higth));
	EnableIrq();

	if (tripcount >= 10000000ul)
	{
		tripcount = 0;
		WriteParaData (&tripcount, EE_TRIPCOUNT, sizeof(tripcount));
	}

	if (op_time >= 324000000ul)						// > 90000 h
	{
		op_time = 0;
		WriteParaData (&op_time, EE_OPERTIME, sizeof(op_time));
	}
	if ((errorhist.histpointer >= MAX_HIST) || (errorhist.check != ERRORCOUNTCHECK))
	{
		memset(&errorhist, 0x00, sizeof(errorhist));
		errorhist.check = ERRORCOUNTCHECK;
		WriteParaData(&errorhist, EE_ERRHISTORY, sizeof(errorhist));
	}
	if (errorcounter.check != ERRORCOUNTCHECK)
	{
		memset(&errorcounter, 0x00, sizeof(errorcounter));
		errorcounter.t.second = t.second;
		errorcounter.t.minute = t.minute;
		errorcounter.t.hour = t.hour;
		errorcounter.t.day = t.day;
		errorcounter.t.month = t.month;
		errorcounter.t.weekday = t.weekday; 
		errorcounter.t.year = t.year;
		errorcounter.check = ERRORCOUNTCHECK;
		WriteParaData(&errorcounter, EE_ERRCOUNTER, sizeof(errorcounter));
	}

	oldtse_arrowtype = p.arrowtype;
	oldese_arrowtype = p.arrowtype;
	oldtse_extnums = p.tse_extnums;
	
	cana_error_count = 0;
	canb_error_count = 0;
	canc_error_count = 0;
	
	landings_off = 0;
	door_off = 0;
	bTestLoad = 0;
	bTest_IlsSwitch = 0;
	reset_mcu = 0;
	if((p.simulation == SIMULATION_AUTO) && p.simu_mult)
		{
			level = p.bot_floor;
			check_dl_short = 0;	
			nice5000_Tcmd_data4	|= 0x00F8;
			iv_para_valid = 1;
		}
	else
		check_dl_short = 1;
	
// PR17 Test runs
	testtrip = 0;
	test_times = 0;
	testtimes = 0;
	test_starttime = 0;
	test_endtime = 0;
	test_interval = 20;		//���м��Ϊ20s

	in = READ_INPUTS & HSE_MARIN[MAX_HSE_IN + MAX_HSE_EX_IN];
	oldin = (~in) & HSE_MARIN[MAX_HSE_IN + MAX_HSE_EX_IN];
}


/****************************************************
* ������: InitParameter
* ����: ������ʼ��
* ���: ��
* ����: ��
* ����ֵ: 0
****************************************************/
void InitParameter (void)
{
	WORD i, j;

	ClrWdt();
	for (i = 0; i < MAX_HSE_IN; ++i)
	{
		if (p.hse_inpar [i][IO_STATE] & POLARITY_BIT)
			bit_set (in_polarity, i);
		else
			bit_reset (in_polarity, i);
		if ((p.hse_inpar [i][IO_BASIC_FUNC] == BRAKE_CHECK) &&
				(p.hse_inpar [i][IO_SUB_FUNC] & (BRAKE1 | BRAKE2 | BRAKE3)) &&
				((p.hse_inpar[i][IO_ENABLE] == ENABLE) || p.ucmp_enable))		//����UCMP����ʱ����ؼ�Ⱨբ�����Լ��Ӵ�������
			brake_check |= p.hse_inpar [i][IO_SUB_FUNC];
		if ((p.hse_inpar [i][IO_BASIC_FUNC] == CONT_CHECK) &&
				(p.hse_inpar [i][IO_SUB_FUNC] & (D_RUNCONT_SUP | D_FXCONT_SUP | D_BRKCONT_SUP)) &&
				((p.hse_inpar[i][IO_ENABLE] == ENABLE) || p.ucmp_enable))
			contactor_check |= p.hse_inpar [i][IO_SUB_FUNC];
		if ((p.hse_inpar [i][IO_BASIC_FUNC] == SPECIAL_FUNC) &&
				(p.hse_inpar [i][IO_SUB_FUNC] == FIRE_CALL1) &&
				(p.hse_inpar[i][IO_ENABLE] == ENABLE))
			firecall_exist = 1;
		if ((p.hse_inpar [i][IO_BASIC_FUNC] == POS_IN) &&
				(p.hse_inpar[i][IO_ENABLE] == ENABLE))
			switchexist |= p.hse_inpar [i][IO_SUB_FUNC];
	}
	for (i = 0; i < MAX_HSE_EX_IN; ++i)
	{
		if (p.hse_ex_inpar [i][IO_STATE] & POLARITY_BIT)
			bit_set (in_polarity, (i + MAX_HSE_IN));
		else
			bit_reset (in_polarity, (i + MAX_HSE_IN));
		if ((p.hse_ex_inpar [i][IO_BASIC_FUNC] == BRAKE_CHECK) &&
				(p.hse_ex_inpar [i][IO_SUB_FUNC] & (BRAKE1 | BRAKE2 | BRAKE3)) &&
				((p.hse_ex_inpar[i][IO_ENABLE] == ENABLE) || p.ucmp_enable))
			brake_check |= p.hse_ex_inpar [i][IO_SUB_FUNC];
		if ((p.hse_ex_inpar [i][IO_BASIC_FUNC] == CONT_CHECK) &&
				(p.hse_ex_inpar [i][IO_SUB_FUNC] & (D_RUNCONT_SUP | D_FXCONT_SUP | D_BRKCONT_SUP)) &&
				((p.hse_ex_inpar[i][IO_ENABLE] == ENABLE) || p.ucmp_enable))
			contactor_check |= p.hse_ex_inpar [i][IO_SUB_FUNC];
		if ((p.hse_ex_inpar [i][IO_BASIC_FUNC] == SPECIAL_FUNC) &&
				(p.hse_ex_inpar [i][IO_SUB_FUNC] == FIRE_CALL1) &&
				(p.hse_ex_inpar[i][IO_ENABLE] == ENABLE))
			firecall_exist = 1;
		if ((p.hse_ex_inpar [i][IO_BASIC_FUNC] == POS_IN) &&
				(p.hse_ex_inpar[i][IO_ENABLE] == ENABLE))
			switchexist |= p.hse_ex_inpar [i][IO_SUB_FUNC];
	}

	oldin_polarity = in_polarity;
	for (i = 0; i < MAX_FVE_IN; i++)
	{
		if ((p.fve_inpar [i][IO_BASIC_FUNC] == SPECIAL_FUNC) &&	
				(p.fve_inpar [i][IO_SUB_FUNC] == FIRE_CALL1) &&
				(p.fve_inpar [i][IO_ENABLE] == ENABLE))
			firecall_exist = 1; 
		if ((p.fve_inpar [i][IO_BASIC_FUNC] == POS_IN) &&
				(p.fve_inpar [i][IO_ENABLE] == ENABLE))
			switchexist |= p.fve_inpar [i][IO_SUB_FUNC];
	}
	for (i = 0; i < MAX_FVE_EX_IN; i++)
	{
		if ((p.fve_ex_inpar [i][IO_BASIC_FUNC] == SPECIAL_FUNC) &&	
				(p.fve_ex_inpar [i][IO_SUB_FUNC] == FIRE_CALL1) &&
				(p.fve_ex_inpar [i][IO_ENABLE] == ENABLE))
			firecall_exist = 1; 
		if ((p.fve_ex_inpar [i][IO_BASIC_FUNC] == POS_IN) &&
				(p.fve_ex_inpar [i][IO_ENABLE] == ENABLE))
			switchexist |= p.fve_inpar [i][IO_SUB_FUNC];
	}
	for (i = 0; i < MAX_HSE_OUT; i++)
	{
		if (p.hse_outpar [i][IO_STATE] & POLARITY_BIT)
			bit_set (out_polarity, i);
		else
			bit_reset (out_polarity, i);
	}
	for (i = 0; i < MAX_HSE_EX_OUT; i++)
	{
		if (p.hse_ex_outpar [i][IO_STATE] & POLARITY_BIT)
			bit_set (out_polarity, (i+MAX_HSE_OUT));
		else
			bit_reset (out_polarity, (i+MAX_HSE_OUT));
	}
	ClrWdt();

	tse_enabled = HSE_MARIN[p.tse_number];
	exe_enabled = HSE_MARIN[p.exe_number];
	for (i = 0; i < p.tse_number; i++)
	{
		for (j = 0; j < MAX_TSE_IN; j++)
		{
			if ((p.tse_inpar [i][j][IO_BASIC_FUNC] == SPECIAL_FUNC) &&
			(p.tse_inpar [i][j][IO_SUB_FUNC] == FIRE_CALL1) &&
			(p.tse_inpar [i][j][IO_ENABLE] == ENABLE))
			{
				firecall_exist = 1;
				break;
			}
		}
		if (firecall_exist)
			break;
	}
	for (i = 0; i < MAX_ESE_DW; i++)
	{
		ese_enabled [i] = 0;
	}
	for (i = 0; i < p.ese_number/32; i++)
		ese_enabled [i] = 0xffffffff;
	ese_enabled[p.ese_number/32] = HSE_MARIN[p.ese_number%32];
	if (!firecall_exist)
	{
		for (i = 0; i < p.ese_number; i++)
		{
			for (j = 0; j < MAX_ESE_IN; j++)
			{
				if ((p.ese_inpar [i][j][IO_BASIC_FUNC] == SPECIAL_FUNC) &&
				(p.ese_inpar [i][j][IO_SUB_FUNC] == FIRE_CALL1) &&
				(p.ese_inpar [i][j][IO_ENABLE] == ENABLE) &&
				(p.ese_inpar [i][j][IO_LIFT]))
				{
					firecall_exist = 1;
					break;
				}
			}
			if (firecall_exist)
				break;
		}
	}	
}

#pragma arm section rwdata = "NOINIT_RAM", zidata = "NOINIT_RAM"
WORD pattern;
#pragma arm section

/****************************************************
* ������: main
* ����: ���������
* ���: ��
* ����: ��
* ����ֵ: 0
****************************************************/
int main (void)
{
	LpcCpuInit();
	LpcPinInit();
	LpcSpiInit();
	LpcUartInit();
	ClrWdt();
	delay(100);
	LpcI2cInit();
	ClrWdt();
	delay(100);
	GetRtc();

	ClrWdt();
	input[2] = READ_INPUTS;
	delay(5);
	input[1] = READ_INPUTS;
	delay(5);
	input[0] = READ_INPUTS;
	para_valid = 0;
	if (VerifyChecksum())
	{
		hse_state = H_EE_ERROR;
		hse_text = HT_EE_ERROR;
		LpcCanInit();
		LpcTimerInit();
		while ((!para_valid) || (iic_busy))
		{
			cycle();
		}
		InitVariables();
		InitParameter();
	}
	else
	{
		para_valid = 1;
		InitVariables();
		InitParameter();
		LpcCanInit();
		LpcTimerInit();
	}

	strcpy((char*)version_drv, "----");
	if (pattern == WD_PATTERN)
		write_errorhist (E_INTERNAL_SW, HSE_ID, 0, W_INTERNAL_SW);
	else
		write_errorhist (E_SW_START, HSE_ID, 0, 0);
	if (rtc_lowpower)
		write_errorhist (E_RTCLOWPOWER, HSE_ID, 0, 0);
	pattern = WD_PATTERN;
	power24v = PWR_IN;		

 	hse_state = H_SEARCH;
	hse_text = HT_SEARCH;
	level_display (0xFF, O_ALL);
	hb_error = 0;
	stick_dect_timer = timer + 5;
	memset(floortried, 0x00, sizeof(floortried));
	exe_state_request = exe_enabled & 0x01;	
	tse_state_request = tse_enabled & 0x01;	

	while(1)
	{
		ClrWdt();
		switch (hse_state)							/* HSE state machine						*/
		{
			case (H_SEARCH):
				search_state ();					/* active state search						*/
				set_state ();						/* set state								*/
				break;
				
			case (H_INIT):
				ini_state ();						/* init state								*/
				break;
				
			case (H_INSP):
				inspection_state ();					/* inspection mode						*/
				break;

			case (H_EM_OP):
				em_op_state ();					/* emergency recall operation				*/
				break;

			case (H_ERROR):
				error_state ();						/* error state								*/
				break;
				
			case (H_CAN_ERROR):
				can_error_state ();					/* can error state							*/
				break;
				
			case (H_DRIVE_ERROR):
				drive_error_state ();					/* drive error state						*/
				break;
				
			case (H_TERM):
				terminal_state ();					/* terminal mode (lift controlled from keypad)	*/
				break;
				
			case (H_HB_ERROR):
				hb_error_state ();					/* building site mode						*/
				break;
			case (H_SC_ERROR):
				sc_error_state ();					/* wait for safety circuit					*/
				break;
			case (H_NO_SETUP):
				nosetup_state ();					/* positioning setup not ready				*/
				break;
			case (H_RESET):
				reset_trip_state ();					/* reset trip to start positioning count		*/
				break;
			case (H_DOOROPEN):
				door_open_state ();				/* door open and not in normal state		*/
				break;
			case (H_MOVING):
				moving_state ();					/* normal trip							*/
				break;
			case (H_ARRIVAL):
				arrival_state ();					/* arrival								*/
				break;
			case (H_DIVING):
				diving_state ();					/* move to next floor						*/
				break;
			case (H_WAIT_STOP):
				wait_for_stop_state ();				/* wait until drive is stopped				*/
				break;
			case (H_STANDSTILL):
				standstill_state ();					/* Standstill								*/
				break;
			case (H_START):
				start_trip_state ();					/* start a normal trip						*/
				break;
			case (H_TEACH_IN):
				teach_in_state ();					/* teach in state							*/
				break;
			case (H_ERROR_RELEV):
				error_relev ();						/* standstill in error state					*/
				break;
			case (H_DOORTEST):
				doortest_state ();					/* doortest mode (door controlled from keypad)	*/
				break;
			case (H_EVACUATION):
				evacuation_state ();				/* emeregency evacuation state (UPS)		*/
				break;
			case (H_IV_TEACH_IN):
				motorturn_state ();
				break;
			default:
				defaultstate ();
				break;
		}
	}
}

