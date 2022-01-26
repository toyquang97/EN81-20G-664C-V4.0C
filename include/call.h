
#ifndef _CALL_H_
#define _CALL_H_

#define CARCALL				0x01				// call type is car call
#define LANDINGCALL_UP		0x02				// call type is landing call up
#define LANDINGCALL_DN		0x04				// call type is landing call down
#define LANDINGCALL			0x08				// call type is landing call without direction
#define LANDINGCALLDIR		0x06				// call type is landing call up or down
#define SPECIAL_CALL		0x10				// call type is special call
#define ADVANCED_CALL		0x20				// call type is advanced call
#define EMERGENCY_CALL		0x40				// call type is emergency call
#define PRIOR_CARCALL		0x80				// call type is priority car call
#define CALL_UP				0x0B				// mask for standard calls in up direction
#define CALL_DN				0x0D				// mask for standard calls in down direction
#define ALL_CALLS			0xFF				// mask for all calls
#define PRIORITY_CALL		0xF0				// mask for all priority calls
#define ALL_STANDARD_CALLS	0x0F				// mask for all standard calls
#define ALL_HALL_CALLS		0x0E				// mask for all standard landing calls

#define NO_CALL				0					// no call to process
#define NEW_CALL				1					// call for other floor
#define CALL_ACT_FLOOR		2					// call for actual floor


struct CallTab 									// Structure for call table
{
	BYTE calltype;								// type of call
	BYTE cc_door;								// doors for car call
	BYTE lcu_door;								// doors for landing calls in up direction
	BYTE lcd_door;								// doors for landing calls in up direction
	BYTE lc_door;									// doors for landing calls with no direction
	BYTE sc_door;								// doors for special landing calls
	BYTE ac_door;								// doors for advanced landing calls
	BYTE ec_door;								// doors for emergency landing calls
};

struct GroupCall									// Structure for calls of other lifts
{
	BYTE floor;									// type of call
	BYTE type;									// doors for car call
	WORD time;									// doors for landing calls in up direction
	BYTE doors;									// doors for landing calls in up direction
};

extern struct CallTab calltab [];
extern struct GroupCall groupcall [];

BYTE number_of_calls (BYTE max, BYTE type);
void clearcalls (BYTE calltype);
void clear_groupcall (void);
void cancel_other_landing_call (BYTE callfloor);
BYTE get_call (BYTE lc_dir, BYTE fl, BYTE door, BYTE handle_lc);
BYTE get_moving_call (BYTE next_fl);


#endif

