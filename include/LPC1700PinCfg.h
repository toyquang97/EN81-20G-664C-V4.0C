
#ifndef __LPC1700PINCFG_H
#define __LPC1700PINCFG_H

/*********************************************************************************************************
** P0�����Ź�������
*********************************************************************************************************/

/*
 *  P0.0
 */
#define P0_0_GPIO             0x00ul                                      /* GPIO                         */
#define P0_0_RD1              0x01ul                                      /* CAN-1��������������          */
#define P0_0_TXD3             0x02ul                                      /* UART3��������                */
#define P0_0_SDA1             0x03ul                                      /* I2C-1�����ߣ���©��          */

#define P0_0_FUNC             P0_0_RD1

/*
 *  P0.1
 */
#define P0_1_GPIO             0x00ul                                      /* GPIO                         */
#define P0_1_TD1              0x01ul                                      /* CAN-1��������������          */
#define P0_1_RXD3             0x02ul                                      /* UART3��������                */
#define P0_1_SCL1             0x03ul                                      /* I2C-1ʱ���ߣ���©��          */

#define P0_1_FUNC             P0_1_TD1
		
/*
 *  P0.2
 */
#define P0_2_GPIO             0x00ul                                      /* GPIO                         */
#define P0_2_TXD0             0x01ul                                      /* UART0��������                */
#define P0_2_AD07             0x02ul                                      /* AD0.7����                    */
//...   Reserved              0x03

#define P0_2_FUNC             P0_2_GPIO

/*
 *  P0.3
 */
#define P0_3_GPIO             0x00ul                                      /* GPIO                         */
#define P0_3_RXD0             0x01ul                                      /* UART0��������                */
#define P0_3_AD06             0x02ul                                      /* AD0.6����                    */
//...   Reserved              0x03 

#define P0_3_FUNC             P0_3_GPIO//P0_3_RXD0

/*
 *  P0.4
 */
#define P0_4_GPIO             0x00ul                                      /* GPIO                         */
#define P0_4_I2SRX_CLK        0x01ul                                      /* I2S���߽���ʱ��SCK           */
#define P0_4_RD2              0x02ul                                      /* CAN-2������                  */
#define P0_4_CAP20            0x03ul                                      /* ��ʱ��2�������룬ͨ��0       */
		
#define P0_4_FUNC             P0_4_GPIO

/*
 *  P0.5
 */
#define P0_5_GPIO             0x00ul                                      /* GPIO                         */
#define P0_5_I2SRX_WS         0x01ul                                      /* I2S���߽�����ѡ��WS          */
#define P0_5_TD2              0x02ul                                      /* CAN-2������                  */
#define P0_5_CAP21            0x03ul                                      /* ��ʱ��2�������룬ͨ��1       */
		
#define P0_5_FUNC             P0_5_GPIO

/*
 *  P0.6
 */
#define P0_6_GPIO             0x00ul                                      /* GPIO                         */
#define P0_6_I2SRX_SDA        0x01                                      /* I2S���߽���������SD          */
#define P0_6_SSEL1            0x02ul                                      /* SSP-1���ߴӻ�ѡ��            */
#define P0_6_MAT20            0x03ul                                      /* ��ʱ��2ƥ�������ͨ��0       */
		
#define P0_6_FUNC             P0_6_GPIO

/*
 *  P0.7
 */
#define P0_7_GPIO             0x00ul                                      /* GPIO                         */
#define P0_7_I2STX_CLK        0x01ul                                      /* I2S���߷���ʱ����SCK         */
#define P0_7_SSCK1            0x02ul                                      /* SSP-1ʱ����                  */
#define P0_7_MAT21            0x03ul                                      /* ��ʱ��2ƥ�������ͨ��1       */
		
#define P0_7_FUNC             P0_7_SSCK1

/*
 *  P0.8
 */
#define P0_8_GPIO             0x00ul                                      /* GPIO                         */
#define P0_8_I2STX_WS         0x01ul                                      /* I2S���߷�����ѡ��WS          */
#define P0_8_SMISO1           0x02ul                                      /* SSP-1����������������      */
#define P0_8_MAT22            0x03ul                                      /* ��ʱ��2ƥ�������ͨ��2       */

#define P0_8_FUNC             P0_8_SMISO1
		
/*
 *  P0.9
 */
#define P0_9_GPIO             0x00ul                                      /* GPIO                         */
#define P0_9_I2STX_SDA        0x01ul                                      /* I2S���߷���������SD          */
#define P0_9_SMOSI1           0x02ul                                      /* SSP-1�����������������      */
#define P0_9_MAT23            0x03ul                                      /* ��ʱ��2ƥ�������ͨ��3       */

#define P0_9_FUNC             P0_9_SMOSI1

/*
 *  P0.10
 */
#define P0_10_GPIO            0x00ul                                      /* GPIO                         */
#define P0_10_TXD2            0x01ul                                      /* UART2��������                */
#define P0_10_SDA2            0x02ul                                      /* I2C-2������(�ǿ�©����)      */
#define P0_10_MAT30           0x03ul                                      /* ��ʱ��3ƥ�������ͨ��0       */

#define P0_10_FUNC            P0_10_GPIO              
  
/*
 *  P0.11
 */
#define P0_11_GPIO            0x00                                      /* GPIO                         */
#define P0_11_RXD2            0x01                                      /* UART2��������                */
#define P0_11_SCL2            0x02                                      /* I2C-2ʱ����(�ǿ�©����)      */
#define P0_11_MAT31           0x03                                      /* ��ʱ��3ƥ�������ͨ��1       */
 
#define P0_11_FUNC            P0_11_GPIO              
    
/*
 *  P0.15
 */
#define P0_15_GPIO            0x00ul                                      /* GPIO                         */
#define P0_15_TXD1            0x01ul                                      /* UART1��������                */
#define P0_15_SCK0            0x02ul                                      /* SSP0ʱ����                   */
#define P0_15_SCK             0x03ul                                      /* SPIʱ����                    */
                                                   
#define P0_15_FUNC            P0_15_SCK0              
                                                   
/*
 *  P0.16
 */
#define P0_16_GPIO            0x00ul                                      /* GPIO                         */
#define P0_16_RXD1            0x01ul                                      /* UART1��������                */
#define P0_16_SSEL0           0x02ul                                      /* SSP0�ӻ�ѡ��                 */
#define P0_16_SSEL            0x03ul                                      /* SPI�ӻ�ѡ��                  */
                                                
#define P0_16_FUNC            P0_16_GPIO           
                                                
/*
 *  P0.17
 */
#define P0_17_GPIO            0x00                                      /* GPIO                         */
#define P0_17_CTS1            0x01                                      /* UART1���㷢���ź�CTS         */
#define P0_17_MISO0           0x02                                      /* SSP0����������������       */
#define P0_17_MISO            0x03                                      /* SPI����ӳ�������            */
                                                
#define P0_17_FUNC            P0_17_MISO0           
                                               
/*
 *  P0.18
 */
#define P0_18_GPIO            0x00ul                                      /* GPIO                         */
#define P0_18_DCD1            0x01ul                                      /* UART1�����ز��������DCD     */
#define P0_18_MOSI0           0x02ul                                      /* SSP-0�����������������      */
#define P0_18_MOSI            0x03ul                                      /* SPI��������������            */
                                                
#define P0_18_FUNC            P0_18_MOSI0           
                                                
/*
 *  P0.19
 */
#define P0_19_GPIO            0x00                                      /* GPIO                         */
#define P0_19_DSR1            0x01                                      /* UART1�������þ���DSR         */
//...   Reserved              0x02                                      /* ����                         */
#define P0_19_SDA1            0x03                                      /* I2C-1�����ߣ��ǿ�©��        */
                                                
#define P0_19_FUNC            P0_19_GPIO           
                                                
/*
 *  P0.20
 */
#define P0_20_GPIO            0x00ul                                      /* GPIO                         */
#define P0_20_DTR1            0x01ul                                      /* UART1�ն˾���DTR             */
//...   Reserved              0x02ul                                  	/* ����                         */
#define P0_20_SCL1            0x03ul                                      /* I2C-1ʱ���ߣ��ǿ�©��        */
                                           
#define P0_20_FUNC            P0_20_GPIO 
                                      
/*
 *  P0.21
 */
#define P0_21_GPIO            0x00ul                                      /* GPIO                         */
#define P0_21_RI1             0x01ul                                      /* UART1�����ź�                */
//...   Reserved              0x02ul                                      /* ����                         */
#define P0_21_RD1             0x03ul                                      /* CAN-1��������                */
                                      
#define P0_21_FUNC            P0_21_GPIO 
                                      
/*
 *  P0.22
 */
#define P0_22_GPIO            0x00ul                                      /* GPIO                         */
#define P0_22_RTS1            0x01ul                                      /* UART1����������            */
//...   Reserved              0x02ul                                      /* ����                         */
#define P0_22_TD1             0x03ul                                      /* CAN-1������                  */
                                      
#define P0_22_FUNC            P0_22_GPIO 
                                      
/*
 *  P0.23
 */
#define P0_23_GPIO            0x00ul                                      /* GPIO                         */
#define P0_23_AD00            0x01ul                                      /* ADC-0��ͨ��0                 */
#define P0_23_I2SRX_CLK       0x02ul                                      /* I2S����ʱ��SCK               */
#define P0_23_CAP30           0x03ul                                      /* ��ʱ��3�������룬ͨ��0       */
                                      
#define P0_23_FUNC            P0_23_AD00 
                                      
/*
 *  P0.24
 */
#define P0_24_GPIO            0x00ul                                      /* GPIO                         */
#define P0_24_AD01            0x01ul                                      /* ADC-0��ͨ��1                 */
#define P0_24_I2SRX_WS        0x02ul                                      /* I2S������ѡ��WS              */
#define P0_24_CAP31           0x03ul                                      /* ��ʱ��3�������룬ͨ��1       */
                                      
#define P0_24_FUNC            P0_24_AD01 
                                      
/*
 *  P0.25
 */
#define P0_25_GPIO            0x00ul                                      /* GPIO                         */
#define P0_25_AD02            0x01ul                                      /* ADC-0��ͨ��2                 */
#define P0_25_I2SRX_SDA       0x02ul                                      /* I2S��������SD                */
#define P0_25_TXD3            0x03ul                                      /* UART3��������                */
                                  
#define P0_25_FUNC            P0_25_GPIO     
                                          
/*
 *  P0.26
 */
#define P0_26_GPIO            0x00ul                                      /* GPIO                         */
#define P0_26_AD03            0x01ul                                      /* ADC-0��ͨ��3                 */
#define P0_26_AOUT            0x02ul                                      /* DAC���                      */
#define P0_26_RXD3            0x03ul                                      /* UART-3��������               */
                                          
#define P0_26_FUNC            P0_26_GPIO     
                                          
/*
 *  P0.27
 */
#define P0_27_GPIO            0x00ul                                      /* GPIO                         */
#define P0_27_SDA0            0x01ul                                      /* I2C0��������/�������©      */
#define P0_27_USB_SDA         0x02ul                                      /* USB�ӿ�I2C��������OTG�շ���  */
//..    Reserved              0x03ul                                      /* ����                         */
                                          
#define P0_27_FUNC            P0_27_SDA0     
                                          
/*
 *  P0.28
 */
#define P0_28_GPIO            0x00ul                                      /* GPIO                         */
#define P0_28_SCL0            0x01ul                                      /* I2C0ʱ�����룬��©           */
#define P0_28_USB_SCL         0x02ul                                      /* USB�ӿ�I2Cʱ���ߣ�OTG�շ���  */
//..    Reserved              0x03ul                                   /* ����                         */
                                      
#define P0_28_FUNC            P0_28_SCL0 
                                      
/*
 *  P0.29
 */
#define P0_29_GPIO            0x00ul                                      /* GPIO                         */
#define P0_29_USB_DD          0x01ul                                      /* USB˫������D+��              */
//..    Reserved              0x02ul                                      /* ����                         */
//..    Reserved              0x03ul                                      /* ����                         */
                                      
#define P0_29_FUNC            P0_29_GPIO 
                                      
/*
 *  P0.30
 */
#define P0_30_GPIO            0x00ul                                      /* GPIO                         */
#define P0_30_USB_DS          0x01ul                                      /* USB˫������D-��              */
//..    Reserved              0x02ul                                      /* ����                         */
//..    Reserved              0x03ul                                      /* ����                         */
                                      							  
#define P0_30_FUNC            P0_30_GPIO 

/*********************************************************************************************************
** P1�����Ź�������
*********************************************************************************************************/

/*
 *  P1.0
 */
#define P1_0_GPIO             0x00ul                                      /* GPIO                         */
#define P1_0_ENET_TXD0        0x01ul                                      /* ��̫���������ݽ�0            */
//..    Reserved              0x02ul                                      /* ����                         */
//..    Reserved              0x03ul                                      /* ����                         */

#define P1_0_FUNC             P1_0_GPIO

/*
 *  P1.1
 */
#define P1_1_GPIO             0x00ul                                      /* GPIO                         */
#define P1_1_ENET_TX_EN       0x01ul                                      /* ��̫���������ݽ�1            */
//..    Reserved              0x02ul                                      /* ����                         */
//..    Reserved              0x03ul                                      /* ����                         */

#define P1_1_FUNC             P1_1_GPIO

/*
 *  P1.4
 */
#define P1_4_GPIO             0x00ul                                      /* GPIO                         */
#define P1_4_ENET_TX_EN       0x01ul                                      /* ��̫������ʹ��               */
//..    Reserved              0x02ul                                      /* ����                         */
//..    Reserved              0x03ul                                      /* ����                         */

#define P1_4_FUNC             P1_4_GPIO

/*
 *  P1.8
 */
#define P1_8_GPIO             0x00ul                                      /* GPIO                         */
#define P1_8_ENET_CRS         0x01ul                                      /* ��̫���ز�����             */
//..    Reserved              0x02ul                                      /* ����                         */
//..    Reserved              0x03ul                                      /* ����                         */
										     
#define P1_8_FUNC             P1_8_GPIO

/*
 *  P1.9
 */
#define P1_9_GPIO             0x00ul                                      /* GPIO                         */
#define P1_9_ENET_RXD0        0x01ul                                      /* ��̫���������ݽ�0            */
//..    Reserved              0x02ul                                      /* ����                         */
//..    Reserved              0x03ul                                      /* ����                         */
													  
#define P1_9_FUNC             P1_9_GPIO

/*
 *  P1.10
 */
#define P1_10_GPIO            0x00ul                                      /* GPIO                         */
#define P1_10_ENET_RXD1       0x01ul                                      /* ��̫���������ݽ�1            */
//..    Reserved              0x02ul                                      /* ����                         */
//..    Reserved              0x03ul                                      /* ����                         */

#define P1_10_FUNC            P1_10_GPIO

/*
 *  P1.11
 */
#define P1_14_GPIO            0x00ul                                      /*  GPIO                        */
#define P1_14_ENET_RX_ER      0x01ul                                      /* ��̫�����մ���               */
//..    Reserved              0x02ul                                      /* ����                         */
//..    Reserved              0x03ul                                      /* ����                         */
													     
#define P1_14_FUNC            P1_14_GPIO

/*
 *  P1.15
 */
#define P1_15_GPIO            0x00ul                                      /* GPIO                         */
#define P1_15_ENET_REF_CLK    0x01ul                                      /* ��̫���ο�ʱ������           */
//..    Reserved              0x02ul                                      /* ����                         */
//..    Reserved              0x03ul                                      /* ����                         */
											   
#define P1_15_FUNC            P1_15_GPIO

/*
 *  P1.16
 */
#define P1_16_GPIO            0x00ul                                      /* GPIO                         */
#define P1_16_ENET_MDC        0x01ul                                      /* ��̫��MIIMʱ��               */
//..    Reserved              0x02ul                                      /* ����                         */
//..    Reserved              0x03ul                                      /* ����                         */

#define P1_16_FUNC            P1_16_GPIO

/*
 *  P1.17
 */
#define P1_17_GPIO            0x00ul                                      /* GPIO                         */
#define P1_17_ENET_MDIO       0x01ul                                      /* ��̫��MI��������\���        */
//..    Reserved              0x02ul                                      /* ����                         */
//..    Reserved              0x03ul                                      /* ����                         */

#define P1_17_FUNC            P1_17_GPIO

/*
 *  P1.18
 */
#define P1_18_GPIO            0x00ul                                      /*  GPIO                        */
#define P1_18_USB_UP_LED      0x01ul                                      /* USB GOODLINKָʾ��           */
#define P1_18_PWM11           0x02ul                                      /* PWM1�����ͨ��1              */
#define P1_18_CAP10           0x03ul                                      /* Timer1�������룬ͨ��0        */
                                              
#define P1_18_FUNC            P1_18_GPIO    
                                             
/*
 *  P1.19
 */
#define P1_19_GPIO            0x00ul                                      /*  GPIO                        */
#define P1_19_MCO0A           0x01ul                                      /* �������PWMͨ��0�����A      */
#define P1_19_USB_PPWR        0x02ul                                      /* USB�˿ڵ�Դʹ���ź�          */
#define P1_19_CAP11           0x03ul                                      /* Timer1�������룬ͨ��1        */
                                              
#define P1_19_FUNC            P1_19_GPIO    
                                              
/*
 *  P1.20
 */
#define P1_20_GPIO            0x00ul                                      /*  GPIO                        */
#define P1_20_MCFB0           0x01ul                                      /* �������PWMͨ��0���ػ�����   */
#define P1_20_PWM12           0x02ul                                      /* PWM-2�����ͨ��2             */
#define P1_20_SCK0            0x03ul                                      /* SSP-0ʱ������                */
                                              
#define P1_20_FUNC            P1_20_GPIO    
                                             
/*
 *  P1.21
 */
#define P1_21_GPIO            0x00ul                                      /* GPIO                         */
#define P1_21_MCABORT         0x01ul                                      /* �������PWM��������ֹ        */
#define P1_21_PWM13           0x02ul                                      /* PWM-1�����ͨ��3             */
#define P1_21_SSEL0           0x03ul                                      /* SSP-0�ӻ�ѡ��                */
                                              
#define P1_21_FUNC            P1_21_GPIO    
                                             
/*
 *  P1.22
 */
#define P1_22_GPIO            0x00ul                                      /* GPIO                         */
#define P1_22_MC0B            0x01ul                                      /* �������PWMͨ��0�����B      */
#define P1_22_USB_PWRD        0x02ul                                      /* USB�˿ڵĵ�Դ״̬            */
#define P1_22_MAT10           0x03ul                                      /* ��ʱ��1ƥ�������ͨ��0       */
                                              
#define P1_22_FUNC            P1_22_GPIO    
                                              
/*
 *  P1.23
 */
#define P1_23_GPIO            0x00ul                                      /* GPIO                         */
#define P1_23_MCFB1           0x01ul                                      /* �������PWMͨ��1����������   */
#define P1_23_PWM14           0x02ul                                      /* PWM-1�����ͨ��4             */
#define P1_23_MISO0           0x03ul                                      /* SSP-0�������룬�ӻ����      */
                                              
#define P1_23_FUNC            P1_23_GPIO    
                                              
/*
 *  P1.24
 */
#define P1_24_GPIO            0x00ul                                      /* GPIO                         */
#define P1_24_MCFB2           0x01ul                                      /* �������PWMͨ��2����������   */
#define P1_24_PWM15           0x02ul                                      /* PWM-1�����ͨ��5             */
#define P1_24_MOSI0           0x03ul                                      /* SSP-0����������ӻ�����      */
                                              
#define P1_24_FUNC            P1_24_GPIO    
                                             
/*
 *  P1.25
 */
#define P1_25_GPIO            0x00ul                                      /* GPIO                         */
#define P1_25_MC1A            0x01ul                                      /* �������PWMͨ��1�����A      */
#define P1_25_CLKOUT          0x02ul                                      /* ʱ���������                 */
#define P1_25_MAT11           0x03ul                                      /* ��ʱ��1ƥ�������ͨ��1       */
                                              
#define P1_25_FUNC            P1_25_GPIO    
                                              
/*
 *  P1.26
 */
#define P1_26_GPIO            0x00ul                                      /* GPIO                         */
#define P1_26_MC1B            0x01ul                                      /* �������PWMͨ��1�����B      */
#define P1_26_PWM16           0x02ul                                      /* PWM-1�����ͨ��6             */
#define P1_26_CAP00           0x03ul                                      /* ��ʱ��0�������룬ͨ��0       */
                                              
#define P1_26_FUNC            P1_26_GPIO    
                                         
/*
 *  P1.27
 */
#define P1_27_GPIO            0x00ul                                      /* GPIO                         */
#define P1_27_CLKOUT          0x01ul                                      /* ʱ���������                 */
#define P1_27_USB_OVRCR       0x02ul                                      /* USB�˿ڹ��ص���״̬          */
#define P1_27_CAP01           0x03ul                                      /* ��ʱ��0�������룬ͨ��1       */
                                              
#define P1_27_FUNC            P1_27_GPIO    
                                              
/*
 *  P1.28
 */
#define P1_28_GPIO            0x00ul                                      /* GPIO                         */
#define P1_28_MC2A            0x01ul                                      /* �������PWMͨ��2�����A      */
#define P1_28_PCAP10          0x02ul                                      /* PWM-1�������룬ͨ��0         */
#define P1_28_MAT00           0x03ul                                      /* ��ʱ��0ƥ�������ͨ��0       */
                                              
#define P1_28_FUNC            P1_28_GPIO    
                                              
/*
 *  P1.29
 */
#define P1_29_GPIO            0x00ul                                      /* GPIO                         */
#define P1_29_MC2B            0x01ul                                      /* �������PWMͨ��2�����B      */
#define P1_29_PCAP11          0x02ul                                      /* PWM-1�������룬ͨ��1         */
#define P1_29_MAT01           0x03ul                                      /* ��ʱ��0ƥ�������ͨ��1       */
                                              
#define P1_29_FUNC            P1_29_GPIO    
                                              
/*
 *  P1.30
 */
#define P1_30_GPIO            0x00ul                                      /* GPIO                         */
//...   Reserved              0x01ul                                      /* ����                         */
#define P1_30_VBUS            0x02ul                                      /* ���ڵ�USB���ߵ�Դ            */
#define P1_30_AD04            0x03ul                                      /* ADCת����0.ͨ��4����         */
                                              
#define P1_30_FUNC            P1_30_GPIO    
                                              
/*
 *  P1.31
 */
#define P1_31_GPIO            0x00ul                                      /* GPIO                         */
//...   Reserved              0x01ul                                      /* ����                         */
#define P1_31_SSCK1           0x02ul                                      /* SSP-1ʱ����                  */
#define P1_31_AD05            0x03ul                                      /* ADC-0���룬ͨ��5             */
                                             
#define P1_31_FUNC            P1_31_GPIO   
                                              
/*********************************************************************************************************
** P2�����Ź�������
*********************************************************************************************************/

/*
 *  P2.0
 */
#define P2_0_GPIO             0x00ul                                      /* GPIO                         */
#define P2_0_PWM11            0x01ul                                      /* PWM-1��ͨ��1                 */
#define P2_0_TXD1             0x02ul                                      /* UART1���ݷ�������            */
//...   Reserved              0x03ul                                      /* ����                         */

#define P2_0_FUNC             P2_0_GPIO

/*
 *  P2.1
 */
#define P2_1_GPIO             0x00ul                                      /* GPIO                         */
#define P2_1_PWM12            0x01ul                                      /* PWM-1��ͨ��2                 */
#define P2_1_RXD1             0x02ul                                      /* UART1���ݽ�������            */
//...   Reserved              0x03ul                                      /* ����                         */

#define P2_1_FUNC             P2_1_GPIO

/*
 *  P2.2
 */
#define P2_2_GPIO             0x00ul                                      /* GPIO                         */
#define P2_2_PWM13            0x01ul                                      /* PWM-1��ͨ��3                 */
#define P2_2_CTS1             0x02ul                                      /* UART1���㷢����              */
//...   Reserved              0x03ul                                      /* ����                         */

#define P2_2_FUNC             P2_2_GPIO

/*
 *  P2.3
 */
#define P2_3_GPIO             0x00ul                                      /* GPIO                         */
#define P2_3_PWM14            0x01ul                                      /* PWM-1��ͨ��4                 */
#define P2_3_DCD1             0x02ul                                      /* UART1�����ز������          */
//...   Reserved              0x03ul                                      /* ����                         */

#define P2_3_FUNC             P2_3_GPIO

/*
 *  P2.4
 */
#define P2_4_GPIO             0x00ul                                      /* GPIO                         */
#define P2_4_PWM15            0x01ul                                      /* PWM-1��ͨ��5                 */
#define P2_4_DSR1             0x02ul                                      /* UART1�������þ�����          */
//...   Reserved              0x03ul                                      /* ����                         */

#define P2_4_FUNC             P2_4_GPIO

/*
 *  P2.5
 */
#define P2_5_GPIO             0x00ul                                      /* GPIO                         */
#define P2_5_PWM16            0x01ul                                      /* PWM-1��ͨ��6                 */
#define P2_5_DTR1             0x02ul                                      /* UART1�����ն˾�����          */
//...   Reserved              0x03ul                                      /* ����                         */

#define P2_5_FUNC             P2_5_GPIO

/*
 *  P2.6
 */
#define P2_6_GPIO             0x00ul                                      /* GPIO                         */
#define P2_6_PCAP10           0x01ul                                      /* PWM-1�������룬ͨ��0         */
#define P2_6_RI1              0x02ul                                      /* UART1��������                */
//...   Reserved              0x03ul                                      /* ����                         */

#define P2_6_FUNC             P2_6_GPIO

/*
 *  P2.7
 */
#define P2_7_GPIO             0x00ul                                      /* GPIO                         */
#define P2_7_RD2              0x01ul                                      /* CAN-2������                  */
#define P2_7_RTS1             0x02ul                                      /* UART1���������            */
                                                                        /* RS-485/EIA-485���ʹ���ź�   */
//...   Reserved              0x03ul                                      /* ����                         */

#define P2_7_FUNC             P2_7_RD2

/*
 *  P2.8
 */
#define P2_8_GPIO             0x00ul                                      /* GPIO                         */
#define P2_8_TD2              0x01ul                                      /* CAN-2������                  */
#define P2_8_TXD2             0x02ul                                      /* UART2��������                */
#define P2_8_ENET_MDC         0x03ul                                      /* ��̫��MIIMʱ��               */

#define P2_8_FUNC             P2_8_TD2

/*
 *  P2.9
 */
#define P2_9_GPIO             0x00ul                                      /* GPIO                         */
#define P2_9_U1CONNECT        0x01ul                                      /* USB1�����ӿ���               */
#define P2_9_RXD2             0x02ul                                      /* UART2��������                */
#define P2_9_ENET_MDIO        0x03ul                                      /* ��̫��MIIM������������     */

#define P2_9_FUNC             P2_9_GPIO

/*
 *  P2.10��ISP��
 */
#define P2_10_GPIO            0x00ul                                      /* GPIO                         */
#define P2_10_EINT0           0x01ul                                      /* �ⲿ�ж�0����                */
#define P2_10_NMI             0x02ul                                      /* �������ж�����               */
//...   Reserved              0x03ul                                      /* ����                         */
                                              
#define P2_10_FUNC            P2_10_GPIO    
                                              
/*
 *  P2.11
 */
#define P2_11_GPIO            0x00ul                                      /* GPIO                         */
#define P2_11_EINT1           0x01ul                                      /* �ⲿ�ж�1����                */
//...   Reserved              0x02ul                                      /* ����                         */
#define P2_11_I2STX_CLK       0x03ul                                      /* I2S����ʱ��SCK               */
                                              
#define P2_11_FUNC            P2_11_EINT1    
                                              
/*
 *  P2.12
 */
#define P2_12_GPIO            0x00ul                                      /* GPIO                         */
#define P2_12_EINT2           0x01ul                                      /* �ⲿ�ж�2����                */
//...   Reserved              0x02ul                                      /* ����                         */
#define P2_12_I2STX_WS        0x03ul                                      /* I2S������ѡ��WS              */
                                              
#define P2_12_FUNC            P2_12_GPIO    
                                              
/*
 *  P2.13
 */
#define P2_13_GPIO            0x00ul                                      /* GPIO                         */
#define P2_13_EINT3           0x01ul                                      /* �ⲿ�ж�3����                */
//...   Reserved              0x02ul                                      /* ����                         */
#define P2_13_I2STX_SDA       0x03ul                                      /* I2S��������SD                */
                                              
#define P2_13_FUNC            P2_13_GPIO    
                                         
/*********************************************************************************************************
** P3�����Ź�������
*********************************************************************************************************/

/*
 *  P3.25
 */
#define P3_25_GPIO            0x00ul                                      /*  GPIO                        */
//...   Reserved              0x01ul                                      /*  ����                        */
#define P3_25_MAT00           0x02ul                                      /*  ��ʱ��0ƥ�������ͨ��0      */
#define P3_25_PWM12           0x03ul                                      /*  PWM-1�����ͨ��2            */
                                               
#define P3_25_FUNC            P3_25_GPIO     
                                               
/*
 *  P3.26
 */
#define P3_26_GPIO            0x00ul                                      /* GPIO                         */
#define P3_26_STCLK           0x01ul                                      /* ϵͳ���Ķ�ʱ��ʱ������       */
#define P3_26_MAT01           0x02ul                                      /* ��ʱ��0ƥ�������ͨ��1       */
#define P3_26_PWM13           0x03ul                                      /* PWM-1�����ͨ��3             */
                                               
#define P3_26_FUNC            P3_26_GPIO     
                                          
/*********************************************************************************************************
** P4�����Ź�������
*********************************************************************************************************/

/*
 *  P4.28
 */
#define P4_28_GPIO            0x00ul                                      /* GPIO                         */
#define P4_28_RX_MCLK         0x01ul                                      /* ��I2S��������ʱ��            */
#define P4_28_MAT20           0x02ul                                      /* ��ʱ��2ƥ�������ͨ��0       */
#define P4_28_TXD3            0x03ul                                      /* UART3��������                */
                                               
#define P4_28_FUNC            P4_28_GPIO     
                                               
/*
 *  P4.29
 */
#define P4_29_GPIO            0x00ul                                      /* GPIO                         */
#define P4_29_TX_MCLK         0x01ul                                      /* I2S��������ʱ��              */
#define P4_29_MAT21           0x02ul                                      /* ��ʱ��2ƥ�������ͨ��1       */
#define P4_29_RXD3            0x03ul                                      /* UART3��������                */
                                               
#define P4_29_FUNC            P4_29_GPIO     
 
                                          
/*********************************************************************************************************
**  P10  The Trace Port Interface Unit (TPIU)
*********************************************************************************************************/

/*
 *  bit[3]
 */
#define TPIU_Disable          0x00ul                                     /* TPIU interface ��ֹ          */
#define TPIU_Enable           0x80ul                                     /* TPIU interface ʹ��          */

#define P10_3_FUNC            TPIU_Disable

/*********************************************************************************************************
** �����ڲ���������������
*********************************************************************************************************/
#define PIN_PULL_UP           0x00ul                                      /* �����ڲ�����                 */
#define PIN_REPEATER          0x01ul                                      /* �����ڲ�����                 */
#define PIN_NO_PULL           0x02ul                                      /* ���żȲ�������Ҳ������       */
#define PIN_PULL_DOWN         0x03ul                                      /* �����ڲ�����                 */

/*
 *  P0
 */
#define P0_0_MODE             PIN_PULL_UP
#define P0_1_MODE             PIN_PULL_UP
#define P0_2_MODE             PIN_PULL_UP
#define P0_3_MODE             PIN_PULL_UP
#define P0_4_MODE             PIN_PULL_UP
#define P0_5_MODE             PIN_PULL_UP
#define P0_6_MODE             PIN_PULL_UP
#define P0_7_MODE             PIN_PULL_UP
#define P0_8_MODE             PIN_PULL_UP
#define P0_9_MODE             PIN_PULL_UP
#define P0_10_MODE            PIN_PULL_UP
#define P0_11_MODE            PIN_PULL_UP
#define P0_12_MODE            PIN_PULL_UP
#define P0_13_MODE            PIN_PULL_UP
#define P0_14_MODE            PIN_PULL_UP
#define P0_15_MODE            PIN_PULL_UP

#define P0_16_MODE            PIN_PULL_UP
#define P0_17_MODE            PIN_PULL_UP
#define P0_18_MODE            PIN_PULL_UP
#define P0_19_MODE            PIN_PULL_UP
#define P0_20_MODE            PIN_PULL_UP
#define P0_21_MODE            PIN_PULL_UP
#define P0_22_MODE            PIN_PULL_UP
#define P0_23_MODE            PIN_PULL_UP
#define P0_24_MODE            PIN_NO_PULL
#define P0_25_MODE            PIN_NO_PULL
#define P0_26_MODE            PIN_PULL_UP
#define P0_27_MODE            PIN_PULL_UP
#define P0_28_MODE            PIN_PULL_UP
#define P0_29_MODE            PIN_PULL_UP
#define P0_30_MODE            PIN_PULL_UP
#define P0_31_MODE            PIN_PULL_UP

/*
 *  P1
 */
#define P1_0_MODE             PIN_PULL_UP
#define P1_1_MODE             PIN_PULL_UP
#define P1_2_MODE             PIN_PULL_UP
#define P1_3_MODE             PIN_PULL_UP
#define P1_4_MODE             PIN_PULL_UP
#define P1_5_MODE             PIN_PULL_UP
#define P1_6_MODE             PIN_PULL_UP
#define P1_7_MODE             PIN_PULL_UP 
#define P1_8_MODE             PIN_PULL_UP
#define P1_9_MODE             PIN_PULL_UP
#define P1_10_MODE            PIN_PULL_UP
#define P1_11_MODE            PIN_PULL_UP
#define P1_12_MODE            PIN_PULL_UP
#define P1_13_MODE            PIN_PULL_UP
#define P1_14_MODE            PIN_PULL_UP
#define P1_15_MODE            PIN_PULL_UP
#define P1_16_MODE            PIN_PULL_UP
#define P1_17_MODE            PIN_PULL_UP    
#define P1_18_MODE            PIN_PULL_UP
#define P1_19_MODE            PIN_PULL_UP
#define P1_20_MODE            PIN_PULL_UP
#define P1_21_MODE            PIN_PULL_UP
#define P1_22_MODE            PIN_PULL_UP
#define P1_23_MODE            PIN_PULL_UP
#define P1_24_MODE            PIN_PULL_UP
#define P1_25_MODE            PIN_PULL_UP
#define P1_26_MODE            PIN_PULL_UP
#define P1_27_MODE            PIN_PULL_UP     
#define P1_28_MODE            PIN_PULL_UP     
#define P1_29_MODE            PIN_PULL_UP     
#define P1_30_MODE            PIN_PULL_UP     
#define P1_31_MODE            PIN_PULL_UP     

/*
 *  P2
 */
#define P2_0_MODE             PIN_PULL_UP
#define P2_1_MODE             PIN_PULL_UP
#define P2_2_MODE             PIN_PULL_UP
#define P2_3_MODE             PIN_PULL_UP
#define P2_4_MODE             PIN_PULL_UP
#define P2_5_MODE             PIN_PULL_UP
#define P2_6_MODE             PIN_PULL_UP
#define P2_7_MODE             PIN_PULL_UP
#define P2_8_MODE             PIN_PULL_UP
#define P2_9_MODE             PIN_PULL_UP
#define P2_10_MODE            PIN_PULL_UP
#define P2_11_MODE            PIN_PULL_UP
#define P2_12_MODE            PIN_PULL_UP
#define P2_13_MODE            PIN_PULL_UP
#define P2_14_MODE            PIN_PULL_UP
#define P2_15_MODE            PIN_PULL_UP

/*
 *  P3
 */
#define P3_25_MODE            PIN_PULL_UP
#define P3_26_MODE            PIN_PULL_UP

/*
 *  P4
 */
#define P4_28_MODE            PIN_PULL_UP     
#define P4_29_MODE            PIN_PULL_UP     
		
/*********************************************************************************************************
** �������λ��PCONP�ж���
*********************************************************************************************************/
#define PCTIM0_Enable         1ul                                         /* ��ֹ��ʱ��/������0����/ʱ��  */
#define PCTIM0_Disable        0ul                                         /* ʹ�ܶ�ʱ��/������0����/ʱ��  */
#define PCTIM0_FUNC           PCTIM0_Enable

#define PCTIM1_Enable         1ul                                         /* ��ֹ��ʱ��/������1����/ʱ��  */
#define PCTIM1_Disable        0ul                                         /* ��ֹ��ʱ��/������1����/ʱ��	*/
#define PCTIM1_FUNC           PCTIM1_Enable

#define PCUART0_Enable        1ul                                         /* ʹ��UART0����/ʱ�ӿ���λ     */
#define PCUART0_Disable       0ul                                         /* ��ֹUART0����/ʱ�ӿ���λ     */
#define PCUART0_FUNC          PCUART0_Enable

#define PCUART1_Enable        1ul                                         /* ʹ��UART1����/ʱ�ӿ���λ	    */
#define PCUART1_Disable       0ul                                         /* ��ֹUART1����/ʱ�ӿ���λ     */
#define PCUART1_FUNC          PCUART0_Disable

#define PWM1_Enable           1ul                                         /* ʹ��PWM1����/ʱ�ӿ���λ      */
#define PWM1_Disable          0ul                                         /* ��ֹPWM1����/ʱ�ӿ���λ      */
#define PWM1_FUNC             PWM1_Disable

#define PCI2C0_Enable         1ul                                         /* ʹ��I2C0�ӿڹ���/ʱ�ӿ���λ  */
#define PCI2C0_Disable        0ul                                         /* ��ֹI2C0�ӿڹ���/ʱ�ӿ���λ  */
#define PCI2C0_FUNC           PCI2C0_Enable

#define PCSPI_Enable          1ul                                         /* ʹ��SPI�ӿڹ���/ʱ�ӿ���λ	*/
#define PCSPI_Disable         0ul                                         /* ��ֹSPI�ӿڹ���/ʱ�ӿ���λ   */
#define PCSPI_FUNC            PCSPI_Disable

#define PCRTC_Enable          1ul                                         /* ʹ��RTC����/ʱ�ӿ���λ       */
#define PCRTC_Disable         0ul                                         /* ��ֹRTC����/ʱ�ӿ���λ       */
#define PCRTC_FUNC            PCRTC_Disable								

#define PCSSP1_Enable         1ul                                         /* ʹ��SSP1�ӿڹ���/ʱ�ӿ���λ  */
#define PCSSP1_Disable        0ul                                         /* ��ֹSSP1�ӿڹ���/ʱ�ӿ���λ  */
#define PCSSP1_FUNC           PCSSP1_Enable

#define PCAD_Enable           1ul                                         /* ʹ��A/Dת��������/ʱ�ӿ���λ */
#define PCAD_Disable          0ul                                         /* ��ֹA/Dת��������/ʱ�ӿ���λ */
#define PCAD_FUNC             PCAD_Enable

#define PCCAN1_Enable         1ul                                         /* ʹ��CAN������1����/ʱ�ӿ���λ*/
#define PCCAN1_Disable        0ul                                         /* ��ֹCAN������1����/ʱ�ӿ���λ*/
#define PCCAN1_FUNC           PCCAN1_Enable

#define PCCAN2_Enable         1ul                                         /* ʹ��CAN������2����/ʱ�ӿ���λ*/
#define PCCAN2_Disable        0ul                                         /* ��ֹCAN������2����/ʱ�ӿ���λ*/
#define PCCAN2_FUNC           PCCAN2_Enable

#define PCGPIO_Enable         1ul                                         /* ʹ��GPIO	                    */
#define PCGPIO_Disable        0ul                                         /* ��ֹGPIO                 	*/
#define PCGPIO_FUNC           PCGPIO_Enable

#define PCRIT_Enable          1ul                                         /* ʹ���ظ��ж϶�ʱ��		    */
                                                                        /* ����/ʱ�ӿ���λ              */
#define PCRIT_Disable         0ul                                         /* ��ֹ�ظ��ж϶�ʱ��           */
                                                                        /* ����/ʱ�ӿ���λ	            */
#define PCRIT_FUNC            PCRIT_Disable

#define PCMC_Enable           1ul                                         /* ʹ�ܵ������PWM              */
#define PCMC_Disable          0ul                                         /* ��ֹ�������PWM              */
#define PCMC_FUNC             PCMC_Disable

#define PCQEI_Enable          1ul                                         /* ʹ�������������ӿ�           */
                                                                        /* ����/ʱ�ӿ���λ              */
#define PCQEI_Disable         0ul                                         /* ��ֹ�����������ӿ�           */
                                                                        /* ����/ʱ�ӿ���λ              */
#define PCQEI_FUNC            PCQEI_Disable

#define PCI2C1_Enable         1ul                                         /* ʹ��I2C1�ӿڹ���/ʱ�ӿ���λ  */
#define PCI2C1_Disable        0ul                                         /* ��ֹI2C1�ӿڹ���/ʱ�ӿ���λ  */
#define PCI2C1_FUNC           PCI2C1_Disable

#define PCSSP0_Enable         1ul                                         /* ʹ��SSP0�ӿڹ���/ʱ�ӿ���λ  */
#define PCSSP0_Disable        0ul                                         /* ��ֹSSP0�ӿڹ���/ʱ�ӿ���λ  */
#define PCSSP0_FUNC           PCSSP0_Enable

#define PCTIM2_Enable         1ul                                         /* ʹ�ܶ�ʱ��2����/ʱ�ӿ���λ   */
#define PCTIM2_Disable        0ul                                         /* ��ֹ��ʱ��2����/ʱ�ӿ���λ	*/
#define PCTIM2_FUNC           PCTIM2_Enable

#define PCTIM3_Enable         1ul                                         /* ʹ�ܶ�ʱ��3����/ʱ�ӿ���λ   */
#define PCTIM3_Disable        0ul                                         /* ��ֹ��ʱ��3����/ʱ�ӿ���λ   */
#define PCTIM3_FUNC           PCTIM3_Enable

#define PCUART2_Enable        1ul                                         /* ʹ��UART2����/ʱ�ӿ���λ	    */
#define PCUART2_Disable       0ul                                         /* ��ֹUART2����/ʱ�ӿ���λ	    */
#define PCUART2_FUNC          PCUART2_Disable

#define PCUART3_Enable        1ul                                         /* ʹ��UART3����/ʱ�ӿ���λ	    */
#define PCUART3_Disable       0ul                                         /* ��ֹUART3����/ʱ�ӿ���λ	    */
#define PCUART3_FUNC          PCUART3_Disable

#define PCI2C2_Enable         1ul                                         /* ʹ��I2C�ӿ�2����/ʱ�ӿ���λ  */
#define PCI2C2_Disable        0ul                                         /* ��ֹI2C�ӿ�2����/ʱ�ӿ���λ	*/
#define PCI2C2_FUNC           PCI2C2_Disable

#define PCI2S_Enable          1ul                                         /* ʹ��I2S�ӿڹ���/ʱ�ӿ���λ   */
#define PCI2S_Disable         0ul                                         /* ��ֹI2S�ӿڹ���/ʱ�ӿ���λ   */
#define PCI2S_FUNC            PCI2S_Disable

#define PCGPDMA_Enable        1ul                                         /* ʹ��GP DMA���ܹ���/ʱ�ӿ���λ*/
#define PCGPDMA_Disable       0ul                                         /* ��ֹGP DMA���ܹ���/ʱ�ӿ���λ*/
#define PCGPDMA_FUNC          PCGPDMA_Disable

#define PCENET_Enable         1ul                                         /* ʹ����̫��ģ�鹦��/ʱ�ӿ���λ*/
#define PCENET_Disable        0ul                                         /* ��ֹ��̫��ģ�鹦��/ʱ�ӿ���λ*/
#define PCENET_FUNC           PCENET_Disable

#define PCUSB_Enable          1ul                                         /* ʹ��USB�ӿڹ���/ʱ�ӿ���λ   */
#define PCUSB_Disable         0ul                                         /* ��ֹUSB�ӿڹ���/ʱ�ӿ���λ	*/
#define PCUSB_FUNC            PCUSB_Disable

#endif     

