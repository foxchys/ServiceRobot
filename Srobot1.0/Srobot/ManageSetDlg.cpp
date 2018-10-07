// ManageSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Srobot.h"
#include "ManageSetDlg.h"
#include "afxdialogex.h"


// CManageSetDlg dialog

IMPLEMENT_DYNAMIC(CManageSetDlg, CDialog)

CManageSetDlg::CManageSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MANAGESET_DIALOG, pParent)
{

}

CManageSetDlg::~CManageSetDlg()
{
}

void CManageSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BaudPhiz, m_BaudPhiz);
	DDX_Control(pDX, IDC_ComPhiz, m_ComPhiz);
	DDX_Control(pDX, IDC_ComMove, m_ComMove);
	DDX_Control(pDX, IDC_BaudMove, m_BaudMove);
	DDX_Control(pDX, IDC_BaudArmF, m_BaudArmF);
	DDX_Control(pDX, IDC_BaudArmS, m_BaudArmS);
	DDX_Control(pDX, IDC_ComArmF, m_ComArmF);
	DDX_Control(pDX, IDC_ComArmS, m_ComArmS);
}


BEGIN_MESSAGE_MAP(CManageSetDlg, CDialog)
	ON_BN_CLICKED(IDC_ExitMana, &CManageSetDlg::OnBnClickedExitmana)
	ON_BN_CLICKED(IDC_LensSw, &CManageSetDlg::OnBnClickedLenssw)
	ON_BN_CLICKED(IDC_ChoosePic, &CManageSetDlg::OnBnClickedChoosepic)
	ON_BN_CLICKED(IDC_TakePic, &CManageSetDlg::OnBnClickedTakepic)
	ON_BN_CLICKED(IDC_UploadPic, &CManageSetDlg::OnBnClickedUploadpic)
	ON_BN_CLICKED(IDC_DetecCom, &CManageSetDlg::OnBnClickedDeteccom)
	ON_BN_CLICKED(IDC_SwPhiz, &CManageSetDlg::OnBnClickedSwphiz)
	ON_BN_CLICKED(IDC_SwMove, &CManageSetDlg::OnBnClickedSwmove)
	ON_BN_CLICKED(IDC_SwArm, &CManageSetDlg::OnBnClickedSwarm)
	ON_BN_CLICKED(IDC_ActTorque, &CManageSetDlg::OnBnClickedActtorque)
END_MESSAGE_MAP()


// CManageSetDlg message handlers

BOOL CManageSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	GetDlgItem(IDC_ExitMana)->SetWindowTextW(_T("退出设置"));
	GetDlgItem(IDC_LensSw)->SetWindowTextW(_T("开始拍照"));
	GetDlgItem(IDC_TakePic)->SetWindowTextW(_T("确认信息"));
	GetDlgItem(IDC_ChoosePic)->SetWindowTextW(_T("选择照片"));
	GetDlgItem(IDC_UploadPic)->SetWindowTextW(_T("录入信息"));
	GetDlgItem(IDC_ActTorque)->SetWindowTextW(_T("激活扭矩"));
	GetDlgItem(IDC_TakePic)->EnableWindow(false);
	GetDlgItem(IDC_UploadPic)->EnableWindow(false);
	ShowLeFlg = false;
	GetDlgItem(IDC_SwPhiz)->SetWindowTextW(_T("打开表情串口"));
	PhizModuleIniti();
	GetDlgItem(IDC_SwMove)->SetWindowTextW(_T("打开移动串口"));
	MoveModuleIniti();
	GetDlgItem(IDC_SwArm)->SetWindowTextW(_T("打开机械手臂串口"));
	ArmModuleIniti();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CManageSetDlg::PhizModuleIniti()
{
	CString temp;
	CSrobotDlg *pDlg = (CSrobotDlg*)this->GetParent();//指向父窗口地址的指针。
	//添加比特率的值到表情模块比特率选择combopx控件
	for (int i = 0; i < sizeof(BaudRate) / sizeof(int); i++)
	{
		temp.Format(_T("%d"), BaudRate[i]);
		m_BaudPhiz.AddString((LPCTSTR)temp);
	}
	temp.Format(_T("%d"), 9600);
	m_BaudPhiz.SetCurSel(m_BaudPhiz.FindStringExact(0, temp));//设定表情模块比特率默认选择为9600
	pDlg->PhizModule.Hkey2ComboBox(m_ComPhiz);//检查串口号并赋给表情模块的COM口选择combobox控件
	if (pDlg->PhizFlg)//如果该模块串口已设置，根据上次的设置进行初始化
	{
		temp.Format(_T("%d"), pDlg->PhizInf[1]);
		m_BaudPhiz.SetCurSel(m_BaudPhiz.FindStringExact(0, temp));
		GetDlgItem(IDC_BaudPhiz)->EnableWindow(false);
		CString CTemp;
		CTemp.Format(_T("COM%d"), pDlg->PhizInf[0]);
		m_ComPhiz.SetCurSel(m_ComPhiz.FindStringExact(0, CTemp));
		GetDlgItem(IDC_ComPhiz)->EnableWindow(false);
		GetDlgItem(IDC_SwPhiz)->SetWindowTextW(_T("关闭表情串口"));
	}
	return;
}

void CManageSetDlg::MoveModuleIniti()
{
	CString temp;
	CSrobotDlg *pDlg = (CSrobotDlg*)this->GetParent();//指向父窗口地址的指针。
													  //添加比特率的值到表情模块比特率选择combopx控件
	for (int i = 0; i < sizeof(BaudRate) / sizeof(int); i++)
	{
		temp.Format(_T("%d"), BaudRate[i]);
		m_BaudMove.AddString((LPCTSTR)temp);
	}
	temp.Format(_T("%d"), 9600);
	m_BaudMove.SetCurSel(m_BaudMove.FindStringExact(0, temp));//设定表情模块比特率默认选择为9600
	pDlg->PhizModule.Hkey2ComboBox(m_ComMove);//检查串口号并赋给表情模块的COM口选择combobox控件
	if (pDlg->MoveFlg)//如果该模块串口已设置，根据上次的设置进行初始化
	{
		temp.Format(_T("%d"), pDlg->MoveInf[1]);
		m_BaudMove.SetCurSel(m_BaudMove.FindStringExact(0, temp));
		GetDlgItem(IDC_BaudMove)->EnableWindow(false);
		CString CTemp;
		CTemp.Format(_T("COM%d"), pDlg->MoveInf[0]);
		m_ComMove.SetCurSel(m_ComMove.FindStringExact(0, CTemp));
		GetDlgItem(IDC_ComMove)->EnableWindow(false);
		GetDlgItem(IDC_SwMove)->SetWindowTextW(_T("关闭移动串口"));
	}
	return;
}

void CManageSetDlg::ArmModuleIniti()
{
	CString temp;
	CSrobotDlg *pDlg = (CSrobotDlg*)this->GetParent();//指向父窗口地址的指针。
													  //添加比特率的值到表情模块比特率选择combopx控件
	for (int i = 0; i < sizeof(BaudRate) / sizeof(int); i++)
	{
		temp.Format(_T("%d"), BaudRate[i]);
		m_BaudArmF.AddString((LPCTSTR)temp);
		m_BaudArmS.AddString((LPCTSTR)temp);
	}
	temp.Format(_T("%d"), 9600);
	m_BaudArmF.SetCurSel(m_BaudArmF.FindStringExact(0, temp));//设定机械臂模块F比特率默认选择为9600
	m_BaudArmS.SetCurSel(m_BaudArmS.FindStringExact(0, temp));//设定机械臂模块S比特率默认选择为9600
	pDlg->ArmModuleF.Hkey2ComboBox(m_ComArmF);//检查串口号并赋给机械臂模块F的COM口选择combobox控件
	pDlg->ArmModuleS.Hkey2ComboBox(m_ComArmS);//检查串口号并赋给机械臂模块F的COM口选择combobox控件
	if (pDlg->ArmFlg)//如果该模块串口已设置，根据上次的设置进行初始化
	{
		temp.Format(_T("%d"), pDlg->ArmInf[1]);
		m_BaudArmF.SetCurSel(m_BaudArmF.FindStringExact(0, temp));
		GetDlgItem(IDC_BaudArmF)->EnableWindow(false);
		CString CTemp;
		CTemp.Format(_T("COM%d"), pDlg->ArmInf[0]);
		m_ComArmF.SetCurSel(m_ComArmF.FindStringExact(0, CTemp));
		GetDlgItem(IDC_ComArmF)->EnableWindow(false);

		temp.Format(_T("%d"), pDlg->ArmInf[3]);
		m_BaudArmS.SetCurSel(m_BaudArmS.FindStringExact(0, temp));
		GetDlgItem(IDC_BaudArmS)->EnableWindow(false);
		CTemp.Format(_T("COM%d"), pDlg->ArmInf[2]);
		m_ComArmS.SetCurSel(m_ComArmS.FindStringExact(0, CTemp));
		GetDlgItem(IDC_ComArmS)->EnableWindow(false);
		GetDlgItem(IDC_SwArm)->SetWindowTextW(_T("关闭机械臂串口"));
	}
	return;
}

void CManageSetDlg::OnBnClickedExitmana()
{
	// TODO: Add your control notification handler code here
	CString temp;
	GetDlgItem(IDC_LensSw)->GetWindowTextW(temp);
	if (temp == "拍照" || temp == "正在载入")
	{
		AfxMessageBox(L"请点击“拍照”后点击退出");
	}
	else
		CDialog::OnOK();
}

void CManageSetDlg::OnBnClickedLenssw()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_Called)->EnableWindow(true);
	GetDlgItem(IDC_UploadPic)->EnableWindow(false);
	CString temptext;
	GetDlgItem(IDC_LensSw)->GetWindowTextW(temptext);
	if ((temptext == "开始拍照")||(temptext == "重拍"))
	{
		GetDlgItem(IDC_TakePic)->EnableWindow(false);
		GetDlgItem(IDC_ChoosePic)->EnableWindow(false);
		GetDlgItem(IDC_LensSw)->EnableWindow(false);
		GetDlgItem(IDC_LensSw)->SetWindowTextW(_T("正在载入"));
		ShowLeFlg = true;
		AfxBeginThread(ShowLens, this, THREAD_PRIORITY_HIGHEST);
	}
	else
	{
		GetDlgItem(IDC_LensSw)->EnableWindow(false);
		ShowLeFlg = false;
	}
}

UINT CManageSetDlg::ShowLens(void* param)
{
	CManageSetDlg*dlg = (CManageSetDlg*)param;
	cv::VideoCapture capture;
	capture.open(0);
	if (!capture.isOpened())
	{
		AfxMessageBox(L"打开摄像头失败，请检查设备");
		dlg->GetDlgItem(IDC_LensSw)->SetWindowTextW(_T("开始拍照"));
		dlg->GetDlgItem(IDC_ChoosePic)->EnableWindow(true);
		dlg->GetDlgItem(IDC_LensSw)->EnableWindow(true);
		return 0;
	}
	cv::Mat frame;
	cv::Mat flipframe;
	cv::Mat grayImage;
	bool buttonflg = true;
	while (dlg->ShowLeFlg)
	{
		capture >> frame;
		cv::flip(frame, flipframe, 1);//opencv3.x的图像翻转函数
		dlg->DrawPic(flipframe, IDC_FacePic);
		if (buttonflg)
		{
			dlg->GetDlgItem(IDC_LensSw)->SetWindowTextW(_T("拍照"));
			dlg->GetDlgItem(IDC_LensSw)->EnableWindow(true);
			buttonflg++;
		}
		cv::waitKey(30);
	}
	cv::imwrite("newface.jpg", frame);
	dlg->GetDlgItem(IDC_LensSw)->SetWindowTextW(_T("重拍"));
	dlg->GetDlgItem(IDC_ChoosePic)->EnableWindow(true);
	dlg->GetDlgItem(IDC_LensSw)->EnableWindow(true);
	dlg->GetDlgItem(IDC_TakePic)->EnableWindow(true);
	return 0;
}

void CManageSetDlg::DrawPic(cv::Mat& img, UINT nID)//镜头显示到界面
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
	return;
}

void CManageSetDlg::OnBnClickedChoosepic()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_Called)->EnableWindow(true);
	GetDlgItem(IDC_UploadPic)->EnableWindow(false);
	GetDlgItem(IDC_ChoosePic)->EnableWindow(false);
	GetDlgItem(IDC_ChoosePic)->SetWindowTextW(_T("正在载入"));
	CFileDialog fileDialog(TRUE, L"jpg", L"*.jpg", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"JPG文件(*.jpg)|*.jpg|JPEG文件(*.jpg)|*.jpeg|PNG文件(*.png)|*.png|", NULL);
	//fileDialog.DoModal();//fileDialog.DoModal()=IDOK
	fileDialog.GetOFN().lpstrInitialDir = L"C:\\";
	INT_PTR result = fileDialog.DoModal();
	CString tempname = fileDialog.GetPathName();
	if (tempname == "*.jpg")
	{
		GetDlgItem(IDC_ChoosePic)->SetWindowTextW(_T("选择照片"));
		GetDlgItem(IDC_ChoosePic)->EnableWindow(true);
		return;
	}
	std::string filename = (LPCSTR)(CStringA)(tempname);
	cv::Mat img = cv::imread(filename);
	imwrite("newface.jpg", img);
	DrawPic(img, IDC_FacePic);
	GetDlgItem(IDC_TakePic)->EnableWindow(true);
	GetDlgItem(IDC_ChoosePic)->SetWindowTextW(_T("选择照片"));
	GetDlgItem(IDC_ChoosePic)->EnableWindow(true);
}


void CManageSetDlg::OnBnClickedTakepic()
{
	// TODO: Add your control notification handler code here
	CString UserCalled;
	GetDlgItem(IDC_Called)->GetWindowTextW(UserCalled);
	if (!(UserCalled.GetLength()))
	{
		AfxMessageBox(L"请填写您的称呼");
		return;
	}
	else
	{
		GetDlgItem(IDC_UploadPic)->EnableWindow(true);
		GetDlgItem(IDC_Called)->EnableWindow(false);
		AfxMessageBox(L"信息已确认，请点击录入信息");
	}
}


void CManageSetDlg::OnBnClickedUploadpic()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_ExitMana)->EnableWindow(false);
	GetDlgItem(IDC_UploadPic)->EnableWindow(false);
	GetDlgItem(IDC_LensSw)->EnableWindow(false);
	GetDlgItem(IDC_TakePic)->EnableWindow(false);
	GetDlgItem(IDC_ChoosePic)->EnableWindow(false);
	GetDlgItem(IDC_UploadPic)->SetWindowTextW(_T("正在录入"));
	AfxBeginThread(UploadFace, this, THREAD_PRIORITY_HIGHEST);
}

UINT CManageSetDlg::UploadFace(void* param)
{
	CManageSetDlg*dlg = (CManageSetDlg*)param;
	Facepp upload;
	std::string facetoken;
	std::string name;
	CString namere;
	if (!(upload.FaceDetect("newface.jpg")))
	{
		AfxMessageBox(L"未在图像中检测到人脸，请重新选择图像");
		goto end;
	}
	else
	{
		facetoken = upload.FDetectToken();
		if (upload.SearchFFSet("newface.jpg", "myface"))
		{
			name=upload.GetSearchRe();
			namere = name.c_str();
			namere += "，您的信息已录入请勿重复录入";
			AfxMessageBox(namere);
			goto end;
		}
		dlg->GetDlgItem(IDC_Called)->GetWindowTextW(namere);
		name= (LPCSTR)(CStringA)(namere);
		if (upload.FaceTokenID(facetoken, name))
		{
			if ((upload.FSetAddFace(facetoken, "myface")))
				AfxMessageBox(L"人脸已录入成功");
			else AfxMessageBox(L"录入失败，请检查信息后重试");
		}
		else AfxMessageBox(L"录入失败，请检查信息后重试");
	}
end:
	dlg->GetDlgItem(IDC_UploadPic)->SetWindowTextW(_T("录入信息"));
	dlg->GetDlgItem(IDC_ExitMana)->EnableWindow(true);
	dlg->GetDlgItem(IDC_ChoosePic)->EnableWindow(true);
	dlg->GetDlgItem(IDC_LensSw)->EnableWindow(true);
	dlg->GetDlgItem(IDC_Called)->EnableWindow(true);
	return 0;
}

void CManageSetDlg::OnBnClickedDeteccom()
{
	// TODO: Add your control notification handler code here
	CSrobotDlg *pDlg = (CSrobotDlg*)this->GetParent();//指向父窗口地址的指针
	if ((pDlg->PhizFlg)||(pDlg->MoveFlg)||(pDlg->ArmFlg))
	{
		AfxMessageBox(L"请先关闭所有串口");
		return;
	}
	pDlg->PhizModule.Hkey2ComboBox(m_ComPhiz);//检查串口号并赋给表情模块的COM口选择combobox控件
	pDlg->MoveModule.Hkey2ComboBox(m_ComMove);//检查串口号并赋给移动模块的COM口选择combobox控件
	pDlg->ArmModuleF.Hkey2ComboBox(m_ComArmF);//检查串口号并赋给机械臂模块F的COM口选择combobox控件
	pDlg->ArmModuleS.Hkey2ComboBox(m_ComArmS);//检查串口号并赋给机械臂模块S的COM口选择combobox控件
}

void CManageSetDlg::OnBnClickedSwphiz()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_SwPhiz)->EnableWindow(false);
	CSrobotDlg *pDlg = (CSrobotDlg*)this->GetParent();//指向父窗口地址的指针
	CString temp;
	int com_no, baudrate;
	UpdateData(true);
	m_ComPhiz.GetWindowText(temp);
	temp.Delete(0, 3);
	com_no = _tstoi(temp);
	m_BaudPhiz.GetWindowText(temp);
	baudrate = _tstoi(temp);
	GetDlgItem(IDC_SwPhiz)->GetWindowTextW(temp);
	if (temp == _T("关闭表情串口"))
	{
		m_ComPhiz.EnableWindow(true);
		m_BaudPhiz.EnableWindow(true);
		pDlg->PhizModule.ClosePort();
		pDlg->PhizFlg = false;
		pDlg->PhizInf[0] = 0, pDlg->PhizInf[1] = 0;
		GetDlgItem(IDC_SwPhiz)->SetWindowTextW(_T("打开表情串口"));
	}
	else if (m_ComPhiz.GetCount() > 0)
	{
		if (pDlg->PhizModule.InitPort(this->GetSafeHwnd(), com_no, baudrate))//打开串口
		{
			/*temp.Format(_T("串口 %d 已打开，设定该串口的波特率为 %d\n "), com_no, baudrate);*/
			pDlg->PhizModule.StartMonitoring();
			pDlg->PhizInf[0] = com_no, pDlg->PhizInf[1] = baudrate;//将串口信息记录到主窗口
			m_ComPhiz.EnableWindow(false);
			m_BaudPhiz.EnableWindow(false);
			pDlg->PhizFlg = true;
			GetDlgItem(IDC_SwPhiz)->SetWindowTextW(_T("关闭表情串口"));
		}
		else
			AfxMessageBox(_T("串口打开失败，请检查该串口是否存在或是否已被占用"));
	}
	else
		AfxMessageBox(_T("没有检测到该串口"));
	UpdateData(false);
	GetDlgItem(IDC_SwPhiz)->EnableWindow(true);
}


void CManageSetDlg::OnBnClickedSwmove()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_SwMove)->EnableWindow(false);
	CSrobotDlg *pDlg = (CSrobotDlg*)this->GetParent();//指向父窗口地址的指针
	CString temp;
	int com_no, baudrate;
	UpdateData(true);
	m_ComMove.GetWindowText(temp);
	temp.Delete(0, 3);
	com_no = _tstoi(temp);
	m_BaudMove.GetWindowText(temp);
	baudrate = _tstoi(temp);
	GetDlgItem(IDC_SwMove)->GetWindowTextW(temp);
	if (temp == _T("关闭移动串口"))
	{
		m_ComMove.EnableWindow(true);
		m_BaudMove.EnableWindow(true);
		pDlg->MoveModule.ClosePort();
		pDlg->MoveFlg = false;
		pDlg->MoveInf[0] = 0, pDlg->MoveInf[1] = 0;
		GetDlgItem(IDC_SwMove)->SetWindowTextW(_T("打开移动串口"));
	}
	else if (m_ComMove.GetCount() > 0)
	{
		if (pDlg->MoveModule.InitPort(this->GetSafeHwnd(), com_no, baudrate))//打开串口
		{
			/*temp.Format(_T("串口 %d 已打开，设定该串口的波特率为 %d\n "), com_no, baudrate);*/
			pDlg->MoveModule.StartMonitoring();
			pDlg->MoveInf[0] = com_no, pDlg->MoveInf[1] = baudrate;//将串口信息记录到主窗口
			m_ComMove.EnableWindow(false);
			m_BaudMove.EnableWindow(false);
			pDlg->MoveFlg = true;
			GetDlgItem(IDC_SwMove)->SetWindowTextW(_T("关闭移动串口"));
		}
		else
			AfxMessageBox(_T("串口打开失败，请检查该串口是否存在或是否已被占用"));
	}
	else
		AfxMessageBox(_T("没有检测到该串口"));
	UpdateData(false);
	GetDlgItem(IDC_SwMove)->EnableWindow(true);
}


void CManageSetDlg::OnBnClickedSwarm()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_SwArm)->EnableWindow(false);
	CSrobotDlg *pDlg = (CSrobotDlg*)this->GetParent();//指向父窗口地址的指针
	CString temp;
	int com_noF, baudrateF, com_noS, baudrateS;
	UpdateData(true);
	m_ComArmF.GetWindowText(temp);
	temp.Delete(0, 3);
	com_noF = _tstoi(temp);
	m_BaudArmF.GetWindowText(temp);
	baudrateF = _tstoi(temp);
	m_ComArmS.GetWindowText(temp);
	temp.Delete(0, 3);
	com_noS = _tstoi(temp);
	m_BaudArmS.GetWindowText(temp);
	baudrateS = _tstoi(temp);

	GetDlgItem(IDC_SwArm)->GetWindowTextW(temp);
	if (temp == _T("关闭机械手臂串口"))
	{
		m_ComArmF.EnableWindow(true);
		m_BaudArmF.EnableWindow(true);
		m_ComArmS.EnableWindow(true);
		m_BaudArmS.EnableWindow(true);
		pDlg->ArmModuleF.ClosePort();
		pDlg->ArmModuleS.ClosePort();
		pDlg->ArmFlg = false;
		pDlg->ArmInf[0] = 0, pDlg->ArmInf[1] = 0, pDlg->ArmInf[2] = 0, pDlg->ArmInf[3] = 0;
		GetDlgItem(IDC_SwArm)->SetWindowTextW(_T("打开机械手臂串口"));
	}
	else if (m_ComArmF.GetCount() > 0&& m_ComArmS.GetCount() > 0)
	{
		if (pDlg->ArmModuleF.InitPort(this->GetSafeHwnd(), com_noF, baudrateF)&&
			pDlg->ArmModuleS.InitPort(this->GetSafeHwnd(), com_noS, baudrateS))//打开串口
		{
			/*temp.Format(_T("串口 %d 已打开，设定该串口的波特率为 %d\n "), com_no, baudrate);*/
			pDlg->ArmModuleF.StartMonitoring();
			pDlg->ArmModuleS.StartMonitoring();
			pDlg->ArmInf[0] = com_noF, pDlg->ArmInf[1] = baudrateF,
				pDlg->ArmInf[2] = com_noS, pDlg->ArmInf[3] = baudrateS;//将串口信息记录到主窗口
			m_ComArmF.EnableWindow(false);
			m_BaudArmF.EnableWindow(false);
			m_ComArmS.EnableWindow(false);
			m_BaudArmS.EnableWindow(false);
			pDlg->ArmFlg = true;
			GetDlgItem(IDC_SwArm)->SetWindowTextW(_T("关闭机械手臂串口"));
		}
		else
			AfxMessageBox(_T("串口打开失败，请检查该串口是否存在或是否已被占用"));
	}
	else
		AfxMessageBox(_T("没有检测到该串口"));
	UpdateData(false);
	GetDlgItem(IDC_SwArm)->EnableWindow(true);
}


void CManageSetDlg::OnBnClickedActtorque()
{
	// TODO: Add your control notification handler code here
	CSrobotDlg *pDlg = (CSrobotDlg*)this->GetParent();//指向父窗口地址的指针
	if(!(pDlg->ArmFlg))
	{
		AfxMessageBox(_T("请先打开机械臂串口"));
		return;
	}
	unsigned char sum = 0, ReValue;
	unsigned char SetTorque[] = { 0xff, 0xff, 0xfe, 4, 0x03, 0x18, 0x01, 0x00 };//舵机指令报包
	for (unsigned char i = 2; i < sizeof(SetTorque) - 1; i++) sum += SetTorque[i];
	SetTorque[sizeof(SetTorque)-1] = ~sum;//计算SetTorque的校验码
	sum = 0;
	ReValue = MessageBox(_T("   确定要激活扭矩吗？"), _T("激活扭矩"), MB_YESNO | MB_ICONQUESTION);
	int aaa = sizeof(SetTorque);
	if (ReValue == 6)
	{
		pDlg->ArmModuleF.WriteToPort(SetTorque, sizeof(SetTorque));
		pDlg->ArmModuleS.WriteToPort(SetTorque, sizeof(SetTorque));
	}
	return;
}
