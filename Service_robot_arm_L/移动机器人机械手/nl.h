
// 移动机器人机械手.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// C移动机器人机械手App: 
// 有关此类的实现，请参阅 移动机器人机械手.cpp
//

class CnlApp : public CWinApp
{
public:
	CnlApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CnlApp theApp;