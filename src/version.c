
#include "typedef.h"
#include "parameter.h"
#include "version.h"
#include "bootloader.h"

//Version record
//V3.0C: Initial version
//V4.0C: On the basis of 3.0C, it meets the functions of EN81-20/50
// a. Increase the pit maintenance function
// b. After returning from the pit inspection, the indicator light of the reset box will be on, and only the reset button can be extinguished-the status page of the operator and the digital tube will give a reset prompt
// After the pit inspection switch is restored, if the reset box does not act, the system should remain in the inspection state, otherwise, the leveling action will be executed

// c. Add intercom filtering function: No alarm in the following 2 ways
// c.1: The car is located in the door zone, and the car door and landing door are fully open
// c.2: During elevator operation
// c.3: It is not allowed to filter any alarm signals during elevator maintenance
// d. The car top adds the door closing in-position mechanical switch detection for the function of the door bypass device
// e. Add the operation menu of UCM function test, refer to the test procedure provided to TUV
// f. According to TUV's definition of UCM function, modify the function program of door lock short-circuit detection
// g. The added K2 relay normally closed point, in the case of elevator abnormality (excluding inspection), becomes a normally open point ------- (unfinished)

// The following content is used for version check in bootloader or firmware check in software upgrade
// Do not change the size or order or insert or delete content at will
#pragma arm section code = "ER_IROM2", rodata = "ER_IROM2"

const DWORD identity [ID_NUMS] = 
{
	0x000000B5,								// vendor ID
	0x00000000,								// product code
	0x00000000,								// revision number
	0x00000000,								// serial number
};

const BYTE version_hse [SW_SIZE] = "V4.2A";		// actual software version of MCU

const BYTE hardware_hse [HW_SIZE] = "V1.3";		// actual hardware version of MCU
const BYTE device_name [DV_SIZE] = "G-664";		// name of PCB is G-664
const WORD version_fver = 0x5AA5;
const DWORD version_chksum = VERSION_CONTENT;

#pragma arm section

