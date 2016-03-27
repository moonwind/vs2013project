#include"Datatype.h"

#ifndef PMT_H
#define PMT_H

typedef struct 
{
                          

DWORD program_info_length : 12; //ǰ��λbitΪ00������ָ���������Խ�Ŀ��Ϣ��������byte����  
DWORD reserved_4 : 4; //Ԥ��Ϊ0x0F  
DWORD PCR_PID : 13; //ָ��TS����PIDֵ����TS������PCR��  //��PCRֵ��Ӧ���ɽ�Ŀ��ָ���Ķ�Ӧ��Ŀ��  
DWORD reserved_3 : 3; //0x07  


DWORD last_section_number : 8; //�̶�Ϊ0x00
DWORD section_number : 8; //�̶�Ϊ0x00  
DWORD current_next_indicator : 1; //����λ��1ʱ����ǰ���͵�Program map section���ã� 
DWORD version_number : 5; //ָ��TS����Program map section�İ汾��  
DWORD reserved_2 : 2; //0x03  

DWORD table_id : 8; //�̶�Ϊ0x02, ��ʾPMT��   /**********************��table_id�����8���ֽ���   ��������section_syntax_indicator����


DWORD program_number : 16;// ָ���ý�Ŀ��Ӧ�ڿ�Ӧ�õ�Program map PID  
DWORD section_length : 12;//������λbit��Ϊ00����ָʾ�ε�byte�����ɶγ�����ʼ������CRC�� 
DWORD reserved_1 : 2; //0x03  
DWORD zero : 1; //0x01 
DWORD section_syntax_indicator : 1; //�̶�Ϊ0x01  

} PMTheader;


typedef struct{
	DWORD ES_info_length : 12;
	DWORD reserved_1 : 4;
	DWORD elementary_PID : 13;
	DWORD reserved_2 : 3;
}PMTStreamPart;

typedef struct PMTStreamStruct
{
	PMTStreamPart    PMTSPart;
	DWORD stream_type : 8;
	unsigned char *descriptor2;
	struct PMTStreamStruct *next;
}PMTStream;

typedef struct pmt
{
	PMTheader     PMTHeader;
	unsigned char *descriptor1;
	PMTStream     *PMTStreamP;
	int            StreamNum;
	DWORD         CRC_32;
	//void(*GetPMT)(TSheader *header, unsigned char *buff);
	void(*DeletePMTClass)(struct pmt *tempThis);

}PMT;

void ChangePMT2BitStream(unsigned char * headerBuf, int num, int flag);
void GetPMT(TSheader *header, unsigned char *buff);
void DeletePMTClass(PMT *tempThis);
PMT * NewPMTClass(TSheader *header, unsigned char *buff);
void PMTprintf(PMT **TempPPMT, int n, int max);
#endif