
#include "typedef.h"
#include "string.h"
#include "parameter.h"
#include "state.h"
#include "can.h"
#include "text.h"
#include "lcd.h"
#include "io.h"
#include "stdio.h"


#define	HW_INIT_READ_NAME		0				/* init request of device name				*/
#define	HW_READ_SEGM_NAME	1				/* read segment request of device name		*/
#define	HW_INIT_READ_HW		2				/* init read request hardware version		*/
#define	HW_READ_SEGM_HW		3				/* read segment request of hardware version	*/
#define	HW_INIT_READ_SW		4				/* init read request software version		*/
#define	HW_READ_SEGM_SW		5				/* read segment request of software version	*/
#define	HW_DISP_SW			6
#define	HW_INIT_READ_IN			7				/* read request inputs						*/
#define	HW_INIT_READ_OUT		8				/* read request outputs					*/
#define	HW_READ_IN				9				/* read request inputs						*/
#define	HW_WRITE_OUT			10				/* write request outputs					*/

/****************************************************************************************************/
/* write sign for type of IO to buffer																*/
/****************************************************************************************************/
void write_io (BYTE * buffer, BYTE * para, BYTE size, BYTE io)
{
	WORD i, j;
	BYTE func;
	struct _specialtext * tablepointer;

	for (i = 0; i < size; i++)
	{
		j = 0;
		buffer [i] = '-';
		if ((para == (BYTE *)&p.fve_inpar[0][0]) && (i >= MAX_FVE_IN))
			func = p.fve_ex_inpar [i-MAX_FVE_IN][IO_BASIC_FUNC];
		else if ((para == (BYTE *)&p.hse_inpar[0][0]) && (i >= MAX_HSE_IN))
			func = p.hse_ex_inpar [i-MAX_HSE_IN][IO_BASIC_FUNC];
		else if ((para == (BYTE *)&p.hse_outpar[0][0]) && (i >= MAX_HSE_OUT))
			func = p.hse_ex_outpar [i-MAX_HSE_OUT][IO_BASIC_FUNC];
		else
			func = para [i * MAX_IO_TYPE + IO_BASIC_FUNC];
		while (((iotype [j].number != func) || (!(iotype [j].io & io))) && (iotype [j].number != ENDSTRING))
			j++;
		if (iotype [j].special == S_SUBTABLE)
		{
			tablepointer = iotype [j].pointer;
			if ((para == (BYTE *)&p.fve_inpar[0][0]) && (i >= MAX_FVE_IN))
				j = get_function_number (tablepointer, p.fve_ex_inpar [i-MAX_FVE_IN][IO_SUB_FUNC]);
			else if ((para == (BYTE *)&p.hse_inpar[0][0]) && (i >= MAX_HSE_IN))
				j = get_function_number (tablepointer, p.hse_ex_inpar [i-MAX_HSE_IN][IO_SUB_FUNC]);
			else if ((para == (BYTE *)&p.hse_outpar[0][0]) && (i >= MAX_HSE_OUT))
				j = get_function_number (tablepointer, p.hse_ex_outpar [i-MAX_HSE_OUT][IO_SUB_FUNC]);
			else
				j = get_function_number (tablepointer, para [i * MAX_IO_TYPE + IO_SUB_FUNC]);
			if (j)
				buffer [i] = tablepointer [j - 1].sign [language];
		}
		else
		{
			buffer [i] = iotype [j].sign [language];
		}
	}
	buffer [i] = 0;
}

/****************************************************************************************************/
/* display device name, version number and state of all inputs and outputs							*/
/****************************************************************************************************/
BYTE hwtest (BYTE state, WORD pcb, WORD number, BYTE *column, BYTE *row, BYTE * page, BYTE key)
{
	BYTE i;
	BYTE buf1 [10];
	BYTE buffer [20];
	BYTE dat = 0;
	BYTE flash = 0;
	BYTE inverse = 0;
	DWORD in;
	DWORD out = 0;
	static BYTE * inpara = NULL;
	static BYTE * outpara = NULL;
	static WORD node_id, id;
	static BYTE bus;
	static WORD * sdo;
	static BYTE *sdodata;
	static DWORD in_old;
	static DWORD out_old;
	static BYTE instate [24];
	static BYTE outstate [24];
	static BYTE data [8];
	static BYTE in_new;
	static BYTE out_new;
	static BYTE size_in;
	static BYTE size_out;
	static BYTE no_response_count = 0;

	if (key)
	{
		switch (pcb)
		{
			case (M_FVE_INFO):
				if (key == 1)
				{//下箭头
					if (((*column >= MAX_FVE_OUT-1) && (*row == 3)) ||
						(*column >= 15) ||
						((*row == 2) && (((*column >= 1) && (!fve_hardware_ver_after_v13)) || ((*column >= 2) && (fve_hardware_ver_after_v13)))))
					{
						*column = 0;
						if (*row == 3)
							*row = 1;
						else
							++ (*row);
					}
					else
						(*column)++;
				}
				else
				{
					if (!(*column))
					{
						if (*row == 1)
						{
							*row = 3;
							*column = MAX_FVE_OUT-1;
						}
						else if (*row == 3)
						{
							*row = 2;
							if (!fve_hardware_ver_after_v13)
								*column = 1;
							else
								*column = 2;
						}
						else
						{
							*row = 1;
							*column = 15;
						}
					}
					else
						(*column)--;
				}
				break;

			case (M_TSE_INFO):
				if (key == 1)
				{
					if (*row == 2)
					{
						if ((*column-8+(*page*8)+1 >= MAX_TSE_IN) || (*column >= 15))
						{
							*column = 8;
							*row = 3;
						}
						else
							++ (*column);
					}
					else if (*row == 3)
					{
						if ((*column-8+(*page*8)+1 >= MAX_TSE_OUT) || (*column >= 15))
						{
							if (*column-8+(*page*8)+1 >= MAX_TSE_OUT)
								*page = 0;		
							else
								++ (*page);		
							*column = 8;
							*row = 2;
							state = HW_INIT_READ_IN;
						}
						else
							++ (*column);
					}
				}
				else
				{
					if (*column > 8)
						-- (*column);
					else
					{
						if (*row == 2)
						{
							if (!(*page))
							{
								*page = (MAX_TSE_OUT+7)/8 - 1;
								*column = 8 + ((MAX_TSE_OUT-1)%8);
								*row = 3;
							}
							else
							{
								-- (*page);
								*column = 15;
								*row = 3;
							}
							state = HW_INIT_READ_IN;
						}
						else if (*row == 3)
						{
							if (MAX_TSE_IN > (*page*8)+8)
								*column = 15;
							else
								*column = 8 + ((MAX_TSE_IN-1)%8);
							*row = 2;
						}
					}
				}
				break;

			case (M_ESE_INFO):
			case (M_EXE_INFO):
				if (key == 1)
				{
					if (*column >= 15)
					{
						*column = 8;
						if (*row == 2)
							*row = 3;
						else
							*row = 2;
					}
					else
						(*column)++;
				}
				else
				{
					if (*column <= 8)
					{
						*column = 15;
						if (*row == 2)
							*row = 3;
						else
							*row = 2;
					}
					else
						(*column)--;
				}
				break;
		}

		gotoxy (*column, *row);
	}

	if (((pcb == M_TSE_INFO) && tse_sdo_master) ||
			((pcb == M_ESE_INFO) && ese_sdo_master) ||
			((pcb == M_EXE_INFO) && exe_sdo_master) ||
			((pcb == M_FVE_INFO  && sdo_master)))
		{
			sdo_interrupt = 0;
			return (state);
		}
	sdo_interrupt = 1;
	switch (state)
	{
		case (HW_INIT_READ_NAME):						/* init read request of device name				*/
			data [0] = INIT_READ_REQ;
			data [1] = (BYTE)DEVICE_NAME;
			data [2] = DEVICE_NAME >> 8;
			switch (pcb)
			{
				case (M_TSE_INFO):
					node_id = id = TSE_ID + number;
					bus = CAN_A;
					sdo = &tse_sdo_id;
					sdodata = tse_sdo_data;
					break;
					
				case (M_ESE_INFO):
					node_id = id = ESE_ID + number;
					bus = CAN_B;
					sdo = &ese_sdo_id;
					sdodata = ese_sdo_data;
					break;
					
				case (M_EXE_INFO):
					node_id = id = EXE_ID + number;
					if(node_id == EXE_ID)
						bus = CAN_A;
					else if(node_id == EXE_ID + 1)
						bus = CAN_B;
					sdo = &exe_sdo_id;
					sdodata = exe_sdo_data;
					break;
					
				case (M_FVE_INFO):
					node_id = id = FVE_ID;
					bus = CAN_A;
					sdo = &sdo_id;
					sdodata = sdo_data;
					break;
			}
			for (i = 3; i < 8; i++)
				data [i] = 0;
			*sdo = 0xFFFF;
			send_message (RSDO, id, 8, data, bus);
			return (HW_READ_SEGM_NAME);

		case (HW_READ_SEGM_NAME):						/* read segment request of device name			*/
			data [0] = READ_SEGM_REQ;
			for (i = 1; i < 8; i++)
				data [i] = 0;
			send_message (RSDO, id, 8, data, bus);
			*sdo = 0xFFFF;
			return (HW_INIT_READ_HW);

		case (HW_INIT_READ_HW):						/* init read request hardware version			*/
			if (*sdo == node_id)							/* answer from PCB								*/
			{
				for (i = 0; i < 7; i++)						/* read PCB name								*/
					data [i] = sdodata [i + 1];
				data[7] = 0;
			}
			else											/* no answer									*/
			{
				data [0] = '?';							/* display ?									*/
				data [1] = 0;
			}
			clrscr ();
			switch (pcb)									/* display name of PCB							*/
			{
				case (M_TSE_INFO):
					sprintf ((char *)buffer, "PCU%u: %s", number + 1, data);
					break;
					
				case (M_EXE_INFO):
					sprintf ((char *)buffer, "EXU%u: %s", number + 1, data);
					break;
						
				case (M_ESE_INFO):
//					GetIDDispBuffer(number, buf1);
					GetLcuConfig(number, buf1);
					trim(&buf1[2]);
					sprintf((char *)buffer, "LCU%s: %s", &buf1[2], data);
					break;
					
				case (M_FVE_INFO):
					buffer [0] = 0;
					break;
			}
			display (0, 0, buffer);
			data [0] = INIT_READ_REQ;						/* request hardware version number				*/
			data [1] = (BYTE)HW_VERSION;
			data [2] = HW_VERSION >> 8;
			for (i = 3; i < 8; i++)
				data [i] = 0;
			*sdo = 0xFFFF;
			send_message (RSDO, id, 8, data, bus);
			return (HW_READ_SEGM_HW);

		case (HW_READ_SEGM_HW):							/* read segment request of hardware version		*/
			data [0] = READ_SEGM_REQ;
			for (i = 1; i < 8; i++)
				data [i] = 0;
			send_message (RSDO, id, 8, data, bus);
			*sdo = 0xFFFF;
			return (HW_INIT_READ_SW);

		case (HW_INIT_READ_SW):						/* init read request software version			*/
			if (*sdo == node_id)							/* answer from PCB								*/
			{
				for (i = 0; i < 7; i++)
					data [i] = sdodata [i + 1];
				data[7] = 0;
			}
			else
			{
				data [0] = '?';
				data [1] = 0;
			}
			sprintf ((char *)buffer, "HW:%s", data);
			if (pcb == M_FVE_INFO)
				display (0, 0, buffer);
			else
				display (0, 1, buffer);
			data [0] = INIT_READ_REQ;
			data [1] = (BYTE)SW_VERSION;
			data [2] = SW_VERSION >> 8;
			for (i = 3; i < 8; i++)
				data [i] = 0;
			*sdo = 0xFFFF;
			send_message (RSDO, id, 8, data, bus);
			return (HW_READ_SEGM_SW);

		case (HW_READ_SEGM_SW):							/* read segment request of software version		*/
			data [0] = READ_SEGM_REQ;
			for (i = 1; i < 8; i++)
				data [i] = 0;
			send_message (RSDO, id, 8, data, bus);
			*sdo = 0xFFFF;
			return (HW_DISP_SW);

		case (HW_DISP_SW):								/* read request inputs							*/
			if (*sdo == node_id)							/* answer from PCB								*/
			{
				for (i = 0; i < 7; i++)
					data [i] = sdodata [i + 1];
				data[7] = 0;
			}
			else
			{
				data [0] = '?';
				data [1] = '?';
				data [2] = '?';
				data [3] = '?';
				data [4] = 0;
			}
			sprintf ((char *)buffer, "SW:%s", data);
			strcpy((char*)ver_buff, (const char *)data);		// 用于数码管显示外设软件版本
			dm_get_answer = 1;							// 
			if (pcb == M_FVE_INFO)
				display (8, 0, buffer);
			else
				display (8, 1, buffer);
			return (HW_INIT_READ_IN);
		
		case (HW_INIT_READ_IN):
			out_new = 1;
			in_new = 1;
			switch (pcb)									/* display name of PCB							*/
			{
				case (M_TSE_INFO):
					inpara = p.tse_inpar [number][*page*8];
					outpara = p.tse_outpar [number][*page*8];
					if (*page*8+8 >= MAX_TSE_IN)
						size_in = MAX_TSE_IN - (*page*8);
					else
						size_in = 8;
					if (*page*8+8 >= MAX_TSE_OUT)
						size_out = MAX_TSE_OUT - (*page*8);
					else
						size_out = 8;
					break;
					
				case (M_ESE_INFO):
					inpara = p.ese_inpar [number][0];
					outpara = p.ese_outpar [number][0];
					size_in = MAX_ESE_IN;
					size_out = MAX_ESE_OUT;
					break;
					
				case (M_EXE_INFO):
					inpara = exe_para.exe_inpar [number][0];
					outpara = exe_para.exe_outpar [number][0];
					size_in = MAX_ESE_IN;
					size_out = MAX_ESE_OUT;
					break;
					
				case (M_FVE_INFO):
					inpara = p.fve_inpar [0];
					outpara = p.fve_outpar [0];
					if (!fve_hardware_ver_after_v13)
						size_in = MAX_FVE_IN;
					else
						size_in = MAX_FVE_IN+MAX_FVE_EX_IN;
					size_out = MAX_FVE_OUT;
					break;
			}
			write_io (instate, inpara,	size_in, INPUT);
			write_io (outstate, outpara, size_out, OUTPUT);
			data [0] = INIT_READ_REQ;
			data [1] = (BYTE)PORT_IN;
			data [2] = PORT_IN >> 8;
			data [3] = *page;
			for (i = 4; i < 8; i++)
				data [i] = 0;
			*sdo = 0xFFFF;
			send_message (RSDO, id, 8, data, bus);
			cursor (_SOLIDCURSOR);
			if (pcb != M_FVE_INFO)
			{
				if (pcb == M_TSE_INFO)
				{
					sprintf((char *)buffer, "I%02d-%02d:", *page*8+1, *page*8+8);
					display (0,2, buffer);
					sprintf((char *)buffer, "O%02d-%02d:", *page*8+1, *page*8+8);
					display (0,3, buffer);
				}
				else
				{
					display (0,2, "I1-I8:");
					display (0,3, "O1-O8:");
				}
				gotoxy (*column, *row);
			}
			return (HW_INIT_READ_OUT);

		case (HW_INIT_READ_OUT):						/* read request outputs							*/
			if ((*sdo == node_id) &&						/* answer from PCB								*/
			((*(WORD *)&sdodata [1]) == PORT_IN))		/* with input object							*/
			{
				in = *(DWORD *)&sdodata [4];				/* read input state								*/
				if ((in != in_old) || in_new)				/* state of inputs changed						*/
				{
					in_new = 0;
					in_old = in;
					for (i = 0; i < size_in; i++)
					{
						if ((inpara == (BYTE *)&p.fve_inpar[0][0]) && (i >= MAX_FVE_IN))
						{
							if (((bit_selectd(in, i)) && (!(p.fve_ex_inpar[i-MAX_FVE_IN][IO_STATE] & POLARITY_BIT))) ||
							(!bit_selectd(in, i) && (p.fve_ex_inpar[i-MAX_FVE_IN][IO_STATE] & POLARITY_BIT)))
								inverse = 1;
							else
								inverse = 0;
						}
						else
						{
							if (((bit_selectd(in, i)) && (!(*(inpara+i*MAX_IO_TYPE+5) & POLARITY_BIT))) ||
							(!bit_selectd(in, i) && (*(inpara+i*MAX_IO_TYPE+5) & POLARITY_BIT)))
								inverse = 1;
							else
								inverse = 0;
						}
						if ((instate[i] == '-') || (instate[i] == 227))
						{
							if (bit_selectd(in, i))
								instate[i] = 227;
							else
								instate[i] = '-';
						}
						else if (inverse)
						{
							if (instate[i] > 90)
								instate[i] -= 32;
						}
						else
						{
							if (instate[i] <= 90)
								instate[i] += 32;
						}
					}
					if (pcb == M_FVE_INFO)
					{
						display (0, 1, instate);
						display (0, 2, &instate [16]);
					}
					else
						display (8, 2, instate);
					gotoxy (*column, *row);
				}
				
//增加闪烁功能		
				for(i=0; i<size_in; i++)
				{							
					if ((inpara == (BYTE *)&p.fve_inpar[0][0]) && (i >= MAX_FVE_IN))
					{
						if (p.fve_ex_inpar[i-MAX_FVE_IN][IO_BASIC_FUNC] && (p.fve_ex_inpar[i-MAX_FVE_IN][IO_ENABLE] == DISABLE))
							flash = 1;
						else
							flash = 0;
					}
					else
					{
						if (inpara [i*MAX_IO_TYPE] && (inpara[i*MAX_IO_TYPE + IO_ENABLE] == DISABLE))
							flash = 1;
						else
							flash = 0;
					}
					if (flash)
					{
						if(sec_cnt % 2)
							dat = instate[i];
						else
							dat = 0x20;
						if(pcb == M_FVE_INFO)
						{
							if(i < 16)
								lcd_display [1][i] = dat;
							else
								lcd_display [2][i-16] = dat;			
						}
						else									
							lcd_display [2][i + 8] = dat;			
					}
				}
//增加闪烁功能		
				no_response_count = 0;
			}
			else
			{
				if (no_response_count < 20)
					no_response_count++;
				else
				{
					if (pcb == M_FVE_INFO)
					{
						display (0, 1, "????????????????");
						if (!fve_hardware_ver_after_v13)
							display (0, 2, "??");
						else
							display (0, 2, "???");
					}
					else
						display (8, 2, "????????");
					in_new = 1;
				}
			}
			data [0] = INIT_READ_REQ;
			data [1] = (BYTE)PORT_OUT;
			data [2] = PORT_OUT >> 8;
			data [3] = *page;
			*sdo = 0xFFFF;
			send_message (RSDO, id, 8, data, bus);
			return (HW_READ_IN);

		case (HW_READ_IN):								/* read request inputs							*/
			if ((*sdo == node_id) &&						/* answer from PCB								*/
			((*(WORD *)&sdodata [1]) == PORT_OUT))	/* with output object							*/
			{
				out = *(DWORD *)&sdodata [4];				/* read output state							*/
				if ((out != out_old) || out_new)			/* state of outputs changed						*/
				{
					out_new = 0;
					out_old = out;
					for (i = 0; i < size_out; i++)
					{
						if ((outstate[i] == '-') || (outstate[i] == 227))
						{
							if (bit_selectd(out, i))
								outstate[i] = 227;
							else
								outstate[i] = '-';
						}
						else if (bit_selectd(out, i))
						{
							if (outstate[i] > 90)
								outstate[i] -= 32;
						}
						else
						{
							if (outstate[i] <= 90)
								outstate[i] += 32;
						}
					}
					if (pcb == M_FVE_INFO)
						display (0, 3, outstate);
					else
						display (8, 3, outstate);
					gotoxy (*column, *row);
				}
				
//增加闪烁功能		
				for(i=0; i<size_out; i++)
				{ 							
					if(outpara [i*MAX_IO_TYPE] && (outpara[i*MAX_IO_TYPE + IO_ENABLE] == DISABLE))
					{
						if(sec_cnt % 2)
							dat = outstate[i];
						else
							dat = 0x20;
						if(pcb == M_FVE_INFO)
							lcd_display [3][i] = dat;			
						else									
							lcd_display [3][i + 8] = dat; 		
					}
				}
//增加闪烁功能		
				no_response_count = 0;
			}
			else
			{
				if (no_response_count < 20)
					no_response_count++;
				else
				{
					if (pcb == M_FVE_INFO)
						display (0, 3, "????????");
					else
						display (8, 3, "????????");
					out_new = 1;
				}
			}
			data [0] = INIT_READ_REQ;
			data [1] = (BYTE)PORT_IN;
			data [2] = PORT_IN >> 8;
			data [3] = *page;
			*sdo = 0xFFFF;
			send_message (RSDO, id, 8, data, bus);
			return (HW_INIT_READ_OUT);

		case (HW_WRITE_OUT):							/* next read request after output write			*/
			data [0] = INIT_READ_REQ;
			data [1] = (BYTE)PORT_OUT;
			data [2] = PORT_OUT >> 8;
			data [3] = *page;
			*sdo = 0xFFFF;
			send_message (RSDO, id, 8, data, bus);
			return (HW_READ_IN);

		default:										/* read request outputs							*/
			if ((state >= '1') && (state <= ('1' + 16)))	/* number 1 - 8 or number 0 pressed				*/
			{
				state -= 49;								/* change from ASCII to number					*/
				out = out_old;								/* read old output state						*/
				if (bit_selectd (out, state))				/* change output state							*/
					bit_resetd (out, state);
				else
					bit_setd (out, state);
			}
			data [0] = INIT_WRITE_REQ | EXPEDITED_BIT;
			data [1] = (BYTE)PORT_OUT;
			data [2] = PORT_OUT >> 8;
			data [3] = *page;
			*(DWORD *)&data [4] = out;					/* write output state							*/
			*sdo = 0xFFFF;
			send_message (RSDO, id, 8, data, bus);
			return (HW_WRITE_OUT);
	}
}

