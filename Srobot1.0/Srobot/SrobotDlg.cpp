
// SrobotDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Srobot.h"
#include "SrobotDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSrobotDlg 对话框



CSrobotDlg::CSrobotDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SROBOT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSrobotDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Sensitivity, m_Sensitivity);
	DDX_Control(pDX, IDC_SrResult, m_SrResult);
	DDX_Control(pDX, IDC_SrSwitch, m_SrSwitch);
}

BEGIN_MESSAGE_MAP(CSrobotDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_EditSensi, &CSrobotDlg::OnBnClickedEditsensi)
	ON_BN_CLICKED(IDC_SrSwitch, &CSrobotDlg::OnBnClickedSrswitch)
	ON_BN_CLICKED(IDC_AISwitch, &CSrobotDlg::OnBnClickedAiswitch)
	ON_BN_CLICKED(IDC_ExitAll, &CSrobotDlg::OnBnClickedExitall)
	ON_BN_CLICKED(IDC_CameraSw, &CSrobotDlg::OnBnClickedCamerasw)
	ON_BN_CLICKED(IDC_AdvancedSet, &CSrobotDlg::OnBnClickedAdvacedset)
	ON_BN_CLICKED(IDC_ActionSw, &CSrobotDlg::OnBnClickedActionsw)
	ON_BN_CLICKED(IDC_RelaseTorque, &CSrobotDlg::OnBnClickedRelasetorque)
	ON_BN_CLICKED(IDC_ActionRes, &CSrobotDlg::OnBnClickedActionres)
	ON_BN_CLICKED(IDC_GymSw, &CSrobotDlg::OnBnClickedGymsw)
	ON_BN_CLICKED(IDC_PiaffeSw, &CSrobotDlg::OnBnClickedPiaffesw)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_StopAll, &CSrobotDlg::OnBnClickedStopall)
	ON_BN_CLICKED(IDC_HugArm, &CSrobotDlg::OnBnClickedHugarm)
	ON_BN_CLICKED(IDC_InviteArm, &CSrobotDlg::OnBnClickedInvitearm)
END_MESSAGE_MAP()


// CSrobotDlg 消息处理程序

BOOL CSrobotDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	XunflySRSwitchFlg = false;//语音识别开关停止标志初始化
	m_Sensitivity.SetRange(1, 100, FALSE);//设置滑块的范围
	m_Sensitivity.SetPos(21);//设置滑块的初始值
	GetDlgItem(IDC_SensiSta)->SetWindowTextW(_T("21"));//将滑块初始值显示设为21
	GetDlgItem(IDC_SrStatu)->SetWindowTextW(_T(""));//状态栏初始值显示设为空
	//XunflySpe.sensitivity = m_Sensitivity.GetPos();//设置录音检测灵敏度
	m_SrSwitch.SetWindowTextW(_T("打开语音识别"));
	GetDlgItem(IDC_AISwitch)->SetWindowTextW(_T("打开智能应答"));
	XunflySpeRFlag = false;//语音识别子线程标志关闭
	GetDlgItem(IDC_AISwitch)->EnableWindow(false);
	GetDlgItem(IDC_CameraSw)->SetWindowTextW(_T("打开摄像头"));
	GetDlgItem(IDC_EditSensi)->SetWindowTextW(_T("设置录音灵敏度"));
	GetDlgItem(IDC_ExitAll)->SetWindowTextW(_T("退出程序"));
	GetDlgItem(IDC_AdvancedSet)->SetWindowTextW(_T("高级设置"));
	GetDlgItem(IDC_ActionSw)->SetWindowTextW(_T("打开演示动作"));
	GetDlgItem(IDC_GymSw)->SetWindowTextW(_T("开始广播体操"));
	GetDlgItem(IDC_PiaffeSw)->SetWindowTextW(_T("开始原地踏步"));
	GetDlgItem(IDC_ActionRes)->SetWindowTextW(_T("动作复位"));
	GetDlgItem(IDC_RelaseTorque)->SetWindowTextW(_T("撤销扭矩"));
	GetDlgItem(IDC_StopAll)->SetWindowTextW(_T("停止全部动作"));
	GetDlgItem(IDC_HugArm)->SetWindowTextW(_T("拥抱"));
	GetDlgItem(IDC_InviteArm)->SetWindowTextW(_T("发出邀请"));
	ShowCaFlg = false;//摄像头开关标志复位
	PostFaceFlg = true;//post请求face++的线程是否结束标志
	SpeechSwFlg = false;//语音和智能应答关闭按钮的标志
	AISwFlg = true;//智能应答开关标志
	AiCloseFlg = true;//关闭Ai按钮的标志
	PhizInf[0] = NULL, PhizInf[1] = NULL;
	MoveInf[0] = NULL, MoveInf[1] = NULL;
	ArmInf[0] = NULL, ArmInf[1] = NULL, ArmInf[2] = NULL, ArmInf[3] = NULL;
	PhizFlg = false;//表情模块串口打开的标志
	MoveFlg = false;//移动模块打开的标志
	ArmFlg = false;//机械手臂模块打开的标志
	SpeechWord = _T("");
	SpeechPlayFlg = false;//语音播报标志
	StaSpeechPlay = false;//语音播报状态标志
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSrobotDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSrobotDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSrobotDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//滑动条消息响应函数
void CSrobotDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default

	//CSliderCtrl *pSlidCtrl= (CSliderCtrl*)GetDlgItem(IDC_Sensitivity);
	int value;
	value = m_Sensitivity.GetPos();//获取滑动条的值
	CString tem;
	tem.Format(_T("%d"), value);
	GetDlgItem(IDC_SensiSta)->SetWindowTextW(tem);
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CSrobotDlg::OnBnClickedEditsensi()
{
	// TODO: Add your control notification handler code here
	//CSliderCtrl *pSlidCtrl = (CSliderCtrl*)GetDlgItem(IDC_Sensitivity);
	int value;
	value = m_Sensitivity.GetPos();//获取滑动条的值
	XunflySpe.sensitivity = value;
	XunflySpe.vda_count = 0;
	GetDlgItem(IDC_SrStatu)->SetWindowTextW(_T("录音灵敏度已设置"));
}


void CSrobotDlg::OnBnClickedSrswitch()
{
	// TODO: Add your control notification handler code here
	int voicedevice=waveInGetNumDevs();//录音设备的数量
	if (!voicedevice)
	{
		AfxMessageBox(L"未检测到录音设备");
		return;
	}
	CString temp;
	/*UpdateData(true);*/
	m_SrSwitch.GetWindowTextW(temp);
	if (temp == "打开语音识别")
	{
		m_SrSwitch.EnableWindow(false);
		m_SrSwitch.SetWindowTextW(_T("正在打开语音"));
		XunflySRSwitchFlg = false;//语音识别开关停止标志
		XunflySpe.WriteFeil("myspeech.wav");//设定.wav文件保存名和路径
		XunflySpe.VDA_Initialize();//录音初始化
		XunflySpe.XflySR_Initialize();//讯飞识别初始化 
		XunflySpe.sensitivity = m_Sensitivity.GetPos();//设置录音检测灵敏度
		XunflySpe.vda_count = 0;
		//打开一个子线程进行语音识别操作
		XunflySpeRFlag = true;
		AfxBeginThread(XunflySpeR, this, THREAD_PRIORITY_HIGHEST);
		m_SrSwitch.SetWindowTextW(_T("关闭语音识别"));
		m_SrSwitch.EnableWindow(true);
		GetDlgItem(IDC_SrStatu)->SetWindowTextW(_T("语音识别已打开"));
		GetDlgItem(IDC_AISwitch)->EnableWindow(true);
	}
	else
	{
		GetDlgItem(IDC_SrSwitch)->EnableWindow(false);
		GetDlgItem(IDC_SrSwitch)->SetWindowTextW(_T("正在关闭识别"));
		XunflySRSwitchFlg = true;//语音识别开关停止标志
		XunflySpe.stop = true;//录音回掉函数停止
		XunflySpeRFlag = false;//关闭进行语音识别操作的子线程
		//XunflySpe.AutoRecord_Close();//关闭录音
		//m_SrSwitch.SetWindowTextW(_T("打开语音识别"));
		GetDlgItem(IDC_AISwitch)->GetWindowTextW(temp);
		if (temp == "关闭智能应答")
		{
			OnBnClickedAiswitch();//关闭智能应答
		}
	}

}

UINT CSrobotDlg::XunflySpeR(void *param)
{
	CSrobotDlg* dlg = (CSrobotDlg*)param;
	dlg->SpeechSwFlg = false;
	/*CEdit* SrResult = (CEdit*)dlg->GetDlgItem(IDC_SrResult);*/
	CString SResult;
	CString TulingRe;
	CString TemAISwitchFlg;
	CString temp;
	char* SpeResult;
	int lastLine;
	while (dlg->XunflySpeRFlag)
	{
		dlg->GetDlgItem(IDC_SrStatu)->SetWindowTextW(_T("开始录音"));
		SpeResult = dlg->SendToXfly();//获取识别结果
		if (0 == strcmp(SpeResult,"抱歉，我没说清楚。"))
			continue;
		dlg->GetDlgItem(IDC_ActionSw)->GetWindowTextW(temp);
		if (temp == "关闭演示动作")
			dlg->OrderAction(SpeResult);
		SResult = _T("我：");
		SResult += SpeResult;
		SResult += _T("\r\n");
		lastLine = dlg->m_SrResult.LineIndex(dlg->m_SrResult.GetLineCount() - 1);
		dlg->m_SrResult.SetSel(lastLine + 1, lastLine + 2, 1);
		dlg->m_SrResult.ReplaceSel(SResult);//在最后一行加新的语音识别结果
		dlg->m_SrResult.UpdateWindow();//更新显示，不加这句有时更新有延迟

		dlg->GetDlgItem(IDC_AISwitch)->GetWindowTextW(TemAISwitchFlg);
		if (TemAISwitchFlg == "关闭智能应答")
		{
			dlg->AISwFlg = false;
			if (0 == strcmp(SpeResult, "抱歉，我没说清楚。"))
				break;
			dlg->GetDlgItem(IDC_SrStatu)->SetWindowTextW(_T("正在请求回复"));
			TulingRe = dlg->SendToTuling(SpeResult).c_str();	//获取图灵机器人应答结果
			while (!(dlg->PostFaceFlg)) Sleep(200);//等待人脸识别语音播报结束
			lastLine = dlg->m_SrResult.LineIndex(dlg->m_SrResult.GetLineCount() - 1);
			dlg->m_SrResult.SetSel(lastLine + 1, lastLine + 2, 1);
			dlg->m_SrResult.ReplaceSel(L"AI：" + TulingRe + "\r\n");//在最后一行加新的图灵机器人返回结果
			dlg->m_SrResult.UpdateWindow();//更新显示，不加这句有时更新有延迟
			dlg->GetDlgItem(IDC_SrStatu)->SetWindowTextW(_T("正在语音播报"));
			//dlg->TextToSpeech(TulingRe);//将图灵机器人应答结果转化为语音
			dlg->StaSpeechPlay = false;//停止当前播报
			while (dlg->SpeechPlayFlg)Sleep(10);//等待上一个图灵回复语音合成进程退出
			dlg->SpeechWord = TulingRe;//将带合成语句赋给公用变量
			AfxBeginThread(SpeechPlay, dlg, THREAD_PRIORITY_HIGHEST);//打开一个
			dlg->AISwFlg = true;
		}
	}
	dlg->XunflySpe.XflySR_Close();//关闭讯飞识别
	OutputDebugStringA("语音识别进程结束\n\n");
	while (!(dlg->AiCloseFlg)) Sleep(200);
	CButton* tem = (CButton*)dlg->GetDlgItem(IDC_SrSwitch);
	tem->SetWindowTextW(_T("打开语音识别"));
	dlg->GetDlgItem(IDC_AISwitch)->EnableWindow(false);
	dlg->GetDlgItem(IDC_SrStatu)->SetWindowTextW(_T("语音识别已关闭"));
	dlg->GetDlgItem(IDC_SrSwitch)->EnableWindow(true);
	dlg->SpeechSwFlg = true;
	return 0;
}

UINT CSrobotDlg::SpeechPlay(void *param)
{
	CSrobotDlg* dlg = (CSrobotDlg*)param;
	dlg->SpeechPlayFlg = true;
	if (!dlg->SpeechWord.IsEmpty())//如果合成内容不为空
	{
		dlg->StaSpeechPlay = true;//语音播报状态控制打开
		dlg->TextToSpeech(dlg->SpeechWord);
	}
	dlg->SpeechPlayFlg = false;
	return 0;
}

void CSrobotDlg::OrderAction(const char* myorder)
{
	std::string theorder;
	theorder = myorder;
	OrderSearch ordersearch;
	int Device, Order;
	if (ordersearch.Search(theorder, Device, Order))
	{
		if (Device == 2)
		{
			switch(Order)
			{
			case 0:
				OnBnClickedGymsw();
				break;
			case 1:
				OnBnClickedPiaffesw();
				break;
			case 2:
				OnBnClickedActionres();
				break;
			case 3:
				OnBnClickedStopall();
				break;
			case 4:
				OnBnClickedHugarm();
				break;
			case 5:
				OnBnClickedInvitearm();
				break;
			default:
				break;
			}
		}
		else
		{
			char sendorder = Order;
			if(Device==0)
				PhizModule.WriteToPort(&sendorder, sizeof(sendorder));
			else
				MoveModule.WriteToPort(&sendorder, sizeof(sendorder));
		}
	}
	return;
}


char* CSrobotDlg::SendToXfly()
{
	XunflySpe.Detection();//开始录音并自动根据端点检测抓取
	if (XunflySRSwitchFlg == true)//结合调用这个函数的函数防止点击关闭语音识别时再次识别
		return "抱歉，我没说清楚。";
	XunflySpe.WriteToWav();//保存音频文件
	XunflySpe.AutoRecord_Reset();//参数复位
	XunflySpe.RecoSpere();//将语音上传至讯飞
	char* result;
	result = XunflySpe.GetResult();
	return result;//获取识别结果
}

std::string CSrobotDlg::SendToTuling(char* mytext)
{
	std::string TSpeResult;
	TSpeResult = mytext;
	Tuling.InputText(TSpeResult);
	Tuling.PostToTuling();
	TSpeResult = Tuling.TulingResult();
	return TSpeResult;
}

//void CSrobotDlg::TextToSpeech(CString mytext)
//{
//	::CoInitialize(NULL);//初始化语音环境
//	ISpVoice * pSpVoice = NULL;//初始化语音变量
//	CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_INPROC_SERVER, IID_ISpVoice, (void **)&pSpVoice);
//	//给语音变量创建环境，相当于创建语音变量，FAILED是个宏定义，就是来判断CoCreateInstance这个函数又没有成功创建语音变量，下面是不成功的提示信息。
//
//	std::string temp;
//	temp = (LPCSTR)(CStringA)(mytext);
//	std::wstring wtemp;
//	wtemp = Tuling.StringToWstring(temp);
//
//	pSpVoice->Speak((wtemp.c_str()), SPF_DEFAULT, NULL);
//
//	pSpVoice->Release(); //释放语音变量
//	::CoUninitialize();//释放语音环境
//}

void CSrobotDlg::TextToSpeech(CString mytext)
{
	::CoInitialize(NULL);//初始化语音环境
	ISpVoice * pSpVoice = NULL;//初始化语音变量
	HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_INPROC_SERVER, IID_ISpVoice, (void **)&pSpVoice);
	HRESULT status = true;//初始化状态句柄变量用于监视语音播报
						  //给语音变量创建环境，相当于创建语音变量，FAILED是个宏定义，就是来判断CoCreateInstance这个函数又没有成功创建语音变量，下面是不成功的提示信息。

	std::string temp;
	temp = (LPCSTR)(CStringA)(mytext);
	std::wstring wtemp;
	wtemp = Tuling.StringToWstring(temp);

	CComPtr<ISpObjectToken> pOldToken;
	hr = pSpVoice->GetVoice(&pOldToken);

	// 首先结束当前的朗读。
	hr = pSpVoice->Speak(NULL, SPF_PURGEBEFORESPEAK, 0);
	if (SUCCEEDED(hr))
	{
		hr = pSpVoice->Speak((wtemp.c_str()), SPF_ASYNC | SPF_IS_NOT_XML, 0);//开始朗读
		while (status&&StaSpeechPlay)//语音播报直至播报完成或外部进行打断
		{
			status = pSpVoice->WaitUntilDone(200); //每次等待朗读200ms(若参数为INFINITE即为等待朗读结束)若朗读结束该函数返回0
		}
	}
	pSpVoice->Release(); //释放语音变量
	::CoUninitialize();//释放语音环境
	SpeechWord.Empty();//清空本次合成语句
	StaSpeechPlay = false;
	return;
}

void CSrobotDlg::OnBnClickedAiswitch()
{
	// TODO: Add your control notification handler code here
	CString temp;
	GetDlgItem(IDC_AISwitch)->GetWindowTextW(temp);
	if (temp == "打开智能应答")
	{
		GetDlgItem(IDC_AISwitch)->EnableWindow(false);
		GetDlgItem(IDC_AISwitch)->SetWindowTextW(_T("正在打开应答"));
		GetDlgItem(IDC_AISwitch)->SetWindowTextW(_T("关闭智能应答"));
		GetDlgItem(IDC_AISwitch)->EnableWindow(true);
		GetDlgItem(IDC_SrStatu)->SetWindowTextW(_T("智能应答已打开"));
	}
	else
	{
		GetDlgItem(IDC_AISwitch)->EnableWindow(false);
		GetDlgItem(IDC_AISwitch)->SetWindowTextW(_T("正在关闭应答"));
		AfxBeginThread(AiClose, this, THREAD_PRIORITY_HIGHEST);
	}
}


void CSrobotDlg::OnBnClickedExitall()
{
	// TODO: Add your control notification handler code here
	unsigned char ReValue;
	ReValue = MessageBox(_T("   确定要退出吗？请确保其他功能已关闭!"), _T("退出"), MB_YESNO | MB_ICONQUESTION);
	if (ReValue == 6)
		CDialog::OnOK();
	else 
	{
		return;
	}
	//AfxGetMainWnd()->SendMessage(WM_CLOSE);
	//退出程序用AfxGetMainWnd()->SendMessage(WM_CLOSE);

	//关闭当前窗口用DestroyWindow();

	//关闭模式对话框用EndDialog(0);
}
UINT CSrobotDlg::AiClose(void* param)
{
	CSrobotDlg* dlg = (CSrobotDlg*)param;
	dlg->AiCloseFlg = false;
	dlg->StaSpeechPlay = false;
	while (!(dlg->AISwFlg)) Sleep(200);
	while (dlg->SpeechPlayFlg) Sleep(200);
	dlg->GetDlgItem(IDC_AISwitch)->SetWindowTextW(_T("打开智能应答"));
	dlg->GetDlgItem(IDC_SrStatu)->SetWindowTextW(_T("智能应答已关闭"));
	CString temp;
	dlg->GetDlgItem(IDC_SrSwitch)->GetWindowTextW(temp);
	dlg->GetDlgItem(IDC_AISwitch)->EnableWindow(true);
	dlg->AiCloseFlg = true;
	return 0;
}

void CSrobotDlg::DrawMat(cv::Mat& img, UINT nID)//摄像头显示到界面
{
	cv::Mat imgTmp;
	CRect rect;
	GetDlgItem(nID)->GetClientRect(&rect);  // 获取控件大小
	cv::resize(img, imgTmp, cv::Size(rect.Width(), rect.Height()));// 缩放Mat并备份
																   // 转一下格式 ,这段可以放外面,
	switch (imgTmp.channels())
	{
	case 1:
		cv::cvtColor(imgTmp, imgTmp, CV_GRAY2BGRA); // GRAY单通道
		break;
	case 3:
		cv::cvtColor(imgTmp, imgTmp, CV_BGR2BGRA);  // BGR三通道   若RGB,把case 3那里改一下,改成CV_RGB2BGRA.
		break;
	default:
		break;
	}
	int pixelBytes = imgTmp.channels()*(imgTmp.depth() + 1); // 计算一个像素多少个字节
															 // 制作bitmapinfo(数据头)
	BITMAPINFO bitInfo;
	bitInfo.bmiHeader.biBitCount = 8 * pixelBytes;
	bitInfo.bmiHeader.biWidth = imgTmp.cols;
	bitInfo.bmiHeader.biHeight = -imgTmp.rows;
	bitInfo.bmiHeader.biPlanes = 1;
	bitInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitInfo.bmiHeader.biCompression = BI_RGB;
	bitInfo.bmiHeader.biClrImportant = 0;
	bitInfo.bmiHeader.biClrUsed = 0;
	bitInfo.bmiHeader.biSizeImage = 0;
	bitInfo.bmiHeader.biXPelsPerMeter = 0;
	bitInfo.bmiHeader.biYPelsPerMeter = 0;
	// Mat.data + bitmap数据头 -> MFC
	CDC *pDC = GetDlgItem(nID)->GetDC();
	::StretchDIBits(
		pDC->GetSafeHdc(),
		0, 0, rect.Width(), rect.Height(),
		0, 0, rect.Width(), rect.Height(),
		imgTmp.data,
		&bitInfo,
		DIB_RGB_COLORS,
		SRCCOPY
	);
	ReleaseDC(pDC);
}

void CSrobotDlg::OnBnClickedCamerasw()
{
	// TODO: Add your control notification handler code here
	CString temp;
	GetDlgItem(IDC_CameraSw)->GetWindowTextW(temp);
	if (temp == "打开摄像头")
	{
		ShowCaFlg = true;
		GetDlgItem(IDC_CameraSw)->EnableWindow(false);
		GetDlgItem(IDC_CameraSw)->SetWindowTextW(_T("正在打开摄像头"));
		AfxBeginThread(ShowCamera, this, THREAD_PRIORITY_HIGHEST);
	}
	else
	{
		GetDlgItem(IDC_CameraSw)->EnableWindow(false);
		GetDlgItem(IDC_CameraSw)->SetWindowTextW(_T("正在关闭摄像头"));
		while (!(PostFaceFlg)) Sleep(200);//等待人脸识别语音播报
		ShowCaFlg = false;
	}
}

UINT CSrobotDlg::ShowCamera(void* param)
{
	CSrobotDlg* dlg = (CSrobotDlg*)param;

	std::vector<cv::Rect> faces;
	cv::CascadeClassifier face_cascade;
	cv::String face_cascade_name = "haarcascade_frontalface_alt.xml";
	face_cascade.load(face_cascade_name);

	cv::VideoCapture capture;
	capture.open(0);
	if (!capture.isOpened())
	{
		AfxMessageBox(L"打开摄像头失败，请检查设备");
		dlg->GetDlgItem(IDC_CameraSw)->SetWindowTextW(_T("打开摄像头"));
		dlg->GetDlgItem(IDC_CameraSw)->EnableWindow(true);
		return 0;
	}
	cv::Mat frame;
	cv::Mat flipframe;
	cv::Mat grayImage;

	long secs = 5;
	clock_t delay = secs*CLOCKS_PER_SEC;
	clock_t lastphoto = clock() - delay;
	bool buttontext = true;
	while (dlg->ShowCaFlg)
	{
		capture >> frame;
		grayImage.create(frame.size(), frame.type());
		cv::cvtColor(frame, grayImage, CV_BGR2GRAY);
		face_cascade.detectMultiScale(grayImage, faces, 1.1, 3, 0 | CV_HAAR_DO_ROUGH_SEARCH, cv::Size(100, 100));
		for (size_t i = 0; i < faces.size(); i++)
		{
			if ((clock() - lastphoto > delay)&&(dlg->PostFaceFlg))
			{
				imwrite("faces.jpg", frame);
				lastphoto = clock();
				AfxBeginThread(PostFacepp, dlg, THREAD_PRIORITY_HIGHEST);
			}
			cv::Rect select;//声明矩形  
			select.x = faces[i].x;
			select.y = faces[i].y;
			select.width = faces[i].width;
			select.height = faces[i].height;
			rectangle(frame, select, cv::Scalar(0, 255, 0), 2, 8, 0);//在脸部画矩形：：参数形式void rectangle(Mat& img, Rect rec, const Scalar& color, int thickness=1, int lineType=8, int shift=0 )
			lastphoto = clock();
		}
		cv::flip(frame, flipframe, 1);//opencv3.x的图像翻转函数
		dlg->DrawMat(flipframe, IDC_CameraDis);
		if (buttontext)
		{
			dlg->GetDlgItem(IDC_CameraSw)->SetWindowTextW(_T("关闭摄像头"));
			dlg->GetDlgItem(IDC_CameraSw)->EnableWindow(true);
			buttontext = false;
		}
		cv::waitKey(30);
	}
	dlg->GetDlgItem(IDC_CameraSw)->SetWindowTextW(_T("打开摄像头"));
	dlg->GetDlgItem(IDC_CameraSw)->EnableWindow(true);
	return 0;
}

UINT CSrobotDlg::PostFacepp(void *param)
{
	CSrobotDlg* dlgt = (CSrobotDlg*)param;
	dlgt->PostFaceFlg = false;
	bool ErroFlg;
	Facepp PostFace;
	std::string PostRe;
	ErroFlg = PostFace.FaceDetect("faces.jpg");
	if (ErroFlg) PostRe = PostFace.GetFaPostRe();
	else
	{
		dlgt->PostFaceFlg = true;
		return 0;
	}
	ErroFlg = PostFace.SearchFFSet("faces.jpg", "myface");
	if (ErroFlg)
	{
		CString SayY;
		PostRe = PostFace.GetSearchRe();
		SayY = PostRe.c_str();
		SayY += "很高兴见到您！";
		while (!(dlgt->AISwFlg)) Sleep(200);
		while (dlgt->SpeechPlayFlg) Sleep(200);
		int lastLine = dlgt->m_SrResult.LineIndex(dlgt->m_SrResult.GetLineCount() - 1);
		dlgt->m_SrResult.SetSel(lastLine + 1, lastLine + 2, 1);
		dlgt->m_SrResult.ReplaceSel(L"AI：" + SayY + "\r\n");//在最后一行加新的图灵机器人返回结果
		dlgt->m_SrResult.UpdateWindow();//更新显示，不加这句有时更新有延迟
		dlgt->StaSpeechPlay = true;
		dlgt->TextToSpeech(SayY);
	}
	else
	{
		dlgt->PostFaceFlg = true;
		return 0;
	}
	dlgt->PostFaceFlg = true;
	return 0;
}

void CSrobotDlg::OnBnClickedAdvacedset()
{
	// TODO: Add your control notification handler code here
	CString temp;
	GetDlgItem(IDC_CameraSw)->GetWindowTextW(temp);
	if (!(temp == "打开摄像头"))
		AfxMessageBox(L"请先关闭摄像头");
	else
	{
		CManageSetDlg Dlg;
		Dlg.DoModal();
	}
}

void CSrobotDlg::OnBnClickedActionsw()
{
	// TODO: Add your control notification handler code here
	CString temp;
	GetDlgItem(IDC_ActionSw)->GetWindowTextW(temp);
	if (temp == "打开演示动作")
	{
		if (PhizFlg == false || MoveFlg == false|| ArmFlg==false)
			AfxMessageBox(L"请设置好各串口");
		else
			GetDlgItem(IDC_ActionSw)->SetWindowTextW(_T("关闭演示动作"));
		return;
	}
	else
	{
		GetDlgItem(IDC_ActionSw)->SetWindowTextW(_T("打开演示动作"));
	}
}


void CSrobotDlg::OnBnClickedRelasetorque()
{
	// TODO: Add your control notification handler code here
	if (!ArmFlg)
	{
		AfxMessageBox(_T("请先打开机械臂串口"));
		return;
	}
	OnBnClickedStopall();
	unsigned char sum = 0, ReValue;
	unsigned char UndoTorque[] = { 0xff, 0xff, 0xfe, 4, 0x03, 0x18, 0x00, 0x00 };//舵机指令报包
	for (unsigned char i = 2; i < sizeof(UndoTorque) - 1; i++) sum += UndoTorque[i];
	UndoTorque[sizeof(UndoTorque) - 1] = ~sum;//计算SetTorque的校验码
	sum = 0;
	ReValue = MessageBox(_T("   确定要撤销扭矩吗？"), _T("撤销扭矩"), MB_YESNO | MB_ICONQUESTION);
	if (ReValue == 6)
	{
		ArmModuleF.WriteToPort(UndoTorque, sizeof(UndoTorque));
		ArmModuleS.WriteToPort(UndoTorque, sizeof(UndoTorque));
	}
	return;
}

void CSrobotDlg::OnBnClickedActionres()
{
	// TODO: Add your control notification handler code here
	unsigned char Id_Tx749[] = { 0xff, 0xff, 0xfe, 19, 0x83, 0x1e, 4, 7, 230, 7, 10, 0, 4, 205, 6, 10, 0, 9, 197, 1, 10, 0, 0x00 };
	unsigned char Id_Tx865[] = { 0xff, 0xff, 0xfe, 19, 0x83, 0x1e, 4, 8, 146, 8, 10, 0, 6, 247, 7, 10, 0, 5, 209, 1, 10, 0, 0x00 };
	if (!ArmFlg)
	{
		AfxMessageBox(_T("机械臂串口未连接！"));
		return;
	}
	else
	{
		OnBnClickedStopall();
		unsigned char sum = 0;
		unsigned char Single[] = { 0xff, 0xff, 0x01, 0x07, 0x03, 0x1e, 244, 1, 100, 0, 0x00 };
		unsigned char Original[] = { 0xff, 0xff, 0xfe, 34, 0x83, 0x1e, 4, 7, 230, 7, 100, 0, 4, 205, 6, 100, 0, 9, 197, 1, 100, 0,
			8, 146, 8, 100, 0, 6, 247, 7, 100, 0, 5, 209, 1, 100, 0, 0x00 };
		for (unsigned char i = 2; i < sizeof(Original) - 1; i++) sum += Original[i];
		Original[sizeof(Original) - 1] = ~sum;//计算校验码
		sum = 0;
		for (unsigned char i = 2; i < sizeof(Single) - 1; i++) sum += Single[i];
		Single[sizeof(Single) - 1] = ~sum;//计算校验码
		sum = 0;
		ArmModuleF.WriteToPort(Original, sizeof(Original));//发送指令
		Sleep(10);
		ArmModuleS.WriteToPort(Single, sizeof(Single));//发送指令
	}
	return;
}


void CSrobotDlg::OnBnClickedGymsw()
{
	// TODO: Add your control notification handler code here
	if (!ArmFlg)
	{
		AfxMessageBox(_T("请先打开机械臂串口！"));
		return;
	}
	CString temp;
	GetDlgItem(IDC_PiaffeSw)->GetWindowTextW(temp);
	if (temp == "停止原地踏步")
		OnBnClickedPiaffesw();
	GetDlgItem(IDC_GymSw)->GetWindowTextW(temp);
	if (temp == "开始广播体操")
	{
		SetTimer(1, 2000, NULL);//广播体操是3000
		GetDlgItem(IDC_GymSw)->SetWindowTextW(_T("停止广播体操"));
	}
	else
	{
		KillTimer(1);
		Gymnastics = 0;
		GetDlgItem(IDC_GymSw)->SetWindowTextW(_T("开始广播体操"));
	}
}


void CSrobotDlg::OnBnClickedPiaffesw()
{
	// TODO: Add your control notification handler code here
	if (!ArmFlg)
	{
		AfxMessageBox(_T("请先打开机械臂串口！"));
		return;
	}
	CString temp;
	GetDlgItem(IDC_GymSw)->GetWindowTextW(temp);
	if (temp == "停止广播体操")
		OnBnClickedGymsw();
	GetDlgItem(IDC_PiaffeSw)->GetWindowTextW(temp);
	if (temp == "开始原地踏步")
	{
		SetTimer(2, 1000, NULL);//广播体操是3000
		GetDlgItem(IDC_PiaffeSw)->SetWindowTextW(_T("停止原地踏步"));
	}
	else
	{
		KillTimer(2);
		Gymnastics = 0;
		GetDlgItem(IDC_PiaffeSw)->SetWindowTextW(_T("开始原地踏步"));
	}
}


void CSrobotDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	int n = nIDEvent;
	unsigned char sum = 0;
	unsigned char GymTx[3][38] = {
		{ 0xff, 0xff, 0xfe, 34, 0x83, 0x1e, 0x04, 0x04, 0x9e, 0x0c, 150, 0, 0x05, 0xc2, 0x01, 150, 0,
		0x06, 0xc3, 0x0d, 150, 0, 0x07, 0xd3, 0x07, 150, 0, 0x08, 0xcd, 0x08, 150, 0, 0x09, 0xb6, 0x01, 150, 0, 0x00 },
		{ 0xff, 0xff, 0xfe, 34, 0x83, 0x1e, 0x04, 0x04, 0x06, 0x07, 150, 0, 0x05, 0xc9, 0x01, 150, 0,
		0x06, 0x35, 0x08, 150, 0, 0x07, 0x00, 0x02, 150, 0, 0x08, 0x79, 0x0e, 150, 0, 0x09, 0xc0, 0x01, 150, 0, 0x00 },
		{ 0xff, 0xff, 0xfe, 34, 0x83, 0x1e, 0x04, 0x04, 0x9e, 0x0c, 150, 0, 0x05, 0x95, 0x00, 150, 0,
		0x06, 0xc3, 0x0d, 150, 0, 0x07, 0x00, 0x02, 150, 0, 0x08, 0x69, 0x0e, 150, 0, 0x09, 0x95, 0x00, 150, 0, 0x00 } };
	unsigned char GymTx1[][18] = {
		{ 0xff, 0xff, 0xfe, 14, 0x83, 0x1e, 0x04, 0x07, 0x00, 0x0a, 200, 0, 0x08, 0xa8, 0x0a, 200, 0, 0x00 },
		{ 0xff, 0xff, 0xfe, 14, 0x83, 0x1e, 0x04, 0x07, 0x6a, 0x05, 200, 0, 0x08, 0x6a, 0x06, 200, 0, 0x00 } };
	switch (n)
	{
	case 1:
		for (unsigned char i = 2; i < sizeof(GymTx[Gymnastics]) - 1; i++) sum += GymTx[Gymnastics][i];
		GymTx[Gymnastics][sizeof(GymTx[Gymnastics]) - 1] = ~sum;//计算校验码
		sum = 0;
		ArmModuleF.WriteToPort(GymTx[Gymnastics], sizeof(GymTx[Gymnastics]));//发送指令
		Gymnastics++;
		if (Gymnastics == 3)Gymnastics = 0;
		break;
	case 2:
		for (unsigned char i = 2; i < sizeof(GymTx1[Gymnastics]) - 1; i++) sum += GymTx1[Gymnastics][i];
		GymTx1[Gymnastics][sizeof(GymTx1[Gymnastics]) - 1] = ~sum;//计算校验码
		sum = 0;
		ArmModuleF.WriteToPort(GymTx1[Gymnastics], sizeof(GymTx1[Gymnastics]));//发送指令
		Gymnastics++;
		if (Gymnastics == 2)Gymnastics = 0;
		break;
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CSrobotDlg::OnBnClickedStopall()
{
	// TODO: Add your control notification handler code here
	char sendorder = 0;
	MoveModule.WriteToPort(&sendorder, sizeof(sendorder));
	if (!ArmFlg)
	{
		AfxMessageBox(_T("请先打开机械臂串口！"));
		return;
	}
	unsigned char sum = 0;
	unsigned char STOP[] = { 0xff, 0xff, 0xfe, 5, 0x03, 0x20, 0X01, 0X00, 0x00 };
	for (unsigned char i = 2; i < sizeof(STOP) - 1; i++) sum += STOP[i];
	STOP[sizeof(STOP) - 1] = ~sum;//计算STOP的校验码
	CString temp;
	GetDlgItem(IDC_GymSw)->GetWindowTextW(temp);
	if (temp == "停止广播体操")OnBnClickedGymsw();
	GetDlgItem(IDC_PiaffeSw)->GetWindowTextW(temp);
	if (temp == "停止原地踏步")OnBnClickedPiaffesw();
	ArmModuleF.WriteToPort(STOP, sizeof(STOP));
	ArmModuleS.WriteToPort(STOP, sizeof(STOP));
}


void CSrobotDlg::OnBnClickedHugarm()
{
	// TODO: Add your control notification handler code here
	if (!ArmFlg)
	{
		AfxMessageBox(_T("请先打开机械臂串口！"));
	}
	else
	{
		OnBnClickedStopall();
		unsigned char sum = 0;
		unsigned char Hug[] = { 0xff, 0xff, 0xfe, 34, 0x83, 0x1e, 4, 7, 55, 4, 100, 0, 4, 200, 7, 100, 0, 9, 238, 0, 100, 0,
			8, 55, 12, 100, 0, 6, 15, 9, 100, 0, 5, 249, 0, 100, 0, 0x00 };
		for (unsigned char i = 2; i < sizeof(Hug) - 1; i++) sum += Hug[i];
		Hug[sizeof(Hug) - 1] = ~sum;//计算校验码
		sum = 0;
		ArmModuleF.WriteToPort(Hug, sizeof(Hug));//发送指令
	}
	return;
}


void CSrobotDlg::OnBnClickedInvitearm()
{
	// TODO: Add your control notification handler code here
	if (!ArmFlg||!PhizFlg)
	{
		AfxMessageBox(_T("请先打开机械臂和脸部串口！"));
	}
	else
	{
		OnBnClickedStopall();
		char order = 6;
		PhizModule.WriteToPort(&order, sizeof(order));
		unsigned char sum = 0;
		unsigned char Single[] = { 0xff, 0xff, 0x01, 0x07, 0x03, 0x1e, 104, 1, 100, 0, 0x00 };
		unsigned char Hug[] = { 0xff, 0xff, 0xfe, 34, 0x83, 0x1e, 4, 7, 188, 3, 100, 0, 4, 190, 8, 100, 0, 9, 181, 1, 100, 0,
			8, 146, 8, 100, 0, 6, 247, 7, 100, 0, 5, 209, 1, 100, 0, 0x00 };
		for (unsigned char i = 2; i < sizeof(Hug) - 1; i++) sum += Hug[i];
		Hug[sizeof(Hug) - 1] = ~sum;//计算校验码
		sum = 0;
		for (unsigned char i = 2; i < sizeof(Single) - 1; i++) sum += Single[i];
		Single[sizeof(Single) - 1] = ~sum;//计算校验码
		sum = 0;
		ArmModuleF.WriteToPort(Hug, sizeof(Hug));//发送指令
		ArmModuleS.WriteToPort(Single, sizeof(Single));//发送指令
	}
	return;
}
