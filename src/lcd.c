
#include "typedef.h"
#include "parameter.h"
#include "stdio.h"
#include "string.h"

#include "state.h"
#include "call.h"
#include "can.h"
#include "text.h"
#include "lcd.h"
#include "history.h"
#include "lcd.h"
#include "io.h"
#include "eeprom.h"


BYTE const ese_dispID[MAX_ESE][3] = {
	{" 1A"}, {" 1B"},	{" 2A"}, {" 2B"}, {" 3A"}, {" 3B"}, {" 4A"}, {" 4B"},	{" 5A"}, {" 5B"}, 
	{" 6A"}, {" 6B"}, {" 7A"}, {" 7B"}, {" 8A"}, {" 8B"}, {" 9A"}, {" 9B"}, {"10A"}, {"10B"}, 
	{"11A"}, {"11B"}, {"12A"}, {"12B"}, {"13A"}, {"13B"}, {"14A"}, {"14B"}, {"15A"}, {"15B"}, 
	{"16A"}, {"16B"}, {"17A"}, {"17B"}, {"18A"}, {"18B"}, {"19A"}, {"19B"}, {"20A"}, {"20B"}, 
	{"21A"}, {"21B"}, {"22A"}, {"22B"}, {"23A"}, {"23B"}, {"24A"}, {"24B"}, {"25A"}, {"25B"}, 
	{"26A"}, {"26B"}, {"27A"}, {"27B"}, {"28A"}, {"28B"}, {"29A"}, {"29B"}, {"30A"}, {"30B"}, 
	{"31A"}, {"31B"}, {"32A"}, {"32B"}, {"33A"}, {"33B"}, {"34A"}, {"34B"}, {"35A"}, {"35B"}, 
	{"36A"}, {"36B"}, {"37A"}, {"37B"}, {"38A"}, {"38B"}, {"39A"}, {"39B"}, {"40A"}, {"40B"}, 
	{"41A"}, {"41B"}, {"42A"}, {"42B"}, {"43A"}, {"43B"}, {"44A"}, {"44B"}, {"45A"}, {"45B"}, 
	{"46A"}, {"46B"}, {"47A"}, {"47B"}, {"48A"}, {"48B"}, {"49A"}, {"49B"}, {"50A"}, {"50B"}, 
	{"51A"}, {"51B"}, {"52A"}, {"52B"}, {"53A"}, {"53B"}, {"54A"}, {"54B"}, {"55A"}, {"55B"}, 

};

/****************************************************************************************************/
/*	Cursor to row /column																*/
/****************************************************************************************************/
void gotoxy (BYTE column, BYTE row)
{
	cursorpos [0] = row;
	cursorpos [1] = column;
}

/****************************************************************************************************/
/*	display a string																				*/
/****************************************************************************************************/
void display (BYTE column, BYTE row, BYTE * ptr)
{
	BYTE sign;

	cursorpos [0] = row;
	cursorpos [1] = column;

	if (column >= MAX_LCD_COL)						/* don't write over line end				*/
		column = MAX_LCD_COL - 1;
	while ((*ptr) && (column < MAX_LCD_COL))		/* watch line end and string end sign		*/
	{
		sign = *ptr ++;
		switch (sign)								/* change special characters				*/
		{
			case 0xe4:
			case 0xc4:
				sign = 225;
				break;

			case 0xf6:
			case 0xd6:
				sign = 239;
				break;

			case 0xfc:
			case 0xdc:
				sign = 245;
				break;

			case 0xdf:
				sign = 226;
				break;

			case 0xb0:
				sign = 223;
				break;
		}
		if ((sign <= 8) && lcdram)
			sign += 8;
		lcd_display [row][column] = sign;				/* save lcd display to variable				*/
		cursorpos [1]++;							/* save new cursor position				*/
		column++;								/* go to next row							*/
	}
}

/****************************************************************************************************/
/*	Clear screen																			*/
/****************************************************************************************************/
void clrscr (void)
{
	BYTE i, j;

	for (i = 0; i < MAX_LCD_ROW; i++)
	{
		for (j = 0; j < MAX_LCD_COL; j++)
			lcd_display [i][j] = ' ';
	}
	cursorpos [0] = 0;
	cursorpos [1] = 0;
}

/****************************************************************************************************/
/*	Clear line																			*/
/****************************************************************************************************/
void clearline (BYTE line)
{
	display (0, line, "                ");
}

/****************************************************************************************************/
/*	set cursor type																		*/
/****************************************************************************************************/
void cursor (BYTE type)
{
	cursortype = type;									/* save type of cursor					*/
}


/****************************************************************************************************/
/*	send LCD display over CAN																*/
/****************************************************************************************************/
void transmit_lcd (void)
{//5ms 发送 4 BYTE
	static BYTE column = 0;
	static BYTE row = 0;
	static BYTE new = 1;
	BYTE pos [2];

	while (row < MAX_LCD_ROW)
	{
		while (column < MAX_LCD_COL)
		{
			if ((lcd_display [row][column] != lcd_old [row][column]) ||
			(lcd_display [row][column + 1] != lcd_old [row][column + 1]) ||
			(lcd_display [row][column + 2] != lcd_old [row][column + 2]) ||
			(lcd_display [row][column + 3] != lcd_old [row][column + 3]))
			{
				if ((cursorpos_old [0] != row) || (cursorpos_old [1] != column))
				{
					transmit_cursorpos (row, column, handheld_active - 1);
					cursorpos_old [0] = row;
					cursorpos_old [1] = column;
				}
				else
				{
					transmit_sign (lcd_display [row][column], lcd_display [row][column + 1], lcd_display [row][column + 2], lcd_display [row][column + 3], handheld_active - 1);
					lcd_old [row][column] = lcd_display [row][column];
					lcd_old [row][column + 1] = lcd_display [row][column + 1];
					lcd_old [row][column + 2] = lcd_display [row][column + 2];
					lcd_old [row][column + 3] = lcd_display [row][column + 3];
					cursorpos_old [1] += 4;
					new = 0;
				}
				return;
			}
			column += 4;
		}
		row++;
		column = 0;
	}
	if (new)										/* complete LCD checked; no change	*/
	{//只更新光标位置
		if (cursortype != cursortype_old)
		{
			transmit_cursortype (cursortype, handheld_active - 1);
			cursortype_old = cursortype;
			return;
		}
		*(WORD *)pos = *(WORD *)cursorpos;
		if ((*(WORD *)pos) != (*(WORD *)cursorpos_old))
		{
			if (cursortype != _NOCURSOR)
			{
				transmit_cursorpos (pos [0], pos [1], handheld_active - 1);
				*(WORD *)cursorpos_old = *(WORD *)pos;
			}
		}
	}
	if (row == MAX_LCD_ROW)
	{
		row = 0;
		column = 0;
		new = 1;
	}
}

/****************************************************************************************************/
/*	display a standard text																	*/
/****************************************************************************************************/
void display_standardtext (BYTE textnumber)
{
	WORD i	= 0;
	BYTE row = 0;

	clrscr ();
	cursor (_NOCURSOR);
	while ((standardtext [i].number != ENDSTRING) && (row < 4))
	{
		if (standardtext [i].number == textnumber)
		{
			display (0, row, (BYTE *)standardtext [i].text [language]);
			row++;
		}
		i++;
	}
}

/****************************************************************************************************/
/*	display a standard text																	*/
/****************************************************************************************************/
void display_line (BYTE textnumber, BYTE row)
{
	WORD i	= 0;
	clearline (row);									/* clear line							*/
	if (!textnumber)
		return;
	while (standardtext [i].number != ENDSTRING)			/* search text structure				*/
	{
		if (standardtext [i].number == textnumber)
		{
			display (0, row, (BYTE *)standardtext [i].text [language]);
			return;
		}
		i++;
	}
}

/****************************************************************************************************/
/*	display a error text																	*/
/****************************************************************************************************/
void display_error (WORD textnumber, BYTE row, WORD id)
{
	BYTE buffer [5];
	WORD errcode, codeid;
	WORD i = 0;

	errcode = textnumber;
	if (!errcode)
		return;
	else if ((errcode & 0xFF00) == E_IV_ERROR)
		errcode = E_IV_ERROR;
	while (errortext [i].number != ENDSTRING)			/* search text structure					*/
	{
		if (errortext [i].number == errcode)
			break;
		i++;
	}
	if (errcode == E_IV_ERROR)
	{
		display (0, row, (BYTE *)errortext [i].text [language]);
		codeid = textnumber & 0x00ff;
		sprintf ((char *)buffer, "E%03d", codeid);
		display (16 - strlen ((char *)buffer), row, buffer);
		return;
	}
	else
		display (0, row, (BYTE *)errortext [i].text [language]);
	if (errortext [i].show_id)							/* show node name of error producer		*/
		{
			switch (id)
				{
					case 0:
						break;
					case (HSE_ID):
						display (13, row, "MCU");
						break;
					case (DRV_ID):
					case (DCU_ID):
						display (13, row, "DRV");
						break;
					case (FVE_ID):
						display (13, row, "CDU");
						break;
					case (DSE_ID):
						display (13, row, "HHT");
						break;

					default:
						if (id >= 128)						/* from shaft bus							*/
							{
								id -= 128;
								if ((id >= ESE_ID) && (id < (ESE_ID + MAX_ESE)))
									sprintf ((char *)buffer, "LCU%u", id - ESE_ID +1);
								else
									sprintf ((char *)buffer, "IDs%u", id);
							}
						else
							{
								if ((id >= TSE_ID) && (id < TSE_ID + MAX_TSE))
									sprintf ((char *)buffer, "PCU%u", id - TSE_ID + 1);
								else
									sprintf ((char *)buffer, "ID%u", id);
							}
						display (16 - strlen ((char *)buffer), row, buffer);
						break;
				}
		}
	else
		{
			codeid = errortext [i].err_code;			
			sprintf ((char *)buffer, "E%03d", codeid);
			display (16 - strlen ((char *)buffer), row, buffer);
		}
}


/****************************************************************************************************/
/*	display a menu text																				*/
/****************************************************************************************************/
WORD display_menu (WORD menunumber, WORD pos)
{
	WORD i	= 0;
	WORD menustart;
	WORD menusize = 0;

	clrscr ();
	cursor (_SOLIDCURSOR);
	menustart = pos / 4 * 4;
	while (menutext [i].number != ENDSTRING)
	{
		if (menutext [i].number == menunumber)
		{
			if ((menusize >= menustart) && (menusize < (menustart + 4)))
				display (0, menusize - menustart, (BYTE *)menutext [i].text [language]);
			menusize++;
		}
		i++;
	}
	gotoxy (0, pos % 4);
	return (menusize);
}

/****************************************************************************************************/
/*	get number of text lines in structure														*/
/****************************************************************************************************/
WORD get_tablesize (struct _specialtext *pointer)
{
	WORD i = 0;

	while ((i < 255) && strlen ((char *)(pointer [i++]).text [language]));
	return (i);
}

/****************************************************************************************************/
/*	get number of text lines in structure														*/
/****************************************************************************************************/
WORD get_stringnumber (struct _textlist * pointer)
{
	WORD i = 0;

	while ((i < 1000) && strlen ((char *)(pointer [i++]).text [language]));
	return (i);											/* limit for text lines in structure is 1000*/
}

/****************************************************************************************************/
/*	return position of 1 bit = 1																*/
/****************************************************************************************************/
BYTE bitpos (DWORD value)
{
	BYTE i = 0;

	while (i < 32)
	{
		if (bit_selectd (value, i++))
			return (i);
	}
	return (0);
}

/****************************************************************************************************/
/*	change a parameter from parameter list													*/
/****************************************************************************************************/
DWORD change_listpara (WORD number, WORD pos, DWORD value, BYTE searchdir)
{
	WORD size;
	BYTE buffer [20];
	struct _textlist *pointer;

	size = get_stringnumber (paratext [number].tablepointer) - 1;	/* get number of values in table	*/
	if (searchdir)										/* get next value						*/
	{
		if (value < (size + paratext [number].min - 1))		/* set new value						*/
			value++;
		else
			value = paratext [number].min;
	}
	else												/* get previous value					*/
	{
		if (value > paratext [number].min)					/* set new value						*/
			value--;
		else
			value = size + paratext [number].min - 1;
	}
	pointer = paratext [number].tablepointer;				/* set pointer to table					*/
	clearline (pos % 4);									/* clear line							*/
	if (paratext [number].array)
	{
		sprintf ((char *)buffer, "%2u.%s", pos, paratext [number].text [language]);
		display (0, pos % 4, buffer);
	}
	else
		display (0, pos % 4, (BYTE *)paratext [number].text [language]);
	sprintf ((char *)buffer, "%s", pointer [value - paratext [number].min].text [language]);
	display (16 - strlen ((char *)buffer), pos % 4, buffer);				/* display new value					*/
	gotoxy (15, pos % 4);
	return (value);
}

/****************************************************************************************************/
/*	get next sign for level display															*/
/****************************************************************************************************/
BYTE next_sign (BYTE sign, BYTE searchdir)
{
	BYTE const signtype [] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ+-abcdefghijklmnopqrstuvwxyz!#$%&()*',:;<>=@[]{}| ";
	BYTE signpos = 0;

	while ((signtype [signpos] != ' ') && (signtype [signpos] != sign))
		signpos++;									/* search actual sign in table			*/
	if (searchdir)
	{
		if (signtype [signpos] == ' ')						/* last sign in table					*/
			signpos = 0;								/* go to beginning of table				*/
		else
			signpos++;								/* point to next sign					*/
	}
	else
	{
		if (!signpos)									/* first sign in table					*/
			return (' ');								/* return last sign					*/
		else
			signpos--;								/* point to previous sign				*/
	}
	return (signtype [signpos]);
}

/****************************************************************************************************/
/*	display a parameter value																*/
/****************************************************************************************************/
void display_paravalue (void *parapointer, WORD parapos, WORD pos, WORD offset)
{
	BYTE buffer [20];
	DWORD value = 0;
	BYTE i, j;
	float floatvalue = 0;

	buffer [0] = 0;
	if (paratext [parapos].size)
	{
		switch (paratext [parapos].size)
		{
			case (1):
				value = *((BYTE *)parapointer + offset);
				break;
			case (2):
				value = *((WORD *)parapointer + offset);
				break;
			case (4):
				value = *((DWORD *)parapointer + offset);
				break;
		}
	}
	switch (paratext [parapos].paratype)
	{
		case (M_FLOORPARA):
		case (M_LIFTPARA):
			value++;
			break;

		case (M_LIFTBITPARA):
			j = bitpos (value);							/* change from bit position to number	*/
			if (j && (value != (1 << (j - 1))))				/* check if more than 1 bit is set		*/
				value = 0xFF;
			else
				value = j;
			break;
			
		case (M_SIGNPARA):
			i = value >> 8;
			j = value;
			if (!i)
				i = ' ';
			if (!j)
				j = ' ';
			sprintf ((char *)buffer, "%c%c", i, j);
			break;
		case (M_1BIT_DECIMAL):
			floatvalue = (float)value / 10.0;
			sprintf ((char *)buffer, "%.1f", floatvalue);
			break;
		case (M_2BIT_DECIMAL):
			floatvalue = (float)value / 100.0;
			sprintf ((char *)buffer, "%.2f", floatvalue);
			break;
		case (M_3BIT_DECIMAL):
			floatvalue = (float)value / 1000.0;
			sprintf ((char *)buffer, "%.3f", floatvalue);
			break;
	}
	if (buffer [0] == 0)									/* buffer still empty					*/
		sprintf ((char *)buffer, "%lu", value);
	if (paratext [parapos].size)
		display (16 - strlen ((char *)buffer), pos, buffer);
}

/****************************************************************************************************/
/*	display a parameter list																*/
/****************************************************************************************************/
WORD display_paralist (WORD menunumber, WORD pos)
{
	WORD i = 0;
	WORD j;
	WORD menustart;
	WORD menusize = 0;
	BYTE buffer [20];
	WORD start = 0;

	clrscr ();
	cursor (_SOLIDCURSOR);
	menustart = pos / 4 * 4;

	while ((paratext [i].menu != ENDSTRING) && (paratext [i].menu != menunumber))
		i++;
	if (paratext [i].array)							/* array parameter								*/
	{
		menusize = (p.top_floor - p.bot_floor + 1) * paratext [i].array;
		start = p.bot_floor;
		for (j = menustart; j < (4 + menustart); j++)
		{
			if (j < menusize)
			{
				switch (paratext [i].array)
				{
					case (1):
						switch (paratext [i].menu)
						{
							case (M_DOORPOS):
							case (M_PARKDOOR):
							case (M_DOOR_NORMAL):
							case (M_DOOR_N_HALL):
							case (M_DOOR_AUTOCALL):
							case (M_DOOR_TIMER_1):
							case (M_DOOR_T1_HALL):
							case (M_DOOR_TIMER_2):
							case (M_DOOR_T2_HALL):
							case (M_DOOR_SPECIAL):
							case (M_DOOR_ADV):
							case (M_DOOR_EMERG):
							case (M_DOOR_ALARM):
							case (M_DOOR_FIRE):
							case (M_FIRE_FLOOR):
							case (M_SIGN):
								if(!language)
									sprintf ((char *)buffer, "%s%02u", paratext [i].text [language], j + start + 1);
								else
									sprintf ((char *)buffer, "%2u.%s", j + start + 1, paratext [i].text [language]);
								break;

							default:
								sprintf ((char *)buffer, "%2u.%s", j + start + 1, paratext [i].text [language]);
								break;
						}
						break;
					case (2):
						if (j % 2)
							sprintf ((char *)buffer, "%2u.%s \6", (j / 2) + start + 1, paratext [i].text [language]);
						else
							sprintf ((char *)buffer, "%2u.%s \5", (j / 2) + start + 1, paratext [i].text [language]);
						break;
				}
				display (0, j % 4, buffer);
				display_paravalue (paratext [i].pointer, i, j % 4, j + (paratext [i].array * start));
			}
		}
		while (paratext [i].menu != ENDSTRING)
			i++;
	}
	else												/* single parameter								*/
	{
		while (paratext [i].menu != ENDSTRING)
		{
			if (paratext [i].menu == menunumber)
			{
				if ((menusize >= menustart) && (menusize < (menustart + 4)))
				{
					display (0, menusize - menustart, (BYTE *)paratext [i].text [language]);
					display_paravalue (paratext [i].pointer, i, menusize - menustart, 0);
				}
				menusize++;
			}
			i++;
		}
	}
	if ((menusize >= menustart) && (menusize < (menustart + 4)))
		display (0, menusize - menustart, (BYTE *)paratext [i].text [language]);
	menusize++;
	gotoxy (0, pos % 4);
	return (menusize);
}

/****************************************************************************************************/
/*	display error history details																	*/
/****************************************************************************************************/
WORD display_hist_detail (WORD number, WORD row)
{
	WORD menusize;
	BYTE * pointer;
	BYTE buffer [20];
	WORD i, j;
	WORD size;
	WORD max;
	DWORD value = 0;
	float floatvalue = 0;

	clrscr ();
	cursor (_SOLIDCURSOR);
	i = 0;
	while (errordetail [i++].size);
	menusize = i + (i / 3);
	if (i % 3)
		menusize++;
	size = sizeof (hist.hist [0]) * number;
	display_error (hist.hist [number].errorcode, 0, 0);
	for (i = 0; i < 3; i++)
	{
		display (0, i + 1, (BYTE *)errordetail [((row / 4) * 3) + i].text [language]);
		if (errordetail [((row / 4) * 3) + i].size)
		{
			pointer = errordetail [((row / 4) * 3) + i].pointer;
			pointer += size;
			buffer [0] = 0;
			switch (errordetail [((row / 4) * 3) + i].size)
			{
				case (1):
					value = *((BYTE *)pointer);
					break;
				case (2):
					value = *((WORD *)pointer);
					break;
				case (4):
					value = *((DWORD *)pointer);
					break;
			}
			switch (errordetail [((row / 4) * 3) + i].paratype)
			{
				case (M_YES_NOPARA):
					if (value)
						value = 1;
					sprintf ((char *)buffer, "%s", (errordetail [((row / 4) * 3) + i].table) [value].text [language]);
					break;

				case (M_FLOORPARA):
					if (value >= 0xFF)
						sprintf ((char *)buffer, "-");
					else
						value++;
					break;

				case (M_ERRORPARA):
					if((hist.hist [number].errorcode & 0xFF00) != E_IV_ERROR)
						{
							j = 0;
							while (errortext [j].number != ENDSTRING) 		/* search text structure					*/
							{
								if (errortext [j].number == hist.hist [number].errorcode)
									break;
								j++;
							}
							value = errortext [j].err_code;
						}
					else
						value = hist.hist [number].errorcode - E_IV_ERROR;
					sprintf ((char *)buffer, "E%03d", value);
					break;

				case (M_NODEIDPARA):
					switch (value)
					{
						case (HSE_ID):
							sprintf ((char *)buffer, "MCU");
							break;
						case (DRV_ID):
						case (DCU_ID):
							sprintf ((char *)buffer, "DRV");
							break;
						case (FVE_ID):
							sprintf ((char *)buffer, "CDU");
							break;
						case (DSE_ID):
							sprintf ((char *)buffer, "HHT");
							break;

						default:
							if (value >= 128)		/* from shaft bus				*/
							{
								value -= 128;
								if ((value >= ESE_ID) && (value < (ESE_ID + MAX_ESE)))
									sprintf ((char *)buffer, "LCU %lu", value - ESE_ID + 1);
								else
									sprintf ((char *)buffer, "IDS %lu", value);
							}
							else
							{
								if ((value >= TSE_ID) && (value < TSE_ID + MAX_TSE))
									sprintf ((char *)buffer, "PCU %lu", value - TSE_ID + 1);
								else
									sprintf ((char *)buffer, "ID %lu", value);
							}
							break;
					}
					break;

				case (M_LISTPARA):
					max = get_stringnumber ((struct _textlist *)errordetail [((row / 4) * 3) + i].table);
					if (value && (value < max))
						sprintf ((char *)buffer, "%s", (errordetail [((row / 4) * 3) + i].table) [value - 1].text [language]);
					break;

				case (M_BITPARA):
					value = bitpos (value);
					sprintf ((char *)buffer, "%s", (errordetail [((row / 4) * 3) + i].table) [value].text [language]);
					break;

				case (M_DOORNUMBERPARA):
				case (M_BITPOSPARA):
					if (errordetail [((row / 4) * 3) + i].paratype == M_DOORNUMBERPARA)
						max = *(BYTE *)errordetail [((row / 4) * 3) + i].table;
					else
						max = (BYTE)((DWORD)errordetail [((row / 4) * 3) + i].table);
					for (j = 0; j < max; j++)
					{
						if ((max < 8) && (value & 0x80))
							buffer [j] = '?';
						else if ((1 << j) & value)
							buffer [j] = 0xFF;
						else
							buffer [j] = 8;
					}
					buffer [j] = 0;
					break;

				case (M_SS_PARA):
					if (value & POS_SGM)
						buffer [0] = 0xFF;
					else
						buffer [0] = 8;

					if ((value & (POS_SGD | POS_SGU)) == (POS_SGD | POS_SGU))
						buffer [1] = 0xff;
					else if (value & POS_SGD)
						buffer [1] = 2;
					else if (value & POS_SGU)
						buffer [1] = 1;
					else
						buffer [1] = 8;

					if ((value & (POS_ILU | POS_ILD)) == (POS_ILU | POS_ILD))
						buffer [2] = 0xff;
					else if (value & POS_ILD)
						buffer [2] = 2;
					else if (value & POS_ILU)
						buffer [2] = 1;
					else
						buffer [2] = 8;

					if ((value & (POS_SDU1 | POS_SDD1)) == (POS_SDU1 | POS_SDD1))
						buffer[5] = 0xff;
					else if (value & POS_SDU1)
						buffer[5] = 1;
					else if (value & POS_SDD1)
						buffer[5] = 2;
					else
						buffer[5] = 8;

					if ((value & (POS_SDU2 | POS_SDD2)) == (POS_SDU2 | POS_SDD2))
						buffer[4] = 0xff;
					else if (value & POS_SDU2)
						buffer[4] = 1;
					else if (value & POS_SDD2)
						buffer[4] = 2;
					else
						buffer[4] = 8;

					if ((value & (POS_SDU3 | POS_SDD3)) == (POS_SDU3 | POS_SDD3))
						buffer[3] = 0xff;
					else if (value & POS_SDU3)
						buffer[3] = 1;
					else if (value & POS_SDD3)
						buffer[3] = 2;
					else
						buffer[3] = 8;
					buffer [6] = 0;
					break;

				case (M_1BIT_DECIMAL):
					floatvalue = (float)value / 10.0;
					if(((WORD*)(errordetail [((row / 4) * 3) + i].pointer) == &hist.hist[0].nice5000_Rfault_outtorque)
							&& (value > 0x8000))
						{
							floatvalue = 0xFFFF - floatvalue;
							buffer[0] = '-';
							sprintf ((char *)(&buffer[1]), "%.1f", floatvalue);
						}
					else
						sprintf ((char *)buffer, "%.1f", floatvalue);
					break;
				case (M_2BIT_DECIMAL):
					floatvalue = (float)value / 100.0;
					if(((WORD*)(errordetail [((row / 4) * 3) + i].pointer) == &hist.hist[0].nice5000_Rfault_outpwr)
							&& (value > 0x8000))
						{
							floatvalue = 0xFFFF - floatvalue;
							buffer[0] = '-';
							sprintf ((char *)(&buffer[1]), "%.1f", floatvalue);
						}
					else
						sprintf ((char *)buffer, "%.2f", floatvalue);
					break;
				case (M_3BIT_DECIMAL):
					floatvalue = (float)value / 1000.0;
					sprintf ((char *)buffer, "%.3f", floatvalue);
					break;

			}
			if (buffer [0] == 0)
				sprintf ((char *)buffer, "%lu", value);
			display (16 - strlen ((char *)buffer), i + 1, buffer);
		}
		else
			i = 4;
	}
	i = row % 4;
	if (!i)
		i++;
	gotoxy (0, i);
	return (menusize);
}

/****************************************************************************************************/
/*	search text number for input or output															*/
/****************************************************************************************************/
WORD get_iotype_number (WORD number, BYTE io)
{
	WORD i = 0;

	while (iotype [i].number != ENDSTRING)
	{
		if ((iotype [i].number == number) && (io & iotype [i].io))
			return (i);
		i++;
	}
	return (0);
}

/****************************************************************************************************/
/*	search special function number																	*/
/****************************************************************************************************/
WORD get_doortext_number (struct _standardtext *pointer, WORD number)
{
	WORD i = 0;

	while (pointer [i].number != ENDSTRING)
	{
		if (pointer [i].number == number)
			return (i + 1);
		i++;
	}
	return (0);
}

/****************************************************************************************************/
/*	search special function number																	*/
/****************************************************************************************************/
WORD get_function_number (struct _specialtext *pointer, WORD number)
{
	WORD i = 0;

	while (pointer [i].number != ENDSTRING)
	{
		if (pointer [i].number == number)
			return (i + 1);
		i++;
	}
	return (0);
}

/****************************************************************************************************/
/*	display input or output parameter																*/
/****************************************************************************************************/
WORD display_io (WORD menunumber, WORD pos, WORD pcb)
{
	WORD i;
	WORD start;
	WORD row = 0;
	WORD func_row;
	WORD func;
	WORD sub_func;
	BYTE floor;
	BYTE buffer [20];
	WORD size;
	struct _textlist *pointer;
	struct _specialtext *tablepointer;

	clrscr ();
	start = pos / 4 * 4;							/* get number of 1. parameter						*/
	while ((iopara [row].menu != ENDSTRING) && (iopara [row].menu != menunumber))
		row++;										/* search parameter in structure					*/
	for (i = start; i < (start + 4); i++)			/* display 4 lines									*/
	{
		if (i < iopara [row].size)					/* dont exceed number of parameters					*/
		{
			sprintf ((char *)buffer, "%2u:", i + 1);			/* display input/output number						*/
			display (0, i % 4, buffer);
			if (menunumber == M_IN_FVE && i >= MAX_FVE_IN)
				func = p.fve_ex_inpar[i-MAX_FVE_IN][IO_BASIC_FUNC];
			else if (menunumber == M_IN_HSE && i >= MAX_HSE_IN)
				func = p.hse_ex_inpar[i-MAX_HSE_IN][IO_BASIC_FUNC];
			else if (menunumber == M_OUT_HSE && i >= MAX_HSE_OUT)
				func = p.hse_ex_outpar[i-MAX_HSE_OUT][IO_BASIC_FUNC];
			else
				func = *(iopara [row].pointer + (MAX_IO_TYPE*iopara[row].size*pcb*iopara[row].array) + (MAX_IO_TYPE * i));
			func_row = get_iotype_number (func, iopara [row].dir);	/* search row for function			*/
			if (func && !(func_row))				/* function is not in structure						*/
				sprintf ((char *)buffer, "%u", func);
			else
			{											/* read sub function								*/
				if (menunumber == M_IN_FVE && i >= MAX_FVE_IN)
				{
					sub_func = p.fve_ex_inpar[i-MAX_FVE_IN][IO_SUB_FUNC];
					floor = p.fve_ex_inpar[i-MAX_FVE_IN][IO_FLOOR];
				}
				else if (menunumber == M_IN_HSE && i >= MAX_HSE_IN)
				{
					sub_func = p.hse_ex_inpar[i-MAX_HSE_IN][IO_SUB_FUNC];
					floor = p.hse_ex_inpar[i-MAX_HSE_IN][IO_FLOOR];
				}
				else if (menunumber == M_OUT_HSE && i >= MAX_HSE_OUT)
				{
					sub_func = p.hse_ex_outpar[i-MAX_HSE_OUT][IO_SUB_FUNC];
					floor = p.hse_ex_outpar[i-MAX_HSE_OUT][IO_FLOOR];
				}
				else
				{
					sub_func = *(iopara [row].pointer+(MAX_IO_TYPE*iopara[row].size*pcb*iopara[row].array)+(MAX_IO_TYPE * i)+IO_SUB_FUNC);
					floor = *(iopara [row].pointer+(MAX_IO_TYPE*iopara[row].size*pcb*iopara[row].array)+(MAX_IO_TYPE * i)+IO_FLOOR);
				}
				buffer [0] = 0;
				switch (iotype [func_row].special)
				{
					case (S_HALL_CALL):
						sprintf ((char *)buffer, "%s %u", iotype [func_row].text [language], floor);
						if (sub_func == HALL_CALL_UP)
							buffer [strlen ((char *)iotype [func_row].text [language])] = 5;
						else if (sub_func == HALL_CALL_DOWN)
							buffer [strlen ((char *)iotype [func_row].text [language])] = 6;
						break;

					case (S_FUNC_SUB):
						sprintf ((char *)buffer, "%s %u", iotype [func_row].text [language], sub_func);
						break;

					case (S_FUNC_NUMBER):
						sprintf ((char *)buffer, "%s %u", iotype [func_row].text [language], floor);
						break;

					case (S_FUNC_SUBTEXT):
						size = get_stringnumber (iotype [func_row].pointer);
						if (sub_func && (sub_func < size))
						{
							pointer = iotype [func_row].pointer;
							sprintf ((char *)buffer, "%s %s", iotype [func_row].text [language], pointer [sub_func - 1].text [language]);
						}
						break;

					case (S_SUBTEXT):
						size = get_stringnumber (iotype [func_row].pointer);
						if (sub_func && (sub_func < size))
						{
							pointer = iotype [func_row].pointer;
							sprintf ((char *)buffer, "%s", pointer [sub_func - 1].text [language]);
						}
						break;

					case (S_SUBTABLE):
						sub_func = get_function_number (iotype [func_row].pointer, sub_func);
						size = get_tablesize (iotype [func_row].pointer);
						if (sub_func && (sub_func < size))
						{
							tablepointer = iotype [func_row].pointer;
							size = sprintf ((char *)buffer, "%s", tablepointer [sub_func - 1].text [language]);
							switch (func)
							{
								case (DRIVE_OUT):
									buffer [size + 1] = 0;
									if (floor == DIR_UP)
										buffer [size] = 5;
									else if (floor == DIR_DN)
										buffer [size] = 6;
									break;

								case (POSITION_INDICATOR):
									if (floor)
										sprintf ((char *)buffer + size, ":%u", floor);
									break;

								case (INSPECT_IN):
								case (POS_IN):
								case (EM_OP_IN):
									sprintf ((char *)buffer, "%s %s", iotype [func_row].text [language], tablepointer [sub_func - 1].text [language]);
									break;
							}
						}
						break;

					case (S_SUBTEXT_FLOOR):
						size = get_stringnumber (iotype [func_row].pointer);
						if (sub_func && (sub_func < size))
						{
							pointer = iotype [func_row].pointer;
							sprintf ((char *)buffer, "%s%u", pointer [sub_func - 1].text [language], floor);
						}
						break;

					case (S_FUNC_ARROW):
						if (sub_func)
						{
							sprintf ((char *)buffer, "%s	", iotype [func_row].text [language]);
							if (sub_func == ARROW_UP)
								buffer [strlen ((char *)buffer) - 1] = 5;
							else if (sub_func == ARROW_DOWN)
								buffer [strlen ((char *)buffer) - 1] = 6;
						}
						else
							sprintf ((char *)buffer, "%s", iotype [func_row].text [language]);
						break;
				}
			}
			if (!strlen ((char *)buffer))
				sprintf ((char *)buffer, "%s", iotype [func_row].text [language]);
			display (16 - strlen ((char *)buffer), i % 4, buffer);
		}
		else
		{
			display_line (T_ESC,	i % 4);				/* display return line								*/
			i = start + 4;							/* dont display more lines							*/
		}
	}
	gotoxy (0, pos % 4);							/* set cursor to beginning of 1. line				*/
	return (iopara [row].size + 1);				/* return total number of input/output parameters	*/
}

/****************************************************************************************************/
/*	read io parameter and write parameter to buffer													*/
/****************************************************************************************************/
WORD get_iopara (WORD menunumber, WORD paranumber, BYTE *para, WORD pcb)
{
	WORD row = 0;
	BYTE i;

	while ((iopara [row].menu != ENDSTRING) && (iopara [row].menu != menunumber))
		row++;										/* search parameter in structure					*/
	if ((menunumber == M_IN_FVE) && (paranumber >= MAX_FVE_IN))
	{
		for (i = 0; i < MAX_IO_TYPE; i++)
			para[i] = p.fve_ex_inpar[paranumber-MAX_FVE_IN][i];
	}
	else if ((menunumber == M_IN_HSE) && (paranumber >= MAX_HSE_IN))
	{
		for (i = 0; i < MAX_IO_TYPE; i++)
			para[i] = p.hse_ex_inpar[paranumber-MAX_HSE_IN][i];
	}
	else if ((menunumber == M_OUT_HSE) && (paranumber >= MAX_HSE_OUT))
	{
		for (i = 0; i < MAX_IO_TYPE; i++)
			para[i] = p.hse_ex_outpar[paranumber-MAX_HSE_OUT][i];
	}
	else
	{
		for (i = 0; i < MAX_IO_TYPE; i++)
			para[i] = *(iopara[row].pointer + (MAX_IO_TYPE*iopara[row].size*pcb*iopara[row].array) + (MAX_IO_TYPE*paranumber) + i);
	}
	return (iopara [row].dir);
}

/****************************************************************************************************/
/*	check if connected input / output is already used												*/
/****************************************************************************************************/
BYTE check_iopara (WORD menunumber, WORD paranumber, WORD pcb)
{
	WORD row = 0;
	WORD i;
	WORD value;

	while ((iopara [row].menu != ENDSTRING) && (iopara [row].menu != menunumber))
		row++;										/* search parameter in structure					*/
	i = iopara [row].checkmenu;
	value = *(iopara[row].pointer + (MAX_IO_TYPE*iopara[row].size*pcb*iopara[row].array) + (MAX_IO_TYPE*paranumber));
	if ((!value) && i && (paranumber >= iopara [row].start) && (paranumber <= iopara [row].stop))
	{
		row = 0;
		while ((iopara [row].menu != ENDSTRING) && (iopara [row].menu != i))
			row++;									/* search check parameter in structure				*/
		value = *(iopara[row].pointer + (MAX_IO_TYPE*iopara[row].size*pcb*iopara[row].array) + (MAX_IO_TYPE*paranumber));
		if (value)
			return (iopara[row].dir);
	}
	return (0);
}

/****************************************************************************************************/
/*	set complementary call																			*/
/****************************************************************************************************/
void set_callpara (WORD menunumber, WORD paranumber, WORD pcb)
{
	WORD row1 = 0;
	WORD row2 = 0;
	BYTE * ptr1;
	BYTE * ptr2;
	BYTE i;

	while ((iopara [row1].menu != ENDSTRING) && (iopara [row1].menu != menunumber))
		row1++;										/* search parameter in structure					*/
	while ((iopara [row2].menu != ENDSTRING) && (iopara [row2].menu != iopara [row1].checkmenu))
		row2++;										/* search parameter in structure					*/
	ptr2 = iopara[row2].pointer + MAX_IO_TYPE*iopara[row2].size*pcb*iopara[row2].array + MAX_IO_TYPE*paranumber;
	ptr1 = iopara[row1].pointer + MAX_IO_TYPE*iopara[row1].size*pcb*iopara[row1].array + MAX_IO_TYPE*paranumber;
	for (i = 0; i < MAX_IO_TYPE; i++)
	{
		*(ptr2 + i) = *(ptr1 + i);
	}
}

/****************************************************************************************************/
/*	write new parameter value from buffer															*/
/****************************************************************************************************/
BYTE write_iopara (WORD menunumber, WORD paranumber, BYTE *para, WORD pcb)
{
	WORD row = 0;
	BYTE * ptr;
	BYTE i;

	while ((iopara [row].menu != ENDSTRING) && (iopara [row].menu != menunumber))
		row++;										/* search parameter in structure					*/

	if ((menunumber == M_IN_FVE) && (paranumber >= MAX_FVE_IN))
		ptr = &p.fve_ex_inpar[paranumber-MAX_FVE_IN][0];
	else if ((menunumber == M_IN_HSE) && (paranumber >= MAX_HSE_IN))
		ptr = &p.hse_ex_inpar[paranumber-MAX_HSE_IN][0];
	else if ((menunumber == M_OUT_HSE) && (paranumber >= MAX_HSE_OUT))
		ptr = &p.hse_ex_outpar[paranumber-MAX_HSE_OUT][0];
	else
		ptr = iopara[row].pointer + MAX_IO_TYPE*iopara[row].size*pcb*iopara[row].array + MAX_IO_TYPE*paranumber;
	for (i = 0; i < MAX_IO_TYPE; i++)						/* write parameter to buffer						*/
		*(ptr+i) = para[i];
	switch (menunumber)
	{
		case (M_IN_TSE):
		case (M_OUT_TSE):
			if (!bit_select (tse_enabled, pcb))
			{
				if (para [0])
					bit_set (tse_enabled, pcb);
			}
			break;
			
		case (M_IN_EXE):
		case (M_OUT_EXE):
			if (!bit_select (exe_enabled, pcb))
				{
					if (para [0])
						bit_set (exe_enabled, pcb);
				}
			break;

		case (M_IN_ESE):
		case (M_OUT_ESE):
			if (!bit_selectd (ese_enabled [pcb / 32], pcb % 32))
			{
				if (para [0])
					bit_setd (ese_enabled [pcb / 32], pcb % 32);
			}
			break;
	}
	switch (para [0])
	{
		case (HALL_CALL):
		case (HALL_CALL_SPECIAL):
		case (HALL_CALL_ADVANCED):
		case (HALL_CALL_EMERGENCY):
		case (CAR_CALL):
		case (0):
			if (iopara [row].checkmenu && (paranumber >= iopara [row].start) && (paranumber <= iopara [row].stop))
			{
				if (para [0])
					return (iopara [row].dir);
				else
					return (iopara [row].dir + 2);
			}
	}
	return (0);
}

/****************************************************************************************************/
/*	check detail parameter for min and max															*/
/****************************************************************************************************/
void check_min_max (BYTE *para, BYTE io)
{
	WORD func;

	func = get_iotype_number (para [0], io);		/* search row for function							*/
	if (iotype [func].special == S_FUNC_SUB)
	{
		if ((para [1] < (p.bot_floor + 1)) || (para [1] > (p.top_floor + 1)))
			para [1] = 0;
	}
	if ((iotype [func].special == S_SUBTEXT_FLOOR) && (para [1] != ONE_OF_N))
	{
		if (para [3] > 6)
			para [3] = 0;
	}
	else if ((iotype [func].special == S_SUBTABLE) && (para [0] == DRIVE_OUT))
	{
		if (para [3] > DIR_DN)
			para [3] = 0;
	}
	else
	{
		if ((para [3] < (p.bot_floor + 1)) || (para [3] > (p.top_floor + 1)))
			para [3] = 0;
	}
}

/****************************************************************************************************/
/*	get detail parameter number																		*/
/****************************************************************************************************/
BYTE get_detail_para (BYTE row, BYTE filter)
{
	BYTE i;
	BYTE j = 0;

	for (i = 0; i < 8; i++)
	{
		if ((1 << i) & filter)
		{
			if (j == row)
				return (i);
			j++;
		}
	}
	return (row);
}

/****************************************************************************************************/
/*	check if it is necessary to disable direct access with xxx*										*/
/****************************************************************************************************/
BYTE check_accesstype (BYTE *para, BYTE row, BYTE io, BYTE bus)
{
	WORD func;
	WORD pos;
	BYTE filter;

	func = get_iotype_number (para [0], io);	/* search row for function							*/
	filter = iotype [func].filter | 0x04;
	if (iotype [func].special == S_SUBTABLE)
	{
		pos = get_function_number (iotype [func].pointer, para [1]);
		if (pos)
			filter = ((struct _specialtext *)iotype [func].pointer) [pos - 1].filter | 0x04;
	}
	pos = get_detail_para (row, filter);
	switch (pos)
	{
		case (0):
		case (5):
			return (0);

		case (1):
			switch (iotype [func].special)
			{
				case (S_HALL_CALL):
				case (S_FUNC_ARROW):
				case (S_FUNC_SUBTEXT):
				case (S_SUBTEXT):
				case (S_SUBTEXT_FLOOR):
				case (S_SUBTABLE):
					return (0);
			}
			break;

		default:
			return (1);
	}
	return (1);
}

/****************************************************************************************************/
/*	set new detail parameter value																	*/
/****************************************************************************************************/
void change_iopara (BYTE *para, BYTE row, BYTE io, WORD key, BYTE new, BYTE bus)
{
	WORD func;
	WORD value;
	BYTE max;
	BYTE buffer [20];
	BYTE pos;
	BYTE i;
	BYTE filter;
	struct _specialtext *tablepointer;

	func = get_iotype_number (para [0], io);		/* search row for function							*/
	filter = iotype [func].filter | 0x04;
	if (iotype [func].special == S_SUBTABLE)
	{
		value = get_function_number (iotype [func].pointer, para [1]);
		if (value)
			filter = ((struct _specialtext *)iotype [func].pointer) [value - 1].filter | 0x04;
	}
	pos = get_detail_para (row, filter);
	switch (pos)
	{
		case (0):
			if ((key != '*') && (key != KEY_UP))
				return;
			if (key == '*')							/* key down											*/
			{
				do func++;								/* next value										*/
				while (!(io & iotype [func].io));		/* check for input or output						*/
				if (iotype [func].number == ENDSTRING)
				func = 0;
			}
			else										/* key up (simulated with 4*)						*/
			{
				i = 0;
				while (iotype [i].number != ENDSTRING)	/* get number of strings							*/
					i++;
				do
				{
					if (func)
						func--;
					else
						func = i - 1;
				} while (!(io & iotype [func].io));		/* check for input or output						*/
			}
			para [0] = iotype [func].number;
			for (value = 1; value < 8; value++)
				para [value] = 0;
			para [2] = LIFT1;
			break;

		case (1):
			value = para [1];
			switch (iotype [func].special)
			{
				case (S_HALL_CALL):
				case (S_FUNC_ARROW):
				case (S_FUNC_SUBTEXT):
				case (S_SUBTEXT):
				case (S_SUBTEXT_FLOOR):
					if ((key != '*') && (key != KEY_UP))
						return;
					max = get_stringnumber (iotype [func].pointer);
					if (key == '*')							/* key down											*/
					{
						if (value < (max - 1))
							value++;
						else
							value = 0;
					}
					else										/* key up (simulated with 4*)						*/
					{
						if (value)
							value--;
						else
							value = max - 1;
					}
					para [1] = value;
					break;

				case (S_SUBTABLE):
					value = get_function_number (iotype [func].pointer, para [1]);
					if ((key != '*') && (key != KEY_UP))
						return;
					max = get_tablesize (iotype [func].pointer);
					if (key == '*')							/* key down											*/
					{
						if (value < (max - 1))
							value++;
						else
							value = 0;
					}
					else										/* key up											*/
					{
						if (value)
							value--;
						else
							value = max - 1;
					}
					tablepointer = iotype [func].pointer;
					if (value)
						para [1] = tablepointer [value - 1].number;
					else
						para [1] = 0;
					para [3] = 0;								/* if sub function changed clear this values		*/
					para [4] = 0;
					para [5] = 0;
					break;

				case (S_FUNC_SUB):
					if (key == '*')
						return;
					if (new)
						para [1] = 0;
					if (para [1])
						sprintf ((char *)buffer, "%u", para [1]);
					else
						buffer [0] = 0;
					max = strlen ((char *)buffer);
					if (max < 2)
					{
						buffer [max] = key;
						buffer [max + 1] = 0;
					}
					sscanf ((const char *)buffer, "%u", (DWORD *)&value);
					if (value <= p.top_floor + 1)
						para [1] = value;
					break;
			}
			break;

		case (2):
			if ((key != '*') && (key != KEY_UP))
				return;
			if (para [IO_ENABLE] & ENABLE)
				para [IO_ENABLE] = DISABLE;
			else
				para [IO_ENABLE] = ENABLE;
			break;
		case (4):	
			if((((key >= '0') && (key <= '3')) && (p.doornumber > 1)) ||
					(((key >= '0') && (key <= '1')) && (p.doornumber == 1)))
				{
					para [4] = key - '0';
				}			
			break;

		case (3):
			if (para [0] == DRIVE_OUT)
			{
				if ((key != '*') && (key != KEY_UP))
					return;
				if (key == '*')							/* key down											*/
				{
					if (para [3] < 2)
						para [3]++;
					else
						para [3] = 0;
				}
				else									/* key up											*/
				{
					if (para [3])
						para [3]--;
					else
						para [3] = 2;
				}
			}
			else
			{
				if (key == '*')
					return;
				if (new)
					para [3] = 0;
				if (para [3])
					sprintf ((char *)buffer, "%u", para [3]);
				else
					buffer [0] = 0;
				max = strlen ((char *)buffer);
				if (max < 2)
				{
					buffer [max] = key;
					buffer [max + 1] = 0;
				}
				sscanf ((const char *)buffer, "%d", (DWORD *)&value);
				if ((iotype [func].special == S_SUBTEXT_FLOOR) && (para [1] != ONE_OF_N))
					max = 6;
				else if (iotype [func].special == S_FUNC_NUMBER)
					max = 8;
				else
					max = p.top_floor + 1;
				if (value <= max)
					para [3] = value;
			}
			break;

		case (5):
			if ((key != '*') && (key != KEY_UP))
				return;
			if (para [5] & POLARITY_BIT)
				para [5] = 0;
			else
				para [5] = POLARITY_BIT;
			break;
	}
}

/****************************************************************************************************/
/*	read and write parameter details																*/
/****************************************************************************************************/
BYTE iopara_detail (BYTE *para, BYTE pos, BYTE io, BYTE bus)
{
	WORD i, j;
	BYTE start;
	BYTE buffer [20];
	WORD func;
	WORD sub_func;
	BYTE size;
	BYTE row = 0;
	BYTE max_row = 0;
	BYTE filter;
	struct _textlist *pointer;
	struct _specialtext *tablepointer;

	clrscr ();
	start = pos / 4 * 4;							/* get number of 1. parameter						*/
	func = get_iotype_number (para [0], io);		/* search row for function							*/
	filter = iotype [func].filter | 0x04;					/* test */
	if (iotype [func].special == S_SUBTABLE)
	{
		i = get_function_number (iotype [func].pointer, para [1]);
		if (i)
			filter = ((struct _specialtext *)iotype [func].pointer) [i - 1].filter | 0x04;
	}
	for (i = 0; i < 8; i++)							/* display lines									*/
	{
		if (((1 << i) & filter) || (func == ENDSTRING))						/* display this detail parameter					*/
		{
			if ((max_row >= start) && ((max_row - start) < 4))
			{
				if (func == ENDSTRING)					/* display general parameter name					*/
					display (0, row, (BYTE *)paradetail [i].text [language]);
				else									/* display parameter name like specified in table	*/
					display (0, row, (BYTE *)(iotype [func].detail) [i].text [language]);
				buffer [0] = 0;							/* clear buffer										*/
				if (func == ENDSTRING)
					sprintf ((char *)buffer, "%u", para [i]);	/* display parameter as number						*/
				else if (!para [0])						/* no function; parameter not used					*/
					sprintf ((char *)buffer, "--");				/* display parameter as number						*/
				else
				{
					switch (i)
					{
						case (0):
							sprintf ((char *)buffer, "%s", iotype [func].text [language]);
							break;

						case (1):
							sub_func = para [1];
							if (sub_func)
								sprintf ((char *)buffer, "%u", sub_func);
							else
								sprintf ((char *)buffer, "--");
							switch (iotype [func].special)
							{
								case (S_HALL_CALL):
								case (S_FUNC_ARROW):
								case (S_SUBTEXT_FLOOR):
								case (S_FUNC_SUBTEXT):
								case (S_SUBTEXT):
									size = get_stringnumber (iotype [func].pointer);
									if (sub_func && (sub_func < size))
									{
										pointer = iotype [func].pointer;
										sprintf ((char *)buffer, "%s", pointer [sub_func - 1].text [language]);
									}
									break;

								case (S_SUBTABLE):
									sub_func = get_function_number (iotype [func].pointer, para [1]);
									size = get_tablesize (iotype [func].pointer);
									if (sub_func && (sub_func < size))
									{
										tablepointer = iotype [func].pointer;
										sprintf ((char *)buffer, "%s", tablepointer [sub_func - 1].text [language]);
									}
									break;
							}
							break;

						case (2):
							sprintf ((char *)buffer, "%s", enable_io [para [IO_ENABLE]].text [language]);
							break;

						case (4):	
							for (j = 0; j < 7; j++)
								buffer [j] = ' ';							
							buffer [7] = '0' + para [4];
							buffer [8] = 0;
							break;

						case (3):
							if ((para [0] == DRIVE_OUT) && (para [3] <= 2))
								sprintf ((char *)buffer, "%s", controldir [para [3]].text [language]);
							else
								sprintf ((char *)buffer, "%u", para [3]);
							break;

						case (5):
							sprintf ((char *)buffer, "%s", polarity [para [5] >> 7].text [language]);
							break;
					}
				}
				if (i < 6)
					display (16 - strlen ((char *)buffer), row, buffer);
				row++;
			}
			max_row++;
		}
	}
	gotoxy (0, pos % 4);							/* set cursor to beginning of 1. line				*/
	return (max_row);
}

WORD get_pcb_hb_status(WORD menu, WORD row, WORD max)
{
	WORD start, status = 0;

	start = ((row%32) / 4) * 4;
	if (menu == M_TSE)
		status = (tse_hb_state >> start) & 0x0F;
	else if (menu == M_EXE)
		status = (exe_hb_state >> start) & 0x0F;
	else if (menu == M_ESE)
		status = (ese_hb_state[row/32] >> start) & 0x0F;
	return status;
}

void display_pcb_status(WORD status, WORD row, WORD max)
{
	WORD i, start;

	start = (row/4) * 4;
	for (i = 0; i < MAX_LCD_ROW; i++)
	{
		if (start + i >= max - 1)
			break;
		if (status & c_bit_select[i])
			display(15, i, "*");
		else
			display(15, i, "-");
	}
	gotoxy (0, row % MAX_LCD_ROW);
}

/****************************************************************************************************/
/*	Display menu to select ESE number																*/
/****************************************************************************************************/
void display_pcb_number (WORD menu, WORD row, WORD max)
{
	WORD i, start;
	BYTE buf1[10];
	BYTE buffer [20];

	clrscr ();
	start = (row / 4) * 4;
	for (i = start; i < max; i++)
	{
		if (i < max - 1)
		{
			if (menu == M_TSE || menu == M_TSE_IN_LIST || menu == M_TSE_OUT_LIST)
			{
				sprintf ((char *)buffer, "PCU %u", i + 1);
				display (0, i - start, buffer);
			} 
			else if (menu == M_EXE || menu == M_EXE_IN_LIST || menu == M_EXE_OUT_LIST)
			{
				sprintf ((char *)buffer, "EXU %u", i + 1);
				display (0, i - start, buffer);
			} 
			else
			{
				GetLcuConfig(i, buf1);
				trim(&buf1[2]);
				sprintf((char *)buffer, "LCU %s", &buf1[2]);
				display (0, i - start, buffer);
			}
		}
		else
		{
			display_line (T_ESC, i - start);			/* display return line								*/
		}
		if (i - start + 1 >= MAX_LCD_ROW)
			break;
	}
}

void GetLcuConfig(BYTE item, BYTE * buffer){
	BYTE i, j, k, d = 0;

	k = 0;
	for (i=p.bot_floor; i<=p.top_floor; ++i)
	{
		for (j=0; j<p.doornumber; ++j)
		{
			if (p.doorpos[i] & (1 << j))
			{
				d = i * MAX_DOOR + j;
				if (k == item)
				{
					buffer[2] = ese_dispID[d][0];
					buffer[3] = ese_dispID[d][1];
					buffer[4] = ese_dispID[d][2];
					buffer[5] = '\0';					
					return;
				}
				else
					++ k;
			}
		}
	}

	buffer[2] = '0' + (item + 1) / 10;
	buffer[3] = '0' + ((item + 1) % 10);
	buffer[4] = '-';
	buffer[5] = '\0';
}

//读取当前外呼传送的ID号
void GetIDDispBuffer(BYTE item, BYTE * buffer){
	BYTE i, j, k;

	buffer[0] = SET_NODE_ID;
	buffer[1] = item + ESE_ID;
	k = 0;
	for (i=p.bot_floor; i<=p.top_floor; ++i)
	{
		for (j=0; j<p.doornumber; ++j)
		{
			if (k == item)
			{
				buffer[2] = (item + 1) / 10;
				if (!buffer[2])
					buffer[2] = 0x20;
				else
					buffer[2] += '0';
				buffer[3] = ((item + 1) % 10) + '0';		

				buffer[4] = 'A' + j;
				buffer[5] = '\0';
				return;
			}
			else
				++ k;
		}
	}

	buffer[2] = '0' + (item+1)/10;
	buffer[3] = '0' + ((item+1)%10);
	buffer[4] = '-';
	buffer[5] = '\0';
}


float GetLoadVoltageBuff(BYTE * str, WORD value)
{
	float vol = 0;

	if (p.loadtype == ANALOG_ON_CDU)
	{
		vol = value * 10;
		vol = vol / 1023;		//10bit AD 转换
	}
	else if (p.loadtype == ANALOG_ON_MCU)
	{
		vol = value * 10.93f;
		vol = vol / 4095;
	}
	sprintf((char *)str, "%.2fV", vol);
	return vol;
}

/****************************************************************************************************/
/*	search a sub menu for a menu point																*/
/****************************************************************************************************/
WORD search_submenu (WORD menunumber, WORD pos, BYTE * right)
{
	WORD i = 0;
	WORD menupos = 0;

	while (menutext [i].number != ENDSTRING)
	{
		if (menutext [i].number == menunumber)
		{
			if (menupos == pos)
			{
				*right = menutext [i].right;
				return (menutext [i].submenu);
			}
			menupos++;
		}
		i++;
	}
	*right = RIGHT_USER;
	return (M_STATUS1);
}

/****************************************************************************************************/
/*	search the higher level menu of the actual menu													*/
/****************************************************************************************************/
WORD search_mainmenu (WORD menunumber)
{
	WORD i = 0;

	while (menutext [i].number != ENDSTRING)
	{
		if (menutext [i].submenu == menunumber)
			return (menutext [i].number);
		i++;
	}
	return (M_MAIN);
}

/****************************************************************************************************/
/*	search row number in previous menu																*/
/****************************************************************************************************/
BYTE search_row (WORD menunumber, WORD submenunumber)
{
	WORD i = 0;
	BYTE menupos = 0;
	BYTE menusize = 0;

	while (menutext [i].number != ENDSTRING)
	{
		if (menutext [i].number == menunumber)
		{
			if (menutext [i].submenu == submenunumber)
				menupos = menusize;
			menusize++;
		}
		i++;
	}
	if (menusize == (menupos + 1))					/* menu point "Zur點k"							*/
		menupos = 0;
	return (menupos);
}

/****************************************************************************************************/
/*	search parameter type																			*/
/****************************************************************************************************/
WORD search_paranumber (WORD menunumber, WORD pos)
{
	WORD i = 0;
	BYTE menupos = 0;
	BYTE menusize;

	while (paratext [i].menu != ENDSTRING)
	{
		if (paratext [i].menu == menunumber)
		{
			if (paratext [i].array)						/* array parameter								*/
			{
				menusize = (p.top_floor - p.bot_floor + 1) * paratext [i].array;
				if (pos < menusize)
					return (i);
			}
			else											/* single parameter								*/
			{
				if (menupos == pos)
					return (i);
			}
			menupos++;
		}
		i++;
	}
	return (i);
}

/****************************************************************************************************/
/*	display actual call table																		*/
/****************************************************************************************************/
void display_calls (BYTE new, BYTE start, BYTE stop, BYTE menu)
{
	BYTE i;
	static BYTE carcall [20];
	static BYTE hallcall [20];
	static BYTE carcall_old [20];
	static BYTE hallcall_old [20];
	BYTE show = 0;

	if (new)
	{
		if (menu)
		{
			display (0, 0, "	");
			sprintf ((char *)carcall, "%02u", start + 1);
			display (0, 0, carcall);
			sprintf ((char *)carcall, "%02u", stop + 1);
			display (14, 0, carcall);
			clearline (1);
			clearline (3);
		}
		for (i = 0; i < 20; i++)
		{
			carcall [i] = ' ';
			hallcall [i] = ' ';
			carcall_old [i] = 0xFF;
			hallcall_old [i] = 0xFF;
		}
		if (menu)
		{
			carcall [16] = 0;
			hallcall [16] = 0;
		}
		else
		{
			carcall [8] = 0;
			hallcall [8] = 0;
		}
	}
	for (i = start; i <= stop; i++)
	{
		if ((i >= p.bot_floor) && (i <= p.top_floor))
		{
			if (calltab [i].calltype & PRIOR_CARCALL)
				carcall [i % start] = 'V';
			else if (calltab [i].calltype & CARCALL)
				carcall [i % start] = '*';
			else if (!(calls_enabled & (CARCALL | PRIOR_CARCALL)))
				carcall [i % start] = 2;
			else
				carcall [i % start] = '-';
			if ((!(calls_enabled & ALL_HALL_CALLS)) &&
			(!(calltab [i].calltype & (ALL_HALL_CALLS | EMERGENCY_CALL | ADVANCED_CALL | SPECIAL_CALL))))
				hallcall [i % start] = 2;
			else
			{
				if (calltab [i].calltype & EMERGENCY_CALL)
					hallcall [i % start] = 'E';
				else if (calltab [i].calltype & ADVANCED_CALL)
					hallcall [i % start] = 'V';
				else if (calltab [i].calltype & SPECIAL_CALL)
					hallcall [i % start] = 'S';
				else
				{
					switch (calltab [i].calltype & 0x0E)
					{
						case (0):
							hallcall [i % start] = '-';
							break;
						case (2):
							hallcall [i % start] = 5;
							break;
						case (4):
							hallcall [i % start] = 6;
							break;
						default:
							hallcall [i % start] = 7;
							break;
					}
				}
			}
		}
	}
	if ((hse_state == H_MOVING) || (hse_state == H_ARRIVAL) ||
			(hse_state == H_START) || (hse_state == H_RESET))
	{
		if ((start_floor <= p.top_floor) && (start_floor >= p.bot_floor)
		&& (start_floor <= stop) && (start_floor >= start))
			carcall [start_floor % start] = 'S';
		if ((target_floor <= p.top_floor) && (target_floor >= p.bot_floor)
		&& (target_floor <= stop) && (target_floor >= start))
		{
			if (parkcall)
				carcall [target_floor % start] = 'P';
			else
				carcall [target_floor % start] = 'T';
		}
	}
	if ((level <= p.top_floor) && (level >= p.bot_floor) && (level <= stop) && (level >= start))
	{
		if (direction == DIR_DN)
		{
			if (hse_state == H_ARRIVAL)
				carcall [level % start] = '<';
			else
				carcall [level % start] = 3;
		}
		else if (direction == DIR_UP)
		{
			if (hse_state == H_ARRIVAL)
				carcall [level % start] = '>';
			else
				carcall [level % start] = 4;
		}
		else
			carcall [level % start] = 0xFF;
	}
	for (i = 0; i < 20; i++)
	{
		if (carcall	[i] != carcall_old [i])
			show |= 1;
		if (hallcall [i] != hallcall_old [i])
			show |= 2;
		carcall_old [i] = carcall [i];
		hallcall_old [i] = hallcall [i];
	}
	if (show & 0x01)
		display (0, menu, carcall);
	if (show & 0x02)
		display (0, 2 * menu + 1, hallcall);
}

const BYTE c_bit_select[] =
{
	0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80,
};


