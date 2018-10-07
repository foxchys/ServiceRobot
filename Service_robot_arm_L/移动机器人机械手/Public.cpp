#include "stdafx.h"
#include "Public.h"

UINT com[2] = { 0 };
bool PortOpened = false;

CPublic::CPublic() :PortIsOpened(false){}//构造函数串口变量初始化
CPublic::~CPublic()
{
	if (PortIsOpened)Clearcom();
	else return;
}

bool CPublic::InitPort_1()
{
	CString str;
	if (PortIsOpened && com[1]==com[0])
	{
		str.Format(_T("%s %u %s"), _T("串口"), com[0], _T("已连接！"));
		AfxMessageBox(str);
		return true;
	}
	else
	{
		Clearcom();
		DCB	dcb;//DCB是一个结构体
		TCHAR cscom[10];//字符数组
		COMMTIMEOUTS commTimeouts;//这也是一个结构体类型
		
		m_rdos1.Offset = 0;
		m_rdos1.OffsetHigh = 0;
		m_rdos1.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		if (m_rdos1.hEvent == NULL)return false;
		m_wtos1.Offset = 0;
		m_wtos1.OffsetHigh = 0;
		m_wtos1.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		if (m_wtos1.hEvent == NULL)
		{
			CloseHandle(m_rdos1.hEvent);
			return false;
		}
	
		wsprintf(cscom, _T("com%u"), com[0]);	//此处给定串口号，可以修改
	
		m_hcom1 = CreateFile(cscom, GENERIC_READ | GENERIC_WRITE,
			NULL,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
			NULL);
		//打开串口，异步I/O方式
		if (m_hcom1 == INVALID_HANDLE_VALUE)
		{
			str.Format(_T("%s %u %s"), _T("串口"), com[0], _T("被占用！"));
			AfxMessageBox(str);
			return false;
		}//如果串口被占用，程序在这里跳出
		else
		{
			SetCommMask(m_hcom1, EV_RXCHAR);//EV_RXCHAR表示输入缓冲区中已收到数据，即接收到一个字节并放入输入缓冲区
			// 分配设备缓冲区
			SetupComm(m_hcom1, 4096, 4096);
			// 初始化缓冲区中的信息
			PurgeComm(m_hcom1, PURGE_TXABORT | PURGE_RXABORT |
				PURGE_TXCLEAR | PURGE_RXCLEAR);
			// set up for overlapped I/O

			commTimeouts.ReadIntervalTimeout = 100;
			commTimeouts.ReadTotalTimeoutConstant = 100;
			commTimeouts.ReadTotalTimeoutMultiplier = 100;
			commTimeouts.WriteTotalTimeoutConstant = 100;
			commTimeouts.WriteTotalTimeoutMultiplier = 100;//均是ms为单位
			SetCommTimeouts(m_hcom1, &commTimeouts);
		}

		EscapeCommFunction(m_hcom1, SETDTR);//设置高电平
		if (!GetCommState(m_hcom1, &dcb))//读取串口设置
		{
			AfxMessageBox(_T("无法获取串口"));
			//这里通常不会执行，因为如果串口被占用则程序在前面已跳出
			return false;
		}
		else
		{
			dcb.BaudRate = 57600;
			dcb.fBinary = TRUE;
			dcb.fParity = FALSE;
			dcb.StopBits = ONESTOPBIT;
			dcb.ByteSize = 8;
			dcb.Parity = NOPARITY;
			dcb.fOutxCtsFlow = FALSE;
			dcb.fOutxDsrFlow = FALSE;
			dcb.fDtrControl = DTR_CONTROL_DISABLE;
			dcb.fDsrSensitivity = FALSE;
			dcb.fTXContinueOnXoff = FALSE;
			dcb.fOutX = FALSE;
			dcb.fInX = FALSE;
			dcb.fErrorChar = FALSE;
			dcb.fNull = FALSE;
			dcb.fRtsControl = RTS_CONTROL_DISABLE;
			dcb.fAbortOnError = TRUE;
			//参看网页资料http://blog.csdn.net/wangshubo1989/article/details/47746401
		}
		if (!SetCommState(m_hcom1, &dcb))//设置DCB参数
		{
			AfxMessageBox(_T("无法设置串口状态"));
			//这里通常不会执行，因为如果串口被占用则程序在前面已跳出
			return false;
		}
		else
		{
			str.Format(_T("%s %u %s"), _T("串口"), com[0], _T("连接成功！"));
			AfxMessageBox(str, MB_ICONINFORMATION);
			com[1] = com[0]; com[0] = 0;
			PortIsOpened = true;//设置串口标志位
			PortOpened = true;
			return true;
		}		
	}
}

bool CPublic::WriteToPort_1(unsigned char*buff, int length)
{
	BOOL fWriteStat;
	DWORD dwBytesWritten;
	DWORD dwErrorFlags;
	DWORD dwError;
	DWORD dwBytesSent = 0;
	COMSTAT comStat;
	TCHAR szError[128];

	fWriteStat = WriteFile(m_hcom1, buff, length, &dwBytesWritten, &(m_wtos1));

	if (!fWriteStat)
	{
		if (GetLastError() == ERROR_IO_PENDING)
		{
			while (!GetOverlappedResult(m_hcom1, &(m_wtos1), &dwBytesWritten, TRUE))
			{
				dwError = GetLastError();
				if (dwError == ERROR_IO_INCOMPLETE)
				{
					dwBytesSent += dwBytesWritten;
					continue;
				}
				else
				{
					wsprintf(szError, _T("<CE-%u>"), dwError);
					ClearCommError(m_hcom1, &dwErrorFlags, &comStat);
					break;
				}
			}
			dwBytesSent += dwBytesWritten;

			if (dwBytesSent != length)
				wsprintf(szError, _T("\nProbable Write Timeout: Total of %ld bytes sent"), dwBytesSent);
		}
		else
		{
			ClearCommError(m_hcom1, &dwErrorFlags, &comStat);
			return (FALSE);
		}
	}
	return true;
}

bool CPublic::ReceiveFromPort_1(unsigned char*buffer, int BytesToRead)
{
	BOOL       fReadStat;
	COMSTAT    comStat;
	DWORD      dwErrorFlags;
	DWORD      dwLength = BytesToRead;
	DWORD      dwActulLength = 0;
	DWORD      dwError;
	TCHAR      szError[10];

	ClearCommError(m_hcom1, &dwErrorFlags, &comStat);

	if (dwLength > 0)
	{
		fReadStat = ReadFile(m_hcom1, buffer, dwLength, &dwActulLength, &(m_rdos1));
		if (!fReadStat)
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				OutputDebugString(_T("\n\rIO Pending"));
				// 必须等到数据读取完成，由于设置了
				// commTimeOuts.ReadTotalTimeoutConstant变量
				// 这个函数受超时限制
				while (!GetOverlappedResult(m_hcom1, &(m_rdos1), &dwLength, TRUE))
				{
					dwError = GetLastError();
					if (dwError == ERROR_IO_INCOMPLETE)
						continue;
					else
					{
						wsprintf(szError, _T("<CE-%u>"), dwError);
						ClearCommError(m_hcom1, &dwErrorFlags, &comStat);
						break;
					}
				}
			}
			else
			{
				// 发生其他错误
				dwLength = 0;
				ClearCommError(m_hcom1, &dwErrorFlags, &comStat);
			}
		}
	}
	return true;
}

void CPublic::Clearcom()
{
	if (PortIsOpened == true)
	{
		SetCommMask(m_hcom1, 0);
		PurgeComm(m_hcom1, PURGE_TXCLEAR | PURGE_RXCLEAR);
		CloseHandle(m_hcom1);
		m_hcom1 = NULL;
		CloseHandle(m_rdos1.hEvent);
		CloseHandle(m_wtos1.hEvent);
		PortIsOpened = false;
	}
}