#include "common.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "SDT.h"
#include "stdafx.h"
SDT *SDTThis = NULL;
unsigned char tempbuff[12] = {0};
static void ChangeSDT2BitStream(unsigned char * headerBuf, int num, int flag)
{
	//unsigned char tempchar;
	for (int index = 0; index < num / 2; index++)
	{
		Swap(&headerBuf[index], &headerBuf[num - 1 - index], sizeof(unsigned char));
	}
	if (flag == 0)
	{//SDTheader 调序
		memcpy(tempbuff, headerBuf, 8);
		memcpy(tempbuff + 9, headerBuf + 8, 3);//跳过一个字节
	}
	if (flag == 2)
	{//SDTSeviveheader调序
		memcpy(tempbuff+2, headerBuf, 2);
		memcpy(tempbuff+5, headerBuf + 2, 3);//跳过一个字节
	}
}

static int GetSDT(TSheader *header, unsigned char *buff)
{
	unsigned char *start = NULL;
	int           SDTSeviveByte = 0;
	int            SteamSize = 0;//有节目信息的空间大小
	//ServiceDesciptor*  PServiceDesciptor=NULL;
	SDTSeicer   *PSDTSeicer=NULL;
	if (header->Start_indicator == 1)
	{
		start = buff + 5 + buff[4];
	}
	else
		start = buff + 4;
	ChangeSDT2BitStream(start, sizeof(SDTheader)-1, 0);
	SDTThis->SDTHeader = *(SDTheader *)tempbuff; //取得SDT的头部信息
	start += sizeof(SDTheader)-1;// 指针偏移到SDTSevive字节
	SDTSeviveByte = SDTThis->SDTHeader.section_length - 4 - 8;
    
	if (SDTThis->SDTHeader.table_id != 0x42 && SDTThis->SDTHeader.table_id != 0x46)
	{//不是SDT表    0x42 为实际流SDT表，0x46为其他TS流SDT表
		return 1;//SDT表搜索失败
	}

	for (int index = 0; index < SDTSeviveByte;)
	{
		SDTThis->SDTSeiceNum++;
		if (index == 0)
		{
			SDTThis->PSDTSeicer = PSDTSeicer = GetSDTSeviveNode(start);
		}
		else
		{
			PSDTSeicer->next = GetSDTSeviveNode(start);
			PSDTSeicer = PSDTSeicer->next;
		}
		start += sizeof(SDTSeviveheader)-3 + PSDTSeicer->SDTSeviveHeader.descriptors_loop_length;// 指针偏移到下一个服务信息
		index += sizeof(SDTSeviveheader)-3 + PSDTSeicer->SDTSeviveHeader.descriptors_loop_length;
	}
	ChangeSDT2BitStream(start, 4, 1);//CRC_32
	SDTThis->CRC_32 = *(DWORD*)start;
	return 0;
}

static ServiceDesciptor* GetServiceDesciptorNode(unsigned char *start)// 生成描述子节点
{
	ServiceDesciptor *PServiceDesciptor;

	PServiceDesciptor = (ServiceDesciptor*)malloc(sizeof(ServiceDesciptor));
	if (PServiceDesciptor == NULL)
		exit(1);
	memset(PServiceDesciptor, 0, sizeof(ServiceDesciptor));
	PServiceDesciptor->desciptor_tag = *start++;
	PServiceDesciptor->desciptor_lenght = *start++;
	PServiceDesciptor->service_type = *start++;
	PServiceDesciptor->service_provider_name_lenght = *start++;
	if (PServiceDesciptor->service_provider_name_lenght != 0)
	{
		PServiceDesciptor->service_provider_name = (char *)malloc(PServiceDesciptor->service_provider_name_lenght + 1);
		if (PServiceDesciptor->service_provider_name == NULL)
			exit(1);

		memset(PServiceDesciptor->service_provider_name, 0, PServiceDesciptor->service_provider_name_lenght + 1);
		memcpy(PServiceDesciptor->service_provider_name, start, PServiceDesciptor->service_provider_name_lenght);
	}
	

	start += PServiceDesciptor->service_provider_name_lenght;//指针下移 获取服务名称

	PServiceDesciptor->service_name_lenght = *start++;
	if (PServiceDesciptor->service_name_lenght != 0)
	{
		PServiceDesciptor->service_name = (char *)malloc(PServiceDesciptor->service_name_lenght + 1);
		if (PServiceDesciptor->service_name == NULL)
			exit(1);
	}
	memset(PServiceDesciptor->service_name, 0, PServiceDesciptor->service_name_lenght + 1);
	memcpy(PServiceDesciptor->service_name, start, PServiceDesciptor->service_name_lenght);

	return  PServiceDesciptor ;
}

static SDTSeicer *GetSDTSeviveNode(unsigned char *start)//// 生成服务子节点
{
	ServiceDesciptor *PServiceDesciptor=NULL;
	SDTSeicer* PSDTSeicer = NULL;
	
		PSDTSeicer = (SDTSeicer*)malloc(sizeof(SDTSeicer));
		if (PSDTSeicer == NULL)
			exit(1);
		memset(PSDTSeicer, 0, sizeof(SDTSeicer));
		ChangeSDT2BitStream(start, sizeof(SDTSeviveheader)-3, 2);
		PSDTSeicer->SDTSeviveHeader = *(SDTSeviveheader *)tempbuff;
		start += sizeof(SDTSeviveheader)-3;// 指针偏移到ServiceDesciptor字节

		for (int index = 0; index < PSDTSeicer->SDTSeviveHeader.descriptors_loop_length;)//建立描述子链表
		{
			PSDTSeicer->DesciptorNum++;
			if (index == 0)
			{
				PSDTSeicer->PServiceDesciptor = PServiceDesciptor = GetServiceDesciptorNode(start);//得到描述子链表头指针
			}
			else
			{
				PServiceDesciptor->next = GetServiceDesciptorNode(start);//链接链表
			}
			start += PServiceDesciptor->desciptor_lenght + 2;
			index += PServiceDesciptor->desciptor_lenght + 2;
		}
		PSDTSeicer->next = NULL;
	return PSDTSeicer;

}

SDT * NewSDTClass(TSheader *header, unsigned char *buff)
{
	SDTThis = (SDT*)malloc(sizeof(SDT)); //SDTClass 中给SDTThis指针赋值
	if (SDTThis == NULL)
		exit(1);
	memset(SDTThis, 0, sizeof(SDT));

	SDTThis->DeleteSDTClass = DeleteSDTClass;
	if (GetSDT(header, buff))   //返回为1 时，搜索SDT失败
	{
		free(SDTThis);
		SDTThis = NULL;
		return NULL;
	}
	return SDTThis;
}
static void DeleteSDTClass(SDT* tempSDTThis)
{
	ServiceDesciptor *PServiceDesciptor = tempSDTThis->PSDTSeicer->PServiceDesciptor;
	ServiceDesciptor *PTempDesciptor = NULL;
	SDTSeicer *PSDTSeicer = tempSDTThis->PSDTSeicer;
	SDTSeicer *PTempSeicer = NULL;
	int index = 0;


	if (tempSDTThis == NULL)
		return;

	while (PSDTSeicer!=NULL)
	{//释放服务节点
		while (PServiceDesciptor != NULL)
		{//释放描述符节点
			free(PServiceDesciptor->service_provider_name);
			free(PServiceDesciptor->service_name);
			PTempDesciptor = PServiceDesciptor;
			PServiceDesciptor = PServiceDesciptor->next;
			free(PTempDesciptor);
			PTempDesciptor = NULL;
		}
		PTempSeicer = PSDTSeicer;
		PSDTSeicer = PSDTSeicer->next;
		free(PTempSeicer);
	}
	free(tempSDTThis);

}








int IsDifferenceSDT(SDT* tempPSDT)
{
	static int table_id[2] = { 0 };
	static int Stream[2][10] = { { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }, { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 } };//假设一个ID 最多10个流
	int index = 0, index1 = 0;

	for (index = 0; index < 2; index++)
	{
		if (tempPSDT->SDTHeader.table_id == table_id[index])  //表号已经存在
		{
			for (index1 = 0; index1 < 10; index1++)
			{
				if (tempPSDT->SDTHeader.transport_stream_id == Stream[index][index1])
				{//流已存在
					return 1;
				}
			}
			for (index1 = 0; index1 < 10; index1++)
			{
				if (Stream[index][index1] == -1)
				{
					Stream[index][index1] = tempPSDT->SDTHeader.transport_stream_id;//记录流号
					return 0;
				}
			}
		}
	}
	//将表号记下来
	if (table_id[0] == 0)
	{
		table_id[0] = tempPSDT->SDTHeader.table_id;
		Stream[0][0] = tempPSDT->SDTHeader.transport_stream_id;
	}
	else
	{
		table_id[1] = tempPSDT->SDTHeader.table_id;
		Stream[1][0] = tempPSDT->SDTHeader.transport_stream_id;
	}
	return 0;


}


