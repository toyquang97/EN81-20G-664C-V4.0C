
#ifndef _DICT_H_
#define _DICT_H_

DWORD read_dict (WORD index, BYTE subindex, BYTE bus);
DWORD read_segment (BYTE type);
DWORD write_dict (WORD index, BYTE subindex, BYTE type, BYTE *value, BYTE bus);
DWORD write_segment (BYTE type, BYTE *value, BYTE bus);


#endif

