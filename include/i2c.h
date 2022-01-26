
#ifndef _I2C_H_
#define _I2C_H_

#define IIC_RSP_DLY		2200		// 1ms

BYTE iic_read(BYTE address, BYTE * data, BYTE count);
BYTE iic_write(BYTE address, BYTE * data, BYTE count);

#endif

