#include"Datatype.h"

#ifndef PMT_H
#define PMT_H

typedef struct 
{
                          

DWORD program_info_length : 12; //前两位bit为00。该域指出跟随其后对节目信息的描述的byte数。  
DWORD reserved_4 : 4; //预留为0x0F  
DWORD PCR_PID : 13; //指明TS包的PID值，该TS包含有PCR域，  //该PCR值对应于由节目号指定的对应节目。  
DWORD reserved_3 : 3; //0x07  


DWORD last_section_number : 8; //固定为0x00
DWORD section_number : 8; //固定为0x00  
DWORD current_next_indicator : 1; //当该位置1时，当前传送的Program map section可用； 
DWORD version_number : 5; //指出TS流中Program map section的版本号  
DWORD reserved_2 : 2; //0x03  

DWORD table_id : 8; //固定为0x02, 表示PMT表   /**********************将table_id差入第8个字节流   正常的在section_syntax_indicator下面


DWORD program_number : 16;// 指出该节目对应于可应用的Program map PID  
DWORD section_length : 12;//首先两位bit置为00，它指示段的byte数，由段长度域开始，包含CRC。 
DWORD reserved_1 : 2; //0x03  
DWORD zero : 1; //0x01 
DWORD section_syntax_indicator : 1; //固定为0x01  

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