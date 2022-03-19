
#include "typedef.h"
#include "lpc1700.h"
#include "lpc1700pincfg.h"
#include "parameter.h"
#include "state.h"
#include "call.h"
#include "can.h"
#include "pos.h"
#include "lcd.h"
#include "history.h"
#include "io.h"
#include "nice5000.h"


void TIMER0_IRQHandler (void){//500ms
	BYTE i, j, m;

	T0IR = 0x01;
	sec_cnt++;
	error_timer++;
	
	if (state_run == LED_ON)	
	{
		second_toggle = 1;
		send_heartbeat = 1;	
		++ menu_toggle_sec;
		if((menu_toggle_sec % 10) == 0)
			send_level = 1;
		if((menu_toggle_sec % 4) == 0)
			send_level_insp = 1;
		time_new = 1;
		SetLed(LED_RUNNING, LED_OFF);
	}
	else
	{
		SetLed(LED_RUNNING, LED_ON);
		if ((hse_state == H_START) || (hse_state == H_MOVING) || (hse_state == H_ARRIVAL))
		{
			op_time++;
			if (((op_time < 3600) && (!(op_time % 60))) || (!(op_time % 3600)))
				save_op_time = 1;
		}
	}
 
	for (i = 0; i < MAX_GR; i++)
	{
		if (!(hse_hb & (1 << i)))
		{
			groupcall [i].type = 0;
			groupcall [i].floor = 0xFF;
			groupcall [i].time = 0;
			groupcall [i].doors = 0;
		}
	}
	if ((hse_hb & (1 << p.groupnumber)) && (!hse_hb_state & (1 << p.groupnumber)))
		write_errorhist (E_GROUPNUMBER, HSE_ID, 0, 0);
	hse_hb_state = hse_hb;
	hse_hb = 0;
	if (tse_hb_timer)
		-- tse_hb_timer;
	else
	{
		if (txa_tse_error)
		{
			tse_hb_error = 0;
			tse_init_error = 0;
		}
		else
		{
			tse_hb_error = tse_heartbeat ^ tse_enabled;
			tse_hb_error &= tse_enabled;
			tse_init_error &= tse_heartbeat;
			if ((tse_hb_error != tse_hb_error_old) && (error_timer > 600) && (hse_state == H_STANDSTILL))
			{
				if ((tse_hb_error == tse_enabled) && tse_enabled)
				{//������COP���������е�COP
					write_errorhist (E_CAN_TSE, HSE_ID, 0, W_HB_TSE);
				}
				else for (i = 0; i < MAX_TSE; i++)
				{//ֻ�ǵ���COP������������
					if (bit_select (tse_hb_error, i))
					{
						if (!bit_select (tse_hb_error_old, i) || (tse_hb_error_old == tse_enabled))
							write_errorhist (E_HEARTBEAT, TSE_ID + i, 0, W_HB_TSE);
					}
				}
				tse_hb_error_old = tse_hb_error;
			}
			if (!tse_hb_error)
				tsetimer = timer + TSE_TIME;				/* restart TSE error warning timer				*/
		}
		tse_state_request |= (tse_hb_state ^ tse_heartbeat);
		tse_state_request &= (tse_heartbeat & tse_enabled);
		tse_hb_state = tse_heartbeat;				/* save old state for display					*/
		tse_heartbeat = 0;							/* reset old marker								*/
		tse_hb_timer = TSE_HBCHK_TIMER;								
	}

	if (exe_hb_timer)
		-- exe_hb_timer;
	else
		{
			if (txa_exe_error)
				{
					exe_hb_error = 0;
					exe_init_error = 0;
				}
			else
				{
					exe_hb_error = exe_heartbeat ^ exe_enabled;
					exe_hb_error &= exe_enabled;
					exe_init_error &= exe_heartbeat;
					if ((exe_hb_error != exe_hb_error_old) && (error_timer > 600))
						{//
							if ((exe_hb_error == exe_enabled) && exe_enabled)
								{
									write_errorhist (E_HEARTBEAT_EXE, HSE_ID, 0, W_HB_TSE);
								}
							else for (i = 0; i < MAX_EXE; i++)
								{
									if (bit_select (exe_hb_error, i))
										{
											if (!bit_select (exe_hb_error_old, i) || (exe_hb_error_old == exe_enabled))
												write_errorhist (E_HEARTBEAT_EXE, EXE_ID + i, 0, W_HB_TSE);
										}
								}
							exe_hb_error_old = exe_hb_error;
						}
					if (!exe_hb_error)
						exetimer = timer + TSE_TIME;				/* restart TSE error warning timer				*/
				}
			exe_state_request |= (exe_hb_state ^ exe_heartbeat);
			exe_state_request &= (exe_heartbeat & exe_enabled);
			exe_hb_state = exe_heartbeat; 			/* save old state for display 				*/
			exe_heartbeat = 0;							/* reset old marker 							*/
			exe_hb_timer = TSE_HBCHK_TIMER; 							
		}

	if (ese_hb_timer)
		-- ese_hb_timer;
	else
	{
		if (txb_ese_error)
		{
			for (i = 0; i < MAX_ESE_DW; i++)
			{
				ese_hb_error [i] = 0;
				ese_init_error [i] = 0;
			}
		}
		else
		{
			m = 0;
			for (i = 0; i < MAX_ESE_DW; i++)
				{
					ese_hb_error [i] = ese_heartbeat [i] ^ ese_enabled [i];
					ese_hb_error [i] &= ese_enabled [i];
					ese_init_error [i] &= ese_heartbeat [i];
					ese_hb_error_old [i] = (ese_hb_error [i] & (~ese_hb_error_old [i]));
					if ((ese_hb_error_old [i]) && (!(canerrorstate & CANB_BUSOFF)))
						m++;
				}
			if((m < (p.ese_number >> 1)) && (hse_state == H_STANDSTILL) && (error_timer > 1200))
				{//ֻ��¼һ��� LCU
					for (i = 0; i < MAX_ESE_DW; i++)
						{
							if ((ese_hb_error_old [i]) && (!(canerrorstate & CANB_BUSOFF)))
								{
									if(canb_heartbeat_time < timer)
										{
											for (j = 0; j < 32; j++)
												{
													if (bit_selectd (ese_hb_error [i], j))
														{
															if (!bit_select (ese_hb_error_old[i], j) || (ese_hb_error_old[i] == ese_enabled[i]))
																write_errorhist (E_HEARTBEAT, ESE_ID + 128 + (32 * i) + j, 0, W_HB_ESE);
														}
												}
											canb_heartbeat_time = timer + CANB_HEARTBEAT_NOTREAD;
										}
								}
							ese_hb_error_old [i] = ese_hb_error [i];
						}
					j = 0;
					for (i = 0; i < MAX_ESE_DW; i++)
						{
							if (ese_hb_error [i])
								j = 1;
						}
				}
			else
				j = 0;
			if (!j)
				esetimer = timer + ESE_TIME;
		}

		for (i = 0; i < MAX_ESE_DW; i++)
		{
			ese_state_request [i] |= (ese_hb_state [i] ^ ese_heartbeat [i]);
			ese_state_request [i] &= (ese_heartbeat [i] & ese_enabled [i]);
			ese_hb_state [i] = ese_heartbeat [i];
			ese_heartbeat [i] = 0;
		}
		ese_hb_timer = ESE_HBCHK_TIMER;
	}

	if (!canbtxerror)								  /* no tx error on CAN B						  */
	{
		if (txb_ese_error)								  /* start heartbeat check on shaft bus delayed   */
			txb_ese_error--;
	}
	if (!canatxerror)								  /* no tx error on CAN A						  */
	{
		if (txa_tse_error) 							  /* start heartbeat check for TSE delayed		  */
			txa_tse_error--;
		if (txa_exe_error) 							  /* start heartbeat check for TSE delayed		  */
			txa_exe_error--;
	}

	if (passwordtimer)
		passwordtimer--;
	if (handheldtimer)
		handheldtimer--;
	else
	{
		handheld_active = 0;
		handheld_bak = 0;
		handheld_key = 0;
	}
	if(menu_keytimer)
		--menu_keytimer;
	if(can_error_timer)
		--can_error_timer;
}

void TIMER1_IRQHandler (void){///1ms
	DWORD m, n;
	static BYTE count = 0;
	static BYTE fx_offstate = 0;
	static BYTE fx_onstate = 0;
	static WORD fx_offtimer = 0;
	static WORD fx_ontimer = 0;
	BYTE output;

	T1IR = 0x01;
	power24v = PWR_IN;
	CLRENA0 = ISE_EINT3;

	if (brake_feedback_timer)
		-- brake_feedback_timer;
	
	if (PSL_IN)
	{
		psl_chk_off = 0;
		if (psl_chk_on < AC_CHK_TMS)
		{
			++ psl_chk_on;
			if (psl_chk_on >= AC_CHK_TMS)
			{
				psl_chk_on = 0;
				if (psl_oncofirm < AC_ON_CONFIRM)
				{
					++ psl_oncofirm;
					if (psl_oncofirm >= AC_ON_CONFIRM)
					{
						if (!state_psl)
						{
							state_psl = 1;
							input_psl = (0x01ul << IO_SC1);
							in |= (0x01ul << IO_SC1);
							input[1] |= (0x01ul << IO_SC1);
							input[0] |= (0x01ul << IO_SC1);
						}
					}
				}
				psl_offcofirm = 0;
			}
		}
	}
	else
	{
		psl_chk_on = 0;
		if (psl_chk_off < AC_CHK_TMS)
		{
			++ psl_chk_off;
			if (psl_chk_off >= AC_CHK_TMS)
			{
				psl_chk_off = 0;
				if (psl_offcofirm < AC_OFF_CONFIRM)
				{
					++ psl_offcofirm;
					if (psl_offcofirm >= AC_OFF_CONFIRM)
					{
						if (state_psl)
						{
							state_psl = 0;
							input_psl = 0;
							in &= ~(0x01ul << IO_SC1);
							input[1] &= ~(0x01ul << IO_SC1);
							input[0] &= ~(0x01ul << IO_SC1);
						}
					}
				}
				psl_oncofirm = 0;
			}
		}
	}
	if (CDSL_IN)
	{
		cdsl_chk_off = 0;
		if (cdsl_chk_on < AC_CHK_TMS)
		{
			++ cdsl_chk_on;
			if (cdsl_chk_on >= AC_CHK_TMS)
			{
				cdsl_chk_on = 0;
				if (cdsl_oncofirm < AC_ON_CONFIRM)
				{
					++ cdsl_oncofirm;
					if (cdsl_oncofirm >= AC_ON_CONFIRM)
					{
						if (!state_cdsl)
						{
							state_cdsl = 1;
							input_cdsl = (0x01ul << IO_SC2);
							in |= (0x01ul << IO_SC2);
							input[1] |= (0x01ul << IO_SC2);
							input[0] |= (0x01ul << IO_SC2);
						}
					}
				}
				cdsl_offcofirm = 0;
			}
		}
	}
	else
	{
		cdsl_chk_on = 0;
		if (cdsl_chk_off < AC_CHK_TMS)
		{
			++ cdsl_chk_off;
			if (cdsl_chk_off >= AC_CHK_TMS)
			{
				cdsl_chk_off = 0;
				if (cdsl_offcofirm < AC_OFF_CONFIRM)
				{
					++ cdsl_offcofirm;
					if (cdsl_offcofirm >= AC_OFF_CONFIRM)
					{
						if (state_cdsl)
						{
							state_cdsl = 0;
							input_cdsl = 0;
							in &= ~(0x01ul << IO_SC2);
							input[1] &= ~(0x01ul << IO_SC2);
							input[0] &= ~(0x01ul << IO_SC2);
						}
					}
				}
				cdsl_oncofirm = 0;
			}
		}
	}
	if (SDSL_IN)
	{
		sdsl_chk_off = 0;
		if (sdsl_chk_on < AC_CHK_TMS)
		{
			++ sdsl_chk_on;
			if (sdsl_chk_on >= AC_CHK_TMS)
			{
				sdsl_chk_on = 0;
				if (sdsl_oncofirm < AC_ON_CONFIRM)
				{
					++ sdsl_oncofirm;
					if (sdsl_oncofirm >= AC_ON_CONFIRM)
					{
						if (!state_sdsl)
						{
							state_sdsl = 1;
							input_sdsl = (0x01ul << IO_SC3);
							in |= (0x01ul << IO_SC3);
							input[1] |= (0x01ul << IO_SC3);
							input[0] |= (0x01ul << IO_SC3);
						}
					}
				}
				sdsl_offcofirm = 0;
			}
		}
	}
	else
	{
		sdsl_chk_on = 0;
		if (sdsl_chk_off < AC_CHK_TMS)
		{
			++ sdsl_chk_off;
			if (sdsl_chk_off >= AC_CHK_TMS)
			{
				sdsl_chk_off = 0;
				if (sdsl_offcofirm < AC_OFF_CONFIRM)
				{
					++ sdsl_offcofirm;
					if (sdsl_offcofirm >= AC_OFF_CONFIRM)
					{
						if (state_sdsl)
						{
							state_sdsl = 0;
							input_sdsl = 0;
							in &= ~(0x01ul << IO_SC3);
							input[1] &= ~(0x01ul << IO_SC3);
							input[0] &= ~(0x01ul << IO_SC3);
						}
					}
				}
				sdsl_oncofirm = 0;
			}
		}
	}
	SETENA0 = ISE_EINT3;
	++ count;
	if (count == 5)
		count = 0;
	if (count)				//ÿ5msִ��һ�����²���
		return;

	if (adv_door_op_dly)
		-- adv_door_op_dly;
	if (rtc_iic_doing)
		-- rtc_iic_doing;
	if (check_para_doing)
		-- check_para_doing;
	input[2] = input[1];
	input[1] = input[0];
	input[0] = READ_INPUTS;
	input[0] |= (input_psl | input_cdsl | input_sdsl);

	for (m = 0; m < MAX_HSE_IN+MAX_HSE_EX_IN+1; ++m)
	{
		n = 0x01ul << m;
		if ((input [0] & n) == (input [1] & n) && 
			(input [0] & n) == (input [2] & n))
		{
			in &= ~n;
			in |= input [2] & n;
		}
	}

	if (in & DOORZONE_INPUT)
	{
		if (!dz_input)
		{
			dz_input = 1;
			adv_door_op_dly = ADV_DR_OPEN_DLY;
			ulsi_off_timer = timer + 2 SEC;
		}
	}
	else
		dz_input = 0;

	if (para_valid)
	{
		output = (out ^ out_polarity) & HSE_MARIN[MAX_HSE_OUT + MAX_HSE_EX_OUT];

		if (fx_offtimer)
			-- fx_offtimer;
		if (fx_ontimer)
			-- fx_ontimer;
		if ((output & 0x01) && (!fx_offstate))
		{
			if (p.fx_active)
			{
				if (!fx_onstate)
				{
					if (contactor_state & contactor_check & D_FXCONT_SUP)
					{
						if (!(driveerror & D_FXCONT_NOTOFF))
						{
							out ^= 0x01;
							driveerror |= D_FXCONT_NOTOFF;
							write_errorhist(E_FXCONTACT_NOTOFF, HSE_ID, C_FENGXING_NOTOFF, W_FENGXING_NOTOFF);
						}
					}
					else
					{
						SET_OUT_3();
						fx_onstate = 1;
						fx_ontimer = p.fx_ontime / 5;
					}
				}
				else if (!fx_ontimer)
				{
					if ((contactor_state & D_FXCONT_SUP) || (!(contactor_check & D_FXCONT_SUP)))
						SET_OUT_1();
					else
					{
						CLR_OUT_1();
						out ^= 0x01;
						fx_offstate = 1;
						if (!(driveerror & D_FXCONT_NOTON))
						{
							driveerror |= D_FXCONT_NOTON;
							write_errorhist(E_FXCONTACT_NOTON, HSE_ID, C_FENGXING_NOTON, W_FENGXING_NOTON);
						}
					}
				}
			}
			else
			{
				SET_OUT_1();
			}
		}
		else
		{
			CLR_OUT_1();
			if (p.fx_active)
			{
				if (fx_onstate)
				{
					fx_onstate = 0;
					fx_offstate = 1;
					fx_offtimer = p.fx_offtime / 5;
					fxtimer = timer + 1 + (p.fx_offtime / 100);
				}
				else if ((!fx_offtimer) && (fx_offstate))
				{
					CLR_OUT_3();
					fx_offstate = 0;
					fxtimer = timer + 5;
				}
			}
		}

		if (output & (0x01 << 1))
			SET_OUT_2();
		else
			CLR_OUT_2();

		if (!p.fx_active)
		{
			if (output & (0x01 << 2))
				SET_OUT_3();
			else
				CLR_OUT_3();
		}

		if (output & (0x01 << 3))
			SET_OUT_4();
		else
			CLR_OUT_4();

		if (output & (0x01 << 4))
			SET_OUT_5();
		else
			CLR_OUT_5();
		if (output & (0x01 << 5))
			SET_OUT_6();
		else
			CLR_OUT_6();
	}

	if (outtimer)
		outtimer--;
	if (handheld_active)
		transmit_lcd ();
	handle_ese = 1;

	++ mstimer;	
	if (!mstimer)
	{
		readload_timer = mstimer + LOAD_TIME;
		groupcalldelay = mstimer + 40;
	}
	DispDigitLED();
	BoardKeyProc();
}

void TIMER2_IRQHandler (void){//50ms
	static BYTE toggle = 0;
	static BYTE toggle1 = 0;
	static BYTE monitoring_delay_time_really = 3;
	BYTE i;

	T2IR = 0x01;
	T2TC = 0;
	if (handheld_dly)
	{
		-- handheld_dly;
		if (!handheld_dly)
			handheld_active = handheld_bak;
	}
	menu ();
	toggle = ~toggle;
	if (!toggle)
	{
		realtimer ++;
		++ toggle1;
		if (toggle1 >= 50)		
			toggle1 = 0;
		if (!toggle1)
		{
			if (!(heartbeat & B_FVE))
			{
				if (!txa_fve_error)
				{//5s �ղ���CDU��������������
					if ((!(hb_error & B_FVE)) && (realtimer > 60))	
					{
						hb_error |= B_FVE;
						if (!(canerrorstate & CANA_BUSOFF))
							write_errorhist (E_HEARTBEAT_CDU, FVE_ID, 0, W_HB_FVE);
					}
				}
				init_error &= ~B_FVE;
				insp = 0;
				door_zone &= ~(POS_SGU | POS_SGD);
				for (i = 0; i < MAX_DOOR; i++)
				{
					door_state [i] = DOOR_CLOSED;
					door_io [i] = NOT_AVAILABLE;
				}
			}
			heartbeat = 0;
		}
		if (!canatxerror)
		{
			if (txa_fve_error)
				txa_fve_error--;
		}
		if(!monitoring_delay_time_really)
		{
			if(monitoring_state & MONITORING_ON)
				monitoring_state |= MONITORING_SEND;
			monitoring_delay_time_really = monitoring_delay_time;
		}
		else
		{
			-- monitoring_delay_time_really;
		}
	}
	else
	{
		if (tea)
		{
			send_heartbeat_a();
			transmit_position(nice5000_Rstore_position,nice5000_Rstore_setspd);
		}
			
		if (nice5000_errrsttime)
		{
			-- nice5000_errrsttime;
			if (!nice5000_errrsttime)
				nice5000_Tcmd_data5 = 0;
		}
//���3��С�����Ƿ�ͬʱ����
	if ((ENT_KEY_IN) && (MENU_KEY_IN) && (UP_KEY_IN))
		{
			++small_key_timer;
			if(small_key_timer >= 30)
				bSmallkey_enable = 1;
		}
	else
		{
			small_key_timer = 0;
			bSmallkey_enable = 0;
		}
	}
}


void TIMER3_IRQHandler (void){///2ms
	static BYTE step = 0;
	static BYTE data4 = 0;
	WORD frm_buff[NICE5000_SPI_FRMSIZE];
	BYTE i;

	T3IR = 0x01;
	T3TC = 0x00;
	
	++ dTmrCnt;
	if (!(SSP1SR & (SSP_SR_BSY|SSP_SR_RNE)))		// SSP�����������ҽ���FIFO��Ϊ�ղ���������
	{
		if (nice5000_crc_error)
		{
			spi_rx_offset = 0;
			nice5000_crc_error = 0;
		}
		switch(step)
		{
			case 0:						// ����������Ϣ1
				frm_buff[0] = FRM_HEAD_CMD1;
				frm_buff[1] = nice5000_Tcmd_data1;
				frm_buff[2] = nice5000_Tcmd_data2;
				if (state_msi)
					frm_buff[2] |= NICE5000_TCMD2_DZONE;
				else
					frm_buff[2] &= ~NICE5000_TCMD2_DZONE;
				frm_buff[3] = nice5000_Tcmd_data3;
				frm_buff[4] = (nice5000_Tcmd_data4 & (~NICE5000_TCMD4_TOGGLE)) | (data4 & NICE5000_TCMD4_TOGGLE);
				++ data4;
				step = 1;
				break;
			case 1:						// ����������Ϣ2
				frm_buff[0] = FRM_HEAD_CMD2;
				frm_buff[1] = nice5000_Tcmd_data1;
				frm_buff[2] = nice5000_Tcmd_data5;
				frm_buff[3] = nice5000_Tcmd_data6;
				frm_buff[4] = nice5000_Tcmd_data7;
				if (nice5000_Tstate_send)
					step = 2;
				else if (nice5000_Tstore_send)
					step = 3;
				else
					step = 0;
				break;
			case 2:						// ���͹����룬���洢
				frm_buff[0] = FRM_HEAD_FCT1;
				if (nice5000_Tstate_send)
				{
					frm_buff[1] = nice5000_Tstate_id1;
					frm_buff[2] = nice5000_Tstate_data1;
					frm_buff[3] = nice5000_Tstate_id2;
					frm_buff[4] = nice5000_Tstate_data2;
					-- nice5000_Tstate_send;
				}
				else
				{
					frm_buff[1] = 0;
					frm_buff[2] = 0;
					frm_buff[3] = 0;
					frm_buff[4] = 0;
				}

				if (nice5000_Tstore_send)
					step = 3;
				else
					step = 0;
				break;
			case 3:						// ���͹����룬Ҫ�洢
				frm_buff[0] = FRM_HEAD_FCT2;
				if (nice5000_Tstore_send)
				{
					frm_buff[1] = nice5000_Tstore_id1;
					frm_buff[2] = nice5000_Tstore_data1;
					frm_buff[3] = nice5000_Tstore_id2;
					frm_buff[4] = nice5000_Tstore_data2;
					-- nice5000_Tstore_send;
				}
				step = 0;
				break;
		}
		// ���� CRC У��
		frm_buff[NICE5000_SPI_FRMSIZE - 1] = CrcChkValue(frm_buff, NICE5000_SPI_FRMSIZE - 1);
		for (i=0; i<NICE5000_SPI_FRMSIZE; ++i)	//�������� 1 ֡����
			SSP1DR = frm_buff[i];
	}
}

