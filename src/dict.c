
#include "typedef.h"
#include "parameter.h"
#include "text.h"
#include "can.h"
#include "nice5000.h"


/****************************************************************************************************/
/* Read object dictionary																			*/
/****************************************************************************************************/
DWORD read_dict (WORD index, BYTE subindex, BYTE bus)
{
	WORD pos;
	WORD i;
	WORD size;
	WORD address = 0;
	BYTE value [4];
	WORD pcb_number = 0;
	BYTE arraysize = 0;

	pos = 0;
	if ((index >= ESE_INPAR) && (index < (ESE_INPAR + MAX_ESE)))
	{
		pcb_number = index - ESE_INPAR;
		arraysize	= MAX_ESE_IN;
	}
	else if ((index >= ESE_OUTPAR) && (index < (ESE_OUTPAR + MAX_ESE)))
	{
		pcb_number = index - ESE_OUTPAR;
		arraysize	= MAX_ESE_OUT;
	}
	else if ((index >= TSE_INPAR) && (index < (TSE_INPAR + MAX_TSE)))
	{
		pcb_number = index - TSE_INPAR;
		arraysize	= MAX_TSE_IN;
	}
	else if ((index >= TSE_OUTPAR) && (index < (TSE_OUTPAR + MAX_TSE)))
	{
		pcb_number = index - TSE_OUTPAR;
		arraysize	= MAX_TSE_OUT;
	}
	index -= pcb_number;
	while (paratext [pos].index != ENDSTRING)			/* search whole dictionary						*/
	{
		if (paratext [pos].index == index)				/* found object									*/
		{
			switch (paratext [pos].object)				/* check subindex and access type				*/
			{
				case (D_VAR):								/* single variable								*/
					if (subindex)
						return (SDO_SUB_WRONG);					/* subindex > 0 for single variable				*/
					else if (!(paratext [pos].access & D_READ))
						return (SDO_READ_OF_WO);				/* read access to write only object				*/
					address = 0;
					break;
				case (D_ARRAY):								/* array of variables of the same type			*/
				case (D_RECORD):							/* any combination of simple variables			*/
					if (subindex > paratext [pos].subindex)
						return (SDO_SUB_WRONG);					/* sub index too big							*/
					else if ((!(paratext [pos].access & D_READ)) && subindex)
						return (SDO_READ_OF_WO);				/* write access to read only object				*/
					if (!subindex)							/* send number of entries						*/
					{
						if (bus)
							sdo_response_b (INIT_READ_RESP | EXPEDITED_BIT | WSIZE_1_DB | SIZE_IND, index + pcb_number, subindex, paratext [pos].subindex);
						else
							sdo_response (INIT_READ_RESP | EXPEDITED_BIT | WSIZE_1_DB | SIZE_IND, index + pcb_number, subindex, paratext [pos].subindex);
						return (0);
					}
					address = ((pcb_number * arraysize) + subindex - 1) * paratext [pos].size;
					break;
				case (D_DOMAIN):							/* Large amount of data e.g. program code		*/
					return (SDO_UNSUPPORTED);					/* to be done									*/
			}
			switch (paratext [pos].datatype)				/* get object size								*/
			{
				case (D_VIS_STRING):						/* object is visible string						*/
					size = 0;
					while ((*((BYTE *)paratext [pos].pointer + address + size)) && (size < paratext [pos].size))
						size++;
					if (!size)									/* if string lenght=0 transmit at least 1 byte	*/
						size++;
					break;
				case (D_REAL32):
					size = 4;
					break;
				default:
					size = paratext [pos].size;
					break;
			}
			if (size <= 4)								/* object size <= 4 (expedited transfer)		*/
			{
				for (i = 0; i < 4; i++)
					value [i] = 0;
				for (i = 0; i < size; i++)
					value [i] = *((BYTE *)paratext [pos].pointer + address + i);
				if (bus)
					sdo_response_b (INIT_READ_RESP | EXPEDITED_BIT | ((4 - size) << 2) | SIZE_IND, index + pcb_number, subindex, *(DWORD *)value);
				else
					sdo_response (INIT_READ_RESP | EXPEDITED_BIT | ((4 - size) << 2) | SIZE_IND, index + pcb_number, subindex, *(DWORD *)value);
			}
			else											/* object size > 4 (normal transfer)			*/
			{
				if (bus)
					sdo_response_b (INIT_READ_RESP | SIZE_IND, index + pcb_number, subindex, size);
				else
					sdo_response (INIT_READ_RESP | SIZE_IND, index + pcb_number, subindex, size);
				sdo_size = size;
				sdo_address = ((BYTE *)paratext [pos].pointer + address);
				sdo_pos = 0;
				sdo_toggle = 0;
				sdo_timer = timer + (SDO_TIMER SEC);
			}
			return (0);
		}
		pos++;											/* go to next dictionary entry					*/
	}
	return (SDO_NOT_EXIST);							/* no entry in object dictionary				*/
}

/****************************************************************************************************/
/* Read segment of an object																		*/
/****************************************************************************************************/
DWORD read_segment (BYTE type)
{
	BYTE value [7];
	WORD i;
	BYTE size = 0;
	BYTE x = 7;

	if (!sdo_size)									/* no init read request before					*/
		return (SDO_UNSUPPORTED);
	if (((type >> 4) & 0x01) != sdo_toggle)			/* toggle bit wrong								*/
		return (SDO_TOGGLEBIT);
	for (i = 0; i < 7; i++)
	{
		if (sdo_pos < sdo_size)
		{
			sdo_pos++;
			size++;
			x--;
			value [i] = *(sdo_address++);
		}
		else
			value [i] = 0;
	}
	i = READ_SEGM_RESP | (x << 1);
	if (sdo_toggle)
		i |= TOGGLE_BIT;								/* set toggle bit								*/
	sdo_toggle = !sdo_toggle;							/* alternate toggle bit							*/
	if (sdo_pos == sdo_size)							/* last segment									*/
	{
		i |= LAST_SEGM_BIT;
		sdo_size = 0;
	}
	sdo_segment (i, value);							/* transmit segment								*/
	sdo_timer = timer + (SDO_TIMER SEC);			/* restart SDO timer							*/
	return (0);
}

//*************************************************************************************
// clear all ESE parameter
//*************************************************************************************
void clear_ese (void)
{
	WORD i;
	BYTE j, k;

	for (i = 0; i < MAX_ESE; i++)
	{
		for (j = 0; j < MAX_ESE_IN; j++)
		{
			for (k = 0; k < 6; k++)
			{
				p.ese_inpar  [i][j][k] = 0;
				p.ese_outpar [i][j][k] = 0;
			}
		}
	}
	
	for (i = 0; i < MAX_ESE_DW; i++)
	{
		ese_enabled [i] = 0;
	}
}

/****************************************************************************************************/
/* Write object dictionary																			*/
/****************************************************************************************************/
DWORD write_dict (WORD index, BYTE subindex, BYTE type, BYTE *value, BYTE bus)
{
	WORD pos;
	BYTE i;
	BYTE size;
	WORD address = 0;
	WORD pcb_number = 0;
	BYTE arraysize = 0;

	pos = 0;
	if (index == CLEAR_ESE)
	{
		clear_ese ();
		if (bus)
			sdo_response_b (INIT_WRITE_RESP, index, subindex, 0);
		else
			sdo_response (INIT_WRITE_RESP, index, subindex, 0);
		return (0);
	}
	if ((index >= ESE_INPAR) && (index < (ESE_INPAR + MAX_ESE)))
	{
		pcb_number = index - ESE_INPAR;
		arraysize	= MAX_ESE_IN;
	}
	else if ((index >= ESE_OUTPAR) && (index < (ESE_OUTPAR + MAX_ESE)))
	{
		pcb_number = index - ESE_OUTPAR;
		arraysize	= MAX_ESE_OUT;
	}
	else if ((index >= TSE_INPAR) && (index < (TSE_INPAR + MAX_TSE)))
	{
		pcb_number = index - TSE_INPAR;
		arraysize	= MAX_TSE_IN;
	}
	else if ((index >= TSE_OUTPAR) && (index < (TSE_OUTPAR + MAX_TSE)))
	{
		pcb_number = index - TSE_OUTPAR;
		arraysize	= MAX_TSE_OUT;
	}
	index -= pcb_number;
	while (paratext [pos].index != ENDSTRING)			/* search whole dictionary						*/
	{
		if (paratext [pos].index == index)				/* found object									*/
		{
			switch (paratext [pos].object)				/* check subindex and access type				*/
			{
				case (D_VAR):								/* single variable								*/
					if (subindex)
						return (SDO_SUB_WRONG);					/* subindex > 0 for single variable				*/
					if (!(paratext [pos].access & D_WRITE))
						return (SDO_WRITE_OF_RO);				/* write access to read only object				*/
					address = 0;
					break;
				case (D_ARRAY):								/* array of variables of the same type			*/
				case (D_RECORD):							/* any combination of simple variables			*/
					if (subindex > paratext [pos].subindex)
						return (SDO_SUB_WRONG);					/* sub index too big							*/
					if ((!(paratext [pos].access & D_WRITE)) || (!subindex))
						return (SDO_WRITE_OF_RO);				/* write access to read only object				*/
					address = ((pcb_number * arraysize) + subindex - 1) * paratext [pos].size;
					break;
				case (D_DOMAIN):							/* Large amount of data e.g. program code		*/
					return (SDO_UNSUPPORTED);					/* to be done									*/
			}
			switch (paratext [pos].datatype)				/* get object size								*/
			{
				case (D_VIS_STRING):						/* object is visible string						*/
					sdo_string = 1;
					break;
				default:
					sdo_string = 0;
					break;
			}
			switch (index)								/* special handling for a few objects			*/
			{
				case (SAVEPARA):							/* save all parameter							*/
					if (numberkey)							/* access not enabled							*/
						return (SDO_LOCAL_CONTROL);
					break;
			}
			if (type & EXPEDITED_BIT)						/* expedited transfer							*/
			{
				if (type & SIZE_IND)						/* size indicated								*/
				{
					size = (4 - ((type >> 2) & 0x03));		/* read data size								*/
					if (size > paratext [pos].size)
						return (SDO_LENGTH);					/* Data size too big							*/
				}
				else
					size = paratext [pos].size;
				for (i = 0; i < paratext [pos].size; i++)	/* clear variable								*/
					*((BYTE *)paratext [pos].pointer + address + i) = 0;
				for (i = 0; i < size; i++)
					*((BYTE *)paratext [pos].pointer + address + i) = *(value + i);
				if (sdo_string)								/* string variable								*/
					*((BYTE *)paratext [pos].pointer + address + i) = 0;
			}
			else											/* normal transfer								*/
			{												/* to be done									*/
				if (type & SIZE_IND)						/* size indicated								*/
				{
					size = *value;								/* read data size								*/
					if (size > paratext [pos].size)
						return (SDO_OUT_OF_MEM);				/* Data size too big							*/
				}
				else
					size = paratext [pos].size;
				sdo_size = size;
				sdo_address = ((BYTE *)paratext [pos].pointer + address);
				sdo_pos = 0;
				sdo_index = index;
				sdo_subindex = subindex;
				sdo_pcb	= pcb_number;
				sdo_toggle = 0;
				sdo_timer = timer + (SDO_TIMER SEC);
			}
			if (bus)
				sdo_response_b (INIT_WRITE_RESP, index + pcb_number, subindex, 0);
			else
				sdo_response (INIT_WRITE_RESP, index + pcb_number, subindex, 0);

//增加测试仪修改变频器参数的处理
			if((index > NICE_INDEX) && niv_addr_match(index - NICE_INDEX))
				{
					pcb_number = *((WORD *)paratext [pos].pointer);
					send_nice5000_parameter(index - NICE_INDEX, pcb_number);
				}
//增加测试仪修改变频器参数的处理
			
			return (0);
		}
		pos++;											/* go to next dictionary entry					*/
	}
	return (SDO_NOT_EXIST);							/* no entry in object dictionary				*/
}

/****************************************************************************************************/
/* Write segment of an object																		*/
/****************************************************************************************************/
DWORD write_segment (BYTE type, BYTE *value, BYTE bus)
{
	BYTE i;
	if (!sdo_size)									/* no init write request before					*/
		return (SDO_UNSUPPORTED);
	if (((type >> 4) & 0x01) != sdo_toggle)			/* toggle bit wrong								*/
		return (SDO_TOGGLEBIT);
	for (i = 0; i < 7; i++)
	{
		if (sdo_pos < sdo_size)
		{
			sdo_pos++;
			*(sdo_address++) = *(value + i);
		}
	}
	if ((sdo_pos == sdo_size) && (!(type & LAST_SEGM_BIT)))
		return (SDO_BLOCKSIZE);							/* too many data blocks							*/
	i = WRITE_SEGM_RESP;
	if (sdo_toggle)
		i |= TOGGLE_BIT;								/* set toggle bit								*/
	sdo_toggle = !sdo_toggle;							/* alternate toggle bit							*/
	if (bus)
		sdo_segment_b (i, value);						/* transmit segment on CAN A					*/
	else
		sdo_segment (i, value);							/* transmit segment	on CAN B					*/
	if (type & LAST_SEGM_BIT)							/* last segment									*/
	{
		sdo_size = 0;
		if (sdo_string)									/* string variable								*/
			*sdo_address = 0;
	}
	else
	sdo_timer = timer + (SDO_TIMER SEC);			/* restart SDO timer							*/
	return (0);
}

