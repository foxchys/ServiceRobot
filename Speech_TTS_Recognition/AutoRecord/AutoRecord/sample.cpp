#include "AutoRecord.h"
#include<iostream>

AutoRecord tes;
void recorddd();

int main()
{
	std::string feilname = "vioce.wav";
	tes.sensitivity = 10;//设置端点检测灵敏度，默认为21
	tes.vda_count = 0;//辅助设置端点检测灵敏度
	tes.WriteFeil(feilname);//设定.wav文件保存名和路径
	tes.VDA_Initialize();//录音初始化
	std::cout << "录音开始\n";
	recorddd();
	std::cout << "已完成一次录音,按回车键进行第二次录音\n";
	std::cin.get();
	std::cout << "第二次录音开始\n";
	recorddd();
	std::cout << "第二次录音结束,若关闭设备请";
	system("pause");
	std::cout << "设备已关闭，";
	system("pause");
}
void recorddd()
{
	tes.Detection();//开始录音并自动根据端点检测抓取
	tes.WriteToWav();//保存为.wav文件
	tes.AutoRecord_Reset();//参数复位
}
