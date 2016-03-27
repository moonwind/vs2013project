#include"Datatype.h"

#ifndef SDT_H
#define SDT_H

typedef struct
{
	DWORD  reserved_future_use2 : 8;
	DWORD  original_network_id : 16;
	DWORD  last_section_number : 8;

	DWORD  section_number : 8;
	DWORD  current_next_indicator : 1;
	DWORD  version_number : 5;
	DWORD  reserved2 : 2;
	DWORD  transport_stream_id : 16;

DWORD: 8;// 4字节 补齐  补齐 8位   
	DWORD  section_length : 12;
	DWORD  reserved1 : 2;
	DWORD  reserved_future_use1 : 1;
	DWORD  section_syntax_indicator : 1;
	DWORD  table_id : 8;
}SDTheader;

typedef struct  //116
{
DWORD: 16;// 4字节 补齐   
	DWORD    descriptors_loop_length : 12;
	DWORD    free_CA_mode : 1;
	DWORD    running_status : 3;

DWORD: 8;// 4字节 补齐  补齐 8位   
	DWORD    EIT_present_following_flag : 1;
	DWORD    EIT_schedule_flag : 1;
	DWORD    reserved_future_use : 6;
	DWORD    service_id : 16;

}SDTSeviveheader;

typedef struct ServiceDesciptorStruct
{
	char desciptor_tag;
	unsigned char desciptor_lenght;
	char service_type;
	char service_provider_name_lenght;
	char *service_provider_name;
	unsigned char service_name_lenght;
	char *service_name;
	struct ServiceDesciptorStruct *next;
}ServiceDesciptor;

typedef struct SDTSeicerStruct //116
{
	SDTSeviveheader SDTSeviveHeader;
	ServiceDesciptor *PServiceDesciptor;//挂载描述子链表
	int    DesciptorNum;//描述子个数
	struct SDTSeicerStruct*next;
}SDTSeicer;

typedef struct SDTStruct
{
	SDTheader SDTHeader;
	SDTSeicer *PSDTSeicer;//挂载业务描述链表
	int    SDTSeiceNum;    //业务个数
	DWORD  CRC_32;

	void(*DeleteSDTClass)(struct SDTStruct* tempThis);
}SDT;
void ChangeSDT2BitStream(unsigned char * headerBuf, int num, int flag);
void DeleteSDTClass(SDT* tempThis);
SDTSeicer *GetSDTSeviveNode(unsigned char *start);
ServiceDesciptor* GetServiceDesciptorNode(unsigned char *start);
int GetSDT(TSheader *header, unsigned char *buff);
SDT * NewSDTClass(TSheader *header, unsigned char *buff);
int IsDifferenceSDT(SDT* tempPSDT);
#endif