#pragma once

#include "resource.h"	
// mediaMode �Ի���
#include "afxcmn.h"

#include "afxwin.h"
class mediaMode : public CDialogEx
{
	DECLARE_DYNAMIC(mediaMode)

public:
	mediaMode(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~mediaMode();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	void OnOpenFile();
	CStringA filepath;
	int isTS;
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};
