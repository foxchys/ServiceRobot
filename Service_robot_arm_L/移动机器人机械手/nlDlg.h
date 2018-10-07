
// 移动机器人机械手Dlg.h : 头文件
//
#pragma once

#include "GRNN.h"
#include "float.h"
#include "Public.h"
#include "afxwin.h"

// C移动机器人机械手Dlg 对话框
class CnlDlg : public CDialogEx
{
// 构造
public:
	CnlDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSyscommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	int m_vec;
	CButton m_1;
	CButton m_2;
	CButton m_3;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedSetcom();
	afx_msg void OnBnClickedTorque(UINT nID);
	afx_msg void OnBnClickedMonitor();
	int m_speed;
	static UINT Thread1(void *param);
	bool ThreadFlag;
	afx_msg void OnBnClickedSinglemove();
	CButton m_4;
	CButton m_5;
	CButton m_6;
	CButton m_7;
	CButton m_8;
	CButton m_9;
	afx_msg void OnBnClickedGym();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	bool TimerFlag;
	afx_msg void OnBnClickedButton4();
};
