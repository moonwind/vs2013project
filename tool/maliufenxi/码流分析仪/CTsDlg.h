
// CTsDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include <stdio.h>
#include<stdlib.h>
#include"common.h"
#include "tsfile.h"
#include "PAT.h"
#include "PMT.h"
#include "CAT.h"
#include "SDT.h"
#include "afxwin.h"

// CCTsDlg �Ի���
class CCTsDlg : public CDialogEx
{
// ����
public:
	CCTsDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl mPSI_SITree;
	void SetTree();

private:
	CImageList mImage;

public:
	afx_msg void OnBnClickedOpenfile();
	void SetTreeContent(int nImage, int nSelectedImage, HTREEITEM hParent,SDT* PSDT);
	void SetSDTServiceTree(int nImage, int nSelectedImage, HTREEITEM hParent, SDTSeicer *PSDTSeicer);
	void PutOutServiceInfoOnTree(int NumOfService, SDTSeicer * PSDTSeicer, HTREEITEM Section);
	void PutOutDscriptorInfoOnTree(int NumOfDscrptor, ServiceDesciptor* PSvericeDscriptor, HTREEITEM TempDescriptor);
	void SetSDTDescriptorTree(int nImage, int nSelectedImage, HTREEITEM hParent, ServiceDesciptor *PSvericeDscriptor);
	void SetSDTTree();
	void SetCATTree();
	int SetPATTree();
	int SetPMTTree();
	CTreeCtrl mPIDtree;
	void SetPIDTree();
	afx_msg void OnTvnSelchangedTree2(NMHDR *pNMHDR, LRESULT *pResult);
	int mPIDEdit;
	int mTotalBagEdit;
	afx_msg void OnBnClickedGetxxbag();
	int mNbag;
	CString mDataBuff;
	void ShowDataBuff();
	CTreeCtrl mTSheader;
	void ShowHeaderTree(TSheader *tempHeader);
	CButton mOpenFile;
	CProgressCtrl mprogress;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	int SetTreePMTContent(int nImage, int nSelectedImage, HTREEITEM hParent, PMT* PPMT);
	int PutOutStreamInfoOnTree(int StreamNum, PMTStream * PMTStreamP, HTREEITEM tempStream);
	afx_msg void OnBnClickedCancel();
	
	afx_msg void OnStnClickedSta();
	CStringA filepath;
	afx_msg void OnBnClickedButton3();
	int programbn;
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	int isTS;
	int totalProgramNumber;
	unsigned char programCount;
	afx_msg void OnStnClickedpic1();
	int startMediaMode();
	afx_msg void OnBnClickedButton6();
};
