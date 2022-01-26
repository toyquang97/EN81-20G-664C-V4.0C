
#ifndef _EEPROM_H_
#define _EEPROM_H_

#define	IIC_ADDR_EEPROM	0xA0

#define EE_BLOCK_SIZE		128			// EEPROM page��С
#define EE_CAP_KB		64ul		// EEPROM ����
#define EE_1K_SIZE		1024ul

//#define	TEST_EEPROM

//-----------���²������ݲ�����У��ͼ��㣬���������޸�---------------------
#define EE_PARAMETER		0x0000ul
#define EE_CHECKSUM		(EE_PARAMETER+sizeof(p))
//-----------���ϲ������ݲ�����У��ͼ��㣬���������޸�---------------------
#define EE_PASSWORD0		(EE_CHECKSUM+sizeof(para_checksum))
#define EE_PASSWORD1		(EE_PASSWORD0+sizeof(password_admin))
#define EE_PASSWORD2		(EE_PASSWORD1+sizeof(password_help))
#define EE_PASSWORD3		(EE_PASSWORD2+sizeof(password_install))
#define EE_TRIPCOUNT		(EE_PASSWORD3+sizeof(password_maintain))
#define EE_OPERTIME		(EE_TRIPCOUNT+sizeof(tripcount))
#define EE_LANGUAGE		(EE_OPERTIME+sizeof(op_time))
#define EE_TESTTRIP		(EE_LANGUAGE+sizeof(language))
#define EE_ERRHISTORY		(EE_TESTTRIP+sizeof(testtrip))
#define EE_ERRCOUNTER		(EE_ERRHISTORY+sizeof(errorhist))
#define	EE_TRIPCOUNT_LIMIT	(EE_ERRCOUNTER+sizeof(errorcounter))
#define	EE_FLOOR_HIGH		(EE_TRIPCOUNT_LIMIT+sizeof(tripcount_limit))
#define	EE_EXE_PARA		(EE_FLOOR_HIGH+sizeof(floor_higth))
#define	EE_FLOORCOUNT	(EE_EXE_PARA+sizeof(exe_para))
#define	EE_UCMP_ERROR_FLAG	(EE_FLOORCOUNT+sizeof(floorcount))

extern BYTE iic_busy;
extern DWORD eeprom_count;

#define EE_MAX_WRITERECORD	30
struct EE_WRITE_REC
{
	BYTE * pointer;
	WORD address;
	WORD size;
};
struct EE_WRITE_QUEUE
{
	struct EE_WRITE_REC rec [EE_MAX_WRITERECORD];
	BYTE read_rec;
	BYTE write_rec;
	BYTE count;
};
extern struct EE_WRITE_QUEUE ee_write_queue;

BYTE ReadEEData (void * pointer, WORD address, WORD size);
BYTE WriteEEData (void * pointer, WORD address, WORD size);

#ifdef TEST_EEPROM
extern BYTE testram[];
extern BYTE backram[];
BYTE EraseEeprom (void);
BYTE TestEeprom (void);
#endif

BYTE WriteParaData (void * pointer, WORD address, WORD size);

#endif

