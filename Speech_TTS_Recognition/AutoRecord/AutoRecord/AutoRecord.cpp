#include"AutoRecord.h"

#pragma region 类AutoRecord
//静态变量初始化
unsigned char AutoRecord::vda_buffer0[ARFRAGMENT_SIZE] = { 0 };
unsigned char AutoRecord::vda_buffer01[ARFRAGMENT_SIZE] = { 0 };
unsigned char AutoRecord::vda_buffer02[ARFRAGMENT_SIZE] = { 0 };
short AutoRecord::vda_buffer2[ARFRAGMENT_SIZE / 2] = { 0 };
float AutoRecord::vda_buffer1[4] = { 0 };
int AutoRecord::vda_buf_count = 0;
int AutoRecord::vda_count = 0;
volatile int AutoRecord::vda_bos = 0, AutoRecord::vda_eos = 0, AutoRecord::eos_flag = 0;
float AutoRecord::vda_start = 0, AutoRecord::vda_end = 0;
int AutoRecord::sensitivity;

std::vector<unsigned char>AutoRecord::vda_buffer = {};
WAVEHDR AutoRecord::wh[ARFRAGMENT_NUM] = {};
volatile bool AutoRecord::stop = false;

AutoRecord::AutoRecord()
{
	dest_path = "myvioce.wav";//初始化.wav保存路径为默认工程路径下且文件名为“myvioce.wav”
	fp = NULL;
}

AutoRecord::~AutoRecord()
{
	vda_bos = 0; vda_eos = 0, eos_flag = 0;
}

void AutoRecord::WriteFeil(std::string Feilna)
{
	dest_path = Feilna;
}

DWORD AutoRecord::waveInProc(HWAVEIN hwi, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
	LPWAVEHDR pwh = (LPWAVEHDR)dwParam1;
	int temp = ARFRAGMENT_SIZE;
	if ((WIM_DATA == uMsg) && (!vda_eos))
	{
		memcpy(vda_buffer02, vda_buffer01, temp);
		memcpy(vda_buffer01, vda_buffer0, temp);
		memcpy(vda_buffer0, pwh->lpData, temp);
		memcpy(vda_buffer2, pwh->lpData, temp);
		if (!stop)//如果录音停止则不继续添加缓存
		{
			waveInAddBuffer(hwi, pwh, sizeof(WAVEHDR));
		}
		if (vda_count < 10)//求每一帧的平均阈值
		{
			//OutputDebugStringA("开始！\n");
			vda_count++;
			for (int i = 0; i < ARFRAGMENT_SIZE / 2; i++)vda_start += abs(vda_buffer2[i]);
			if (vda_count == 10)
			{
				vda_start /= (vda_count * 4);
				/*vda_end = vda_start * 3;*/
				vda_end = vda_start * 3;
				vda_start *= sensitivity;
				/*****************此处可改阈值的大小****************/
				//sprintf_s(msgbuf, "vda_start=%f\n", vda_start);
				//OutputDebugStringA(msgbuf);
			}
		}
		else
		{
			for (int i = 0; i < 4; i++)vda_buffer1[i] = 0;
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < ARFRAGMENT_SIZE / 2 / 4; j++)
				{
					vda_buffer1[i] += abs(vda_buffer2[j + ARFRAGMENT_SIZE / 2 / 4 * i]);
				}
				/*sprintf_s(msgbuf, "buffer1[%d]=%f\n", i, buffer1[i]);
				OutputDebugStringA(msgbuf);*/
			}
			if ((vda_buffer1[0] > vda_start) && (vda_buffer1[1] > vda_start) && (vda_buffer1[2] > vda_start) && (vda_buffer1[3] > vda_start) && vda_bos == 0)
			{
				vda_bos = 1;
				OutputDebugStringA("检测到前端点\n\n");
				//MCIWndStop(hMCI);
				//PlaySound(NULL, NULL, NULL);
			}
			if (vda_bos)
			{
				unsigned int ls = sizeof(vda_buffer02);
				for (unsigned int i = 0; i < ls; i++) vda_buffer.push_back(vda_buffer02[i]);
				//memcpy(vda_buffer + vda_buf_count, vda_buffer02, temp);//从上上一个缓存区的数据开始保存
				vda_buf_count += temp;

				if ((vda_buffer1[0] < vda_end) && (vda_buffer1[1] < vda_end) && (vda_buffer1[2] < vda_end) && (vda_buffer1[3] < vda_end))
				{
					eos_flag++;
					if (eos_flag == 3)
					{
						vda_eos = 1;
						eos_flag = 0;
						unsigned int ls0 = sizeof(vda_buffer01);
						for (unsigned int i = 0; i < ls; i++) vda_buffer.push_back(vda_buffer01[i]);
						/*memcpy(vda_buffer + vda_buf_count, vda_buffer01, temp);*/
						OutputDebugStringA("检测到后端点\n\n");
					}
				}
			}
		}
	}
	return 0;
}
void AutoRecord::VDA_Initialize()
{
	stop = false;
	int nReturn = waveInGetNumDevs();//定义输入设备的数目
									 //printf("输入设备数目：%d\n", nReturn);

									 //识别输入的设备
	for (int i = 0; i < nReturn; i++)
	{
		WAVEINCAPS wic;  //WAVEINCAPS结构描述波形音频输入设备的能力
		waveInGetDevCaps(i, &wic, sizeof(WAVEINCAPS)); //waveInGetDevCaps功能检索一个给定的波形音频输入设备的能力
													   //printf("#%d\t设备名：%s\n", i, wic.szPname);
	}

	wavform.wFormatTag = WAVE_FORMAT_PCM;  //WAVE_FORMAT_PCM即脉冲编码
	wavform.nChannels = 1;  // 声道
	wavform.nSamplesPerSec = 16000; // 采样频率
	wavform.nAvgBytesPerSec = 16000 * 16 / 8;  // 每秒数据量
	wavform.nBlockAlign = 2;
	wavform.wBitsPerSample = 16; // 样本大小
	wavform.cbSize = 0;  //大小，以字节，附加额外的格式信息WAVEFORMATEX结构

						 //打开录音设备函数
	waveInOpen(&hWaveIn, WAVE_MAPPER, &wavform, (DWORD_PTR)(AutoRecord::waveInProc), 0, CALLBACK_FUNCTION);

	//识别打开的录音设备
	WAVEINCAPS wic;
	waveInGetDevCaps((UINT_PTR)hWaveIn, &wic, sizeof(WAVEINCAPS));
	//printf("打开的输入设备：%s\n", wic.szPname);

	//for (int i = 0; i < ARFRAGMENT_NUM; i++)
	//{
	//	wh[i].lpData = new char[ARFRAGMENT_SIZE];
	//	wh[i].dwBufferLength = ARFRAGMENT_SIZE;
	//	wh[i].dwBytesRecorded = 0;
	//	wh[i].dwUser = NULL;
	//	wh[i].dwFlags = 0;
	//	wh[i].dwLoops = 1;
	//	wh[i].lpNext = NULL;
	//	wh[i].reserved = 0;
	//	//为录音设备准备缓存函数：
	//	waveInPrepareHeader(hWaveIn, &wh[i], sizeof(WAVEHDR));
	//	//给输入设备增加一个缓存：
	//	waveInAddBuffer(hWaveIn, &wh[i], sizeof(WAVEHDR));
	//}
}

void AutoRecord::Detection()
{
	/*********************************** 录音 *************************************/ // Device  

																				   // prepare buffer
																				   //static WAVEHDR wh[FRAGMENT_NUM];
	for (int i = 0; i < ARFRAGMENT_NUM; i++)
	{
		wh[i].lpData = new char[ARFRAGMENT_SIZE];
		wh[i].dwBufferLength = ARFRAGMENT_SIZE;
		wh[i].dwBytesRecorded = 0;
		wh[i].dwUser = NULL;
		wh[i].dwFlags = 0;
		wh[i].dwLoops = 0;
		wh[i].lpNext = NULL;
		wh[i].reserved = 0;

		//为录音设备准备缓存函数：
		waveInPrepareHeader(hWaveIn, &wh[i], sizeof(WAVEHDR));

		//给输入设备增加一个缓存：
		waveInAddBuffer(hWaveIn, &wh[i], sizeof(WAVEHDR));
	}


	// record  
	//初始化裸数据缓存
	vda_buffer.clear();
	vda_buffer.reserve(10);
	vda_eos = 0;//初始化末端标志
	vda_buf_count = 0; //刚开始录音的时候缓冲区的个数初始化为
					   //开始录音函数
					   //MMRESULT waveInStart(  HWAVEIN hwi  );  
	waveInStart(hWaveIn); //开始录音
	OutputDebugStringA("Start to Record...\n");
	//while (vda_buf_count < ARBUFFER_SIZE)
	while (!vda_eos)//直到检测到末端端点才跳出这个while
	{
		if (stop)
			break;
		Sleep(1);
	}

	//printf("Record Over!\n\n");

	OutputDebugStringA("Record Over!\n\n");
	Stop();

}

void AutoRecord::Stop()
{
	stop = true;
	waveInStop(hWaveIn);//waveInStop功能停止的波形音频输入
						//停止录音函数：
						//MMRESULT waveInReset( HWAVEIN hwi ); 
	waveInReset(hWaveIn);//停止录音
						 /*清除缓存函数：*/
						 /*MMRESULT waveInUnprepareHeader( HWAVEIN hwi,LPWAVEHDR pwh, UINT cbwh); */
	for (int i = 0; i < ARFRAGMENT_NUM; i++)
	{
		waveInUnprepareHeader(hWaveIn, &wh[i], sizeof(WAVEHDR));
		delete wh[i].lpData;
	}

}
void AutoRecord::WriteToWav()
{
	fopen_s(&fp, dest_path.c_str(), "wb");
	WavHeader.data_size = vda_buffer.size();
	WavHeader.size_8 = WavHeader.data_size + 32;
	fwrite(&WavHeader, sizeof(WavHeader), 1, fp);
	// 追加RawData
	/*std::cout << (vda_buffer.size()) << std::endl;*/
	fwrite(vda_buffer.data(), vda_buffer.size(), 1, fp);
	//fwrite(vda_buffer.data(),vda_buffer.size(), 1, fp);
	// 写入结束
	fclose(fp);
	OutputDebugStringA("write to .wav over !\n\n");
}
void AutoRecord::AutoRecord_Reset()
{
	vda_buffer.clear();
	stop = false;
	vda_bos = 0, vda_eos = 0, eos_flag = 0;
}
void AutoRecord::AutoRecord_Close()
{
	//关闭录音设备函数：
	//MMRESULT waveInClose( HWAVEIN hwi );
	AutoRecord_Reset();
	waveInClose(hWaveIn);
	vda_buffer.clear();
	vda_count = 0;
}
#pragma endregion
//------------------------------------------------------
