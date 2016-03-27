#include "Datatype.h"
#ifndef PAT_H
#define PAT_H
//#PACK(2)

typedef struct 
{
 
 DWORD last_section_number : 8;  //最后一个分段的号码
 DWORD section_number : 8; //分段的号码。PAT可能分为多段传输，第一段为00，以后每个分段加1，最多可能有256个分段  
 DWORD current_next_indicator : 1; //发送的PAT是当前有效还是下一个PAT有效  
 DWORD version_number : 5; //范围0-31，表示PAT的版本号  
 DWORD reserved_2 : 2;// 保留位   3BYTE


 DWORD  table_id : 8; //固定为0x00 ，标志是该表是PAT表  //////////////////   注意  该字节本来在  section_syntax_indicator  的下面的 ，为调节2进制流序 该到 中间来的 

 DWORD transport_stream_id : 16; //该传输流的ID，区别于一个网络中其它多路复用的流
 DWORD section_length : 12; //表示从下一个字段开始到CRC32(含)之间有用的字节数  
 DWORD reserved_1 : 2; // 保留位 
 DWORD zero : 1; //0  
 DWORD section_syntax_indicator : 1; //段语法标志位，固定为1  4byte

} PATheader;

typedef struct
{
  DWORD PMT_pid : 13;
  DWORD reserved : 3;
  DWORD program_number : 16;
}PATprogram;

typedef struct
{
	PATheader   PATHeader;
	PATprogram *PATprogramP;
	int        ProgramNum;
	DWORD       CRC_32;
	void      (*GetPAT)(TSheader *header, unsigned char *buff);
	void      (*DeletePATClass)();
}PAT;

void ChangePAT2BitStream(unsigned char * headerBuf, int num, int flag);
void GetPAT(TSheader *header, unsigned char *buff, PAT* tempPAT);
PAT * NewPATClass();
void DeletePATClass();
#endif