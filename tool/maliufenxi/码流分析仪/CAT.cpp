#include "CAT.h"
#include "PAT.h"
#include "common.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "stdafx.h"
CAT *CATThis = NULL;
static CATheader ChangeCAT2BitStream(unsigned char * headerBuf, int num, int flag)
{
	//unsigned char tempchar;
	unsigned char tempbuff[12];
	CATheader   CATHeader = {0};
	for (int index = 0; index < num / 2; index++)
	{
		Swap(&headerBuf[index], &headerBuf[num - 1 - index], sizeof(unsigned char));
	}

	if (flag == 0)
	{
		memcpy(tempbuff+3, headerBuf, 5);
		memcpy(tempbuff + 9, headerBuf + 5, 3);
		CATHeader = *(CATheader*)tempbuff;
	}
	return CATHeader;
}
static void GetCAT(TSheader *header, unsigned char *buff)
{

	unsigned char *start = NULL;
	if (header->Start_indicator == 1)
	{
		start = buff + 5 + buff[4];
	}
	else
		start = buff + 4;
	CATThis->CATHeader = ChangeCAT2BitStream(start, sizeof(CATheader)-4, 0);
	//int temp = CATThis->CATHeader.section_length - 9;
	if ((int)(CATThis->CATHeader.section_length - 9) > 0)
	{
		CATThis->CA_descriptor = (unsigned char *)malloc(CATThis->CATHeader.section_length - 9);
		if (CATThis->CA_descriptor == NULL)
		{
			exit(1);
		}
	}
	start += sizeof(CATheader)-4 + CATThis->CATHeader.section_length - 9;
	ChangeCAT2BitStream(start, 4, 1);//CRC_32
	CATThis->CRC_32 = *(DWORD*)start;
}
CAT* NewCATclass()
{
	CATThis = (CAT *)malloc(sizeof(CAT));
	if (CATThis == NULL)
		exit(1);
	memset(CATThis, 0, sizeof(CAT));
	CATThis->GetCAT = GetCAT;
	CATThis->DeleteCATClass = DeleteCATClass;
	return CATThis;
}
static void DeleteCATClass()
{
	if (CATThis->CA_descriptor != NULL)
	{
		free(CATThis->CA_descriptor);
	}
	free(CATThis);
}







