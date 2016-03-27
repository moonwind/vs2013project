// mediaMode.cpp : 实现文件
//

#include "stdafx.h"
#include "mediaMode.h"
#include "afxdialogex.h"
#include "ffplaycore.h"

SendAgrStruct  MediaSend;
// mediaMode 对话框

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


// mediaMode 消息处理程序

UINT Thread_Play1(LPVOID lpParam){
	//CFFMPE_MFC_SDL_testDlg *dlg = (CFFMPE_MFC_SDL_testDlg *)lpParam;
	ffmfc_play(lpParam);
	return 0;
}
void mediaMode::OnOpenFile()
{

	CString FilePathName, FileName;
	CFileDialog dlg(TRUE);///TRUE为OPEN对话框，FALSE为SAVE AS对话框 
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
	
	AfxBeginThread(Thread_Play1, &MediaSend);//开启线程
}


void mediaMode::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	CString temp;
	temp.Empty();
	temp.Format(L"%c",nChar);
	MessageBox(temp);

	CDialogEx::OnChar(nChar, nRepCnt, nFlags);
}
