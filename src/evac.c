
//#include "typedef.h"
//#include "parameter.h"
//#include "nice5000.h"
//#include "state.h"
//#include "call.h"
//#include "can.h"
//#include "drive.h"
//#include "io.h"
//#include "pos.h"
//#include "history.h"

///****************************************************************************************************/
///* Evacuation with UPS power supply														*/
///****************************************************************************************************/
//void evacuation_state (void)
//{
//	BYTE doorcommand = 0;
//	BYTE door_closing = 0;
//	BYTE shutoff_power = 0;
//	BYTE stopcommand = 0; 
//	BYTE half_load = 0;
//	BYTE door = 0;
//	BYTE i;
//	BYTE startdir = 0;
//	DWORD shutoff_delay = 0;
//	DWORD stoptimer = 0;
//	BYTE stop = 0;
//	BYTE speaker_fg = 0;
//	DWORD ups_run_time = 0;
//	BYTE	start_fg = 0;

//	send_hse_state ();
//	calls_off = 1;										/* disable calls						*/
//	ups_startcount = 0;	
//	clearcalls (ALL_CALLS);								/* clear all calls						*/
//	if (load & HALF_LOAD)
//		half_load = 1;
//	she_calldoor_double = 0;
//	nice5000_Tcmd_data2 = (nice5000_Tcmd_data2 & (~(NICE5000_TCMD2_STATE|NICE5000_TCMD2_TARGET))) | TCMD2_STATE_EVACUATION;

//	ct = timer + 1 SEC;	
//	
//	while(ct > timer)
//		{
//			check_ilu_ild();
//			cycle ();
//			set_state ();									/* set new state						*/
//		}

//	ct = timer + 2 SEC;									/* wait 2 seconds						*/

//	while (1)
//	{
//		check_ilu_ild();
//		cycle ();
//		set_state ();									/* set new state						*/
//		if (hse_state != H_EVACUATION)					/* state changed						*/
//		{
//			if (drive_state != D_STOPPED)				/* drive not in stopped state			*/
//			{
//				stop_drive();
//			}
//			if ((p.setup_ready) && (p.cooperate_sel1 & COOP1_INSP_RETURN))
//				insp_return = 1;
//			return;								/* go to new state					*/
//		}
//		if ((door_zone & POS_SGM) && 
//		(((level == 0xFF) || (p.doorpos[level])) ||
//		(poserror) || 								/* lift in level zone or positioning error	*/
////		(doorstate != ALL_DOORS_CLOSED) ||			/* or doors already opened			*/
//		(doorcommand) ||								/* or already doorcommand 		*/
//		(ups_startcount == 4))) 					/* or lift did try to go to level 		*/
//		{
//			if (drive_state == D_STOPPED)
//			{
//				if (!doorcommand)						/* no door command					*/
//				{
//					if (p.doornumber == 1)				/* lift has only 1 car door				*/
//						door = p.doorpos [level] & 0x01;
//					else								/* lift has more than 1 car door		*/
//					{
//						if (level != 0xFF)				/* lift did already start counting		*/
//							door = p.doorpos [level];
//						else
//						{
//							for (i = 0; i < p.doornumber; i++)
//								bit_set (door, i);
//						}
//					}
//					if(door)
//						{
//							doorcommand = 1;					
//							level_display (level, O_ALL);
//							set_door (door, OPEN_DOOR);				/* open doors									*/
//							set_out (SPECIAL_FUNC, UPS_EVAC_READY_SPEAKER, 0, 0, 1, (O_CANA|O_CANB|O_HSE));		//������ʾ������ʾ"Overload",����
//							shutoff_delay = timer + 30 SEC;		//30S���ж�UPS��Դ
//						}
//				}
//				else
//				{
//					if(shutoff_delay < timer + 15 SEC)
//						{//���������� 15S ��ر�
//							if(!speaker_fg)
//								{
//									speaker_fg = 1;
//									set_out (SPECIAL_FUNC, UPS_EVAC_READY_SPEAKER, 0, 0, 0, (O_CANA|O_CANB|O_HSE)); 	//������ʾ������ʾ"Overload",����
//								}							
//						}
//					if ((shutoff_delay < timer) && ((door_state[0] == DOOR_OPENED) || (door_state[1] == DOOR_OPENED)))
//					{
//						if (!shutoff_power)
//						{
//							shutoff_power = 1;
//							set_out (SPECIAL_FUNC, UPS_EVAC_READY, 0, 0, 1, O_HSE);		//�ȴ� 1 ���ӹر�UPS���
//						}
//					}
//				}
//				start_fg = 0;
//			}
//			else
//				{
//					if (!stop && p.doorpos [level])
//						{
//							stop = 1;
//							stoptimer = timer + (p.upstrip_stop_delay / 50);
//						}
//					else if ((stoptimer <= timer) && (!stopcommand) && stop)
//						{
//							stop_drive();
//							stopcommand = 1;
//						}
//				}
//		}
//		else if (((doorstate != ALL_DOORS_CLOSED) ||	/* not all doors closed				*/
//		(!(safety_circuit & SC3)))						/* or safety circuit shaft door is off		*/
//		&& (drive_state == D_STOPPED))
//		{
//			if (ct <= timer)								/* time over							*/
//			{
//				door_command (CLOSE_DOOR, CLOSE_DOOR);
//				ct = timer + 60 SEC;					/* wait 60 seconds for door closed		*/
//			}
//			door_closing = 1;
//		}
//		else											/* lift not in level, door closed			*/
//		{
//			if (door_closing)
//			{
//				door_closing = 0;
//				ct = timer + 2 SEC;						/* start delay 2 seconds				*/
//			}
//			if ((drive_state == D_STOPPED)
//				&& (level != 0xff)
//				&& ((nice5000_Rcmd_data2 & NICE5000_RCMD2_STATE) == RCMD2_STATE_EVACUATION))
//			{
//				if ((ct <= timer) && (ups_startcount < 4))	/* time over, not more than 4 starts		*/
//				{
//					start_floor = level;
//					startdir = ((ups_startcount + half_load) % 2) + 1;
//					if(p.ups_direction_mode & UPS_DIR_SEL)
//						start_drive (0);
//					else
//						{
//							startdir = get_target_floor_ups(startdir);
//							nice5000_Tcmd_data2 |= target_floor + 1;
//							start_drive (startdir);
//						}
//					ups_startcount ++;					/* start drive							*/
//					stopcommand = 0;					
//					start_fg = 1;
//					ups_run_time = timer + ((p.triptime + 10) SEC);		/* set trip time						*/
//					
//					if(p.ups_direction_mode & UPS_ALARM)		//UPS��Ԯʱ�������Ъ��ʾ
//						set_out(SPEAKER_BUZ, BUZZER_NORMAL, 2, SPEEKER_PAUSE, 1, O_CANA | O_CANB | O_HSE);
//				}
//			}
//			else
//				ct = timer + 2 SEC;						/* start delay 2 seconds				*/
//		}
//		if((start_fg) && (ups_run_time < timer) && (!(p.ups_direction_mode & UPS_NO_TIME))) 								
//		{
//			emergency_stop ();						/* send emergency stop				*/
//			general_error |= G_TRIPTIME;	/* set trip time exceeded error 		*/
//			write_errorhist (E_TRIP_TIME, HSE_ID, C_TRIP_TIME, 0);
//			hse_state = H_WAIT_STOP;
//			return;
//		}
//	}
//}


#include "typedef.h"
#include "parameter.h"
#include "nice5000.h"
#include "state.h"
#include "call.h"
#include "can.h"
#include "drive.h"
#include "io.h"
#include "pos.h"
#include "history.h"

/****************************************************************************************************/
/* Evacuation with UPS power supply														*/
/****************************************************************************************************/
DWORD shutoff_delay = 0;
DWORD Door_delay_time[2] = {0xFFFFFFFF,0xFFFFFFFF};
void evacuation_state (void)
{

	WORD doorstaytime_cc;
	WORD doorstaytime_nc;
	BYTE doorclosepush_old;
	BYTE doorclosepush_act;
	BYTE maxretry;
	BYTE retrycnt = 0;
	BYTE doorclosecmd = 0;
	BYTE dooropencmd = 0;
	BYTE d1cmd = DOOR_OFF;
	BYTE d2cmd = DOOR_OFF;
	DWORD doortimer = 0;
	
	BYTE doorcommand = 0;
	BYTE door_closing = 0;
	BYTE shutoff_power = 0;
	BYTE stopcommand = 0; 
	BYTE half_load = 0;
	BYTE door = 0;
	BYTE i;
	BYTE startdir = 0;
	
	DWORD Speaker_time = 0;
	DWORD stoptimer = 0;
	BYTE stop = 0;
	BYTE speaker_fg = 0;
	DWORD ups_run_time = 0;
	BYTE	start_fg = 0;

	if (level == p.main_floor)
	{
		doorstaytime_cc = p.doorstaytime_main_cc;
		doorstaytime_nc = p.doorstaytime_main_nc;
	}
	else
	{
		doorstaytime_cc = p.doorstaytime_cc;
		doorstaytime_nc = p.doorstaytime_nc;
	}
	send_hse_state ();
	calls_off = 1;										/* disable calls						*/
	ups_startcount = 0;	
	clearcalls (ALL_CALLS);								/* clear all calls						*/
	if (load & HALF_LOAD)
		half_load = 1;
	she_calldoor_double = 0;
	nice5000_Tcmd_data2 = (nice5000_Tcmd_data2 & (~(NICE5000_TCMD2_STATE|NICE5000_TCMD2_TARGET))) | TCMD2_STATE_EVACUATION;

	ct = timer + 1 SEC;	
	
	while(ct > timer)
		{
			check_ilu_ild();
			cycle ();
			set_state ();									/* set new state						*/
		}

	ct = timer + 2 SEC;									/* wait 2 seconds						*/

	while (1)
	{
		check_ilu_ild();
		cycle ();
		set_state ();									/* set new state						*/
		if (hse_state != H_EVACUATION)					/* state changed						*/
		{
			if (drive_state != D_STOPPED)				/* drive not in stopped state			*/
			{
				stop_drive();
			}
			if ((p.setup_ready) && (p.cooperate_sel1 & COOP1_INSP_RETURN))
				insp_return = 1;
			return;								/* go to new state					*/
		}
		if ((door_zone & POS_SGM) && 
		(((level == 0xFF) || (p.doorpos[level])) ||
		(poserror) || 								/* lift in level zone or positioning error	*/
//		(doorstate != ALL_DOORS_CLOSED) ||			/* or doors already opened			*/
		(doorcommand) ||								/* or already doorcommand 		*/
		(ups_startcount == 4))) 					/* or lift did try to go to level 		*/
		{
			if (drive_state == D_STOPPED)
			{
				if (!doorcommand)						/* no door command					*/
				{
					if (p.doornumber == 1)				/* lift has only 1 car door				*/
						door = p.doorpos [level] & 0x01;
					else								/* lift has more than 1 car door		*/
					{
						if (level != 0xFF)				/* lift did already start counting		*/
							door = p.doorpos [level];
						else
						{
							for (i = 0; i < p.doornumber; i++)
								bit_set (door, i);
						}
					}
					if(door)
						{
							doorcommand = 1;					
							level_display (level, O_ALL);
							set_door (door, OPEN_DOOR);				/* open doors									*/
							set_out (SPECIAL_FUNC, DOOR_OPEN, 0, EXISTING_DOORS, 1, O_CANA);
							set_out (SPECIAL_FUNC, UPS_EVAC_READY_SPEAKER, 0, 0, 1, (O_CANA|O_CANB|O_HSE));		//??????????????"Overload",????
							Speaker_time = timer +15 SEC;
						}
				}
				else
				{

					if(doorcommand ==1)
					{ 
						if((door & DOOR1) &&  (door_state [0] == DOOR_OPENED))
						{
							if(Door_delay_time[0] == 0xFFFFFFFF)
							{
								Door_delay_time[0] = timer +doorstaytime_nc SEC;
							}
						}
						if((door & DOOR2) && (door_state [1] == DOOR_OPENED))
						{
							if(Door_delay_time[1] == 0xFFFFFFFF)
							{
								Door_delay_time[1] = timer +doorstaytime_nc SEC;
							}
						}
						if((door & DOOR1) &&  (door_state [0] == DOOR_CLOSED))
						{
							if(Door_delay_time[0] != 0xFFFFFFFF)
							{
								Door_delay_time[0] = 0xFFFFFFFF;
							}
						}
						if((door & DOOR2) && (door_state [1] == DOOR_CLOSED))
						{
							if(Door_delay_time[1] != 0xFFFFFFFF)
							{
								Door_delay_time[1] = 0xFFFFFFFF;
							}
						}
						if(Door_delay_time[0] < timer) // door open time out
						{
							if(door)
							{
								set_out (SPECIAL_FUNC, DOOR_CLOSE, 0, EXISTING_DOORS, 1, O_CANA);
								set_door (DOOR1, CLOSE_DOOR);				/* close doors*/	
								Door_delay_time[0] = 0xFFFFFFFF;
								
							}	
						}
						if(Door_delay_time[1] < timer) // door open time out
						{
							if(door)
							{
								set_out (SPECIAL_FUNC, DOOR_CLOSE, 0, EXISTING_DOORS, 1, O_CANA);
								set_door (DOOR2, CLOSE_DOOR);				/* close doors*/	
								Door_delay_time[1] = 0xFFFFFFFF;
							}	
						}
						if(Speaker_time < timer)
						{//?????????? 15S ????
							if(!speaker_fg)
								{
									speaker_fg = 1;
									set_out (SPECIAL_FUNC, UPS_EVAC_READY_SPEAKER, 0, 0, 0, (O_CANA|O_CANB|O_HSE)); 	//??????????????"Overload",????
								}					
						}
						while(doorcloseerrorcount [0] || dooropenerrorcount [0] ||doorcloseerrorcount [1] || dooropenerrorcount [1] )
						{
							for (i = 0; i < p.doornumber; i++)
							{
								if( doorcloseerrorcount [i] || dooropenerrorcount [i])
								{									/* lift cant close doors				*/
										if (!doorcloseerrorcount[0])
											general_error &= ~G_CLOSEDOOR1;
										if (!doorcloseerrorcount[1])
											general_error &= ~G_CLOSEDOOR2;
										if ((doorcloseerrorcount[0] | doorcloseerrorcount[1]) && (general_error))
										{
											if ((general_error & (G_CLOSEDOOR1 | G_CLOSEDOOR2)) == (G_CLOSEDOOR1 | G_CLOSEDOOR2))
												maxretry = GetMin(p.doorcloselimit [0], p.doorcloselimit [1]);
											else if (general_error & G_CLOSEDOOR1)
												maxretry = p.doorcloselimit [0];
											else
												maxretry = p.doorcloselimit [1];
											if (retrycnt < maxretry)
											{
												if ((!dooropencmd) && (door_zone & POS_SGM))
												{
													dooropencmd = 1;
													d1cmd = DOOR_OFF;
													d2cmd = DOOR_OFF;
													if (general_error & G_CLOSEDOOR1)
														d1cmd = OPEN_DOOR;
													if (general_error & G_CLOSEDOOR2)
														d2cmd = OPEN_DOOR;
													door_command (d1cmd, d2cmd);
													if ((general_error & (G_CLOSEDOOR1 | G_CLOSEDOOR2)) == (G_CLOSEDOOR1 | G_CLOSEDOOR2))
														doortimer = timer + 10ul * GetMax(p.dooropentime[0], p.dooropentime[1]);
													else if (general_error & G_CLOSEDOOR1)
														doortimer = p.dooropentime[0];
													else
														doortimer = p.dooropentime[1];
												}
												else
												{
													if (!doorclosecmd)
													{
														if (((general_error == G_CLOSEDOOR1) && (door_state[0] == DOOR_OPENED))
															|| ((general_error == G_CLOSEDOOR2) && (door_state[1] == DOOR_OPENED))
															|| ((general_error == (G_CLOSEDOOR1|G_CLOSEDOOR2)) && (door_state[0] == DOOR_OPENED) && (door_state[1] == DOOR_OPENED))
															|| (doortimer < timer))
														{
															doorclosecmd = 1;
															d1cmd = DOOR_OFF;
															d2cmd = DOOR_OFF;
															if (general_error & G_CLOSEDOOR1)
																d1cmd = CLOSE_DOOR;
															if (general_error & G_CLOSEDOOR2)
																d2cmd = CLOSE_DOOR;
															door_command (d1cmd, d2cmd);
															if ((general_error & (G_CLOSEDOOR1 | G_CLOSEDOOR2)) == (G_CLOSEDOOR1 | G_CLOSEDOOR2))
																doortimer = timer + 10ul * GetMax(p.doorclosetime[0], p.doorclosetime[1]);
															else if (general_error & G_CLOSEDOOR1)
																doortimer = p.doorclosetime[0];
															else
																doortimer = p.doorclosetime[1];
														}
													}
													else if (doortimer < timer)
													{
														doorclosecmd = 0;
														dooropencmd = 0;
														++ retrycnt;
														if ((retrycnt >= maxretry) && (door_zone & POS_SGM))
														{
															if (d1cmd == CLOSE_DOOR)
																d1cmd = OPEN_DOOR;
															if (d2cmd == CLOSE_DOOR)
																d2cmd = OPEN_DOOR;
															door_command (d1cmd, d2cmd);
														}
													}
												}
											}
											else
											{
												if ((!dooropencmd) && (door_zone & POS_SGM))
												{
													dooropencmd = 1;
													d1cmd = DOOR_OFF;
													d2cmd = DOOR_OFF;
													if (general_error & G_CLOSEDOOR1)
														d1cmd = OPEN_DOOR;
													if (general_error & G_CLOSEDOOR2)
														d2cmd = OPEN_DOOR;
													door_command (d1cmd, d2cmd);
												}
											}
										}
								}
							}
						}

						if (handle_dooropenpush ())							/* handle door open push						*/
						{
								set_out (SPECIAL_FUNC, DOOR_OPEN, 0, EXISTING_DOORS, 1, O_CANA);
								set_out (SPECIAL_FUNC, DOOR_CLOSE, 0, EXISTING_DOORS, 0, O_CANA);
							Door_delay_time[0] = 0xFFFFFFFF;
							Door_delay_time[1] = 0xFFFFFFFF;

						}
						
						/*------------------------door close push-----------------------------*/
						if(!(p.landingcall_push & PUSH_STOP_SHIP))
							doorclosepush_act = doorclosepush & (~dooropenpush) & (~doorstopstate) & p.doorpos [level];
						else			
							doorclosepush_act = doorclosepush & (~dooropenpush) & (~doorstoppush) & p.doorpos [level];
						if ((callpriority == C_FIREMAN) && (!firekey) && (!auto_fire))
									doorclosepush_act = 0;							/* ignore door close push 					*/		
						if (doorclosepush_act != doorclosepush_old)				/* door close push changed					*/
						{
							if (doorclosepush_act)							/* doors to close								*/
							{
								if (!overload)								/* no overload								*/
								{
									if ((!(p.lift_func_sel1 & LIFTFUN1_PRESS_CLOSEBUTTOM))
											|| (door_state [0] == DOOR_OPENED)
											|| (door_state [1] == DOOR_OPENED))
									{
											set_door (doorclosepush, CLOSE_DOOR);	/* send door command						*/
											set_out (SPECIAL_FUNC, DOOR_OPEN, 0, EXISTING_DOORS, 0, O_CANA);
											set_out (SPECIAL_FUNC, DOOR_CLOSE, 0, EXISTING_DOORS, 1, O_CANA);	
									}
								}
								door_attendance = 0;
							}
							else if (firedoormode)
							{
								if (p.fire_doorclosemode == FIREDOOR_STOP)
									fire_door (doorclosepush_old, STOP_DOOR);	/* stop door									*/
								else if (p.fire_doorclosemode == FIREDOOR_REVERSE)
									fire_door (doorclosepush_old, OPEN_DOOR);	/* reopen door								*/
							}
							doorclosepush_old = doorclosepush_act;				/* save old state of door close push				*/
						}
						/*----------------------------------------------------------------------------------------*/
						if((door_state[0] == DOOR_CLOSED) && (door_state[1] == DOOR_CLOSED))
						{
							if(!shutoff_delay)
							{
								shutoff_delay = timer + 30 SEC;		//shut off after all door fully closed
							}
							else
							{
								if (shutoff_delay < timer )
								{
									if (!shutoff_power)
									{
										shutoff_power = 1;
										set_out (SPECIAL_FUNC, UPS_EVAC_READY, 0, 0, 1, O_HSE);		//??? 1 ??????UPS???
									}
								}
							}

						}
						else
						{
							shutoff_delay = 0;
						}
						/*------------------------------------------------------------------*/
						if(((door_state[0] == DOOR_OPENED) || ((door_state[1] == DOOR_OPENED) && (p.doornumber > 1)))
							&& (door_close_open == 8))
						{
							set_out (SPECIAL_FUNC, DOOR_OPEN, 0, EXISTING_DOORS, 0, O_CANA);
							door_close_open = 0;
							cl_op_fg = 0;
						}
						else if(((door_state[0] == DOOR_CLOSED) && (door_close_open & 1))
								|| ((door_state[1] == DOOR_CLOSED) && (door_close_open & 2)))
						{
							set_out (SPECIAL_FUNC, DOOR_CLOSE, 0, EXISTING_DOORS, 0, O_CANA);
							door_close_open = 0;
							cl_op_fg = 0;  
						}
					}
				}
				start_fg = 0;
			}
			else
				{
					if (!stop && p.doorpos [level])
						{
							stop = 1;
							stoptimer = timer + (p.upstrip_stop_delay / 50);
						}
					else if ((stoptimer <= timer) && (!stopcommand) && stop)
						{
							stop_drive();
							stopcommand = 1;
						}
				}
		}
		else if (((doorstate != ALL_DOORS_CLOSED) ||	/* not all doors closed				*/
		(!(safety_circuit & SC3)))						/* or safety circuit shaft door is off		*/
		&& (drive_state == D_STOPPED))
		{
			if (ct <= timer)								/* time over							*/
			{
				door_command (CLOSE_DOOR, CLOSE_DOOR);
				ct = timer + 60 SEC;					/* wait 60 seconds for door closed		*/
			}
			door_closing = 1;
		}
		else											/* lift not in level, door closed			*/
		{
			if (door_closing)
			{
				door_closing = 0;
				ct = timer + 2 SEC;						/* start delay 2 seconds				*/
			}
			if ((drive_state == D_STOPPED)
				&& (level != 0xff)
				&& ((nice5000_Rcmd_data2 & NICE5000_RCMD2_STATE) == RCMD2_STATE_EVACUATION))
			{
				if ((ct <= timer) && (ups_startcount < 4))	/* time over, not more than 4 starts		*/
				{
					start_floor = level;
					startdir = ((ups_startcount + half_load) % 2) + 1;
					if(p.ups_direction_mode & UPS_DIR_SEL)
						start_drive (0);
					else
						{
							startdir = get_target_floor_ups(startdir);
							nice5000_Tcmd_data2 |= target_floor + 1;
							start_drive (startdir);
						}
					ups_startcount ++;					/* start drive							*/
					stopcommand = 0;					
					start_fg = 1;
					ups_run_time = timer + ((p.triptime + 10) SEC);		/* set trip time						*/
					
					if(p.ups_direction_mode & UPS_ALARM)		//UPS???????????????
						set_out(SPEAKER_BUZ, BUZZER_NORMAL, 2, SPEEKER_PAUSE, 1, O_CANA | O_CANB | O_HSE);
				}
			}
			else
				ct = timer + 2 SEC;						/* start delay 2 seconds				*/
		}
		if((start_fg) && (ups_run_time < timer) && (!(p.ups_direction_mode & UPS_NO_TIME))) 								
		{
			emergency_stop ();						/* send emergency stop				*/
			general_error |= G_TRIPTIME;	/* set trip time exceeded error 		*/
			write_errorhist (E_TRIP_TIME, HSE_ID, C_TRIP_TIME, 0);
			hse_state = H_WAIT_STOP;
			return;
		}
	}
}

