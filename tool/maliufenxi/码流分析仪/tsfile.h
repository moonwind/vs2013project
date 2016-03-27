#include "Datatype.h"

#ifndef TSFILE_H
#define TSFILE_H




#define  BYTESHIFT  204  
typedef  struct{
	unsigned long Maxlenght;
	unsigned long HaveReadLenght;
	unsigned long HaveDealLenght;
	unsigned long RemindLenght;
	unsigned char *QiuckP;
	char LastSection;
}QuickV;


typedef  struct{                      // ts文件结构体
	char  FileName[50];
	char  FilePath[256];
	long  FileLenght;
	long   TSBag;
}FileInfo;


typedef  struct{
	unsigned int PID;
	long  Total;       //该PID包的总数 
	long  MemSize;     // 指向空间的大小   
	PIDBag * PPIDBag;//指向PID包组的首地址   
}PIDKind;

typedef struct{
	PIDKind PIDArr[256];
	FileInfo mFile;
	QuickV  mQucikV;
	int    NewPIDCount;
	int    (*OpenTSFile)();
	int    (*DivTS)();//提取包的信息
	void   (*CloseTsFile)();
	void   (*printTSinfo)();
	int    (*GetXXTSBag)(int PID, int Position, unsigned char *buff, int num, TSheader *header);
	void   (*DeleteTSFileClass)();
	int   (*GetXXPIDNum)(int PID);
	unsigned long (*GetCurrentOffSet)();
	void (*GetFilepath)(char* FilePath, char *FileName);
}TSfileClass;

void GetFilepath(char* FilePath, char* FileName);
int  OpenTSFile();
int DivTS();
int IsNewPID(unsigned int tempPID);
void CloseTsFile();
int  comp(const void * a, const void *b);
void printTSinfo();
int GetXXTSBag(int PID, int Position, unsigned char *buff, int num, TSheader *header);
int GetXXPIDNum(int PID);
TSfileClass *  NewTSFileClass();
void DeleteTSFileClass();
unsigned long GetCurrentOffSet();
static int CheckTsBagLenght();
static unsigned long  DealData(unsigned long NeedBeDeal);
static void BiuldPID(unsigned long HaveDeal);
#endif // !TSFILE_H
