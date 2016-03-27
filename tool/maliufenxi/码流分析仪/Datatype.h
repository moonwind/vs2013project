
#ifndef DATATYPE_H
#define DATATYPE_H

#define DWORD unsigned long
#define erro   1
#define ok    0


typedef  struct{                       //TS包头数据结构  不含自适应区	
	DWORD Counter : 4;
	DWORD Diy_zone : 2;
	DWORD Condition_control : 2;
	DWORD PID : 13;
	DWORD Transport_error_indicator : 1;
	DWORD Start_indicator : 1;
	DWORD Transport_pri : 1;
	DWORD Sync_byte : 8;
}TSheader;

typedef  struct{                      // //TS包头数据操作
	TSheader header;
	unsigned char other[200];
}TSBag;


typedef  struct{                      // //TS包头数据操作
	TSheader header;
	int(*GetSync_byte)();
	int(*GetPID)();
	int(*GetCounter)();
}TSheaderOpreator;


typedef  struct{

	TSheader header;  //包头
	//long  Total;       //该PID包的总数  
	long OffSet;//包相对文件头的偏移量
	//int   PID;  //包的pID编号(与 header 中PID重复)
}PIDBag;         //某个特定PID包全部相关信息


int GetSync_byte_fun();
int GetPID_fun();
int GetCounter_fun();
TSheaderOpreator NewheaderOpreator();


#endif