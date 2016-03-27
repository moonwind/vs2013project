
// CTsDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CTs.h"
#include "CTsDlg.h"
#include "afxdialogex.h"
#include"ffplaycore.h"
#include"startTV.h"
#include "mediaMode.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���


TSfileClass * PTSfile = NewTSFileClass();
PAT* PATP = NewPATClass();
CAT* PCAT = NewCATclass();
PMT **PPMT = NULL;
TSheader header;
int PID, Position, Result, choose;
unsigned char buff[204];
PMTheader  PMTHeader;
PATprogram *PATprogramP;
SDT** PPSDT = NULL;
int totalPID=0;
int accessSDTNum=0;
int isReadOver=0;

SendAgrStruct  TSSend;

DWORD WINAPI GetData(LPVOID lpParamter);

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCTsDlg �Ի���



CCTsDlg::CCTsDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CCTsDlg::IDD, pParent)
, mPIDEdit(0)
, mTotalBagEdit(0)
, mNbag(1)
, mDataBuff(_T(""))
, programbn(0)
, isTS(0)
, totalProgramNumber(1)
, programCount(0)
{
	//accessSDTNum = 0;
	TSSend.dlgCCTs = this;
	TSSend.dlgMedia = NULL;
	TSSend.dlgtype = ts;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}

void CCTsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, mPSI_SITree);
	DDX_Control(pDX, IDC_TREE2, mPIDtree);
	DDX_Text(pDX, IDC_EDIT2, mPIDEdit);
	DDX_Text(pDX, IDC_EDIT3, mTotalBagEdit);
	DDX_Text(pDX, IDC_EDIT4, mNbag);
	DDX_Text(pDX, IDC_EDIT1, mDataBuff);
	DDX_Control(pDX, IDC_TREE3, mTSheader);
	DDX_Control(pDX, IDOpenFile, mOpenFile);
	DDX_Control(pDX, IDC_PROGRESS1, mprogress);
}

BEGIN_MESSAGE_MAP(CCTsDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOpenFile, &CCTsDlg::OnBnClickedOpenfile)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE2, &CCTsDlg::OnTvnSelchangedTree2)
	ON_BN_CLICKED(IDC_GetXXBag, &CCTsDlg::OnBnClickedGetxxbag)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDCANCEL, &CCTsDlg::OnBnClickedCancel)
//	ON_STN_CLICKED(IDC_STA, &CCTsDlg::OnStnClickedSta)
ON_BN_CLICKED(IDC_BUTTON3, &CCTsDlg::OnBnClickedButton3)
ON_BN_CLICKED(IDC_BUTTON4, &CCTsDlg::OnBnClickedButton4)
ON_BN_CLICKED(IDC_BUTTON5, &CCTsDlg::OnBnClickedButton5)
ON_STN_CLICKED(IDC_pic1, &CCTsDlg::OnStnClickedpic1)
ON_BN_CLICKED(IDC_BUTTON6, &CCTsDlg::OnBnClickedButton6)
END_MESSAGE_MAP()


// CCTsDlg ��Ϣ�������

BOOL CCTsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	HICON hIcon[3];      // ͼ��������   
	//HTREEITEM hRoot;     // ���ĸ��ڵ�ľ��   
//	HTREEITEM hCataItem; // �ɱ�ʾ��һ����ڵ�ľ��   
//	HTREEITEM hArtItem;  // �ɱ�ʾ��һ���½ڵ�ľ��   

	// ��������ͼ�꣬�������ǵľ�����浽����   
	hIcon[0] = theApp.LoadIcon(IDI_ICON1);
	hIcon[1] = theApp.LoadIcon(IDI_ICON2);
	hIcon[2] = theApp.LoadIcon(IDI_ICON3);

	// ����ͼ������CImageList����   
	mImage.Create(16, 16, ILC_COLOR32, 3, 3);
	// ������ͼ����ӵ�ͼ������   
	for (int i = 0; i < 3; i++)
	{
		mImage.Add(hIcon[i]);
	}


	mPSI_SITree.SetImageList(&mImage, TVSIL_NORMAL);
	this->mPIDtree.SetImageList(&mImage, TVSIL_NORMAL);
	mTSheader.SetImageList(&mImage, TVSIL_NORMAL);
	this->mprogress.SetRange32(0, 100);
	mprogress.SetPos(0);;
	//SetTree();

	//startTVinit(this);
	HBITMAP hBitmap;
	//����ͼƬID 
	hBitmap = (HBITMAP)LoadImage(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP1),IMAGE_BITMAP, 0, 0,LR_LOADMAP3DCOLORS);
	//���ָ��̬�ؼ���ָ�� 
	CStatic *pStatic = (CStatic *)GetDlgItem(IDC_pic);//�ؼ�ID 
	//���þ�̬�ؼ�����ʽ��ʹ�����ʹ��λͼ������λ����ʾʹ���� 
	pStatic->ModifyStyle(0xF, SS_BITMAP | SS_CENTERIMAGE);
	//���þ�̬�ؼ���ʾλͼ 
	pStatic->SetBitmap(hBitmap);

	this->GetDlgItem(IDC_BUTTON3)->EnableWindow(0);
	this->GetDlgItem(IDC_BUTTON4)->EnableWindow(0);
	this->GetDlgItem(IDC_BUTTON5)->EnableWindow(0);
	this->GetDlgItem(IDC_BUTTON6)->ShowWindow(0);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CCTsDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CCTsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CCTsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}







DWORD WINAPI GetData(LPVOID lpParamter)
{
	PTSfile->DivTS();
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
	PCAT->GetCAT(&header, buff);

	//PTSfile->GetXXTSBag(17, 6, buff, 204, &header);
	totalPID = PTSfile->GetXXPIDNum(17);//***************************
	PPSDT = (SDT**)malloc(totalPID*sizeof(SDT*));
	for (int index = 0; index < totalPID; index++)//�������п��ܴ��ڵ�SDT��
	{
		PTSfile->GetXXTSBag(17, index + 1, buff, 204, &header);
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
	isReadOver=1;
	return 0;
}


void CCTsDlg::SetTree()
{

	//GetData();
	SetCATTree();
	SetPATTree();
	SetPMTTree();
    SetSDTTree();
	SetPIDTree();
	
}
UINT Thread_Play(LPVOID lpParam){
	//CFFMPE_MFC_SDL_testDlg *dlg = (CFFMPE_MFC_SDL_testDlg *)lpParam;
	ffmfc_play(lpParam);
	return 0;
}
void CCTsDlg::OnBnClickedOpenfile()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString FilePathName,FileName;  
	CFileDialog dlg(TRUE, L"(*.TS)|*.TS", L"*.TS", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"�����ļ�(*.TS)");///TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի��� 
	if (dlg.DoModal() == IDOK)
	{
		FilePathName = dlg.GetPathName();
		FileName = dlg.GetFileName();
	}
	if (FilePathName == "")
	{
		return;
	}
	this->isTS = 0;
	//SDL_Quit();
   //Sleep(100);
	CStringA tempPath= CStringA(FilePathName.GetBuffer());
	CStringA tempName = CStringA(FileName.GetBuffer());

	this->GetDlgItem(IDC_BUTTON3)->EnableWindow(1);
	this->GetDlgItem(IDC_BUTTON4)->EnableWindow(1);
	this->GetDlgItem(IDC_BUTTON5)->EnableWindow(1);//�������ư�ť
	//this->mOpenFile.EnableWindow(0); IDCANCEL
	this->GetDlgItem(IDCANCEL)->EnableWindow(0);
	filepath = CStringA(tempPath);
	this->mOpenFile.EnableWindow(0);
	PTSfile->GetFilepath(tempPath.GetBuffer(), tempName.GetBuffer());

	this->programbn = 0;
	AfxBeginThread(Thread_Play, &TSSend);//�����߳�

	PDWORD ThreadIDTCPRev;
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)GetData,NULL, 0,(LPDWORD)&ThreadIDTCPRev);

	this->SetTimer(1, 200, NULL);
	

}
void CCTsDlg::SetTreeContent(int nImage, int nSelectedImage, HTREEITEM hParent, SDT* PSDT)
{
	CString temp;
	HTREEITEM  hCataItem;
	temp.Empty();
	temp.Format(L"table_id = 0x%x", PSDT->SDTHeader.table_id);
	hCataItem = mPSI_SITree.InsertItem(temp, nImage, nSelectedImage, hParent, TVI_LAST);
	   
	mPSI_SITree.SetItemData(hCataItem, 1);

	temp.Empty();
	temp.Format(L"section_syntax_indicator = 0x%x", PSDT->SDTHeader.section_syntax_indicator);
	hCataItem = mPSI_SITree.InsertItem(temp, nImage, nSelectedImage, hParent, TVI_LAST);
	   
	mPSI_SITree.SetItemData(hCataItem, 1);


	temp.Empty();
	temp.Format(L"section_length = 0x%x", PSDT->SDTHeader.section_length);
	hCataItem = mPSI_SITree.InsertItem(temp, nImage, nSelectedImage, hParent, TVI_LAST);
	   
	mPSI_SITree.SetItemData(hCataItem, 1);

	temp.Empty();
	temp.Format(L"transport_stream_id = 0x%x", PSDT->SDTHeader.transport_stream_id);
	hCataItem = mPSI_SITree.InsertItem(temp, nImage, nSelectedImage, hParent, TVI_LAST);
	   
	mPSI_SITree.SetItemData(hCataItem, 1);

	temp.Empty();
	temp.Format(L"version_number = 0x%x", PSDT->SDTHeader.version_number);
	hCataItem = mPSI_SITree.InsertItem(temp, nImage, nSelectedImage, hParent, TVI_LAST);
	   
	mPSI_SITree.SetItemData(hCataItem, 1);

	temp.Empty();
	temp.Format(L"current_next_indicator = 0x%x", PSDT->SDTHeader.current_next_indicator);
	hCataItem = mPSI_SITree.InsertItem(temp, nImage, nSelectedImage, hParent, TVI_LAST);
	   
	mPSI_SITree.SetItemData(hCataItem, 1);

	temp.Empty();
	temp.Format(L"section_number = 0x%x", PSDT->SDTHeader.section_number);
	hCataItem = mPSI_SITree.InsertItem(temp, nImage, nSelectedImage, hParent, TVI_LAST);
	   
	mPSI_SITree.SetItemData(hCataItem, 1);

	temp.Empty();
	temp.Format(L"last_section_number = 0x%x", PSDT->SDTHeader.last_section_number);
	hCataItem = mPSI_SITree.InsertItem(temp, nImage, nSelectedImage, hParent, TVI_LAST);
	   
	mPSI_SITree.SetItemData(hCataItem, 1);


	temp.Empty();
	temp.Format(L"original_network_id = 0x%x", PSDT->SDTHeader.original_network_id);
	hCataItem = mPSI_SITree.InsertItem(temp, nImage, nSelectedImage, hParent, TVI_LAST);
	   
	mPSI_SITree.SetItemData(hCataItem, 1);

}
void CCTsDlg::SetSDTServiceTree(int nImage, int nSelectedImage, HTREEITEM hParent, SDTSeicer *PSDTSeicer)
{
	CString temp;
	HTREEITEM  hCataItem;
	temp.Empty();
	temp.Format(L"service_id = 0x%x", PSDTSeicer->SDTSeviveHeader.service_id);
	hCataItem = mPSI_SITree.InsertItem(temp, nImage, nSelectedImage, hParent, TVI_LAST);
	mPSI_SITree.SetItemData(hCataItem, 1);

	temp.Empty();
	temp.Format(L"EIT_schedule_flag = 0x%x", PSDTSeicer->SDTSeviveHeader.EIT_schedule_flag);
	hCataItem = mPSI_SITree.InsertItem(temp, nImage, nSelectedImage, hParent, TVI_LAST);
	mPSI_SITree.SetItemData(hCataItem, 1);

	temp.Empty();
	temp.Format(L"EIT_present_following_flag = 0x%x", PSDTSeicer->SDTSeviveHeader.EIT_present_following_flag);
	hCataItem = mPSI_SITree.InsertItem(temp, nImage, nSelectedImage, hParent, TVI_LAST);
	mPSI_SITree.SetItemData(hCataItem, 1);

	temp.Empty();
	temp.Format(L"running_status = 0x%x", PSDTSeicer->SDTSeviveHeader.running_status);
	hCataItem = mPSI_SITree.InsertItem(temp, nImage, nSelectedImage, hParent, TVI_LAST);
	mPSI_SITree.SetItemData(hCataItem, 1);

	temp.Empty();
	temp.Format(L"free_CA_mode = 0x%x", PSDTSeicer->SDTSeviveHeader.free_CA_mode);
	hCataItem = mPSI_SITree.InsertItem(temp, nImage, nSelectedImage, hParent, TVI_LAST);
	mPSI_SITree.SetItemData(hCataItem, 1);

	temp.Empty();
	temp.Format(L"descriptors_loop_length = 0x%x", PSDTSeicer->SDTSeviveHeader.descriptors_loop_length);
	hCataItem = mPSI_SITree.InsertItem(temp, nImage, nSelectedImage, hParent, TVI_LAST);
	mPSI_SITree.SetItemData(hCataItem, 1);
}


void CCTsDlg::PutOutServiceInfoOnTree(int NumOfService, SDTSeicer * PSDTSeicer, HTREEITEM Section)
{
	CString temp;
	for (int i = 0; i < NumOfService; i++)
	{
		temp.Empty();
		temp.Format(L"ҵ������%d", i + 1);

		HTREEITEM Service = mPSI_SITree.InsertItem(temp, 1, 1, Section, TVI_LAST);//ServiceDesciptor
		SetSDTServiceTree(1, 1, Service, PSDTSeicer);
		PutOutDscriptorInfoOnTree(PSDTSeicer->DesciptorNum, PSDTSeicer->PServiceDesciptor, Service);
		PSDTSeicer = PSDTSeicer->next;
	}
}


void CCTsDlg::PutOutDscriptorInfoOnTree(int NumOfDscrptor, ServiceDesciptor* PSvericeDscriptor, HTREEITEM TempDescriptor)
{
	CString temp;
	for (int i = 0; i < NumOfDscrptor; i++)
	{
		temp.Empty();
		if (NumOfDscrptor == 1)
		{
			temp.Format(L"������");
		}
		else
		{
			temp.Format(L"������%d", i + 1);
		}
		HTREEITEM Descriptor = mPSI_SITree.InsertItem(temp, 1, 1, TempDescriptor, TVI_LAST);//ServiceDesciptor
		
		temp.Empty();
		temp.Format(L"service_descriptor (descriptor_tag=0x%x)", PSvericeDscriptor->desciptor_tag);
		Descriptor = mPSI_SITree.InsertItem(temp, 1, 1, Descriptor, TVI_LAST);//ServiceDesciptor

		SetSDTDescriptorTree(1, 1, Descriptor, PSvericeDscriptor);
		PSvericeDscriptor = PSvericeDscriptor->next;
	}
}


void CCTsDlg::SetSDTDescriptorTree(int nImage, int nSelectedImage, HTREEITEM hParent, ServiceDesciptor *PSvericeDscriptor)
{
	CString temp;
	CString AtoUNi;
	HTREEITEM hCataItem;
	temp.Empty();
	temp.Format(L"desciptor_tag = 0x%x", PSvericeDscriptor->desciptor_tag);
	hCataItem = mPSI_SITree.InsertItem(temp, nImage, nSelectedImage, hParent, TVI_LAST);
	mPSI_SITree.SetItemData(hCataItem, 1);

	temp.Empty();
	temp.Format(L"desciptor_lenght = 0x%x", PSvericeDscriptor->desciptor_lenght);
	hCataItem = mPSI_SITree.InsertItem(temp, nImage, nSelectedImage, hParent, TVI_LAST);
	mPSI_SITree.SetItemData(hCataItem, 1);

	temp.Empty();
	temp.Format(L"service_type = 0x%x", PSvericeDscriptor->service_type);
	hCataItem = mPSI_SITree.InsertItem(temp, nImage, nSelectedImage, hParent, TVI_LAST);
	mPSI_SITree.SetItemData(hCataItem, 1);

	temp.Empty();
	temp.Format(L"service_provider_name_lenght = 0x%x", PSvericeDscriptor->service_provider_name_lenght);
	hCataItem = mPSI_SITree.InsertItem(temp, nImage, nSelectedImage, hParent, TVI_LAST);
	mPSI_SITree.SetItemData(hCataItem, 1);

	temp.Empty();


	AtoUNi = CString(PSvericeDscriptor->service_provider_name);
	temp.Format(L"service_provider_name = ");
	temp += AtoUNi;
	hCataItem = mPSI_SITree.InsertItem(temp, nImage, nSelectedImage, hParent, TVI_LAST);
	mPSI_SITree.SetItemData(hCataItem, 1);

	temp.Empty();
	temp.Format(L"service_name_lenght = 0x%x", PSvericeDscriptor->service_name_lenght);
	hCataItem = mPSI_SITree.InsertItem(temp, nImage, nSelectedImage, hParent, TVI_LAST);
	mPSI_SITree.SetItemData(hCataItem, 1);

	temp.Empty();
	AtoUNi = CString(PSvericeDscriptor->service_name);
	temp.Format(L"service_name = ");
	temp += AtoUNi;
	hCataItem = mPSI_SITree.InsertItem(temp, nImage, nSelectedImage, hParent, TVI_LAST);
	mPSI_SITree.SetItemData(hCataItem, 1);
}

void CCTsDlg::SetSDTTree()
{
	int index = 0;
	HTREEITEM SDTTree[2];
	int SDTindex = 0;;
	SDTTree[0] = mPSI_SITree.InsertItem(L"SDT_Actual", 0, 0);
	SDTTree[1] = mPSI_SITree.InsertItem(L"SDT_Other", 0, 0);
	for (int index1 = 0; index1 < totalPID; index1++)
	{
		if (PPSDT[index1] != NULL)
		{
			CString temp;
			temp.Format(L"Section:(table_id=0x%x,original_network_id=%d,transport_stream_id=%d,version_number=0x%x,section_number=0x%x)", PPSDT[index1]->SDTHeader.table_id, PPSDT[index1]->SDTHeader.original_network_id, PPSDT[index1]->SDTHeader.transport_stream_id, PPSDT[index1]->SDTHeader.version_number, PPSDT[index1]->SDTHeader.section_number);
			if (PPSDT[index1]->SDTHeader.table_id == 0x42)
			{
				SDTindex = 0;
			}
			else
			{
				SDTindex = 1;
			}
			HTREEITEM Section = mPSI_SITree.InsertItem(temp, 2, 2, SDTTree[SDTindex], TVI_LAST);
			SetTreeContent(1, 1, Section, PPSDT[index1]);
			PutOutServiceInfoOnTree(PPSDT[index1]->SDTSeiceNum, PPSDT[index1]->PSDTSeicer, Section);

			temp.Empty();
			temp.Format(L"CRC_32 = 0x%x", PPSDT[index1]->CRC_32);
			mPSI_SITree.InsertItem(temp, 1, 1, Section, TVI_LAST);

		}

	}
}


void CCTsDlg::SetCATTree()
{
	HTREEITEM CATTree, hCataItem, hCataItem1;
	CString temp;
	CATTree = mPSI_SITree.InsertItem(L"CAT", 0, 0);

	temp.Empty();
	temp.Format(L"Section :(version_number=0x%x,section_number=0x%x,current_next_indicator=0x%x)", PCAT->CATHeader.version_number, PCAT->CATHeader.section_number, PCAT->CATHeader.current_next_indicator);
	hCataItem1 = mPSI_SITree.InsertItem(temp, 2, 2, CATTree, TVI_LAST);
	mPSI_SITree.SetItemData(hCataItem1, 1);

	temp.Empty();
	temp.Format(L"table_id =0x%x", PCAT->CATHeader.table_id);
	hCataItem = mPSI_SITree.InsertItem(temp,1,1, hCataItem1, TVI_LAST);
	mPSI_SITree.SetItemData(hCataItem, 1);

	temp.Empty();
	temp.Format(L"section_synatx_indicator = 0x%x", PCAT->CATHeader.section_synatx_indicator);
	hCataItem = mPSI_SITree.InsertItem(temp,1,1, hCataItem1, TVI_LAST);
	mPSI_SITree.SetItemData(hCataItem, 1);

	temp.Empty();
	temp.Format(L"section_length = 0x%x", PCAT->CATHeader.section_length);
	hCataItem = mPSI_SITree.InsertItem(temp,1,1, hCataItem1, TVI_LAST);
	mPSI_SITree.SetItemData(hCataItem, 1);
    
	temp.Empty();
	temp.Format(L"current_next_indicator =0x%x", PCAT->CATHeader.current_next_indicator);
	hCataItem = mPSI_SITree.InsertItem(temp,1,1, hCataItem1, TVI_LAST);
	mPSI_SITree.SetItemData(hCataItem, 1);

	temp.Empty();
	temp.Format(L"section_number = 0x%x", PCAT->CATHeader.section_number);
	hCataItem = mPSI_SITree.InsertItem(temp,1,1, hCataItem1, TVI_LAST);
	mPSI_SITree.SetItemData(hCataItem, 1);

	temp.Empty();
	temp.Format(L"last_section_number = 0x%x", PCAT->CATHeader.last_section_number);
	hCataItem = mPSI_SITree.InsertItem(temp,1,1, hCataItem1, TVI_LAST);
	mPSI_SITree.SetItemData(hCataItem, 1);

	temp.Empty();
	temp.Format(L"CRC_32 = 0x%x", PCAT->CRC_32);
	hCataItem = mPSI_SITree.InsertItem(temp,1,1, hCataItem1, TVI_LAST);
	mPSI_SITree.SetItemData(hCataItem, 1);



}


int CCTsDlg::SetPATTree()
{
	HTREEITEM PATTree, hCataItem, hCataItem1, hCataItem2;
	CString temp;
	PATprogram *PATProgramP=NULL;
	int index = 0;
	PATTree = mPSI_SITree.InsertItem(L"PAT", 0, 0);

	temp.Empty();
	temp.Format(L"Section :(transport_stream_id=0x%x,version_number=0x%x,section_number=0x%x)", PATP->PATHeader.transport_stream_id, PATP->PATHeader.version_number, PATP->PATHeader.section_number);
	hCataItem1 = mPSI_SITree.InsertItem(temp, 2, 2, PATTree, TVI_LAST);
	mPSI_SITree.SetItemData(hCataItem1, 1);

	temp.Empty();
	temp.Format(L"table_id =0x%x", PATP->PATHeader.table_id);
	hCataItem = mPSI_SITree.InsertItem(temp, 1, 1, hCataItem1, TVI_LAST);
	mPSI_SITree.SetItemData(hCataItem, 1);

	temp.Empty();
	temp.Format(L"section_syntax_indicator = 0x%x", PATP->PATHeader.section_syntax_indicator);
	hCataItem = mPSI_SITree.InsertItem(temp, 1, 1, hCataItem1, TVI_LAST);
	mPSI_SITree.SetItemData(hCataItem, 1);

	temp.Empty();
	temp.Format(L"section_length = 0x%x", PATP->PATHeader.section_length);
	hCataItem = mPSI_SITree.InsertItem(temp, 1, 1, hCataItem1, TVI_LAST);
	mPSI_SITree.SetItemData(hCataItem, 1);

	temp.Empty();
	temp.Format(L"transport_stream_id =0x%x", PATP->PATHeader.transport_stream_id);
	hCataItem = mPSI_SITree.InsertItem(temp, 1, 1, hCataItem1, TVI_LAST);
	mPSI_SITree.SetItemData(hCataItem, 1);

	temp.Empty();
	temp.Format(L"version_number = 0x%x", PATP->PATHeader.version_number);
	hCataItem = mPSI_SITree.InsertItem(temp, 1, 1, hCataItem1, TVI_LAST);
	mPSI_SITree.SetItemData(hCataItem, 1);

	temp.Empty();
	temp.Format(L"current_next_indicator = 0x%x", PATP->PATHeader.current_next_indicator);
	hCataItem = mPSI_SITree.InsertItem(temp, 1, 1, hCataItem1, TVI_LAST);
	mPSI_SITree.SetItemData(hCataItem, 1);

	temp.Empty();
	temp.Format(L"section_number = 0x%x", PATP->PATHeader.section_number);
	hCataItem = mPSI_SITree.InsertItem(temp, 1, 1, hCataItem1, TVI_LAST);
	mPSI_SITree.SetItemData(hCataItem, 1);

	temp.Empty();
	temp.Format(L"last_section_number = 0x%x", PATP->PATHeader.last_section_number);
	hCataItem = mPSI_SITree.InsertItem(temp, 1, 1, hCataItem1, TVI_LAST);
	mPSI_SITree.SetItemData(hCataItem, 1);

	hCataItem2 = mPSI_SITree.InsertItem(L"PMT PID��Ϣ", 1, 1, hCataItem1, TVI_LAST);
	mPSI_SITree.SetItemData(hCataItem2, 1);

	PATProgramP = PATP->PATprogramP;
	for (index = 0; index < PATP->ProgramNum; index++)
	{
		temp.Empty();
		temp.Format(L"��Ŀ%d (program_number=%d,PMT_pid=0x%x)", index + 1, PATProgramP->program_number,PATProgramP->PMT_pid);
		hCataItem = mPSI_SITree.InsertItem(temp, 1, 1, hCataItem2, TVI_LAST);
		mPSI_SITree.SetItemData(hCataItem, 1);
		PATProgramP++;
	}

	temp.Empty();
	temp.Format(L"CRC_32 = 0x%x", PATP->CRC_32);
	hCataItem = mPSI_SITree.InsertItem(temp, 1, 1, hCataItem1, TVI_LAST);
	mPSI_SITree.SetItemData(hCataItem, 1);
	return 0;
}


int CCTsDlg::SetPMTTree()
{
	int index = 0;
	HTREEITEM PMTTree;
	int PMTindex = 0;;
	PMTTree = mPSI_SITree.InsertItem(L"PMT", 0, 0);
	for (int index1 = 0; index1 < PATP->ProgramNum; index1++)
	{
			CString temp;
			temp.Format(L"Section:(��Ŀ��=%d,version_number=0x%x,section_number=0x%d)", PPMT[index1]->PMTHeader.program_number, PPMT[index1]->PMTHeader.version_number, PPMT[index1]->PMTHeader.section_number);
			HTREEITEM Section = mPSI_SITree.InsertItem(temp, 2, 2, PMTTree, TVI_LAST);
			SetTreePMTContent(1, 1, Section, PPMT[index1]);
			PutOutStreamInfoOnTree(PPMT[index1]->StreamNum, PPMT[index1]->PMTStreamP, Section);//

			temp.Empty();
			temp.Format(L"CRC_32 = 0x%x", PPMT[index1]->CRC_32);
			mPSI_SITree.InsertItem(temp, 1, 1, Section, TVI_LAST);
	}
	return 0;
}


void CCTsDlg::SetPIDTree()
{
	CString temp;
	HTREEITEM  hCataItem;
	temp.Empty();
	mPIDtree.DeleteAllItems();
	for (int index = 0; index < PTSfile->NewPIDCount; index++)
	{
		temp.Format(L"PID:0x%.4x(%5d) ", PTSfile->PIDArr[index].PID, PTSfile->PIDArr[index].PID);
		hCataItem = mPIDtree.InsertItem(temp, 0, 0);
		this->mPIDtree.SetItemData(hCataItem, 1);
		mPIDtree.SetItemData(hCataItem, index);
	}
}


void CCTsDlg::OnTvnSelchangedTree2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	int ID = this->mPIDtree.GetSelectedCount();
	HTREEITEM  tempTree = this->mPIDtree.GetSelectedItem();
	ID = mPIDtree.GetItemData(tempTree);

	this->UpdateData(TRUE);

	mPIDEdit = PTSfile->PIDArr[ID].PID;
	mTotalBagEdit = PTSfile->PIDArr[ID].Total;
	mNbag = 1;
	this->UpdateData(FALSE);
	if (isReadOver == 1)
	{
		ShowDataBuff();
	}
	
	*pResult = 0;
}

void CCTsDlg::OnBnClickedGetxxbag()
{
	this->UpdateData(TRUE);

	if (mNbag = 987654321)
	{
		startMediaMode();
		return;
	}
	if (mNbag > mTotalBagEdit)
	{
		MessageBox(L"��������û����ô��İ�������", L"��ʾ", MB_OK);
		return;
	}
	
	ShowDataBuff();
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

void CCTsDlg::ShowDataBuff()
{
	this->UpdateData(TRUE);

	PTSfile->GetXXTSBag(mPIDEdit, mNbag, buff, 204, &header);

	CString temp, dataBuf;
	temp.Empty();
	dataBuf.Empty();
	mDataBuff.Empty();
	for (int index = 0; index < 204; index++)
	{
		temp.Format(L"%02X  ", buff[index]);
		dataBuf += temp;
		if (index % 20 == 19)
		{
			dataBuf += "\r\n";
		}
	}
	ShowHeaderTree(&header);
	mDataBuff = dataBuf;
	this->UpdateData(FALSE);
}


void CCTsDlg::ShowHeaderTree(TSheader *tempHeader)
{
	this->mTSheader.DeleteAllItems();

	HTREEITEM TSTree, hCataItem, hCataItem1;
	CString temp;
	PATprogram *PATProgramP = NULL;
	int index = 0;
	TSTree = mTSheader.InsertItem(L"TS��ͷ", 0, 0);

	temp.Empty();
	temp.Format(L"Sync_byte=0x%x", tempHeader->Sync_byte);
	hCataItem1 = mTSheader.InsertItem(temp, 1, 1, TSTree, TVI_LAST);
	mTSheader.SetItemData(hCataItem1, 1);

	temp.Empty();
	temp.Format(L"Transport_error_indicator =0x%x", tempHeader->Transport_error_indicator);
	hCataItem = mTSheader.InsertItem(temp, 1, 1, TSTree, TVI_LAST);
	mTSheader.SetItemData(hCataItem, 1);

	temp.Empty();
	temp.Format(L"payloader_unit_start_indicator = 0x%x", tempHeader->Start_indicator);
	hCataItem = mTSheader.InsertItem(temp, 1, 1, TSTree, TVI_LAST);
	mTSheader.SetItemData(hCataItem, 1);

	temp.Empty();
	temp.Format(L"transport_priority=0x%x", tempHeader->Transport_pri);
	hCataItem1 = mTSheader.InsertItem(temp, 1, 1, TSTree, TVI_LAST);
	mTSheader.SetItemData(hCataItem1, 1);

	temp.Empty();
	temp.Format(L"PID =0x%x", tempHeader->PID);
	hCataItem = mTSheader.InsertItem(temp, 1, 1, TSTree, TVI_LAST);
	mTSheader.SetItemData(hCataItem, 1);

	temp.Empty();
	temp.Format(L"transport_scrambling_control = 0x%x", tempHeader->Condition_control);
	hCataItem = mTSheader.InsertItem(temp, 1, 1, TSTree, TVI_LAST);
	mTSheader.SetItemData(hCataItem, 1);

	temp.Empty();
	temp.Format(L"adaptation_field_control = 0x%x", tempHeader->Diy_zone);
	hCataItem = mTSheader.InsertItem(temp, 1, 1, TSTree, TVI_LAST);
	mTSheader.SetItemData(hCataItem, 1);

	temp.Empty();
	temp.Format(L"continuity = 0x%x", tempHeader->Counter);
	hCataItem = mTSheader.InsertItem(temp, 1, 1, TSTree, TVI_LAST);
	mTSheader.SetItemData(hCataItem, 1);

	mTSheader.Expand(TSTree, TVE_EXPAND);
}


void CCTsDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	static unsigned long count = 0;

	if (count == 0)
	{
		this->mprogress.SetRange32(0, PTSfile->mFile.FileLenght);
		mprogress.SetPos(PTSfile->GetCurrentOffSet());
		count = 1;
	}
	if (count == 1)
	{
		mPIDtree.DeleteAllItems();
		SetPIDTree();
	}
	
	if (isReadOver == 1&&count==1)
	{
		//isReadOver = 0;
		count = 2;
		this->SetTree();
		this->GetDlgItem(IDCANCEL)->EnableWindow(1);
	}
		
	mprogress.SetPos(PTSfile->GetCurrentOffSet());
	CDialogEx::OnTimer(nIDEvent);
}


int CCTsDlg::SetTreePMTContent(int nImage, int nSelectedImage, HTREEITEM hParent, PMT* PPMT)
{
	CString temp;
	HTREEITEM  hCataItem;
	temp.Empty();
	temp.Format(L"table_id = 0x%x", PPMT->PMTHeader.table_id);
	hCataItem = mPSI_SITree.InsertItem(temp, nImage, nSelectedImage, hParent, TVI_LAST);
	mPSI_SITree.SetItemData(hCataItem, 1);

	temp.Empty();
	temp.Format(L"section_syntax_indicator = 0x%x", PPMT->PMTHeader.section_syntax_indicator);
	hCataItem = mPSI_SITree.InsertItem(temp, nImage, nSelectedImage, hParent, TVI_LAST);
	mPSI_SITree.SetItemData(hCataItem, 1);

	temp.Empty();
	temp.Format(L"section_length = 0x%x", PPMT->PMTHeader.section_length);
	hCataItem = mPSI_SITree.InsertItem(temp, nImage, nSelectedImage, hParent, TVI_LAST);
	mPSI_SITree.SetItemData(hCataItem, 1);

	temp.Empty();
	temp.Format(L"program_number = 0x%x", PPMT->PMTHeader.program_number);
	hCataItem = mPSI_SITree.InsertItem(temp, nImage, nSelectedImage, hParent, TVI_LAST);
	mPSI_SITree.SetItemData(hCataItem, 1);

	temp.Empty();
	temp.Format(L"version_number = 0x%x", PPMT->PMTHeader.version_number);
	hCataItem = mPSI_SITree.InsertItem(temp, nImage, nSelectedImage, hParent, TVI_LAST);
	mPSI_SITree.SetItemData(hCataItem, 1);

	temp.Empty();
	temp.Format(L"current_next_indicator = 0x%x", PPMT->PMTHeader.current_next_indicator);
	hCataItem = mPSI_SITree.InsertItem(temp, nImage, nSelectedImage, hParent, TVI_LAST);
	mPSI_SITree.SetItemData(hCataItem, 1);

	temp.Empty();
	temp.Format(L"section_number = 0x%x", PPMT->PMTHeader.section_number);
	hCataItem = mPSI_SITree.InsertItem(temp, nImage, nSelectedImage, hParent, TVI_LAST);
	mPSI_SITree.SetItemData(hCataItem, 1);

	temp.Empty();
	temp.Format(L"last_section_number = 0x%x", PPMT->PMTHeader.last_section_number);
	hCataItem = mPSI_SITree.InsertItem(temp, nImage, nSelectedImage, hParent, TVI_LAST);
	mPSI_SITree.SetItemData(hCataItem, 1);

	temp.Empty();
	temp.Format(L"PCR_PID = 0x%x", PPMT->PMTHeader.PCR_PID);
	hCataItem = mPSI_SITree.InsertItem(temp, nImage, nSelectedImage, hParent, TVI_LAST);
	mPSI_SITree.SetItemData(hCataItem, 1);

	temp.Empty();
	temp.Format(L"program_info_length = 0x%x", PPMT->PMTHeader.program_info_length);
	hCataItem = mPSI_SITree.InsertItem(temp, nImage, nSelectedImage, hParent, TVI_LAST);
	mPSI_SITree.SetItemData(hCataItem, 1);

	return 0;
}


int CCTsDlg::PutOutStreamInfoOnTree(int StreamNum, PMTStream * PMTStreamP, HTREEITEM tempStream)
{
	CString temp;
	HTREEITEM  hCataItem;
	PMTStream *tempPMTStreamP = PMTStreamP;
	temp.Empty();

	for (int index = 0; index < StreamNum; index++)
	{
		temp.Empty();
		temp.Format(L"��Ԫ��%d", index+1);
		hCataItem = mPSI_SITree.InsertItem(temp, 1, 1, tempStream, TVI_LAST);
		mPSI_SITree.SetItemData(hCataItem, 1);

		temp.Empty();
		temp.Format(L"ES PID =0x%x, stream_tpye=0x%x, Es_Info_len=%d", tempPMTStreamP->PMTSPart.elementary_PID, tempPMTStreamP->stream_type, tempPMTStreamP->PMTSPart.ES_info_length);
		hCataItem = mPSI_SITree.InsertItem(temp, 1, 1, hCataItem, TVI_LAST);
		mPSI_SITree.SetItemData(hCataItem, 1);
		tempPMTStreamP = tempPMTStreamP->next;
	}
	return 0;
}





void CCTsDlg::OnBnClickedCancel()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	ffmfc_quit();
	for (int index = 0; index < totalPID; index++)
	{
		if (PPSDT[index] != NULL)
		{
			PPSDT[index]->DeleteSDTClass(PPSDT[index]);
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

	CDialogEx::OnCancel();
}

/*
void CCTsDlg::OnStnClickedSta()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}*/


void CCTsDlg::OnBnClickedButton3()
{
	//::afxdestoryth
	
	this->programbn = (++programCount)%totalProgramNumber;
	ffmfc_quit();
	Sleep(200);
	AfxBeginThread(Thread_Play, &TSSend);//�����߳�
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CCTsDlg::OnBnClickedButton4()
{
	if (programCount == 0)
		programCount = totalProgramNumber*100;

	this->programbn = (--programCount)%totalProgramNumber;

	ffmfc_quit();
	//Sleep(300);
	//startTVinit(this);
	//Sleep(200);
	//SDL_Quit();
	Sleep(300);
	AfxBeginThread(Thread_Play, &TSSend);//�����߳�
	//AfxBeginThread(Thread_Play, this);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CCTsDlg::OnBnClickedButton5()
{
	ffmfc_quit();
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CCTsDlg::OnStnClickedpic1()
{
	//POINT temp;
	//GetCursorPos(&temp);
	//ClientToScreen(&temp);
	//ffmfc_quit();
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


int CCTsDlg::startMediaMode()
{
	MessageBox(L"Media mode begin"); 

	this->GetDlgItem(IDCANCEL)->ShowWindow(0);
	this->GetDlgItem(IDOpenFile)->ShowWindow(0);
	this->GetDlgItem(IDC_BUTTON3)->ShowWindow(0);
	this->GetDlgItem(IDC_BUTTON4)->ShowWindow(0);
	this->GetDlgItem(IDC_BUTTON5)->ShowWindow(0);
	this->GetDlgItem(IDC_BUTTON6)->ShowWindow(1);
	return 0;
}


void CCTsDlg::OnBnClickedButton6()
{
     this->ShowWindow(0);
	 mediaMode TempMediaMode;
	 TempMediaMode.DoModal();
	
	 OnBnClickedCancel();
	
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}
