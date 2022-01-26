
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

#define	NICE5000_RCMD1_RUN					(0x01 << 0)		// 1运行; 0无效
#define	NICE5000_RCMD1_DOWN					(0x01 << 1)		// 1下行; 0无效
#define	NICE5000_RCMD1_UP					(0x01 << 2)		// 1上行; 0无效
#define	NICE5000_RCMD1_ZEROSPD				(0x01 << 3)		// 1零速（调谐结束等表示当前给的指令已经运行完成）;0无效
#define	NICE5000_RCMD1_UNDERVOL				(0x01 << 4)		// 欠压标记
#define	NICE5000_RCMD1_BRAKE				(0x01 << 5)		// 1打开抱闸标记; 0关闭抱闸标记

#define	NICE5000_RCMD2_RUNNING_STEP			0x1F
	#define RCMD2_STEP_IDLE 				0				// 待机状态
	#define RCMD2_STEP_0SPD_RUNNING			1				// 0速开始运行段,斜坡电流增加
	#define RCMD2_STEP_0SPD_KEEPING			2				// 0速保持阶段;（也是无称重调整阶段）
	#define RCMD2_STEP_STARTING				4				// 启动速度阶段
	#define RCMD2_STEP_ACC_STARTING			5				// 加速开始段
	#define RCMD2_STEP_ACC_DOING			6				// 直线加速段
	#define RCMD2_STEP_ACC_ENDING			7				// 加速结束段
	#define RCMD2_STEP_EVEN_SPEED			8				// 稳速度运行
	#define RCMD2_STEP_DEC_STARTING			9				// 减速开始段
	#define RCMD2_STEP_DEC_DOING			10				// 直线减速段
	#define RCMD2_STEP_DEC_ENDING			11				// 减速结束段
	#define RCMD2_STEP_0SPD_STOPPING		12				// 停车0速保持阶段
	#define RCMD2_STEP_CURRENT_WITHDRAW		13	// 电流停止阶段,斜坡电流减小
	#define RCMD2_STEP_STOPPED				15				// 停止数据整理
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

#define	NICE5000_RCMD3_CURRENTFLOOR			0xFF		//当前楼层
#define	NICE5000_RCMD3_PSTOPFLOOR			0xFF00		//下一可停靠楼层


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
#define	NICE5000_BRAKE_CHECK_ID			542		//抱闸力检测结果

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
	WORD F0_00;		// 控制方式    
	WORD F0_01;		// 命令源选择  
	WORD F0_02;		// 面板运行速度
	WORD F0_03;		// 最大速度    
	WORD F0_04;		// 电梯额定速度
	WORD F0_05;		// 方向选择    
	WORD F0_07;		// 载波频率    

	WORD F1_01;		// 额定功率       
	WORD F1_02;		// 额定电压       
	WORD F1_03;		// 额定电流       
	WORD F1_04;		// 额定频率       
	WORD F1_05;		// 额定转速       
	WORD F1_06;		// 定子电阻       
	WORD F1_07;		// 转子电阻       
	WORD F1_08;		// 漏感抗         
	WORD F1_09;		// 互感抗         
	WORD F1_10;		// 空载电流       
	WORD F1_11;		// 调谐选择       
	WORD F1_12;		// 调谐设置       
	WORD F1_13;		// 调谐电流       
	WORD F1_15;		// Q轴电感（转矩）
	WORD F1_16;		// D轴电感（励磁）
	WORD F1_17;		// 反电动势系数   
	WORD F1_25;		// 电动机类型     

	WORD F2_00;		// 速度环KP1    
	WORD F2_01;		// 速度环TI1    
	WORD F2_02;		// 切换频率1    
	WORD F2_03;		// 速度环KP2    
	WORD F2_04;		// 速度环TI2    
	WORD F2_05;		// 切换频率2    
	WORD F2_06;		// 电流比例增益1
	WORD F2_07;		// 电流积分增益1
	WORD F2_08;		// 转矩上限     
	WORD F2_09;		// 电流比例增益2
	WORD F2_10;		// 电流积分增益2
	WORD F2_11;		// 无称重启动   
	WORD F2_12;		// 零伺服速度KP 
	WORD F2_13;		// 零伺服速度KI 
	WORD F2_14;		// 零伺服电流KP1
	WORD F2_15;		// 零伺服电流KI1
	WORD F2_16;		// 零伺服电流KP2
	WORD F2_17;		// 零伺服电流KI2
	WORD F2_18;		// 力矩加速时间 
	WORD F2_19;		// 力矩减速时间 
	WORD F2_20;		// ADC采样延迟  
	WORD F2_22;		// 启动加速时间 
	WORD F2_23;		// 错层设定距离

	WORD F3_00;		// 启动速度    
	WORD F3_01;		// 启动保持时间
	WORD F3_02;		// 加速度      
	WORD F3_03;		// 加速开始段  
	WORD F3_04;		// 加速结束段  
	WORD F3_05;		// 减速度      
	WORD F3_06;		// 减速开始段  
	WORD F3_07;		// 减速结束段  
	WORD F3_08;		// 特殊减速度  
	WORD F3_09;		// 停车距离裕量
	WORD F3_10;		// 再平层速度  
	WORD F3_11;		// 检修运行速度
	WORD F3_12;		// 低速自救速度
	WORD F3_13;		// 端站校验速度
	WORD F3_14;		// 救援运行选择
	WORD F3_15;		// 救援速度    
	WORD F3_16;		// 救援加速度  

	WORD F4_00;		// 平层调整    
	WORD F4_01;		// 当前楼层    
	WORD F4_02;		// 当前位置高位
	WORD F4_03;		// 当前位置低位
	WORD F4_04;		// 插板长度1   
	WORD F4_05;		// 插板长度2   
	WORD F4_06;		// 平层延迟时间
	WORD F4_07;		// 下行平层调整
	WORD F4_08;		// 校验1       
	WORD F4_09;		// 校验2       
	WORD F4_10;		// 层高1高位   
	WORD F4_11;		// 层高1低位   
	WORD F4_12;		// 层高2高位   
	WORD F4_13;		// 层高2低位   
	WORD F4_14;		// 层高3高位   
	WORD F4_15;		// 层高3低位   
	WORD F4_16;		// 层高4高位   
	WORD F4_17;		// 层高4低位   
	WORD F4_18;		// 层高5高位   
	WORD F4_19;		// 层高5低位   
	WORD F4_20;		// 层高6高位   
	WORD F4_21;		// 层高6低位   
	WORD F4_22;		// 层高7高位   
	WORD F4_23;		// 层高7低位   
	WORD F4_24;		// 层高8高位   
	WORD F4_25;		// 层高8低位   
	WORD F4_26;		// 层高9高位   
	WORD F4_27;		// 层高9低位   
	WORD F4_28;		// 层高10高位  
	WORD F4_29;		// 层高10低位  
	WORD F4_30;		// 层高11高位  
	WORD F4_31;		// 层高11低位  
	WORD F4_32;		// 层高12高位  
	WORD F4_33;		// 层高12低位  
	WORD F4_34;		// 层高13高位  
	WORD F4_35;		// 层高13低位  
	WORD F4_36;		// 层高14高位  
	WORD F4_37;		// 层高14低位  
	WORD F4_38;		// 层高15高位  
	WORD F4_39;		// 层高15低位  
	WORD F4_40;		// 层高16高位  
	WORD F4_41;		// 层高16低位  
	WORD F4_42;		// 层高17高位  
	WORD F4_43;		// 层高17低位  
	WORD F4_44;		// 层高18高位  
	WORD F4_45;		// 层高18低位  
	WORD F4_46;		// 层高19高位  
	WORD F4_47;		// 层高19低位  
	WORD F4_48;		// 层高20高位  
	WORD F4_49;		// 层高20低位  
	WORD F4_50;		// 层高21高位  
	WORD F4_51;		// 层高21低位  
	WORD F4_52;		// 层高22高位  
	WORD F4_53;		// 层高22低位  
	WORD F4_54;		// 层高23高位  
	WORD F4_55;		// 层高23低位  
	WORD F4_56;		// 层高24高位  
	WORD F4_57;		// 层高24低位  
	WORD F4_58;		// 层高25高位  
	WORD F4_59;		// 层高25低位  
	WORD F4_60;		// 层高26高位  
	WORD F4_61;		// 层高26低位  
	WORD F4_62;		// 层高27高位  
	WORD F4_63;		// 层高27低位  
	WORD F4_64;		// 层高28高位  
	WORD F4_65;		// 层高28低位  
	WORD F4_66;		// 层高29高位  
	WORD F4_67;		// 层高29低位  
	WORD F4_68;		// 层高30高位  
	WORD F4_69;		// 层高30低位  
	WORD F4_70;		// 层高31高位  
	WORD F4_71;		// 层高31低位  
	WORD F4_72;		// 层高32高位  
	WORD F4_73;		// 层高32低位  
	WORD F4_74;		// 层高33高位  
	WORD F4_75;		// 层高33低位  
	WORD F4_76;		// 层高34高位  
	WORD F4_77;		// 层高34低位  
	WORD F4_78;		// 层高35高位  
	WORD F4_79;		// 层高35低位  
	WORD F4_80;		// 层高36高位  
	WORD F4_81;		// 层高36低位  
	WORD F4_82;		// 层高37高位  
	WORD F4_83;		// 层高37低位  
	WORD F4_84;		// 层高38高位  
	WORD F4_85;		// 层高38低位  
	WORD F4_86;		// 层高39高位  
	WORD F4_87;		// 层高39低位  
	WORD F4_88;		// 层高40高位  
	WORD F4_89;		// 层高40低位  
	WORD F4_90;		// 层高41高位  
	WORD F4_91;		// 层高41低位  
	WORD F4_92;		// 层高42高位  
	WORD F4_93;		// 层高42低位  
	WORD F4_94;		// 层高43高位  
	WORD F4_95;		// 层高43低位  
	WORD F4_96;		// 层高44高位  
	WORD F4_97;		// 层高44低位  
	WORD F4_98;		// 层高45高位  
	WORD F4_99;		// 层高45低位  
	WORD F4_100;	// 层高46高位 
	WORD F4_101;	// 层高46低位 
	WORD F4_102;	// 层高47高位 
	WORD F4_103;	// 层高47低位 
	WORD F4_104;	// 层高48高位 
	WORD F4_105;	// 层高48低位 
	WORD F4_106;	// 层高49高位 
	WORD F4_107;	// 层高49低位 
	WORD F4_108;	// 层高50高位 
	WORD F4_109;	// 层高50低位 
	WORD F4_110;	// 层高51高位 
	WORD F4_111;	// 层高51低位 
	WORD F4_112;	// 层高52高位 
	WORD F4_113;	// 层高52低位 
	WORD F4_114;	// 层高53高位 
	WORD F4_115;	// 层高53低位 
	WORD F4_116;	// 层高54高位 
	WORD F4_117;	// 层高54低位 
	WORD F4_118;	// 层高55高位 
	WORD F4_119;	// 层高55低位 
	
	WORD F5_X1;		// X1输入点设置
	WORD F5_X2;		// X2输入点设置
	WORD F5_X3;		// X3输入点设置
	WORD F5_X4;		// X4输入点设置
	WORD F5_X5;		// X5输入点设置
	WORD F5_X6;		// X6输入点设置
	WORD F5_X7;		// X7输入点设置
	WORD F5_X8;		// X8输入点设置
	
	WORD F6_top_floor;		// 电梯最高层
	WORD F6_bot_floor;		// 电梯最低层

	WORD FA_06;		// "软件版本"  
	WORD FA_07;		// 散热器温度   
	WORD FA_08;		// 特征因子     
	WORD FA_09;		// 保护功能选择 
	WORD FA_10;		// 过载保护系数 
	WORD FA_11;		// 过载预警系数 
	WORD FA_12;		// 驱动程序选择1
	WORD FA_13;		// 驱动程序选择2
	WORD FA_14;		// 保留         
	WORD FA_15;		// 程序控制选择3
	WORD FA_16;		// 保留FA29     
	WORD FA_17;		// 保留FA30     
	WORD FA_18;		// 保留FA31     
	WORD FA_19;		// 保留FA32     
	WORD FA_20;		// 保留FA33     
	WORD FA_21;		// 保留FA34     
	WORD FA_22;		// 保留FA35     
	WORD FA_23;		// 保留FA36     
	WORD FA_24;		// 保留FA37     
	WORD FA_25;		// 保留FA38     
	WORD FA_26;		// 保留FA39     
	WORD FA_27;		// 保留FA40     
	WORD FA_28;		// 保留FA41     
	WORD FA_29;		// 保留FA42     
	WORD FA_30;		// 保留FA43   
	WORD FA_31;		// 保留FA40     
	WORD FA_32;		// 保留FA41     
	WORD FA_33;		// 保留FA42     
	WORD FA_34;		// 保留FA43     
	WORD FA_35;		// 保留FA40     
	WORD FA_36;		// 保留FA41     
	WORD FA_37;		// 保留FA42     
	WORD FA_38;		// 保留FA43     
	WORD FA_39;		// 保留FA43     
	WORD FA_40;		// 保留FA43     

	WORD FH_01;		// 编码器选择    
	WORD FH_02;		// 编码器脉冲数  
	WORD FH_03;		// 断线检测时间  
	WORD FH_04;		// PG原点角度    
	WORD FH_05;		// PG当前角度    
	WORD FH_06;		// 接线方式      
	WORD FH_07;		// 编码器功能选择
	WORD FH_08;		// 信号零漂1     
	WORD FH_09;		// 信号零漂2     
	WORD FH_10;		// 信号零漂3     
	WORD FH_11;		// 信号零漂4     
	WORD FH_12;		// 旋变极对数    
	WORD FH_17;		// 上1级开关位置 
	WORD FH_18;		// 下1级开关位置 
	WORD FH_19;		// 上2级开关位置 
	WORD FH_20;		// 下2级开关位置 
	WORD FH_21;		// 上3级开关位置 
	WORD FH_22;		// 下3级开关位置 
	WORD FH_23;		// 上4级开关位置 
	WORD FH_24;		// 下4级开关位置 
	WORD FH_25;		// 上5级开关位置 
	WORD FH_26;		// 下5级开关位置 
	WORD FH_27;		// 上6级开关位置 
	WORD FH_28;		// 下6级开关位置 
	WORD FH_29;		// 位置开关延迟  

	WORD FF_00;		// 厂家密码    
	WORD FF_01;		// 变频器型号  
	WORD FF_02;		// 变频器功率  
	WORD FF_03;		// 温度曲线    
	WORD FF_05;		// 保留        
	WORD FF_06;		// 软件欠压点  
	WORD FF_07;		// 软件过流点  
	WORD FF_08;		// 电压校正系数
	WORD FF_09;		// 电流校正系数
	WORD FF_10;		// 保留        
	WORD FF_11;		// 保留        

	WORD FR_00;		//平层调整使能
	WORD FR_01;		//调整记录
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

	WORD reserve[69];	// 保留
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

