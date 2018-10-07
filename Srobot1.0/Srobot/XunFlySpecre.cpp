#include"stdafx.h"
#include"XunFlySpecre.h"



//-----------------------类AutoRecord-------------------
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



//------------------类SpecRe:public AutoRecord----------
#pragma region 类SpecRe:public AutoRecord
SpecRe::SpecRe()
{
	rsec_result[BUFFER_SIZE] = { 0 };//语音识别的结果初始化
	ret = MSP_SUCCESS;
	int	upload_on = 1; //是否上传用户词表,1上传，0不上传
	login_params = "appid = , work_dir = ."; // 登录参数，appid与msc库绑定,请勿随意改动
	session_begin_params = "sub = iat, domain = iat, language = zh_cn, accent = mandarin, sample_rate = 16000, result_type = plain, result_encoding = gb2312";
	//sub: 请求业务类型   domain: 领域   language: 语言  accent:	方言  sample_rate: 音频采样率  result_type: 识别结果格式  result_encoding: 结果编码格式
}
SpecRe::~SpecRe()
{
	//MSPLogout(); //退出登录
	XflySR_Close();
}

void SpecRe::XflySR_Initialize()
{
	VDA_Initialize();
	rsec_result[BUFFER_SIZE] = { 0 };//语音识别的结果初始化
	ret = MSP_SUCCESS;
	int	upload_on = 1; //是否上传用户词表,1上传，0不上传
	login_params = "appid = , , work_dir = ."; // 登录参数，appid与msc库绑定,请勿随意改动
	session_begin_params = "sub = iat, domain = iat, language = zh_cn, accent = mandarin, sample_rate = 16000, result_type = plain, result_encoding = gb2312";
	//sub: 请求业务类型   domain: 领域   language: 语言  accent:	方言  sample_rate: 音频采样率  result_type: 识别结果格式  result_encoding: 结果编码格式
	ret = MSPLogin(NULL, NULL, login_params); //用户登陆。第一个参数是用户名，第二个参数是密码，均传NULL即可，第三个参数是登录参数	
	if (MSP_SUCCESS != ret)
	{
		OutputDebugStringA("MSPLogin failed , Error code :");
		char ssret[100] = {};
		sprintf_s(ssret, 100, "%d", ret);
		OutputDebugStringA(ssret);
		OutputDebugStringA("\n\n");
		MSPLogout(); //登录失败，退出登录
	}
	ret = upload_userwords();//上传用户词组
	if (MSP_SUCCESS != ret)//如果上传用户词组失败
		MSPLogout();//退出登录
}

void SpecRe :: XflySR_Close()
{
	MSPLogout();//退出登录
	AutoRecord_Close();//录音关闭
	ret = MSP_SUCCESS;
	int	upload_on = 1; //是否上传用户词表,1上传，0不上传
	login_params = "appid = , work_dir = ."; // 登录参数，appid与msc库绑定,请勿随意改动
	session_begin_params = "sub = iat, domain = iat, language = zh_cn, accent = mandarin, sample_rate = 16000, result_type = plain, result_encoding = gb2312";
	//sub: 请求业务类型   domain: 领域   language: 语言  accent:	方言  sample_rate: 音频采样率  result_type: 识别结果格式  result_encoding: 结果编码格式
}

int SpecRe::upload_userwords()
{
	char*			userwords = NULL;
	unsigned int	len = 0;
	unsigned int	read_len = 0;
	FILE*			fp = NULL;
	int				ret = -1;

	fp = fopen("userwords.txt", "rb");
	if (NULL == fp)
	{
		OutputDebugStringA("open [userwords.txt] failed! \n\n");
		goto upload_exit;
	}

	fseek(fp, 0, SEEK_END);
	len = ftell(fp); //获取音频文件大小
	fseek(fp, 0, SEEK_SET);

	userwords = (char*)malloc(len + 1);
	if (NULL == userwords)
	{
		OutputDebugStringA("\nout of memory! \n");
		goto upload_exit;
	}

	read_len = fread((void*)userwords, 1, len, fp); //读取用户词表内容
	if (read_len != len)
	{
		OutputDebugStringA("read [userwords.txt] failed!\n");
		goto upload_exit;
	}
	userwords[len] = '\0';

	MSPUploadData("userwords", userwords, len, "sub = uup, dtt = userword", &ret); //上传用户词表
	if (MSP_SUCCESS != ret)
	{
		char ssret[100] = {};
		sprintf_s(ssret, 100, "%d", ret);
		OutputDebugStringA(ssret);
		OutputDebugStringA("\nMSPUploadData failed ! errorCode:");
		OutputDebugStringA(ssret);
		OutputDebugStringA("\n\n");
		goto upload_exit;
	}

upload_exit:
	if (NULL != fp)
	{
		fclose(fp);
		fp = NULL;
	}
	if (NULL != userwords)
	{
		free(userwords);
		userwords = NULL;
	}

	return ret;
}

void SpecRe::run_iat(const char* audio_file, const char* session_begin_params)
{

	const char*		session_id = NULL;
	char			rec_result[BUFFER_SIZE] = { NULL };
	char			hints[HINTS_SIZE] = { NULL }; //hints为结束本次会话的原因描述，由用户自定义
	unsigned int	total_len = 0;
	int				aud_stat = MSP_AUDIO_SAMPLE_CONTINUE;		//音频状态
	int				ep_stat = MSP_EP_LOOKING_FOR_SPEECH;		//端点检测
	int				rec_stat = MSP_REC_STATUS_SUCCESS;			//识别状态
	int				errcode = MSP_SUCCESS;

	FILE*			f_pcm = NULL;
	char*			p_pcm = NULL;
	long			pcm_count = 0;
	long			pcm_size = 0;
	long			read_size = 0;


	if (NULL == audio_file)
		goto iat_exit;

	f_pcm = fopen(audio_file, "rb");
	if (NULL == f_pcm)
	{
		OutputDebugStringA("\nopen userword.txt failed! \n\n");
		goto iat_exit;
	}

	fseek(f_pcm, 0, SEEK_END);
	pcm_size = ftell(f_pcm); //获取音频文件大小 
	fseek(f_pcm, 0, SEEK_SET);

	p_pcm = (char *)malloc(pcm_size);
	if (NULL == p_pcm)
	{
		OutputDebugStringA("\nout of memory! \n\n");
		goto iat_exit;
	}

	read_size = fread((void *)p_pcm, 1, pcm_size, f_pcm); //读取音频文件内容
	if (read_size != pcm_size)
	{
		OutputDebugStringA("read [%s] error!\n\n");
		goto iat_exit;
	}

	OutputDebugStringA("start speech re ...\n\n");
	session_id = QISRSessionBegin(NULL, session_begin_params, &errcode); //听写不需要语法，第一个参数为NULL
	if (MSP_SUCCESS != errcode)//如果错误代码不为0
	{
		OutputDebugStringA("QISRSessionBegin failed! \n\n");
		goto iat_exit;
	}

	while (1)
	{
		//unsigned int len = 10 * FRAME_LEN; // 每次写入200ms音频(16k，16bit)：1帧音频20ms，10帧=200ms。16k采样率的16位音频，一帧的大小为640Byte
		long len = 10 * FRAME_LEN;
		int ret = 0;

		if (pcm_size < 2 * len)
			len = pcm_size;
		if (len <= 0)
			break;

		aud_stat = MSP_AUDIO_SAMPLE_CONTINUE;
		if (0 == pcm_count)
			aud_stat = MSP_AUDIO_SAMPLE_FIRST;

		//printf(">");
		ret = QISRAudioWrite(session_id, (const void *)&p_pcm[pcm_count], len, aud_stat, &ep_stat, &rec_stat);
		if (MSP_SUCCESS != ret)
		{
			OutputDebugStringA("QISRAudioWrite failed! \n\n");
			goto iat_exit;
		}

		pcm_count += (long)len;
		pcm_size -= (long)len;

		if (MSP_REC_STATUS_SUCCESS == rec_stat) //已经有部分听写结果
		{
			const char *rslt = QISRGetResult(session_id, &rec_stat, 0, &errcode);
			if (MSP_SUCCESS != errcode)
			{
				OutputDebugStringA("\nQISRGetResult failed!\n\n");
				goto iat_exit;
			}
			if (NULL != rslt)
			{
				unsigned int rslt_len = strlen(rslt);
				total_len += rslt_len;
				if (total_len >= BUFFER_SIZE)
				{
					OutputDebugStringA("no enough buffer for rec_result !\n\n");
					goto iat_exit;
				}
				strncat(rec_result, rslt, rslt_len);
			}
		}

		if (MSP_EP_AFTER_SPEECH == ep_stat)
			break;
		Sleep(1); //原为200模拟人说话时间间隙。200ms对应10帧的音频
	}
	errcode = QISRAudioWrite(session_id, NULL, 0, MSP_AUDIO_SAMPLE_LAST, &ep_stat, &rec_stat);
	if (MSP_SUCCESS != errcode)
	{
		OutputDebugStringA("QISRAudioWrite failed! \n\n");
		goto iat_exit;
	}

	while (MSP_REC_STATUS_COMPLETE != rec_stat)
	{
		const char *rslt = QISRGetResult(session_id, &rec_stat, 0, &errcode);
		if (MSP_SUCCESS != errcode)
		{
			OutputDebugStringA("QISRGetResult failed, error code: \n\n");
			goto iat_exit;
		}
		if (NULL != rslt)
		{
			unsigned int rslt_len = strlen(rslt);
			total_len += rslt_len;
			if (total_len >= BUFFER_SIZE)
			{
				OutputDebugStringA("no enough buffer for rec_result \n\n");
				goto iat_exit;
			}
			strncat(rec_result, rslt, rslt_len);
		}
		Sleep(150); //防止频繁占用CPU
	}
	OutputDebugStringA("speech re over \n\n");
	
	//printf("%s\n", rec_result);//这个可以改，改成函数返回字符串数组
	if (rec_result[0] == '\0') strcpy_s(rec_result, "抱歉，我没说清楚。");
	strcpy_s(rsec_result, rec_result);//将识别结果拷贝给rsec_result
iat_exit:
	if (NULL != f_pcm)
	{
		fclose(f_pcm);
		f_pcm = NULL;
	}
	if (NULL != p_pcm)
	{
		free(p_pcm);
		p_pcm = NULL;
	}

	QISRSessionEnd(session_id, hints);
	memset(rec_result, 0, sizeof(rec_result));
}

void SpecRe::RecoSpere()
{
	const char* feil_name = dest_path.data();//获得录音文件名
	run_iat(feil_name, session_begin_params);//识别并将识别结果赋给rsec_result[BUFFER_SIZE] 
}

char* SpecRe::GetResult()
{
	return rsec_result;
}
#pragma endregion
//------------------------------------------------------