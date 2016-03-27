#include <stdio.h>
#include<stdlib.h>
#include"common.h"
#include "tsfile.h"
#include "PAT.h"
#include "PMT.h"
#include "CAT.h"
#include "SDT.h"
int main(void)
{
	TSfileClass * PTSfile=NewTSFileClass();
	PAT* PATP = NewPATClass();
	CAT* PCAT = NewCATclass();
	PMT **PPMT= NULL;
	TSheader header;
	int PID, Position,Result,choose;
	unsigned char buff[204];
	PMTheader  PMTHeader;
	PATprogram *PATprogramP;
	SDT** PPSDT = NULL;

	printf("CATheader:%d\n", sizeof(CATheader));


	printf("输入码流文件路径(暂时处理包长渡为204)：");
	if (PTSfile->OpenTSFile()==err)
		printf("file open error !!!\n");

	printf("waiting……\n");
	PTSfile->DivTS();
	printf("文件统计完毕！\n\n");


	PTSfile->GetXXTSBag(0, 1, buff, 204, &header);
	PATP->GetPAT(&header, buff);
	PATprogramP = PATP->PATprogramP;


	PPMT = (PMT**)malloc(PATP->ProgramNum*sizeof(PMT*));
	for (int index = PATP->ProgramNum - 1; index >= 0; index--)
	{
		PTSfile->GetXXTSBag(PATprogramP[index].PMT_pid, 1, buff, 204, &header);   //收取PMT表
		PPMT[index] = NewPMTClass(&header, buff);

	}


	PTSfile->GetXXTSBag(1, 1, buff, 204, &header);
	PCAT->GetCAT(&header,buff);


	//PTSfile->GetXXTSBag(17, 6, buff, 204, &header);
	int totalPID = PTSfile->GetXXPIDNum(17);
	int accessSDTNum = 0;
	PPSDT =(SDT**)malloc(totalPID*sizeof(SDT*));
	for (int index = 0; index < totalPID; index++)//搜索所有可能存在的SDT表
	{
		PTSfile->GetXXTSBag(17, index+1, buff, 204, &header);
		PPSDT[index] = NewSDTClass(&header, buff);
		if (PPSDT[index] != NULL)
		{
			
			if (IsDifferenceSDT(PPSDT[index]))//重复了
			{
				PPSDT[index]->DeleteSDTClass(PPSDT[index]);
				PPSDT[index] = NULL;
			}
			else
			{
				accessSDTNum++;
			}
		}
	}




	//销毁
	for (int index = 0; index < totalPID; index++)
	{
		if (PPSDT[index] != NULL)
		{
			PPSDT[index]->DeleteSDTClass(PPSDT[index]);
		}
	}


	//PSDT = NewSDTClass(&header,buff);
	while (1)
	{
		printf("功能选择：\n");
		printf("1.查看PID索引\n");
		printf("2.查看PAT数据表\n");
		printf("3.查看PMT数据表\n");
		printf("4.退出\n");
		scanf("%d", &choose);
		if (choose == 4)
			break;
		if (choose == 1)
		{
			printf("包的统计信息：\n");
			PTSfile->printTSinfo();
			while (1)
			{
				printf("输入PID号和包号 如（8191 33）,退出输入(-1 -1)，不要括号：");
				scanf("%d%d", &PID, &Position);
				if (PID == -1)//退出
					break;
				Result = PTSfile->GetXXTSBag(PID, Position, buff, 204, &header);
				if (Result == -1)
				{
					printf("输入有误\n");
					continue;
				}

				for (int index = 0; index < 204; index++)
				{
					printf("%4x", buff[index]);
				}
				printf("\n");
				printf(" 同步字节:%x\n 传输错误指示:%d\n 起始指示:%d\n 传输优先级:%d\n PID:%d\n", header.Sync_byte, header.Transport_error_indicator, header.Start_indicator, header.Transport_pri, header.PID);
				printf(" 加扰控制:%d\n 自适应区控制:%d\n 连续记数器:%d\n\n\n", header.Condition_control, header.Diy_zone, header.Counter);
				printf("\n");
			}
		}
		if (choose == 2)
		{
			
			
			printf("PAT表信息：\n");
			printf(" table_id:%d\n section_syntax_indicator:%d\n reserved_1:%d\n", PATP->PATHeader.table_id, PATP->PATHeader.section_syntax_indicator, PATP->PATHeader.reserved_1);
			printf(" section_length:%d\n transport_stream_id:%d\n reserved_2:%d\n", PATP->PATHeader.section_length, PATP->PATHeader.transport_stream_id, PATP->PATHeader.reserved_2);
			printf(" version_number:%d\n current_next_indicator:%d\n section_number:%d\n last_section_number:%d\n", PATP->PATHeader.version_number, PATP->PATHeader.current_next_indicator, PATP->PATHeader.section_number, PATP->PATHeader.last_section_number);
			printf("\n\n节目信息：\n");
			for (int index = PATP->ProgramNum-1; index >= 0; index--)
			{
				printf("节目（program_number%d ,PMT_pid0x%.4x）\n", PATprogramP[index].program_number, PATprogramP[index].PMT_pid);			
			}
			printf("\n");
			getchar();
		}
		if (choose == 3)
		{
			while (1)
			{
				printf("\n\n\n\n\n有%d个PMT表，请问要查看第几个表(输入 0 返回上级菜单):\n", PATP->ProgramNum);
				scanf("%d", &choose);
				if (choose == 0)
					break;
				PMTprintf(PPMT, choose, PATP->ProgramNum);
			}

		}
	}



	PCAT->DeleteCATClass();
	for (int index = PATP->ProgramNum - 1; index >= 0; index--)
	{
		  //销毁PMT内存
		PPMT[index]->DeletePMTClass(PPMT[index]);
	}
	PATP->DeletePATClass();
	PTSfile->CloseTsFile();
	PTSfile->DeleteTSFileClass();

	return 0;
}





