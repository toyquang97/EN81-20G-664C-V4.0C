
#include "typedef.h"
#include "stdio.h"
#include "string.h"
#include "math.h"
#include "parameter.h"
#include "state.h"
#include "call.h"
#include "text.h"
#include "version.h"
#include "eeprom.h"
#include "pos.h"
#include "io.h"
#include "history.h"
#include "eeprom.h"
#include "lcd.h"
#include "can.h"
#include "rtc.h"
#include "hwtest.h"
#include "test.h"
#include "load.h"
#include "temperature.h"
#include "Nice5000.h"

extern	BYTE cana_rx_err, cana_tx_err, error_bit, error_code;


const WORD temper_C[] =
{
	2933, 2894, 2854, 2814, 2773, 2732, 2690, 2649,
	2607, 2564, 2522, 2479, 2436, 2393, 2350, 2307,
	2265, 2221, 2178, 2136, 2094, 2051, 2009, 1968,
	1926, 1885, 1844, 1804, 1764, 1724, 1685, 1647,
	1609, 1572, 1535, 1499, 1463, 1427, 1393, 1359,
	1325, 1293, 1260, 1229, 1198, 1168, 1138, 1109,
	1081, 1053, 1026, 999, 973, 948, 923, 899,
	876, 853, 830, 809, 787, 766, 746, 727,
	707, 689, 671, 653,
};
const char * const temper_str[] =
{
	"-11", "-10", "-9", "-8", "-7", "-6", "-5", "-4",
	"-3", "-2", "-1", "0", "1", "2", "3", "4",
	"5", "6", "7", "8", "9", "10", "11", "12",
	"13", "14", "15", "16", "17", "18", "19", "20",
	"21", "22", "23", "24", "25", "26", "27", "28",
	"29", "30", "31", "32", "33", "34", "35", "36",
	"37", "38", "39", "40", "41", "42", "43", "44",
	"45", "46", "47", "48", "49", "50", "51", "52",
	"53", "54", "55", "56", "++",
};

const struct _nice5000_loadvar c_nice5000_reload_para [] =
{
	{	&biv.F0_00,		1,		1,		},
	{	&biv.F0_01,		2,		1,		},
	{	&biv.F0_02,		3,		50,		},
	{	&biv.F0_04,		5,		1600,	},	
	{	&biv.F0_03,		4,		1600,	},
	{	&biv.F0_05,		6,		0,		},
	{	&biv.F0_07,		7,		80,		},
     	                    			
	{	&biv.F1_12,		20,		0,		},
	{	&biv.F1_13,		21,		60,		},
     	                        		
	{	&biv.F2_00,		26,		40,		},
	{	&biv.F2_01,		27,		60,		},
	{	&biv.F2_02,		29,		200		},
	{	&biv.F2_03,		30,		35,		},
	{	&biv.F2_04,		28,		80,		},
	{	&biv.F2_05,		32,		500,	},
	{	&biv.F2_06,		33,		60,		},
	{	&biv.F2_07,		34,		30,		},
	{	&biv.F2_08,		35,		1500,	},
	{	&biv.F2_09,		44,		60,		},
	{	&biv.F2_10,		45,		30,		},
	{	&biv.F2_11,		36,		0,		},
	{	&biv.F2_12,		37,		35,		},
	{	&biv.F2_13,		38,		80,		},
	{	&biv.F2_14,		39,		60,		},
	{	&biv.F2_15,		40,		30,		},
	{	&biv.F2_16,		41,		60,		},
	{	&biv.F2_17,		42,		30,		},
	{	&biv.F2_18,		1060,	1,		},
	{	&biv.F2_19,		1061,	350,	},
	{	&biv.F2_20,		1784,	0,		},
	{	&biv.F2_22,		1605,	0,		},
     	                        		
	{	&biv.F3_00,		46,		0,		},
	{	&biv.F3_01,		47,		150,	},
	{	&biv.F3_02,		48,		600,	},
	{	&biv.F3_03,		49,		2500,	},
	{	&biv.F3_04,		50,		2500,	},
	{	&biv.F3_05,		51,		600,	},
	{	&biv.F3_06,		52,		2500,	},
	{	&biv.F3_07,		53,		2500,	},
	{	&biv.F3_08,		54,		900,	},
	{	&biv.F3_09,		55,		0,		},
	{	&biv.F3_10,		56,		40,		},
	{	&biv.F3_11,		57,		250,	},
	{	&biv.F3_12,		58,		100,	},
	{	&biv.F3_13,		59,		500,	},
	{	&biv.F3_14,		1609,	32,		},
	{	&biv.F3_15,		649,	80,		},
	{	&biv.F3_16,		648,	500,	},
     	                        		
	{	&biv.F4_00,		65,		30,		},
	{	&biv.F4_06,		71,		28,		},
	 	                        		
	{	&biv.FA_09,		325,	3,		},
	{	&biv.FA_10,		326,	100,	},
	{	&biv.FA_11,		327,	80,		},
	{	&biv.FA_15,		1777,	0,		},
     	                        		
	{	&biv.FH_03,		434,	21,		},
	{	&biv.FH_29,		458,	0,		},
     	                        		
};

//**************************************************************************************************
// Main menu function and key evaluation
//**************************************************************************************************
void menu (void)
{
	static WORD menu = M_STATUS1;
	static WORD menu_old = M_STATUS1;
	static BYTE new = 1;
	static WORD row = 0;
	static WORD key = 0;
	static BYTE keypointer;
	static BYTE keybuff [5];
	static BYTE accesstimer = 0;
	static DWORD displaytimer = 0;
	static WORD menusize;
	static BYTE date;
	WORD accessvalue;
	static BYTE passwdbuf[10];
	static BYTE buffer[30];
	static BYTE buf1[MAX_HSE_IN + MAX_HSE_EX_IN + 1];
	static BYTE buf2[MAX_HSE_OUT + MAX_HSE_EX_OUT + 1];
	static BYTE numberpos;
	static WORD paranumber;
	static WORD paratype;
	static BYTE wait = 0;
	static DWORD oldvalue, value;
	static WORD help [10];
	static BYTE ioparabuffer [10];
	static DWORD dwordvalue [8];
	static WORD wordvalue [8];
	static SWORD swordvalue[8];
	static WORD load_value;
	static float floatvalue;
	WORD i;
	WORD j;
	BYTE right;
	BYTE inverse;
	static BYTE sure = 0;
	static BYTE bus;
	static WORD pcb_number;
	static void * vptr = NULL;
	static WORD address = 0;

	if (menu == M_MOVE)
		key = get_key (1);
	else
		key = get_key (0);

	if (savepara && (!numberkey))						/* save all parameter					*/
	{
		if (!sure)
			menu_old = menu;
		sure = 1;
		menu = M_SAVEPARA;
		row = 0;
		new = 1;
		savepara = 0;
	}
	if (hht_menu)
	{
		if (hht_menu == M_TSE_INFO)
			pcb_number = hht_pcb_ciu-1;
		else if (hht_menu == M_ESE_INFO)
			pcb_number = hht_pcb_liop-1;
		else if (hht_menu == M_STATUS1)
			sdo_interrupt = 0;
		menu = hht_menu;
		new = 1;
		hht_menu = 0;
	}
	if (!numberkey)									/* direct access with xxx* enabled		*/
	{
		if ((key >= '0') && (key <= '9'))						/* number key pressed				*/
		{
			if (keypointer < 4)
			{
				keybuff [keypointer++] = key;				/* save key in buffer					*/
				accesstimer = 60;						/* key valid for at least 3 seconds		*/
			}
			else
			{
				keypointer = 0;							/* clear key buffer					*/
				accesstimer = 0;						/* reset access timer					*/
			}
			key = 0;									/* key not valid anymore				*/
		}
		if ((key == '#') && keypointer)					/* direct access to menu, parameter or command	*/
		{
			keybuff [keypointer] = 0;					/* complete string					*/
			sscanf ((const char *)keybuff, "%u", (DWORD *)&accessvalue);		/* read value for direct access					*/
			accessvalue += (keypointer * 10000);			/* number of digits (000* != 0*)!!!	*/
			value = 0xFFFF;
			if (passwordtimer)
			{
				switch (accessvalue)
				{
					case (10000):
						value = M_STATUS1;			/* String 0* status display				*/
						break;
					case (10001):
						if (password_right <= RIGHT_INSTALL)
							key_doorcommand = DR1_OPEN;	/* String 1* open door 1			*/
						break;
					case (10002):
						if (password_right <= RIGHT_INSTALL)
							key_doorcommand = DR2_OPEN;	/* String 2* open door 2			*/
						break;
					case (10003):
						if (password_right <= RIGHT_INSTALL)
							key_doorcommand = DR3_OPEN;	/* String 3* open door 3			*/
						break;
					case (10004):
						if (password_right <= RIGHT_INSTALL)
							key_doorcommand = DR_CLOSE;	/* String 4* close all doors		*/
						break;

					case (10005):
						if (password_right <= RIGHT_HELP)
						{
							if (bit_select(door_off,0))		/* String 5*							*/
								bit_reset (door_off, 0);	/* switch door movement on			*/
							else
								bit_set (door_off, 0);		/* switch door movement off			*/
						}
						break;

					case (10006):
						if (password_right <= RIGHT_INSTALL)
							landings_off ^= 0x01;
						break;						/* String 6* disable landing calls		*/

					case (10007):
						if (password_right <= RIGHT_ADMIN)
						{
							if (testtrip)					/* String 7*							*/
								testtrip = 0;				/* switch test trips off					*/
							else
							{
								testtrip = 1;				/* start test trips						*/
								testtimes = test_times;
							}
						}
						break;

					case (10008):
						if (password_right <= RIGHT_INSTALL)
							value = M_MOVE;				/* String 8*							*/
						break;
					case (20000):
						value = M_BASIC;				/* String 00*						*/
						break;
					case (20001):
						if (password_right <= RIGHT_INSTALL)
						{
							if (out_of_order_menu)			/* String 01*									*/
								out_of_order_menu = 0;	/* Switch out of order display off				*/
							else
								out_of_order_menu = 1;	/* Switch out of order display on				*/
						}
						break;
					case (30012):
						if (password_right <= RIGHT_INSTALL)
							value = M_SHOW_HIST;			/* String 012*						*/
						break;

					case (30015):
						if (password_right <= RIGHT_INSTALL)
						{
							if (menu < M_PARAINPUT)		/* not in parameter input mode			*/
							{
								value = M_CL_HIST;		/* String 015*						*/
								if (!sure)
									menu_old = menu;		/* save actual menu					*/
								sure = 1;
							}
						}
						break;

					case (30000):
						value = M_GEN_PARA;			/* String 000*						*/
						break;
					case (30001):
						value = M_TIMERPARA;			/* String 001*						*/
						break;
					case (30100):
						if (password_right <= RIGHT_INSTALL)
						{
							value = M_TEACH_IN;			/* String 100*						*/
							menu_old = search_mainmenu (M_TEACH_IN);
						}
						break;

					case (10009):
						if (password_right <= RIGHT_INSTALL)
						{
							if (menu < M_PARAINPUT)		/* not in parameter input mode			*/
							{
								value = M_SAVEPARA;		/* for MCU String 9*, for HSE 600*		*/
								if (!sure)
									menu_old = menu;		/* save actual menu					*/
								sure = 1;
							}
						}
						break;

					case (30017):
						if (password_right <= RIGHT_INSTALL)
						{
							if (menu < M_PARAINPUT)		/* not in parameter input mode			*/
							{
								value = M_CLR_HOURCOUNT;	/* String 017*					*/
								if (!sure)
									menu_old = menu;		/* clear operation time counter		*/
								sure = 1;
							}
						}
						break;

					case (30018):
						if (password_right <= RIGHT_INSTALL)
						{
							if (menu < M_PARAINPUT)		/* not in parameter input mode			*/
							{
								value = M_CLR_TRIPCOUNT;	/* String 018*						*/
								if (!sure)
									menu_old = menu;		/* cleartrip counter					*/
								sure = 1;
							}
						}
						break;

					case (30400):
						value = M_HSE_INFO;			/* String 400*						*/
						break;
					case (30401):
						value = M_FVE_INFO;			/* String 401*						*/
						break;
					case (30402):
						value = M_TSE;				/* String 402*						*/
						break;
					case (30403):
						value = M_ESE;				/* String 403*						*/
						break;
					case (30404):
						value = M_GROUP;				/* String 404*						*/
						break;

					case (30999):
						if (password_right <= RIGHT_HELP)
							{
								if(ucmp_test_finish)
									value = M_UCMP_TEST;
								else
									value = M_STATUS_INTERNAL;		/* String 999*						*/
							}
						break;

					case (30888): value = M_STATUS_LIMIT;
						new = 1;
						break;
				}
			}
			switch (accessvalue / 100)
			{
				case (301):							/* set car call						*/
					if (password_right <= RIGHT_INSTALL)
					{
						i = (accessvalue % 100) - 1;
						if ((i >= p.bot_floor) && (i <= p.top_floor) && (calls_enabled & CARCALL))
						{								/* valid call floor						*/
							calltab [i].calltype |= CARCALL;
							calltab [i].cc_door |= p.doorpos [i];
						}
					}
					break;

				case (302):							/* set landing call up direction			*/
					if (password_right <= RIGHT_INSTALL)
					{
						i = (accessvalue % 100) - 1;
						if ((i >= p.bot_floor) && (i <= p.top_floor) && (calls_enabled & LANDINGCALL_UP))
						{								/* valid call floor						*/
							calltab [i].calltype |= LANDINGCALL_UP;
							calltab [i].lcu_door |= p.doorpos [i];
						}
					}
					break;

				case (303):							/* set landing call down direction		*/
					if (password_right <= RIGHT_INSTALL)
					{
						i = (accessvalue % 100) - 1;
						if ((i >= p.bot_floor) && (i <= p.top_floor) && (calls_enabled & LANDINGCALL_DN))
						{								/* valid call floor						*/
							calltab [i].calltype |= LANDINGCALL_DN;
							calltab [i].lcd_door |= p.doorpos [i];
						}
					}
					break;

				case (305):							/* set emergency call					*/
					if (password_right <= RIGHT_INSTALL)
					{
						i = (accessvalue % 100) - 1;
						if ((i >= p.bot_floor) && (i <= p.top_floor) && (calls_enabled & EMERGENCY_CALL))
						{								/* valid call floor						*/
							calltab [i].calltype |= EMERGENCY_CALL;
							calltab [i].ec_door |= p.doorpos [i];
						}
					}
					break;

				case (306):							/* set advanced call					*/
					if (password_right <= RIGHT_INSTALL)
					{
						i = (accessvalue % 100) - 1;
						if ((i >= p.bot_floor) && (i <= p.top_floor) && (calls_enabled & ADVANCED_CALL))
						{								/* valid call floor						*/
							calltab [i].calltype |= ADVANCED_CALL;
							calltab [i].ac_door |= p.doorpos [i];
						}
					}
					break;

				case (307):							/* set special call						*/
					if (password_right <= RIGHT_INSTALL)
					{
						i = (accessvalue % 100) - 1;
						if ((i >= p.bot_floor) && (i <= p.top_floor) && (calls_enabled & SPECIAL_CALL))
						{								/* valid call floor						*/
							calltab [i].calltype |= SPECIAL_CALL;
							calltab [i].sc_door |= p.doorpos [i];
						}
					}
					break;

				case (308):							/* set output HSE						*/
					if (password_right <= RIGHT_INSTALL)
					{
						if (passwordtimer)
						{
							i = (accessvalue % 100);
							if (!i)							/* String 800*						*/
							{
							}
							else if (i <= MAX_HSE_OUT)		/* String 801* to 805*				*/
							{
								if (bit_select (out_polarity, i - 1))
									bit_reset (out, i - 1);
								else
									bit_set (out, i - 1);
							}
#if	SRU_OLD
							else if (i == 9)					/* String 809*						*/
							{
								SetSwitch(SWITCH_ULSI, SWITCH_ON);
							}
#endif							
						}
					}
					break;

				case (309):							/* reset output HSE					*/
					if (password_right <= RIGHT_INSTALL)
					{
						if (passwordtimer)
						{
							i = (accessvalue % 100);
							if (!i)							/* String 900*						*/
							{
							}
							else if (i <= MAX_HSE_OUT)		/* String 901* to 905*				*/
							{
								if (bit_select (out_polarity, i - 1))
									bit_set (out, i - 1);
								else
									bit_reset (out, i - 1);
							}
#if	SRU_OLD							
							else if (i == 9)					/* String 909*						*/
								SetSwitch(SWITCH_ULSI, SWITCH_OFF);
#endif							
						}
					}
					break;
			}
			switch (accessvalue / 1000)
			{
				case (41):							/* set car call						*/
					if (password_right <= RIGHT_INSTALL)
					{
						i = ((accessvalue / 10) % 100) - 1;		/* read floor number					*/
						j = (accessvalue % 10) - 1;			/* read door number					*/
						if ((i >= p.bot_floor) && (i <= p.top_floor) && (calls_enabled & CARCALL))
						{								/* valid call floor						*/
							if (p.doorpos [i] & (1 << j))			/* door exists for this floor			*/
							{
								calltab [i].calltype |= CARCALL;
								calltab [i].cc_door |= (1 << j);
							}
						}
					}
					break;

				case (42):							/* set landing call up direction			*/
					if (password_right <= RIGHT_INSTALL)
					{
						i = ((accessvalue / 10) % 100) - 1;
						j = (accessvalue % 10) - 1;			/* read door number					*/
						if ((i >= p.bot_floor) && (i <= p.top_floor) && (calls_enabled & LANDINGCALL_UP))
						{								/* valid call floor						*/
							if (p.doorpos [i] & (1 << j))			/* door exists for this floor			*/
							{
								calltab [i].calltype |= LANDINGCALL_UP;
								calltab [i].lcu_door |= (1 << j);
							}
						}
					}
					break;

				case (43):							/* set landing call down direction		*/
					if (password_right <= RIGHT_INSTALL)
					{
						i = ((accessvalue / 10) % 100) - 1;
						j = (accessvalue % 10) - 1;			/* read door number					*/
						if ((i >= p.bot_floor) && (i <= p.top_floor) && (calls_enabled & LANDINGCALL_DN))
						{								/* valid call floor						*/
							if (p.doorpos [i] & (1 << j))			/* door exists for this floor			*/
							{
								calltab [i].calltype |= LANDINGCALL_DN;
								calltab [i].lcd_door |= (1 << j);
							}
						}
					}
					break;
			}
			if (value != 0xFFFF)
			{
				menu = value;
				row = 0;
				new = 1;
			}
			if (key != KEY_UP)
				key = 0;								/* key not valid anymore				*/
			accesstimer = 0;							/* reset access timer					*/
		}
	}

	if (accesstimer)
		accesstimer--;
	else
	{
		keypointer = 0;									/* clear key buffer					*/
		accesstimer = 0;								/* reset access timer					*/
		save_double_door = 0;
	}

	if (!passwordtimer)
	{
		if (password_admin)
			password_right = RIGHT_ADMIN;//RIGHT_USER;
		else if(password_help)			
			password_right = RIGHT_HELP;
		else if(password_install)			
			password_right = RIGHT_INSTALL;
		else if(password_maintain)			
			password_right = RIGHT_MAINTAIN;
		else
		{
			password_right = RIGHT_ADMIN;
			passwordtimer = PASSWORDTIME;
		}
		switch (menu)
		{
			case (M_STATUS1):
			case (M_STATUS2):
			case (M_STATUS3):
			case (M_STATUS4):
			case (M_STATUS5):
			case (M_PASSWORD):
				break;

			default:
				numberkey = 0;						/* enable direct access with xxx*		*/
				menu = M_STATUS1;
				new = 1;
				break;
		}
	}
	switch (menu)
	{
		case (M_STATUS1):
			if (new)									/* show status window 1				*/
			{
				display_standardtext (T_STATUS1);
				new = 0;
				help [0] = ~hse_text;
				help [1] = ~safety_circuit;
				help [2] = 0xFF;
				help [3] = 0;
				help [4] = 0;
				help [6] = D_NORMAL;
				help [8] = ~level;
				dwordvalue [0] = 0;
				dwordvalue [1] = 0;
				dwordvalue [2] = 0xFFFFFFFF;
				buf1 [0] = 0;
				for (i = 0; i < p.doornumber; i++)
					display (5 + i, 2, "?");
				lcdram = 0;
				wait = 1;
			}
			if (key == '#')
			{
				new = 1;
				menu = M_PASSWORD;
				menu_old = menu;
				row = 0;
			}
			else if (key == '*')
			{
				new = 1;
				menu_old = menu;
				if (p.top_floor > 7)
					menu = M_STATUS2;
				else
					menu = M_STATUS3;
			}
			else if (key == KEY_UP)
			{
				new = 1;
				menu_old = menu;
				menu = M_STATUS7;
			}
			else if (key == KEY_ESC)
			{
				if (handheld_active)
					transmit_sign (KEY_ESC, 'X', 0, 0, handheld_active - 1);
				handheld_active = 0;
				handheld_bak = 0;
				handheld_key = 0;
			}
			j = 0;
			for (i = 0; i < MAX_ESE_DW; i++)
			{
				if (ese_enabled [i])
					j = 1;
			}
			if (!para_valid)
			{
				if (menu_toggle_sec % 2)
					help [7] = hse_text;
				else
					help [7] = HT_ERROR;
			}
			else
			{
				if (((errorwarning >= ERROR_WARNINGLIMIT)
					|| (triperrorcount >= MAX_TRIP_ERROR)
					|| ((esetimer < timer) && j)
					|| ((tsetimer < timer) && tse_enabled)
					|| ((exetimer < timer) && exe_enabled))
				&& p.setup_ready && p.motor_tuned)
				{
					if (menu_toggle_sec % 2)
						help [7] = hse_text;
					else
					{
						if (triperrorcount >= MAX_TRIP_ERROR)
							help [7] = HT_RESTART_INH;
						else if(triperrorcount)			
							help [7] = HT_WARNING;
						else
							help [7] = hse_text;							
					}
				}
				else
					help [7] = hse_text;
			}
			if (help [0] != help [7])
			{
				if (help [7])
				{
					sprintf ((char *)buffer, "%-10s", statetext [help [7] - 1].text [language]);
					display (0, 3, buffer);
				}
				help [0] = help [7];
			}
			if (help [1] != safety_circuit)
			{
				for (i = 0; i < 3; i++)
				{
					if (safety_circuit & (1 << i))
						buffer [i] = 0xFF;
					else
						buffer [i] = 8;
				}
				buffer [3] = 0;
				display (13, 3, buffer);
				help [1] = safety_circuit;
			}
			if (help [2] != door_zone)
			{
				help [2] = door_zone;
				if (help [2] & POS_SGM)
					buffer [0] = 0xFF;
				else
					buffer [0] = 8;

				if ((help [2] & (POS_SGD | POS_SGU)) == (POS_SGD | POS_SGU))
					buffer [1] = 0xff;
				else if (help [2] & POS_SGD)
					buffer [1] = 2;
				else if (help [2] & POS_SGU)
					buffer [1] = 1;
				else
					buffer [1] = 8;

				if ((help [2] & (POS_ILU | POS_ILD)) == (POS_ILU | POS_ILD))
					buffer [2] = 0xff;
				else if (help [2] & POS_ILD)
					buffer [2] = 2;
				else if (help [2] & POS_ILU)
					buffer [2] = 1;
				else
					buffer [2] = 8;

				if ((help[2] & (POS_SDU1 | POS_SDD1)) == (POS_SDU1 | POS_SDD1))
					buffer[5] = 0xff;
				else if (help[2] & POS_SDU1)
					buffer[5] = 1;
				else if (help[2] & POS_SDD1)
					buffer[5] = 2;
				else
					buffer[5] = 8;
			
				if ((help[2] & (POS_SDU2 | POS_SDD2)) == (POS_SDU2 | POS_SDD2))
					buffer[4] = 0xff;
				else if (help[2] & POS_SDU2)
					buffer[4] = 1;
				else if (help[2] & POS_SDD2)
					buffer[4] = 2;
				else
					buffer[4] = 8;
			
				if ((help[2] & (POS_SDU3 | POS_SDD3)) == (POS_SDU3 | POS_SDD3))
					buffer[3] = 0xff;
				else if (help[2] & POS_SDU3)
					buffer[3] = 1;
				else if (help[2] & POS_SDD3)
					buffer[3] = 2;
				else
					buffer[3] = 8;

				buffer [6] = 0;
				display (10, 2, buffer);
			}
			buffer [1] = 0;
			for (i = 0; i < p.doornumber; i++)
			{
				if (state_run & door_off)
					buffer [0] = '#';
				else if (bit_select (doorstate, i))
					buffer [0] = 0xFF;
				else
					buffer [0] = 8;
				if (buffer [0] != help [3 + i])
				{
					display (5 + i, 2, buffer);
					help [3 + i] = buffer [0];
				}
			}
			if (testtrip && state_run)
				dwordvalue [0] = 0xFFFFFFFF;
			else
				dwordvalue [0] = tripcount;
			if (dwordvalue [0] != dwordvalue [2])
			{
				dwordvalue [2] = dwordvalue [0];
				if (dwordvalue [0] == 0xFFFFFFFF)
				{
					i = 0;
					while ((standardtext [i].number != ENDSTRING) && (standardtext [i].number != T_TESTTRIP))
						i++;
					sprintf ((char *)buffer, "%8.8s", standardtext [i].text [language]);
				}
				else
					sprintf ((char *)buffer, "%8lu", dwordvalue [0]);
				display (8, 0, buffer);
			}
			i = D_NORMAL;
			if(firekey || auto_fire)
				i = D_FIRE;							/* display fireman mode				*/
			else if(firecall || firemode)
				i = D_FIRE_RECALL;
			else if (callpriority == C_FIREALARM)
				i = D_FIREALARM;						/* display emergency mode			*/
			else if (earthquake)						/* earthquake input on				*/
				i = D_EARTHQUAKE;
			else if ((callpriority == C_EMERGENCY) || (calltype == EMERGENCY_CALL))
				i = D_EMERGENCY;					/* display emergency mode			*/
			else if ((callpriority == C_ADVANCED) || (calltype == ADVANCED_CALL))
				i = D_ADVANCED;						/* display advanced mode				*/
			else if ((callpriority == C_SPECIAL) || (calltype == SPECIAL_CALL))
				i = D_SPECIAL;						/* display special mode				*/
			else if (car_switched_off)					/* car switched off					*/
				i = D_LIFT_OFF;						/* display lift off						*/
			else if (intermediate_door)					/* intermediate door is open			*/
				i = D_INTERM_DOOR;					/* display intermediate door open		*/
			else if (remote_off)							/* lift is remote switched off			*/
				i = D_REMOTE_OFF;					/* display remote off					*/
			else if (landings_off || landings_off_key || correct_level)
				i = D_LANDINGS_OFF;					/* display landings off				*/
			else if (fullload)							/* car is full							*/
				i = D_FULL_LOAD;						/* display full load					*/
			else if (auto_call)							/* autocall function ("Sabbat control")	*/
				i = D_AUTOCALL;
			else if (parkcall)
				i = D_PARKTRIP;						/* display parking trip					*/
			else if (timer2_trip)							/* timer trip 2						*/
				i = D_TIMER2;							/* display timer 2 trip					*/
			else if (timer1_trip || timer_trip)				/* timer trip 1						*/
				i = D_TIMER1;							/* display timer 1 trip					*/
			else if (car_attendance)
				i = D_ATTENDANCE;					/* display car attendance mode			*/
			else if (up_peak_time || up_peak_input)
				i = D_UP_PEAK;						/* display up peak traffic				*/
			else if (dn_peak_time || dn_peak_input)
				i = D_DN_PEAK;						/* display down peak traffic			*/
			else if (call_bypass || hallcall_bypass)
				i = D_BYPASS;
			else if(callpriority == C_EVACUATION)				
				i = D_EVACTION;
			else if(ucmp_state)
				i = D_UCMPLOCK;
			else if(emp_power)
				i = D_EMPPOWER;
			else if (door_zone & POS_ILU)
				i = D_ILO;
			else if (door_zone & POS_ILD)
				i = D_ILU;
			else if(temperature & (OVERTEMP1 | OVERTEMP2 | ROOMTEMP_MIN | ROOMTEMP_MAX))
				i = D_OVERTEMP;
			else if(p.forced_stop)
				i = D_FORCE_STOP;
			else if(door_short_conn)
				i = D_DOOR_CONNECT;
			else if(doorstopstate || she_doorstoppush)
				i = D_DOOR_STOP;
			else if(brake_value == BRAKE_NO_GOOD)
				i = D_BRAKE_CHECK;
			else if(bFunc_flag & FUNC_SDD1_SDU1_ON)
				i = D_SDD1_SDU1_ON;
			else if(pitbox_reset_active && (!insp))
				i = D_PIT_RESET;
			if (i != D_NORMAL)							/* show call mode					*/
			{
				dwordvalue [1] = ~op_time;
				if (help [6] != i)
				{
					help [6] = i;
					sprintf ((char *)buffer, "%s", callmode [i].text [language]);
					display (16 - strlen ((char *)buffer), 1, buffer);
				}
			}
			else										/* show operating time				*/
			{
				help [6] = D_NORMAL;
				if (dwordvalue [1] != op_time)
				{
					dwordvalue [1] = op_time;
					sprintf ((char *)buffer, "%5lu:%02lu", (dwordvalue [1] / 3600), (dwordvalue [1] / 60) % 60);
					display (8, 1, buffer);
				}
			}
			if (help [8] != level)
			{
				help [8] = level;
				if (help [8] != 0xFF)
				{
					sprintf ((char *)buffer, "%2u", help [8] + 1);
					display (1, 2, buffer);
				}
			}
			if (level > p.top_floor)
			{
				if (buf1 [0])
				{
					wait = 1;
					buf1 [0] = 0;
				}
			}
			else if (level > ((buf1 [0] * 8) + 7))
			{
				buf1 [0]++;
				wait = 1;
			}
			else if (level < (buf1 [0] * 8))
			{
				buf1 [0]--;
				wait = 1;
			}
			display_calls (wait, buf1 [0] * 8, buf1 [0] * 8 + 7, 0);
			wait = 0;
			break;

		case (M_STATUS2):
			if (new)									/* show status window 2				*/
			{
				lcdram = 0;
				display_standardtext (T_STATUS2);
				if (menu_old == M_STATUS3)
					help [0] = p.top_floor / 16;
				else
					help [0] = 0;
				new = 0;
				wait = 1;
			}
			if (key == '#')
			{
				new = 1;
				menu = M_PASSWORD;
				menu_old = menu;
				row = 0;
			}
			else if (key == '*')
			{
				if (help [0] < (p.top_floor / 16))
				{
					help [0]++;
					wait = 1;
				}
				else
				{
					new = 1;
					wait = 0;
					menu_old = menu;
					menu = M_STATUS3;
				}
			}
			else if (key == KEY_UP)
			{
				if (help [0])
				{
					help [0]--;
					wait = 1;
				}
				else
				{
					new = 1;
					menu_old = menu;
					menu = M_STATUS1;
				}
			}
			display_calls (wait, help [0] * 16, help [0] * 16 + 15, 1);
			wait = 0;
			break;

		case (M_STATUS3):
			if (new)									/* show status window 3				*/
			{
				wait = 0;
				lcdram = 0;
				display_standardtext (T_STATUS3);
				if (p.doornumber == 1)				/* only 1 car door					*/
				{
					display (8, 0, "        ");			/* clear text for 2. door				*/
					display (8, 2, "        ");
					display (8, 3, "        ");
				}
				date = 0;
				for (i = 0; i < MAX_FVE_IN; i++)			/* search door switches				*/
				{
					if (p.fve_inpar [i][IO_BASIC_FUNC] == DOOR_IO)
					{
						if (p.fve_inpar [i][IO_SUB_FUNC] == DOOR_OP)
							date |= ((p.fve_inpar [i][IO_DOOR] >> (wait * 2)) & 0x03);
						if (p.fve_inpar [i][IO_SUB_FUNC] == DOOR_CL)
							date |= (((p.fve_inpar [i][IO_DOOR] >> (wait * 2))& 0x03) << 2);
						if (p.fve_inpar [i][IO_SUB_FUNC] == DOOR_REV)
							date |= (((p.fve_inpar [i][IO_DOOR] >> (wait * 2)) & 0x03) << 4);
					}
				}
				for (i = 0; i < MAX_FVE_EX_IN; i++)			/* search door switches				*/
				{
					if (p.fve_ex_inpar [i][IO_BASIC_FUNC] == DOOR_IO)
					{
						if (p.fve_ex_inpar [i][IO_SUB_FUNC] == DOOR_OP)
							date |= ((p.fve_ex_inpar [i][IO_DOOR] >> (wait * 2)) & 0x03);
						if (p.fve_ex_inpar [i][IO_SUB_FUNC] == DOOR_CL)
							date |= (((p.fve_ex_inpar [i][IO_DOOR] >> (wait * 2))& 0x03) << 2);
						if (p.fve_ex_inpar [i][IO_SUB_FUNC] == DOOR_REV)
							date |= (((p.fve_ex_inpar [i][IO_DOOR] >> (wait * 2)) & 0x03) << 4);
					}
				}

				for (i = 0; i < 16; i++)
					buffer [i] = ' ';						/* clear buffer						*/
				help [0] = ~door_io [wait * 2];
				help [2] = ~(ls_state >> (wait * 2));
				help [3] = ~(dooropenpush >> (wait * 2));
				help [4] = ~doortimer [wait * 2];
				wordvalue [0] = ~door_state [wait * 2];
				help [6] = 0xFF;
				if (!wait)
				{
					help [1] = ~door_io [1];
					help [5] = ~doortimer [1];
					wordvalue [1] = ~door_state [1];
				}
				new = 0;
			}
			switch (key)
			{
				case ('#'):
					menu = M_PASSWORD;
					menu_old = menu;
					row = 0;
					new = 1;
					break;

				case ('*'):
					new = 1;
					menu_old = menu;
					if ((!wait) && (p.doornumber > 2))
						wait = 1;
					else
						menu = M_STATUS4;
					break;

				case (KEY_UP):
					new = 1;
					menu_old = menu;
					if (wait)
						wait = 0;
					else if (p.top_floor > 7)
						menu = M_STATUS2;
					else
						menu = M_STATUS1;
					break;
			}
			for (i = 0; i < 2; i++)
			{
				if ((i + (wait * 2)) < p.doornumber)			/* door exists						*/
				{
					if (help [i] != door_io [(wait * 2) + i])
					{								/* door input changed					*/
						help [i] = door_io [(wait * 2) + i];
						if (help [i] == NOT_AVAILABLE)
						{/* door input state not available		*/
							buffer [8*i + 4] = '?';			/* door open switch not available		*/
							buffer [8*i + 5] = '?';			/* door close switch not available		*/
							buffer [8*i + 1] = '?';			/* door reverse switch not available		*/
						}
						else
						{
							if (help [i] & DOOR_OP)		/* door open switch on				*/
								buffer [8*i + 4] = 0xFF;
							else						/* door open switch off				*/
								buffer [8*i + 4] = 8;
							if (help [i] & DOOR_CL)		/* door close switch on				*/
								buffer [8*i + 5] = 0xFF;
							else						/* door close switch off				*/
								buffer [8*i + 5] = 8;
							if (help [i] & DOOR_REV)		/* door reverse switch on				*/
								buffer [8*i + 1] = 0xFF;
							else						/* door reverse switch off				*/
								buffer [8*i + 1] = 8;
						}
						if (!(date & (1 << i)))			/* door has no door open switch		*/
							buffer [8*i + 4] = '-';
						if (!(date & (1 << (2 + i))))		/* door has no door close switch		*/
							buffer [8*i + 5] = '-';
						if (!(date & (1 << (4 + i))))		/* door has no door reverse switch		*/
							buffer [8*i + 1] = '-';
						display (0, 1, buffer);
					}
					if (help [4 + i] != doortimer [(wait * 2) + i])
					{								/* use buf1 instead of normal buffer	*/
						help [4 + i] = doortimer [(wait * 2) + i];
						display (8 * i + 5, 2, "   ");
						sprintf ((char *)buf1, "%2u", help [4 + i]);
						display (8 * i + 5, 2, buf1);
					}
					if (wordvalue [i] != door_state [(wait * 2) + i])
					{
						wordvalue [i] = door_state [(wait * 2) + i];
						row = get_doortext_number ((struct _standardtext *)doortext, wordvalue [i] >> 8);
						if (row)
							sprintf ((char *)buf1, "%-8s", doortext [row - 1].text [language]);
						else
							sprintf ((char *)buf1, "%-8u", wordvalue [i]);
						display (8 * i, 3, buf1);
					}
				}
			}
			help [6] = ~(doorstopstate >> (wait * 2));
			if ((help [2] != ((ls_state >> (wait * 2)) & 0x03)) ||
			(help [6] != ((doorstopstate >> (wait * 2)) & 0x03)) ||
			(help [3] != ((dooropenpush >> (wait * 2)) & 0x03)))
			{										/* light screen or door open push changed	*/
				help [2] = ((ls_state >> (wait * 2)) & 0x03);
				help [3] = ((dooropenpush >> (wait * 2)) & 0x03);
				help [6] = ((doorstopstate >> (wait * 2)) & 0x03);
				for (i = 0; i < 2; i++)
				{
					if ((i + (wait * 2)) < p.doornumber)		/* door exists						*/
					{
						if (bit_select (help [2], i))			/* light screen is interrupted			*/
							buffer [8*i] = 0xFF;
						else							/* light screen is not interrupted		*/
							buffer [8*i] = 8;
						if (bit_select (help [3], i))
							buffer [8*i + 2] = 0xFF;		/* door open push pressed				*/
						else
							buffer [8*i + 2] = 8;			/* door open push not pressed			*/
						if (bit_select (help [6], i))
							buffer [8*i + 3] = 0xFF;		/* door stop push active				*/
						else
							buffer [8*i + 3] = 8;			/* door stop push not active			*/
					}
				}
				display (0, 1, buffer);
			}
			break;

		case (M_STATUS4):
			if (new)									/* show status window 4				*/
			{
				lcdram = 0;
				display_standardtext (T_STATUS4);
				new = 0;
				date = 0;
				wordvalue [1] = 0xFFFF;
				wordvalue [3] = 0xFFFF;
				wordvalue [5] = 0xFFFF;
				wordvalue [7] = 0xFFFF;
			}
			if (key == '#')
			{
				new = 1;
				menu = M_PASSWORD;
				menu_old = menu;
				row = 0;
			}
			else if (key == '*')
			{
				new = 1;
				menu_old = menu;
				menu = M_STATUS5;
			}
			else if (key == KEY_UP)
			{
				new = 1;
				menu_old = menu;
				if (p.doornumber > 2)
					wait = 1;
				else
					wait = 0;
				menu = M_STATUS3;
			}
			if (time_new)
			{
				time_new = 0;
				sprintf ((char *)buffer, "%02u:%02u:%02u", t.hour, t.minute, t.second);
				display (8, 1, buffer);
				if (date != t.day)
				{
					sprintf ((char *)buffer, "%02u.%02u.%02u", t.day, t.month, t.year);
					display (8, 0, buffer);
					date = t.day;
				}
			}
			if ((hse_state == H_STANDSTILL) && (ct > timer))
				wordvalue [0] = (ct - timer + 9) / 10;
			else
				wordvalue [0] = 0;
			if (wordvalue [1] != wordvalue [0])
			{
				wordvalue [1] = wordvalue [0];
				sprintf ((char *)buffer, "%4u", wordvalue [0]);
				display (3, 1, buffer);
			}
			if (parkingtimer > timer)
				wordvalue [2] = (parkingtimer - timer + 9) / 10;
			else
				wordvalue [2] = 0;
			if (wordvalue [3] != wordvalue [2])
			{
				wordvalue [3] = wordvalue [2];
				sprintf ((char *)buffer, "%4u", wordvalue [2]);
				display (3, 2, buffer);
			}
			if (((hse_state == H_MOVING) || (hse_state == H_ARRIVAL) || (hse_state == H_RESET))
			&& (ct > timer))
				wordvalue [4] = (ct - timer + 9) / 10;
			else
				wordvalue [4] = 0;
			if (wordvalue [5] != wordvalue [4])
			{
				wordvalue [5] = wordvalue [4];
				sprintf ((char *)buffer, "%4u", wordvalue [4]);
				display (12, 3, buffer);
			}
			if ((doorstoptimer > timer) && doorstopstate)
				wordvalue [6] = (doorstoptimer - timer + 9) / 10;
			else
				wordvalue [6] = 0;
			if (wordvalue [7] != wordvalue [6])
			{
				wordvalue [7] = wordvalue [6];
				sprintf ((char *)buffer, "%4u", wordvalue [6]);
				display (3, 3, buffer);
			}
			break;

		case (M_STATUS5):
			if (new)									/* show status window 5				*/
			{
				lcdram = 0;
				display_standardtext (T_STATUS5);
				new = 0;
				dwordvalue [0] = ~dCurrentPulse;
				swordvalue [0] = ~nice5000_Rstore_position;
				help [0] = ~level;
				wordvalue [0] = ~nice5000_Rstore_outcurrent;
			}
			if (key == '#')
			{
				new = 1;
				menu = M_PASSWORD;
				menu_old = menu;
				row = 0;
			}
			else if (key == '*')
			{
				new = 1;
				menu_old = menu;
				menu = M_STATUS6;
			}
			else if (key == KEY_UP)
			{
				new = 1;
				menu_old = menu;
				menu = M_STATUS4;
			}
			if (help [0] != level)
			{
				help [0] = level;
				sprintf ((char *)buffer, "%2u", help [0] + 1);
				display (14, 0, buffer);
			}
			if (swordvalue [0] != nice5000_Rstore_position)
			{
				swordvalue [0] = nice5000_Rstore_position;
				sprintf ((char *)buffer, "%6d", swordvalue [0]);
				display (8, 1, buffer);
			}
			if (dwordvalue[0] != dCurrentPulse)
			{
				dwordvalue[0] = dCurrentPulse;
				sprintf ((char *)buffer, "%10d", dwordvalue[0]);
				display (6, 2, buffer);
			}
			if (wordvalue[0] != nice5000_Rstore_outcurrent)
			{
				wordvalue[0] = nice5000_Rstore_outcurrent;
				sprintf ((char *)buffer, "%3d.%02d", wordvalue[0]/100, wordvalue[0]%100);
				display (9, 3, buffer);
			}
			break;

		case M_STATUS6:
			if (new)									/* show status window 5				*/
			{
				lcdram = 0;
				display_standardtext (T_STATUS6);
				new = 0;
				wordvalue [0] = ~nice5000_Rstore_setspd;
				wordvalue [1] = ~nice5000_Rstore_feedspd;
				wordvalue [2] = ~nice5000_Rstore_dcvoltage;
				wordvalue [3] = ~nice5000_Rstore_outvoltage;
			}
			if (key == '#')
			{
				new = 1;
				menu = M_PASSWORD;
				menu_old = menu;
				row = 0;
			}
			else if (key == '*')
			{
				new = 1;
				menu_old = menu;
				menu = M_STATUS7;
			}
			else if (key == KEY_UP)
			{
				new = 1;
				menu_old = menu;
				menu = M_STATUS5;
			}
			if (wordvalue[0] != nice5000_Rstore_setspd)
			{
				wordvalue[0] = nice5000_Rstore_setspd;
				floatvalue = (float)wordvalue[0] / 1000.0;
				sprintf ((char *)buffer, "%2.3f", floatvalue);
				display (8, 0, buffer);
			}
			if (wordvalue [1]!= nice5000_Rstore_feedspd)
			{
				wordvalue[1] = nice5000_Rstore_feedspd;
				floatvalue = (float)wordvalue[1] / 1000.0;
				sprintf ((char *)buffer, "%2.3f", floatvalue);
				display (8, 1, buffer);
			}
			if (wordvalue[2] != nice5000_Rstore_dcvoltage)
			{
				wordvalue[2] = nice5000_Rstore_dcvoltage;
				sprintf ((char *)buffer, "%4d.%d", wordvalue[2]/10, wordvalue[2]%10);
				display (9, 2, buffer);
			}
			if (wordvalue[3] != nice5000_Rstore_outvoltage)
			{
				wordvalue[3] = nice5000_Rstore_outvoltage;
				sprintf ((char *)buffer, "%3d.%d", wordvalue[3]/10, wordvalue[3]%10);
				display (10, 3, buffer);
			}
			break;

		case M_STATUS7:
			if (new)									/* show status window 5 			*/
			{
				lcdram = 0;
				display_standardtext (T_STATUS7);
				new = 0;
				wordvalue [0] = ~nice5000_Rstore_outpwr;
				wordvalue [1] = ~nice5000_Rstore_outfreq;
				wordvalue [2] = ~nice5000_Rstore_outtorque;
				wordvalue [3] = ~nice5000_Rstore_tkcurrent;
			}
			if (key == '#')
			{
				new = 1;
				menu = M_PASSWORD;
				menu_old = menu;
				row = 0;
			}
			else if (key == '*')
			{
				new = 1;
				menu_old = menu;
				menu = M_STATUS1;
			}
			else if (key == KEY_UP)
			{
				new = 1;
				menu_old = menu;
				menu = M_STATUS6;
			}
			if (wordvalue[0] != nice5000_Rstore_outpwr)
			{
				wordvalue[0] = nice5000_Rstore_outpwr;
				if(nice5000_Rstore_outpwr > 0x8000)
					{
						wordvalue[0] = 0xFFFF - nice5000_Rstore_outpwr;
						buffer[0] = '-';
						sprintf ((char *)(&buffer[1]), "%3d.%02d", wordvalue[0]/100, wordvalue[0]%100);
					}
				else
					{
						wordvalue[0] = nice5000_Rstore_outpwr;
						sprintf ((char *)buffer, "%3d.%02d", wordvalue[0]/100, wordvalue[0]%100);
					}
				display (8, 0, buffer);
			}
			if (wordvalue [1]!= nice5000_Rstore_outfreq)
			{
				wordvalue[1] = nice5000_Rstore_outfreq;
				sprintf ((char *)buffer, "%3d.%02d", wordvalue[1]/100, wordvalue[1]%100);
				display (8, 1, buffer);
			}
			if (wordvalue[2] != nice5000_Rstore_outtorque)
			{
				wordvalue[2] = nice5000_Rstore_outtorque;
				if(nice5000_Rstore_outtorque > 0x8000)
					{
						wordvalue[2] = 0xFFFF - nice5000_Rstore_outtorque;
						buffer[0] = '-';
						sprintf ((char *)(&buffer[1]), "%3d.%d", wordvalue[2]/10, wordvalue[2]%10);
					}
				else
					{
						wordvalue[2] = nice5000_Rstore_outtorque;
						sprintf ((char *)buffer, "%3d.%d", wordvalue[2]/10, wordvalue[2]%10);
					}
				display (10, 2, buffer);
			}
			if (wordvalue[3] != nice5000_Rstore_tkcurrent)
			{
				wordvalue[3] = nice5000_Rstore_tkcurrent;
				sprintf ((char *)buffer, "%3d.%02d", wordvalue[3]/100, wordvalue[3]%100);
				display (9, 3, buffer);
			}
			break;

		case (M_GROUP):
			if (new)									/* show HSE of other groups			*/
			{
				display_standardtext (T_GROUP);
				help [0] = ~hse_hb_state;
				new = 0;
			}
			if (help [0] != hse_hb_state)					/* state of hse boards changed			*/
			{
				help [0] = hse_hb_state;					/* save actual state					*/
				for (i = 0; i < MAX_GR; i++)
				{
					if (i == p.groupnumber)				/* number of this HSE					*/
					{
						display (i, 2, "L");
					}
					else if (i < p.groupsize)				/* HSE is enabled						*/
					{
						if (bit_select (hse_hb_state, i))
							display (i, 2, "*");	/* heartbeat from HSE					*/
						else						
							display (i, 2, "-");	/* no heartbeat from HSE				*/
					}
					else if (bit_select (hse_hb_state, i))	/* HSE not enabled, but on bus			*/
						display (i, 2, "+");
					else								/* HSE not enabled and not on bus		*/
						display (i, 2, " ");
				}
			}
			if (key)									/* finish with every key				*/
			{
				menu_old = menu;
				menu = search_mainmenu (menu);
				new = 1;
			}
			break;

		case (M_HSE_INFO):
			if (new)
			{
				clrscr();
				cursor(_SOLIDCURSOR);
				sprintf((char *)buffer, "HW:%s", hardware_hse);
				display(0, 0, buffer);
				sprintf((char *)buffer, "SW:%s", version_hse);
				display(8, 0, buffer);
				value = ~in;
				help[4] = ~out;
				help[0] = ~state_msi;
				new = 0;
				numberkey = 1;								// disable direct access with xxx*
				write_io(buf1,	p.hse_inpar[0], MAX_HSE_IN + MAX_HSE_EX_IN, INPUT);
				write_io(buf2, p.hse_outpar[0], MAX_HSE_OUT + MAX_HSE_EX_OUT, OUTPUT);
				help[3] = ~dz_input;
				if (menu_old != M_IOPARA)
				{
					help[1] = 0;
					help[2] = 1;
				}
			}
			if (value != in)								// input state changed
			{
				value = in;
				for(i = 0; i < MAX_HSE_IN + MAX_HSE_EX_IN; i++)
				{
					j = bit_selectd(value, i);
					if (i < MAX_HSE_IN)
					{
						if (((j) && (!(p.hse_inpar[i][5] & POLARITY_BIT))) || (!j) && (p.hse_inpar[i][5] & POLARITY_BIT))
							inverse = 1;
						else
							inverse = 0;
					}
					else
					{
						if (((j) && (!(p.hse_ex_inpar[i - MAX_HSE_IN][5] & POLARITY_BIT))) || (!j) && (p.hse_ex_inpar[i - MAX_HSE_IN][5] & POLARITY_BIT))
							inverse = 1;
						else
							inverse = 0;
					}
					if ((buf1[i] == '-') || (buf1[i] == 227))
					{
						if (j)
							buf1[i] = 227;
						else
							buf1[i] = '-';
					}
					else if (inverse)
					{
						if (buf1[i] > 90)
							buf1[i] -= 32;
					}
					else
					{
						if (buf1[i] <= 90)
							buf1[i] += 32;
					}
				}
				display(0, 1, buf1);
				display(0, 2, &buf1[16]);
				gotoxy(help[1], help[2]);
			}
			if (help[4] != out)
			{
				help[4] = out;				
				for(i = 0; i < MAX_HSE_OUT + MAX_HSE_EX_OUT; i++)
				{
					j = bit_selectd(help[4], i);
					if (i < MAX_HSE_OUT)
					{
						if (((j) && (!(p.hse_outpar[i][5] & POLARITY_BIT))) || ((!j) && (p.hse_outpar[i][5] & POLARITY_BIT)))
							inverse = 1;
						else
							inverse = 0;
					}
					else
					{
						if (((j) && (!(p.hse_ex_outpar[i - MAX_HSE_OUT][5] & POLARITY_BIT))) || ((!j) && (p.hse_ex_outpar[i - MAX_HSE_OUT][5] & POLARITY_BIT)))
							inverse = 1;
						else
							inverse = 0;
					}
					if ((buf2[i] == '-') || (buf2[i] == 227))
					{
						if (j)
							buf2[i] = 227;
						else
							buf2[i] = '-';
					}
					else if (inverse)
					{
						if (buf2[i] > 90)
							buf2[i] -= 32;
					}
					else
					{
						if (buf2[i] <= 90)
							buf2[i] += 32;
					}
				}
				display(0, 3, buf2);
				gotoxy(help[1], help[2]);
			}
			if (help[0] != state_msi)
			{
				if (state_msi)
					display(9, 2, "MSI");
				else
					display(9, 2, "msi");
				help[0] = state_msi;
				gotoxy(help[1], help[2]);
			}
			if (help[3] != dz_input)
			{
				help[3] = dz_input;
				if (help[3])
					display(13, 2, "ZON");
				else
					display(13, 2, "zon");
				gotoxy(help[1], help[2]);
			}
//增加闪烁功能
			for(i=0; i<MAX_HSE_IN + MAX_HSE_EX_IN; i++)
			{
				if (i < MAX_HSE_IN)
				{
					if(p.hse_inpar[i][IO_BASIC_FUNC] && (p.hse_inpar[i][IO_ENABLE] == DISABLE))
					{
						if(sec_cnt % 2)
							help[5] = buf1[i];
						else
							help[5] = 0x20;
						if(i < 16)
							lcd_display [1][i] = (BYTE)help[5];
						else
							lcd_display [2][i-16] = (BYTE)help[5];
					}
				}
				else
				{
					if(p.hse_ex_inpar[i - MAX_HSE_IN][IO_BASIC_FUNC] && (p.hse_ex_inpar[i - MAX_HSE_IN][IO_ENABLE] == DISABLE))
					{
						if(sec_cnt % 2)
							help[5] = buf1[i];
						else
							help[5] = 0x20;
						if(i < 16)
							lcd_display [1][i] = (BYTE)help[5];
						else
							lcd_display [2][i-16] = (BYTE)help[5];
					}
				}
			}
			for(i=0; i<MAX_HSE_OUT + MAX_HSE_EX_OUT; i++)
			{
				if (i < MAX_HSE_OUT)
				{
					if(p.hse_outpar[i][IO_BASIC_FUNC] && (p.hse_outpar[i][IO_ENABLE] == DISABLE))
					{
						if(sec_cnt % 2)
							help[5] = buf2[i];
						else
							help[5] = 0x20;
						lcd_display [3][i] = (BYTE)help[5];
					}
				}
				else
				{
					if(p.hse_ex_outpar[i - MAX_HSE_OUT][IO_BASIC_FUNC] && (p.hse_ex_outpar[i - MAX_HSE_OUT][IO_ENABLE] == DISABLE))
					{
						if(sec_cnt % 2)
							help[5] = buf2[i];
						else
							help[5] = 0x20;
						lcd_display [3][i] = (BYTE)help[5];
					}
				}
			}
//增加闪烁功能
			switch(key)
			{
#if	SRU_OLD			
				case('9'):
					if (password_right <= RIGHT_INSTALL)
						SetSwitch(SWITCH_ULSI, ~state_ulsi);
					break;
#endif

				case(0):
					break;
				case(KEY_UP):
					if (help[1])
						help[1]--;
					else
					{
						if (help[2] == 1)
						{
							help[2] = 3;
							help[1] = MAX_HSE_OUT + MAX_HSE_EX_OUT - 1;
						}
						else if (help[2] == 2)
						{
							help[2] = 1;
							help[1] = 15;
						}
						else if (help[2] == 3)
						{
							help[2] = 2;
							help[1] = MAX_HSE_IN + MAX_HSE_EX_IN - 16 - 1;
						}
					}
					gotoxy(help[1], help[2]);
					break;
				case('*'):
					if (((help[1] >= MAX_HSE_IN+MAX_HSE_EX_IN - 16 - 1) && (help[2] == 2)) || ((help[1] >= MAX_HSE_OUT+MAX_HSE_EX_OUT-1) && (help[2] == 3)) || (help[1] >= 15))
					{
						help[1] = 0;
						if (help[2] == 3)
							help[2] = 1;
						else
							++ help[2];
					}
					else
						help[1]++;
					gotoxy(help[1], help[2]);
					break;
				case('#'):
					bus = CAN_A;
					if (help[2] == 2)
						paranumber = help[1] + 16;
					else
						paranumber = help[1];
					if (help[2] == 3)
						paratype = M_OUT_HSE;
					else
						paratype = M_IN_HSE;
					menu_old = menu;
					menu = M_IOPARA;
					new = 1;
					break;
				case(KEY_ESC):
					numberkey = 0;							// enable direct access with xxx* again
					menu_old = menu;
					menu = search_mainmenu(menu);
					new = 1;
					break;
				default:									// number 1 - 8; set outputs
					if (password_right <= RIGHT_INSTALL)
					{
						if (key == '0')
						{
							if (help[2] == 3)
							{
								if (bit_select(out, help[1]))
									bit_reset(out, help[1]);
								else
									bit_set(out, help[1]);
							}
						}
					}
					break;
			}
			break;

		case (M_TSE_INFO):
		case (M_ESE_INFO):
		case (M_FVE_INFO):
		case (M_EXE_INFO):
			if (new)
			{
				display_standardtext (T_WAIT);
				new = 0;
				wait = 0;
				numberkey = 1;								/* disable direct access with xxx*				*/
				if (menu_old != M_IOPARA)
				{
					if (menu == M_FVE_INFO)
					{
						buf1 [0] = 0;
						buf1 [1] = 1;
					}
					else
					{
						buf1 [0] = 8;
						buf1 [1] = 2;
					}
					buf1[2] = 0;
				}
				buf1[3] = 0;
			}
			wait = hwtest(wait, menu, pcb_number, &buf1[0], &buf1[1], &buf1[2], buf1[3]);
			buf1[3] = 0;
			switch (key)
			{
				case (0):
				case ('9'):
					break;
				case ('0'):									/* toggle the output on the cursor position		*/
					if ((wait > 6) && (buf1 [1] == 3))		/* startup ready and cursor located on output	*/
						wait = '1' + buf1 [0];					/* set ASCII value of output					*/
					if (menu != M_FVE_INFO)					/* on all other boards output 1 is on pos. 8	*/
						wait -= 8;
					break;
				case ('*'):
					buf1 [3] = 1;
					break;
				case (KEY_UP):
					buf1 [3] = 2;
					break;
				case ('#'):
					bus = CAN_A;
					switch (menu)
					{
						case (M_TSE_INFO):
							if (buf1 [1] == 2)
								paratype = M_IN_TSE;
							else
								paratype = M_OUT_TSE;
							paranumber = buf1[2] * 8 + buf1 [0] - 8;
							break;
							
						case (M_EXE_INFO):			//需要考虑轿厢CAN还是外呼CAN的转换问题
							if (buf1 [1] == 2)
								paratype = M_IN_EXE;
							else
								paratype = M_OUT_EXE;
							paranumber = buf1[2] * 8 + buf1 [0] - 8;
							if(pcb_number == 1)
								bus = CAN_B;
							break;
							
						case (M_ESE_INFO):
							bus = CAN_B;
							if (buf1 [1] == 2)
								paratype = M_IN_ESE;
							else
								paratype = M_OUT_ESE;
							paranumber = buf1 [0] - 8;
							break;
							
						case (M_FVE_INFO):
							if (buf1 [1] != 3)
								paratype = M_IN_FVE;
							else
								paratype = M_OUT_FVE;
							if (buf1 [1] == 2)
								paranumber = buf1 [0] + 16;
							else
								paranumber = buf1 [0];
							break;
					}
					menu_old = menu;
					menu = M_IOPARA;
					new = 1;
					break;
				case (KEY_ESC):
					numberkey = 0;							/* enable direct access with xxx* again			*/
					menu_old = menu;
					switch (menu_old)
					{
						case (M_TSE_INFO):
							menu = M_TSE;
							break;
						case (M_EXE_INFO):
							menu = M_EXE;
							break;
						case (M_ESE_INFO):
							menu = M_ESE;
							break;
						case (M_FVE_INFO):
							menu = M_HARDWARE;
							break;
					}
					sdo_interrupt = 0;
					new = 1;
					break;
				default:									/* number 1 - 8; set outputs					*/
					if (wait > 6)								/* startup ready								*/
						wait = key;
			}
			break;

		case (M_STATUS_INTERNAL):
		if (new)
		{
			clrscr ();
			dwordvalue[4] = 0x01;
			dwordvalue[6] = 0x00;
			dwordvalue[0] = ~next_floor;//ese_sdo_master;//cana_tx_err;
			dwordvalue[1] = ~target_floor;//ese_init_error[0];//cana_rx_err;
			dwordvalue[2] = ~ese_state;//ese_state;//error_bit;
			dwordvalue[3] = ~drive_state;//ese_sdo_id;//error_code;
			
			new = 0;
		}
		if (dwordvalue[4])
		{
			if (!dwordvalue[6])
			{
				if (dwordvalue [0] != next_floor)
				{
					dwordvalue [0] = next_floor;
					sprintf ((char *)buffer, "nextfloor: %d", dwordvalue [0]);
					display (0, 0, buffer);
				}
				if (dwordvalue [1] != target_floor)
				{
					dwordvalue [1] = target_floor;
					sprintf ((char *)buffer, "target: %2d", dwordvalue [1]);
					display (0, 1, buffer);
				}
				if (dwordvalue [2] != ese_state)
				{
					dwordvalue [2] = ese_state;
					sprintf ((char *)buffer, "state: %3d", dwordvalue [2]);
					display (0, 2, buffer);
				}
				if (dwordvalue [3] != drive_state)
				{
					dwordvalue [3] = drive_state;
					sprintf ((char *)buffer, "dr state: %01d", dwordvalue [3]);
					display (0, 3, buffer);
				}
			}
		}
		passwordtimer = PASSWORDTIME;
		break;

		case (M_PASSWORD):
			if (new)
			{
				if ((!password_admin) && (!password_help) && (!password_install) && (!password_maintain))
				{
					passwordtimer = PASSWORDTIME;
					menu = M_MAIN;
					new = 1;
					numberkey = 0;
					password_right = RIGHT_ADMIN;
				}
				else
				{
					new = 0;
					numberpos = 0;
					for (i = 0; i < 10; i++)
						buffer [i] = 0;
					display_standardtext (T_PASSWORD);
					cursor (_NORMALCURSOR);
					gotoxy (0, 1);
					numberkey = 1;					/* disable direct access with xxx*		*/
					value = 0;
					wait = 0;
				}
			}
			else if (wait)
			{
				if (key)
				{
					if (key != '#')
						menu = M_STATUS1;
					new = 1;
					numberkey = 0;					/* enable direct access with xxx* again	*/
				}
			}
			else
			{
				switch (key)
				{
					case (0):
						break;
					case (KEY_UP):
					case (KEY_ESC):
					case ('*'):
						menu = M_STATUS1;
						new = 1;
						numberkey = 0;				/* enable direct access with xxx* again	*/
						for(i=0; i<9; i++)
							passwdbuf[i] = 0x00;
						break;

					case ('#'):
						sscanf ((const char *)buffer, "%lu", &value);
						if ((((value == password_admin) || (value == DEFAULT_PSW_ADMIN)) && (password_admin)) ||
									(((value == password_help) || (value == DEFAULT_PSW_HELP)) && (password_help))	||
									(((value == password_install) || (value == DEFAULT_PSW_INSTALL)) && (password_install)) ||
									(((value == password_maintain) || (value == DEFAULT_PSW_MAINTAIN)) && (password_maintain)))
						{
							passwordtimer = PASSWORDTIME;
							menu = M_MAIN;
							new = 1;
							numberkey = 0;			/* enable direct access with xxx* again			*/
							if ((value == password_admin))// || (!password_admin))
								password_right = RIGHT_ADMIN;
							else if ((value == password_help))// || (!password_help))
								password_right = RIGHT_HELP;
							else if ((value == password_install))// || (!password_install))
								password_right = RIGHT_INSTALL;
							else if ((value == password_maintain))// || (!password_maintain))
								password_right = RIGHT_MAINTAIN;
						}
						else if (value == (4455 + 12345) * (p.top_floor + 1 - p.bot_floor) / p.doornumber)
						{
							password_maintain = 0;
							password_right = RIGHT_MAINTAIN;
							WriteParaData (&password_maintain, EE_PASSWORD3, sizeof(password_maintain));
							passwordtimer = PASSWORDTIME;
							menu = M_MAIN;
							new = 1;
							numberkey = 0;			/* enable direct access with xxx* again	*/
						}
						else if (value == SUPER_PROGRAM)
						{
							password_admin = 0;
							password_help = 0;
							password_install = 0;
							password_maintain = 0;
							WriteParaData (&password_admin, EE_PASSWORD0, sizeof(password_admin));
							WriteParaData (&password_help, EE_PASSWORD1, sizeof(password_help));
							WriteParaData (&password_install, EE_PASSWORD2, sizeof(password_install));
							WriteParaData (&password_maintain, EE_PASSWORD3, sizeof(password_maintain));
							menu = M_MAIN;
							new = 1;
							numberkey = 0;
							passwordtimer = PASSWORDTIME;
							password_right = RIGHT_ADMIN;
						}
						else
						{
							display_standardtext (T_PASSWORD_WRONG);
							wait = 1;
							menu = menu_old;
						}
						for(i=0; i<9; i++)
							passwdbuf[i] = 0x00;
						break;

					default:
						if (numberpos < 9)
						{
							passwdbuf [numberpos] = '*';
							buffer [numberpos++] = key;
							display (0, 1, passwdbuf);
						}
						break;
				}
			}
			break;

		case (M_SHOW_HIST):
			if (new)
			{
				if (menu_old != M_HIST_DETAIL)
				{
					for (i = 0; i < sizeof (hist); i++)			/* read error history					*/
						*(((BYTE *)&hist) + i) = *(((BYTE *)&errorhist) + i);
					help [2] = 0;
				}
				help [0] = ~hist.histpointer;
				new = 0;
				cursor (_SOLIDCURSOR);
			}
			if (help [0] != hist.histpointer)
			{
				clrscr ();
				help [0] = hist.histpointer;
				for (i = 0; i < 2; i++)
				{
					help [1] = (help [0] + MAX_HIST - i - 1 - ((help [2] / 2) * 2)) % MAX_HIST;
					if ((help [2] == MAX_HIST) || (!(hist.hist [help [1]].errorcode)))
					{
						display_line (T_HIST_END, 2 * i);
						display_line (T_ESC, (2 * i) + 1);
						i = 2;
					}
					else
					{
						sprintf ((char *)buffer, "%u.%u.%02u", hist.hist [help [1]].t.day, hist.hist [help [1]].t.month, hist.hist [help [1]].t.year);
						display (0, 2 * i, buffer);
						sprintf ((char *)buffer, "%2u:%02u:%02u",	hist.hist [help [1]].t.hour, hist.hist [help [1]].t.minute, hist.hist [help [1]].t.second);
						display (8, 2 * i, buffer);
						display_error (hist.hist [help [1]].errorcode, (2 * i) + 1, hist.hist [help [1]].idorpwr.node_id);
					}
				}
				gotoxy (0, ((help [2] % 2) * 2) + 1);
			}
			switch (key)
			{
				case ('#'):
					help [1] = (help [0] + MAX_HIST - 1 - help [2]) % MAX_HIST;
					if ((help [2] == MAX_HIST) || (!(hist.hist [help [1]].errorcode)))
					{
						numberkey = 0;				/* enable direct access with xxx* again	*/
						menu_old = M_SHOW_HIST;		/* set old menu						*/
						menu = search_mainmenu (M_SHOW_HIST);
						new = 1;
					}
					else
					{
						menu_old = M_SHOW_HIST;
						menu = M_HIST_DETAIL;
						new = 1;
					}
					break;

				case ('*'):
					help [1] = (help [0] + MAX_HIST - 1 - help [2]) % MAX_HIST;
					if ((help [2] == MAX_HIST) || (!(hist.hist [help [1]].errorcode)))
						help [2] = 0;
					else
						help [2] += 1;
					gotoxy (0, ((help [2] % 2) * 2) + 1);
					if (!(help [2] % 2))
						help [0] = ~ hist.histpointer;
					break;

				case (KEY_UP):
					if (!help [2])
					{
						help [2] = MAX_HIST;				/* set to last value in error history			*/
						for (i = 0; i < MAX_HIST; i++)
						{							/* search last input in error history		*/
							help [1] = (help [0] + MAX_HIST - 1 - i) % MAX_HIST;
							if (!(hist.hist [help [1]].errorcode))
							{
								help [2] = i;			/* set pointer to last input in history	*/
								i = MAX_HIST;			/* stop search						*/
							}
						}
						help [0] = ~ hist.histpointer;
					}
					else
						help [2]--;
					gotoxy (0, ((help [2] % 2) * 2) + 1);
					if (help [2] % 2)
						help [0] = ~ hist.histpointer;
					break;

				case (KEY_ESC):
					numberkey = 0;					/* enable direct access with xxx* again	*/
					menu_old = M_SHOW_HIST;			/* set old menu						*/
					menu = search_mainmenu (M_SHOW_HIST);
					new = 1;
					break;
			}
			break;

		case (M_SHOW_COUNT):
			if (new)
			{
				help [0] = 1;							/* number of errors								*/
				count [0].pcb = 0xFFFF;
				for (i = 0; i < MAX_GEN_ERROR; i++)
				{
					if (errorcounter.error_gen [i])
					{
						count [help [0]].count = errorcounter.error_gen [i];
						count [help [0]].number = i;
						count [help [0]].pcb = 0;
						help [0]++;
					}
				}
				for (i = 0; i < MAX_PCB_ERROR; i++)
				{
					if (errorcounter.error_hse [i])
					{
						count [help [0]].count = errorcounter.error_hse [i];
						count [help [0]].number = i;
						count [help [0]].pcb = HSE_ID;
						help [0]++;
					}
					if (errorcounter.error_fve [i])
					{
						count [help [0]].count = errorcounter.error_fve [i];
						count [help [0]].number = i;
						count [help [0]].pcb = FVE_ID;
						help [0]++;
					}
					if (errorcounter.error_drv [i])
					{
						count [help [0]].count = errorcounter.error_drv [i];
						count [help [0]].number = i;
						count [help [0]].pcb = EXE_ID;
						help [0]++;
					}
					for (j = 0; j < MAX_TSE; j++)
					{
						if (errorcounter.error_tse [j][i])
						{
							count [help [0]].count = errorcounter.error_tse [j][i];
							count [help [0]].number = i;
							count [help [0]].pcb = TSE_ID + j;
							help [0]++;
						}
					}
					for (j = 0; j < MAX_ESE; j++)
					{
						if (errorcounter.error_ese [j][i])
						{
							count [help [0]].count = errorcounter.error_ese [j][i];
							count [help [0]].number = i;
							count [help [0]].pcb = ESE_ID + j + 128;
							help [0]++;
						}
					}
				}
				new = 0;
				help [1] = 0;							/* actual error to display						*/
				help [2] = 1;							/* display new values							*/
				cursor (_NOCURSOR);
			}
			if (help [2])
			{
				clrscr ();
				help [2] = 0;
				for (i = 0; i < 2; i++)
				{
					if ((help [1] + i) < help [0])
					{
						switch (count [help [1] + i].pcb)
						{
							case (0xFFFF):
								display_line (T_ERRCNT_START, 0);
								sprintf ((char *)buffer, "%u.%u.%02u", errorcounter.t.day, errorcounter.t.month, errorcounter.t.year);
								display (0, 1, buffer);
								sprintf ((char *)buffer, "%2u:%02u:%02u",	errorcounter.t.hour, errorcounter.t.minute, errorcounter.t.second);
								display (8, 1, buffer);
								break;
								
							case (0):
								buffer[0] = 'E';
								sprintf ((char *)(&buffer[1]), "%03d", errortext [count [help [1] + i].number].err_code);
								display (0, 2 * i, buffer);
								sprintf ((char *)buffer, "%u", count [help [1] + i].count);
								display (16 - strlen ((char *)buffer), 2 * i + 1, buffer);
								break;
								
							default:
								buffer[0] = 'E';
								sprintf ((char *)(&buffer[1]), "%03d", errortext_bg [count [help [1] + i].number].err_code);
								display (0, 2 * i, buffer);
								sprintf ((char *)buffer, "%u", count [help [1] + i].count);
								display (16 - strlen ((char *)buffer), 2 * i + 1, buffer);
								switch (count [help [1] + i].pcb)
								{
									case (HSE_ID):
										display (0, 2 * i + 1, "MCU:");
										break;
									case (FVE_ID):
										display (0, 2 * i + 1, "CDU:");
										break;
									case (DRV_ID):
									case (DCU_ID):
									case (EXE_ID):
										display (0, 2 * i + 1, "EXE:");
										break;
									default:
										if ((count [help [1] + i].pcb >= (128 + ESE_ID))
										&& (count [help [1] + i].pcb <	(128 + ESE_ID + MAX_ESE)))
										{
											sprintf ((char *)buffer, "LCU%u.%u:", ((count [help [1] + i].pcb - 128 - ESE_ID) % MAX_FLOOR) + 1, ((count [help [1] + i].pcb - 128 - ESE_ID) / MAX_FLOOR) + 1);
										}
										else if ((count [help [1] + i].pcb >= TSE_ID)
										&& (count [help [1] + i].pcb < (TSE_ID + MAX_TSE)))
										{
											sprintf ((char *)buffer, "PCU%u:", count [help [1] + i].pcb - TSE_ID + 1);
										}
										display (0, 2 * i + 1, buffer);
										break;
								}
								break;
						}
					}
				}
			}
			switch (key)
			{
				case ('*'):
					help [1] += 2;
					if (help [1] >= help [0])
						help [1] = 0;
					help [2] = 1;
					break;
				case (KEY_UP):
					if (help [1] > 1)
						help [1] -= 2;
					else
						help [1] = (help [0] - 1) / 2 * 2;
					help [2] = 1;
					break;
				case (KEY_ESC):
					menu_old = menu;						/* set old menu									*/
					menu = search_mainmenu (menu_old);
					new = 1;
					break;
			}
			break;

		case (M_MOTOR_TUNE):
			menu = M_TEACH_IN;
			new = 1;
			row = 0;
			break;

		case (M_SHAFT_SENSORS):
			if (new)									/* show status window 3				*/
			{
				if ((hse_state == H_NO_SETUP || hse_state == H_STANDSTILL)
					&& (p.motor_tuned))
				{
					teach_in = TI_SHAFT;
					teach_in_text = T_T_DOORCL;
				}
				else
					teach_in_text = T_T_NO_ACCESS;
				help [0] = 0;
				help [1] = hse_text;
				new = 0;
			}
			if (key == '#')
			{
				if (help [0] == T_T_READY)
					menu = M_SAVEPARA;
				else
				{
					menu_old = menu;
					menu = M_TEACH_IN;
				}
				new = 1;
				row = 0;
			}
			else if (key == KEY_ESC)
			{
				menu_old = menu;
				menu = M_TEACH_IN;
				teach_in = 0;
				new = 1;
				row = 0;
			}
			if ((help [0] != teach_in_text) || (help [1] != hse_text))
			{
				display_standardtext (teach_in_text);
				help [0] = teach_in_text;
				help [1] = hse_text;
				if ((help [0] == T_T_GEN_ERROR) && hse_text)
				{
					sprintf ((char *)buffer ,"%-9s", statetext [hse_text - 1].text [language]);
					display (0, 3, buffer);
				}
			}
			passwordtimer = PASSWORDTIME;
			break;

		case (M_NUMBERPARA):						/* set a parameter value by numbers	*/
		case (M_FLOORPARA):
		case (M_LIFTPARA):
		case (M_LIFTBITPARA):
			if (new)
			{
				new = 0;
				numberpos = 0;
				clearline (row % 4);
				if (paratext [paranumber].array)			/* array parameter					*/
				{
					help [1] = p.bot_floor;
					switch (paratext [paranumber].array)
					{
						case (1):
							sprintf ((char *)buffer, "%2u.%s", row + help [1] + 1, paratext [paranumber].text [language]);
							break;

						case (2):
							if (row % 2)
								sprintf ((char *)buffer, "%2u.%s \6", (row / 2) + help [1] + 1, paratext [paranumber].text [language]);
							else
								sprintf ((char *)buffer, "%2u.%s \5", (row / 2) + help [1] + 1, paratext [paranumber].text [language]);
							break;
					}
					help [0] = row + (paratext [paranumber].array * help [1]);
				}
				else									/* single parameter					*/
				{
					display (0, row % 4, (BYTE *)paratext [paranumber].text [language]);
					help [0] = 0;
				}
				for (i = 0; i < 20; i++)
					buffer [i] = 0;
				cursor (_NORMALCURSOR);
				gotoxy (15, row % 4);
				numberkey = 1;						/* disable direct access with xxx*		*/
				wait = 0;
			}
			if (wait == 1)								/* wrong parameter, wait for key				*/
			{
				if (key)
				{
					wait = 0;
					menu = menu_old;
					new = 1;
					numberkey = 0;							/* enable direct access with xxx* again			*/
				}
			}
			else if (wait == 2)							/* send parameter by SDO						*/
			{
				if (!send_parameter (paratext [paranumber].pointer, help [0], value))
				{
					switch (paratext [paranumber].size)
					{
						case (1):
							vptr = (BYTE *)paratext [paranumber].pointer + help [0];
							oldvalue = *((BYTE *)vptr);
							*((BYTE *)vptr) = value;
							break;
						case (2):
							vptr = (WORD *)paratext [paranumber].pointer + help [0];
							oldvalue = *((WORD *)vptr);
							*((WORD *)vptr) = value;
							break;
						case (4):
							vptr = (DWORD *)paratext [paranumber].pointer + help [0];
							oldvalue = *((DWORD *)vptr);
							*((DWORD *)vptr) = value;
							break;
						case (5):
							vptr = (float *)paratext [paranumber].pointer + help [0];
							oldvalue = *((float *)vptr);
							*((float *)vptr) = floatvalue;
							break;
					}
					if (((BYTE *)vptr == &p.simulation) && (*(BYTE *)vptr))
					{
						menu = M_SIMU;
						p.simulation = *(BYTE *)vptr;
						new = 1;
					}
					else if(((BYTE *)vptr == &settimeflag) && (value))
					{
						menu = M_SET_TIME;
						new = 1;
					}
					else if(((BYTE *)vptr == &savepara) && (*(BYTE *)vptr))
					{
						menu = M_BASIC_SAVE;
						new = 1;
					}
					else if(((BYTE *)vptr == &start_limit) && (*(BYTE *)vptr))
					{
						menu_old = M_TRIPLIMIT;
						menu = M_STARTLIMIT;
						start_limit = 0;
						new = 1;
					}
					else if(((BYTE *)vptr == &reset_mcu) && (*(BYTE *)vptr))
					{
						menu = M_RESET_MCU;
						new = 1;
					}
//增加抱闸力检测功能	2017-03-14
					else if((BYTE *)vptr == &bTest_brake)
					{
						if((drive_state == D_STOPPED) && bTest_brake && (!driveerror))
							{
								brake_check_forced = 1;
								bTest_brake = 1;
								brake_check_return = 0;
								menu = M_BRAKE_CHECK;
								new = 1;
							}
						else
							{
								brake_check_forced = 0;
								bTest_brake = 0;
							}
					}
//增加抱闸力检测功能	2017-03-14
					else
					{
						if (paratext [paranumber].menu == M_CHANGE_PW0)
							WriteParaData (&password_admin, EE_PASSWORD0, sizeof(password_admin));
						else if (paratext [paranumber].menu == M_CHANGE_PW1)
							WriteParaData (&password_help, EE_PASSWORD1, sizeof(password_help));
						else if (paratext [paranumber].menu == M_CHANGE_PW2)
							WriteParaData (&password_install, EE_PASSWORD2, sizeof(password_install));
						else if (paratext [paranumber].menu == M_CHANGE_PW3)
							WriteParaData (&password_maintain, EE_PASSWORD3, sizeof(password_maintain));
						else if ((BYTE *)vptr == &testtrip)
						{
							if (testtrip)
								testtimes = test_times;
						}
						else if((BYTE *)vptr == &language)
						{
							WriteParaData (&language, EE_LANGUAGE, sizeof(language));
						}					
						else if (vptr == (void *)&p.tse_number)
						{
							tse_enabled = HSE_MARIN[p.tse_number];
						}
						else if (vptr == (void *)&p.exe_number)
						{
							exe_enabled = HSE_MARIN[p.exe_number];
						}
						else if (vptr == &p.relevel)
						{
							if (relev_active != p.relevel)
							{
								relev_active = p.relevel;
								relev_new = relev_active;
							}
						}
						else if((BYTE *)vptr == &p.ups_direction_mode)
							{
								if(p.ups_direction_mode & UPS_DIR_SEL)
									niv.F3_14 = 0;
								else									
									niv.F3_14 = 32;								
								send_nice5000_parameter(1609, niv.F3_14);
							}
						else if((BYTE *)vptr == &save_double_door)
							{//只有双门系统才配置此功能
								if(save_double_door)
									{
										config_double_door();		//配置双门系统	
										accesstimer = 60;	
									}
							}
						else if(((BYTE *)vptr == &p.door_connect) && (drive_state == D_STOPPED))		//只允许在停梯状态下才可以改动
							{
								if(*(BYTE *)vptr)
									{//层门在前，轿门在后
										p.hse_inpar [IO_SC2][IO_SUB_FUNC] = SC3;			//对应的门锁接线对调，因此同步更改
										p.hse_inpar [IO_SC3][IO_SUB_FUNC] = SC2;
									}
								else
									{//轿门在前，层门在后
										p.hse_inpar [IO_SC2][IO_SUB_FUNC] = SC2;
										p.hse_inpar [IO_SC3][IO_SUB_FUNC] = SC3;
									}
							}
						else if((BYTE *)vptr == &ucmp_state)
							ucmp_state = 0;
						else if(((BYTE *)vptr == &p.bot_floor) && 	(p.bot_floor != bot_floor_old))
							{
								bot_floor_old = p.bot_floor;
								config_botfloor_change();
							}
						menu = menu_old;
						new = 1;
						numberkey = 0;							/* enable direct access with xxx* again 		*/
					}
				}
				if (key == KEY_ESC)
				{
					menu = menu_old;
					new = 1;
					numberkey = 0;					/* enable direct access with xxx* again	*/
				}
			}
			else if (wait == 3)
			{
				if (iv_para_valid)
				{
					display_standardtext (T_WAIT);
					send_nice5000_parameter(address - NICE_INDEX, (WORD)value);
					wait = 4;
					displaytimer = timer + 3 SEC;
				}
				else
				{
					display_standardtext (T_ACK_OVERTIMER);
					wait = 1;
					displaytimer = timer + 3 SEC;
				}
			}
			else if(wait == 4)
			{
				if(!nice5000_Tstore_send)
				{
					switch (paratext [paranumber].size)
					{
						case (1):
							vptr = (BYTE *)paratext [paranumber].pointer + help [0];
							*((BYTE *)vptr) = value;
							break;
						case (2):
							vptr = (WORD *)paratext [paranumber].pointer + help [0];
							*((WORD *)vptr) = value;
							break;
						case (4):
							vptr = (DWORD *)paratext [paranumber].pointer + help [0];
							*((DWORD *)vptr) = value;
							break;
						case (5):
							vptr = (float *)paratext [paranumber].pointer + help [0];
							*((float *)vptr) = floatvalue;
							break;
					}
					
					if(vptr == &niv.FR_00)
						{
							if(correct_level != (BYTE)niv.FR_00)
								correct_level = (BYTE)niv.FR_00;
						}
					menu = menu_old;
					new = 1;
					numberkey = 0;
				}
				else if (displaytimer < timer)
				{
					display_standardtext (T_ACK_OVERTIMER);
					wait = 1;
					displaytimer = timer + 3 SEC;
				}
			}
			else
			{
				switch (key)
				{
					case (0):
					case (KEY_UP):
						break;

					case (KEY_ESC):
					case ('*'):
						menu = menu_old;
						new = 1;
						numberkey = 0;				/* enable direct access with xxx* again	*/
						break;

					case ('#'):
						if (numberpos)
						{
							sscanf ((const char *)buffer, "%lu", &value);
							if (paratext [paranumber].max)
								dwordvalue [0] = paratext [paranumber].max;
							else
								dwordvalue [0] = *((BYTE *)paratext [paranumber].tablepointer);
							if (paratext [paranumber].min != 0xFFFFFFFF)
								dwordvalue [1] = paratext [paranumber].min;
							else
								dwordvalue [1] = *((BYTE *)paratext [paranumber].minpointer);
							switch (menu)
							{
								case (M_FLOORPARA):
									dwordvalue [0]++;
									dwordvalue [1]++;
									break;
								case (M_LIFTPARA):
									dwordvalue [1]++;
									break;
							}
							if(paratext [paranumber].access == D_READ)
							{
								display_standardtext (T_NO_SUCCESS);
								wait = 1;
							}
							else if (value > dwordvalue [0])
							{
								display_standardtext (T_TOO_BIG);
								wait = 1;
							}
							else if (value < dwordvalue [1])
							{
								display_standardtext (T_TOO_SMALL);
								wait = 1;
							}
							else
							{
								switch (menu)
								{
									case (M_FLOORPARA):
									case (M_LIFTPARA):
										value -= 1;
										break;
									case (M_LIFTBITPARA):
										value = 1 << (value - 1);
										break;
								}
								display_standardtext (T_WAIT);
								if(menu_old >= M_NICE_F0)
									wait = 3;
								else
									wait = 2;
							}
						}
						else
						{
							menu = menu_old;
							new = 1;
							numberkey = 0;			/* enable direct access with xxx* again	*/
						}
						break;

					default:
						if (numberpos < 9)
						{
							buffer [numberpos++] = key;
							display (15 - strlen ((char *)buffer), row % 4, buffer);
						}
						break;
				}
			}
			break;

		case (M_SIGNPARA):							/* choose parameter value from list		*/
			if (new)
			{
				new = 0;
				cursor (_NORMALCURSOR);
				gotoxy (14, row % 4);
				wordvalue [0] = *((WORD *)paratext [paranumber].pointer + p.bot_floor + row);
				ioparabuffer [0] = wordvalue [0] >> 8;
				ioparabuffer [1] = wordvalue [0];
				if (!ioparabuffer [0])
					ioparabuffer [0] = ' ';
				if (!ioparabuffer [1])
					ioparabuffer [1] = ' ';
				ioparabuffer [2] = 0;
			}
			switch (key)
			{
				case ('*') :
					ioparabuffer [ioparabuffer [2]] = next_sign (ioparabuffer [ioparabuffer [2]], 1);
					break;

				case (KEY_UP) :
					ioparabuffer [ioparabuffer [2]] = next_sign (ioparabuffer [ioparabuffer [2]], 0);
					break;

				case (KEY_ESC) :
					ioparabuffer [2] = 0;
					menu = menu_old;
					new = 1;
					break;

				case ('#') :
					if (!ioparabuffer [2])
						ioparabuffer [2] = 1;
					else
					{
						ioparabuffer [2] = 0;
						vptr = (WORD *)paratext [paranumber].pointer + p.bot_floor + row;
						*((WORD *)vptr) = ((WORD)ioparabuffer [0] << 8) + ioparabuffer [1];
						floor_name_change(row);
					}
					break;
			}
			if (key)
			{
				sprintf ((char *)buffer, "%c%c", ioparabuffer [0], ioparabuffer [1]);
				display (16 - strlen ((char *)buffer), row % 4, buffer);
				gotoxy (14 + ioparabuffer [2], row % 4);
			}
			break;

		case (M_DOORNUMBERPARA):					/* door position parameter				*/
			if (new)
			{
				new = 0;
				cursor (_SOLIDCURSOR);
				if (paratext [paranumber].array)			/* array parameter					*/
					help [1] = row + p.bot_floor;		/* calculate offset					*/
				else									/* single parameter					*/
					help [1] = 0;						/* no offset							*/
				help [0] = *((BYTE *)paratext [paranumber].pointer + help [1]);
				numberkey = 1;						/* disable direct access with xxx*		*/
				wait = 0;
				gotoxy(15, row % 4);
			}
			if (wait)
			{
				if (!send_parameter (paratext [paranumber].pointer, help [1], help [0]))
				{
					vptr = (BYTE *)paratext [paranumber].pointer + help [1];
					*((BYTE *)vptr) = help [0];
					menu = menu_old;
					numberkey = 0;							/* enable direct access with xxx* again			*/
					new = 1;
				}
				if (key == KEY_ESC)
				{
					menu = menu_old;
					new = 1;
					numberkey = 0;					/* enable direct access with xxx* again	*/
				}
			}
			else
			{
				switch (key)
				{
					case (KEY_ESC):
					case ('*') :
						menu = menu_old;
						numberkey = 0;				/* enable direct access with xxx* again	*/
						new = 1;
					break;

					case ('#') :
						cursor(_NOCURSOR);
						wait = 1;
						display_standardtext (T_WAIT);
						break;

					default:
						if ((key >= '0') && (key <= '3'))
						{
							if (p.doornumber == 0x01 && key >= '2')
								break;
							buffer[0] = key;
							buffer[1] = '\0';
							help[0] = key - '0';
							display (15, row % 4, buffer);
							gotoxy(15, row % 4);
						}
						break;
				}
			}
			break;

		case (M_SAVEPARA):
			if (new)									/* start saving of all parameters		*/
				{
					display_standardtext (T_WAIT);
					new = 0;
					wait = 1;
					numberkey = 1;						/* disable direct access with xxx*		*/
				}
			if (wait == 1)									/* wait until other task is ready with EEPROM	*/
				{
					wait = 2;							/* start EEPROM writing							*/
					WriteParaData(&p, EE_PARAMETER, sizeof(p));
					WriteParaData(&password_admin, EE_PASSWORD0, sizeof(password_admin));
					WriteParaData(&password_help, EE_PASSWORD1, sizeof(password_help));
					WriteParaData(&password_install, EE_PASSWORD2, sizeof(password_install));
					WriteParaData(&password_maintain, EE_PASSWORD3, sizeof(password_maintain));
					WriteParaData(&tripcount, EE_TRIPCOUNT, sizeof(tripcount));
					WriteParaData(&tripcount_limit, EE_TRIPCOUNT_LIMIT, sizeof(tripcount_limit));
					WriteParaData(&op_time, EE_OPERTIME, sizeof(op_time));
					WriteParaData(&language, EE_LANGUAGE, sizeof(language));
					WriteParaData(&exe_para, EE_EXE_PARA, sizeof(exe_para));
					WriteParaData (floorcount, EE_FLOORCOUNT, sizeof (floorcount));
					para_valid = 1;
				}
			else if(wait == 2)
				{
					if (!iic_busy)			/* write complete parameter structure	*/
						{
							displaytimer = timer + 2 SEC;			/* trigger timer	*/
							wait = 0;
						}
				}
			else if(!wait && (displaytimer < timer))
				{
					menu = menu_old;
					new = 1;
					numberkey = 0;					/* enable direct access with xxx* again */
					sure = 0;
				}
			break;

		case (M_CL_HIST):
		case (M_CLEAR_COUNT):
			if (new)									/* start to clear error history			*/
			{
				display_standardtext (T_SURE);			/* Text: Are you sure ?				*/
				help [0] = 1;
				new = 0;
			}
			if (help [0])
			{
				if (key == '#')							/* User input for continue				*/
				{
					display_standardtext (T_WAIT);
					wait = 1;
					numberkey = 1;						/* disable direct access with xxx*		*/
					help [0] = 0;
				}
				else if (key)							/* User break						*/
				{
					menu = menu_old;
					new = 1;
					sure = 0;
					numberkey = 0;					/* enable direct access with xxx* again	*/
				}
			}
			else
			{
				if (wait)
				{
					wait = 0;								/* start EEPROM writing 						*/
					if (menu == M_CL_HIST)
					{
						memset(&errorhist, 0x00, sizeof(errorhist));
						errorhist.check = ERRORCOUNTCHECK;
						WriteParaData(&errorhist, EE_ERRHISTORY, sizeof(errorhist));
					}
					else
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
				}
				else
				{
					if (!iic_busy)	/* write complete parameter structure			*/
					{
						menu = menu_old;
						new = 1;
						sure = 0;
						numberkey = 0;				/* enable direct access with xxx* again		*/
					}
				}
			}
			break;

		case (M_EMPTY_LOAD):
		case (M_HALF_LOAD):
		case (M_FULL_LOAD):
		case (M_OVER_LOAD):
			if (new)
			{
				display_standardtext (T_RENDER_LOAD);
				GetLoadVoltageBuff(buf1, p.adlimit[row]);
				display (16 - strlen((char *)buf1), 0, buf1);
				cursor (_NOCURSOR);
				if (p.loadtype == ANALOG_ON_CDU)
				{
					buffer [0] = INIT_READ_REQ;
					buffer [1] = (BYTE)LOAD_VALUE;
					buffer [2] = LOAD_VALUE >> 8;
					buffer [3] = 1;
					for (i = 4; i < 8; i++)
						buffer [i] = 0;
				}
				help [2] = 0;
				help [3] = 0;
				help [5] = 1;
				load_value = 0xffff;
				wait = 0;
				new = 0;
			}
			if (!wait)
			{
				if (p.loadtype == ANALOG_ON_CDU)
				{
					if ((sdo_id == FVE_ID) && (*(WORD *)&sdo_data [1] == LOAD_VALUE))					/* response from FVE							*/
					{
						load_value = *(WORD *)&sdo_data [4];
						help [3] = 0;
					}
					else
					{
						if (help [3] < 40)
							help [3] ++;
						else
							load_value = 0xffff;
					}
				}
				else if (p.loadtype == ANALOG_ON_MCU)
				{
					load_value = ldvalue;
					help [3] = 0;
				}
				if ((load_value != help [2]) || (help [5]))
				{
					help [2] = load_value;
					help [5] = 0;
					GetLoadVoltageBuff(buf1, load_value);
					display(9, 1, "       ");
					display (16 - strlen((char *)buf1), 1, buf1);
				}
				if ((p.loadtype == ANALOG_ON_CDU) && (!sdo_master))							/* SDO transfer not used by other task			*/
				{
					sdo_interrupt = 1;						/* mark SDO transfer as used					*/
					sdo_id = 0xFFFF;						/* request actual load value					*/
					send_message (RSDO, FVE_ID, 8, buffer, CAN_A);
				}
				switch (key)
				{
					case ('#'):
						if (load_value != 0xffff)
						{
							p.adlimit[row] = load_value;
							display_standardtext (T_WAIT);
							wait = 1;
						}
						break;
					case (KEY_ESC):
						sdo_interrupt = 0;					/* release SDO transfer							*/
						menu_old = menu;
						menu = search_mainmenu (menu);		/* search higher menu level						*/
						new = 1;
						break;
				}
			}
			else if (wait == 1)
			{
				if (p.loadtype == ANALOG_ON_CDU)
				{
					if (!sdo_master)							/* SDO transfer not used by other task			*/
					{
						sdo_interrupt = 1;						/* mark SDO transfer as used					*/
						sdo_id = 0xFFFF;						/* send calculated value						*/
						buffer [0] = INIT_WRITE_REQ | EXPEDITED_BIT;
						buffer [1] = (BYTE)LOAD_SIG_LIMIT;
						buffer [2] = LOAD_SIG_LIMIT >> 8;
						buffer [3] = row+1;
						*(WORD *)&buffer [4] = p.adlimit[row];
						send_message (RSDO, FVE_ID, 8, buffer, CAN_A);
						displaytimer = timer + 2 SEC;				/* waiting time 2 s								*/
					}
				}
				wait = 2;								/* wait for response							*/
			}
			else if (wait == 2)
			{
				if (p.loadtype == ANALOG_ON_CDU)
				{
					if ((sdo_id == FVE_ID) ||					/* response from FVE							*/
					(displaytimer < timer))				/* or waiting time out							*/
					{
						sdo_interrupt = 0;						/* release SDO transfer							*/
						if (sdo_id == FVE_ID)
							display_standardtext (T_SUCCESS);		/* setting successfull							*/
						else
							display_standardtext (T_NO_SUCCESS);	/* setting not possible in the moment			*/
						wait = 3;
						displaytimer = timer + 4 SEC;
					}
				}
				else
				{
					display_standardtext (T_SUCCESS);
					wait = 3;
					displaytimer = timer + 4 SEC;
				}
			}
			else if (wait == 3)							/* finish menu by timer or by any key			*/
			{
				if (key || (displaytimer < timer))
				{
					menu_old = menu;
					menu = search_mainmenu (menu);			/* search higher menu level						*/
					new = 1;
				}
			}
			break;

		case (M_SET_TIME):
			if (new)
			{
				display_standardtext (T_WAIT);
				wait = 1;
				new = 0;
			}
			if (wait)
			{
				if (!settimeflag)
				{
					display_standardtext (T_SUCCESS);
					displaytimer = timer + 4 SEC;
					wait = 0;
				}
			}
			else
			{
				if (key || (displaytimer < timer))
				{
					menu_old = M_TIME;
					menu = search_submenu (menu, 0, &right);
					new = 1;
				}
			}
			break;

		case (M_CLR_TRIPCOUNT):
		case (M_CLR_HOURCOUNT):
		case (M_CLR_FLOORCNT):
			if (new)
			{
				display_standardtext (T_SURE);			/* Text: Are you sure ?					*/
				help [0] = 1;
				new = 0;
			}
			if (help [0])
			{
				if (key == '#')							/* User input for continue					*/
				{
					switch (menu)
					{
						case (M_CLR_TRIPCOUNT):		/* clear trip counter						*/
							tripcount = 0;
							WriteParaData (&tripcount, EE_TRIPCOUNT, sizeof (tripcount));
							break;

						case (M_CLR_HOURCOUNT):	/* clear operational time counter			*/
							op_time = 0;
							WriteParaData (&op_time, EE_OPERTIME, sizeof (op_time));
							break;
						case (M_CLR_FLOORCNT):					/* clear floor counter (number of trips)		*/
							for (i = 0; i < MAX_FLOOR; i++)
								floorcount [i] = 0;
							WriteParaData (floorcount, EE_FLOORCOUNT, sizeof (floorcount));
							break;
					}
					display_standardtext (T_SUCCESS);
					displaytimer = timer + 4 SEC;
					help [0] = 0;
				}
				else if (key)							/* user break							*/
				{
					menu = menu_old;
					new = 1;
					sure = 0;
				}
			}
			else
			{
				if (key || (displaytimer < timer))
				{
					menu = menu_old;
					new = 1;
				}
			}
			break;

		case (M_MOVE):
			if (new)
			{
				lcdram = 0;
				display_standardtext (T_MOVE);
				term_active = 1;						/* set terminal mode						*/
				numberkey = 1;						/* disable direct acces with xxx*			*/
				new = 0;
				help [1] = 0xFF;
				help[2] = ~door_zone;
				help[3] = ~level;
			}
			if (help[3] != level)
			{
				help[3] = level;
				if (help[3] != 0xFF)
					sprintf ((char *)buffer, "%2u", help[3] + 1);
				else
					sprintf ((char *)buffer, "??");
				display (1, 1, buffer);
			}

			if (help [2] != door_zone)
			{
				help [2] = door_zone;
				if (help [2] & POS_SGM)
					buffer [0] = 0xFF;
				else
					buffer [0] = 8;

				if ((help [2] & (POS_SGD | POS_SGU)) == (POS_SGD | POS_SGU))
					buffer [1] = 0xff;
				else if (help [2] & POS_SGD)
					buffer [1] = 2;
				else if (help [2] & POS_SGU)
					buffer [1] = 1;
				else
					buffer [1] = 8;

				if ((help [2] & (POS_ILU | POS_ILD)) == (POS_ILU | POS_ILD))
					buffer [2] = 0xff;
				else if (help [2] & POS_ILD)
					buffer [2] = 2;
				else if (help [2] & POS_ILU)
					buffer [2] = 1;
				else
					buffer [2] = 8;

				if ((help[2] & (POS_SDU1 | POS_SDD1)) == (POS_SDU1 | POS_SDD1))
					buffer[5] = 0xff;
				else if (help[2] & POS_SDU1)
					buffer[5] = 1;
				else if (help[2] & POS_SDD1)
					buffer[5] = 2;
				else
					buffer[5] = 8;
			
				if ((help[2] & (POS_SDU2 | POS_SDD2)) == (POS_SDU2 | POS_SDD2))
					buffer[4] = 0xff;
				else if (help[2] & POS_SDU2)
					buffer[4] = 1;
				else if (help[2] & POS_SDD2)
					buffer[4] = 2;
				else
					buffer[4] = 8;
			
				if ((help[2] & (POS_SDU3 | POS_SDD3)) == (POS_SDU3 | POS_SDD3))
					buffer[3] = 0xff;
				else if (help[2] & POS_SDU3)
					buffer[3] = 1;
				else if (help[2] & POS_SDD3)
					buffer[3] = 2;
				else
					buffer[3] = 8;

				buffer [6] = 0;
				display (10, 1, buffer);
			}

			switch (key)
			{
				case (KEY_ESC):
					term_active = 0;
					numberkey = 0;					/* enable direct access with xxx* again		*/
					menu_old = menu;							/* set old menu									*/
					menu = search_mainmenu (M_MOVE);	/* search higher menu level				*/
					new = 1;
					break;

				case (KEY_UP):
					term_active = 3;		//up direction
					break;
					
				case ('*'):
					term_active = 5;		//down direction
					break;
					
				case ('0'):						
					term_active = 7;		//slow stop 
					break;
					
				case ('#'):
					term_active = 1;	//return 
					break;
			}
			if (!(safety_circuit & SC3)) 				/* safety circuit off						*/
				help [0] = T_MOVEERROR1;				/* set error text safety circuit off			*/
			else if (hb_error & B_DRV)					/* no heartbeat from drive					*/
				help [0] = T_MOVEERROR2;				/* set error text heartbeat error drive		*/
			else if (driveerror & (D_BRAKE_NOTON | D_BRAKE_NOTOFF))			/* Brake is not opened					*/
				help [0] = T_MOVEERROR3;				/* set error text brake error				*/
			else if (driveerror & D_NICE_MOVE_NOTOFF)	/* Lift is not stopped in max waiting time	*/
				help [0] = T_MOVEERROR4;				/* set error text brake not closed			*/
			else if (driveerror & (D_RUNCONT_NOTON | D_RUNCONT_NOTOFF))		/* Contactors are not on					*/
				help [0] = T_MOVEERROR5;				/* set error text contactor error				*/
			else if (driveerror & D_EMERG_STOP)			/* Drive is in error state					*/
				help [0] = T_MOVEERROR6;				/* set error text general drive error			*/
			else
				help [0] = 0;							/* no error								*/
			if (help [0] != help [1])						/* error state changed						*/
			{
				display_line (help [0], 2);
				help [1] = help [0];						/* save old error state						*/
				if(help[0])
					{//故障状态下退出终端模式	
						term_active = 0;
						new = 1;
						menu_old = menu;
						menu = M_STATUS1;
					}
			}
			passwordtimer = PASSWORDTIME;
			break;

		case (M_CHANGE_IOPARA):
			switch (key)
			{
				case (KEY_ESC):
				case (0):
						break;

				case ('#'):
					check_min_max (ioparabuffer, date);
					menusize = iopara_detail (ioparabuffer, row, date, bus);
					cursor (_SOLIDCURSOR);
					gotoxy (0, row % 4);
					menu = M_IOPARA;
					numberkey = 0;					/* enable direct acces with xxx* again		*/
					new = 0;
					break;

				default:
					change_iopara (ioparabuffer, row, date, key, new, bus);
					new = 0;
					iopara_detail (ioparabuffer, row, date, bus);
					gotoxy (15, row % 4);
					break;
			}
			break;

		case (M_IOPARA):
			if (new)
			{
				new = 0;
				wait = 1;
				i = check_iopara (paratype, paranumber, pcb_number);
				if (i == OUTPUT)
					display_standardtext (T_OUTPARA);
				else if (i == INPUT)
					display_standardtext (T_INPARA);
				else
					key = '#';
			}
			if (wait == 1)								/* wait for user: warning that io is used		*/
			{
				switch (key)
				{
					case (0):
						break;

					case ('#'):
						wait = 0;
						row = 0;
						lcdram = 1;
						cursor (_SOLIDCURSOR);
						date = get_iopara (paratype, paranumber, ioparabuffer, pcb_number);
						menusize = iopara_detail (ioparabuffer, row, date, bus);
						break;

					default:
						menu = menu_old;
						menu_old = M_IOPARA;
						new = 1;
						numberkey = 0;				/* enable direct acces with xxx* again		*/
						break;
				}
			}
			else if (wait == 2)							/* wait for user: change not possible		*/
			{
				if (!send_write_request (paratype, paranumber, pcb_number, bus, 0))
					wait = 3;
				if (key == KEY_ESC)
				{
					menu = menu_old;
					menu_old = M_IOPARA;
					new = 1;
					lcdram = 0;
					numberkey = 0;					/* enable direct acces with xxx* again		*/
				}
			}
			else if (wait == 3)							/* CAN send io data						*/
			{
				if ((paratype != M_IN_HSE) && (paratype != M_OUT_HSE))
					send_iodata (ioparabuffer, pcb_number, bus);
				wait = 4;
			}
			else if (wait == 4)							/* CAN send io polarity					*/
			{
				if (paratype == M_IN_HSE)
				{
					if (ioparabuffer [IO_ENABLE] & ENABLE)
						p.hse_inpar[paranumber][IO_ENABLE] = ENABLE;
					else
						p.hse_inpar[paranumber][IO_ENABLE] = DISABLE;

					if (paranumber == INPUT_SDU1 || paranumber == INPUT_SDD1 || 
						paranumber == INPUT_SDU2 || paranumber == INPUT_SDD2 || 
						paranumber == INPUT_SDU3 || paranumber == INPUT_SDD3)
					{
						switch (paranumber)
						{
							case INPUT_SDU1:
								vptr = &p.F5_X3;
								break;
							case INPUT_SDD1:
								vptr = &p.F5_X4;
								break;
							case INPUT_SDU2:
								vptr = &p.F5_X5;
								break;
							case INPUT_SDD2:
								vptr = &p.F5_X6;
								break;
							case INPUT_SDU3:
								vptr = &p.F5_X7;
								break;
							case INPUT_SDD3:
								vptr = &p.F5_X8;
								break;
						}
						if ((ioparabuffer[IO_ENABLE] & ENABLE) &&
							(ioparabuffer[IO_BASIC_FUNC] == FORCE_SWITCH))
						{
							switch (ioparabuffer[IO_SUB_FUNC])
							{
								case FORCE_SDU1:
									*((WORD *)vptr) = NICE5000_INPUT_SDU1;
									break;
								case FORCE_SDD1:
									*((WORD *)vptr) = NICE5000_INPUT_SDD1;
									break;
								case FORCE_SDU2:
									*((WORD *)vptr) = NICE5000_INPUT_SDU2;
									break;
								case FORCE_SDD2:
									*((WORD *)vptr) = NICE5000_INPUT_SDD2;
									break;
								case FORCE_SDU3:
									*((WORD *)vptr) = NICE5000_INPUT_SDU3;
									break;
								case FORCE_SDD3:
									*((WORD *)vptr) = NICE5000_INPUT_SDD3;
									break;
								default:
									*((WORD *)vptr) = 0;
									break;
							}
						}
						else
							*((WORD *)vptr) = 0;
						if ((ioparabuffer [IO_STATE] & POLARITY_BIT) && (*((WORD *)vptr)))
							*((WORD *)vptr) += NICE5000_INPUT_LOGIC;
						force_switch = 0;
						oldin = (~in) & HSE_MARIN[MAX_HSE_IN+MAX_HSE_EX_IN];
						init_error |= B_DRV;
					}
				}
				else if (paratype == M_OUT_HSE)
				{
					if (ioparabuffer [IO_ENABLE] & ENABLE)
						p.hse_outpar[paranumber][IO_ENABLE] = ENABLE;
					else
						p.hse_outpar[paranumber][IO_ENABLE] = DISABLE;
				}
				else
				{
					send_ioenable (paratype, paranumber, pcb_number, ioparabuffer [IO_ENABLE], bus, 0);
				}
				wait = 10;

			}
			else if (wait == 5)							/* wait for user to set call acknowledge		*/
			{
				switch (key)
				{
					case (0):
						break;

					case ('#'):
						set_callpara (paratype, paranumber, pcb_number);
						wait = 6;
						break;

					default:
						menu = menu_old;
						menu_old = M_IOPARA;
						new = 1;
						lcdram = 0;
						numberkey = 0;				/* enable direct acces with xxx* again		*/
						break;
				}
			}
			else if (wait == 6)							/* CAN write request						*/
			{
				send_write_request (paratype, paranumber, pcb_number, bus, 1);
				wait = 7;
			}
			else if (wait == 7)							/* CAN send IO data						*/
			{
				if ((paratype != M_IN_HSE) && (paratype != M_OUT_HSE))
					send_iodata (ioparabuffer, pcb_number, bus);
				wait = 8;
			}
			else if (wait == 8)							/* CAN send polarity						*/
			{
				if ((paratype != M_IN_HSE) && (paratype != M_OUT_HSE))
					send_ioenable (paratype, paranumber, pcb_number, ioparabuffer [IO_ENABLE], bus, 1);
				wait = 11;
			}
			else if (wait == 9)
			{
				if ((displaytimer < timer) || key)
					new = 1;
			}
			else if(wait == 10)
			{
				if (paratype == M_IN_HSE)
				{
					if (ioparabuffer [IO_STATE] & POLARITY_BIT)
						bit_set (in_polarity, paranumber);
					else
						bit_reset (in_polarity, paranumber);
				}
				else if (paratype == M_OUT_HSE)
				{
					if (ioparabuffer [IO_STATE] & POLARITY_BIT)
						bit_set (out_polarity, paranumber);
					else
						bit_reset (out_polarity, paranumber);
				}
				else
				{
					send_iopolarity (paratype, paranumber, pcb_number, ioparabuffer [IO_STATE], bus, 0);
				}

				i = write_iopara (paratype, paranumber, ioparabuffer, pcb_number);
				if (i)
				{
					switch (i)
					{
						case (INPUT):
							display_standardtext (T_SET_QUIT);
							break;
						case (OUTPUT):
							display_standardtext (T_SET_CALL);
							break;
						case (INPUT+2):
							display_standardtext (T_CLEAR_OUTPUT);
							break;
						case (OUTPUT+2):
							display_standardtext (T_CLEAR_INPUT);
							break;
					}
					wait = 5;
				}
				else
				{
					menu = menu_old;
					menu_old = M_IOPARA;
					new = 1;
					lcdram = 0;
					numberkey = 0;					/* enable direct acces with xxx* again		*/
				}
			}
			else if(wait == 11)
			{
				if ((paratype != M_IN_HSE) && (paratype != M_OUT_HSE))
					send_iopolarity (paratype, paranumber, pcb_number, ioparabuffer [5], bus, 1);
				menu = menu_old;
				menu_old = M_IOPARA;
				new = 1;
				lcdram = 0;
				numberkey = 0;						/* enable direct acces with xxx* again		*/
			}
			else
			{
				switch (key)
				{
					case ('*'):
						if (row < (menusize - 1))
							row++;
						else
							row = 0;
						if (!(row % 4))
							iopara_detail (ioparabuffer, row, date, bus);
						else
							gotoxy (0, row % 4);
						break;

					case (KEY_ESC):
						menu = menu_old;
						menu_old = M_IOPARA;
						new = 1;
						lcdram = 0;
						numberkey = 0;				/* enable direct acces with xxx* again		*/
						break;

					case (KEY_UP):
						if (row)
							row--;
						else
							row = menusize - 1;
						if ((!((row+1) % 4)) || (row == menusize - 1))
							iopara_detail (ioparabuffer, row, date, bus);
						else
							gotoxy (0, row % 4);
						break;

					case ('#'):
						if (row < (menusize - 2))			/* enter new parameter values				*/
						{
							if (password_right <= RIGHT_INSTALL)
							{
								menu = M_CHANGE_IOPARA;
								numberkey = check_accesstype (ioparabuffer, row, date, bus);
								cursor (_NORMALCURSOR);
								gotoxy (15, row % 4);
								new = 1;
							}
							else
							{
								wait = 9;
								display_standardtext (T_DENY_ACCESS);
								displaytimer = timer + 3 SEC;
							}
						}
						else
						{
							if (row == (menusize - 2))		/* change parameter (save command)		*/
							{
								if (password_right <= RIGHT_INSTALL)
								{
									wait = 2;
									display_standardtext (T_WAIT);
								}
								else
								{
									wait = 9;
									display_standardtext (T_DENY_ACCESS);
									displaytimer = timer + 3 SEC;
								}
							}
							else
							{
								menu = menu_old;
								menu_old = M_IOPARA;
								new = 1;
								lcdram = 0;
								numberkey = 0;		/* enable direct acces with xxx* again		*/
							}
						}
						break;
				}
			}
			break;

		case (M_IN_HSE):
		case (M_OUT_HSE):
		case (M_IN_FVE):
		case (M_OUT_FVE):
		case (M_IN_TSE):
		case (M_OUT_TSE):
		case (M_IN_ESE):
		case (M_OUT_ESE):
		case (M_IN_EXE):
		case (M_OUT_EXE):
			if (new)
			{
				if (menu_old != M_IOPARA)
					row = 0;
				else
					row = paranumber;
				menusize = display_io (menu, row, pcb_number);
				cursor (_SOLIDCURSOR);
				new = 0;
			}
			switch (key)
			{
				case ('*'):
					if (row < (menusize - 1))
						row ++;
					else
						row = 0;
					if (!(row % 4))
						display_io (menu, row, pcb_number);
					else
						gotoxy (0, row % 4);
					break;

				case (KEY_UP):
					if (row)
						row --;
					else
						row = menusize - 1;
					if ((!((row+1) % 4)) || (row == menusize - 1))
						display_io (menu, row, pcb_number);
					else
						gotoxy (0, row % 4);
					break;

				case (KEY_ESC):
					menu_old = menu;
					if (menu == M_IN_TSE)
						menu = M_TSE_IN_LIST;
					else if (menu == M_OUT_TSE)
						menu = M_TSE_OUT_LIST;
					else if (menu == M_IN_ESE)
						menu = M_ESE_IN_LIST;
					else if (menu == M_OUT_ESE)
						menu = M_ESE_OUT_LIST;
					else if (menu == M_IN_EXE)
						menu = M_EXE_IN_LIST;
					else if (menu == M_OUT_EXE)
						menu = M_EXE_OUT_LIST;
					else
						menu = search_mainmenu (menu);
					new = 1;
					break;

				case ('#'):
					if (row == (menusize - 1))
					{
						menu_old = menu;
						menu = search_mainmenu (menu);
					}
					else
					{
						if ((menu == M_IN_ESE) || (menu == M_OUT_ESE))
							bus = 1;
						else
							bus = 0;
						menu_old = menu;
						menu = M_IOPARA;
						paranumber = row;
						paratype = menu_old;
					}
					new = 1;
					break;
			}
			break;

		case (M_TSE):
		case (M_ESE):
		case (M_TSE_IN_LIST):
		case (M_TSE_OUT_LIST):
		case (M_ESE_IN_LIST):
		case (M_ESE_OUT_LIST):
		case (M_EXE):
		case (M_EXE_IN_LIST):
		case (M_EXE_OUT_LIST):
			if (new)
			{
				new = 0;
				wait = 0;
				if ((!p.tse_number) && (menu == M_TSE || menu == M_TSE_IN_LIST || menu == M_TSE_OUT_LIST))
				{
					display_standardtext (T_TSE_NOTALLOCATE);
					displaytimer = timer + 4 SEC;
					wait = 1;
				}
				else if ((!p.exe_number) && (menu == M_EXE || menu == M_EXE_IN_LIST || menu == M_EXE_OUT_LIST))
				{
					display_standardtext (T_EXE_NOTALLOCATE);
					displaytimer = timer + 4 SEC;
					wait = 1;
				}
				else if ((!p.ese_number) && (menu == M_ESE || menu == M_ESE_IN_LIST || menu == M_ESE_OUT_LIST))
				{
					display_standardtext (T_ESE_NOTALLOCATE);
					displaytimer = timer + 4 SEC;
					wait = 1;
				}
				else
				{
					if ((menu_old == M_TSE_INFO || menu_old == M_ESE_INFO) ||
							(menu_old == M_IN_TSE || menu_old == M_OUT_TSE) ||
							(menu_old == M_IN_ESE || menu_old == M_OUT_ESE) || 
							(menu_old == M_IN_EXE || menu_old == M_OUT_EXE || menu_old == M_EXE_INFO))
						row = pcb_number;
					else
						row = 0;
					if (menu == M_TSE || menu == M_TSE_IN_LIST || menu == M_TSE_OUT_LIST)
						menusize = p.tse_number + 1;
					else if (menu == M_EXE || menu == M_EXE_IN_LIST || menu == M_EXE_OUT_LIST)
						menusize = p.exe_number + 1;
					else
						menusize = p.ese_number + 1;
					display_pcb_number (menu, row, menusize);
					gotoxy (0, row % MAX_LCD_ROW);
					if (menu == M_TSE || menu == M_ESE || menu == M_EXE)
						help[0] = 1;
				}
			}
			if (wait)
			{
				if (key || (displaytimer < timer))
				{
					menu_old = menu;
					menu = search_mainmenu (menu);
					new = 1;
				}
			}
			else
			{
				if (menu == M_TSE || menu == M_ESE || menu == M_EXE)
				{
					if (help[0])
					{
						help[2] = get_pcb_hb_status(menu, row, menusize);
						help[0] = 0;
						help[3] = 1;
					}
					else
					{
						help[1] = get_pcb_hb_status(menu, row, menusize);
						if (help[2] != help[1])
						{
							help[2] = help[1];
							help[3] = 1;
						}
					}
					if (help[3])
					{
						help[3] = 0;
						display_pcb_status(help[2], row, menusize);
					}
				}
				switch (key)
				{
					case ('*'):
						if (row < menusize-1)
							row++;
						else
							row = 0;
						if (!(row % MAX_LCD_ROW) && (menusize > MAX_LCD_ROW))
						{
							display_pcb_number (menu, row, menusize);
							if (menu == M_TSE || menu == M_ESE || menu == M_EXE)
								help[0] = 1;
						}
						gotoxy (0, row % MAX_LCD_ROW);
						break;

					case (KEY_UP):
						if (row)
							row--;
						else
							row = menusize-1;
						if ((menusize > MAX_LCD_ROW) && ((row == menusize-1) || !((row+1)%MAX_LCD_ROW)))
						{
							display_pcb_number (menu, row, menusize);
							if (menu == M_TSE || menu == M_ESE || menu == M_EXE)
								help[0] = 1;
						}
						gotoxy (0, row % MAX_LCD_ROW);
						break;

					case (KEY_ESC):
						menu_old = menu;
						menu = search_mainmenu (menu);
						new = 1;
						break;

					case ('#'):
						if (row == (menusize - 1))
						{
							menu_old = menu;
							menu = search_mainmenu (menu);
						}
						else
						{
							if (menu == M_TSE)
							{
								if ((help[2] & c_bit_select[row % MAX_LCD_ROW]) && (!tse_sdo_master))
									menu = M_TSE_INFO;
								else
									break;
							}
							else if (menu == M_EXE)
							{
								if ((help[2] & c_bit_select[row % MAX_LCD_ROW]) && (!exe_sdo_master))
									menu = M_EXE_INFO;
								else
									break;
							}
							else if (menu == M_ESE)
							{
								if ((help[2] & c_bit_select[row % MAX_LCD_ROW]) && (!ese_sdo_master))
									menu = M_ESE_INFO;
								else
									break;
							}
							else if (menu == M_TSE_IN_LIST)
								menu = M_IN_TSE;
							else if (menu == M_TSE_OUT_LIST)
								menu = M_OUT_TSE;
							else if (menu == M_ESE_IN_LIST)
								menu = M_IN_ESE;
							else if (menu == M_ESE_OUT_LIST)
								menu = M_OUT_ESE;
							else if (menu == M_EXE_IN_LIST)
								menu = M_IN_EXE;
							else if (menu == M_EXE_OUT_LIST)
								menu = M_OUT_EXE;
							menu_old = menu;
							pcb_number = row;
							row = 0;
						}
						new = 1;
						break;
				}
			}

			break;
		case M_BASIC_SAVE:
			set_parameter();
			new = 0;
			sure = 1;
			menu_old = M_MAIN;
			write_errorhist (E_BASIC_SETTING, HSE_ID, 0, 0);
			savepara = 1;
			numberkey = 0;
			break;

		case (M_CAR_CALLS):
			if (new)
			{
				display_standardtext (T_CAR_CALL);
				numberkey = 1;								/* disable direct access with xxx*				*/
				help [0] = ~level;
				help [1] = 0;
				help [3] = 0;
				if (calls_enabled & CARCALL)
					help [4] = 0;
				else
					help [4] = 1;
				help [5] = 1;
				buffer [0] = buffer [1] = buffer [2] = 0;
				cursor (_NORMALCURSOR);
				new = 0;
			}
			if (help [0] != level)
			{
				help [0] = level;
				if (level == 0xFF)
					display (14, 0, "??");
				else
				{
					sprintf ((char *)buf1, "%2u", help [0] + 1);
					display (14, 0, buf1);
				}
				help [5] = 1;
			}
			if (help [4] && (!(calls_enabled & CARCALL)))
			{
				help [4] = 0;
				display_line (T_CALLS_DISABLED, 3);
				help [5] = 1;
			}
			else if ((!help [4]) && (calls_enabled & CARCALL))
			{
				help [4] = 1;
				clearline (3);
				help [5] = 1;
			}
			switch (key)
			{
				case ('#'):
					sscanf ((const char *)buffer, "%u", (DWORD *)&help [2]);
					help [2]--;						/* Umrechnen der Etage							*/
					if ((help [2] >= p.bot_floor) && (help [2] <= p.top_floor) && (calls_enabled & CARCALL))
					{								/* valid call floor								*/
						calltab [help [2]].calltype |= CARCALL;
						calltab [help [2]].cc_door |= p.doorpos [help [2]];
						display_line (T_CALL_READY, 2);
						displaytimer = timer + 3 SEC;
						help [3] = 1;
					}
					help [1] = 0;
					buffer [0] = buffer [1] = buffer [2] = 0;
					display (14, 1, "  ");
					help [5] = 1;
					break;
				case (KEY_ESC):
					if (!help [1])
					{
						menu = menu_old;
						new = 1;
						numberkey = 0;				/* enable direct access with xxx* again			*/
					}
					else
					{
						help [1]--;
						display (14 + help [1], 1, " ");
						help [5] = 1;
					}
					break;
				case ('0'):
				case ('1'):
				case ('2'):
				case ('3'):
				case ('4'):
				case ('5'):
				case ('6'):
				case ('7'):
				case ('8'):
				case ('9'):
					if (help [1] < 2)
					{
						buffer [help [1]] = key;
						display (14, 1, buffer);
						help [1]++;
						help [5] = 1;
					}
			}
			if (help [3] && (displaytimer < timer))
			{
				help [3] = 0;
				clearline (2);
				help [5] = 1;
			}
			if (help [5])
			{
				help [5] = 0;
				if (help [1])
					gotoxy (15, 1);
				else
					gotoxy (14, 1);
			}
			break;

		case (M_DISP_ID):
			if (new)
			{
				display_standardtext(T_DISP_ID);
				numberkey = 1;
				new = 0;
				wait = 1;
				displaytimer = timer + 1 SEC;
			}
			if (wait)
			{
				buffer[0] = DISP_NODE_ID;
				send_message(LSS, LSS_REQ_ID, 1, buffer, CAN_B);
				displaytimer = timer + 1 SEC;
				wait = 0;
			}
			if (displaytimer < timer)
				wait = 1;
			if (key == '#')
			{
				buffer[0] = ABORT_NODE_ID;
				send_message(LSS, LSS_REQ_ID, 1, buffer, CAN_B);
				menu = menu_old;
				numberkey = 0;
				new = 1;
			}
			passwordtimer = PASSWORDTIME;
			break;

		case (M_SET_ID_1):
			if (new)
			{
				new = 0;
				if (!p.ese_number)
				{
					display_standardtext(T_ESE_NOTALLOCATE);
					displaytimer = timer + 3 SEC;
					wait = 2;
				}
				else
				{
					display_standardtext(T_DISP_ID);
					numberkey = 1;
					wait = 1;
					displaytimer = timer + 1 SEC;
					buf1[0] = 0;
					GetIDDispBuffer(buf1[0], buffer);
				}
			}
			if (wait == 2)
			{
				if (key || (displaytimer < timer))
				{
					menu = menu_old;
					numberkey = 0;
					new = 1;
				}
			}
			else
			{
				if (lss_res == buffer[1])
				{
					lss_res = 0;
					wait = 1;
					++ buf1[0];
					GetIDDispBuffer(buf1[0], buffer);
				}
				if (key == '#' || buf1[0] >= p.ese_number)
				{
					buffer[0] = ABORT_NODE_ID;
					send_message(LSS, LSS_REQ_ID, 1, buffer, CAN_B);
					if (key == '#')
						display_standardtext(T_ABORT_ID);
					else
						display_standardtext(T_SUCCESS);
					displaytimer = timer + 3 SEC;
					wait = 2;
				}
				else if (displaytimer < timer)
					wait = 1;
				if (wait == 1)
				{
					send_message(LSS, LSS_REQ_ID, 5, buffer, CAN_B);
					displaytimer = timer + 1 SEC;
					wait = 0;
				}
			}
			passwordtimer = PASSWORDTIME;
			break;

		case (M_SET_ID_2):
			if (new)
			{
				new = 0;
				if (!p.ese_number)
				{
					display_standardtext(T_ESE_NOTALLOCATE);
					displaytimer = timer + 3 SEC;
					wait = 2;
				}
				else
				{
					display_standardtext(T_DISP_ID);
					buffer[0] = SET_NODE_ID2;
					send_message(LSS, LSS_REQ_ID, 1, buffer, CAN_B);
					numberkey = 1;
					wait = 1;
				}
			}
			if (wait == 2)
			{
				if (key || (displaytimer < timer))
				{
					menu = menu_old;
					numberkey = 0;
					new = 1;
				}
			}
			else
			{
				if (key == '#')
				{
					buffer[0] = ABORT_NODE_ID;
					send_message(LSS, LSS_REQ_ID, 1, buffer, CAN_B);
					display_standardtext(T_ABORT_ID);
					displaytimer = timer + 3 SEC;
					wait = 2;
				}
			}
			passwordtimer = PASSWORDTIME;
			break;

		case (M_1BIT_DECIMAL):
		case (M_2BIT_DECIMAL):
		case (M_3BIT_DECIMAL):
		case (M_FLOOR_LEVEL):
			if (new)
			{
				wait = 0;
				new = 0;
				clearline (row % 4);
				display (0, row % 4, (BYTE *)paratext [paranumber].text [language]);
				help[1] = 0;
				switch (paratext [paranumber].size)
				{
					case 1:
						value = *(BYTE *)paratext [paranumber].pointer;
						help[0] = 4;
						if (menu == M_1BIT_DECIMAL)
						{
							help[2] = 0;
							sprintf ((char *)buffer, "%d%d.%d", value/100, (value%100)/10, value%10);
						}
						else if (menu == M_2BIT_DECIMAL)
						{
							help[2] = 1;
							sprintf ((char *)buffer, "%d.%d%d", value/100, (value%100)/10, value%10);
						}
						else if (menu == M_3BIT_DECIMAL)
						{
							help[2] = 2;
							help[0] = 5;
							sprintf ((char *)buffer, "0.%d%d%d", value/100, (value%100)/10, value%10);
						}
						else
							{
								help[2] = 0;
								sprintf ((char *)buffer, "%05d", value);
							}
						break;
					case 2:
						value = *(WORD *)paratext [paranumber].pointer;
						help[0] = 6;
						if (menu == M_1BIT_DECIMAL)
						{
							help[0] = 6;
							help[2] = 0;
							sprintf ((char *)buffer, "%d%d%d%d.%d", value/10000, (value%10000)/1000, (value%1000)/100, (value%100)/10, value%10);
						}
						else if (menu == M_2BIT_DECIMAL)
						{
							help[0] = 6;
							help[2] = 1;
							sprintf ((char *)buffer, "%d%d%d.%d%d", value/10000, (value%10000)/1000, (value%1000)/100, (value%100)/10, value%10);
						}
						else if (menu == M_3BIT_DECIMAL)
						{
							help[0] = 6;
							help[2] = 2;
							sprintf ((char *)buffer, "%d%d.%d%d%d", value/10000, (value%10000)/1000, (value%1000)/100, (value%100)/10, value%10);
						}
						else
							{
								help[0] = 5;
								help[2] = 0;
								sprintf ((char *)buffer, "%05d", value);
							}
						break;
				}
				display (16 - strlen ((char *)buffer), row % 4, buffer);
				cursortype	= _NORMALCURSOR;
				gotoxy (help[1]+16-strlen ((char *)buffer), row % 4);
				numberkey = 1;
			}
			if (wait == 1)
			{
				if ((key) || (displaytimer < timer))
				{
					menu = menu_old;
					new = 1;
					numberkey = 0;
				}
			}
			else if (wait == 2)
			{
				if (iv_para_valid)
				{
					display_standardtext (T_WAIT);
					send_nice5000_parameter(address - NICE_INDEX, (WORD)value);
					wait = 3;
					displaytimer = timer + 2 SEC;
				}
				else
				{
					display_standardtext (T_ACK_OVERTIMER);
					displaytimer = timer + 3 SEC;
					wait = 1;
				}
			}
			else if (wait == 3)
			{
				if(!nice5000_Tstore_send)
				{
					switch (paratext [paranumber].size)
					{
						case (1):
							vptr = (BYTE *)paratext [paranumber].pointer;
							*((BYTE *)vptr) = value;
							break;
						case (2):
							vptr = (WORD *)paratext [paranumber].pointer;
							*((WORD *)vptr) = value;
							break;
						case (4):
							vptr = (DWORD *)paratext [paranumber].pointer;
							*((DWORD *)vptr) = value;
							break;
					}
					menu = menu_old;
					new = 1;
					numberkey = 0;
				}
				else if (displaytimer < timer)
				{
					display_standardtext (T_ACK_OVERTIMER);
					wait = 1;
					displaytimer = timer + 3 SEC;
				}
			}
			else
			{
				switch (key)
				{
					case (KEY_UP):
						++ help[1];
						if (help[1] == help[0])
							help[1] = 0;
						else if ((help[1] == help[0]-2-help[2]) && (menu != M_FLOOR_LEVEL))
							++ help[1];
						gotoxy (16-help[0]+help[1], row % 4);
						break;
					case '*':
						if (help[1] == 0)
							help[1] = help[0]-1;
						else if ((help[1] == help[0]-1-help[2]) && (menu != M_FLOOR_LEVEL))
							help[1] -= 2;
						else
							-- help[1];
						gotoxy (16-help[0]+help[1], row % 4);
						break;
					case (KEY_ESC):
						menu = menu_old;
						new = 1;
						numberkey = 0;				/* enable direct access with xxx* again */
						break;

					case ('#'):
						i = 0;
						value = 0;
						while(buffer[i])
						{
							if (buffer[i] >= '0' && buffer[i] <= '9')
								value = value*10ul + (buffer[i]-'0');
							++ i;
						}
						if(paratext [paranumber].access == D_READ)
						{
							display_standardtext (T_NO_SUCCESS);
							wait = 1;
							displaytimer = timer + 3 SEC;
						}
						else if (value > paratext [paranumber].max)
						{
							display_standardtext (T_TOO_BIG);
							wait = 1;
							displaytimer = timer + 3 SEC;
						}
						else if (value < paratext [paranumber].min)
						{
							display_standardtext (T_TOO_SMALL);
							wait = 1;
							displaytimer = timer + 3 SEC;
						}
						else
						{
							display_standardtext (T_WAIT);
							wait = 2;
						}
						break;

					default:
						if (key >= '0' && key <= '9')
						{
							buffer [help[1]] = key;
							display (16 - strlen ((char *)buffer), row % 4, buffer);
							++ help[1];
							if (help[1] == help[0])
								help[1] = 0;
							else if ((help[1] == help[0]-2-help[2]) && (menu != M_FLOOR_LEVEL))
								++ help[1];
							gotoxy (16-help[0]+help[1], row % 4);
						}
						break;
				}
				break;
			}
			break;

		case M_TEACH_IN_SHAFT:

			break;

		case (M_STARTLIMIT):
			if (new)
			{
				wait = 0;
				if (testtrip)
				{
					display_standardtext (T_TRIPLMT_NOTAVAILABLE);
					limitstatus = 0;
					p.limitenable = 0;
				}
				else if ((p.t_limit.year && p.t_limit.month && p.t_limit.day) || p.limittimes)
				{
					display_standardtext (T_TRIPLMT_START);
					wait = 1;
				}
				else
				{
					display_standardtext (T_TRIPLMT_CANCELLED);
					limitstatus = 0;
					p.limitenable = 0;
					tripcount_limit = 0;		//restart the limit trip count
				}
				displaytimer = timer + 4 SEC;
				new = 0;
			}
			if (key || (displaytimer < timer))
			{
				if (wait)
				{
					if(p.t_limit.year > 2000)
						p.t_limit.year -= 2000;
					limitstatus = 0;
					p.limitenable = 1;
					tripcount_limit = 0;		//restart the limit trip count
					WriteParaData(&tripcount_limit, EE_TRIPCOUNT_LIMIT, sizeof(tripcount_limit));
				}
				menu = menu_old;
				new = 1;
			}
			break;

		case (M_STATUS_LIMIT):
			if (new)
			{
				clrscr ();
				cursor (_NOCURSOR);
				new = 0;
				sprintf ((char *)buffer, "ET: %02u.%02u.%02u.%02u", p.t_limit.hour, p.t_limit.day, p.t_limit.month, p.t_limit.year);
				display (0, 0, buffer);
				sprintf ((char *)buffer, "RT: %02u.%02u.%02u.%02u", t.hour, t.day, t.month, t.year);
				display (0, 1, buffer);

				if (p.limitenable)
				{
					dwordvalue [2] = p.limittimes;
					dwordvalue [4] = tripcount_limit;
				}
				else
				{
					dwordvalue [2] = 0;
					dwordvalue [4] = 0;
				}
				sprintf ((char *)buffer, "CALL: %6u", dwordvalue [2]);
				display (0, 2, buffer);
				sprintf ((char *)buffer, "RCALL:%6u", dwordvalue [4]);
				display (0, 3, buffer);
			}

			if (time_new)
			{
				time_new = 0;
				sprintf ((char *)buffer, "%02u.%02u.%02u.%02u", t.hour, t.day, t.month, t.year);
				display (4, 1, buffer);
			}
			if(p.limitenable == 1)
				dwordvalue [2] = (p.limittimes);
			else
				dwordvalue [2] = 0;

			if (dwordvalue [3] != dwordvalue [2])
			{
				dwordvalue [3] = dwordvalue [2];
				sprintf ((char *)buffer, "%6u", dwordvalue [2]);
				display (6, 2, buffer);
			}

			if (p.limitenable == 1)
				dwordvalue [4] = (WORD)tripcount_limit;
			else
				dwordvalue [4] = 0;

			if (dwordvalue [5] != dwordvalue [4])
			{
				dwordvalue [5] = dwordvalue [4];
				sprintf ((char *)buffer, "%6u", dwordvalue [4]);
				display (6, 3, buffer);
			}
			break;
			
		case M_RESET_MCU:
			if(new)
				{
					display_standardtext (T_PASSWORD);
					cursor (_NORMALCURSOR);
					gotoxy (0, 1);
					numberkey = 1;
					value = 0;
					new = 0;
					wait = 0;
					numberpos = 0;
					for (i = 0; i < 10; i++)
						{
							buffer [i] = 0;
							passwdbuf [i] = 0;
						}
				}
			if (wait == 1)
				{
					if ((key) || (displaytimer < realtimer))	// 不使用timer是因为读入para后可能造成参数混乱而在cycle中死机,timer不能递增
						new = 1;
				}
			else if (wait == 2)
				{		
					if (menu == M_RESET_MCU)
						{
							display_standardtext (T_OK_AND_RESET);
							displaytimer = realtimer + 3 SEC;	// 不使用timer是因为读入para后可能造成参数混乱而在cycle中死机,timer不能递增
							wait = 3;
						}
				}
			else if (wait == 3)
			{
				if ((key) || (displaytimer < realtimer))	// 不使用timer是因为读入para后可能造成参数混乱而在cycle中死机,timer不能递增
				{
					DisableIrq();
					while(1);					// 强迫看门狗复位
				}
			}
			else if (wait == 4)
				{
					if ((key) || (displaytimer < realtimer))
						{
							menu = menu_old;
							new = 1;
							numberkey = 0;
						}
				}
			else
				{
					switch (key)
						{
							case ('#'):
								sscanf ((const char *)buffer, "%lu", &value);
								if (value == PSW_PARA)
									{
										display_standardtext (T_WAIT);
										wait = 2;
									}
								else
									{
										display_standardtext (T_PASSWORD_WRONG);
										displaytimer = realtimer + 3 SEC;
										wait = 1;
									}
								break;

							case (KEY_ESC):
								menu = menu_old;
								*((BYTE *)vptr) = (BYTE)oldvalue;		//密码错误返回之前值
								new = 1;
								numberkey = 0;
								break;

							default:
								if ((key >= '0' && key <= '9') && (numberpos < 9))
									{
										passwdbuf [numberpos] = '*';
										buffer [numberpos++] = key;
										display (0, 1, passwdbuf);
									}
								break;
						}
				}
			break;
		
		case (M_SIMU):
			if (new)									/* show text to load simulation parameter*/
			{
				wait = 0;
				if (dwordvalue [0])							/* start simulation					*/
					{
						numberpos = 0;
						for (i = 0; i < 10; i++)
							{
								buffer [i] = 0;
								passwdbuf [i] = 0;
							}
						display_standardtext (T_PASSWORD);
						cursor (_NORMALCURSOR);
						gotoxy (0, 1);
						numberkey = 1;					/* disable direct access with xxx*		*/
						value = 0;
						new = 0;
					}
				else									/* stop simulation					*/
					{
						p.simulation = 0;
						menu = menu_old;
						numberkey = 0;
						break;
					}
			}
			if (wait == 1)
				{
					if (!iic_busy)
						{
							menu = menu_old;
							new = 1;
							numberkey = 0;
						}
				}
			else if (wait == 2)
				{
					if ((key) || (displaytimer < timer))
						{
							new = 1;
						}
				}
			switch (key)
			{
				case ('#'):
					sscanf ((const char *)buffer, "%lu", &value);
					if (value == DEFAULT_PSW_INSTALL)
					{
						display_standardtext (T_WAIT);
						LoadDefaultPara ();				/* set simulation parameter			*/
						WriteParaData(&p, EE_PARAMETER, sizeof(p));
						WriteParaData(&password_admin, EE_PASSWORD0, sizeof(password_admin));
						WriteParaData(&password_help, EE_PASSWORD1, sizeof(password_help));
						WriteParaData(&password_install, EE_PASSWORD2, sizeof(password_install));
						WriteParaData(&password_maintain, EE_PASSWORD3, sizeof(password_maintain));
						WriteParaData(&tripcount, EE_TRIPCOUNT, sizeof(tripcount));
						WriteParaData(&op_time, EE_OPERTIME, sizeof(op_time));
						WriteParaData(&exe_para, EE_EXE_PARA, sizeof(exe_para));
						WriteParaData (floorcount, EE_FLOORCOUNT, sizeof (floorcount));
						para_valid = 1;
						wait = 1;
					}
					else
					{
						display_standardtext (T_PASSWORD_WRONG);
						p.simulation = 0;
						displaytimer = timer + 40;
						wait = 2;
					}
					break;

				case (KEY_ESC):
					p.simulation = 0;
					menu = menu_old;
					new = 1;
					break;

				default:
					if ((key >= '0' && key <= '9') && (numberpos < 9))
					{
						passwdbuf [numberpos] = '*';
						buffer [numberpos++] = key;
						display (0, 1, passwdbuf);
					}
					break;
			}
			break;

		case (M_BRAKE_CHECK):
			if (new)
				{
					display_standardtext(T_BRAKE_CHECK1);
					numberkey = 1;
					new = 0;
					wait = 0;
					displaytimer = timer + 2 SEC;
				}
			 if(!wait)
				{
					if(!(safety_circuit & SC3) && (displaytimer < timer))
						{//安全回路不通							
							display_standardtext(T_BRAKE_CHECK4);
							displaytimer = timer + 2 SEC;
							wait = 1;
						}
					else if(brake_check_noactive)
						{
							display_standardtext(T_BRAKE_CHECK5);
							displaytimer = timer + 2 SEC;
							wait = 1;
						}
					else if(displaytimer < timer)
						{
							wait = 2;
							displaytimer = timer + 2 SEC;
						}
				}
			else if(wait == 1)
				{
					if ((key == '#') && (displaytimer < timer))
						{
							menu = menu_old;
							bTest_brake = 0;
							brake_check_forced = 0;
							numberkey = 0;
							new = 1;
						}
				}
			else if (wait == 2)
				{				
					if ((key == '#') && (displaytimer < timer))
						{
							menu = menu_old;
							bTest_brake = 0;
							brake_check_forced = 0;
							numberkey = 0;
							new = 1;
						}
					if(brake_check_return)
						{//测试完毕
							if(brake_value == BRAKE_GOOD)
								{//测试合格									
									display_standardtext(T_BRAKE_CHECK2);
								}
							else
								{//测试不合格
									display_standardtext(T_BRAKE_CHECK3); 								
									if((brake_value != brake_value_old) && (brake_value == BRAKE_NO_GOOD))
										{
											write_errorhist (E_BRAKE_CHECK, HSE_ID, C_BRAKE_CHECK, 0);
											brake_value_old = brake_value;
										}
								}
						}
				}
			passwordtimer = PASSWORDTIME;
			break;

		case M_UCMP_TEST:					
			if (new)
				{
					clrscr ();
					cursor (_NOCURSOR);
					new = 0;
					if(ucmp_test_finish)
						{	
							help[0] = ucmp_test_distance;							
							help[1] = p.ucmp_door_height - ucmp_test_distance;
							help[2] = ucmp_test_distance;				
							help[4] = ucmp_test_floor;
							help[5] = nice5000_Rstore_position;
							help[6] = p.ucmp_test_speed;
							help[7] = 0;
							if(p.ucmp_test_dir == DIR_UP)
								{
									if(ucmp_test_distance > p.ucmp_car_apron)
										help[3] = ucmp_test_distance - p.ucmp_car_apron;
									else
										help[3] = 0;
									sprintf ((char *)buffer, "Direction: UP");
								}
							else if(p.ucmp_test_dir == DIR_DN)
								{
									if(ucmp_test_distance > p.ucmp_hall_apron)
										help[3] = sqrt((ucmp_test_distance - p.ucmp_hall_apron) * (ucmp_test_distance - p.ucmp_hall_apron)
															+ (p.ucmp_car_distance * p.ucmp_car_distance));	
									else
										help[3] = 0;
									sprintf ((char *)buffer, "Direction: DOWN");
								}
							display (0, 0, buffer);
							sprintf ((char *)buffer, "Distance A: %4u", help[1]);
							display (0, 1, buffer);
							sprintf ((char *)buffer, "Distance B: %4u", help[2]);
							display (0, 2, buffer);
							sprintf ((char *)buffer, "Distance C: %4u", help[3]);
							display (0, 3, buffer);
							wait = 0;
						}
					else			
						{
							display_standardtext(T_UCMP_NOT_FINISH); 		
							displaytimer = timer + 2 SEC;
							wait = 1;
						}
					menu_old = menu;

				}
			if((key == '*') || (key == KEY_UP))
				{
					if(ucmp_test_finish)
						{
							clrscr ();
							cursor (_NOCURSOR);
							help[7] = !help[7];
							if(help[7])
								{
									sprintf ((char *)buffer, "FLoor: %2u", help[4] + 1);
									display (0, 0, buffer);
									sprintf ((char *)buffer, "Height: 0.%2um", help[5] / 1000);
									display (0, 1, buffer);
									sprintf ((char *)buffer, "Speed: 0.%2um/s", help[6] / 1000);
									display (0, 2, buffer);
								}
							else
								{
									if(p.ucmp_test_dir == DIR_UP)
										sprintf ((char *)buffer, "Direction: UP");
									else if(p.ucmp_test_dir == DIR_DN)
										sprintf ((char *)buffer, "Direction: DOWN");
									display (0, 0, buffer);
									sprintf ((char *)buffer, "Distance A: %4u", help[1]);
									display (0, 1, buffer);
									sprintf ((char *)buffer, "Distance B: %4u", help[2]);
									display (0, 2, buffer);
									sprintf ((char *)buffer, "Distance C: %4u", help[3]);
									display (0, 3, buffer);
								}
						}								
			
					else if((key == KEY_ESC) || (wait && (displaytimer < timer)))
						{
							menu = menu_old;
							new = 1;
						}
				}
			break;			

		default:
			if (new)									/* show standard menu or parameter list	*/
			{
				switch (menu)
				{
					case (M_DOORPOS):
					case (M_PARKDOOR):
					case (M_FORCED_STOP):
					case (M_REMOTE_OFF):
					case (M_DOOR_NORMAL):
					case (M_DOOR_TIMER_1):
					case (M_DOOR_TIMER_2):
					case (M_DOOR_SPECIAL):
					case (M_DOOR_ADV):
					case (M_DOOR_EMERG):
					case (M_DOOR_FIRE):
					case (M_DOOR_ALARM):
					case (M_DOOR_N_HALL):
					case (M_DOOR_T1_HALL):
					case (M_DOOR_T2_HALL):
					case (M_DOOR_AUTOCALL):
					case (M_TESTTRIP):
						lcdram = 1;
						break;

					default:
						lcdram = 0;
						break;
				}
				if (menu_old != menu)
					row = search_row (menu, menu_old);
				if (menu == M_HIST_DETAIL)
				{
					menusize = display_hist_detail (help [1], row);
					row = 1;
				}
				else if (menu < M_GEN_PARA)
					menusize = display_menu (menu, row);
				else
					menusize = display_paralist (menu, row);
				cursor (_SOLIDCURSOR);
				new = 0;
				wait = 0;
			}
			if (wait)
			{
				if ((displaytimer < timer) || key)
				{
					wait = 0;
					if (menu == M_HIST_DETAIL)
						display_hist_detail (help [1], row);
					else if (menu < M_GEN_PARA)
						display_menu (menu, row);
					else
						display_paralist (menu, row);
					cursor (_SOLIDCURSOR);
				}
			}
			else
			{
				switch (key)
				{
					case ('*'):
						if (row < (menusize - 1))
							row++;
						else
							row = 0;
						if (!(row % 4))
						{
							if (menu == M_HIST_DETAIL)
							{
								display_hist_detail (help [1], row);
								row++;
							}
							else if (menu < M_GEN_PARA)
								display_menu (menu, row);
							else
								display_paralist (menu, row);
						}
						else
							gotoxy (0, row % 4);
						break;

					case (KEY_UP):
						if (row)
							row--;
						else
							row = menusize - 1;
						if (menu == M_HIST_DETAIL)
						{								/* don't go to first line				*/
							if (!row)
								row = menusize - 1;
							else if (!(row % 4))
								row--;
						}
						if ((!((row + 1) % 4)) || ((row + 1) == menusize))
						{
							if (menu == M_HIST_DETAIL)
								display_hist_detail (help [1], row);
							else if (menu < M_GEN_PARA)
								display_menu (menu, row);
							else
								display_paralist (menu, row);
						}
						else
							gotoxy (0, row % 4);
						break;

					case (KEY_ESC):
						menu_old = menu;
						if (menu == M_HIST_DETAIL)
							menu = M_SHOW_HIST;
						else if (menu < M_GEN_PARA)
							menu = search_submenu (menu, menusize - 1, &right);
						else
							menu = search_mainmenu (menu);
						new = 1;
						break;

					case ('#'):
						if (menu == M_HIST_DETAIL)
						{
							menu_old = menu;
							menu = M_SHOW_HIST;
						}
						else if (menu < M_GEN_PARA)
						{
							i = search_submenu (menu, row, &right);
							if (i == M_STATUS1)
							{
								menu_old = menu;
								menu = i;
							}
							else if ((password_right <= right) &&
							(!((password_right == RIGHT_MAINTAIN) && (!para_valid))))
							{
								if ((i == M_SET_LOAD) && (p.loadtype != ANALOG_ON_CDU) && (p.loadtype != ANALOG_ON_MCU))
								{
									wait = 1;
									display_standardtext (T_DIGIT_LOAD_NA);
									displaytimer = timer + 3 SEC;
									break;
								}
								menu_old = menu;
								menu = i;
								if (menu == M_TIME)
								{
									for (i = 0; i < sizeof (t); i++)
										*(((BYTE *)&t_set) + i) = *(((BYTE *)&t) + i);
									t_set.weekday &= 0x0F;
									t_set.year += 2000;
								}
							}
							else
							{
								wait = 1;
								display_standardtext (T_DENY_ACCESS);
								displaytimer = timer + 3 SEC;
								break;
							}

							help [2] = 0;					/* important for error history			*/
						}
						else
						{
							menu_old = menu;
							paranumber = search_paranumber (menu, row);
							if (paratext [paranumber].menu == ENDSTRING)
								menu = search_mainmenu (menu);
							else
							{
								if (password_right <= paratext [paranumber].right)
								{
									menu = paratext [paranumber].paratype;
									address = paratext [paranumber].index;		//增加index，主要针对 NICE5000 变量
								}
								else
								{
									wait = 1;
									display_standardtext (T_DENY_ACCESS);
									displaytimer = timer + 3 SEC;
									break;
								}
							}
						}
						new = 1;
						break;
				}
			}
			break;
	}
}


