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


	printf("���������ļ�·��(��ʱ���������Ϊ204)��");
	if (PTSfile->OpenTSFile()==err)
		printf("file open error !!!\n");

	printf("waiting����\n");
	PTSfile->DivTS();
	printf("�ļ�ͳ����ϣ�\n\n");


	PTSfile->GetXXTSBag(0, 1, buff, 204, &header);
	PATP->GetPAT(&header, buff);
	PATprogramP = PATP->PATprogramP;


	PPMT = (PMT**)malloc(PATP->ProgramNum*sizeof(PMT*));
	for (int index = PATP->ProgramNum - 1; index >= 0; index--)
	{
		PTSfile->GetXXTSBag(PATprogramP[index].PMT_pid, 1, buff, 204, &header);   //��ȡPMT��
		PPMT[index] = NewPMTClass(&header, buff);

	}


	PTSfile->GetXXTSBag(1, 1, buff, 204, &header);
	PCAT->GetCAT(&header,buff);


	//PTSfile->GetXXTSBag(17, 6, buff, 204, &header);
	int totalPID = PTSfile->GetXXPIDNum(17);
	int accessSDTNum = 0;
	PPSDT =(SDT**)malloc(totalPID*sizeof(SDT*));
	for (int index = 0; index < totalPID; index++)//�������п��ܴ��ڵ�SDT��
	{
		PTSfile->GetXXTSBag(17, index+1, buff, 204, &header);
		PPSDT[index] = NewSDTClass(&header, buff);
		if (PPSDT[index] != NULL)
		{
			
			if (IsDifferenceSDT(PPSDT[index]))//�ظ���
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




	//����
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
		printf("����ѡ��\n");
		printf("1.�鿴PID����\n");
		printf("2.�鿴PAT���ݱ�\n");
		printf("3.�鿴PMT���ݱ�\n");
		printf("4.�˳�\n");
		scanf("%d", &choose);
		if (choose == 4)
			break;
		if (choose == 1)
		{
			printf("����ͳ����Ϣ��\n");
			PTSfile->printTSinfo();
			while (1)
			{
				printf("����PID�źͰ��� �磨8191 33��,�˳�����(-1 -1)����Ҫ���ţ�");
				scanf("%d%d", &PID, &Position);
				if (PID == -1)//�˳�
					break;
				Result = PTSfile->GetXXTSBag(PID, Position, buff, 204, &header);
				if (Result == -1)
				{
					printf("��������\n");
					continue;
				}

				for (int index = 0; index < 204; index++)
				{
					printf("%4x", buff[index]);
				}
				printf("\n");
				printf(" ͬ���ֽ�:%x\n �������ָʾ:%d\n ��ʼָʾ:%d\n �������ȼ�:%d\n PID:%d\n", header.Sync_byte, header.Transport_error_indicator, header.Start_indicator, header.Transport_pri, header.PID);
				printf(" ���ſ���:%d\n ����Ӧ������:%d\n ����������:%d\n\n\n", header.Condition_control, header.Diy_zone, header.Counter);
				printf("\n");
			}
		}
		if (choose == 2)
		{
			
			
			printf("PAT����Ϣ��\n");
			printf(" table_id:%d\n section_syntax_indicator:%d\n reserved_1:%d\n", PATP->PATHeader.table_id, PATP->PATHeader.section_syntax_indicator, PATP->PATHeader.reserved_1);
			printf(" section_length:%d\n transport_stream_id:%d\n reserved_2:%d\n", PATP->PATHeader.section_length, PATP->PATHeader.transport_stream_id, PATP->PATHeader.reserved_2);
			printf(" version_number:%d\n current_next_indicator:%d\n section_number:%d\n last_section_number:%d\n", PATP->PATHeader.version_number, PATP->PATHeader.current_next_indicator, PATP->PATHeader.section_number, PATP->PATHeader.last_section_number);
			printf("\n\n��Ŀ��Ϣ��\n");
			for (int index = PATP->ProgramNum-1; index >= 0; index--)
			{
				printf("��Ŀ��program_number%d ,PMT_pid0x%.4x��\n", PATprogramP[index].program_number, PATprogramP[index].PMT_pid);			
			}
			printf("\n");
			getchar();
		}
		if (choose == 3)
		{
			while (1)
			{
				printf("\n\n\n\n\n��%d��PMT������Ҫ�鿴�ڼ�����(���� 0 �����ϼ��˵�):\n", PATP->ProgramNum);
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
		  //����PMT�ڴ�
		PPMT[index]->DeletePMTClass(PPMT[index]);
	}
	PATP->DeletePATClass();
	PTSfile->CloseTsFile();
	PTSfile->DeleteTSFileClass();

	return 0;
}





