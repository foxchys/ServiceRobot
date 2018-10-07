#pragma once

#include"SrobotDlg.h"//父窗口SrobotDlg的头文件
// CManageSetDlg dialog
//---------------------opencv-------------------
#ifndef OPENCVHEAD
#define OPENCVHEAD
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#endif
//---------------------------------------------
#include "afxwin.h"


class CManageSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CManageSetDlg)

public:
	CManageSetDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CManageSetDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MANAGESET_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	int BaudRate[12] = { 300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 38400, 56000, 57600, 115200 };
	afx_msg void OnBnClickedExitmana();
	afx_msg void OnBnClickedLenssw();
	void DrawPic(cv::Mat& img, UINT nID);//镜头显示到界面
	volatile bool ShowLeFlg;//镜头显示子线程开关的标志
	static UINT ShowLens(void* param);//镜头显示子线程
	afx_msg void OnBnClickedChoosepic();
	afx_msg void OnBnClickedTakepic();
	afx_msg void OnBnClickedUploadpic();
	static UINT UploadFace(void* param);//人脸录入子线程
	void PhizModuleIniti();//表情模块初始化
	// 表情的波特率
	CComboBox m_BaudPhiz;
	// 表情的COM口号
	CComboBox m_ComPhiz;
	afx_msg void OnBnClickedDeteccom();
	afx_msg void OnBnClickedSwphiz();
	afx_msg void OnBnClickedSwmove();
	// 移动模块串口号
	CComboBox m_ComMove;
	// 移动模块波特率
	CComboBox m_BaudMove;
	void MoveModuleIniti();//移动模块初始化
	void ArmModuleIniti();//机械手臂模块初始化
	afx_msg void OnBnClickedSwarm();
	// 机械臂模块波特率F
	CComboBox m_BaudArmF;
	// 机械臂模块波特率S
	CComboBox m_BaudArmS;
	// 机械臂模块串口号F
	CComboBox m_ComArmF;
	// 机械臂模块串口号S
	CComboBox m_ComArmS;
	afx_msg void OnBnClickedActtorque();
};
//访问父窗口类	CSrobotDlg *pDlg = (CSrobotDlg*)this->GetParent();