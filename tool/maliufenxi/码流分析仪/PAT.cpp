#include "PAT.h"
#include "common.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "stdafx.h"

PAT * PATThis=NULL;
static void ChangePAT2BitStream(unsigned char * headerBuf, int num, int flag)
{
	unsigned char tempchar;

	for (int index = 0; index < num / 2; index++)
	{
		Swap(&headerBuf[index], &headerBuf[num - 1 - index], sizeof(unsigned char));
	}
		
	if (flag == 0)
	{
	
		tempchar = headerBuf[num - 1];//最后的那个直接
		Swap(&headerBuf[3], &headerBuf[4], 4*sizeof(unsigned char));//覆盖式后移
		headerBuf[3] = tempchar;
		return;
	}
	return;
}
static void GetPAT(TSheader *header, unsigned char *buff)
{
	unsigned char *start = NULL;
	int            ProgramSize = 0;//有节目信息的空间大小
	if (header->Start_indicator == 1)
	{

		start = buff + 5 + buff[4];
	}
	else
		start = buff + 4;
	ChangePAT2BitStream(start, sizeof(PATheader), 0);
	PATThis->PATHeader =*(PATheader *)start; //取得PAT的头部信息

	start += sizeof(PATheader);
	ProgramSize = PATThis->PATHeader.section_length - 4 - 5;
	ChangePAT2BitStream(start, ProgramSize, 1);

	PATThis->PATprogramP = (PATprogram*)malloc(ProgramSize);
	memcpy(PATThis->PATprogramP, start, ProgramSize);

	start += ProgramSize;
	ChangePAT2BitStream(start, 4, 1);//CRC_32
	PATThis->CRC_32 = *(DWORD*)start;
	PATThis->ProgramNum = ProgramSize / 4;
}

static void DeletePATClass()
{
	free(PATThis->PATprogramP);
	free(PATThis);
}
PAT * NewPATClass()
{
	PATThis = (PAT*)malloc(sizeof(PAT)); //PATClass 中给PATThis指针赋值
	memset(PATThis, 0, sizeof(PAT));
	PATThis->GetPAT = GetPAT;
	PATThis->DeletePATClass = DeletePATClass;
	return PATThis;
}

