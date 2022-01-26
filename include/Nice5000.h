
#ifndef _NICE5000_H_
#define	_NICE5000_H_


#define	NICE5000_SPI_CLK					86		//279.07K
#define	SSP_SR_TFE							0x01ul
#define SSP_SR_TNF							0x02ul
#define	SSP_SR_RNE							0x04ul
#define SSP_SR_BSY							0x10ul


#define NICE5000_SPI_FRMSIZE				6
#define	FRM_HEAD_CMD1						0x3100
#define	FRM_HEAD_CMD2						0x3200
#define	FRM_HEAD_FCT1						0x5100
#define	FRM_HEAD_FCT2						0x5200
#define	FRM_HEAD_FAULT						0x5300
#define	FRM_FCT_SENDTIMES					4

#define	NICE5000_TCMD1_RUN					(0x01 << 0)
#define	NICE5000_TCMD1_DOWN					(0x01 << 1)
#define	NICE5000_TCMD1_UP					(0x01 << 2)
#define	NICE5000_TCMD1_BRAKE				(0x01 << 3)
#define	NICE5000_TCMD1_KEYDEC				(0x01 << 4)
#define	NICE5000_TCMD1_SPECIALDEC				(0x01 << 8)

#define	NICE5000_TCMD2_TARGET				0xff
	#define MOTOR_TURN_00					0
	#define MOTOR_TURN_01					1
	#define MOTOR_TURN_02					2
	#define MOTOR_TURN_03					3
#define	NICE5000_TCMD2_STATE				0x1f00
	#define TCMD2_STATE_INSPECTION			(0<<8)
	#define TCMD2_STATE_SHAFT_TEACHIN		(1<<8)
	#define TCMD2_STATE_RELEVEL				(2<<8)
	#define TCMD2_STATE_OTHER				(7<<8)
	#define TCMD2_STATE_DIVING				(10<<8)
	#define TCMD2_STATE_EVACUATION			(11<<8)
	#define TCMD2_STATE_MOTOR_TURN			(12<<8)
	#define TCMD2_STATE_KEY 				(13<<8)
	#define TCMD2_STATE_RESET_TRIP			(14<<8)
#define	NICE5000_TCMD2_DZONE				0x8000

#define	NICE5000_TCMD4_TOGGLE				0x07
#define	NICE5000_TCMD4_R_READY				0x08
#define	NICE5000_TCMD4_T_READY				0x10
#define	NICE5000_TCMD4_L_READY				0x20
#define	NICE5000_TCMD4_PSL					0x40
#define	NICE5000_TCMD4_SDSL					0x80

#define	NICE_TCMD5_ERRCODE					0xFF

#define	NICE5000_RCMD1_RUN					(0x01 << 0)		// 1����; 0��Ч
#define	NICE5000_RCMD1_DOWN					(0x01 << 1)		// 1����; 0��Ч
#define	NICE5000_RCMD1_UP					(0x01 << 2)		// 1����; 0��Ч
#define	NICE5000_RCMD1_ZEROSPD				(0x01 << 3)		// 1���٣���г�����ȱ�ʾ��ǰ����ָ���Ѿ�������ɣ�;0��Ч
#define	NICE5000_RCMD1_UNDERVOL				(0x01 << 4)		// Ƿѹ���
#define	NICE5000_RCMD1_BRAKE				(0x01 << 5)		// 1�򿪱�բ���; 0�رձ�բ���

#define	NICE5000_RCMD2_RUNNING_STEP			0x1F
	#define RCMD2_STEP_IDLE 				0				// ����״̬
	#define RCMD2_STEP_0SPD_RUNNING			1				// 0�ٿ�ʼ���ж�,б�µ�������
	#define RCMD2_STEP_0SPD_KEEPING			2				// 0�ٱ��ֽ׶�;��Ҳ���޳��ص����׶Σ�
	#define RCMD2_STEP_STARTING				4				// �����ٶȽ׶�
	#define RCMD2_STEP_ACC_STARTING			5				// ���ٿ�ʼ��
	#define RCMD2_STEP_ACC_DOING			6				// ֱ�߼��ٶ�
	#define RCMD2_STEP_ACC_ENDING			7				// ���ٽ�����
	#define RCMD2_STEP_EVEN_SPEED			8				// ���ٶ�����
	#define RCMD2_STEP_DEC_STARTING			9				// ���ٿ�ʼ��
	#define RCMD2_STEP_DEC_DOING			10				// ֱ�߼��ٶ�
	#define RCMD2_STEP_DEC_ENDING			11				// ���ٽ�����
	#define RCMD2_STEP_0SPD_STOPPING		12				// ͣ��0�ٱ��ֽ׶�
	#define RCMD2_STEP_CURRENT_WITHDRAW		13	// ����ֹͣ�׶�,б�µ�����С
	#define RCMD2_STEP_STOPPED				15				// ֹͣ��������
#define	NICE5000_RCMD2_STATE				0x1F00
	#define RCMD2_STATE_INSPECTION			(0<<8)
	#define RCMD2_STATE_SHAFT_TEACHIN		(1<<8)
	#define RCMD2_STATE_RELEVEL				(2<<8)
	#define RCMD2_STATE_OTHER				(7<<8)
	#define RCMD2_STATE_DIVING				(10<<8)
	#define RCMD2_STATE_EVACUATION			(11<<8)
	#define RCMD2_STATE_MOTOR_TURN			(12<<8)
	#define RCMD2_STATE_KEY 				(13<<8)
	#define RCMD2_STATE_RESET_TRIP			(14<<8)
#define	NICE5000_RCMD2_SETUP_READY			0x4000
#define	NICE5000_RCMD2_TARGET_LOCKED		0x8000

#define	NICE5000_RCMD3_CURRENTFLOOR			0xFF		//��ǰ¥��
#define	NICE5000_RCMD3_PSTOPFLOOR			0xFF00		//��һ��ͣ��¥��


#define	NICE5000_RCMD4_POSERROR				0x0080

#define	NICE5000_RSTORE_SETSPEED			510
#define	NICE5000_RSTORE_FEEDSPEED			511
#define	NICE5000_RSTORE_DCVOLTAGE			512
#define	NICE5000_RSTORE_OUTVOLTAGE			513
#define	NICE5000_RSTORE_OUTCURRENT			514
#define	NICE5000_RSTORE_OUTFREQ				515
#define	NICE5000_RSTORE_OUTTORQUE			516
#define	NICE5000_RSTORE_TKCURRENT			517
#define	NICE5000_RSTORE_OUTPWR				518
#define	NICE5000_RSTORE_PULSEH				67
#define	NICE5000_RSTORE_PULSEL				68
#define	NICE5000_RSTORE_POSITION			519
#define	NICE5000_BRAKE_CHECK_ID			542		//��բ�������

#define	NICE5000_RFAULT_SETSPEED			1212
#define	NICE5000_RFAULT_FEEDSPEED			1213
#define	NICE5000_RFAULT_DCVOLTAGE			1214
#define	NICE5000_RFAULT_OUTVOLTAGE			1215
#define	NICE5000_RFAULT_OUTCURRENT			1216
#define	NICE5000_RFAULT_OUTFREQ				1217
#define	NICE5000_RFAULT_OUTTORQUE			1218
#define	NICE5000_RFAULT_TKCURRENT			1219
#define	NICE5000_RFAULT_OUTPWR				1220
#define	NICE5000_RFAULT_POSITION			1221
#define	NICE5000_RFAULT_ERRCODE				1206
#define	NICE5000_RFAULT_ERRSUB				1207		

#define	NICE5000_RSTATE_BOTFLOOR			224
#define	NICE5000_RSTATE_TOPFLOOR			225
#define	NICE5000_RSTATE_CURRFLOOR			66

#define	NICE5000_INPUT_MSI					3
#define	NICE5000_INPUT_SDU1					16
#define	NICE5000_INPUT_SDD1				17
#define	NICE5000_INPUT_SDU2					18
#define	NICE5000_INPUT_SDD2				19
#define	NICE5000_INPUT_SDU3					20
#define	NICE5000_INPUT_SDD3				21
#define	NICE5000_INPUT_LOGIC				100

#define	NICE5000_P_F1_25_IM					0x00
#define	NICE5000_P_F1_25_PMSM				0x01

#define	NICE5000_BRAKE_CHECK_COMMAND	0x1000
#define	NICE5000_BRAKE_CHECK_FINISH		0x07C0

struct _nice5000_readvar
{
	WORD * ptr;
	WORD id;
};
struct _nice5000_loadvar
{
	WORD * ptr;
	WORD id;
	WORD defaultvalue;
};
struct _nice5000_writevar
{
	void * ptr;
	BYTE size;
	WORD id;
};
struct _nice5000_initvar
{
	void * ptr;
	BYTE size;
	WORD id;
	BYTE mode;
	BYTE bit;
	DWORD value;
};

#define	NICE5000_INITPARA_READSIZE			300
#define	NICE5000_MAX_LEVEL_ADJ				30

#define	NICE5000_INITMODE_VALUEGIVE			0
#define	NICE5000_INITMODE_BITSET			1
#define	NICE5000_INITMODE_BITCLR			2


struct Nice5000Parameter
{
	WORD F0_00;		// ���Ʒ�ʽ    
	WORD F0_01;		// ����Դѡ��  
	WORD F0_02;		// ��������ٶ�
	WORD F0_03;		// ����ٶ�    
	WORD F0_04;		// ���ݶ�ٶ�
	WORD F0_05;		// ����ѡ��    
	WORD F0_07;		// �ز�Ƶ��    

	WORD F1_01;		// �����       
	WORD F1_02;		// ���ѹ       
	WORD F1_03;		// �����       
	WORD F1_04;		// �Ƶ��       
	WORD F1_05;		// �ת��       
	WORD F1_06;		// ���ӵ���       
	WORD F1_07;		// ת�ӵ���       
	WORD F1_08;		// ©�п�         
	WORD F1_09;		// ���п�         
	WORD F1_10;		// ���ص���       
	WORD F1_11;		// ��гѡ��       
	WORD F1_12;		// ��г����       
	WORD F1_13;		// ��г����       
	WORD F1_15;		// Q���У�ת�أ�
	WORD F1_16;		// D���У����ţ�
	WORD F1_17;		// ���綯��ϵ��   
	WORD F1_25;		// �綯������     

	WORD F2_00;		// �ٶȻ�KP1    
	WORD F2_01;		// �ٶȻ�TI1    
	WORD F2_02;		// �л�Ƶ��1    
	WORD F2_03;		// �ٶȻ�KP2    
	WORD F2_04;		// �ٶȻ�TI2    
	WORD F2_05;		// �л�Ƶ��2    
	WORD F2_06;		// ������������1
	WORD F2_07;		// ������������1
	WORD F2_08;		// ת������     
	WORD F2_09;		// ������������2
	WORD F2_10;		// ������������2
	WORD F2_11;		// �޳�������   
	WORD F2_12;		// ���ŷ��ٶ�KP 
	WORD F2_13;		// ���ŷ��ٶ�KI 
	WORD F2_14;		// ���ŷ�����KP1
	WORD F2_15;		// ���ŷ�����KI1
	WORD F2_16;		// ���ŷ�����KP2
	WORD F2_17;		// ���ŷ�����KI2
	WORD F2_18;		// ���ؼ���ʱ�� 
	WORD F2_19;		// ���ؼ���ʱ�� 
	WORD F2_20;		// ADC�����ӳ�  
	WORD F2_22;		// ��������ʱ�� 
	WORD F2_23;		// ����趨����

	WORD F3_00;		// �����ٶ�    
	WORD F3_01;		// ��������ʱ��
	WORD F3_02;		// ���ٶ�      
	WORD F3_03;		// ���ٿ�ʼ��  
	WORD F3_04;		// ���ٽ�����  
	WORD F3_05;		// ���ٶ�      
	WORD F3_06;		// ���ٿ�ʼ��  
	WORD F3_07;		// ���ٽ�����  
	WORD F3_08;		// ������ٶ�  
	WORD F3_09;		// ͣ������ԣ��
	WORD F3_10;		// ��ƽ���ٶ�  
	WORD F3_11;		// ���������ٶ�
	WORD F3_12;		// �����Ծ��ٶ�
	WORD F3_13;		// ��վУ���ٶ�
	WORD F3_14;		// ��Ԯ����ѡ��
	WORD F3_15;		// ��Ԯ�ٶ�    
	WORD F3_16;		// ��Ԯ���ٶ�  

	WORD F4_00;		// ƽ�����    
	WORD F4_01;		// ��ǰ¥��    
	WORD F4_02;		// ��ǰλ�ø�λ
	WORD F4_03;		// ��ǰλ�õ�λ
	WORD F4_04;		// ��峤��1   
	WORD F4_05;		// ��峤��2   
	WORD F4_06;		// ƽ���ӳ�ʱ��
	WORD F4_07;		// ����ƽ�����
	WORD F4_08;		// У��1       
	WORD F4_09;		// У��2       
	WORD F4_10;		// ���1��λ   
	WORD F4_11;		// ���1��λ   
	WORD F4_12;		// ���2��λ   
	WORD F4_13;		// ���2��λ   
	WORD F4_14;		// ���3��λ   
	WORD F4_15;		// ���3��λ   
	WORD F4_16;		// ���4��λ   
	WORD F4_17;		// ���4��λ   
	WORD F4_18;		// ���5��λ   
	WORD F4_19;		// ���5��λ   
	WORD F4_20;		// ���6��λ   
	WORD F4_21;		// ���6��λ   
	WORD F4_22;		// ���7��λ   
	WORD F4_23;		// ���7��λ   
	WORD F4_24;		// ���8��λ   
	WORD F4_25;		// ���8��λ   
	WORD F4_26;		// ���9��λ   
	WORD F4_27;		// ���9��λ   
	WORD F4_28;		// ���10��λ  
	WORD F4_29;		// ���10��λ  
	WORD F4_30;		// ���11��λ  
	WORD F4_31;		// ���11��λ  
	WORD F4_32;		// ���12��λ  
	WORD F4_33;		// ���12��λ  
	WORD F4_34;		// ���13��λ  
	WORD F4_35;		// ���13��λ  
	WORD F4_36;		// ���14��λ  
	WORD F4_37;		// ���14��λ  
	WORD F4_38;		// ���15��λ  
	WORD F4_39;		// ���15��λ  
	WORD F4_40;		// ���16��λ  
	WORD F4_41;		// ���16��λ  
	WORD F4_42;		// ���17��λ  
	WORD F4_43;		// ���17��λ  
	WORD F4_44;		// ���18��λ  
	WORD F4_45;		// ���18��λ  
	WORD F4_46;		// ���19��λ  
	WORD F4_47;		// ���19��λ  
	WORD F4_48;		// ���20��λ  
	WORD F4_49;		// ���20��λ  
	WORD F4_50;		// ���21��λ  
	WORD F4_51;		// ���21��λ  
	WORD F4_52;		// ���22��λ  
	WORD F4_53;		// ���22��λ  
	WORD F4_54;		// ���23��λ  
	WORD F4_55;		// ���23��λ  
	WORD F4_56;		// ���24��λ  
	WORD F4_57;		// ���24��λ  
	WORD F4_58;		// ���25��λ  
	WORD F4_59;		// ���25��λ  
	WORD F4_60;		// ���26��λ  
	WORD F4_61;		// ���26��λ  
	WORD F4_62;		// ���27��λ  
	WORD F4_63;		// ���27��λ  
	WORD F4_64;		// ���28��λ  
	WORD F4_65;		// ���28��λ  
	WORD F4_66;		// ���29��λ  
	WORD F4_67;		// ���29��λ  
	WORD F4_68;		// ���30��λ  
	WORD F4_69;		// ���30��λ  
	WORD F4_70;		// ���31��λ  
	WORD F4_71;		// ���31��λ  
	WORD F4_72;		// ���32��λ  
	WORD F4_73;		// ���32��λ  
	WORD F4_74;		// ���33��λ  
	WORD F4_75;		// ���33��λ  
	WORD F4_76;		// ���34��λ  
	WORD F4_77;		// ���34��λ  
	WORD F4_78;		// ���35��λ  
	WORD F4_79;		// ���35��λ  
	WORD F4_80;		// ���36��λ  
	WORD F4_81;		// ���36��λ  
	WORD F4_82;		// ���37��λ  
	WORD F4_83;		// ���37��λ  
	WORD F4_84;		// ���38��λ  
	WORD F4_85;		// ���38��λ  
	WORD F4_86;		// ���39��λ  
	WORD F4_87;		// ���39��λ  
	WORD F4_88;		// ���40��λ  
	WORD F4_89;		// ���40��λ  
	WORD F4_90;		// ���41��λ  
	WORD F4_91;		// ���41��λ  
	WORD F4_92;		// ���42��λ  
	WORD F4_93;		// ���42��λ  
	WORD F4_94;		// ���43��λ  
	WORD F4_95;		// ���43��λ  
	WORD F4_96;		// ���44��λ  
	WORD F4_97;		// ���44��λ  
	WORD F4_98;		// ���45��λ  
	WORD F4_99;		// ���45��λ  
	WORD F4_100;	// ���46��λ 
	WORD F4_101;	// ���46��λ 
	WORD F4_102;	// ���47��λ 
	WORD F4_103;	// ���47��λ 
	WORD F4_104;	// ���48��λ 
	WORD F4_105;	// ���48��λ 
	WORD F4_106;	// ���49��λ 
	WORD F4_107;	// ���49��λ 
	WORD F4_108;	// ���50��λ 
	WORD F4_109;	// ���50��λ 
	WORD F4_110;	// ���51��λ 
	WORD F4_111;	// ���51��λ 
	WORD F4_112;	// ���52��λ 
	WORD F4_113;	// ���52��λ 
	WORD F4_114;	// ���53��λ 
	WORD F4_115;	// ���53��λ 
	WORD F4_116;	// ���54��λ 
	WORD F4_117;	// ���54��λ 
	WORD F4_118;	// ���55��λ 
	WORD F4_119;	// ���55��λ 
	
	WORD F5_X1;		// X1���������
	WORD F5_X2;		// X2���������
	WORD F5_X3;		// X3���������
	WORD F5_X4;		// X4���������
	WORD F5_X5;		// X5���������
	WORD F5_X6;		// X6���������
	WORD F5_X7;		// X7���������
	WORD F5_X8;		// X8���������
	
	WORD F6_top_floor;		// ������߲�
	WORD F6_bot_floor;		// ������Ͳ�

	WORD FA_06;		// "����汾"  
	WORD FA_07;		// ɢ�����¶�   
	WORD FA_08;		// ��������     
	WORD FA_09;		// ��������ѡ�� 
	WORD FA_10;		// ���ر���ϵ�� 
	WORD FA_11;		// ����Ԥ��ϵ�� 
	WORD FA_12;		// ��������ѡ��1
	WORD FA_13;		// ��������ѡ��2
	WORD FA_14;		// ����         
	WORD FA_15;		// �������ѡ��3
	WORD FA_16;		// ����FA29     
	WORD FA_17;		// ����FA30     
	WORD FA_18;		// ����FA31     
	WORD FA_19;		// ����FA32     
	WORD FA_20;		// ����FA33     
	WORD FA_21;		// ����FA34     
	WORD FA_22;		// ����FA35     
	WORD FA_23;		// ����FA36     
	WORD FA_24;		// ����FA37     
	WORD FA_25;		// ����FA38     
	WORD FA_26;		// ����FA39     
	WORD FA_27;		// ����FA40     
	WORD FA_28;		// ����FA41     
	WORD FA_29;		// ����FA42     
	WORD FA_30;		// ����FA43   
	WORD FA_31;		// ����FA40     
	WORD FA_32;		// ����FA41     
	WORD FA_33;		// ����FA42     
	WORD FA_34;		// ����FA43     
	WORD FA_35;		// ����FA40     
	WORD FA_36;		// ����FA41     
	WORD FA_37;		// ����FA42     
	WORD FA_38;		// ����FA43     
	WORD FA_39;		// ����FA43     
	WORD FA_40;		// ����FA43     

	WORD FH_01;		// ������ѡ��    
	WORD FH_02;		// ������������  
	WORD FH_03;		// ���߼��ʱ��  
	WORD FH_04;		// PGԭ��Ƕ�    
	WORD FH_05;		// PG��ǰ�Ƕ�    
	WORD FH_06;		// ���߷�ʽ      
	WORD FH_07;		// ����������ѡ��
	WORD FH_08;		// �ź���Ư1     
	WORD FH_09;		// �ź���Ư2     
	WORD FH_10;		// �ź���Ư3     
	WORD FH_11;		// �ź���Ư4     
	WORD FH_12;		// ���伫����    
	WORD FH_17;		// ��1������λ�� 
	WORD FH_18;		// ��1������λ�� 
	WORD FH_19;		// ��2������λ�� 
	WORD FH_20;		// ��2������λ�� 
	WORD FH_21;		// ��3������λ�� 
	WORD FH_22;		// ��3������λ�� 
	WORD FH_23;		// ��4������λ�� 
	WORD FH_24;		// ��4������λ�� 
	WORD FH_25;		// ��5������λ�� 
	WORD FH_26;		// ��5������λ�� 
	WORD FH_27;		// ��6������λ�� 
	WORD FH_28;		// ��6������λ�� 
	WORD FH_29;		// λ�ÿ����ӳ�  

	WORD FF_00;		// ��������    
	WORD FF_01;		// ��Ƶ���ͺ�  
	WORD FF_02;		// ��Ƶ������  
	WORD FF_03;		// �¶�����    
	WORD FF_05;		// ����        
	WORD FF_06;		// ���Ƿѹ��  
	WORD FF_07;		// ���������  
	WORD FF_08;		// ��ѹУ��ϵ��
	WORD FF_09;		// ����У��ϵ��
	WORD FF_10;		// ����        
	WORD FF_11;		// ����        

	WORD FR_00;		//ƽ�����ʹ��
	WORD FR_01;		//������¼
	WORD FR_02;
	WORD FR_03;
	WORD FR_04;
	WORD FR_05;
	WORD FR_06;
	WORD FR_07;
	WORD FR_08;
	WORD FR_09;
	WORD FR_10;
	WORD FR_11;
	WORD FR_12;
	WORD FR_13;
	WORD FR_14;
	WORD FR_15;
	WORD FR_16;
	WORD FR_17;
	WORD FR_18;
	WORD FR_19;
	WORD FR_20;
	WORD FR_21;
	WORD FR_22;
	WORD FR_23;
	WORD FR_24;
	WORD FR_25;
	WORD FR_26;
	WORD FR_27;
	WORD FR_28;

	WORD reserve[69];	// ����
};

#define	NICE5000_CRC_ERRCNT	500
#define	NICE5000_HB_RSTCNT		2000
#define	NICE_INDEX			0x8000

#define	NICE_ERR_45			45


extern WORD spi_rx_fifo[];
extern BYTE spi_rx_offset;
extern BYTE nice5000_crc_error;
extern WORD nice5000_crc_errcnt;
extern BYTE nice5000_errrsttime;
extern BYTE nice5000_poserr_flag;
extern DWORD nice5000_startpos;
extern WORD nice5000_running_step;


extern WORD nice5000_Tcmd_data1;
extern WORD nice5000_Tcmd_data2;
extern WORD nice5000_Tcmd_data3;
extern WORD nice5000_Tcmd_data4;
extern WORD nice5000_Tcmd_data5;
extern WORD nice5000_Tcmd_data6;
extern WORD nice5000_Tcmd_data7;

extern WORD nice5000_Rcmd_data1;
extern WORD nice5000_Rcmd_data2;
extern WORD nice5000_Rcmd_data3;
extern WORD nice5000_Rcmd_data4;
extern WORD nice5000_Rcmd_data5;
extern WORD nice5000_Rcmd_data6;
extern WORD nice5000_Rcmd_data7;

extern WORD nice5000_Tstate_id1;
extern WORD nice5000_Tstate_data1;
extern WORD nice5000_Tstate_id2;
extern WORD nice5000_Tstate_data2;
extern BYTE nice5000_Tstate_send;

extern WORD nice5000_Tstore_id1;
extern WORD nice5000_Tstore_data1;
extern WORD nice5000_Tstore_id2;
extern WORD nice5000_Tstore_data2;
extern BYTE nice5000_Tstore_send;

extern WORD nice5000_Rstate_id1;
extern WORD nice5000_Rstate_data1;
extern WORD nice5000_Rstate_id2;
extern WORD nice5000_Rstate_data2;
extern BYTE nice5000_Rstate_new;

extern WORD nice5000_Rstore_id1;
extern WORD nice5000_Rstore_data1;
extern WORD nice5000_Rstore_id2;
extern WORD nice5000_Rstore_data2;
extern BYTE nice5000_Rstore_new;

extern WORD nice5000_Rstore_setspd;
extern WORD nice5000_Rstore_feedspd;
extern WORD nice5000_Rstore_dcvoltage;
extern WORD nice5000_Rstore_outvoltage;
extern WORD nice5000_Rstore_outcurrent;
extern WORD nice5000_Rstore_outfreq;
extern WORD nice5000_Rstore_outtorque;
extern WORD nice5000_Rstore_tkcurrent;
extern WORD nice5000_Rstore_outpwr;
extern WORD nice5000_Rstore_pulseH;
extern WORD nice5000_Rstore_pulseL;
extern WORD nice5000_Rstore_position;

extern WORD nice5000_Rfault_id1;
extern WORD nice5000_Rfault_data1;
extern WORD nice5000_Rfault_id2;
extern WORD nice5000_Rfault_data2;
extern BYTE nice5000_Rfault_new;

extern WORD nice5000_Rfault_setspd;
extern WORD nice5000_Rfault_feedspd;
extern WORD nice5000_Rfault_dcvoltage;
extern WORD nice5000_Rfault_outvoltage;
extern WORD nice5000_Rfault_outcurrent;
extern WORD nice5000_Rfault_outfreq;
extern WORD nice5000_Rfault_outtorque;
extern WORD nice5000_Rfault_tkcurrent;
extern WORD nice5000_Rfault_outpwr;
extern WORD nice5000_Rfault_position;
extern WORD nice5000_Rfault_errcode;
extern WORD nice5000_Rfault_errsub;

extern BYTE nice5000_basicsetting_init;
extern BYTE version_drv[];
extern struct Nice5000Parameter niv;
extern struct Nice5000Parameter biv;
extern struct Nice5000Parameter tiv;


WORD CrcChkValue(WORD * dataptr, WORD len);
void GetNiceStatus(void);
void GetNiceFault(void);
void ResetNiceFaultChk(void);
BYTE IsNiceFaultChked(void);
void ConductNiceSpiData(void);
void GetNiceTivPara(WORD * ptr);
void init_nice5000(void);
void drive_nice5000(void);
void send_nice5000_parameter(WORD address, WORD value);
WORD GetAddrByReadPara(WORD * ptr, WORD * addr);
WORD * GetTivPtrByAddr(WORD addr);
BYTE niv_addr_match(WORD addr);
void Load_pos(WORD lastl, WORD lasth);


#endif

