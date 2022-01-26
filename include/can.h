
#ifndef _CAN_H_
#define _CAN_H_

#define ADDR_FILTER_RAM			0x40038000ul

#define CAN_INT_RECV				0x01ul
#define CAN_INT_SEND1			0x02ul
#define CAN_INT_SEND3			0x0400ul

#define CAN_A					0x00
#define CAN_B					0x01

#define CMD_TRAN_BUFF1			0x21		//缓冲区1发送SDO之外的消息
#define CMD_TRAN_BUFF2			0x41		//缓冲区2发送心跳
#define CMD_TRAN_BUFF3			0x81		//缓冲区3发送SDO消息
#define CMD_CLR_RECV			0x04			//释放接收缓冲器
#define CMD_CLR_DATAOVERRUN	0x08		//清除数据超载位

#define SEND_PRIORITY_HIGH		0x00
#define SEND_PRIORITY_MIDD		0x01
#define SEND_PRIORITY_LOW		0x02

#define SR_TCS2					(0x01ul << 11)


void init_can_a (void);
void init_can_b (void);
void init_can_c(void);

void ReadCanA (void);
void WriteCanA (void);
void ReadCanB (void);
void WriteCanB (void);
void ReadCanC (void);
void WriteCanC (void);

void send_hse_state (void);
void send_update_message (BYTE can);
void level_display (BYTE floor, BYTE bus);
void set_out (BYTE func, BYTE sub_func, BYTE floor, BYTE doors, BYTE state, BYTE bus);
void send_message (BYTE fct_code, BYTE node_id, BYTE length, BYTE *data, BYTE bus);
void transmit_sign (BYTE sign1, BYTE sign2, BYTE sign3, BYTE sign4, BYTE bus);
void transmit_cursortype (BYTE type, BYTE bus);
void transmit_cursorpos (BYTE row, BYTE column, BYTE bus);
void transmit_virtual_heartbeat(WORD key, BYTE bus);


BYTE check_txa (void);
void send_heartbeat_a (void);
void send_hse_state_a (void);
void send_nmt (BYTE state, BYTE node_id);
void transmit_sdo_req (BYTE node_id, BYTE command, WORD index, BYTE subindex, DWORD value);
void transmit_sdo_seg (BYTE node_id, BYTE command, BYTE *pointer, BYTE size);
void transmit_inputs (BYTE func, BYTE sub_func, BYTE state);
void door_command (BYTE door1, BYTE door2);
void transmit_load (BYTE state);
void transmit_emergency (WORD errortype);
void transmit_position (DWORD act_pos, WORD act_speed);
void level_display_a (BYTE floor);
void monitoring_message_cana(void);
void monitoring_message_canb(void);
void abort_sdo (DWORD errorcode);
void sdo_response (BYTE command, WORD index, BYTE subindex, DWORD value);
void sdo_segment (BYTE command, BYTE *value);
void set_out_a (BYTE func, BYTE sub_func, BYTE floor, BYTE doors, BYTE state);
void time_message_a (void);

BYTE check_txb (void);
void transmit_sdo_req_ese (WORD node_id, BYTE command, WORD index, BYTE subindex, DWORD value);
void transmit_sdo_seg_ese (WORD node_id, BYTE command, BYTE *pointer, BYTE size);
void sdo_response_b (BYTE command, WORD index, BYTE subindex, DWORD value);
void sdo_segment_b (BYTE command, BYTE *value);
void abort_sdo_b (DWORD errorcode);
void send_nmt_ese (BYTE state, WORD ese_number);
void level_display_b (BYTE floor);
void set_out_b (BYTE func, BYTE sub_func, BYTE floor, BYTE doors, BYTE state);
void send_heartbeat_b (void);
void send_hse_state_b (void);
void time_message_b (void);


void send_heartbeat_c(void);
void set_out_c (BYTE func, BYTE sub_func, BYTE floor, BYTE doors, BYTE state);
void set_hallcall_c(void);
void	send_drive_pos(void);


SWORD __GoToUpgrade(DWORD addr, DWORD mode, DWORD can);

#endif

