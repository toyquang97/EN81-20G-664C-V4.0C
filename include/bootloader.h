
#ifndef _BOOTLOADER_H_
#define _BOOTLOADER_H_


#define	NOINIT_RAM_ADDR			0x20083ff0ul			//
#define	WD_PATTERN				0x5AA5					//	Variables for restart reason

#define	APP_UPGRADE_ENTRY		0x200


#define	APP_ADDRESS				0x4000
#define	RESET_HANDLER			0x4004
#define	NMI_Handler				0x4008
#define	HardFault_Handler		0x400c
#define	MemManage_Handler		0x4010
#define	BusFault_Handler		0x4014
#define	UsageFault_Handler		0x4018
#define	REVERSED_01				0x401c
#define	REVERSED_02				0x4020
#define	REVERSED_03				0x4024
#define	REVERSED_04				0x4028
#define	SVC_Handler				0x402c
#define	DebugMon_Handler		0x4030
#define	REVERSED_05				0x4034
#define	PendSV_Handler			0x4038
#define	SysTick_Handler			0x403c


#define	WDT_IRQHandler			0x4040
#define	TIMER0_IRQHandler		0x4044
#define	TIMER1_IRQHandler		0x4048
#define	TIMER2_IRQHandler		0x404c
#define	TIMER3_IRQHandler		0x4050
#define	UART0_IRQHandler		0x4054
#define	UART1_IRQHandler		0x4058
#define	UART2_IRQHandler		0x405c
#define	UART3_IRQHandler		0x4060
#define	PWM1_IRQHandler			0x4064
#define	I2C0_IRQHandler			0x4068
#define	I2C1_IRQHandler			0x406c
#define	I2C2_IRQHandler			0x4070
#define	SPI_IRQHandler			0x4074
#define	SSP0_IRQHandler			0x4078
#define	SSP1_IRQHandler			0x407c
#define	PLL0_IRQHandler			0x4080
#define	RTC_IRQHandler			0x4084
#define	EINT0_IRQHandler		0x4088
#define	EINT1_IRQHandler		0x408c
#define	EINT2_IRQHandler		0x4090
#define	EINT3_IRQHandler		0x4094
#define	ADC_IRQHandler			0x4098
#define	BOD_IRQHandler			0x409c
#define	USB_IRQHandler			0x40a0
#define	CAN_IRQHandler			0x40a4
#define	DMA_IRQHandler			0x40a8
#define	I2S_IRQHandler			0x40ac
#define	ENET_IRQHandler			0x40b0
#define	RIT_IRQHandler			0x40b4
#define	MCPWM_IRQHandler		0x40b8
#define	QEI_IRQHandler			0x40bc
#define	PLL1_IRQHandler			0x40c0
#define	USB_Wakeup_IRQhandler	0x40c4
#define	CAN_Wakeup_IRQhandler	0x40c8


#define	IDENTITY_ADDR			0x0007FFD8ul
#define	ID_NUMS					4
#define	SW_VERSION_ADDR			0x0007FFE8ul
#define	SW_SIZE					6
#define	HW_VERSION_ADDR			0x0007FFEEul
#define	HW_SIZE					6
#define	DEVICE_NAME_ADDR		0x0007FFF4ul
#define	DV_SIZE					6
#define	FVER_ADDR				0x0007FFFAul
#define	FVER_SIZE				2
#define	CHECKSUM_ADDR			0x0007FFFCul

#define	VERSION_CONTENT			0xAA5555AAul


#define	UPDATE_FILENAME			"g-584.bin"
#define	BIN_FILE_SIZE			0x80000ul

#endif
