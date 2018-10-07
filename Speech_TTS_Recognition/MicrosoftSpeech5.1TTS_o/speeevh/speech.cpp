#include <sapi.h>
#include <sphelper.h>
#include <spuihelp.h>
#pragma comment(lib,"ole32.lib")   //CoInitialize CoCreateInstance需要调用ole32.dll
#pragma comment(lib,"sapi.lib")    //sapi.lib在SDK的lib目录,必需正确配置

#include<iostream>

int main()
{
	::CoInitialize(NULL);//初始化语音环境
	ISpVoice * pSpVoice = NULL;//初始化语音变量
	CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_INPROC_SERVER, IID_ISpVoice, (void **)&pSpVoice);
		//给语音变量创建环境，相当于创建语音变量，FAILED是个宏定义，就是来判断CoCreateInstance这个函数又没有成功创建语音变量，下面是不成功的提示信息。

	pSpVoice->Speak(L"speek english有一位著名女舞蹈家给萧伯纳写了一封热情洋溢的信，信中建议：如果让他俩结婚，那将对后代和优生学都是件好事。她着重指出：“将来，生个孩子有你那样的智慧和我这样的外貌，该有多么美妙！”萧伯纳在回信中表示不能接受这番好意，他说：“那个孩子如果只有我这样的外貌和你那样的智慧，就槽透了。", SPF_DEFAULT, NULL);//执行语音变量的Speek函数，这个函数用来读文字。
	pSpVoice->Release(); //释放语音变量
	::CoUninitialize();//释放语音环境
	return 0;
}