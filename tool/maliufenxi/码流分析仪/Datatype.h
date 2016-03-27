
#ifndef DATATYPE_H
#define DATATYPE_H

#define DWORD unsigned long
#define erro   1
#define ok    0


typedef  struct{                       //TS��ͷ���ݽṹ  ��������Ӧ��	
	DWORD Counter : 4;
	DWORD Diy_zone : 2;
	DWORD Condition_control : 2;
	DWORD PID : 13;
	DWORD Transport_error_indicator : 1;
	DWORD Start_indicator : 1;
	DWORD Transport_pri : 1;
	DWORD Sync_byte : 8;
}TSheader;

typedef  struct{                      // //TS��ͷ���ݲ���
	TSheader header;
	unsigned char other[200];
}TSBag;


typedef  struct{                      // //TS��ͷ���ݲ���
	TSheader header;
	int(*GetSync_byte)();
	int(*GetPID)();
	int(*GetCounter)();
}TSheaderOpreator;


typedef  struct{

	TSheader header;  //��ͷ
	//long  Total;       //��PID��������  
	long OffSet;//������ļ�ͷ��ƫ����
	//int   PID;  //����pID���(�� header ��PID�ظ�)
}PIDBag;         //ĳ���ض�PID��ȫ�������Ϣ


int GetSync_byte_fun();
int GetPID_fun();
int GetCounter_fun();
TSheaderOpreator NewheaderOpreator();


#endif