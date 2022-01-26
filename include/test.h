
#ifndef _TEST_H_
#define _TEST_H_

#define SUPER_PROGRAM				88888888
#define PSW_PARA							66666666
#define DEFAULT_PSW_ADMIN			22222222
#define DEFAULT_PSW_HELP			33333333
#define DEFAULT_PSW_INSTALL		44444444
#define DEFAULT_PSW_MAINTAIN	55555555

BYTE VerifyChecksum (void);
DWORD GetChecksum(void * pointer, WORD size);
//void CompareParaDiff(void);
void set_floorhigth(void);
void handle_drive(void);
void pos_control(void);

#endif

