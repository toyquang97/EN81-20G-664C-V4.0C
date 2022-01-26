
#ifndef _LCD_H_
#define _LCD_H_

#include "text.h"

#define _NOCURSOR    		0x0C
#define _SOLIDCURSOR	0x0F
#define _NORMALCURSOR	0x0E


extern const BYTE c_bit_select[];

void clrscr (void);
void clearline (BYTE line);
void display_line (BYTE textnumber, BYTE row);
void display_standardtext (BYTE textnumber);
void display_iv_status_text (BYTE textnumber);
void display_text_from_line (BYTE textnumber, BYTE start);
void display_paravalue (void *parapointer, WORD parapos, WORD pos, WORD offset);
WORD display_paralist (WORD menunumber, WORD pos);
WORD display_hist_detail (WORD number, WORD row);
WORD get_pcb_hb_status(WORD menu, WORD row, WORD max);
void display_pcb_status(WORD status, WORD row, WORD max);
void display_pcb_number (WORD menu, WORD row, WORD max);
void GetIDDispBuffer(BYTE item, BYTE * buffer);
float GetLoadVoltageBuff(BYTE * str, WORD value);
WORD display_io (WORD menunumber, WORD pos, WORD pcb);
void display_calls (BYTE new, BYTE start, BYTE stop, BYTE menu);
void display_error (WORD textnumber, BYTE row, WORD id);
WORD display_menu (WORD menunumber, WORD pos);
DWORD change_listpara (WORD number, WORD pos, DWORD value, BYTE searchdir);
void cursor (BYTE type);
void gotoxy (BYTE column, BYTE row);
void display (BYTE column, BYTE row, BYTE * ptr);
WORD get_stringnumber (struct _textlist * pointer);
WORD get_function_number (struct _specialtext *pointer, WORD number);
WORD search_mainmenu (WORD menunumber);
WORD get_doortext_number (struct _standardtext *pointer, WORD number);
WORD search_paranumber (WORD menunumber, WORD pos);
WORD search_submenu (WORD menunumber, WORD pos, BYTE * right);
BYTE search_row (WORD menunumber, WORD submenunumber);
void check_min_max (BYTE *para, BYTE io);
BYTE iopara_detail (BYTE *para, BYTE pos, BYTE io, BYTE bus);
void change_iopara (BYTE *para, BYTE row, BYTE io, WORD key, BYTE new, BYTE bus);
BYTE check_iopara (WORD menunumber, WORD paranumber, WORD pcb);
void set_callpara (WORD menunumber, WORD paranumber, WORD pcb);
BYTE check_accesstype (BYTE *para, BYTE row, BYTE io, BYTE bus);
BYTE write_iopara (WORD menunumber, WORD paranumber, BYTE *para, WORD pcb);
WORD get_iopara (WORD menunumber, WORD paranumber, BYTE *para, WORD pcb);
BYTE next_sign (BYTE sign, BYTE searchdir);
void transmit_lcd (void);
void GetLcuConfig(BYTE item, BYTE * buffer);

#endif

