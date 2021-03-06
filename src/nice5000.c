
#include "typedef.h"
#include "stdio.h"
#include "string.h"
#include "canopen.h"
#include "parameter.h"
#include "nice5000.h"
#include "state.h"
#include "lcd.h"
#include "can.h"
#include "io.h"
#include "drive.h"
#include "call.h"
#include "eeprom.h"
#include "history.h"
#include "pos.h"
#include "bootloader.h"



struct Nice5000Parameter niv;
struct Nice5000Parameter biv;
struct Nice5000Parameter tiv;


WORD spi_rx_fifo[NICE5000_SPI_FRMSIZE];
WORD nice5000_hbrst_count = 0;			//心跳故障时，等待心跳恢复的时间
BYTE spi_rx_offset = 0;
BYTE nice5000_crc_error = 0;
WORD nice5000_crc_errcnt = 0;
BYTE nice5000_errrsttime = 0;
BYTE nice5000_poserr_flag = 0;
DWORD nice5000_startpos = 0;

WORD nice5000_Tcmd_data1 = 0;
WORD nice5000_Tcmd_data2 = 0;
WORD nice5000_Tcmd_data3 = 0;
WORD nice5000_Tcmd_data4 = 0;
WORD nice5000_Tcmd_data5 = 0;
WORD nice5000_Tcmd_data6 = 0;
WORD nice5000_Tcmd_data7 = 0;

WORD nice5000_Rcmd_data1 = 0;
WORD nice5000_Rcmd_data2 = 0;
WORD nice5000_Rcmd_data3 = 0;
WORD nice5000_Rcmd_data4 = 0;
WORD nice5000_Rcmd_data5 = 0;
WORD nice5000_Rcmd_data6 = 0;
WORD nice5000_Rcmd_data7 = 0;
WORD nice5000_running_step = 0;

WORD nice5000_Tstate_id1 = 0;
WORD nice5000_Tstate_data1 = 0;
WORD nice5000_Tstate_id2 = 0;
WORD nice5000_Tstate_data2 = 0;
BYTE nice5000_Tstate_send = 0;

WORD nice5000_Tstore_id1 = 0;
WORD nice5000_Tstore_data1 = 0;
WORD nice5000_Tstore_id2 = 0;
WORD nice5000_Tstore_data2 = 0;
BYTE nice5000_Tstore_send = 0;

WORD nice5000_Rstate_id1 = 0;
WORD nice5000_Rstate_data1 = 0;
WORD nice5000_Rstate_id2 = 0;
WORD nice5000_Rstate_data2 = 0;
BYTE nice5000_Rstate_new = 0;

WORD nice5000_Rstore_id1 = 0;
WORD nice5000_Rstore_data1 = 0;
WORD nice5000_Rstore_id2 = 0;
WORD nice5000_Rstore_data2 = 0;
BYTE nice5000_Rstore_new = 0;

//以下参数就是 FU-06 之后的值
WORD nice5000_Rstore_setspd = 0;
WORD nice5000_Rstore_feedspd = 0;
WORD nice5000_Rstore_dcvoltage = 0;
WORD nice5000_Rstore_outvoltage = 0;
WORD nice5000_Rstore_outcurrent = 0;
WORD nice5000_Rstore_outfreq = 0;
WORD nice5000_Rstore_outtorque = 0;		//存在正负之分
WORD nice5000_Rstore_tkcurrent = 0;
WORD nice5000_Rstore_outpwr = 0;			//存在正负之分
WORD nice5000_Rstore_pulseH = 0;
WORD nice5000_Rstore_pulseL = 0;
WORD nice5000_Rstore_position = 0;

WORD nice5000_Rfault_id1 = 0;
WORD nice5000_Rfault_data1 = 0;
WORD nice5000_Rfault_id2 = 0;
WORD nice5000_Rfault_data2 = 0;
BYTE nice5000_Rfault_new = 0;

WORD nice5000_Rfault_setspd = 0;
WORD nice5000_Rfault_feedspd = 0;
WORD nice5000_Rfault_dcvoltage = 0;
WORD nice5000_Rfault_outvoltage = 0;
WORD nice5000_Rfault_outcurrent = 0;
WORD nice5000_Rfault_outfreq = 0;
WORD nice5000_Rfault_outtorque = 0;
WORD nice5000_Rfault_tkcurrent = 0;
WORD nice5000_Rfault_outpwr = 0;
WORD nice5000_Rfault_position = 0;
WORD nice5000_Rfault_errcode = 0;
WORD nice5000_Rfault_errsub = 0;

BYTE nice5000_basicsetting_init = 0;			//初始化变频器的部分基本参数
BYTE version_drv[SW_SIZE];
const struct _nice5000_readvar c_nice5000_read_para [NICE5000_INITPARA_READSIZE] =  {
	{ &niv.F0_00,		1, },
	{ &niv.F0_01,		2, },
	{ &niv.F0_02,		3, },
	{ &niv.F0_03,		4, },
	{ &niv.F0_04,		5, },
	{ &niv.F0_05,		6, },
	{ &niv.F0_07,		7, },

	{ &niv.F1_01,		9, },
	{ &niv.F1_02,		10, },
	{ &niv.F1_03,		11, },
	{ &niv.F1_04,		12, },
	{ &niv.F1_05,		13, },
	{ &niv.F1_06,		14, },
	{ &niv.F1_07,		15, },
	{ &niv.F1_08,		16, },
	{ &niv.F1_09,		17, },
	{ &niv.F1_10,		18, },
	{ &niv.F1_11,		19, },
	{ &niv.F1_12,		20, },
	{ &niv.F1_13,		21, },
	{ &niv.F1_15,		22, },
	{ &niv.F1_16,		23, },
	{ &niv.F1_17,		24, },
	{ &niv.F1_25,		25, },

	{ &niv.F2_00,		26, },
	{ &niv.F2_01,		27, },
	{ &niv.F2_02,		29, },
	{ &niv.F2_03,		30, },
	{ &niv.F2_04,		28, },
	{ &niv.F2_05,		32, },
	{ &niv.F2_06,		33, },
	{ &niv.F2_07,		34, },
	{ &niv.F2_08,		35, },
	{ &niv.F2_09,		44, },
	{ &niv.F2_10,		45, },
	{ &niv.F2_11,		36, },
	{ &niv.F2_12,		37, },
	{ &niv.F2_13,		38, },
	{ &niv.F2_14,		39, },
	{ &niv.F2_15,		40, },
	{ &niv.F2_16,		41, },
	{ &niv.F2_17,		42, },
	{ &niv.F2_18,		1060, },
	{ &niv.F2_19,		1061, },
	{ &niv.F2_20,		1784, },
	{ &niv.F2_22,		1605, },
	{ &niv.F2_23, 	1606, },

	{ &niv.F3_00,		46, },
	{ &niv.F3_01,		47, },
	{ &niv.F3_02,		48, },
	{ &niv.F3_03,		49, },
	{ &niv.F3_04,		50, },
	{ &niv.F3_05,		51, },
	{ &niv.F3_06,		52, },
	{ &niv.F3_07,		53, },
	{ &niv.F3_08,		54, },
	{ &niv.F3_09,		55, },
	{ &niv.F3_10,		56, },
	{ &niv.F3_11,		57, },
	{ &niv.F3_12,		58, },
	{ &niv.F3_13,		59, },
	{ &niv.F3_14,		1609, },
	{ &niv.F3_15,		649, },
	{ &niv.F3_16,		648, },

	{ &niv.F4_00,		65, },
	{ &niv.F4_01,		66, },
	{ &niv.F4_02,		67, },
	{ &niv.F4_03,		68, },
	{ &niv.F4_04,		69, },
	{ &niv.F4_05,		70, },
	{ &niv.F4_06,		71, },
	{ &niv.F4_07,		72, },
	{ &niv.F4_08,		1063, },
	{ &niv.F4_09,		1064, },
	{ &niv.F4_10,		73, },
	{ &niv.F4_11,		74, },
	{ &niv.F4_12,		75, },
	{ &niv.F4_13,		76, },
	{ &niv.F4_14,		77, },
	{ &niv.F4_15,		78, },
	{ &niv.F4_16,		79, },
	{ &niv.F4_17,		80, },
	{ &niv.F4_18,		81, },
	{ &niv.F4_19,		82, },
	{ &niv.F4_20,		83, },
	{ &niv.F4_21,		84, },
	{ &niv.F4_22,		85, },
	{ &niv.F4_23,		86, },
	{ &niv.F4_24,		87, },
	{ &niv.F4_25,		88, },
	{ &niv.F4_26,		89, },
	{ &niv.F4_27,		90, },
	{ &niv.F4_28,		91, },
	{ &niv.F4_29,		92, },
	{ &niv.F4_30,		93, },
	{ &niv.F4_31,		94, },
	{ &niv.F4_32,		95, },
	{ &niv.F4_33,		96, },
	{ &niv.F4_34,		97, },
	{ &niv.F4_35,		98, },
	{ &niv.F4_36,		99, },
	{ &niv.F4_37,		100, },
	{ &niv.F4_38,		101, },
	{ &niv.F4_39,		102, },
	{ &niv.F4_40,		103, },
	{ &niv.F4_41,		104, },
	{ &niv.F4_42,		105, },
	{ &niv.F4_43,		106, },
	{ &niv.F4_44,		107, },
	{ &niv.F4_45,		108, },
	{ &niv.F4_46,		109, },
	{ &niv.F4_47,		110, },
	{ &niv.F4_48,		111, },
	{ &niv.F4_49,		112, },
	{ &niv.F4_50,		113, },
	{ &niv.F4_51,		114, },
	{ &niv.F4_52,		115, },
	{ &niv.F4_53,		116, },
	{ &niv.F4_54,		117, },
	{ &niv.F4_55,		118, },
	{ &niv.F4_56,		119, },
	{ &niv.F4_57,		120, },
	{ &niv.F4_58,		121, },
	{ &niv.F4_59,		122, },
	{ &niv.F4_60,		123, },
	{ &niv.F4_61,		124, },
	{ &niv.F4_62,		125, },
	{ &niv.F4_63,		126, },
	{ &niv.F4_64,		127, },
	{ &niv.F4_65,		128, },
	{ &niv.F4_66,		129, },
	{ &niv.F4_67,		130, },
	{ &niv.F4_68,		131, },
	{ &niv.F4_69,		132, },
	{ &niv.F4_70,		133, },
	{ &niv.F4_71,		134, },
	{ &niv.F4_72,		135, },
	{ &niv.F4_73,		136, },
	{ &niv.F4_74,		137, },
	{ &niv.F4_75,		138, },
	{ &niv.F4_76,		139, },
	{ &niv.F4_77,		140, },
	{ &niv.F4_78,		141, },
	{ &niv.F4_79,		142, },
	{ &niv.F4_80,		143, },
	{ &niv.F4_81,		144, },
	{ &niv.F4_82,		145, },
	{ &niv.F4_83,		146, },
	{ &niv.F4_84,		147, },
	{ &niv.F4_85,		148, },
	{ &niv.F4_86,		149, },
	{ &niv.F4_87,		150, },
	{ &niv.F4_88,		151, },
	{ &niv.F4_89,		152, },
	{ &niv.F4_90,		153, },
	{ &niv.F4_91,		154, },
	{ &niv.F4_92,		155, },
	{ &niv.F4_93,		156, },
	{ &niv.F4_94,		157, },
	{ &niv.F4_95,		158, },
	{ &niv.F4_96,		159, },
	{ &niv.F4_97,		160, },
	{ &niv.F4_98,		161, },
	{ &niv.F4_99,		162, },
	{ &niv.F4_100,	163, },
	{ &niv.F4_101,	164, },
	{ &niv.F4_102,	165, },
	{ &niv.F4_103,	166, },
	{ &niv.F4_104,	167, },
	{ &niv.F4_105,	168, },
	{ &niv.F4_106,	169, },
	{ &niv.F4_107,	170, },
	{ &niv.F4_108,	171, },
	{ &niv.F4_109,	172, },
	{ &niv.F4_110,	173, },
	{ &niv.F4_111,	174, },
	{ &niv.F4_112,	175, },
	{ &niv.F4_113,	176, },
	{ &niv.F4_114,	177, },
	{ &niv.F4_115,	178, },
	{ &niv.F4_116,	179, },
	{ &niv.F4_117,	180, },
	{ &niv.F4_118,	181, },
	{ &niv.F4_119,	182, },

	{ &niv.FA_06,		323, },
	{ &niv.FA_07,		324, },
	{ &niv.FA_08,		1201, },
	{ &niv.FA_09,		325, },
	{ &niv.FA_10,		326, },
	{ &niv.FA_11,		327, },
	{ &niv.FA_12,		328, },
	{ &niv.FA_13,		329, },
	{ &niv.FA_14,		1202, },
	{ &niv.FA_15,		1777, },
	{ &niv.FA_16,		1778, },
	{ &niv.FA_17,		1779, },
	{ &niv.FA_18,		1780, },
	{ &niv.FA_19,		1781, },
	{ &niv.FA_20,		1782, },
	{ &niv.FA_21,		1783, },
	{ &niv.FA_22,		1062, },
	{ &niv.FA_23,		1785, },
	{ &niv.FA_24,		1786, },
	{ &niv.FA_25,		1787, },
	{ &niv.FA_26,		1788, },
	{ &niv.FA_27,		1789, },
	{ &niv.FA_28,		1790, },
	{ &niv.FA_29,		1791, },
	{ &niv.FA_30, 	1792, },
	{ &niv.FA_31, 	1793, },
	{ &niv.FA_32, 	1794, },
	{ &niv.FA_33, 	1795, },
	{ &niv.FA_34, 	1796, },
	{ &niv.FA_35, 	1797, },
	{ &niv.FA_36, 	1798, },
	{ &niv.FA_37, 	1799, },
	{ &niv.FA_38, 	1800, },
	{ &niv.FA_39, 	1801, },
	{ &niv.FA_40, 	1802, },

	{ &niv.FH_01,		432, },
	{ &niv.FH_02,		433, },
	{ &niv.FH_03,		434, },
	{ &niv.FH_04,		438, },
	{ &niv.FH_05,		439, },
	{ &niv.FH_06,		437, },
	{ &niv.FH_07,		440, },
	{ &niv.FH_08,		441, },
	{ &niv.FH_09,		442, },
	{ &niv.FH_10,		443, },
	{ &niv.FH_11,		444, },
	{ &niv.FH_12,		445, },
	{ &niv.FH_17,		446, },
	{ &niv.FH_18,		447, },
	{ &niv.FH_19,		448, },
	{ &niv.FH_20,		449, },
	{ &niv.FH_21,		450, },
	{ &niv.FH_22,		451, },
	{ &niv.FH_23,		452, },
	{ &niv.FH_24,		453, },
	{ &niv.FH_25,		454, },
	{ &niv.FH_26,		455, },
	{ &niv.FH_27,		456, },
	{ &niv.FH_28,		457, },
	{ &niv.FH_29,		458, },

	{ &niv.FF_00,		548, },
	{ &niv.FF_01,		549, },
	{ &niv.FF_02,		550, },
	{ &niv.FF_03,		551, },
	{ &niv.FF_05,		552, },
	{ &niv.FF_06,		552, },
	{ &niv.FF_07,		554, },
	{ &niv.FF_08,		555, },
	{ &niv.FF_09,		556, },
	{ &niv.FF_10,		1937, },
	{ &niv.FF_11,		1938, },

	{ &niv.FR_00,		1181, },
	{ &niv.FR_01,		1182, },
	{ &niv.FR_02,		1183, },
	{ &niv.FR_03,		1184, },
	{ &niv.FR_04,		1185, },
	{ &niv.FR_05,		1186, },
	{ &niv.FR_06,		1187, },
	{ &niv.FR_07,		1188, },
	{ &niv.FR_08,		1189, },
	{ &niv.FR_09,		1190, },
	{ &niv.FR_10,		1191, },
	{ &niv.FR_11,		1907, },
	{ &niv.FR_12,		1908, },
	{ &niv.FR_13,		1909, },
	{ &niv.FR_14,		1910, },
	{ &niv.FR_15,		1911, },
	{ &niv.FR_16,		1912, },
	{ &niv.FR_17,		1913, },
	{ &niv.FR_18,		1914, },
	{ &niv.FR_19,		1915, },
	{ &niv.FR_20,		1916, },
	{ &niv.FR_21,		1917, },
	{ &niv.FR_22,		1918, },
	{ &niv.FR_23,		1919, },
	{ &niv.FR_24,		1920, },
	{ &niv.FR_25,		1921, },
	{ &niv.FR_26,		1922, },
	{ &niv.FR_27,		1923, },
	{ &niv.FR_28,		1924, },
	{ NULL,				0,	  },
};

const struct _nice5000_readvar c_nice5000_tiv_para [] =
{
	{ &tiv.F0_00,		1, },
	{ &tiv.F0_01,		2, },
	{ &tiv.F0_02,		3, },
	{ &tiv.F0_03,		4, },
	{ &tiv.F0_04,		5, },
	{ &tiv.F0_05,		6, },
	{ &tiv.F0_07,		7, },

	{ &tiv.F1_01,		9, },
	{ &tiv.F1_02,		10, },
	{ &tiv.F1_03,		11, },
	{ &tiv.F1_04,		12, },
	{ &tiv.F1_05,		13, },
	{ &tiv.F1_06,		14, },
	{ &tiv.F1_07,		15, },
	{ &tiv.F1_08,		16, },
	{ &tiv.F1_09,		17, },
	{ &tiv.F1_10,		18, },
	{ &tiv.F1_11,		19, },
	{ &tiv.F1_12,		20, },
	{ &tiv.F1_13,		21, },
	{ &tiv.F1_15,		22, },
	{ &tiv.F1_16,		23, },
	{ &tiv.F1_17,		24, },
	{ &tiv.F1_25,		25, },

	{ &tiv.F2_00,		26, },
	{ &tiv.F2_01,		27, },
	{ &tiv.F2_02,		29, },
	{ &tiv.F2_03,		30, },
	{ &tiv.F2_04,		28, },
	{ &tiv.F2_05,		32, },
	{ &tiv.F2_06,		33, },
	{ &tiv.F2_07,		34, },
	{ &tiv.F2_08,		35, },
	{ &tiv.F2_09,		44, },
	{ &tiv.F2_10,		45, },
	{ &tiv.F2_11,		36, },
	{ &tiv.F2_12,		37, },
	{ &tiv.F2_13,		38, },
	{ &tiv.F2_14,		39, },
	{ &tiv.F2_15,		40, },
	{ &tiv.F2_16,		41, },
	{ &tiv.F2_17,		42, },
	{ &tiv.F2_18,		1060, },
	{ &tiv.F2_19,		1061, },
	{ &tiv.F2_20,		1784, },
	{ &tiv.F2_22,		1605, },
	{ &tiv.F2_23, 	1606, },

	{ &tiv.F3_00,		46, },
	{ &tiv.F3_01,		47, },
	{ &tiv.F3_02,		48, },
	{ &tiv.F3_03,		49, },
	{ &tiv.F3_04,		50, },
	{ &tiv.F3_05,		51, },
	{ &tiv.F3_06,		52, },
	{ &tiv.F3_07,		53, },
	{ &tiv.F3_08,		54, },
	{ &tiv.F3_09,		55, },
	{ &tiv.F3_10,		56, },
	{ &tiv.F3_11,		57, },
	{ &tiv.F3_12,		58, },
	{ &tiv.F3_13,		59, },
	{ &tiv.F3_14,		1609, },
	{ &tiv.F3_15,		649, },
	{ &tiv.F3_16,		648, },

	{ &tiv.F4_00,		65, },
	{ &tiv.F4_01,		66, },
	{ &tiv.F4_02,		67, },
	{ &tiv.F4_03,		68, },
	{ &tiv.F4_04,		69, },
	{ &tiv.F4_05,		70, },
	{ &tiv.F4_06,		71, },
	{ &tiv.F4_07,		72, },
	{ &tiv.F4_08,		1063, },
	{ &tiv.F4_09,		1064, },
	{ &tiv.F4_10,		73, },
	{ &tiv.F4_11,		74, },
	{ &tiv.F4_12,		75, },
	{ &tiv.F4_13,		76, },
	{ &tiv.F4_14,		77, },
	{ &tiv.F4_15,		78, },
	{ &tiv.F4_16,		79, },
	{ &tiv.F4_17,		80, },
	{ &tiv.F4_18,		81, },
	{ &tiv.F4_19,		82, },
	{ &tiv.F4_20,		83, },
	{ &tiv.F4_21,		84, },
	{ &tiv.F4_22,		85, },
	{ &tiv.F4_23,		86, },
	{ &tiv.F4_24,		87, },
	{ &tiv.F4_25,		88, },
	{ &tiv.F4_26,		89, },
	{ &tiv.F4_27,		90, },
	{ &tiv.F4_28,		91, },
	{ &tiv.F4_29,		92, },
	{ &tiv.F4_30,		93, },
	{ &tiv.F4_31,		94, },
	{ &tiv.F4_32,		95, },
	{ &tiv.F4_33,		96, },
	{ &tiv.F4_34,		97, },
	{ &tiv.F4_35,		98, },
	{ &tiv.F4_36,		99, },
	{ &tiv.F4_37,		100, },
	{ &tiv.F4_38,		101, },
	{ &tiv.F4_39,		102, },
	{ &tiv.F4_40,		103, },
	{ &tiv.F4_41,		104, },
	{ &tiv.F4_42,		105, },
	{ &tiv.F4_43,		106, },
	{ &tiv.F4_44,		107, },
	{ &tiv.F4_45,		108, },
	{ &tiv.F4_46,		109, },
	{ &tiv.F4_47,		110, },
	{ &tiv.F4_48,		111, },
	{ &tiv.F4_49,		112, },
	{ &tiv.F4_50,		113, },
	{ &tiv.F4_51,		114, },
	{ &tiv.F4_52,		115, },
	{ &tiv.F4_53,		116, },
	{ &tiv.F4_54,		117, },
	{ &tiv.F4_55,		118, },
	{ &tiv.F4_56,		119, },
	{ &tiv.F4_57,		120, },
	{ &tiv.F4_58,		121, },
	{ &tiv.F4_59,		122, },
	{ &tiv.F4_60,		123, },
	{ &tiv.F4_61,		124, },
	{ &tiv.F4_62,		125, },
	{ &tiv.F4_63,		126, },
	{ &tiv.F4_64,		127, },
	{ &tiv.F4_65,		128, },
	{ &tiv.F4_66,		129, },
	{ &tiv.F4_67,		130, },
	{ &tiv.F4_68,		131, },
	{ &tiv.F4_69,		132, },
	{ &tiv.F4_70,		133, },
	{ &tiv.F4_71,		134, },
	{ &tiv.F4_72,		135, },
	{ &tiv.F4_73,		136, },
	{ &tiv.F4_74,		137, },
	{ &tiv.F4_75,		138, },
	{ &tiv.F4_76,		139, },
	{ &tiv.F4_77,		140, },
	{ &tiv.F4_78,		141, },
	{ &tiv.F4_79,		142, },
	{ &tiv.F4_80,		143, },
	{ &tiv.F4_81,		144, },
	{ &tiv.F4_82,		145, },
	{ &tiv.F4_83,		146, },
	{ &tiv.F4_84,		147, },
	{ &tiv.F4_85,		148, },
	{ &tiv.F4_86,		149, },
	{ &tiv.F4_87,		150, },
	{ &tiv.F4_88,		151, },
	{ &tiv.F4_89,		152, },
	{ &tiv.F4_90,		153, },
	{ &tiv.F4_91,		154, },
	{ &tiv.F4_92,		155, },
	{ &tiv.F4_93,		156, },
	{ &tiv.F4_94,		157, },
	{ &tiv.F4_95,		158, },
	{ &tiv.F4_96,		159, },
	{ &tiv.F4_97,		160, },
	{ &tiv.F4_98,		161, },
	{ &tiv.F4_99,		162, },
	{ &tiv.F4_100,		163, },
	{ &tiv.F4_101,		164, },
	{ &tiv.F4_102,		165, },
	{ &tiv.F4_103,		166, },
	{ &tiv.F4_104,		167, },
	{ &tiv.F4_105,		168, },
	{ &tiv.F4_106,		169, },
	{ &tiv.F4_107,		170, },
	{ &tiv.F4_108,		171, },
	{ &tiv.F4_109,		172, },
	{ &tiv.F4_110,		173, },
	{ &tiv.F4_111,		174, },
	{ &tiv.F4_112,		175, },
	{ &tiv.F4_113,		176, },
	{ &tiv.F4_114,		177, },
	{ &tiv.F4_115,		178, },
	{ &tiv.F4_116,		179, },
	{ &tiv.F4_117,		180, },
	{ &tiv.F4_118,		181, },
	{ &tiv.F4_119,		182, },

	{ &tiv.F5_X1,		184, },
	{ &tiv.F5_X2,		185, },
	{ &tiv.F5_X3,		186, },
	{ &tiv.F5_X4,		187, },
	{ &tiv.F5_X5,		188, },
	{ &tiv.F5_X6,		189, },
	{ &tiv.F5_X7,		190, },
	{ &tiv.F5_X8,		191, },

	{ &tiv.F6_top_floor,	224, },
	{ &tiv.F6_bot_floor,	225, },

	{ &tiv.FA_06,		323, },
	{ &tiv.FA_07,		324, },
	{ &tiv.FA_08,		1201, },
	{ &tiv.FA_09,		325, },
	{ &tiv.FA_10,		326, },
	{ &tiv.FA_11,		327, },
	{ &tiv.FA_12,		328, },
	{ &tiv.FA_13,		329, },
	{ &tiv.FA_14,		1202, },
	{ &tiv.FA_15,		1777, },
	{ &tiv.FA_16,		1778, },
	{ &tiv.FA_17,		1779, },
	{ &tiv.FA_18,		1780, },
	{ &tiv.FA_19,		1781, },
	{ &tiv.FA_20,		1782, },
	{ &tiv.FA_21,		1783, },
	{ &tiv.FA_22, 	1062, },
	{ &tiv.FA_23, 	1785, },
	{ &tiv.FA_24, 	1786, },
	{ &tiv.FA_25, 	1787, },
	{ &tiv.FA_26, 	1788, },
	{ &tiv.FA_27, 	1789, },
	{ &tiv.FA_28, 	1790, },
	{ &tiv.FA_29, 	1791, },
	{ &tiv.FA_30, 	1792, },
	{ &tiv.FA_31, 	1793, },
	{ &tiv.FA_32, 	1794, },
	{ &tiv.FA_33, 	1795, },
	{ &tiv.FA_34, 	1796, },
	{ &tiv.FA_35, 	1797, },
	{ &tiv.FA_36, 	1798, },
	{ &tiv.FA_37, 	1799, },
	{ &tiv.FA_38, 	1800, },
	{ &tiv.FA_39, 	1801, },
	{ &tiv.FA_40, 	1802, },

	{ &tiv.FH_01,		432, },
	{ &tiv.FH_02,		433, },
	{ &tiv.FH_03,		434, },
	{ &tiv.FH_04,		438, },
	{ &tiv.FH_05,		439, },
	{ &tiv.FH_06,		437, },
	{ &tiv.FH_07,		440, },
	{ &tiv.FH_08,		441, },
	{ &tiv.FH_09,		442, },
	{ &tiv.FH_10,		443, },
	{ &tiv.FH_11,		444, },
	{ &tiv.FH_12,		445, },
	{ &tiv.FH_17,		446, },
	{ &tiv.FH_18,		447, },
	{ &tiv.FH_19,		448, },
	{ &tiv.FH_20,		449, },
	{ &tiv.FH_21,		450, },
	{ &tiv.FH_22,		451, },
	{ &tiv.FH_23,		452, },
	{ &tiv.FH_24,		453, },
	{ &tiv.FH_25,		454, },
	{ &tiv.FH_26,		455, },
	{ &tiv.FH_27,		456, },
	{ &tiv.FH_28,		457, },
	{ &tiv.FH_29,		458, },

	{ &tiv.FF_00,		548, },
	{ &tiv.FF_01,		549, },
	{ &tiv.FF_02,		550, },
	{ &tiv.FF_03,		551, },
	{ &tiv.FF_05,		552, },
	{ &tiv.FF_06,		552, },
	{ &tiv.FF_07,		554, },
	{ &tiv.FF_08,		555, },
	{ &tiv.FF_09,		556, },
	{ &tiv.FF_10,		1937, },
	{ &tiv.FF_11,		1938, },

	{ &tiv.FR_00,		1181, },
	{ &tiv.FR_01,		1182, },
	{ &tiv.FR_02,		1183, },
	{ &tiv.FR_03,		1184, },
	{ &tiv.FR_04,		1185, },
	{ &tiv.FR_05,		1186, },
	{ &tiv.FR_06,		1187, },
	{ &tiv.FR_07,		1188, },
	{ &tiv.FR_08,		1189, },
	{ &tiv.FR_09,		1190, },
	{ &tiv.FR_10,		1191, },
	{ &tiv.FR_11,		1907, },
	{ &tiv.FR_12,		1908, },
	{ &tiv.FR_13,		1909, },
	{ &tiv.FR_14,		1910, },
	{ &tiv.FR_15,		1911, },
	{ &tiv.FR_16,		1912, },
	{ &tiv.FR_17,		1913, },
	{ &tiv.FR_18,		1914, },
	{ &tiv.FR_19,		1915, },
	{ &tiv.FR_20,		1916, },
	{ &tiv.FR_21,		1917, },
	{ &tiv.FR_22,		1918, },
	{ &tiv.FR_23,		1919, },
	{ &tiv.FR_24,		1920, },
	{ &tiv.FR_25,		1921, },
	{ &tiv.FR_26,		1922, },
	{ &tiv.FR_27,		1923, },
	{ &tiv.FR_28,		1924, },
	{ NULL,				0,	  },

};


const struct _nice5000_writevar c_nice5000_write_para[] = {
	{ &p.F5_X1,			sizeof(p.F5_X1),			184, },
	{ &p.F5_X2,			sizeof(p.F5_X2),			185, },
	{ &p.F5_X3,			sizeof(p.F5_X3),			186, },
	{ &p.F5_X4,			sizeof(p.F5_X4),			187, },
	{ &p.F5_X5,			sizeof(p.F5_X5),			188, },
	{ &p.F5_X6,			sizeof(p.F5_X6),			189, },
	{ &p.F5_X7,			sizeof(p.F5_X7),			190, },
	{ &p.F5_X8,			sizeof(p.F5_X8),			191, },

	{ &p.top_floor,		sizeof(p.top_floor),		224, },
	{ &p.bot_floor,		sizeof(p.bot_floor),		225, },
	{ NULL,				0,							0,	 },
};

const struct _nice5000_initvar c_nice5000_init_para[] = {//Basic Setting 设置的变频器参数
	{ &niv.F3_11,		sizeof(niv.F3_11),			57,			NICE5000_INITMODE_VALUEGIVE,0,		200, },
	{ &niv.F3_13,		sizeof(niv.F3_13),			59,			NICE5000_INITMODE_VALUEGIVE,0,		400, },
	{ &niv.F3_14, 	sizeof(niv.F3_14),			1609,		NICE5000_INITMODE_VALUEGIVE,0,		0, },
	{ &niv.FA_15,		sizeof(niv.FA_15),			1777,		NICE5000_INITMODE_VALUEGIVE,	0,	80, },
	{ &niv.FA_12, 	sizeof(niv.FA_12),			328, 		NICE5000_INITMODE_VALUEGIVE,0,		512, },
	{ &niv.F2_18, 	sizeof(niv.F2_18),			1060, 	NICE5000_INITMODE_VALUEGIVE,0,		200, },
	{ &niv.FA_35, 	sizeof(niv.FA_35),			1797, 	NICE5000_INITMODE_VALUEGIVE,	0,	5, },
	{ &niv.FA_36, 	sizeof(niv.FA_36),			1798,		NICE5000_INITMODE_VALUEGIVE,0,		100, },
	{ &niv.FA_37, 	sizeof(niv.FA_37),			1799, 	NICE5000_INITMODE_VALUEGIVE,0,		5, },
	{ &niv.FA_38, 	sizeof(niv.FA_38),			1800, 	NICE5000_INITMODE_VALUEGIVE,0,		0, },

	{ NULL,				0,							0,	 		0,							0,		0ul, },
};


DWORD nice5000_Rfault_chk = 0;
BYTE nice5000_Rfault_recorded = 0;

WORD CrcChkValue(WORD * dataptr, WORD len)
{
	WORD crc = 0xEAFA;
//	WORD crc = 0xF5F5;

	while (len --)
	{
		crc ^= *dataptr ++;
	}
	return crc;
}

void ConductNiceSpiData(void)
{
	if (SSP1SR & SSP_SR_RNE)
	{
		spi_rx_fifo[spi_rx_offset] = ~SSP1DR;
		if (!nice5000_crc_error)
		{
			++ spi_rx_offset;
			if (spi_rx_offset == NICE5000_SPI_FRMSIZE)
			{
				spi_rx_offset = 0;
				if (spi_rx_fifo[NICE5000_SPI_FRMSIZE-1] == CrcChkValue(spi_rx_fifo, NICE5000_SPI_FRMSIZE-1))
				{
					if (hb_error & B_DRV)
					{
						if (nice5000_hbrst_count)
						{
							-- nice5000_hbrst_count;
							if (!nice5000_hbrst_count)
								hb_error &= ~B_DRV;
						}
					}
					switch(spi_rx_fifo[0])
					{
						case FRM_HEAD_CMD1:
							nice5000_Rcmd_data1 = spi_rx_fifo[1];
							nice5000_Rcmd_data2 = spi_rx_fifo[2];
							nice5000_running_step = nice5000_Rcmd_data2 & NICE5000_RCMD2_RUNNING_STEP;
							nice5000_Rcmd_data3 = spi_rx_fifo[3];
							level = (nice5000_Rcmd_data3 & NICE5000_RCMD3_CURRENTFLOOR) - 1;
							nice5000_Rcmd_data4 = spi_rx_fifo[4];
							break;
						case FRM_HEAD_CMD2:
							nice5000_Rcmd_data1 = spi_rx_fifo[1];
							nice5000_Rcmd_data5 = spi_rx_fifo[2];
							nice5000_Rcmd_data6 = spi_rx_fifo[3];
							nice5000_Rcmd_data7 = spi_rx_fifo[4];
							break;
						case FRM_HEAD_FCT1:
							if (nice5000_Rstate_new)
								nice5000_Rstate_new = 0;
							nice5000_Rstate_id1 = spi_rx_fifo[1];
							nice5000_Rstate_data1 = spi_rx_fifo[2];
							nice5000_Rstate_id2 = spi_rx_fifo[3];
							nice5000_Rstate_data2 = spi_rx_fifo[4];
							nice5000_Rstate_new = 1;
							if (hse_state != H_INIT)
								GetNiceTivPara(spi_rx_fifo);
							break;
						case FRM_HEAD_FCT2:
							if (nice5000_Rstore_new)
								nice5000_Rstore_new = 0;
							nice5000_Rstore_id1 = spi_rx_fifo[1];
							nice5000_Rstore_data1 = spi_rx_fifo[2];
							nice5000_Rstore_id2 = spi_rx_fifo[3];
							nice5000_Rstore_data2 = spi_rx_fifo[4];
							GetNiceStatus();
							nice5000_Rstore_new = 1;
							if (hse_state != H_INIT)
								GetNiceTivPara(spi_rx_fifo);
							break;
						case FRM_HEAD_FAULT:
							if (nice5000_Rfault_new)
								nice5000_Rfault_new = 0;
							nice5000_Rfault_id1 = spi_rx_fifo[1];
							nice5000_Rfault_data1 = spi_rx_fifo[2];
							nice5000_Rfault_id2 = spi_rx_fifo[3];
							nice5000_Rfault_data2 = spi_rx_fifo[4];
							GetNiceFault();
							nice5000_Rfault_new = 1;
							break;
					}
					nice5000_crc_errcnt = 0;
				}
				else
				{// CRC error
					nice5000_crc_error = 1;
					if ((nice5000_crc_errcnt >= NICE5000_CRC_ERRCNT) && ((p.simulation < SIMULATION_AUTO)))
					{
						if (!(hb_error & B_DRV))
						{
							hb_error |= B_DRV;
							nice5000_hbrst_count = NICE5000_HB_RSTCNT;
							write_errorhist (E_HEARTBEAT_DRV, DRV_ID, 0, W_HB_ASE);
							init_error &= ~B_DRV;
							iv_para_valid = 0;
						}
					}
					else
						++ nice5000_crc_errcnt;
				}
			}
		}
	}
	if (safety_circuit & SC1)
		nice5000_Tcmd_data4 |= NICE5000_TCMD4_PSL;
	else
		nice5000_Tcmd_data4 &= ~NICE5000_TCMD4_PSL;
	if ((safety_circuit & (SC2 | SC3)) == (SC2 | SC3))
		nice5000_Tcmd_data4 |= NICE5000_TCMD4_SDSL;
	else
		nice5000_Tcmd_data4 &= ~NICE5000_TCMD4_SDSL;
}

void GetNiceStatus(void)
{
	static WORD lastposH, lastposL;

	switch (nice5000_Rstore_id1)
	{
		case NICE5000_RSTORE_SETSPEED:
			nice5000_Rstore_setspd = nice5000_Rstore_data1;
			break;
		case NICE5000_RSTORE_FEEDSPEED:
			nice5000_Rstore_feedspd = nice5000_Rstore_data1;
			break;
		case NICE5000_RSTORE_DCVOLTAGE:
			nice5000_Rstore_dcvoltage = nice5000_Rstore_data1;
			break;
		case NICE5000_RSTORE_OUTVOLTAGE:
			nice5000_Rstore_outvoltage = nice5000_Rstore_data1;
			break;
		case NICE5000_RSTORE_OUTCURRENT:
			nice5000_Rstore_outcurrent = nice5000_Rstore_data1;
			break;
		case NICE5000_RSTORE_OUTFREQ:
			nice5000_Rstore_outfreq = nice5000_Rstore_data1;
			break;
		case NICE5000_RSTORE_OUTTORQUE:
			nice5000_Rstore_outtorque = nice5000_Rstore_data1;
			break;
		case NICE5000_RSTORE_TKCURRENT:
			nice5000_Rstore_tkcurrent = nice5000_Rstore_data1;
			break;
		case NICE5000_RSTORE_OUTPWR:
			nice5000_Rstore_outpwr = nice5000_Rstore_data1;
			break;
		case NICE5000_RSTORE_PULSEH:
			lastposH = nice5000_Rstore_pulseH;
			nice5000_Rstore_pulseH = nice5000_Rstore_data1;
			dPosHcnt = dTmrCnt;
			break;
		case NICE5000_RSTORE_PULSEL:
			lastposL = nice5000_Rstore_pulseL;
			nice5000_Rstore_pulseL = nice5000_Rstore_data1;
			dPosLcnt = dTmrCnt;
			Load_pos(lastposL, lastposH);
			break;
		case NICE5000_RSTORE_POSITION:
			nice5000_Rstore_position = nice5000_Rstore_data1;
			break;
			
		case NICE5000_BRAKE_CHECK_ID:
			brake_check_data = (BYTE)nice5000_Rstore_data1;
			break;
	}
	switch (nice5000_Rstore_id2)
	{
		case NICE5000_RSTORE_SETSPEED:
			nice5000_Rstore_setspd = nice5000_Rstore_data2;
			break;
		case NICE5000_RSTORE_FEEDSPEED:
			nice5000_Rstore_feedspd = nice5000_Rstore_data2;
			break;
		case NICE5000_RSTORE_DCVOLTAGE:
			nice5000_Rstore_dcvoltage = nice5000_Rstore_data2;
			break;
		case NICE5000_RSTORE_OUTVOLTAGE:
			nice5000_Rstore_outvoltage = nice5000_Rstore_data2;
			break;
		case NICE5000_RSTORE_OUTCURRENT:
			nice5000_Rstore_outcurrent = nice5000_Rstore_data2;
			break;
		case NICE5000_RSTORE_OUTFREQ:
			nice5000_Rstore_outfreq = nice5000_Rstore_data2;
			break;
		case NICE5000_RSTORE_OUTTORQUE:
			nice5000_Rstore_outtorque = nice5000_Rstore_data2;
			break;
		case NICE5000_RSTORE_TKCURRENT:
			nice5000_Rstore_tkcurrent = nice5000_Rstore_data2;
			break;
		case NICE5000_RSTORE_OUTPWR:
			nice5000_Rstore_outpwr = nice5000_Rstore_data2;
			break;
		case NICE5000_RSTORE_PULSEH:
			lastposH = nice5000_Rstore_pulseH;
			nice5000_Rstore_pulseH = nice5000_Rstore_data2;
			dPosHcnt = dTmrCnt;
			break;
		case NICE5000_RSTORE_PULSEL:
			lastposL = nice5000_Rstore_pulseL;
			nice5000_Rstore_pulseL = nice5000_Rstore_data2;
			dPosLcnt = dTmrCnt;
			Load_pos(lastposL, lastposH);
			break;
		case NICE5000_RSTORE_POSITION:
			nice5000_Rstore_position = nice5000_Rstore_data2;
			break;
		case NICE5000_BRAKE_CHECK_ID:
			brake_check_data = (BYTE)nice5000_Rstore_data2;
			break;
	}
}


// 清除变频器故障信息读取标记
void ResetNiceFaultChk(void)
{
	nice5000_Rfault_recorded = 0;
	nice5000_Rfault_chk = 0;
}


const WORD _nice5000_fault_info_id[] =
{
	NICE5000_RFAULT_SETSPEED,
	NICE5000_RFAULT_FEEDSPEED,
	NICE5000_RFAULT_DCVOLTAGE,
	NICE5000_RFAULT_OUTVOLTAGE,
	NICE5000_RFAULT_OUTCURRENT,
	NICE5000_RFAULT_OUTFREQ,
	NICE5000_RFAULT_OUTTORQUE,
	NICE5000_RFAULT_TKCURRENT,
	NICE5000_RFAULT_OUTPWR,
	NICE5000_RFAULT_POSITION,
	NICE5000_RFAULT_ERRCODE,
	NICE5000_RFAULT_ERRSUB,
};

//判断是否所有故障信息都已读取，如果是，将变频器故障信息和系统状态一起写入EEPROM
BYTE IsNiceFaultChked(void)
{
	if (nice5000_Rfault_chk != HSE_MARIN[sizeof(_nice5000_fault_info_id)/sizeof(WORD)])
		return 0x00;
	if ((!nice5000_Rfault_recorded) && (nice5000_Rfault_errcode == nice5000_Rcmd_data5))
	{
		nice5000_Rfault_recorded = 1;
		write_errorhist (E_IV_ERROR + nice5000_Rfault_errcode, DRV_ID, C_NICE_ERROR, W_NICE_ERROR);
	}
	return 0x01;
}

// 从交互协议中抽取故障信息，并作已读标记
void GetNiceFault(void)
{
	switch (nice5000_Rfault_id1)
	{
		case NICE5000_RFAULT_SETSPEED:
			nice5000_Rfault_setspd = nice5000_Rfault_data1;
			nice5000_Rfault_chk |= 0x01ul;
			break;
			
		case NICE5000_RFAULT_FEEDSPEED:
			nice5000_Rfault_feedspd = nice5000_Rfault_data1;
			nice5000_Rfault_chk |= 0x02ul;
			break;
			
		case NICE5000_RFAULT_DCVOLTAGE:
			nice5000_Rfault_dcvoltage = nice5000_Rfault_data1;
			nice5000_Rfault_chk |= 0x04ul;
			break;
			
		case NICE5000_RFAULT_OUTVOLTAGE:
			nice5000_Rfault_outvoltage = nice5000_Rfault_data1;
			nice5000_Rfault_chk |= 0x08ul;
			break;
		case NICE5000_RFAULT_OUTCURRENT:
			nice5000_Rfault_outcurrent = nice5000_Rfault_data1;
			nice5000_Rfault_chk |= 0x10ul;
			break;
			
		case NICE5000_RFAULT_OUTFREQ:
			nice5000_Rfault_outfreq = nice5000_Rfault_data1;
			nice5000_Rfault_chk |= 0x20ul;
			break;
		case NICE5000_RFAULT_OUTTORQUE:
			nice5000_Rfault_outtorque = nice5000_Rfault_data1;
			nice5000_Rfault_chk |= 0x40ul;
			break;
			
		case NICE5000_RFAULT_TKCURRENT:
			nice5000_Rfault_tkcurrent = nice5000_Rfault_data1;
			nice5000_Rfault_chk |= 0x80ul;
			break;
		case NICE5000_RFAULT_OUTPWR:
			nice5000_Rfault_outpwr = nice5000_Rfault_data1;
			nice5000_Rfault_chk |= 0x100ul;
			break;
			
		case NICE5000_RFAULT_POSITION:
			nice5000_Rfault_position = nice5000_Rfault_data1;
			nice5000_Rfault_chk |= 0x200ul;
			break;
			
		case NICE5000_RFAULT_ERRCODE:
			nice5000_Rfault_errcode = nice5000_Rfault_data1;
			nice5000_Rfault_chk |= 0x400ul;
			break;	
			
		case NICE5000_RFAULT_ERRSUB:
			nice5000_Rfault_errsub = nice5000_Rfault_data1;
			nice5000_Rfault_chk |= 0x800ul;
			break;			
	}
	switch (nice5000_Rfault_id2)
	{
		case NICE5000_RFAULT_SETSPEED:
			nice5000_Rfault_setspd = nice5000_Rfault_data2;
			nice5000_Rfault_chk |= 0x01ul;
			break;
			
		case NICE5000_RFAULT_FEEDSPEED:
			nice5000_Rfault_feedspd = nice5000_Rfault_data2;
			nice5000_Rfault_chk |= 0x02ul;
			break;
			
		case NICE5000_RFAULT_DCVOLTAGE:
			nice5000_Rfault_dcvoltage = nice5000_Rfault_data2;
			nice5000_Rfault_chk |= 0x04ul;
			break;
			
		case NICE5000_RFAULT_OUTVOLTAGE:
			nice5000_Rfault_outvoltage = nice5000_Rfault_data2;
			nice5000_Rfault_chk |= 0x08ul;
			break;
			
		case NICE5000_RFAULT_OUTCURRENT:
			nice5000_Rfault_outcurrent = nice5000_Rfault_data2;
			nice5000_Rfault_chk |= 0x10ul;
			break;
			
		case NICE5000_RFAULT_OUTFREQ:
			nice5000_Rfault_outfreq = nice5000_Rfault_data2;
			nice5000_Rfault_chk |= 0x20ul;
			break;
			
		case NICE5000_RFAULT_OUTTORQUE:
			nice5000_Rfault_outtorque = nice5000_Rfault_data2;
			nice5000_Rfault_chk |= 0x40ul;
			break;
			
		case NICE5000_RFAULT_TKCURRENT:
			nice5000_Rfault_tkcurrent = nice5000_Rfault_data2;
			nice5000_Rfault_chk |= 0x80ul;
			break;
			
		case NICE5000_RFAULT_OUTPWR:
			nice5000_Rfault_outpwr = nice5000_Rfault_data2;
			nice5000_Rfault_chk |= 0x100ul;
			break;
			
		case NICE5000_RFAULT_POSITION:
			nice5000_Rfault_position = nice5000_Rfault_data2;
			nice5000_Rfault_chk |= 0x200ul;
			break;
			
		case NICE5000_RFAULT_ERRCODE:
			nice5000_Rfault_errcode = nice5000_Rfault_data2;
			nice5000_Rfault_chk |= 0x400ul;
			break;
			
		case NICE5000_RFAULT_ERRSUB:
			nice5000_Rfault_errsub = nice5000_Rfault_data2;
			nice5000_Rfault_chk |= 0x800ul;
			break;			
	}
}

void Load_pos(WORD lastl, WORD lasth){
	DWORD tmrcnt_diff1;
	WORD ldiff;

	if (nice5000_Rstore_pulseL > lastl)
		ldiff = nice5000_Rstore_pulseL - lastl;
	else
		ldiff = lastl - nice5000_Rstore_pulseL;
	if ((lasth == nice5000_Rstore_pulseH) && (ldiff > 0x8000))	//传送延误造成
		return;

	tmrcnt_diff1 = dPosLcnt - dPosHcnt;
	if (tmrcnt_diff1 > 10)				// 相邻的 pos 高低位，两次 SPI 访问不应超过 5 次
		return;							// 判断为当次 posH 丢失
	dCurrentPulse = (((DWORD)nice5000_Rstore_pulseH) << 16) + nice5000_Rstore_pulseL;
}

void GetNiceTivPara(WORD * ptr)
{
	WORD * word_ptr;
	WORD i;

	for (i=0; c_nice5000_tiv_para[i].ptr != NULL; ++i)
	{
		if (c_nice5000_tiv_para[i].id == *(ptr + 1))
		{
			word_ptr = c_nice5000_tiv_para[i].ptr;
			*word_ptr = *(ptr + 2);
		}
		if (c_nice5000_tiv_para[i].id == *(ptr + 3))
		{
			word_ptr = c_nice5000_tiv_para[i].ptr;
			*word_ptr = *(ptr + 4);
		}
	}
}

void init_nice5000(void)
{
	BYTE para_chked[(NICE5000_INITPARA_READSIZE / 8) + 1];
	BYTE nice5000_Rstate_chk = 1;
	BYTE nice5000_Tstate_init = 1;
	DWORD * dword_ptr;
	WORD * word_ptr;
	BYTE * byte_ptr;
	WORD value = 0;
	WORD i = 0;
	WORD j = 0;
	WORD k = 0;
	WORD m = 0;

	memset(para_chked, 0x00, sizeof(para_chked));
	nice5000_Rstore_new = 0;
	while(nice5000_Rstate_chk | nice5000_Tstate_init | nice5000_basicsetting_init)
	{
		cycle();
		if ((nice5000_Rstate_new) && (nice5000_Rstate_chk))
		{
			for (i=0; c_nice5000_read_para[i].ptr != NULL; ++i)
			{
				if (c_nice5000_read_para[i].id == nice5000_Rstate_id1)
				{
					word_ptr = c_nice5000_read_para[i].ptr;
					*word_ptr = nice5000_Rstate_data1;
					para_chked[i/8] |= 1 << (i%8);
				}
				if (c_nice5000_read_para[i].id == nice5000_Rstate_id2)
				{
					word_ptr = c_nice5000_read_para[i].ptr;
					*word_ptr = nice5000_Rstate_data2;
					para_chked[i/8] |= 1 << (i%8);
				}
			}
			j = 1;
			for (i=0; c_nice5000_read_para[i].ptr != NULL; ++i)
			{
				if (!(para_chked[i/8] & (1 << (i%8))))
				{
					j = 0;
					break;
				}
			}
			if (j)
			{
				nice5000_Rstate_chk = 0;
				nice5000_Tcmd_data4 |= NICE5000_TCMD4_R_READY;
			}
			nice5000_Rstate_new = 0;
		}
		if ((!nice5000_Tstate_send) && (nice5000_Tstate_init))
		{
			switch (c_nice5000_write_para[k].size)
			{
				case 1:
					byte_ptr = (BYTE *)c_nice5000_write_para[k].ptr;
					value = *byte_ptr;
					break;
				case 2:
					word_ptr = (WORD *)c_nice5000_write_para[k].ptr;
					value = *word_ptr;
					break;
				case 4:
					dword_ptr = (DWORD *)c_nice5000_write_para[k].ptr;
					value = *dword_ptr;
					break;
			}
			nice5000_Tstate_id1 = c_nice5000_write_para[k].id;
			if ((nice5000_Tstate_id1 == NICE5000_RSTATE_BOTFLOOR)
				|| (nice5000_Tstate_id1 == NICE5000_RSTATE_TOPFLOOR))
				++ value;
			nice5000_Tstate_data1 = value;
			nice5000_Tstate_id2 = 0;
			nice5000_Tstate_data2 = 0;
			nice5000_Tstate_send = FRM_FCT_SENDTIMES;
			++ k;
			if (c_nice5000_write_para[k].ptr == NULL)
			{
				nice5000_Tstate_init = 0;
				nice5000_Tcmd_data4 |= NICE5000_TCMD4_T_READY;
			}
		}
		if ((nice5000_basicsetting_init) && (!nice5000_Rstate_chk) && (!nice5000_Tstate_init))
		{
			if (!nice5000_Tstore_send)
			{
				switch(c_nice5000_init_para[m].mode)
				{
					case NICE5000_INITMODE_VALUEGIVE:
						switch(c_nice5000_init_para[m].size)
						{
							case 1:
								byte_ptr = (BYTE *)c_nice5000_init_para[m].ptr;
								*byte_ptr = (BYTE)c_nice5000_init_para[m].value;
								value = (WORD)c_nice5000_init_para[m].value;
								break;
							case 2:
								word_ptr = (WORD *)c_nice5000_init_para[m].ptr;
								*word_ptr = (WORD)c_nice5000_init_para[m].value;
								value = (WORD)c_nice5000_init_para[m].value;
								break;
							case 4:
								dword_ptr = (DWORD *)c_nice5000_init_para[m].ptr;
								*dword_ptr = c_nice5000_init_para[m].value;
								value = (WORD)c_nice5000_init_para[m].value;
								break;
						}						
						break;
					case NICE5000_INITMODE_BITSET:
						switch(c_nice5000_init_para[m].size)
						{
							case 1:
								byte_ptr = (BYTE *)c_nice5000_init_para[m].ptr;
								*byte_ptr |= (0x01ul << c_nice5000_init_para[m].bit);
								value = *byte_ptr;
								break;
							case 2:
								word_ptr = (WORD *)c_nice5000_init_para[m].ptr;
								*word_ptr |= (0x01ul << c_nice5000_init_para[m].bit);
								value = *word_ptr;
								break;
							case 4:
								dword_ptr = (DWORD *)c_nice5000_init_para[m].ptr;
								*dword_ptr |= (0x01ul << c_nice5000_init_para[m].bit);
								value = (WORD)(*dword_ptr);
								break;
						}						
						break;
					case NICE5000_INITMODE_BITCLR:
						switch(c_nice5000_init_para[m].size)
						{
							case 1:
								byte_ptr = (BYTE *)c_nice5000_init_para[m].ptr;
								*byte_ptr &= ~(0x01ul << c_nice5000_init_para[m].bit);
								value = *byte_ptr;
								break;
							case 2:
								word_ptr = (WORD *)c_nice5000_init_para[m].ptr;
								*word_ptr &= ~(0x01ul << c_nice5000_init_para[m].bit);
								value = *word_ptr;
								break;
							case 4:
								dword_ptr = (DWORD *)c_nice5000_init_para[m].ptr;
								*dword_ptr &= ~(0x01ul << c_nice5000_init_para[m].bit);
								value = (WORD)(*dword_ptr);
								break;
						}						
						break;
				}
				nice5000_Tstore_id1 = c_nice5000_init_para[m].id;
				nice5000_Tstore_data1 = value;
				nice5000_Tstore_id2 = 0;
				nice5000_Tstore_data2 = 0;
				nice5000_Tstore_send = FRM_FCT_SENDTIMES;

				++ m;
				if (c_nice5000_init_para[m].ptr == NULL)
					nice5000_basicsetting_init = 0;
			}
		}
	}

	sprintf((char*)version_drv, "%4d", niv.FA_06);
	init_error &= ~B_DRV;
	nice5000_Tcmd_data4 |= NICE5000_TCMD4_L_READY;
	iv_para_valid = 1;
	correct_level = (BYTE)niv.FR_00;
}

void drive_nice5000(void){
	static BYTE drive_state_old = 0;
	static BYTE slowing = 0;
	static BYTE wait;
	static DWORD dt;
	static DWORD ct;
	static DWORD bt;
	static DWORD wt;
	static DWORD mt;
	volatile WORD cmd;
//以下变量给抱闸力检测专用
	static DWORD wait_timer = 0;
	static BYTE step = 0;
	
	if (nice5000_Rcmd_data5 & NICE_TCMD5_ERRCODE)
		{
			if (!(driveerror & D_ERRORSTATE))
				{
					RecordErrorStatus();		//记录变频器故障时的系统状态
					set_contactor (0, 0);
					driveerror |= D_ERRORSTATE;
					ResetNiceFaultChk();
					drive_state = D_STOPPED;
				}
		}
	switch (drive_state)
		{
			case (D_START_MOVING):
				if (drive_state_old != drive_state)
					{
						if ((contactor_state & contactor_check & (D_RUNCONT_SUP | D_BRKCONT_SUP)) 
								|| (brake_state & brake_check)
								|| (nice5000_Rcmd_data1 & (NICE5000_RCMD1_RUN | NICE5000_RCMD1_BRAKE)))
							{
								if (contactor_state & D_RUNCONT_SUP)
									{
										driveerror |= D_RUNCONT_NOTOFF;
										write_errorhist (E_RUNCONTACT_NOTOFF, HSE_ID, C_RUNCONTACT_NOTOFF, W_RUNCONTACT_NOTOFF);
									}
								if (contactor_state & D_BRKCONT_SUP)
									{
										driveerror |= D_BRKCONT_NOTOFF;
										write_errorhist (E_BRKCONTACT_NOTOFF, HSE_ID, C_BRKCONTACT_NOTOFF, W_BRKCONTACT_NOTOFF);
									}
								if (brake_state)
									{
										driveerror |= D_BRAKE_NOTOFF;
										write_errorhist (E_BRAKE_NOTOFF, HSE_ID, C_BRAKE_NOTOFF, W_BRAKE_NOTOFF);
									}
								if (nice5000_Rcmd_data1 & NICE5000_RCMD1_RUN)
									{
										driveerror |= D_NICE_RUN_NOTOFF;
										write_errorhist (E_NICE_RUN_NOTOFF, DRV_ID, C_NICE_RUN_NOTOFF, W_NICE_RUN_NOTOFF);
									}
								if (nice5000_Rcmd_data1 & NICE5000_RCMD1_BRAKE)
									{
										driveerror |= D_NICE_BRAKE_NOTOFF;
										write_errorhist (E_NICE_BRAKE_NOTOFF, DRV_ID, C_NICE_BRAKE_NOTOFF, W_NICE_BRAKE_NOTOFF);
									}
								drive_state = D_STOPPED;
							}
						else if (fxtimer < timer)
							{
								drive_state_old = drive_state;
								set_contactor (D_MAIN, 1);
								if (p.fx_active)
									{
										dt = timer + (p.fx_ontime / 100);
										ct = timer + (p.wait_for_contactor / 100) + (p.fx_ontime / 100);
									}
								else
									{
										dt = timer;
										ct = timer + (p.wait_for_contactor / 100);
									}
								wait = 1;
							}
					}
				else if (wait == 1)
					{
						if (nice5000_Tcmd_data1 & NICE5000_TCMD1_SPECIALDEC)
							{// 还未给运行命令
								set_contactor (0, 0);
								drive_state = D_STOPPED;	
							}
						else if ((contactor_state & D_RUNCONT_SUP) || 
								((!(contactor_check & D_RUNCONT_SUP)) && (dt < timer)))
							{
								wait = 2;
								dt = timer + 4;
							}
						else if (ct < timer)
							{
								driveerror |= D_RUNCONT_NOTON;
								write_errorhist (E_RUNCONTACT_NOTON, HSE_ID, C_RUNCONTACT_NOTON, W_RUNCONTACT_NOTON);
								set_contactor (0, 0);
								drive_state = D_STOPPED;
							}
					}
				else if (wait == 2)
					{//运行接触器反馈正常
						if (nice5000_Tcmd_data1 & NICE5000_TCMD1_SPECIALDEC)
							{// 还未给运行命令
								set_contactor (0, 0);
								drive_state = D_STOPPED;		
							}
						else if (dt < timer)
							{
								if (safety_circuit & SC3)
									{									
										if(p.ups_direction_mode && ups_power)
											{//ups 救援状态
												wait = 20;
												nice5000_Tcmd_data1 |= NICE5000_TCMD1_RUN;
												dt = timer + 10 SEC;
											}
										else
											{
												cmd = nice5000_Tcmd_data1 | NICE5000_TCMD1_RUN;
												if (direction == DIR_UP)
													cmd |= NICE5000_TCMD1_UP;
												else if (direction == DIR_DN)
													cmd |= NICE5000_TCMD1_DOWN;
												nice5000_Tcmd_data1 = cmd;
												wt = timer + (p.max_ready_on / 100);
												wait = 3;
											}
									}
								else
									{
										if ((!(insp & (INSP_TOPCAR_ACTIVE | INSP_PITBOX_ACTIVE))) && (!(em_op & INSP_ON)))
											{
												driveerror |= D_NO_SC;
												write_errorhist (E_SC_VOLTAGE_MOVE, HSE_ID, 0, 0);
												sc_limit_fg = 1;
											}
										set_contactor (0, 0);
										drive_state = D_STOPPED;
									}
							}
					}				
				else if((wait == 20) && ups_power)
					{//ups 救援使用状态					
						if (nice5000_Tcmd_data1 & NICE5000_TCMD1_SPECIALDEC)
							{// 还未给运行命令
								set_contactor (0, 0);
								drive_state = D_STOPPED;	
							}
						else if (dt < timer)
							{//等待10s仍未收到方向信号							
								driveerror |= D_NICE_DIR_NOTON;
								write_errorhist (E_NICE_DIR_NOTON, DRV_ID, 0, W_NICE_RUN_NOTOFF);
								set_contactor (0, 0);
								drive_state = D_STOPPED;
							}
						else if(nice5000_Rcmd_data1 & (NICE5000_RCMD1_UP | NICE5000_RCMD1_DOWN))
							{
								if (safety_circuit & SC3)
									{
										cmd = nice5000_Tcmd_data1 | NICE5000_TCMD1_RUN;
										if (nice5000_Rcmd_data1 & NICE5000_RCMD1_UP)
											direction = DIR_UP;
										else if (nice5000_Rcmd_data1 & NICE5000_RCMD1_DOWN)
											direction = DIR_DN;
										direction = get_target_floor_ups(direction);
										if(direction == DIR_UP)											
											cmd |= NICE5000_TCMD1_UP;
										else if(direction == DIR_DN)											
											cmd |= NICE5000_TCMD1_DOWN;
										
										target_floor %= (p.top_floor + 1);											
										nice5000_Tcmd_data2 |= (target_floor + 1);
										nice5000_Tcmd_data1 = cmd;
										wt = timer + (p.max_ready_on / 100);
										wait = 3;
									}
								else
									{
										if ((!(insp & (INSP_TOPCAR_ACTIVE | INSP_PITBOX_ACTIVE))) && (!(em_op & INSP_ON)))
											{
												driveerror |= D_NO_SC;
												write_errorhist (E_SC_VOLTAGE_MOVE, HSE_ID, 0, 0);												
												sc_limit_fg = 1;
											}
										set_contactor (0, 0);
										drive_state = D_STOPPED;
									}						
							}
					}
				else if (wait == 3)
					{
						if (!(safety_circuit & SC3))
						{
							if ((!(insp & (INSP_TOPCAR_ACTIVE | INSP_PITBOX_ACTIVE))) && (!(em_op & INSP_ON)))
								{
									driveerror |= D_NO_SC;
									write_errorhist (E_SC_VOLTAGE_MOVE, HSE_ID, 0, 0);
									sc_limit_fg = 1;
								}
							set_contactor (0, 0);
							drive_state = D_STOPPED;
						}
						else if (nice5000_Tcmd_data1 & NICE5000_TCMD1_SPECIALDEC)
							{
								set_contactor (0, 0);
								drive_state = D_STOPPED;	
							}
						else if (nice5000_Rcmd_data1 & NICE5000_RCMD1_RUN)
							{
								bt = timer + (p.max_brake_on / 100);
								dt = timer + (p.brake_on_delay / 100);
								if (dt >= bt)
									bt = dt + 2;
								drive_state = D_READY_TO_SWITCH_ON;
							}
						else if (wt < timer)
							{
								driveerror |= (D_NICE_RUN_NOTON);
								write_errorhist (E_NICE_RUN_NOTON, DRV_ID, C_NICE_RUN_NOTON, W_NICE_RUN_NOTON);
								set_contactor (0, 0);
								drive_state = D_STOPPED;
							}
					}
				break;

			case (D_READY_TO_SWITCH_ON):
				drive_state_old = drive_state;				
				nice5000_Tcmd_data1 |= NICE5000_TCMD1_RUN;
				if ((bt <= timer) && ((nice5000_Rcmd_data2 & NICE5000_RCMD2_STATE) != RCMD2_STATE_MOTOR_TURN))
					{
						driveerror |= (D_NICE_BRAKE_NOTON);
						write_errorhist (E_NICE_BRAKE_NOTON, DRV_ID, C_NICE_BRAKE_NOTON, W_NICE_BRAKE_NOTON);
						drive_state = D_STOPPED;
					}
				else
					{
						if (nice5000_Tcmd_data1 & NICE5000_TCMD1_SPECIALDEC)
							{
								set_contactor (0, 0);
								drive_state = D_STOPPED;	
							}
						else if ((nice5000_Rcmd_data1 & NICE5000_RCMD1_BRAKE) && (dt <= timer))
							{
								set_contactor (D_BRAKE, 1);
								drive_state = D_OPERATION_ENABLED;
								canbus_check_timer = timer + 2 SEC;
								brake_feedback_timer = p.brake_feedback_dly;
								mt = timer + (p.max_start / 100);
								bt = timer + (p.wait_for_brake / 100);
								slowing = 0;
							}
						if (!(nice5000_Rcmd_data1 & NICE5000_RCMD1_RUN))
							{
								if ((tiv.F1_25 == NICE5000_P_F1_25_IM) && ((nice5000_Rcmd_data2 & NICE5000_RCMD2_STATE) == RCMD2_STATE_MOTOR_TURN))
									{
										nice5000_Tcmd_data1 &= ~(NICE5000_TCMD1_RUN | NICE5000_TCMD1_DOWN | NICE5000_TCMD1_UP | NICE5000_TCMD1_BRAKE);
										dt = timer + (p.ready_off_delay / 100);
										wt = timer + (p.max_ready_off / 100);
										if (wt <= dt)
											wt = dt + 2;
										drive_state = D_WAIT_STOP;
									}
								else
									{
										driveerror |= (D_NICE_RUN_NOTON);
										write_errorhist (E_NICE_RUN_NOTON, DRV_ID, C_NICE_RUN_NOTON, W_NICE_RUN_NOTON);
										set_contactor (0, 0);
										drive_state = D_STOPPED;
									}
							}
					}
				if (!(safety_circuit & SC3))
					{
						if ((!(insp & (INSP_TOPCAR_ACTIVE | INSP_PITBOX_ACTIVE))) && (!(em_op & INSP_ON)))
							{
								driveerror |= D_NO_SC;
								write_errorhist (E_SC_VOLTAGE_MOVE, HSE_ID, 0, 0);
								sc_limit_fg = 1;
							}
						set_contactor (0, 0);
						drive_state = D_STOPPED;
					}
				break;

			case (D_OPERATION_ENABLED):
			case (D_SLOW_DOWN):
				drive_state_old = drive_state;
				nice5000_Tcmd_data1 |= NICE5000_TCMD1_RUN;
				if ((contactor_state & D_RUNCONT_SUP) || (!(contactor_check & D_RUNCONT_SUP)))
					{
						ct = timer + 1 SEC;
					}
				if ((brake_state == brake_check) &&
						((contactor_state & D_BRKCONT_SUP) || (!(contactor_check & D_BRKCONT_SUP))))
					{
						bt = timer + (p.wait_for_brake / 100);
						if (!brake_feedback_timer)
							nice5000_Tcmd_data1 |= NICE5000_TCMD1_BRAKE;
					}
				if (slowing)						
					{	// 已经开始减速
						if ((nice5000_Rcmd_data1 & NICE5000_RCMD1_ZEROSPD) || (nice5000_running_step == RCMD2_STEP_0SPD_STOPPING))
							drive_state = D_STOPPING;
						mt = timer + 4;
					}
				else
				{
					if (!(nice5000_Rcmd_data1 & NICE5000_RCMD1_ZEROSPD))
						{
							if ((mt - timer <= 4) || (mt <= timer))
								mt = timer + 4;
						}
					else if (brake_state != brake_check)
						mt = timer + 3 SEC;

					switch (nice5000_running_step)
						{
							case RCMD2_STEP_DEC_STARTING:
							case RCMD2_STEP_DEC_DOING:
							case RCMD2_STEP_DEC_ENDING:
								drive_state = D_SLOW_DOWN;
								slowing = 1;
								break;
							case RCMD2_STEP_0SPD_STOPPING:
								mt = timer;
								drive_state = D_STOPPING;
								break;
						}
				}
				if ((ct < timer) 
						|| (bt < timer) 
						|| (mt < timer)
						|| (!(nice5000_Rcmd_data1 & NICE5000_RCMD1_RUN)))
					{
						if (ct < timer)
							{//接触器未闭合
								driveerror |= D_RUNCONT_NOTON;
								write_errorhist (E_RUNCONTACT_NOTON, HSE_ID, C_RUNCONTACT_NOTON, W_RUNCONTACT_NOTON);
							}
						if (bt < timer)
							{//抱闸未打开
								if (brake_state != brake_check)
									{
										driveerror |= D_BRAKE_NOTON;
										write_errorhist (E_BRAKE_NOTON, HSE_ID, C_BRAKE_NOTON, W_BRAKE_NOTON);
									}
								if (!(contactor_state & D_BRKCONT_SUP))
									{
										driveerror |= D_BRKCONT_NOTON;
										write_errorhist (E_BRKCONTACT_NOTON, HSE_ID, C_BRKCONTACT_NOTON, W_BRKCONTACT_NOTON);
									}
							}
						if (mt < timer)
							{//变频器未撤掉零速信号
								driveerror |= D_NICE_MOVE_NOTON;
								write_errorhist (E_NICE_MOVE_NOTON, HSE_ID, C_NICE_MOVE_NOTON, W_NICE_MOVE_NOTON);
							}
						if (!(nice5000_Rcmd_data1 & NICE5000_RCMD1_RUN))
							{
								driveerror |= (D_NICE_RUN_NOTON);
								write_errorhist (E_NICE_RUN_NOTON, DRV_ID, C_NICE_RUN_NOTON, W_NICE_RUN_NOTON);
							}
						if (!(nice5000_Rcmd_data1 & NICE5000_RCMD1_BRAKE))
							{
								driveerror |= (D_NICE_BRAKE_NOTON);
								write_errorhist (E_NICE_BRAKE_NOTON, DRV_ID, C_NICE_BRAKE_NOTON, W_NICE_BRAKE_NOTON);
							}
						set_contactor (0, 0);
						drive_state = D_STOPPED;
					}

				if (!(safety_circuit & SC3))							/* safety circuit is open					*/
					{
						if ((!(insp & (INSP_TOPCAR_ACTIVE | INSP_PITBOX_ACTIVE))) && (!(em_op & INSP_ON)))
							{
								driveerror |= D_NO_SC;
								write_errorhist (E_SC_VOLTAGE_MOVE, HSE_ID, 0, 0);
								sc_limit_fg = 1;
							}
						set_contactor (0, 0);
						drive_state = D_STOPPED;
					}
				break;

			case (D_STOPPING):					
				nice5000_Tcmd_data1 |= NICE5000_TCMD1_RUN;
				if (drive_state != drive_state_old)
					{
						drive_state_old = drive_state;
						dt = timer + (p.max_stop / 100);
					}
				if (nice5000_Rcmd_data1 & NICE5000_RCMD1_ZEROSPD)
					{
						drive_state = D_OPERATION_DISABLED;
						if ((nice5000_Rcmd_data2 & NICE5000_RCMD2_STATE) != RCMD2_STATE_MOTOR_TURN)
							dt = timer + (p.brake_off_delay / 100);
						else
							dt = timer + 2;
						wt = timer + (p.max_brake_off / 100);
						if (wt <= dt)
							wt = dt + 2;
					}
				else if (!(nice5000_Rcmd_data1 & NICE5000_RCMD1_RUN))
					{
						driveerror |= (D_NICE_RUN_NOTON);
						write_errorhist (E_NICE_RUN_NOTON, DRV_ID, C_NICE_RUN_NOTON, W_NICE_RUN_NOTON);
						set_contactor (0, 0);
						drive_state = D_STOPPED;
					}
				else if (dt < timer) 										/* time over for speed signal = 0			*/
					{
						driveerror |= (D_NICE_MOVE_NOTOFF);							/* set drive error state					*/
						write_errorhist (E_NICE_MOVE_NOTOFF, DRV_ID, C_NICE_MOVE_NOTOFF, W_NICE_MOVE_NOTOFF);
						set_contactor (0, 0);
						drive_state = D_STOPPED;							/* go to next state 					*/
					}
				if (!(safety_circuit & SC3))							/* safety circuit is open					*/
					{
						if ((!(insp & (INSP_TOPCAR_ACTIVE | INSP_PITBOX_ACTIVE))) && (!(em_op & INSP_ON)))
							{
								driveerror |= D_NO_SC;
								write_errorhist (E_SC_VOLTAGE_MOVE, HSE_ID, 0, 0);								
								sc_limit_fg = 1;
							}
						set_contactor (0, 0);
						drive_state = D_STOPPED;
					}
				break;

			case (D_OPERATION_DISABLED):
				drive_state_old = drive_state;									/* save old drive state 					*/
				nice5000_Tcmd_data1 |= NICE5000_TCMD1_RUN;
				if ((dt < timer) && (!(nice5000_Rcmd_data1 & NICE5000_RCMD1_BRAKE)))
					{														/* drive state switched off (brake off) 	*/
						set_contactor (D_BRAKE, 0); 							/* switch off brake contactor				*/
						bt = timer + (p.wait_for_brake / 100);
						mt = timer + (p.brake_off_sig_dly / 100);
						if (bt <= mt)
							bt = mt + 2;
						wait = 0;
						drive_state = D_SWITCHED_OFF;
					}
				else if (!(nice5000_Rcmd_data1 & NICE5000_RCMD1_RUN))
					{
						driveerror |= (D_NICE_RUN_NOTON);
						write_errorhist (E_NICE_RUN_NOTON, DRV_ID, C_NICE_RUN_NOTON, W_NICE_RUN_NOTON);
						set_contactor (0, 0);
						drive_state = D_STOPPED;
					}
				else if (wt < timer)
					{
						if (!(driveerror & D_NICE_BRAKE_NOTOFF))
							{
								driveerror |= D_NICE_BRAKE_NOTOFF;
								write_errorhist (E_NICE_BRAKE_NOTOFF, DRV_ID, C_NICE_BRAKE_NOTOFF, W_NICE_BRAKE_NOTOFF);
							}
						set_contactor (D_BRAKE, 0);	
						bt = timer + (p.wait_for_brake / 100);
						mt = timer + (p.brake_off_sig_dly / 100);
						if (bt <= mt)
							bt = mt + 2;
						wait = 0;
						drive_state = D_SWITCHED_OFF;
					}
				if (!(safety_circuit & SC3))							/* safety circuit is open					*/
					{
						if ((!(insp & (INSP_TOPCAR_ACTIVE | INSP_PITBOX_ACTIVE))) && (!(em_op & INSP_ON)))
							{
								driveerror |= D_NO_SC;
								write_errorhist (E_SC_VOLTAGE_MOVE, HSE_ID, 0, 0);
								sc_limit_fg = 1;
							}
						set_contactor (0, 0);
						drive_state = D_STOPPED;
					}
				break;

			case (D_SWITCHED_OFF):
				drive_state_old = drive_state;
				if (!wait)
					{
						if ((mt < timer) &&
								((!brake_state) || (!brake_check)) &&
								((!(contactor_state & D_BRKCONT_SUP)) || (!(contactor_check & D_BRKCONT_SUP))))
							{
								nice5000_Tcmd_data1 &= ~NICE5000_TCMD1_BRAKE;		//抱闸关闭完成
								dt = timer + (p.run_off_delay / 100);
								wait = 1;
							}
						else if (bt < timer)
							{
								if (brake_check & brake_state)
									{
										driveerror |= D_BRAKE_NOTOFF;				// set brake error
										write_errorhist (E_BRAKE_NOTOFF, HSE_ID, C_BRAKE_NOTOFF, W_BRAKE_NOTOFF);
									}
								if (contactor_state & D_BRKCONT_SUP)
									{
										driveerror |= D_BRKCONT_NOTOFF;
										write_errorhist (E_BRKCONTACT_NOTOFF, HSE_ID, C_BRKCONTACT_NOTOFF, W_BRKCONTACT_NOTOFF);
									}
								set_contactor (0, 0);
								drive_state = D_STOPPED;
							}
					}
				else
					{
						if (dt < timer)
							{
								nice5000_Tcmd_data1 &= ~(NICE5000_TCMD1_DOWN | NICE5000_TCMD1_UP | NICE5000_TCMD1_BRAKE); 
								direction = 0;
								dt = timer + (p.ready_off_delay / 100);
								wt = timer + (p.max_ready_off / 100);
								if (wt <= dt)
									wt = dt + 2;
								drive_state = D_WAIT_STOP;
							}
					}
				if (!(safety_circuit & SC3))							// safety circuit is open
					{
						if ((!(insp & (INSP_TOPCAR_ACTIVE | INSP_PITBOX_ACTIVE))) && (!(em_op & INSP_ON)))
							{
								driveerror |= D_NO_SC;
								write_errorhist (E_SC_VOLTAGE_MOVE, HSE_ID, 0, 0);
								sc_limit_fg = 1;
							}
						set_contactor (0, 0);
						drive_state = D_STOPPED;
					}
				break;

			case (D_WAIT_STOP):
				if (drive_state != drive_state_old) 							/* drive state changed						*/
					{
						if (p.fx_active)
							ct = timer + (p.wait_for_contactor / 100) + (p.fx_offtime / 100);
						else
							ct = timer + (p.wait_for_contactor / 100);
						if ((dt <= timer) && (!(nice5000_Rcmd_data1 & NICE5000_RCMD1_RUN)))
							{//撤运行信号以及关闭运行接触器								
								nice5000_Tcmd_data1 &= ~(NICE5000_TCMD1_RUN | NICE5000_TCMD1_DOWN | NICE5000_TCMD1_UP
																			| NICE5000_TCMD1_BRAKE | NICE5000_TCMD1_SPECIALDEC);
								set_contactor (0, 0);								
								canbus_check_timer = timer + 2 SEC;
								drive_state_old = drive_state;							/* save old drive state 					*/
							}
						else if (wt < timer)
							{
								driveerror |= D_NICE_RUN_NOTOFF;				// set brake error
								write_errorhist (E_NICE_RUN_NOTOFF, HSE_ID, C_NICE_RUN_NOTOFF, W_NICE_RUN_NOTOFF);
								set_contactor (0, 0);
								drive_state = D_STOPPED;
							}
					}
				else
					{
						if ((contactor_check && contactor_state & D_RUNCONT_SUP)
								&& (ct < timer))
							{
								if (!(driveerror & D_RUNCONT_NOTOFF))
									{
										driveerror |= D_RUNCONT_NOTOFF;
										write_errorhist (E_RUNCONTACT_NOTOFF, HSE_ID, C_RUNCONTACT_NOTOFF, W_RUNCONTACT_NOTOFF);
									}
							}
						if (p.fx_active)
							{
								if ((contactor_check && contactor_state & D_FXCONT_SUP)
										&& (ct < timer))
									{
										if (!(driveerror & D_FXCONT_NOTOFF))
											{
												driveerror |= D_FXCONT_NOTOFF;
												write_errorhist (E_FXCONTACT_NOTOFF, HSE_ID, C_FENGXING_NOTOFF, W_FENGXING_NOTOFF);
											}
									}
							}
						if (p.fx_active)
							{
								if ((((!(contactor_check & D_RUNCONT_SUP)) || 
										(!(contactor_state & D_RUNCONT_SUP))) &&
										((!(contactor_check & D_FXCONT_SUP)) || 
										(!(contactor_state & D_FXCONT_SUP)))) ||
										(ct < timer))
									drive_state = D_STOPPED;
							}
						else
							{
								if (((!(contactor_check & D_RUNCONT_SUP)) || 
										(!(contactor_state & D_RUNCONT_SUP))) ||
										(ct < timer))
									drive_state = D_STOPPED;
							}
					}
				break;

			case (D_STOPPED):
				if(bTest_brake && brake_check_forced)
					{
						if(step == 0)
							{//发送测试命令给变频器
								nice5000_Tcmd_data6 = NICE5000_BRAKE_CHECK_COMMAND;
								wait_timer = timer + 5 SEC;
								step = 1;
							}
						else if(step == 1)
							{
								if((nice5000_Rcmd_data6 & NICE5000_BRAKE_CHECK_COMMAND) == NICE5000_BRAKE_CHECK_COMMAND)
									{//收到变频器的检测状态字
										step = 2;
										wait_timer = timer + 3;		//等待300ms给出运行信号
										ct = timer + 5 SEC;
									}
								else if((wait_timer < timer) || (!(safety_circuit & SC3)) || (driveerror))
									{//超时未收到回复，则退出检测模式
										step = 10;
										brake_check_noactive = 1;
									}
							}
						else if(step == 2)
							{
								if((fxtimer < timer) && (wait_timer < timer))
									{
										set_contactor (D_MAIN, 1);
										wait_timer = timer + 20 SEC;
										ct = timer + 20;
										step = 3;
									}
								else if((ct < timer) || (!(safety_circuit & SC3)) || (driveerror))
									{//超时未收到回复，则退出检测模式
										step = 10;
										brake_check_noactive = 1;
									}
							}
						else if(step == 3)
							{
								if ((safety_circuit & SC3) && (ct < timer))
									nice5000_Tcmd_data1 |= NICE5000_TCMD1_RUN;
								if((nice5000_Rcmd_data6 & NICE5000_BRAKE_CHECK_FINISH) == NICE5000_BRAKE_CHECK_FINISH)
									{//收到变频器的结束命令
										wait_timer = timer + 5 SEC;
										step = 4;
									}
								else if((wait_timer < timer) || (!(safety_circuit & SC3)) || (driveerror))
									{//超时未收到回复，则退出检测模式
										step = 10;
										brake_check_noactive = 1;
									}
							}
						else if(step == 4)
							{
								if(wait_timer < timer)
									{//结束测试
										brake_check_return = 1;
										brake_check_noactive = 0;
										brake_value = brake_check_data;
										step = 10;
										nice5000_Tcmd_data6 = 0;
										bTest_brake = 0;
										brake_check_forced = 0;
										nice5000_Tcmd_data1 = 0;
										set_contactor (0, 0); 								/* switch off all contactors					*/
									}
							}
						else if(step == 10)
							{								
								nice5000_Tcmd_data6 = 0;
								bTest_brake = 0;
								brake_check_forced = 0;
								nice5000_Tcmd_data1 = 0;
								set_contactor (0, 0); 								/* switch off all contactors					*/
								step = 0;
							}
					}
				else
					{
						if (drive_state != drive_state_old) 							/* drive state changed						*/
							{
								nice5000_Tcmd_data1 &= ~(NICE5000_TCMD1_RUN | NICE5000_TCMD1_DOWN | NICE5000_TCMD1_UP
																			| NICE5000_TCMD1_BRAKE | NICE5000_TCMD1_SPECIALDEC);
								direction = 0;											/* reset direction							*/
								relevelflag = 0;
								drive_state_old = drive_state;
								set_contactor (0, 0);									/* switch off all contactors					*/
							}
						nice5000_Tcmd_data1 = 0;

						direction = 0;											/* reset direction							*/
						relevelflag = 0;
						nice5000_Tcmd_data6 = 0;	//不在测试状态，命令字务必清除
					}
				break;
		}
}

void send_nice5000_parameter(WORD address, WORD value){
	BYTE i;
	for (i=0; c_nice5000_write_para[i].ptr != NULL; ++i)
		{
			if (address == c_nice5000_write_para[i].id)
				{
					nice5000_Tstate_id1 = address;
					nice5000_Tstate_data1 = value;
					nice5000_Tstate_id2 = 0;
					nice5000_Tstate_data2 = 0;
					nice5000_Tstate_send = FRM_FCT_SENDTIMES;
					return;
				}
		}
	if(niv_addr_match(address))
		{
			nice5000_Tstore_id1 = address;
			nice5000_Tstore_data1 = value;
			nice5000_Tstore_id2 = 0;
			nice5000_Tstore_data2 = 0;
			nice5000_Tstore_send = FRM_FCT_SENDTIMES;
		}
}

WORD GetAddrByReadPara(WORD * ptr, WORD * addr)
{
	WORD i;

	for (i=0; c_nice5000_read_para[i].ptr != NULL; ++i)
	{
		if (ptr == c_nice5000_read_para[i].ptr)
		{
			*addr = c_nice5000_read_para[i].id;
			return i;
		}
	}
	return 0xffff;
}
WORD * GetTivPtrByAddr(WORD addr)
{
	WORD i;

	for (i=0; c_nice5000_tiv_para[i].ptr != NULL; ++i)
	{
		if (addr == c_nice5000_tiv_para[i].id)
		{
			return c_nice5000_tiv_para[i].ptr;
		}
	}
	return NULL;
}

BYTE niv_addr_match(WORD addr){
	WORD i;

	for (i=0; c_nice5000_tiv_para[i].ptr != NULL; ++i)
		{
			if (addr == c_nice5000_tiv_para[i].id)
				return 1;
		}
	return 0;	
}

