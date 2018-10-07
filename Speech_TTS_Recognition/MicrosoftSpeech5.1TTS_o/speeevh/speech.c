//#include <sphelper.h>//语音头文件
//#include <iostream>//C++头文件，用来提示错误信息
//
//int main()
//{
//	::CoInitialize(NULL);//初始化语音环境
//	ISpVoice * pSpVoice = NULL;//初始化语音变量
//	if (FAILED(CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_INPROC_SERVER, IID_ISpVoice, (void **)&pSpVoice)))
//		//给语音变量创建环境，相当于创建语音变量，FAILED是个宏定义，就是来判断CoCreateInstance这个函数又没有成功创建语音变量，下面是不成功的提示信息。
//	{
//		std::cout << "Failed to create instance of ISpVoice!" << std::endl;
//		return -1;
//	}
//
//	pSpVoice->Speak(L"你好，来交个朋友吧", SPF_DEFAULT, NULL);//执行语音变量的Speek函数，这个函数用来读文字。
//	pSpVoice->Release(); //释放语音变量
//	::CoUninitialize();//释放语音环境
//	return 0;
//}