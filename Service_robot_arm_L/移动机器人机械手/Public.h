#pragma once
//只有Public.cpp和nlDlg.h包含了Public.h，主要是一些串口的配置

class CPublic
{
public:
	CPublic();
	~CPublic();
	/*成员变量*/
	HANDLE        m_hcom1,m_hcom2;//一个通用句柄
	OVERLAPPED    m_rdos1,m_rdos2;//是一个结构体变量
	OVERLAPPED    m_wtos1,m_wtos2;
	bool PortIsOpened;
	/*成员函数（方法）*/
	bool InitPort_1();//串口初始化
	bool InitPort_2();
	bool WriteToPort_1(unsigned char*, int length);//发送数据
	bool WriteToPort_2(unsigned char*, int length);
	bool ReceiveFromPort_1(unsigned char* buffer, int BytesToRead);//接收数据
	bool ReceiveFromPort_2(unsigned char* buffer, int BytesToRead);
	void Clearcom();
};
