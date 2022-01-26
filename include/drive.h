
#ifndef _DRIVE_H_
#define _DRIVE_H_

void set_contactor (BYTE type, BYTE state);
void start_drive (BYTE drv_direction);
void emergency_stop (void);
void stop_drive (void);


#endif

