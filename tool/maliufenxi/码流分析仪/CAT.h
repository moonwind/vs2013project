#include"Datatype.h"

#ifndef CAT_H
#define CAT_H

typedef struct
{
	DWORD    : 24;// 4�ֽڲ��� ��Ч����     9~11

	DWORD	last_section_number : 8;

	DWORD   section_number : 8;
	DWORD   current_next_indicator : 1;
	DWORD	version_number : 5;
	DWORD   reserved2 : 18;
	
	DWORD     :8;// 4�ֽ� ����  ���� 8λ   ��3�ֽ�

	DWORD	section_length : 12;
	DWORD   reserved1 : 2;
	DWORD   zero : 1;
	DWORD   section_synatx_indicator : 1;
	DWORD   table_id : 8;
	
}CATheader;
typedef struct
{
	CATheader  CATHeader;
	unsigned char *CA_descriptor;
    DWORD      CRC_32;
	void (*GetCAT)(TSheader *header, unsigned char *buff);
    void (*DeleteCATClass)();
}CAT;

void GetCAT(TSheader *header, unsigned char *buff);
 CATheader ChangeCAT2BitStream(unsigned char * headerBuf, int num, int flag);
 void DeleteCATClass();
 CAT* NewCATclass();
#endif