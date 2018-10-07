#pragma once
#ifndef _AutoRecord_H_
#define _AutoRecord_H_

#include "stdlib.h"
#include <cmath>
#include <stdio.h> 
#include <windows.h>  
#include <mmsystem.h>  


#include<string>
#include<vector>

#define ARBUFFER_SIZE (16000*16/8*30)    // 录制声音长度
#define ARFRAGMENT_SIZE 1024              // 缓存区大小
#define ARFRAGMENT_NUM 2                  // 缓存区个数 


//static unsigned char vda_buffer[BUFFER_SIZE] = { 0 };
#pragma region 录音头文件和宏
#include <stdlib.h>
#include <cmath>
#include <stdio.h> 
#include <windows.h>  
#include <mmsystem.h>  
#pragma comment(lib, "winmm.lib")

#include<string>
#include<vector>

#define ARBUFFER_SIZE (16000*16/8*30)    // 录制声音长度
#define ARFRAGMENT_SIZE 1024              // 缓存区大小
#define ARFRAGMENT_NUM 2                  // 缓存区个数 
#pragma endregion
//--------------------------------------------


//static unsigned char vda_buffer[BUFFER_SIZE] = { 0 };
class AutoRecord//录音类
{
#pragma region 公有
public://---------------------------------------------------------------
	AutoRecord();
	virtual ~AutoRecord();
	static std::vector<unsigned char> vda_buffer;//采集到的音频的缓存区
	static unsigned char vda_buffer0[ARFRAGMENT_SIZE];
	static unsigned char vda_buffer01[ARFRAGMENT_SIZE];
	static unsigned char vda_buffer02[ARFRAGMENT_SIZE];
	static short vda_buffer2[ARFRAGMENT_SIZE / 2];
	static float vda_buffer1[4];
	static int vda_buf_count;
	static int vda_count;
	static volatile int vda_bos, vda_eos, eos_flag;//vda_bos为端点首端标志 vda_eos为端点末端标志
	static float vda_start, vda_end;
	static int sensitivity;
	static volatile bool stop;//是否触发停止
	std::string dest_path;								// 存储路径
	static WAVEHDR wh[ARFRAGMENT_NUM];

	HWAVEIN hWaveIn;//波形音频数据格式Wave_audio数据格式
	WAVEFORMATEX wavform;//WAVEFORMATEX结构定义了波形音频数据格式。包括在这个结构中唯一的格式信息，共同所有波形音频数据格式。对于需要额外的信息的格式，这个结构包含在另一个结构的第一个成员，以及与其他信息

	void VDA_Initialize();////////////////////==============录音初始化函数=============///////////////////////////
	static DWORD(CALLBACK waveInProc)(HWAVEIN hwi, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2);//录音回掉函数
	void WriteFeil(std::string Feilna);
	void Detection();//自动录音
	void Stop();//停止录音
	void WriteToWav();//保存为.wav
	void AutoRecord_Reset();//复位参数
	void AutoRecord_Close();//关闭设备
#pragma endregion

private://-------------------------------------------------------------
#pragma region 私有
	typedef struct WAVEPCMHDR
	{
		char            riff[4];					// = "RIFF"
		UINT32			size_8;						// = FileSize - 8
		char            wave[4];					// = "WAVE"
		char            fmt[4];						// = "fmt "
		UINT32			fmt_size;					// = PCMWAVEFORMAT的大小 : 
													//PCMWAVEFORMAT
		UINT16	        format_tag;					// = PCM : 1
		UINT16	        channels;					// = 通道数 : 1
		UINT32			samples_per_sec;			// = 采样率 : 8000 | 6000 | 11025 | 16000
		UINT32			avg_bytes_per_sec;			// = 每秒平均字节数 : samples_per_sec * bits_per_sample / 8
		UINT16		    block_align;				// = 每采样点字节数 : wBitsPerSample / 8
		UINT16			bits_per_sample;			// = 量化精度: 8 | 16
		char            data[4];					// = "data";
													//DATA
		UINT32			data_size;					// = 裸数据长度 
	} WAVEPCMHDR;
	/* 默认wav音频头部数据 */
	WAVEPCMHDR WavHeader =
	{
		{ 'R', 'I', 'F', 'F' },
		0,
		{ 'W', 'A', 'V', 'E' },
		{ 'f', 'm', 't', ' ' },
		sizeof(PCMWAVEFORMAT) ,
		WAVE_FORMAT_PCM,
		1,
		16000,
		16000 * (16 / 8),
		16 / 8,
		16,
		{ 'd', 'a', 't', 'a' },
		0
	};
	/* wav音频裸数据放在公有变量 */
	FILE* fp;								// wave文件指针

};

#endif
#pragma endregion
