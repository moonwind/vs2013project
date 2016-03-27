#include "common.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"PMT.h"
#include "stdafx.h"
PMT * PMTThis = NULL;

 static void ChangePMT2BitStream(unsigned char * headerBuf, int num, int flag)
{
	unsigned char tempchar;

	for (int index = 0; index < num / 2; index++)
	{
		Swap(&headerBuf[index], &headerBuf[num - 1 - index], sizeof(unsigned char));
	}

	if (flag == 0)
	{

		tempchar = headerBuf[num - 1];//最后的那个直接
		Swap(&headerBuf[7], &headerBuf[8], 4 * sizeof(unsigned char));//覆盖式后移
		headerBuf[7] = tempchar;
		return;
	}
	return;
}

 static void GetPMT(TSheader *header, unsigned char *buff)
 {
	 unsigned char *start = NULL;
	 PMTStream  *p=NULL,*q=NULL;
	 int            SteamSize = 0;//有节目信息的空间大小
	 if (header->Start_indicator == 1)
	 {
		 start = buff + 5 + buff[4];
	 }
	 else
		 start = buff + 4;

	 ChangePMT2BitStream(start, sizeof(PMTheader), 0);
	 PMTThis->PMTHeader = *(PMTheader *)start; //取得PMT的头部信息
	 start += sizeof(PMTheader);// 指针偏移到descriptor1字节
	 if (PMTThis->PMTHeader.program_info_length > 0)
	 {
		 PMTThis->descriptor1 = (unsigned char  *)malloc(PMTThis->PMTHeader.program_info_length); //为描述符开辟空间
		 memcpy(PMTThis->descriptor1, start, PMTThis->PMTHeader.program_info_length);//拷贝描述符数据
	 }
	 start += PMTThis->PMTHeader.program_info_length; //指针偏移到stream字节
	 SteamSize = PMTThis->PMTHeader.section_length - 9 - 4 - PMTThis->PMTHeader.program_info_length;
	 if (SteamSize > 0)
	 {
		 p = PMTThis->PMTStreamP = (PMTStream  *)malloc(sizeof(PMTStream));
		 memset(p, 0, sizeof(PMTStream));
		 for (int index = 0; index < SteamSize;)
		 {
			 PMTThis->StreamNum++;
			 p->stream_type = *start++;//取出第一个字节
			 ChangePMT2BitStream(start, sizeof(PMTStreamPart), 1);
			 p->PMTSPart = *(PMTStreamPart*)start;
			 start += sizeof(PMTStreamPart);//指针后移
			 if (0 < p->PMTSPart.ES_info_length)  //如果有描述符
			 {
				 p->descriptor2 = (unsigned char  *)malloc(p->PMTSPart.ES_info_length); //为描述符开辟空间
				 memcpy(p->descriptor2, start, p->PMTSPart.ES_info_length);//拷贝描述符数据
				 start += p->PMTSPart.ES_info_length;
			 }
			 q = p;
			 p->next = (PMTStream  *)malloc(sizeof(PMTStream));
			 p = p->next;
			 index += sizeof(PMTStreamPart)+PMTThis->PMTStreamP->PMTSPart.ES_info_length+1;
		 }
		 free(p);
		 q->next = NULL;
	 }

	 ChangePMT2BitStream(start, 4, 1);//CRC_32
	 PMTThis->CRC_32 = *(DWORD*)start;                     
	 return;
 }

 static void DeletePMTClass(PMT *tempPMTThis)  //************
 {
	 PMTStream  *p = NULL, *q = NULL;
	 if (tempPMTThis->descriptor1 != NULL)
	 {
		 free(tempPMTThis->descriptor1);
	 }
	 q = p = tempPMTThis->PMTStreamP;
	 for (int index = 0; index < tempPMTThis->StreamNum; index++)
	 {
		 q = p->next;
		 free(p);
		 p = q;
	 }
	 free(tempPMTThis);
 }
 PMT * NewPMTClass(TSheader *header, unsigned char *buff)
 {
	 PMTThis = (PMT*)malloc(sizeof(PMT)); //PATClass 中给PMTThis指针赋值
	 memset(PMTThis, 0, sizeof(PMT));
	// PMTThis->GetPMT = GetPMT;
	 PMTThis->DeletePMTClass = DeletePMTClass;

	 GetPMT(header, buff);
	 return PMTThis;
 }
 void PMTprintf(PMT **TempPPMT, int n, int max)
 {

	 if (n > max)
	 {
		 printf("输入有误！\n");
		 return;
	 }
		 

	 PMTStream *p = TempPPMT[n - 1]->PMTStreamP;
	 printf("PMT%d", n);
	 printf(" table_id:0x%x\n section_syntax_indicator:0x%x\n section_length:0x%x\n", TempPPMT[n - 1]->PMTHeader.table_id, TempPPMT[n - 1]->PMTHeader.section_syntax_indicator, TempPPMT[n - 1]->PMTHeader.section_length);
	 printf(" program_number:0x%x\n version_number:0x%x\n current_next_indicator:0x%x\n", TempPPMT[n - 1]->PMTHeader.program_number, TempPPMT[n - 1]->PMTHeader.version_number, TempPPMT[n - 1]->PMTHeader.current_next_indicator);
	 printf(" section_number:0x%x\n last_section_number:0x%x\n PCR_PID:0x%x\n program_info_length:0x%x\n\n", TempPPMT[n - 1]->PMTHeader.section_number, TempPPMT[n - 1]->PMTHeader.last_section_number, TempPPMT[n - 1]->PMTHeader.PCR_PID, TempPPMT[n - 1]->PMTHeader.program_info_length);


	 for (int index = 0; index < TempPPMT[n - 1]->StreamNum; index++)
	 {
		 printf("单元流%d：\n", index + 1);
		 printf("ES PID = 0x%x,stream_type = 0x%x,Es_Info_Len=0x%x\n", p->PMTSPart.elementary_PID, p->stream_type, p->PMTSPart.ES_info_length);
		 p = p->next;
	 }
 }
