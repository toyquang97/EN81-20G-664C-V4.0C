
#include "typedef.h"
#include "parameter.h"
#include "state.h"
#include "call.h"
#include "can.h"
#include "history.h"
#include "io.h"

extern BYTE  NugdingMode;
extern BYTE  Enabal_opendoor;
/****************************************************************************************************/
/* handle input																						*/
/* active: 0=SDO message; 1=PDO message;															*/
/****************************************************************************************************/
void handle_input (BYTE liftnumber, BYTE active)
{
	BYTE floor = 0, dir = 0, door = 0;
	WORD i;
	static DWORD doublepresstimer = 0;
	static BYTE old_calldoor = 0;
	static BYTE old_callfloor = 0;
	static BYTE insp_dir_top = 0;
	static BYTE insp_dir_pit = 0;

	if (!(liftnumber & virt_in [IO_LIFT]))						/* input not for this lift						*/
		return;

	switch (virt_in [IO_BASIC_FUNC])
	{
		case (INSPECT_IN):
			if ((doortest_active & 0x02) && 				/* doortest with inspection switches			*/
					(hse_state == H_DOORTEST) &&
					((virt_in [IO_SUB_FUNC] == INSP_TOPCAR_UP) || (virt_in [IO_SUB_FUNC] == INSP_TOPCAR_DOWN)))
				{
					if ((virt_in [IO_STATE]) && active) 			/* switch is on 							*/
						{
							if (virt_in [IO_SUB_FUNC] == INSP_TOPCAR_UP)
								doortest_command = DR_OPEN; 				/* open all doors 							*/
							else if(virt_in [IO_SUB_FUNC] == INSP_TOPCAR_DOWN)
								doortest_command = DR_CLOSE;				/* close all doors								*/
						}
					if (virt_in [IO_STATE]) 					/* switch is on 							*/
						insp |= (1 << ((virt_in [IO_SUB_FUNC] & 0xF0) >> 4));
					else										/* switch is off								*/
						insp &= ~(1 << ((virt_in [IO_SUB_FUNC] & 0xF0) >> 4));
				}
			else
				{
					if(virt_in [IO_SUB_FUNC] & INSP_ON)
						{
							if (virt_in [IO_STATE])
								{
									insp |= (1 << ((virt_in [IO_SUB_FUNC] & 0xF0) >> 4));
									if(insp & INSP_PITBOX_ACTIVE)
										{
//											set_out(SPECIAL_FUNC, RESET_PITBOX, 0, 0, 1, O_CANA | O_CANB);		//������λָʾ��
											pitbox_reset_active |= PIO_0;
										}
								}
							else
								{
									insp &= ~(1 << ((virt_in [IO_SUB_FUNC] & 0xF0) >> 4));
									if((!(insp & INSP_PITBOX_ACTIVE)) && (pitbox_reset_active & PIO_0))
										{
											pitbox_reset_active |= PIO_1;
											insp |= INSP_PITBOX_ACTIVE;
											set_out(SPECIAL_FUNC, RESET_PITBOX, 0, 0, 1, O_CANA | O_CANB);		//������λָʾ��
										}
								}
						}				
//�����������ȼ�		
					else
						{
							if(insp & INSP_TOPCAR_ACTIVE)
								{//���ް�ť����								
									if((virt_in [IO_SUB_FUNC] == INSP_TOPCAR_UP) || (virt_in [IO_SUB_FUNC] == INSP_TOPCAR_DOWN))
										{
											if (virt_in [IO_STATE]) 					/* switch is on 							*/
												insp_dir_top |= (virt_in [IO_SUB_FUNC] & 0x0F);
											else										/* switch is off								*/
												insp_dir_top &= ~(virt_in [IO_SUB_FUNC] & 0x0F);
										}
								}
							else
								insp_dir_top = 0;
							
							if(insp & INSP_PITBOX_ACTIVE)
								{
									if((virt_in [IO_SUB_FUNC] == INSP_PITBOX_UP) || (virt_in [IO_SUB_FUNC] == INSP_PITBOX_DOWN))
										{
											if (virt_in [IO_STATE]) 					/* switch is on 							*/
												insp_dir_pit |= (virt_in [IO_SUB_FUNC] & 0x0F);
											else										/* switch is off								*/
												insp_dir_pit &= ~(virt_in [IO_SUB_FUNC] & 0x0F);
										}
								}
							else
								insp_dir_pit = 0;
							
							if((insp & (INSP_TOPCAR_ACTIVE | INSP_PITBOX_ACTIVE)) == (INSP_TOPCAR_ACTIVE | INSP_PITBOX_ACTIVE))
								{//�ζ�������׿Ӽ���ͬʱ��Ч
									insp_dir = insp_dir_pit & insp_dir_top;
								}
							else if(insp & (INSP_TOPCAR_ACTIVE | INSP_PITBOX_ACTIVE))
								{
									insp_dir = insp_dir_pit | insp_dir_top;
								}
							else								
								{
									insp_dir = 0;
									insp_dir_top = 0;
									insp_dir_pit = 0;
								}
						}					
		  	}			
			break;

		case (POS_IN):									/* Position switch inputs						*/
			if (virt_in [IO_STATE])						/* switch is on								*/
			{
				door_zone |= virt_in [IO_SUB_FUNC];
			}
			else										/* switch is off								*/
			{
				door_zone &= (~virt_in [IO_SUB_FUNC]);
			}
			break;

		case (FORCE_SWITCH):
			if (virt_in [IO_STATE])
			{
				force_switch |= virt_in [IO_SUB_FUNC];
				door_zone |= virt_in [IO_SUB_FUNC] << 8;
			}
			else
			{
				force_switch &= (~virt_in [IO_SUB_FUNC]);
				door_zone &= ~(virt_in [IO_SUB_FUNC] << 8);
			}
			break;

		case (SPECIAL_FUNC):								/* special functions							*/
			switch (virt_in [IO_SUB_FUNC])					/* handle sub function						*/
			{
				case (FAN_1):								/* cabin fan push								*/
					if (virt_in [IO_STATE])					/* cabin fan push presses						*/
					{
						if (fan_state)						/* cabin fan is on								*/
						{
							fan_state = 0;					/* switch fan off								*/
						}
						else								/* cabin fan is off								*/
						{
							fantimer = timer + (p.fan_time SEC);	/* start fan timer							*/
							fan_state = 1;					/* switch fan on								*/
						}
						set_out (SPECIAL_FUNC, FAN_1, 0, 0, fan_state, (O_CANA|O_CANB|O_HSE));
					}
					break;

				case (DOOR_OPEN):						/* door open push							*/
					if ((!virt_in [IO_FLOOR]) ||			/* door open push inside the car				*/
						((virt_in [IO_FLOOR] - 1) == level))	/* door open push in a landing					*/
					{
						if (virt_in [IO_STATE] && (Enabal_opendoor))			/* door open push presses and enable when nuding on					*/
							{
								if((p.cooperate_sel1 & COOP1_THROUGH_DOOR) && 
										(virt_in [IO_DOOR] == (DOOR1 | DOOR2)))
									{//��ͨ�ŵĴ���
										door = virt_in [IO_DOOR] & p.doorpos[level] & p.she_doornumber;
										if(door)
											dooropenpush = door;
										else
											dooropenpush = virt_in [IO_DOOR] & p.doorpos[level];
									}
								else
									dooropenpush = virt_in [IO_DOOR];
								if ((!door_close_open) && (!firedoormode) && (hse_state == H_STANDSTILL) )
							//	if ((!door_close_open) && (!firedoormode) && (hse_state == H_STANDSTILL))
	
									{
										door = dooropenpush & p.doorpos[level];
										if(door)
											set_out (SPECIAL_FUNC, DOOR_OPEN, 0, EXISTING_DOORS, 1, O_CANA);
									}
							}
						else							/* door open push off							*/
							{
								dooropenpush = 0;
								if(((door_state[0] == DOOR_OPENED) || ((door_state[1] == DOOR_OPENED) && (p.doornumber > 1)))
										&& (!door_close_open) && (!firedoormode))
									set_out (SPECIAL_FUNC, DOOR_OPEN, 0, EXISTING_DOORS, 0, O_CANA);
							}
					}
					break;

				case (DOOR_CLOSE):						/* door close push								*/
					if ((!virt_in [IO_FLOOR]) ||				/* door close push inside the car				*/
					((virt_in [IO_FLOOR] - 1) == level))		/* door close push in a landing					*/
					{
						if ((virt_in [IO_STATE]) && (callpriority != C_FIREALARM))				/* door close push presses						*/
							{
								doorclosepush = virt_in [IO_DOOR];
								if ((!door_close_open) && (!firedoormode) && (hse_state == H_STANDSTILL))
									{
										door = doorclosepush & p.doorpos[level];
										if(door)
											set_out (SPECIAL_FUNC, DOOR_CLOSE, 0, EXISTING_DOORS, 1, O_CANA);
									}
							}
						else								/* door close push off							*/
							{
								doorclosepush = 0;
								if(((door_state[0] == DOOR_CLOSED) || ((door_state[1] == DOOR_CLOSED) && (p.doornumber > 1)))
										&& (!door_close_open) && (!firedoormode))
									set_out (SPECIAL_FUNC, DOOR_CLOSE, 0, EXISTING_DOORS, 0, O_CANA);
							}
					}
					break;

				case (DOOR_STOP):						/* door stop push							*/
					if(p.landingcall_push & PUSH_STOP_SHIP)
						{
							door = virt_in [IO_DOOR];
							if (virt_in [IO_STATE])
								{
									she_doorstoppush |= door;
									++she_doorstoppush_count;
									if(she_doorstoppush_count > 2)		//���ֻ����2�� HOLD ����
										she_doorstoppush_count = 2;
								}
							else
								{
									if(she_doorstoppush_count)
										--she_doorstoppush_count;
									if((she_doorstoppush_count) && door < 3)
										{
											she_doorstoppush &= ~door;
											if(she_doorstoppush == 0)
												she_doorstoppush_count = 0;
										}
									else
										she_doorstoppush = 0;
								}							
							if(she_doorstoppush)
								{// HOLD ���صĴ���
									door = she_doorstoppush & p.doorpos[level] & p.she_doornumber;
									if(door)
										doorstoppush = door;
									else
										{
											if(p.doorpos[level] == 3)
												doorstoppush = p.doorpos[level] & she_doorstoppush;
											else
												doorstoppush = p.doorpos[level];
										}
								}
							else
								{
									doorstoppush = 0; 		
									doorstoppush_sheopen_flag = 0;
									doorstoppush_sheclose_flag = 0;
								}
						}
					else
						{
							if (virt_in [IO_STATE]) 				/* door close push presses						*/
								doorstoppush = virt_in [IO_DOOR];
						}
					break;		

				case (INTERMEDIATE_DOOR):					/* intermediate car door						*/
					intermediate_door = virt_in [IO_STATE];
					break;

				case (KEY_OFF):								/* remote off switch							*/
					if(p.custumer1 & CUSTUMER1_SHE_MODE)
						{							
							if (!virt_in [IO_STATE])						/* remote off switch is off 					*/
								{
									car_switched_off = 0;
									landings_off = 0;
								}
							else
								{
									car_switched_off = 1;
									landings_off = 1;
								}
						}
					else
						car_switched_off = virt_in [IO_STATE];
					break;
					
				case (OUT_OF_ORDER):						/* input to set the "Out of order" output"		*/
					out_of_order_input = virt_in [IO_STATE];
					break;
				case (EARTHQUAKE):
					if (virt_in [IO_STATE])
						earthquake = 1;
					break;

				case (REMOTE_OFF):							/* remote off switch							*/
					if (!virt_in [IO_STATE])						/* remote off switch is off						*/
					{
						remote_switched_off = 0;
						remote_off &= ~ONE_OF_N;
						remote_clearcalls = 0;
					}
					else if (!(remote_off & ONE_OF_N)	)					/* state of remote off switch changed			*/
					{
						if (p.remote_off_func & REMOTE_CALL)					/* clear all calls								*/
							remote_clearcalls = 1;
						remote_off  |= ONE_OF_N;
						remote_call = virt_in [IO_FLOOR] - 1;
						remote_door = virt_in [IO_DOOR];
					}
					break;

				case (HALL_CALL_DISABLE):					/* landing calls off							*/
					landings_off_key = virt_in [IO_STATE];
					break;
					
				case (UPS_EVACUATION):						/* UPS evacuation 						*/
					if ((!ups_power) && virt_in [IO_STATE])
						ups_startcount = 0; 				/* reset UPS start count						*/
					ups_power = virt_in [IO_STATE];
					set_out (SPECIAL_FUNC, UPS_EVACUATION, 0, 0, ups_power, (O_CANA|O_CANB|O_HSE));
					break;					

				case (LIGHT_SWITCH):
					light_switch_status = virt_in [IO_STATE];
					if (light_switch_status)
						set_out (CARLIGHT, 0, 0, 0, 0, (O_CANA|O_CANB|O_HSE));
					else
					{
						if (carlight_switched_off)
							set_out (CARLIGHT, 0, 0, 0, 0, (O_CANA|O_CANB|O_HSE));
						else
							set_out (CARLIGHT, 0, 0, 0, 1, (O_CANA|O_CANB|O_HSE));
					}
					break;

				case (FAN_SWITCH):
					fan_switch_status = virt_in [IO_STATE];
					if (fan_switch_status)
						set_out (SPECIAL_FUNC, FAN_1, 0, 0, 0, (O_CANA|O_CANB|O_HSE));
					else
						set_out (SPECIAL_FUNC, FAN_1, 0, 0, fan_state, (O_CANA|O_CANB|O_HSE));
					break;
					

//=========================             �����Ǻ�����صĹ���               =========================
					case (KEY_SPECIAL):
						special_key = virt_in [IO_STATE]; 	/* save state of special key on 				*/
						break;
					
					case (KEY_ADVANCED):
						advanced_key = virt_in [IO_STATE];		/* save state of advanced key on				*/
						break;
					
					case (KEY_EMERGENCY):
						emergency_key = virt_in [IO_STATE]; 	/* save state of emergency key on 			*/
						break;
						
					case (TIMER1_TRIP): 						/* Timer 1 trip 							*/
						if ((callpriority == C_STANDARD) && 		/* special call mode							*/
								(timer1_trip != virt_in [IO_STATE]))			/* state of timer 1 trip changed					*/
							{
								if (timer1_trip)							/* timer 1 input is on							*/
									{
										if (p.timer_clearcall & 0x01) 		/* cancel calls when input is switched on 	*/
											clearcalls (ALL_CALLS);
									}
								else									/* timer 1 input is off 						*/
									{
										if (p.timer_clearcall & 0x02) 		/* cancel calls when input is switched off		*/
											clearcalls (ALL_CALLS);
									}
							}
						timer1_trip = virt_in [IO_STATE];
						break;
						
					case (TIMER2_TRIP): 						/* Timer 2 trip 							*/
						if ((callpriority == C_STANDARD) && 		/* special call mode							*/
								(timer2_trip != virt_in [IO_STATE]))			/* state of timer 2 trip changed					*/
							{
								if (!timer2_trip) 					/* timer 2 input was off						*/
									{
										if (p.timer_clearcall & 0x01) 		/* cancel calls when input is switched on 	*/
											{
												clearcalls (ALL_CALLS);
												floor = p.timer2_parkfloor;
												door = p.doortable [DOOR_TIMER_2][floor] & p.doorpos [floor];
												if (door)
													{
														calltab [floor].calltype |= CARCALL;
														calltab [floor].cc_door  |= door; 	
													}
											}
									}
								else									/* timer 2 input was on 					*/
									{
										if (p.timer_clearcall & 0x02) 		/* cancel calls when input is switched off		*/
											clearcalls (ALL_CALLS);
									}
							}
						timer2_trip = virt_in [IO_STATE];
						break;

					case (UPS_BRAKE): 						/* UPS evacuation by brake opening				*/
						if ((!ups_brake) && virt_in [IO_STATE])
							ups_brake_new = 1;
						ups_brake = virt_in [IO_STATE];
						break;
						
//����Ӧ����Դ����
					case (EMP_POWER): 					/* fireman call 								*/
						emp_power = virt_in [IO_STATE];
						if (emp_power)
							{
								emp_floor = virt_in [IO_FLOOR] - 1; 	/* save floor for fire call 					*/
								emp_doors = virt_in [IO_DOOR];			/* save doors for fire call 					*/
							}
						break;
//����Ӧ����Դ����

					case (EVACUATION_ACTIVE): 						/* evacuation on / off	evac_active �ź���enable�ź�ͬ��������evac_active��ǰ 					*/
						if ((!evacuation) && (virt_in [IO_STATE]))
							{ 									/* evacuation switched on 					*/
								evacuation = 1;
								evacuationtimer = timer + (p.evacuationtime  SEC);
								evacuationdelay = timer + (p.evacuationdelay SEC);
								evacuationfloor = virt_in [IO_FLOOR] - 1;
								evacuationdoor	= virt_in [IO_DOOR] & p.doorpos [evacuationfloor];
							}
						else if ((!virt_in [IO_STATE]) && p.evac_autoreturn)
							{
								if (evacuation_ready)
									{
										evacuation_ready = 0;
										set_out (SPECIAL_FUNC, EVACUATION_READY, 0, 0, 0, (O_CANA|O_CANB|O_HSE));
									}
								evacuation = 0;
							}
						break;
						
					case (EVACUATION_READY):					/* evacuation of other lift ready - enable		*/
						evacuation_enable = virt_in [IO_STATE];
						if(evacuation_enable | evacuation_key)
							evaction_waittime_nomove = timer + (DWORD)p.evaction_waittime SEC;
						break;
						
					case (EVACUATION_KEY):
						evacuation_key = virt_in [IO_STATE];
						if(evacuation_enable | evacuation_key)
							evaction_waittime_nomove = timer + (DWORD)p.evaction_waittime SEC;
						break;
						
					case (EVACUATION_NORMAL): 				/* go back from evacuation to normal operation	*/
						evacuation_normal = virt_in [IO_STATE];
						if(evacuation_normal)
							evacuation = 0;
						break;

					case (CAR_ATTENDANCE):						/* car attendance 							*/
						car_attendance	= virt_in [IO_STATE];
						door_attendance = virt_in [IO_STATE]; 	/* door mode in car attendance					*/
						if((car_attendance) && (p.attend_carcall & ATTEND_CANCEL_CALL))
							clearcalls(ALL_STANDARD_CALLS);
						break;

					case (CALLDIR_UP)  :							/* set call direction to up 					*/
						if ((virt_in [IO_STATE]) && 				/* input is on								*/
						(hse_state == H_STANDSTILL))				/* only if the lift is not moving 				*/
						{
							calldir = DIR_UP;
							door_attendance = 0;					/* close doors automatically again				*/
							dir_attendance = 1; 				/* direction changed							*/
						}
						 break;
					
					case (CALLDIR_DN)  :							/* set call direction to down 				*/
						if ((virt_in [IO_STATE]) && 				/* input is on								*/
						(hse_state == H_STANDSTILL))				/* only if the lift is not moving 				*/
						{
							calldir = DIR_DN;
							door_attendance = 0;					/* close doors automatically again				*/
							dir_attendance = 1; 				/* direction changed							*/
						}
						break;
					
					case (CALL_BYPASS): 						/* call bypass								*/
						call_bypass = virt_in [IO_STATE];
						break;
					
					case (HALLCALL_BYPASS): 					/* hall call bypass 						*/
						if (virt_in [IO_STATE]) 					/* toggle by pressing the push					*/
						{
							if (hallcall_bypass)					/* hallcall_bypass was on before				*/
								hallcall_bypass = 0;				/* switch to on now 						*/
							else										/* hallcall_bypass was off before 			*/
								hallcall_bypass = 1;				/* switch to off now							*/
						}
						break;
					
					case (CLEAR_CALLS): 						/* clear all calls								*/
						if (virt_in [IO_STATE])
							clearcalls (ALL_CALLS);
						break;
					
					
					case (AUTO_CALL): 						/* auto-call function ("Sabbat-control")			*/
						auto_call = virt_in [IO_STATE];
						break;
					
					case (UP_PEAK_TRAFFIC): 					/* up peak traffic								*/
						up_peak_input = virt_in [IO_STATE];
						break;
					
					case (DN_PEAK_TRAFFIC): 					/* down peak traffic							*/
						dn_peak_input = virt_in [IO_STATE];
						break;

					case STEEL_BRAKE:		
						if(virt_in [IO_STATE])
							{
								if(!steel_broke)
									{
										steel_broke = virt_in [IO_STATE];
										write_errorhist (E_STEEL_BROKE, HSE_ID, W_STEEL_BROKE, 0);

										clearcalls(ALL_CALLS);
									}
							}
						else
							steel_broke = 0;
						break;

					case RESET_PITBOX:
						if((!virt_in [IO_STATE]) && (bFunc_flag & FUNC_PITBOX_RESET_HOLD))
							{
								i = INSP_PITBOX_ACTIVE;			//��Ҫ��λ��ť�ͷŲſ��Ը�λ
								bFunc_flag &= ~FUNC_PITBOX_RESET_HOLD;
							}
						else
							{
								i = 0;
								bFunc_flag |= FUNC_PITBOX_RESET_HOLD;
							}
//						door = i ^ (insp & (INSP_TOPCAR_ACTIVE | INSP_PITBOX_ACTIVE));
//						if((door & INSP_PITBOX_ACTIVE) && i)
						if((i & INSP_PITBOX_ACTIVE) && ((pitbox_reset_active & (PIO_1 | PIO_0)) == (PIO_1 | PIO_0)))
							{
								set_out(SPECIAL_FUNC, RESET_PITBOX, 0, 0, 0, O_CANA | O_CANB);
								pitbox_reset_active = 0;	
								insp &= ~INSP_PITBOX_ACTIVE;
							}
						break;

//=======================              ��������������              ====================================
					case (FIRE_SERVICE_ENABLE): 			/* fireman service enabled						*/
						firekey = virt_in [IO_STATE];
						break;
						
					case (FIRE_SERVICE):						/* fireman call 								*/
						firecall = virt_in [IO_STATE];
						if (firecall)
						{
							firefloor = virt_in [IO_FLOOR] - 1; 	/* save floor for fire call 					*/
							firedoors = virt_in [IO_DOOR];			/* save doors for fire call 					*/
						}
						break;						
						
					case (FIRE_EVACUATION): 					/* fire evacuation								*/
						firealarm = virt_in [IO_STATE] & (!fire_bypass);
						break;
						
					case (FIRE_ALARM):							/* fire alarm 								*/
						floor = virt_in [IO_FLOOR] - 1;
						if ((floor < MAX_FLOOR) && (!fire_bypass))
							{
								if (virt_in [IO_STATE]) 				/* input is on								*/
									bit_setd (fireinput [floor / 32], floor % 32);
								else
									bit_resetd (fireinput [floor / 32], floor % 32);
							}
						break;
						
					case (FIRE_ALARM_RETURN): 				/* fire alarm return						*/
						if (virt_in [IO_STATE])
							{
								if (callpriority == C_FIREALARM)			/* ignore input if lift not in fire alarm state */
									firealarmreturn = 1;
								else if((callpriority == C_FIREMAN) && (!firekey) && (!auto_fire))
									{//����Ա���в�����
										firecall = 0;
										firemode = 0; 									
									}
							}
						else
							firealarmreturn = 0;
						break;
											
					case FIRE_BYPASS: 				
						fire_bypass = virt_in [IO_STATE];
						break;

		  	}
			break;

		case (DISABLE_CARCALL):								/* disable car call or doors						*/
			floor = virt_in [IO_SUB_FUNC] - 1;
			if ((floor >= p.bot_floor) && (floor <= p.top_floor))
			{
				if (virt_in [IO_STATE])
					disable_carcall [floor] |= virt_in [IO_DOOR];
				else
					disable_carcall [floor] &= (~virt_in [IO_DOOR]);
			}
			break;

		case (ENABLE_CARCALL):								/* enable car call or doors						*/
			floor = virt_in [IO_SUB_FUNC] - 1;
			if ((floor >= p.bot_floor) && (floor <= p.top_floor))
			{
				if (virt_in [IO_STATE])
				{
					enable_carcall [floor] |= virt_in [IO_DOOR];
			  	if (active &&								/* call is pressed								*/
							(p.call_disable_enable & CALL_DISEN_AUTO_CC) &&							/* set call automatic							*/
							(calls_enabled & CARCALL)	&&				/* calls enabled								*/
							((callpriority == C_STANDARD) ||			/* standard call mode							*/
							((callpriority <= C_EMERGENCY) && (p.call_disable_enable & CALL_DISEN_ENCALL))))
					{
						if ((floor >= p.bot_floor) && (floor <= p.top_floor))
						{
							i = virt_in [IO_DOOR] & p.doorpos [floor];
							if (i)								/* read doors								*/
							{								/* set call and write call to call table			*/
								set_out (CAR_CALL, floor + 1, 0, i, 1, (O_CANA|O_CANB|O_HSE));
								calltab [floor].calltype |= CARCALL;
								calltab [floor].cc_door  |= i;
							}
						}
			  	}
				}
				else
  				enable_carcall [floor] &= (~virt_in [IO_DOOR]);
			}
			break;

		case (DISABLE_HALLCALL):								/* disable hall call or doors					*/
			floor = virt_in [IO_FLOOR] - 1;
			if ((floor >= p.bot_floor) && (floor <= p.top_floor))
			{
				if (virt_in [IO_STATE])
					disable_hallcall [floor] |= virt_in [IO_DOOR];
				else
					disable_hallcall [floor] &= (~virt_in [IO_DOOR]);
		  	}
			break;

		case (ENABLE_HALLCALL):								/* enable hall call or doors					*/
			floor = virt_in [IO_FLOOR] - 1;
			if ((floor >= p.bot_floor) && (floor <= p.top_floor))
			{
				if (virt_in [IO_STATE])
				{
					enable_hallcall [floor] |= virt_in [IO_DOOR];
					if (active &&								/* call is pressed								*/
							(p.call_disable_enable & CALL_DISEN_AUTO_LC) &&							/* set call automatic							*/
							(calls_enabled & ALL_HALL_CALLS) &&		/* calls enabled								*/
							((callpriority == C_STANDARD) ||			/* standard call mode							*/
							((callpriority <= C_EMERGENCY)  && (p.call_disable_enable & CALL_DISEN_ENCALL))))
					{
						if ((floor >= p.bot_floor) && (floor <= p.top_floor))
						{
							door = virt_in [IO_DOOR] & p.doorpos [floor];
							if (door)							/* read doors								*/
							{
								dir = virt_in [IO_SUB_FUNC];
								switch (dir)					/* direction for automatic call					*/
								{
									case (HALL_CALL_UP):		/* up hall call								*/
										calltab [floor].calltype |= LANDINGCALL_UP;
										calltab [floor].lcu_door |= door;
										set_out (HALL_CALL, dir, floor + 1, door, 1, O_ALL);
										break;

									case (HALL_CALL_DOWN):	/* down hall call								*/
										calltab [floor].calltype |= LANDINGCALL_DN;
										calltab [floor].lcd_door |= door;
										set_out (HALL_CALL, dir, floor + 1, door, 1, O_ALL);
										break;

									case (HALL_CALL_NO_DIR):	/* hall call without direction					*/
										calltab [floor].calltype |= LANDINGCALL;
										calltab [floor].lc_door  |= door;
										set_out (HALL_CALL, dir, floor + 1, door, 1, O_ALL);
										break;
								}
							}
						}
					}
				}
				else
  				enable_hallcall [floor] &= (~virt_in [IO_DOOR]);
			}
			break;

		case (CARLIGHT):			/* car light input								*/
			if (virt_in [IO_STATE]) 							/* car light is on								*/
				{
					if (!carlight)									/* car light was off							*/
						carlight = 1;
					set_out (EMERGENCY_LIGHT, 0, 0, 0, 0, (O_CANA|O_CANB|O_HSE));
				}
			else												/* car light is off 							*/
				{
					if (carlight) 								/* car light was on 						*/
						{
							carlight = 0;
							set_out (EMERGENCY_LIGHT, 0, 0, 0, 1, (O_CANA|O_CANB|O_HSE));
						}
				}
			carlight = 1; 

			break;

		case (DOORTEST):										/* Doortest input 							*/
			if (active && (hse_state != H_DOORTEST))
				doortest_command = DR_STOP;
			if (virt_in [IO_STATE]) 							/* switch is on 							*/
				doortest_active |= 0x02;
			else												/* switch is off								*/
				doortest_active &= (~0x02);
			break;
			
		case (DOOR_LS): 									/* light screen input 						*/
			if (virt_in [IO_STATE]) 							/* light screen is on (interrupted) 			*/
				ls_state |= virt_in [IO_DOOR];
			else
				ls_state &= ~virt_in [IO_DOOR];
			break;

		case (DOOR_IO):										/* door input									*/
			i = 0;
			while ((!(virt_in [IO_DOOR] & (1 << i))) && (i <= MAX_DOOR))
		  		i++;
			if ((i < p.doornumber) && (virt_in [IO_SUB_FUNC] != DOOR_CL_LIMIT))
			{
				if (virt_in [IO_STATE])							/* switch is on								*/
					door_io [i] |= virt_in [IO_SUB_FUNC];
				else											/* switch is off								*/
					door_io [i] &= (~virt_in [IO_SUB_FUNC]);
				door_io [i] &= ~NOT_AVAILABLE;
	  	}
			if(virt_in [IO_SUB_FUNC] == DOOR_REV)
				{//��ȫ����
					if (virt_in [IO_STATE]) 							/* light screen is on (interrupted) 			*/
						ls_state |= (virt_in [IO_DOOR] << 4);
					else
						ls_state &= ~(virt_in [IO_DOOR] << 4);
				}
			if(virt_in [IO_SUB_FUNC] == DOOR_CL_LIMIT)
				{//���ŵ�λ����
					if (virt_in [IO_STATE]) 							/* light screen is on (interrupted) 			*/
						door_closeswitch_limit |= virt_in [IO_DOOR];
					else
						door_closeswitch_limit &= ~(virt_in [IO_DOOR]);
				}			
			break;

		case (LOAD_IN):										/* load measurement input					*/
			if (virt_in [IO_STATE])								/* input is on								*/
				load |= virt_in [IO_SUB_FUNC];
			else
				load &= ~virt_in [IO_SUB_FUNC];
			break;
			
		case (EM_OP_IN):
			if (virt_in [IO_STATE])								/* input is on								*/
				em_op |= virt_in [IO_SUB_FUNC];
			else
				em_op &= ~virt_in [IO_SUB_FUNC];

			if ((em_op & 0x06) && (stick_dect_timer >= timer) && (!emop_btn_stick))
			{
				emop_btn_stick = 1;
				write_errorhist (E_EMOP_BTN_STICK, HSE_ID, 0, 0);
			}
			else if (!(em_op & 0x06))
				emop_btn_stick = 0;
			break;

		case (SAFETY_CIRCUIT):
			if (virt_in [IO_STATE])								/* input is on								*/
				safety_circuit |= virt_in [IO_SUB_FUNC];
			else
				safety_circuit &= ~virt_in [IO_SUB_FUNC];
			if (virt_in [IO_STATE] && (virt_in [IO_SUB_FUNC] == SC1))
			{
				safety_circuit_1 = 1;							/* marker that safety circuit was on				*/
				limit_switch_error = 0;
			}
			break;

		case (BRAKE_CHECK):
			switch (virt_in [IO_SUB_FUNC])
			{
				case BRAKE1:
				case BRAKE2:
				case BRAKE3:
		  			if (virt_in [IO_STATE])
						brake_state |= virt_in [IO_SUB_FUNC];
					else
						brake_state &= ~virt_in [IO_SUB_FUNC];
					break;
			}
			break;

		case (CONT_CHECK):
			switch (virt_in [IO_SUB_FUNC])
			{
				case D_RUNCONT_SUP:
				case D_FXCONT_SUP:
				case D_BRKCONT_SUP:
	  			if (virt_in [IO_STATE])
						contactor_state |= virt_in [IO_SUB_FUNC];
					else
						contactor_state &= ~virt_in [IO_SUB_FUNC];
					break;
			}
			break;

		case (CONNECTED_IO):
			if (virt_in [IO_FLOOR] && (virt_in [IO_FLOOR] <= 8))
			{
				set_out (CONNECTED_IO, 0, virt_in [IO_FLOOR], 0, virt_in [IO_STATE], (O_CANA|O_CANB|O_HSE));
				if (virt_in [IO_STATE])
					bit_set (connected_io, virt_in [IO_FLOOR] - 1);
				else
					bit_reset (connected_io, virt_in [IO_FLOOR] - 1);
	  	}
			break;

		case (TEMPERATURE):
			switch (virt_in [IO_SUB_FUNC])
			{
				case (OVERTEMP1):
				case (OVERTEMP2):
				case (ROOMTEMP_MIN):
				case (ROOMTEMP_MAX):
		  		if (virt_in [IO_STATE])
					{
						if (!(temperature & virt_in [IO_SUB_FUNC]))
						{
							temperature |= virt_in [IO_SUB_FUNC];	/* set temperature input 						*/
							switch (virt_in [IO_SUB_FUNC])
							{
								case (OVERTEMP1):
									write_errorhist (E_MOTOR_TEMP1, HSE_ID, 0, W_MOTOR_TEMP);
									break;

								case (OVERTEMP2):
									write_errorhist (E_MOTOR_TEMP2, HSE_ID, 0, W_MOTOR_TEMP);
									break;

								case (ROOMTEMP_MIN):
									write_errorhist (E_ROOM_TEMP_MIN, HSE_ID, 0, 0);
									break;

								case (ROOMTEMP_MAX):
									write_errorhist (E_ROOM_TEMP_MAX, HSE_ID, 0, 0);
									break;
							}
							clearcalls (ALL_CALLS);					/* clear all calls								*/
						}
	  			}
					else
						temperature &= ~virt_in [IO_SUB_FUNC];		/* reset temperature input						*/
					break;

				case (WARNINGTEMP):
					break;
			}
			break;

		case (DOOR_FUNCTION):			
			switch (virt_in [IO_SUB_FUNC])					/* handle sub function						*/
				{
					case CARDOOR_CONNECT:
					case SHAFTDOOR_CONNECT:
						if (virt_in [IO_STATE])
							door_short_conn |= virt_in [IO_SUB_FUNC];
						else
							door_short_conn &= ~virt_in [IO_SUB_FUNC];
						break;
						
					default: break;
				}
			break;

		case (CAR_CALL):											/* car call									*/
			if (active)												/* call is pressed								*/
			{
				if (calls_enabled & (CARCALL | PRIOR_CARCALL))
				{												/* calls enabled								*/
					floor = virt_in [IO_SUB_FUNC] - 1;					/* read floor number							*/
					if ((floor >= p.bot_floor) && (floor <= p.top_floor))
					{
						i = virt_in [IO_DOOR];						/* read doors								*/
						if ((doublepresstimer > timer) &&				/* timer for quick double pressing				*/
						(old_callfloor == floor) &&						/* same call like before						*/
						(old_calldoor  == i))
						{
							set_out (CAR_CALL, floor + 1, 0, i, 0, (O_CANA|O_CANB|O_HSE));
							calltab [floor].cc_door &= ~i;				/* cancel all doors for this call					*/
							if (!calltab [floor].cc_door)				/* no other calls; clear whole car call			*/
								calltab [floor].calltype &= ~(CARCALL | PRIOR_CARCALL);
						}
						else if ((p.cc_cancel == SINGLE) &&			/* clar car call by pressing again				*/
						(calltab [floor].calltype & (CARCALL | PRIOR_CARCALL)) &&
						(calltab [floor].cc_door  & i))					/* car call already exist						*/
						{
							set_out (CAR_CALL, floor + 1, 0, i, 0, (O_CANA|O_CANB|O_HSE));
							calltab [floor].cc_door &= ~i;				/* clear doors								*/
							if (!calltab [floor].cc_door)				/* no other doors								*/
								calltab [floor].calltype &= ~(CARCALL | PRIOR_CARCALL);
						}
						else if (p.max_cc_empty &&					/* watch maximum number of car calls if empty	*/
								zeroload &&								/* car is empty								*/
								(callpriority == C_STANDARD) &&
								number_of_calls (p.max_cc_empty, CARCALL))
						{/* do nothing	��������¶�����ݲ��Ǽ�							*/
						}
						else
						{										/* set call acknowledgement					*/
							if (firemode)
								clearcalls (CARCALL);	
							door = p.doortable [adt][floor];
							if (!(p.call_disable_enable & CALL_DISEN_PRIORITY))					/* disable has higher priority than enable		*/
							{
								if ((callpriority == C_STANDARD) ||	/* standard call mode							*/
								((callpriority <= C_EMERGENCY) && (p.call_disable_enable & CALL_DISEN_ENCALL)))
									door |= enable_carcall [floor];	/* set enabled doors							*/
							}
							if ((callpriority == C_STANDARD) ||		/* standard call mode							*/
							((callpriority <= C_EMERGENCY) && (p.call_disable_enable & CALL_DISEN_DISCALL)))
								door &= (~disable_carcall [floor]);		/* clear disabled doors						*/
							if (p.call_disable_enable & CALL_DISEN_PRIORITY)						/* enable has higher priority than disable		*/
							{
								if ((callpriority == C_STANDARD) ||	/* standard call mode							*/
								((callpriority <= C_EMERGENCY) && (p.call_disable_enable & CALL_DISEN_ENCALL)))
									door |= enable_carcall [floor];	/* set enabled doors							*/
							}
							door = door & i & p.doorpos [floor];			/* only open called and available doors			*/
							if (door &&							/* enabled door on this floor					*/
							((!p.max_cc [callpriority]) ||				/* don't watch max. number of car calls			*/
							(!number_of_calls (p.max_cc [callpriority], CARCALL | PRIOR_CARCALL))))
							{									/* or number of allowed car calls not exceeded	*/
								if (p.clear_other_lc_if_cc)				/* if car call opposite to actual direction 		*/
									cancel_other_landing_call (floor);/* than cancel landing call in opposite dir.		*/
								set_out (CAR_CALL, floor + 1, 0, i, 1, (O_CANA|O_CANB|O_HSE));
								set_out (HALL_CALL, dir, floor+1, i, 1, O_ALL);
								if (calls_enabled & PRIOR_CARCALL)	/* write call to call table						*/
									calltab [floor].calltype |= PRIOR_CARCALL;
								else
									calltab [floor].calltype |= CARCALL;
								calltab [floor].cc_door  |= door;
							}
						}
						if (p.cc_cancel == DOUBLE)				/* clear call by quick double pressing enabled	*/
							{
								old_callfloor = floor;
								old_calldoor  = i;
								doublepresstimer = timer + 1 SEC;		/* start quick double press timer				*/
							}
					}
				}
		  	}
			else												/* call acknowledgement requested by SDO		*/
			{
				if (virt_in [IO_STATE])							/* call is acknowledged						*/
				{
					if (!(calls_enabled & CARCALL))				/* calls disabled								*/
						clearcalls (ALL_CALLS);					/* clear all calls								*/
					else
					{
						floor = virt_in [IO_SUB_FUNC] - 1;			/* read floor number							*/
						if ((floor >= p.bot_floor) && (floor <= p.top_floor) &&
						(virt_in [IO_DOOR] & p.doorpos [floor]))
						{									/* write call to call table						*/
							if (callpriority == C_STANDARD)		/* write call to call table						*/
								calltab [floor].calltype |= CARCALL;
							else
								calltab [floor].calltype |= PRIOR_CARCALL;
							calltab [floor].cc_door  |= virt_in [IO_DOOR];
						}
					}
				}
			}
			break;

		case (HALL_CALL):										/* hall call									*/
			if (active)											/* call is pressed								*/
				{
					landingcall_push = virt_in [IO_STATE];
					if (landingcall_push)									/* call is acknowledged						*/
						{
							floor = virt_in [IO_FLOOR] - 1;					/* read floor									*/
							if ((floor >= p.bot_floor) && (floor <= p.top_floor))
								{
									dir = virt_in [IO_SUB_FUNC];					/* read landing call direction					*/
									i = virt_in [IO_DOOR];						/* read doors								*/
									door = i & p.doortable [adt_hall][floor] & p.doorpos [floor];
									if (!(p.call_disable_enable & CALL_DISEN_PRIORITY))						/* disable has higher priority than enable		*/
										{
											if ((callpriority == C_STANDARD) ||		/* standard call mode							*/
											((callpriority <= C_EMERGENCY) && (p.call_disable_enable & CALL_DISEN_ENCALL)))
												door |= enable_hallcall [floor];		/* set enabled doors							*/
										}
									if ((callpriority == C_STANDARD) ||			/* standard call mode							*/
											((callpriority <= C_EMERGENCY) && (p.call_disable_enable & CALL_DISEN_DISCALL)))
										door &= (~disable_hallcall [floor]);			/* clear disabled doors						*/
									if (p.call_disable_enable & CALL_DISEN_PRIORITY)							/* enable has higher priority than disable		*/
										{
											if ((callpriority == C_STANDARD) ||		/* standard call mode							*/
													((callpriority <= C_EMERGENCY) && (p.call_disable_enable & CALL_DISEN_ENCALL)))
												door |= enable_hallcall [floor];		/* set enabled doors							*/
										}
									door &= i & p.doorpos [floor];					/* only open called and available doors			*/
									if (door)									/* enabled door on this floor					*/
										{
											switch (dir)									/* call direction								*/
												{
													case (HALL_CALL_UP):						/* up hall call								*/
														if ((calls_enabled & LANDINGCALL_UP)	/* up hall calls enabled						*/
																|| ((floor == p.bot_floor) && (calls_enabled & LANDINGCALL_DN)))
															{									/* if peak traffic also LC up from bottom floor	*/
																calltab [floor].calltype |= LANDINGCALL_UP;
																calltab [floor].lcu_door |= door;
																set_out (HALL_CALL, dir, floor + 1, i, 1, O_ALL);
																if(car_attendance)
																	{
																		if (p.attend_carcall & ATTEND_HALLCALL_CARENABLE)
																			{								/* if car attendance also create car call			*/
																				calltab [floor].calltype |= CARCALL;
																				calltab [floor].cc_door |= door;
																				set_out (CAR_CALL, floor + 1, 0, i, 1, (O_CANA|O_CANB|O_HSE));
																			}
																		if(p.attend_carcall & ATTEND_SPEEKER)
																			set_out (SPEAKER_BUZ, BUZZER_NORMAL, 3, SPEEKER_PAUSE, 1, O_CANA);
																	}
															}
														break;

													case (HALL_CALL_DOWN):					/* down hall call								*/
														if ((calls_enabled & LANDINGCALL_DN)	/* down hall calls enabled						*/
																|| ((floor == p.top_floor) && (calls_enabled & LANDINGCALL_UP)))
															{									/* if peak traffic also LC dowm from top floor	*/
																calltab [floor].calltype |= LANDINGCALL_DN;
																calltab [floor].lcd_door |= door;
																set_out (HALL_CALL, dir, floor+1, i, 1, O_ALL);
																if(car_attendance)
																	{
																		if (p.attend_carcall & ATTEND_HALLCALL_CARENABLE)
																			{ 							/* if car attendance also create car call 		*/
																				calltab [floor].calltype |= CARCALL;
																				calltab [floor].cc_door |= door;
																				set_out (CAR_CALL, floor + 1, 0, i, 1, (O_CANA|O_CANB|O_HSE));
																			}
																		if(p.attend_carcall & ATTEND_SPEEKER)
																			set_out (SPEAKER_BUZ, BUZZER_NORMAL, 3, SPEEKER_PAUSE, 1, O_CANA);
																	}
															}
														break;

													case (HALL_CALL_NO_DIR):					/* hall call without direction					*/
														if (calls_enabled & LANDINGCALL)		/* hall calls without direction enabled			*/
															{
																calltab [floor].calltype |= LANDINGCALL;
																calltab [floor].lc_door  |= door;
																set_out (HALL_CALL, dir, floor+1, i, 1, O_ALL);
																if(car_attendance)
																	{
																		if (p.attend_carcall & ATTEND_HALLCALL_CARENABLE)
																			{ 							/* if car attendance also create car call 		*/
																				calltab [floor].calltype |= CARCALL;
																				calltab [floor].cc_door |= door;
																				set_out (CAR_CALL, floor + 1, 0, i, 1, (O_CANA|O_CANB|O_HSE));
																			}
																		if(p.attend_carcall & ATTEND_SPEEKER)
																			set_out (SPEAKER_BUZ, BUZZER_NORMAL, 3, SPEEKER_PAUSE, 1, O_CANA);
																	}
															}
														break;
												}
										}
								}
						}						/* set call acknowledgement					*/
				}
			else														/* call acknowledgement requested by SDO		*/
			{				
				landingcall_push = virt_in [IO_STATE];
				if (virt_in [IO_STATE])									/* call is acknowledged						*/
				{
					if ((calls_enabled & ALL_HALL_CALLS) != ALL_HALL_CALLS)
			 			set_out (HALL_CALL, 0, 0xFF, 0, 0, O_ALL);			/* clear all landing calls						*/
					else
					{
						floor = virt_in [IO_FLOOR] - 1;					/* read floor									*/
						if ((floor >= p.bot_floor) && (floor <= p.top_floor))
						{
							i = virt_in [IO_DOOR];						/* read door 									*/
							if (i & p.doorpos [floor])
							{
								switch (virt_in [IO_SUB_FUNC])			/* call direction								*/
								{
									case (HALL_CALL_UP):				/* up call									*/
										calltab [floor].calltype |= LANDINGCALL_UP;
										calltab [floor].lcu_door |= i;
										break;

									case (HALL_CALL_DOWN):			/* down call									*/
										calltab [floor].calltype |= LANDINGCALL_DN;
										calltab [floor].lcd_door |= i;
										break;

									case (HALL_CALL_NO_DIR):			/* hall call without direction					*/
										calltab [floor].calltype |= LANDINGCALL;
										calltab [floor].lc_door  |= i;
										break;
								}
							}
						}
					}
				}
			}
			break;

		case (HALL_CALL_SPECIAL):									/* special hall call							*/
		case (HALL_CALL_ADVANCED):									/* advanced hall call							*/
		case (HALL_CALL_EMERGENCY):									/* emergency hall call							*/
			if (active)													/* call is pressed								*/
			{
				i = virt_in [IO_DOOR];									/* read door 									*/
				floor = virt_in [IO_SUB_FUNC] - 1;							/* read floor									*/
				door  = i & p.doorpos [floor];
				if (door)
				{
					if ((floor >= p.bot_floor) && (floor <= p.top_floor))
					{
						switch (virt_in [IO_BASIC_FUNC])
						{
							case (HALL_CALL_SPECIAL):				/* special hall call							*/
								if (virt_in [IO_STATE])					/* call is pressed								*/
								{
									if (calls_enabled & SPECIAL_CALL)	/* special hall calls enabled					*/
									{
										calltab [floor].calltype |= SPECIAL_CALL;
										calltab [floor].sc_door  |= door;
										set_out (virt_in [IO_BASIC_FUNC], floor + 1, 0, i, 1, O_ALL);
									}
									priorstate [0][floor] |= i;
								}
								else
									priorstate [0][floor] &= ~i;
								break;

							case (HALL_CALL_ADVANCED):				/* advanced hall call							*/
								if (virt_in [IO_STATE])					/* call is pressed								*/
								{
									if (calls_enabled & ADVANCED_CALL)	/* advanced hall calls enabled					*/
									{
										calltab [floor].calltype |= ADVANCED_CALL;
										calltab [floor].ac_door  |= i;
										set_out (virt_in [IO_BASIC_FUNC], floor + 1, 0, i, 1, O_ALL);
									}
									priorstate [1][floor] |= i;
								}
								else
									priorstate [1][floor] &= ~i;
								break;

							case (HALL_CALL_EMERGENCY):				/* emergency hall call							*/
								if (virt_in [IO_STATE])					/* call is pressed								*/
								{
									if (calls_enabled & EMERGENCY_CALL)
									{								/* emergency hall calls enabled				*/
										calltab [floor].calltype |= EMERGENCY_CALL;
										calltab [floor].ec_door  |= i;
										set_out (virt_in [IO_BASIC_FUNC], floor + 1, 0, i, 1, O_ALL);
									}
									priorstate [2][floor] |= i;
								}
								else
									priorstate [2][floor] &= ~i;
								break;
						}
					}
				}
			}
			else														/* call acknowledgement requested by SDO		*/
			{
				if (virt_in [IO_STATE])									/* call is acknowledged						*/
				{
					floor = virt_in [IO_SUB_FUNC] - 1;						/* read floor									*/
					if ((floor >= p.bot_floor) && (floor <= p.top_floor))
					{
						i = virt_in [IO_DOOR];							/* read door 									*/
						if (i == p.doorpos [floor])
						{
							switch (virt_in [IO_BASIC_FUNC])
							{
								case (HALL_CALL_SPECIAL):			/* special hall call							*/
									if (!(calls_enabled & SPECIAL_CALL))
										set_out (HALL_CALL_SPECIAL, 0, 0xFF, 0, 0, O_ALL);
									else
									{
										calltab [floor].calltype |= SPECIAL_CALL;
										calltab [floor].sc_door  |= i;
									}
									break;

								case (HALL_CALL_ADVANCED):			/* advanced hall call							*/
									if (!(calls_enabled & ADVANCED_CALL))
										set_out (HALL_CALL_ADVANCED, 0, 0xFF, 0, 0, O_ALL);
									else
									{
										calltab [floor].calltype |= ADVANCED_CALL;
										calltab [floor].ac_door  |= i;
									}
									break;

								case (HALL_CALL_EMERGENCY):			/* emergency hall call							*/
									if (!(calls_enabled & EMERGENCY_CALL))
										set_out (HALL_CALL_EMERGENCY, 0, 0xFF, 0, 0, O_ALL);
									else
									{
										calltab [floor].calltype |= EMERGENCY_CALL;
										calltab [floor].ec_door  |= i;
									}
									break;
							}
						}
					}
				}
			}
			break;

		case (ALARM_CALL):											/* alarm call									*/
			if (virt_in [IO_STATE])									/* alarm call is pressed, write to error stack		*/
				write_errorhist (E_ALARM_CALL, HSE_ID, 0, 0);
			break;		
	}
}



