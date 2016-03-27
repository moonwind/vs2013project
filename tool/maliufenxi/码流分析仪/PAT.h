#include "Datatype.h"
#ifndef PAT_H
#define PAT_H
//#PACK(2)

typedef struct 
{
 
 DWORD last_section_number : 8;  //���һ���ֶεĺ���
 DWORD section_number : 8; //�ֶεĺ��롣PAT���ܷ�Ϊ��δ��䣬��һ��Ϊ00���Ժ�ÿ���ֶμ�1����������256���ֶ�  
 DWORD current_next_indicator : 1; //���͵�PAT�ǵ�ǰ��Ч������һ��PAT��Ч  
 DWORD version_number : 5; //��Χ0-31����ʾPAT�İ汾��  
 DWORD reserved_2 : 2;// ����λ   3BYTE


 DWORD  table_id : 8; //�̶�Ϊ0x00 ����־�Ǹñ���PAT��  //////////////////   ע��  ���ֽڱ�����  section_syntax_indicator  ������� ��Ϊ����2�������� �õ� �м����� 

 DWORD transport_stream_id : 16; //�ô�������ID��������һ��������������·���õ���
 DWORD section_length : 12; //��ʾ����һ���ֶο�ʼ��CRC32(��)֮�����õ��ֽ���  
 DWORD reserved_1 : 2; // ����λ 
 DWORD zero : 1; //0  
 DWORD section_syntax_indicator : 1; //���﷨��־λ���̶�Ϊ1  4byte

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