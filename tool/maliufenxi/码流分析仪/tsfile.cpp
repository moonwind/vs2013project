#include "stdafx.h"
#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TSfile.h"

//PIDKind TSfile->PIDArr[256];//PID总类索引///
#define  _30MB           (31457280) 

FILE *pf=NULL;
TSBag  mTSBag;
TSfileClass   *TSfile; 
int    TSbagLength = 0;
unsigned char *getbuf=NULL;

static void GetFilepath(char* FilePath,char *FileName)
{
    //scanf("%256[^\n]s",TSfile->mFile.FilePath); 
	strcpy_s(TSfile->mFile.FilePath, FilePath);
	strcpy_s(TSfile->mFile.FileName, FileName);
	
}
int  OpenTSFile()
{
  // GetFilepath();

    pf=fopen( TSfile->mFile.FilePath, "rb");
   if(pf==NULL)
   {
	   return erro;
   }
   return ok;
}
static int DivTS()//提取包的信息
{
	OpenTSFile();

	//int test = 0;
	long count=0;
	unsigned char tempchar;	
	unsigned long HaveFread = 0;
	//OpenTSFile();
	if(pf==NULL)
		return erro;
	TSbagLength = CheckTsBagLenght();


	TSfile->mQucikV.QiuckP=(unsigned char*)malloc(30*1024*1024);

	if (TSfile->mQucikV.QiuckP == NULL)
		exit(1);

	
	fseek(pf, 0, SEEK_END);//获取文件长度;

	TSfile->mFile.FileLenght = ftell(pf);
	TSfile->mQucikV.Maxlenght = TSfile->mFile.FileLenght;
	fseek(pf, 0, 0);
	do
	{
		tempchar = fgetc(pf);
		count++;
	} while (tempchar != EOF && tempchar != 0x47);
	fseek(pf, count - 1, 0);
	TSfile->mQucikV.HaveDealLenght = count - 1;
	//fseek(pf,0,0);
	while (1)
	{
		unsigned long HaveDeal = 0;
		HaveFread = fread(TSfile->mQucikV.QiuckP + TSfile->mQucikV.RemindLenght, 1, _30MB - TSfile->mQucikV.RemindLenght, pf);
		TSfile->mQucikV.HaveReadLenght += HaveFread;
		if (HaveFread < _30MB - TSfile->mQucikV.RemindLenght)
		{
			TSfile->mQucikV.LastSection = 1;
		}
		HaveDeal = DealData(HaveFread + TSfile->mQucikV.RemindLenght);
		TSfile->mQucikV.RemindLenght = HaveFread + TSfile->mQucikV.RemindLenght - HaveDeal;
		memcpy(TSfile->mQucikV.QiuckP, TSfile->mQucikV.QiuckP + HaveDeal, TSfile->mQucikV.RemindLenght);//把尾部数据 考到开头去
		TSfile->mQucikV.HaveDealLenght += HaveDeal;
		if (TSfile->mQucikV.LastSection == 1)
			break;
	}
	free(TSfile->mQucikV.QiuckP);
	TSfile->mQucikV.QiuckP = NULL;
	//TSfile->mFile.TSBag = count;
	qsort(TSfile->PIDArr, TSfile->NewPIDCount, sizeof(PIDKind), comp);
	return ok;
}
static int  comp(const void * a,const void *b)
{
	return ((PIDKind*)a)->PID - ((PIDKind*)b)->PID;
}

static void CloseTsFile()
{
	if (pf!=NULL)
     	fclose(pf);
}
static int IsNewPID(unsigned int tempPID)
{
     int index=0;
	 for(index=0; index < TSfile->NewPIDCount ;index++)
	 {
         if(TSfile->PIDArr[index].PID == tempPID)
		 {
				return index;
		 }
	 }
	 return -1;
}
static void printTSinfo()
{
	for (int index = 0; index < TSfile->NewPIDCount; index++)
	{
		printf("PID: 0x%.5x(%5d) 包的个数：%5d \n", TSfile->PIDArr[index].PID, TSfile->PIDArr[index].PID, TSfile->PIDArr[index].Total);
	}
}
static int GetXXPIDNum(int PID)
{//得到某个PID包的数量
	int Result = IsNewPID(PID);
	if (Result == -1)
		return 0;
	return TSfile->PIDArr[Result].Total;

}

static int GetXXTSBag(int PID,int Position,unsigned char *buff,int num,TSheader *header)
{
	int Result = IsNewPID(PID);
	PIDBag*  PPIDBag = NULL;
	if (Result == -1)
		return Result;
	if (Position > TSfile->PIDArr[Result].Total || Position<0)
		return -1;

	PPIDBag = TSfile->PIDArr[Result].PPIDBag + Position-1;
	*header = PPIDBag->header;
	fseek(pf, PPIDBag->OffSet, 0);
	fread(buff, 1, num, pf);
	return Result;
}
TSfileClass *  NewTSFileClass()
{

	TSfile = (TSfileClass *)malloc(sizeof(TSfileClass));
	memset(TSfile, 0, sizeof(TSfileClass));
	TSfile->OpenTSFile = OpenTSFile;
	TSfile->DivTS      = DivTS;
	TSfile->CloseTsFile = CloseTsFile;
	TSfile->printTSinfo = printTSinfo;
	TSfile->GetXXTSBag = GetXXTSBag;
	TSfile->DeleteTSFileClass = DeleteTSFileClass;
	TSfile->GetXXPIDNum = GetXXPIDNum;
	TSfile->GetCurrentOffSet = GetCurrentOffSet;
	TSfile->GetFilepath = GetFilepath;
	return TSfile;
}
static void DeleteTSFileClass()
{
	for (int index = 0; index < TSfile->NewPIDCount; index++)
	{
		free(TSfile->PIDArr[index].PPIDBag);
	}
	free(TSfile);
	TSfile = NULL;
}
static unsigned long GetCurrentOffSet()
{
	return ftell(pf);
}

static int CheckTsBagLenght()
{
	unsigned char tempChar;
	while (1)
	{
		tempChar = fgetc(pf);
		if (tempChar == 0x47)
		{
			fseek(pf, 203, 1);
			tempChar = fgetc(pf);
			if (tempChar == 0x47)
			{
				fseek(pf, 0, 0);
				return 204;
			}
			else
			{
				fseek(pf, -204, 1);
				fseek(pf, 187, 1);
				tempChar = fgetc(pf);
				if (tempChar == 0x47)
				{
					fseek(pf, 0, 0);
					return 188;
				}
				else
				{
					fseek(pf, -188, 1);
					continue;
				}
			}
		}
	}
}

static void BiuldPID(unsigned long HaveDeal)
{
	int IsNewPID_Result;
	PIDBag* PPIDBag = NULL;
	IsNewPID_Result = IsNewPID(mTSBag.header.PID); //*****
	if (IsNewPID_Result == -1)  // 为新的PID值 返回-1，若存在返回PID所在TSfile->PIDArr数组的位置
	{
		TSfile->PIDArr[TSfile->NewPIDCount].PPIDBag = (PIDBag*)malloc(100 * sizeof(PIDBag));
		TSfile->PIDArr[TSfile->NewPIDCount].PID = mTSBag.header.PID;
		TSfile->PIDArr[TSfile->NewPIDCount].MemSize = 100;
		IsNewPID_Result = TSfile->NewPIDCount++;
	}
	else
	{
		if (TSfile->PIDArr[IsNewPID_Result].MemSize <= TSfile->PIDArr[IsNewPID_Result].Total)
		{
			//重新分配内存空间
			PIDBag*  tempp = NULL;
			tempp = (PIDBag*)realloc(TSfile->PIDArr[IsNewPID_Result].PPIDBag, (TSfile->PIDArr[IsNewPID_Result].MemSize + 100)*sizeof(PIDBag));
			//realloc_mem(TSfile->PIDArr[IsNewPID_Result].PPIDBag,)
			if (tempp == NULL)
			{
				printf("mem error!!\n");
				getchar();
			}
			TSfile->PIDArr[IsNewPID_Result].PPIDBag = tempp;
			TSfile->PIDArr[IsNewPID_Result].MemSize = TSfile->PIDArr[IsNewPID_Result].MemSize + 100;
		}
	}
	PPIDBag = (PIDBag*)(TSfile->PIDArr[IsNewPID_Result].PPIDBag) + TSfile->PIDArr[IsNewPID_Result].Total;//指向PID相同的包的信息空间
	PPIDBag->header = mTSBag.header;
	PPIDBag->OffSet = HaveDeal+TSfile->mQucikV.HaveDealLenght - TSbagLength;
	TSfile->PIDArr[IsNewPID_Result].Total++;
}

static unsigned long  DealData(unsigned long NeedBeDeal)
{
	unsigned char tempchar;
	unsigned long HaveDealLenght = 0;
	while (HaveDealLenght < NeedBeDeal - TSbagLength)
	{
		//int index = 0;
		DWORD temp = 0;
		unsigned char *p;
		PIDBag*  PPIDBag = NULL;
		int      IsNewPID_Result = -1;
		memcpy(&mTSBag, TSfile->mQucikV.QiuckP + HaveDealLenght, TSbagLength);
		HaveDealLenght += TSbagLength;

		p = (unsigned char *)(&mTSBag);
		for (int i = 0; i<4; i++)
		{
			temp = temp << 8;                                 //改变字节序列
			temp |= *p++;
		}
		mTSBag.header = *(TSheader *)(&temp);

		tempchar = TSfile->mQucikV.QiuckP[HaveDealLenght++];

		if (mTSBag.header.Sync_byte != 0x47 || tempchar != 0x47)//包的过滤   丢掉坏包
		{
			HaveDealLenght -= TSbagLength;
			do
			{
				tempchar = TSfile->mQucikV.QiuckP[HaveDealLenght++];

			} while (tempchar != 0x47);
			HaveDealLenght--;
			continue;
		}
		HaveDealLenght--;
		//count++;
		BiuldPID(HaveDealLenght);
	}
	return HaveDealLenght;
}