#include "stdio.h"
//#include "WinDef.h"
#include "Stdlib.h"
#include "Windows.h"
int main()
{
	char flag=0;
	printf("��������ʾģʽ�� (1): 0��1\n");
		printf("                 (2): %C%C%C%\n\n����ѡ��",3,3,3);
		scanf("%C",&flag);
		if(!(flag=='1'||flag=='2'))
		{
			printf("\n������ˣ�BYE������");
			getchar();
			getchar();
			exit(0);
		}
		if(flag=='2')
		{
			system("COLOR 0C");
		}

			
		
			// TODO: Add your command handler code here
  //CFileDialog dlg(TRUE);///TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի���
  //MessageBox(FilePathName);


  
typedef struct tagBITMAPINFOHEADER{
DWORD biSize; //��ʾ���ṹ�Ĵ�С
LONG biWidth; //λͼ�Ŀ��
LONG biHeight; //λͼ�ĸ߶�
WORD biPlanes; //��ԶΪ1 ,����û���ù����� û���о� ��msdn����
//Specifies the number of planes for the target device. This value must be set to 1.
WORD biBitCount;//λͼ��λ�� ��Ϊ1 4 8 16 24 32 ����û��1 4 �����о�
DWORD biCompression; //����Ϊѹ�����ͣ�����ȴ���������ã��Ժ����
DWORD biSizeImage; //��ʾλͼ��������Ĵ�С���ֽ�Ϊ��λ
LONG biXPelsPerMeter;
LONG biYPelsPerMeter;
DWORD biClrUsed;
DWORD biClrImportant;
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;


typedef struct {
   unsigned char  blue;
   unsigned char  green;
   unsigned char  red;

}clor,*Pclor;
typedef struct{
	int x;
	int y;
}CPoint;

  WORD    bfType; 
  DWORD   bfSize;  
  
  WORD    bfReserved1; 
  WORD    bfReserved2; 
  DWORD   bfOffBits;

   BITMAPINFOHEADER bmpfile;
    
	FILE *pf;
	///Pclor pdata,ptemp;
	CPoint point;
	unsigned char *Pchar,*Pstart;

	unsigned char index,index1;
	
	
   
	if((pf=fopen("dandan.lxg","r"))==NULL)
	{
		printf("����ʧ�ܣ�����");
		getchar();
		getchar();
		exit(0);
	}
	//pfw=fopen("C:\\1231.txt","w");
	fread(&bfType,sizeof(WORD),1,pf);
	fread(&bfSize,sizeof(DWORD),1,pf);
	fread(&bfReserved1,sizeof(WORD),1,pf);
	fread(&bfReserved2,sizeof(WORD),1,pf);
	fread(&bfOffBits,sizeof(DWORD),1,pf);
    fread(&bmpfile,sizeof(BITMAPINFOHEADER),1,pf);
    
	if(3*bmpfile.biWidth%4)
	{
    Pstart=Pchar=(unsigned char *)malloc((3*bmpfile.biWidth+4-3*bmpfile.biWidth%4)*bmpfile.biHeight);  // ���ص� 
	while(fread(Pchar,sizeof(unsigned char),(3*bmpfile.biWidth+4-3*bmpfile.biWidth%4)*bmpfile.biHeight,pf));//bmpfile.biSizeImage
	}
	else
	{
	Pstart=Pchar=(unsigned char *)malloc((3*bmpfile.biWidth)*bmpfile.biHeight);  // ���ص� 
	
    while(fread(Pchar,sizeof(unsigned char),(3*bmpfile.biWidth)*bmpfile.biHeight,pf));//bmpfile.biSizeImage
	}
	
   // Pchar=Pstart;
         Pchar+=(3*bmpfile.biWidth)*bmpfile.biHeight-1;
	point.x =0;
	point.y =bmpfile.biHeight-1;//bmpfile.biHeight
    
	for(;point.y>=0;point.y--)
	{
         // Pchar -=80*3;
		for(point.x =0;point.x < bmpfile.biWidth;point.x++)//bmpfile.biWidth
		{
			//dc.SetPixel(point,RGB(*(Pchar+2),*(Pchar+1),*(Pchar)));
			//dc.SetPixel(point,RGB(255,0,0));
			
			
			int a =*(Pchar-2)+*(Pchar-1)+*(Pchar);
			if(flag=='1')
				if(a>600)
					printf("0");
				else printf("1");
			else if(flag=='2')
				if(a>600)
					printf(" ");
				else printf("%c",3);

			Pchar -=3;
			//dc.SetPixel(point,RGB(test1[a].red  ,test1[a].green,test1[a].blue));
			//a++;
			Sleep(2);
		}   
		if(3*bmpfile.biWidth%4)
		for(index=0;index<4-3*bmpfile.biWidth%4;index++)
		{
//			fprintf(pfw,"\n");
            Pchar++;
         //    fprintf(pfw,"%x ",*(Pchar));
			// fprintf(pfw,"\n");
		}
		//printf("\n");
	}
   free(Pstart);
   fclose(pf);
	getchar();
	getchar();
getchar();
	getchar();
	exit(0);
	
		
		return 0;
}