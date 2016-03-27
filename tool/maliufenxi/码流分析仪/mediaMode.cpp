// mediaMode.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "mediaMode.h"
#include "afxdialogex.h"
#include "ffplaycore.h"

SendAgrStruct  MediaSend;
// mediaMode �Ի���

IMPLEMENT_DYNAMIC(mediaMode, CDialogEx)

mediaMode::mediaMode(CWnd* pParent /*=NULL*/)
	: CDialogEx(mediaMode::IDD, pParent)
	, isTS(0)
{
	MediaSend.dlgMedia = this;
	MediaSend.dlgCCTs = NULL;
	MediaSend.dlgtype = media;
}

mediaMode::~mediaMode()
{
}

void mediaMode::DoDataExchange(CDataExchange* pDX)
{
	
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(mediaMode, CDialogEx)
	ON_COMMAND(ID_NEXT_PANE, OnOpenFile)
	ON_WM_CHAR()
END_MESSAGE_MAP()


// mediaMode ��Ϣ�������

UINT Thread_Play1(LPVOID lpParam){
	//CFFMPE_MFC_SDL_testDlg *dlg = (CFFMPE_MFC_SDL_testDlg *)lpParam;
	ffmfc_play(lpParam);
	return 0;
}
void mediaMode::OnOpenFile()
{

	CString FilePathName, FileName;
	CFileDialog dlg(TRUE);///TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի��� 
	if (dlg.DoModal() == IDOK)
	{
		FilePathName = dlg.GetPathName();
		FileName = dlg.GetFileName();
	}
	if (FilePathName == "")
	{
		return;
	}

	CStringA tempPath = CStringA(FilePathName.GetBuffer());
	CStringA tempName = CStringA(FileName.GetBuffer());
	filepath = CStringA(tempPath);
	
	AfxBeginThread(Thread_Play1, &MediaSend);//�����߳�
}


void mediaMode::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CString temp;
	temp.Empty();
	temp.Format(L"%c",nChar);
	MessageBox(temp);

	CDialogEx::OnChar(nChar, nRepCnt, nFlags);
}
