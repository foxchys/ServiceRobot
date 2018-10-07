
// SrobotDlg.h : 头文件
//

#pragma once

#include"ManageSetDlg.h"//子窗口ManageSetDlg的头文件

#include"XunFlySpecre.h"//录音加讯飞
#include"SerialPort.h"//串口通信
#include"NetCurl.h"//图灵机器人webapi请求
#include"OrderSearch.h"
#include <sphelper.h>//微软Microsoft Speech SDK 语音头文件
//---------------------opencv-------------------
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
//---------------------------------------------
#include<ctime>//系统时间

#include "afxcmn.h"
#include "afxwin.h"

// CSrobotDlg 对话框
class CSrobotDlg : public CDialogEx
{
// 构造
public:
	CSrobotDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SROBOT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	SpecRe XunflySpe;//语音识别类
	TulingRobot Tuling;//图灵机器人webapi类

	// 录音灵敏度滑块
	CSliderCtrl m_Sensitivity;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedEditsensi();
	// 语音识别结果
	CEdit m_SrResult;
	// 语音识别开关
	CButton m_SrSwitch;
	afx_msg void OnBnClickedSrswitch();
	volatile bool XunflySpeRFlag;//语音识别线程标志
	volatile bool XunflySRSwitchFlg;//语音识别停止标志
	volatile bool SpeechSwFlg;//语音和智能应答关闭按钮的标志
	volatile bool AISwFlg;//智能应答开关标志
	static UINT XunflySpeR(void *param);//语音识别线程函数
	char* SendToXfly();//录音并发送至讯飞获得识别结果
	std::string SendToTuling(char* mytext);//将识别结果发送至图灵机器人并获得回复
	volatile bool StaSpeechPlay;//语音播报状态控制标志
	void TextToSpeech(CString mytext);//使用微软 speech sdk 文字转语音每次调用该函数要将StaSpeechPlay = true;
	afx_msg void OnBnClickedAiswitch();
	afx_msg void OnBnClickedExitall();
	volatile bool AiCloseFlg;//关闭Ai按钮的标志
	static UINT AiClose(void* param);//关闭Ai智能应答按钮防界面卡死的线程
	void DrawMat(cv::Mat& img, UINT nID);//摄像头显示到界面
	afx_msg void OnBnClickedCamerasw();//摄像头开关
	volatile bool ShowCaFlg;//摄像头显示的标志
	static UINT ShowCamera(void* param);//摄像头显示子线程
	static UINT PostFacepp(void *param);//post请求face++的子线程
	volatile bool PostFaceFlg;//post到face++的线程是否结束标志
	afx_msg void OnBnClickedAdvacedset();
	CSerialPort PhizModule;//表情模块串口
	volatile bool PhizFlg;//表情模块串口打开的标志
	volatile int PhizInf[2];//表情模块串口信息记录
	CSerialPort MoveModule;//轮式移动模块串口
	volatile bool MoveFlg;//轮式移动模块串口打开的标志
	volatile int MoveInf[2];//轮式移动模块串口信息记录
	CSerialPort ArmModuleF;//机械手臂模块串口First
	CSerialPort ArmModuleS;//机械手臂模块串口Second
	volatile int Gymnastics;//手臂动作定时器线程计数
	volatile bool ArmFlg;//机械手臂模块打开的标志
	volatile int ArmInf[4];//机械手臂模块串口信息记录
	afx_msg void OnBnClickedActionsw();//动作开关
	void OrderAction(const char* myorder);//语音转动作
	volatile bool SpeechPlayFlg;//图灵语音播报线程控制标志
	CString SpeechWord;//本次语音播报的语句
	static UINT SpeechPlay(void *param);//语音播报线程函数
	afx_msg void OnBnClickedRelasetorque();
	afx_msg void OnBnClickedActionres();
	afx_msg void OnBnClickedGymsw();
	afx_msg void OnBnClickedPiaffesw();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedStopall();
	afx_msg void OnBnClickedHugarm();
	afx_msg void OnBnClickedInvitearm();
};
