#include"XunFlySpecre.h"
#include<iostream>
SpecRe test;
void RecSpeR();
char* result;
int main()
{
	std::string feilname = "vioce.wav";
	test.WriteFeil(feilname);//设定.wav文件保存名和路径
	test.VDA_Initialize();//录音初始化
	test.XflySR_Initialize();//讯飞识别初始化 
	test.sensitivity = 21;//设置端点检测灵敏度，默认为21
	test.vda_count = 0;//加上这个上句才能在识别的过程中有用

	while (1)
	{
		std::cout << "请讲话：\n";
		RecSpeR();
		std::cout << result << std::endl;
		strcpy(result, "");
	}

	test.AutoRecord_Close();//关闭录音
	test.XflySR_Close();//关闭讯飞识别

	std::cin.get();

}
void RecSpeR()
{ 
	test.Detection();//开始录音并自动根据端点检测抓取
	test.WriteToWav();//保存为.wav文件
	test.AutoRecord_Reset();//参数复位
	test.RecoSpere();//将语音上传至讯飞
	result = test.GetResult();//获取识别结果

}