#pragma once

#include "resource.h"	
// mediaMode 对话框
#include "afxcmn.h"

#include "afxwin.h"
class mediaMode : public CDialogEx
{
	DECLARE_DYNAMIC(mediaMode)

public:
	mediaMode(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~mediaMode();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void OnOpenFile();
	CStringA filepath;
	int isTS;
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};
