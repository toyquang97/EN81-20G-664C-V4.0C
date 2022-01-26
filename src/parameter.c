
#include "typedef.h"
#include "lpc1700.h"
#include "string.h"
#include "parameter.h"
#include "history.h"
#include "state.h"

DWORD password_admin;
DWORD password_help;
DWORD password_install;
DWORD password_maintain;
BYTE password_right = RIGHT_USER;
DWORD tripcount = 0;					// �г̼���
DWORD op_time = 0;						// ����ʱ��
BYTE language = CHINESE;
DWORD para_checksum = 0;
BYTE write_checksum = 0;

//�������в���
BYTE start_test_trip = 0;				// start test trip (simulate call)
BYTE testtrip = 0;						// ��������
BYTE testtimes = 0;
DWORD testtimer;
BYTE test_starttime;
BYTE test_endtime;
BYTE test_times;
BYTE test_interval;

BYTE iv_para_valid = 0;
BYTE dummy = 1;
DWORD tripcount_limit;
BYTE limitstatus = 0;
BYTE bTestUCMP = 0;
BYTE bTestLoad = 0;
BYTE bForceResetTrip = 0;				//ǿ�Ƹ�λ����
BYTE check_times = 0;
BYTE check_para_doing = 0;
BYTE para_checkbuff[EE_READCHECK_SIZE];
BYTE bTest_IlsSwitch = 0;
BYTE bBreak_check_enable = 1;			///0: ȡ����բ����		1: ��Ⱨբ����
BYTE bFx_check_enable = 1;			///0: ȡ�����ǹ���		1: �����ǹ���
BYTE bTemp_check_enable = 1;			///0: ȡ�����ƹ��¶ȼ��		1: �����ƹ��¶�
BYTE bTestDL = 0;							///1: �������е��Ŷ̽����
BYTE bTest_SIL = 0;							///1: ��������λ   2: ��������λ    3: 2����λ����
BYTE forced_stop = 0;
BYTE bTest_brake = 0;					///1: ���Ա�բ��
BYTE brake_value = 0;					//���Ա�բ���Ľ���: 1: �ϸ�    2: ���ϸ�
BYTE brake_value_old = 0;	
BYTE brake_check_forced = 0;
BYTE brake_check_data = 0;
BYTE brake_check_return = 0;
BYTE brake_check_noactive = 0;
BYTE brake_check_count = 0;
BYTE error_recode_timer = 0;

//*************************************************************************
// CAN variables CAN A
//*************************************************************************
BYTE rca = 0; 	     				// RX message counter
BYTE roa = 0;		    			// pointer for RX FIFO read
BYTE ria = 0;		    			// pointer for RX FIFO write
BYTE tca = 0; 	     				// TX message counter
BYTE toa = 0;		    			// pointer for TX FIFO read
BYTE tia = 0;		    			// pointer for TX FIFO write
BYTE tra = 1;					// TX ready
BYTE tea = 1;					// TX enabled
BYTE sca = 0;
BYTE sia = 0;
BYTE soa = 0;
BYTE sra = 1;
BYTE rxa[RXASW_SIZE][10];		// FIFO RX message buffer
BYTE txa[TXASW_SIZE][10];		// FIFO TX message buffer
BYTE sxa[SXASW_SIZE][10];		// FIFO SX message buffer
BYTE txa_fve_error = 20;
DWORD cana_busoff_checktime = 0;

//*************************************************************************
// CAN variables CAN B
//*************************************************************************
BYTE rcb = 0; 	     				// RX message counter
BYTE rob = 0;		    			// pointer for FIFO read
BYTE rib = 0;		    			// pointer for FIFO write
BYTE tcb = 0; 	     				// TX message counter
BYTE tob = 0;		    			// pointer for TX FIFO read
BYTE tib = 0;		    			// pointer for TX FIFO write
BYTE trb = 1;					// TX ready
BYTE teb = 1;					// TX enabled
BYTE scb = 0;
BYTE sib = 0;
BYTE sob = 0;
BYTE srb = 1;
BYTE rxb[RXBSW_SIZE][10];		// FIFO RX message buffer
BYTE txb[RXBSW_SIZE][10];		// FIFO TX message buffer
BYTE sxb[SXBSW_SIZE][10];		// FIFO SX message buffer
DWORD canb_busoff_checktime = 0;

//*************************************************************************
// CAN variables CAN C
//*************************************************************************
BYTE rcc = 0;		    			// RX message counter
BYTE roc = 0;		   				// pointer for FIFO read
BYTE ric = 0;		   				// pointer for FIFO write
BYTE tcc = 0;		    			// TX message counter
BYTE toc = 0;		   				// pointer for TX FIFO read
BYTE tic = 0;		   				// pointer for TX FIFO write
BYTE trc = 1;						// TX ready
BYTE tec = 1;						// TX enabled
BYTE canc_busoff_flag = 0;			// CANC bussoff flag
BYTE rxc[RXCSW_SIZE][10];			// FIFO RX message buffer
BYTE txc[TXCSW_SIZE][10];			// FIFO TX message buffer
BYTE txc_error = 20;
BYTE canctxerror = 0;				// CAN bus transmit not possible CAN C

//*************************************************************************
//	Variablen for free programmable IO
//*************************************************************************
BYTE out = 0x00;
DWORD in = 0x00;		// Variable to read physical inputs
DWORD input[3];
DWORD oldin = 0;
DWORD in_polarity = 0;			// inputs inverted
DWORD oldin_polarity;
BYTE out_polarity = 0;				// inputs not inverted
BYTE virt_in [MAX_IO_TYPE];				// virtual input (last changed)

//*************************************************************************
//	Error variables
//*************************************************************************
BYTE triperrorcount;			// Trip error count
BYTE errorwarning = 0;			// Error warning: many errors
//BYTE errorreg = 0;				// error register
//WORD errorcode = 0;			// error code
DWORD driveerror = 0;			// drive error
BYTE poserror = 0;				// error positioning system
WORD general_error = 0;		// general error
BYTE doorcloseerrorcount [MAX_DOOR] = { 0, 0 };
BYTE dooropenerrorcount [MAX_DOOR] = { 0, 0 };
BYTE dooropenerrorfloor [MAX_DOOR] = { 0xFF, 0xFF };
BYTE nextfloortrycount = 0;
BYTE save_hist;				// history still to save
BYTE save_all;
WORD * wptr_errcounter;

//*************************************************************************
//	SDO and LSS variables
//*************************************************************************
BYTE lss_res;					// response to LSS message
WORD sdo_id;				// response to SDO message
WORD hse_sdo_id;			// response to SDO message from HSE (Sync.)
WORD sdo_index = 0;			// save SDO index
BYTE sdo_subindex = 0;		// save SDO sub-index
WORD sdo_pcb = 0;			// save PCB for SDO
BYTE sdo_data [8];				// data of SDO message
BYTE tse_sdo_data [8];			// data of SDO message from TSE
BYTE ese_sdo_data [8];			// data of SDO message from ESE
BYTE hse_sdo_data [8];			// data of SDO message from HSE
BYTE exe_sdo_data [8];			// data of SDO message from HSE
DWORD sdo_timer = 0;			// timer for SDO segment transfer
WORD sdo_size = 0;			// size of SDO object
BYTE * sdo_address;			// address of SDO object
WORD sdo_pos = 0;			// last transferred BYTE inside SDO object
BYTE sdo_toggle = 0;			// toggle BYTE for SDO transfer
BYTE sdo_string = 0;			// marker if data type is string
BYTE sdo_master = 0;			// SDO transfer from normal software
BYTE tse_sdo_master = 1;		// TSE SDO transfer from normal software
BYTE ese_sdo_master = 1;		// ESE SDO transfer from normal software
BYTE exe_sdo_master = 1;		// ESE SDO transfer from normal software
BYTE sdo_interrupt = 0;			// SDO transfer from interrupt
BYTE handle_ese = 0;			// handle ESE boards

BYTE start_limit = 0;
BYTE reset_mcu = 0;

//*************************************************************************
//	Variables for LCD display
//*************************************************************************
WORD handheld_key = 0;								// key from handheld terminal
BYTE lcd_display [MAX_LCD_ROW][MAX_LCD_COL];			// LCD display in memory
BYTE lcd_old [MAX_LCD_ROW][MAX_LCD_COL];			// old LCD display in memory
BYTE cursortype;				// cursor type
BYTE cursortype_old;			// old cursor type
BYTE cursorpos [2];			// cursor position
BYTE cursorpos_old [2];		// old cursor position
BYTE lcdram = 1;				// content of user character generator RAM
BYTE handheld_active = 0;		// handheld terminal active
BYTE handheldtimer = 0;		// handheld supervision time
BYTE handheld_bak = 0;
BYTE handheld_dly = 0;

//*************************************************************************
//	Variables for special inputs
//*************************************************************************
BYTE dooropenpush = 0;		// door open push
BYTE doorclosepush = 0;		// door close push
BYTE doorstoppush = 0;		// door stop push
BYTE doorstopstate = 0;		// door stop state
BYTE special_key = 0;			// key switch special trip
BYTE advanced_key = 0;		// key switch advanced trip
BYTE emergency_key = 0;		// key switch emergency trip
BYTE intermediate_door = 0;		// contact from intermediate door
BYTE car_attendance = 0;		// car attendance
BYTE door_attendance = 0;		// door mode while car attendance
BYTE dir_attendance = 0;		// direction changed while car attendance
BYTE call_bypass = 0;			// bypass a floor while car attendance
BYTE hallcall_bypass = 0;		// bypass a hall call while car attendance
BYTE fire_bypass = 0;			//bypass firealaem

//SHE ��˾�Ǳ깦��ʹ��
BYTE doorstoppush_sheopen_flag = 0;		//���ű�־
BYTE doorstoppush_sheclose_flag = 0;		//�����ű�־
BYTE she_photonsensor = 0;
BYTE she_calldoor_double = 0;				//����ʱ��ͨ�ŵĴ���
BYTE she_calldir = 0;		//����ʱ��ͨ�ŵĴ���
BYTE she_doorstoppush = 0;
BYTE she_doorstoppush_count = 0;

//*************************************************************************
//	Variables for evacuation
//*************************************************************************
BYTE ups_power = 0;			// UPS power
BYTE ups_brake = 0;			// UPS power for brake opening
BYTE ups_brake_new = 0;		// state changed to UPS brake opening
BYTE ups_startcount = 0;		// UPS evacuation start count
BYTE evacuation = 0;			// Evacuation on / off
BYTE evacuation_enable = 1;	// Evacuation enable
BYTE evacuation_ready = 0;		// Evacuation ready
BYTE evacuation_normal = 0;	// Evacuation enable, but back to normal
DWORD evacuationtimer = 0;	// Max. waiting time in evacuation mode��
DWORD evacuationdelay = 0;	// Min. time delay in evacuation mode
BYTE evacuationfloor = 0xFF;	// evacuation floor
BYTE evacuationdoor = 0;		// doors to open in evacuation floor
BYTE evacuation_key = 0;
DWORD evaction_waittime_nomove = 0;

BYTE emp_power = 0;
BYTE emp_floor = 0;
BYTE emp_doors = 0;

//*************************************************************************
//	Variables for fireman mode
//*************************************************************************
BYTE firemode = 0;				// fireman mode
BYTE firedoormode = 0;			// door mode in fireman mode ����״̬���ŵĴ���ʽ(1: ��Ҫ�����Ű�ť   0: �����Զ�������, �˳�����Աģʽ��0)
BYTE firekey = 0;				// fireman key in car
BYTE auto_fire = 0;				// fireman key in car
BYTE firecall = 0;				// fireman call (fire key in fireman floor)
BYTE firefloor = 0xFF;			// fireman floor
BYTE firedoors = 0;			// doors in fireman floor
BYTE firecall_exist = 0;			// lift has fireman call key(�����������ݿ���: ���ص�SUB����Ϊ Fire call�� ���ϵ���)
BYTE firealarm = 0;				// fire alarm on
BYTE firealarmfloor = 0;			// evacuation floor in case of fire alarm
BYTE firealarmmode = 0;		// fire alarm mode
BYTE firealarmreturn = 0;		// input to return from fire alarm mode
DWORD fireinput [2] = {0, 0};		// fire alarm inputs in floor

//*************************************************************************
//	Variables for calls
//*************************************************************************
BYTE calldir = 0;				// direction for call processing
BYTE startdir = 0;
BYTE calldir_old = 0;			// old call direction
BYTE callfloor = 0xFF;  			// floor for next call
BYTE calltype = 0;				// call type next call
BYTE calldoors = 0;     			// doors for next call
BYTE lastcalltype = 0;			// last call type
BYTE lastcallfloor = 0; 			// last call floor
BYTE lastcalldoors = 0; 			// last call doors
BYTE next_callfloor = 0xFF;		// floor for possible next call (wait for group)
BYTE next_calltype = 0;			// call type possible next call	(wait for group)
BYTE next_calldoors = 0;		// doors for possible next call (wait for group)
WORD next_calltime = 0;		// time to process next call (wait for
BYTE landingcalldir = 0;			// variable to save direction of last land.call
BYTE calls_enabled = 0;		// enabled calls
BYTE handle_calls = 0;			// calls to handle
BYTE calls_off = 0;				// calls disabled (error, inspection...) 
BYTE carcallcounter = 0;		// counter for car calls
BYTE callpriority = 0;			// actual call priority
DWORD callprioritytimer = 0;	// timer for call priority
DWORD callenabledtimer = 0;	// timer for calls enabled in priority mode
DWORD groupcalldelay = 0;		// delay timer for group control 
BYTE calltype_old = 0;			// last call type
BYTE callfloor_old = 0;			// last call floor
BYTE calldoors_old = 0;			// last call doors
BYTE landings_off = 0;			// landing calls off (from menu)
BYTE landings_off_key = 0;		// landing calls off (from key)
BYTE priorstate [3][MAX_FLOOR];
BYTE parkcall = 0;				// floor for parking trip
BYTE doordelay = 0;			// group delay if problems with door

//*************************************************************************
//	Variables for up and down peak traffic
//*************************************************************************
BYTE up_peak_time  = 0;		// time  for up   peak traffic
BYTE dn_peak_time  = 0;		// time  for down peak traffic
BYTE up_peak_input = 0;		// input for up   peak traffic
BYTE dn_peak_input = 0;		// input for down peak traffic

//*************************************************************************
//	Variables for TSE boards
//*************************************************************************
BYTE tse_init_error;			// TSE board not init
BYTE tse_start_init = 0;		// TSE board init since last HSE start
BYTE tse_enabled;			// list of enabled TSE board
BYTE tse_hb_error;			// heartbeat error TSE
BYTE tse_hb_error_old;		// heartbeat error TSE
BYTE tse_hb_state = 0;		// old heartbeat state TSE (for display)
BYTE tse_state_request = 0;	// state request of TSE board necessary
BYTE tse_heartbeat;			// heartbeat message TSE
WORD tse_sdo_id;				// id of SDO producer

//*************************************************************************
//	Variables for EXE boards
//*************************************************************************
BYTE exe_init_error;			// TSE board not init
BYTE exe_start_init = 0;		// TSE board init since last HSE start
BYTE exe_enabled;			// list of enabled TSE board
BYTE exe_hb_error;			// heartbeat error TSE
BYTE exe_hb_error_old;		// heartbeat error TSE
BYTE exe_hb_state = 0;		// old heartbeat state TSE (for display)
BYTE exe_state_request = 0;	// state request of TSE board necessary
BYTE exe_heartbeat;			// heartbeat message TSE
WORD exe_sdo_id;				// id of SDO producer


//*************************************************************************
//	Variables for ESE boards
//*************************************************************************
DWORD ese_state_request[MAX_ESE_DW];	// ESE state request necessary
DWORD ese_init_error [MAX_ESE_DW];	// ESE board not init  ��Ӧλ = 1: δ��ʼ��
DWORD ese_start_init [MAX_ESE_DW];	// ESE board init since last HSE start
DWORD ese_enabled [MAX_ESE_DW];	// list of enabled ESE board
DWORD ese_hb_error [MAX_ESE_DW];	// heartbeat error ESE
DWORD ese_hb_error_old [MAX_ESE_DW];	// heartbeat error ESE
DWORD ese_hb_state [MAX_ESE_DW+1];	// old heartbeat state ESE (for display)
DWORD ese_heartbeat [MAX_ESE_DW];	// heartbeat message ESE
WORD ese_sdo_id;					// id of SDO producer

//*************************************************************************
//	Variables for drive
//*************************************************************************
WORD drivestatusword = 0;				// Drive status word
BYTE brake_check = 0;					// Brake check enabled
BYTE brake_state = 0;					// Brake state
BYTE drive_fault = 0;					// Drive fault input
BYTE contactor_check = 0;				// contactor check enabled
BYTE contactor_state = 0;				// Contactor state

//*************************************************************************
//	Variables for door table
//*************************************************************************
BYTE adt = 0;							// actual door table
BYTE adt_hall = DOOR_N_HALL;			// actual door table for hall calls

//*************************************************************************
//	Help parameter for basic setting
//*************************************************************************
BYTE save_double_door = 0;

//*************************************************************************
//	Statistic variables
//*************************************************************************
BYTE save_op_time = 0;				// write operation time to EEPROM
WORD floorcount [MAX_FLOOR];			// floor counter (trips to this floor)
BYTE floortried [MAX_FLOOR];
//*************************************************************************
// State variables
//*************************************************************************
BYTE hse_state;						// HSE state
BYTE hse_text;						// Text for HSE state
BYTE drive_state = 0;					// Drive state
BYTE rtc_lowpower = 0;
BYTE level = 0xFF;						// Actual floor
BYTE start_floor = 0;					// Start floor
BYTE target_floor = 0;					// Actual target floor
BYTE next_floor;						// Next possible target floor		����ͣ�������¥��
BYTE max_target;						// Maximum target floor		һ�㶼Ϊ��վ¥����
BYTE direction = 0;						// Actual drive direction
BYTE gong_direction = 0;		//��վ�ӵ�վ����
BYTE insp = 0;						// inspection switches
BYTE insp_dir = 0;						// inspection switches
BYTE em_op = 0;						// emergency recall operation switches
BYTE access_op = 0;						// Access operation switches
BYTE access_floor = 0;						// emergency recall operation switches
BYTE safety_circuit = 0;					// safety circuit state
BYTE safety_circuit_1 = 1;				// marker for safety circuit 1 state
BYTE limit_switch_error = 0;				// marker that final limit switch was off
WORD door_zone = 0;					// door zone state
BYTE door_zone_old = 0;
BYTE force_switch = 0;
WORD door_state [MAX_DOOR];			// Door state
BYTE door_io	[MAX_DOOR];				// Door input state
BYTE ls_state = 0;						// Light screen state
		//bit0: 1-->��1��Ļ�ź�(��������1�����Ű�ȫ��Ļ�ź�)
		//bit1: 1-->��2��Ļ�ź�(��������2�����Ű�ȫ��Ļ�ź�)
		//bit2: 1-->��3��Ļ�ź�(��������3�����Ű�ȫ��Ļ�ź�)
		//bit4: 1-->��1��ȫ�����ź�
		//bit5: 1-->��2��ȫ�����ź�
		//bit6: 1-->��3��ȫ�����ź�
BYTE doorstate;						// internal door state (closed/not closed)  1: ���ŵ�λ    0: δ���ŵ�λ
BYTE gongoutputenable = 0;				// gong output enable
BYTE doornew = 0;						// doors still to open in arrival state
BYTE adv_door_op_dly = 0;					// door open delay time(*5ms) after zone input detected in advanced door open mode
BYTE load = 0;						// load measurement state
BYTE overload = 0;						// car is overloaded
BYTE fullload = 0;						// car is full
BYTE zeroload = 0;						// car is empty
BYTE temperature = 0;					// temperature state of motor and machine room
BYTE mrt_state = 0;					// machine room temperature state
BYTE carlight = 1;						// car light state
BYTE term_active = 0;					// terminal mode (lift controlled from pushes)
BYTE doortest_active = 0;				// doortest mode (door controlled from pushes)
BYTE doortest_command = 0;			// doortest command
BYTE key_doorcommand = 0;			// doortest command from keys
BYTE teach_in = 0;						// teach in mode
BYTE teach_in_text = 0;					// teach in text
DWORD canerrorstate = 0;					// CAN bus error CAN A (bus off, overrun...)
BYTE canatxerror = 0;					// CAN bus transmit not possible CAN A
BYTE canbtxerror = 0;					// CAN bus transmit not possible CAN B
BYTE txb_ese_error = 0;					// transmit error counter CAN B
BYTE txa_tse_error = 0;					// transmit error counter for TSE
BYTE txa_exe_error = 0;					// transmit error counter for TSE
BYTE switchexist = 0;					// shows if door zone switch exists or not
BYTE doortimer [MAX_DOOR] = {0, 0};	//
BYTE fan_state = 0;					// state of cabin fan (on/off)
BYTE numberkey = 0;					// direct access with xxx* enabled (= 0)
BYTE savepara = 0;					// command to save all parameter
BYTE relevelnumber = 0;				// number of not successful re-levelling
BYTE relevelflag = 0;
BYTE dl_shorted = 0;			//�����Ķ̽�״̬
BYTE insp_return = 0;
BYTE relev_active = 0;
BYTE relev_new = 1;
BYTE diving_fail_cnt = 0;				// diving failed counts
BYTE ucmp_state = 0;		//���޲����޷���λUCMP���ϣ�ֻ�������ϵ���߲�������λ����
BYTE cte_mode_send = 0;
BYTE check_dl_short = 1;		//ʹ�������̽Ӽ��
BYTE door_off = 0;
BYTE sc_limit_fg = 0;
BYTE dl_short_test_finish = 0;

//*************************************************************************
//	State variables for special outputs
//*************************************************************************
BYTE fault_signal = 0;
BYTE out_of_order = 0;
BYTE out_of_order_input = 0;
BYTE out_of_order_menu = 0;
BYTE direction_ind = 0;
BYTE hall_lantern = 0;
BYTE occupied = 0;
BYTE carlight_switched_off = 0;
BYTE connected_io = 0;
BYTE overload_ind = 0;
BYTE fullload_ind = 0;
BYTE dooropenpush_ind = 0;
BYTE doorclosepush_ind = 0;
BYTE doorstop_ind = 0;
BYTE special_ind = 0;
BYTE advanced_ind = 0;
BYTE emergency_ind = 0;
BYTE switched_off_ind = 0;
BYTE remote_off_ind = 0;
BYTE firecall_ind = 0;
BYTE firefloor_ind = 0;
BYTE firekey_ind = 0;
BYTE specialmode = 0;
BYTE buzzer = 0;
BYTE door_closing = 0;
BYTE firealarmfloor_ind = 0;
BYTE dooroutputstate [MAX_DOOR] = {0, 0};
BYTE hallcall_bypass_ind = 0;

//*************************************************************************
//	Variables for special trips and special functions
//*************************************************************************
BYTE remote_switched_off = 0;			// remote switch off state
BYTE remote_off = 0;					// remote switch off input
BYTE remote_call = 0xFF;				// remote switch off floor
BYTE remote_door = 0;					// remote switch off doors
BYTE remote_clearcalls = 0;				// clear calls if remote switch off
BYTE car_switched_off = 0;				// switched off from car
BYTE earthquake = 0;					// earthquake (same function like switch off)
BYTE timer_trip = 0;					// timer trip (real timer)
BYTE timer1_trip = 0;					// timer 1 trip (input)
BYTE timer2_trip = 0;					// timer 2 trip (input)
BYTE enable_carcall [MAX_FLOOR];		
BYTE disable_carcall [MAX_FLOOR];		
BYTE enable_hallcall [MAX_FLOOR];		
BYTE disable_hallcall [MAX_FLOOR];	
BYTE auto_call = 0;					// auto-call function ("Sabbat-control")
BYTE auto_call_dir = DIR_UP;			// direction for auto-call function

//**************************************************************************
// variables to check heartbeat of all boards
//**************************************************************************
BYTE heartbeat = 0xFF;					// heartbeat message received
BYTE init_error = 0;					// boards not init
BYTE hb_error = 0;						// mandatory boards without heartbeat
BYTE start_init = 0;						// init since last start of HSE
BYTE hse_hb = 0;					// heartbeat of other HSE in group
BYTE hse_hb_state = 0;					// actual state of other HSE

//**************************************************************************
//	Timer variables
//**************************************************************************
DWORD timer = 0;						// general timer count variable
DWORD fxtimer = 0;
DWORD realtimer = 0;					// timer count variable (used in interrupt)
DWORD ulsi_off_timer = 0;
DWORD mstimer = 0;
DWORD ct;							// general timer for all state functions
DWORD parkingtimer;					// timer for parking trip
DWORD doorstoptimer = 0;				// timer for door stop function
DWORD esetimer;						// timer for error warning if ESE fault
DWORD tsetimer;						// timer for error warning if TSE fault
DWORD exetimer = 0;						// timer for error warning if TSE fault
DWORD fantimer = 0;					// timer for cabin fan
BYTE outtimer = 0;						// timer variable for setting output state
WORD brake_feedback_timer = 0;
WORD passwordtimer = 0;				// password timer
DWORD readload_timer = 0;
DWORD stick_dect_timer = 0;
DWORD canbus_check_timer = 0;
//**************************************************************************
//	Variables for monitoring
//**************************************************************************
BYTE monitoring_state = MONITORING_ON;	// state for the monitoring's state/
BYTE monitoring_delay_time = 3;			// every time sending after delay time

//**************************************************************************
// Variables for actual time
//**************************************************************************
BYTE time_new = 0;					// Time changed
BYTE second_toggle = 0;
BYTE menu_toggle_sec = 0;
BYTE send_heartbeat = 0;
BYTE sec_cnt = 0;
BYTE send_level = 0;
BYTE send_level_insp = 0;		//����״̬����¥��
BYTE level_insp_fg = 0;				//����ʱ¥�㽻����ʾ��־
BYTE settimeflag = 0;
DWORD error_timer = 0;			//500ms����

//**************************************************************************
//	String variables for software version of other boards
//**************************************************************************
BYTE version_fve [7];					// software version number of FVE
BYTE hardware_fve [7];					//���ڶ�ȡ CDU ��Ӳ���汾����� G-554C ʹ��

//**************************************************************************
//	Variables for insp key press
//**************************************************************************
BYTE para_valid = 0;
BYTE oldtse_arrowtype;
BYTE oldese_arrowtype;
BYTE oldtse_extnums;
BYTE emop_btn_stick = 0;

//**************************************************************************
//	Variables for id teach in
//**************************************************************************
BYTE tse_hb_timer = 16;
BYTE tse_state = TSE_WATCH;

BYTE ese_hb_timer = 40;//16;		�տ�ʼ�ϵ�20�����ȥ�������������
BYTE ese_state = ESE_WATCH;

BYTE exe_hb_timer = 40;
BYTE exe_state = EXE_WATCH;
//**************************************************************************
//	Variables for digit LED display
//**************************************************************************
BYTE led_buff[LED_BUFF_LEN];		//ż�� 8 �Σ� ����ΪС����, ������Ų�ΪD4D3D2D1

BYTE light_switch_status = 0;
BYTE fan_switch_status = 0;
BYTE dz_input = 0;
BYTE power24v = 1;
BYTE diving = 0;
BYTE bResetTripDoing = 0;
BYTE fve_hardware_ver_after_v13 = 0;
BYTE correct_level = 0;					// correct level mode
WORD floor_higth[MAX_FLOOR];	//¥��
BYTE floor_record_fg[MAX_FLOOR];

DWORD dTmrCnt = 0;
DWORD dCurrentPulse = 0;
DWORD dPosHcnt, dPosLcnt;

BYTE carlight_flag = 0;
BYTE alarmfloor_flag = 0;
BYTE landingcall_push = 0;
BYTE landingcall_actfloor = 0;
BYTE door_close_open = 0;
BYTE cl_op_fg = 0xAA;

WORD menu_keytimer = 0;
BYTE send_pos_fg = 0;
BYTE car_alarm = 0;
BYTE door_short_conn = 0;

//�ж�ǿ�����ñ���
BYTE check_sdd1_working_status = 0;
BYTE check_sdu1_working_status = 0;
BYTE check_sds_working_status = 0;
BYTE vu_not_on_recovery = 0;
BYTE vo_not_on_recovery = 0;
BYTE vovu_not_off_recovery = 0;
WORD sdd1_unwork_times = 0;
WORD sdu1_unwork_times = 0;
WORD sds_unwork_times = 0;

BYTE small_key_timer = 0;
BYTE bSmallkey_enable = 0;
BYTE move_dooropen_flag = 0;

//ģ������ʹ�ñ���
DWORD pos = INIT_POSITION;				// Actual lift position
BYTE simu_step = 0;

//CAN �Ż��ı���
BYTE can_disturb[3] = {0};
DWORD cana_error_count = 0;
DWORD canb_error_count = 0;
DWORD canc_error_count = 0;
BYTE can_error_timer = 0;
DWORD canb_heartbeat_time;

//*************************************************************************
//	CANopen specific variables
//*************************************************************************
//Virtual terminal ���ñ���
DWORD const device_type = MULTIPLE_DEVICES;
WORD const virtual_device [7] = {
	CALL_CONTROLLER >> 16,					// Call controller
	INPUT_PANEL_UNIT >> 16,					// Input panel unit
	OUTPUT_PANEL_UNIT >> 16,				// Output panel unit
	CAR_DOOR_CONTROLLER >> 16,			// Car door controller
	CAR_DRIVE_CONTROLLER >> 16,			// Car drive controller
	CAR_POSITION_UNIT >> 16,				// Car position unit
	LOAD_MEASURING_UNIT >> 16,				// Load measuring unit
};
	
//HHT ר�ò���

struct exu_parameter exe_para;

DWORD bFunc_flag = 0;
		//bit0:	 1-->�����źű仯
		//bit1:	 1-->��ʱ���Ű�ťʱ�䵽
		//bit2: 1-->	��վ��ҹ��ʱ�䵽
		//bit3: 1-->	VIP���е�ʱ�䵽
		//bit4: 1-->	���������е�ʱ�䵽
		//bit5: 1-->	IC ��ˢ����Ч
		//bit6: 1-->����ƽ��ʱ����
		//bit7: 1-->CANA ���ϵĳ�ʼ����־
		//bit8: 1-->CANA �Ѿ������˹���(�����־λ������ʹ��,���Ϸ���ʱӦ��ͣ��)
		//bit9:  1-->�����⵽�Ĵ���־
		//bit10: 1-->��Ƶ����⵽�Ĵ���־
		//bit11: 1-->���ݰ�ťָʾ��Ϩ���־
		//bit12: 1-->2��ǿ���ź�ͬʱ��Ч��־

WORD max_speed = 0;

BYTE bot_floor_old = 0;
BYTE top_floor_old = 0;
BYTE steel_broke = 0;

//�׿Ӽ�����صı���
BYTE pitbox_reset_active = 0;
	//bit0: �׿Ӽ��ޱ�����
	//bit1: �׿Ӽ��޴Ӽ���λ�ûָ�
BYTE alarm_filter = 0;

BYTE door_closeswitch_limit = ALL_DOORS_CLOSED;

//UCMP ������صı���
BYTE ucmp_test_finish = 0;
BYTE ucmp_test_floor = 0;
WORD ucmp_test_distance = 0;

#pragma arm section rwdata = "SRAM", zidata = "SRAM"
struct Parameter p;
struct ErrorHist errorhist;
struct ErrorHist hist;
#pragma arm section

WORD random (WORD max)
{
	static WORD ret;

	ret = ret + STCURR;
	return (ret % max);
}

// ȥ���ַ�����β�ո�
void trim(BYTE * str)
{
	WORD i, j;

	j = 0;
	while(str[j] == 0x20)
		++ j;
	if (j)
	{
		i = 0;
		while(1)
		{
			str[i] = str[j];
			if (str[j] == '\0')
				break;
			++ i;
			++ j;
		}
	}
	i = strlen((char *)str);
	while (i)
	{
		if (str[i-1] == 0x20)
			str[i-1] = '\0';
		else
			break;
		-- i;
	}
}


DWORD GetMin(DWORD a, DWORD b)
{
	if (a > b)
		return b;
	else
		return a;
}

DWORD GetMax(DWORD a, DWORD b)
{
	if (a > b)
		return a;
	else
		return b;
}


