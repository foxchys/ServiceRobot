
// 移动机器人机械手Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "nl.h"
#include "nlDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern UINT com[2];
extern float Lalp[3];
extern float Ralp[3];
extern bool PortOpened;
static int Gymnastics = 0;
CPublic g_cpc;		//CPublic类定义的对象，用于串口初始化以及发送指令
GRNNNetwork  g_grn; //GRNNNetwork类定义的对象，用于实现神经网络的初始化以及一些计算功能

unsigned char Id_Tx749[] = { 0xff, 0xff, 0xfe, 19, 0x83, 0x1e, 4, 7, 230, 7, 10, 0, 4, 205, 6, 10, 0, 9, 197, 1, 10, 0, 0x00 };
unsigned char Id_Tx865[] = { 0xff, 0xff, 0xfe, 19, 0x83, 0x1e, 4, 8, 146, 8, 10, 0, 6, 247, 7, 10, 0, 5, 209, 1, 10, 0, 0x00 };

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD){}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// C移动机器人机械手Dlg 对话框

CnlDlg::CnlDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CnlDlg::IDD, pParent)
	, m_vec(0)
	, m_speed(10)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CnlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, m_1);
	DDX_Control(pDX, IDC_CHECK2, m_2);
	DDX_Control(pDX, IDC_CHECK3, m_3);
	DDX_Text(pDX, IDC_EDIT10, m_speed);
	DDX_Control(pDX, IDC_CHECK4, m_4);
	DDX_Control(pDX, IDC_CHECK5, m_5);
	DDX_Control(pDX, IDC_CHECK6, m_6);
	DDX_Control(pDX, IDC_CHECK7, m_7);
	DDX_Control(pDX, IDC_CHECK8, m_8);
	DDX_Control(pDX, IDC_CHECK9, m_9);
}

BEGIN_MESSAGE_MAP(CnlDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CnlDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CnlDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CnlDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON5, &CnlDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON3, &CnlDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_SETcom, &CnlDlg::OnBnClickedSetcom)
	ON_CONTROL_RANGE(BN_CLICKED,IDC_BUTTON10,IDC_BUTTON12,OnBnClickedTorque)
	ON_BN_CLICKED(IDC_Monitor, &CnlDlg::OnBnClickedMonitor)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_SingleMove, &CnlDlg::OnBnClickedSinglemove)
	ON_BN_CLICKED(IDC_Gym, &CnlDlg::OnBnClickedGym)
	ON_BN_CLICKED(IDC_BUTTON4, &CnlDlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// C移动机器人机械手Dlg 消息处理程序

BOOL CnlDlg::OnInitDialog()
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

	// TODO:  在此添加额外的初始化代码
	//g_cpc.InitPort_1();			//打开串口
	//OnBnClickedButton3();			//神经网络初始化
	return TRUE;					// 除非将焦点设置到控件，否则返回 TRUE
}

void CnlDlg::OnSyscommand(UINT nID, LPARAM lParam)
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

void CnlDlg::OnPaint()
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
HCURSOR CnlDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


/////////////////////////            取消关闭            ////////////////////////////
void CnlDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}

////////////////////////             移动按钮              //////////////////////////
void CnlDlg::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码
	//控制3个舵机
	if (PortOpened == false)AfxMessageBox(_T("串口未连接！"));
	else
	{
		BOOL flag;
		int a;
		char *p;
		char *pp[6];
		unsigned char sum = 0;	
		int alp_4, alp_5, alp_6, alp_7, alp_8, alp_9;

		a = GetDlgItemInt(IDC_EDIT10, &flag, true);
		p = (char*)&a;

		if (a < 1 || a > 1023)
		{
			AfxMessageBox(_T("给定速度超出合理范围，\n请重新赋值！"));
		}
		else
		{		
			alp_7 = int((Lalp[0] + 33) * 4095 / 250);//ID7
			alp_4 = int((Lalp[1] + 110) * 4095 / 250);//ID4
			alp_9 = int((Lalp[2] + 222) * 1023 / 300);//ID9
			alp_8 = int((-Ralp[0] + 224) * 4095 / 250);
			alp_6 = int((-Ralp[1] + 130) * 4095 / 250);
			alp_5 = int((Ralp[2] + 225) * 1023 / 300);

			if (alp_7 > 350 && alp_7 < 3750 && alp_4>1700 && alp_4 < 3500 && alp_9>100 && alp_9 < 800
				&& alp_8>500 && alp_8 < 3900 && alp_6>2000 && alp_6 < 3750 && alp_5>100 && alp_5 < 850)
			{
				pp[0] = (char*)&alp_7;
				pp[1] = (char*)&alp_4;
				pp[2] = (char*)&alp_9;
				pp[3] = (char*)&alp_8;
				pp[4] = (char*)&alp_6;
				pp[5] = (char*)&alp_5;
				//速度赋值
				Id_Tx749[10] = *p;
				Id_Tx749[11] = *(p + 1);
				Id_Tx749[15] = *p;
				Id_Tx749[16] = *(p + 1);
				Id_Tx749[20] = *p;
				Id_Tx749[21] = *(p + 1);
				Id_Tx865[10] = *p;
				Id_Tx865[11] = *(p + 1);
				Id_Tx865[15] = *p;
				Id_Tx865[16] = *(p + 1);
				Id_Tx865[20] = *p;
				Id_Tx865[21] = *(p + 1);
				//位置赋值
				Id_Tx749[8] = *pp[0];
				Id_Tx749[9] = *(pp[0] + 1);
				Id_Tx749[13] = *pp[1];
				Id_Tx749[14] = *(pp[1] + 1);
				Id_Tx749[18] = *pp[2];
				Id_Tx749[19] = *(pp[2] + 1);
				Id_Tx865[8] = *pp[3];
				Id_Tx865[9] = *(pp[3] + 1);
				Id_Tx865[13] = *pp[4];
				Id_Tx865[14] = *(pp[4] + 1);
				Id_Tx865[18] = *pp[5];
				Id_Tx865[19] = *(pp[5] + 1);

				for (unsigned char i = 2; i < sizeof(Id_Tx749) - 1; i++) sum += Id_Tx749[i];
				Id_Tx749[sizeof(Id_Tx749) - 1] = ~sum;//计算校验码
				sum = 0;
				for (unsigned char i = 2; i < sizeof(Id_Tx865) - 1; i++) sum += Id_Tx865[i];
				Id_Tx865[sizeof(Id_Tx865) - 1] = ~sum;//计算校验码
				sum = 0;
				g_cpc.WriteToPort_1(Id_Tx749, sizeof(Id_Tx749));//发送指令	
				g_cpc.WriteToPort_1(Id_Tx865, sizeof(Id_Tx865));
			}
			else AfxMessageBox(_T("神经网络求解的角度\n超出机械臂活动范围！"));
		}
	}
}

///////////////////////////            倒水按钮           /////////////////////////
void CnlDlg::OnBnClickedButton2()
{
	// TODO:  在此添加控件通知处理程序代码
	m_vec = 10;
}

/////////////////////////          神经网络求取工作空间        ////////////////////
void CnlDlg::OnBnClickedButton3()
{
	// TODO:  在此添加控件通知处理程序代码
	CString strtemp;
	int alp_4, alp_5, alp_6, alp_7, alp_8, alp_9;
	static short flag = 0;
	if (flag == 0)
	{
		flag++;
		g_grn.Initcenter();
		GetDlgItem(IDC_BUTTON3)->SetWindowTextW(_T("求解关节目标角度"));
	}//网络初始化
	else
	{
		CString str[6];
		GetDlgItemText(IDC_EDIT16, str[0]);
		GetDlgItemText(IDC_EDIT17, str[1]);
		GetDlgItemText(IDC_EDIT18, str[2]);//获取期望的坐标值
		g_grn.Calculate_Left(_tstof(str[0]), _tstof(str[1]), _tstof(str[2]));//_tstof(CString->double)
		GetDlgItemText(IDC_EDIT20, str[3]);
		GetDlgItemText(IDC_EDIT21, str[4]);
		GetDlgItemText(IDC_EDIT22, str[5]);//获取期望的坐标值
		g_grn.Calculate_Right(_tstof(str[3]), _tstof(str[4]), _tstof(str[5]));//_tstof(CString->double)
		//神经网络输出角度
		alp_7 = int((Lalp[0] + 33) * 4095 / 250);//ID7
		alp_4 = int((Lalp[1] + 110) * 4095 / 250);//ID4
		alp_9 = int((Lalp[2] + 222) * 1023 / 300);//ID9
		alp_8 = int((-Ralp[0] + 224) * 4095 / 250);
		alp_6 = int((-Ralp[1] + 130) * 4095 / 250);
		alp_5 = int((Ralp[2] + 225) * 1023 / 300);
		strtemp.Format(_T("%d"), alp_7);
		GetDlgItem(IDC_EDIT13)->SetWindowTextW(strtemp);
		strtemp.Format(_T("%d"), alp_4);
		GetDlgItem(IDC_EDIT14)->SetWindowTextW(strtemp);
		strtemp.Format(_T("%d"), alp_9);
		GetDlgItem(IDC_EDIT15)->SetWindowTextW(strtemp);
		strtemp.Format(_T("%d"), alp_8);
		GetDlgItem(IDC_EDIT23)->SetWindowTextW(strtemp);
		strtemp.Format(_T("%d"), alp_6);
		GetDlgItem(IDC_EDIT24)->SetWindowTextW(strtemp);
		strtemp.Format(_T("%d"), alp_5);
		GetDlgItem(IDC_EDIT25)->SetWindowTextW(strtemp);

		if (_isnan(double(Lalp[0])) && _isnan(double(Ralp[0])))//_isnan()函数返回true表示alp[0]不是数字，返回false表示alp[0]是数字
		{
			MessageBox(_T("两臂均无解！"), _T("神经网络求解结果"), MB_ICONWARNING);
		}
		else if (_isnan(double(Lalp[0])))MessageBox(_T("左臂无解！"), _T("神经网络求解结果"), MB_ICONWARNING);
		else if (_isnan(double(Ralp[0])))MessageBox(_T("右臂无解！"), _T("神经网络求解结果"), MB_ICONWARNING);
		else
		{
			strtemp.Format(_T("Lalp[0]=%g\nLalp[1]=%g\nLalp[2]=%g\nRalp[0]=%g\nRalp[1]=%g\nRalp[2]=%g"),
				Lalp[0], Lalp[1], Lalp[2], Ralp[0], Ralp[1], Ralp[2]);
			MessageBox(strtemp, _T("神经网络求解结果"), MB_ICONASTERISK);
		}
	}
}

/////////////////////////////                复位按钮                  /////////////////////
void CnlDlg::OnBnClickedButton5()
{
	// TODO:  在此添加控件通知处理程序代码
	if (!PortOpened)AfxMessageBox(_T("串口未连接！"));
	else
	{
		int a;
		BOOL flag;
		char *p;	//速度指针
		unsigned char sum = 0;
		a = GetDlgItemInt(IDC_EDIT10, &flag, true);
		p = (char*)&a;
		if (a < 1 || a > 1023)
		{
			AfxMessageBox(_T("给定速度超出合理范围，\n请重新赋值！"));
		}
		else
		{
			//速度赋值
			Id_Tx749[10] = *p;
			Id_Tx749[11] = *(p + 1);
			Id_Tx749[15] = *p;
			Id_Tx749[16] = *(p + 1);
			Id_Tx749[20] = *p;
			Id_Tx749[21] = *(p + 1);
			Id_Tx865[10] = *p;
			Id_Tx865[11] = *(p + 1);
			Id_Tx865[15] = *p;
			Id_Tx865[16] = *(p + 1);
			Id_Tx865[20] = *p;
			Id_Tx865[21] = *(p + 1);
			//位置赋值
			Id_Tx749[8] = 230;//ID7
			Id_Tx749[9] = 7;
			Id_Tx749[13] = 205;//ID4
			Id_Tx749[14] = 6;
			Id_Tx749[18] = 197;//ID9
			Id_Tx749[19] = 1;
			Id_Tx865[8] = 146;//ID8
			Id_Tx865[9] = 8;
			Id_Tx865[13] = 247;//ID6
			Id_Tx865[14] = 7;
			Id_Tx865[18] = 209;//ID5
			Id_Tx865[19] = 1;
			for (unsigned char i = 2; i < sizeof(Id_Tx749) - 1; i++) sum += Id_Tx749[i];
			Id_Tx749[sizeof(Id_Tx749) - 1] = ~sum;//计算校验码
			sum = 0;
			for (unsigned char i = 2; i < sizeof(Id_Tx865) - 1; i++) sum += Id_Tx865[i];
			Id_Tx865[sizeof(Id_Tx865) - 1] = ~sum;//计算校验码
			sum = 0;
			g_cpc.WriteToPort_1(Id_Tx749, sizeof(Id_Tx749));//发送指令	
			g_cpc.WriteToPort_1(Id_Tx865, sizeof(Id_Tx865));
		}
	}
}


void CnlDlg::OnBnClickedSetcom()//设置串口
{
	// TODO:  在此添加控件通知处理程序代码
	BOOL flag;
	com[0] = GetDlgItemInt(IDC_EDIT19, &flag, true);
	g_cpc.InitPort_1();
}


void CnlDlg::OnBnClickedTorque(UINT nID)
{
	if (PortOpened == false)AfxMessageBox(_T("串口未连接！"));
	else
	{
		int ID = nID - IDC_BUTTON10;
		unsigned char sum = 0, ReValue;//消息框的返回值
		unsigned char STOP[] = { 0xff, 0xff, 0xfe, 5, 0x03, 0x20, 0X01, 0X00, 0x00 };
		unsigned char SetTorque[] = { 0xff, 0xff, 0xfe, 4, 0x03, 0x18, 0x01, 0x00 };
		unsigned char UndoTorque[] = { 0xff, 0xff, 0xfe, 4, 0x03, 0x18, 0x00, 0xe2 };
		for (unsigned char i = 2; i < sizeof(UndoTorque) - 1; i++) sum += UndoTorque[i];
		UndoTorque[sizeof(UndoTorque) - 1] = ~sum;//计算SetTorque的校验码
		sum = 0;
		for (unsigned char i = 2; i < sizeof(SetTorque) - 1; i++) sum += SetTorque[i];
		SetTorque[sizeof(SetTorque) - 1] = ~sum;//计算SetTorque的校验码
		sum = 0;
		for (unsigned char i = 2; i < sizeof(STOP) - 1; i++) sum += STOP[i];
		STOP[sizeof(STOP) - 1] = ~sum;//计算STOP的校验码
		sum = 0;

		switch (ID)
		{
		case 0:
			ReValue = MessageBox(_T("   确定要撤销扭矩吗？"), _T("撤销扭矩"), MB_YESNO | MB_ICONQUESTION);
			if (ReValue == 6)g_cpc.WriteToPort_1(UndoTorque, sizeof(UndoTorque));
			ReValue = 0; break;
		case 1:
			ReValue = MessageBox(_T("   确定要激活扭矩吗？"), _T("激活扭矩"), MB_YESNO | MB_ICONQUESTION);
			if (ReValue == 6)g_cpc.WriteToPort_1(SetTorque, sizeof(SetTorque));
			ReValue = 0; break;
		case 2:
			g_cpc.WriteToPort_1(STOP, sizeof(STOP));
			ReValue = MessageBox(_T("   舵机已停止运行！\n    是否撤销扭矩？"), _T("急停！！！"), MB_YESNO | MB_ICONSTOP);
			if (ReValue == 6)g_cpc.WriteToPort_1(UndoTorque, sizeof(UndoTorque));
			ReValue = 0; break;
		default:
			break;
		}
	}
}

void CnlDlg::OnBnClickedMonitor()
{
	// TODO:  在此添加控件通知处理程序代码
	/*if (PortOpened == false)AfxMessageBox(_T("请先打开串口！"));
	else SetTimer(1, 100, NULL);*/
	if (!PortOpened)AfxMessageBox(_T("请先打开串口！"));
	else
	{
		if (ThreadFlag)
		{
			ThreadFlag = false;
			GetDlgItem(IDC_Monitor)->SetWindowTextW(_T("打开实时监测"));
		}
		else
		{
			ThreadFlag = true;
			AfxBeginThread(Thread1, this);
			GetDlgItem(IDC_Monitor)->SetWindowTextW(_T("关闭实时监测"));
		}
	}
}


UINT CnlDlg::Thread1(void*param)
{
	//线程函数
	CnlDlg*dlg = (CnlDlg*)param;
	CEdit*ID_1 = (CEdit*)dlg->GetDlgItem(IDC_EDIT1);
	CEdit*ID_2 = (CEdit*)dlg->GetDlgItem(IDC_EDIT2);
	CEdit*ID_3 = (CEdit*)dlg->GetDlgItem(IDC_EDIT3);
	CEdit*ID_4 = (CEdit*)dlg->GetDlgItem(IDC_EDIT4);
	CEdit*ID_5 = (CEdit*)dlg->GetDlgItem(IDC_EDIT5);
	CEdit*ID_6 = (CEdit*)dlg->GetDlgItem(IDC_EDIT6);
	CEdit*ID_7 = (CEdit*)dlg->GetDlgItem(IDC_EDIT7);
	CEdit*ID_8 = (CEdit*)dlg->GetDlgItem(IDC_EDIT8);
	CEdit*ID_9 = (CEdit*)dlg->GetDlgItem(IDC_EDIT9);
	CString str;
	unsigned char sum = 0;
	unsigned char StateBag[9][9];
	unsigned char READ[] = { 0xff, 0xff, 0x01, 0x04, 0x02, 0x24, 0x02, 0x00 };
	while (dlg->ThreadFlag)
	{
		if (com[1] == 1)
		{
			for (unsigned char i = 3; i < 9; i++)
			{
				READ[2] = i + 1;
				for (unsigned char ii = 2; ii < sizeof(READ) - 1; ii++) sum += READ[ii];
				READ[sizeof(READ) - 1] = ~sum;
				sum = 0;
				g_cpc.WriteToPort_1(READ, sizeof(READ));
				g_cpc.ReceiveFromPort_1(StateBag[i], 9);
			}
			str.Format(_T("%d"), StateBag[3][5] + 256 * StateBag[3][6]);
			ID_4->SetWindowTextW(str);
			str.Format(_T("%d"), StateBag[4][5] + 256 * StateBag[4][6]);
			ID_5->SetWindowTextW(str);
			str.Format(_T("%d"), StateBag[5][5] + 256 * StateBag[5][6]);
			ID_6->SetWindowTextW(str);
			str.Format(_T("%d"), StateBag[6][5] + 256 * StateBag[6][6]);
			ID_7->SetWindowTextW(str);
			str.Format(_T("%d"), StateBag[7][5] + 256 * StateBag[7][6]);
			ID_8->SetWindowTextW(str);
			str.Format(_T("%d"), StateBag[8][5] + 256 * StateBag[8][6]);
			ID_9->SetWindowTextW(str);
		}
		if (com[1] == 4)
		{
			for (unsigned char i = 0; i < 3; i++)
			{
				READ[2] = i + 1;
				for (unsigned char ii = 2; ii < sizeof(READ) - 1; ii++) sum += READ[ii];
				READ[sizeof(READ) - 1] = ~sum;
				sum = 0;
				g_cpc.WriteToPort_1(READ, sizeof(READ));
				g_cpc.ReceiveFromPort_1(StateBag[i], 9);
			}
			str.Format(_T("%d"), StateBag[0][5] + 256 * StateBag[0][6]);
			ID_1->SetWindowTextW(str);
			str.Format(_T("%d"), StateBag[1][5] + 256 * StateBag[1][6]);
			ID_2->SetWindowTextW(str);
			str.Format(_T("%d"), StateBag[2][5] + 256 * StateBag[2][6]);
			ID_3->SetWindowTextW(str);
		}
	}
	return 0;
}


void CnlDlg::OnBnClickedSinglemove()
{
	// TODO:  在此添加控件通知处理程序代码
	int a, b, num = 0;
	BOOL flag1, flag2, error = 0;
	char *p,*pp;	//速度、位置指针
	unsigned char sum = 0;
	unsigned char Single[] = { 0xff, 0xff, 0x01, 0x07, 0x03, 0x1e, 0, 0, 0x10, 0, 0x00 };
	a = GetDlgItemInt(IDC_EDIT10, &flag1, true);
	p = (char*)&a;
	b = GetDlgItemInt(IDC_EDIT12, &flag2, true);
	pp = (char*)&b;
	
	if ( a>0&&a<1023 )
	{
		if (m_1.GetCheck())
		{
			if (b < 0 || b>1023)error = 1;
			Single[2] = 1;
			GetDlgItem(IDC_EDIT11)->SetWindowTextW(_T("0~1023"));
			num++;
		}
		if (m_2.GetCheck())
		{
			if (b < 0 || b>1023)error = 1;
			Single[2] = 2;
			GetDlgItem(IDC_EDIT11)->SetWindowTextW(_T("0~1023"));
			num++;
		}
		if (m_3.GetCheck())
		{
			if (b < 0 || b>1023)error = 1;
			Single[2] = 3;
			GetDlgItem(IDC_EDIT11)->SetWindowTextW(_T("0~1023"));
			num++;
		}
		if (m_4.GetCheck())
		{
			if (b < 1700 || b>3500)error = 1;
			Single[2] = 4;
			GetDlgItem(IDC_EDIT11)->SetWindowTextW(_T("1700~3500"));
			num++;
		}
		if (m_5.GetCheck())
		{
			if (b < 100 || b>850)error = 1;
			Single[2] = 5;
			GetDlgItem(IDC_EDIT11)->SetWindowTextW(_T("100~850"));
			num++;
		}
		if (m_6.GetCheck())
		{
			if (b < 2000 || b>3750)error = 1;
			Single[2] = 6;
			GetDlgItem(IDC_EDIT11)->SetWindowTextW(_T("2000~3750"));
			num++;
		}
		if (m_7.GetCheck())
		{
			if (b < 350 || b>3750)error = 1;
			Single[2] = 7;
			GetDlgItem(IDC_EDIT11)->SetWindowTextW(_T("350~3750"));
			num++;
		}
		if (m_8.GetCheck())
		{
			if (b < 500 || b>3900)error = 1;
			Single[2] = 8;
			GetDlgItem(IDC_EDIT11)->SetWindowTextW(_T("500~3900"));
			num++;
		}
		if (m_9.GetCheck())
		{
			if (b < 100 || b>800)error = 1;
			Single[2] = 9;
			GetDlgItem(IDC_EDIT11)->SetWindowTextW(_T("100~800"));
			num++;
		}
		if (num == 1)
		{	
			if (error == 0)
			{
				Single[8] = *p;
				Single[9] = *(p + 1);
				Single[6] = *pp;
				Single[7] = *(pp + 1);
				for (unsigned char i = 2; i < sizeof(Single) - 1; i++) sum += Single[i];
				Single[sizeof(Single) - 1] = ~sum;
				sum = 0;
				g_cpc.WriteToPort_1(Single, sizeof(Single));
			}
			else AfxMessageBox(_T("给定角度超出该舵机活动范围，\n\n\t请重新赋值！"));
		}
		else AfxMessageBox(_T("请选择 1 个舵机ID！"));
		num = 0;
		error = 0;
	}
	else AfxMessageBox(_T("给定速度超出合理范围，\n请重新赋值！"));
}


void CnlDlg::OnBnClickedGym()
{
	// TODO:  在此添加控件通知处理程序代码
	if (!PortOpened)AfxMessageBox(_T("请先打开串口！"));
	else
	{
		if (TimerFlag)
		{
			KillTimer(1);
			TimerFlag = false;
			GetDlgItem(IDC_Gym)->SetWindowTextW(_T("开始广播体操"));
		}
		else
		{
			KillTimer(2);
			TimerFlag = true;
			Gymnastics = 0;
			SetTimer(1, 2000, NULL);//广播体操是3000
			GetDlgItem(IDC_Gym)->SetWindowTextW(_T("停止广播体操"));
		}
	}
}


void CnlDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
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
		g_cpc.WriteToPort_1(GymTx[Gymnastics], sizeof(GymTx[Gymnastics]));//发送指令
		Gymnastics++;
		if (Gymnastics == 3)Gymnastics = 0;
		break;
	case 2:
		for (unsigned char i = 2; i < sizeof(GymTx1[Gymnastics]) - 1; i++) sum += GymTx1[Gymnastics][i];
		GymTx1[Gymnastics][sizeof(GymTx1[Gymnastics]) - 1] = ~sum;//计算校验码
		sum = 0;
		g_cpc.WriteToPort_1(GymTx1[Gymnastics], sizeof(GymTx1[Gymnastics]));//发送指令
		Gymnastics++;
		if (Gymnastics == 2)Gymnastics = 0;
		break;
	default:
		break;
	}
}


void CnlDlg::OnBnClickedButton4()
{
	// TODO:  在此添加控件通知处理程序代码
	if (!PortOpened)AfxMessageBox(_T("请先打开串口！"));
	else
	{
		if (TimerFlag)
		{
			KillTimer(2);
			TimerFlag = false;
			GetDlgItem(IDC_BUTTON4)->SetWindowTextW(_T("原地踏步"));
		}
		else
		{
			KillTimer(1);
			TimerFlag = true;
			Gymnastics = 0;
			SetTimer(2, 1000, NULL);//广播体操是3000
			GetDlgItem(IDC_BUTTON4)->SetWindowTextW(_T("停！"));
		}
	}
}
