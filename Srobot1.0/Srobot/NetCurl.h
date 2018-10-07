#pragma once
#ifndef _NetCurl_H
#define _NetCurl_H
//#define _CRT_SECURE_NO_WARNINGS
//在工程上右键-》属性-》c/c++-》代码生成-》运行库  选择MTD
// 右击项目->属性->配置属性->常规，然后在右边的“项目默认值”中的“MFC的使用”选项中选择“在静态库中使用MFC”
#include "curl/curl.h"
#include <iostream>
#include "json//json.h"    
#pragma comment(lib, "json_vc71_libmtd.lib")    
#pragma comment(lib, "libcurld.lib")

class NetCurl
{
public:
	NetCurl();
	virtual ~NetCurl();
#pragma region 数据类型转换

#pragma region 这几个函数需要手动释放返回值处的内存地址
	char* ANSItoUTF8(const char* strAnsi);
	char* UTF8toANSI(const char* strUTF8);
	char* EncodeToUTF8(const char* mbcsStr);
#pragma endregion

//////////////////数据类型转换，这几个不需要手动释放返回值处的内存地址
	std::wstring AsciiToUnicode(const std::string& str);
	std::string UnicodeToUtf8(const std::wstring& wstr);
	std::string AsciiToUtf8(const std::string& str);
	std::string UTF8ToUnicode(const char* mbcsStr);

	std::wstring StringToWstring(const std::string str);
	std::string WstringToString(const std::wstring str);
/////////////////////////////////////////////////////////////
#pragma endregion
	int curlpost(std::string urlofpost,Json::Value rec_result);//使用curl进行post,第一个参数为post的url,第二个参数为post的body此处应为json格式的string,设定post的body为utf8
	Json::Value GetPostResult();//获取post返回Json结果
	bool stopcallback;//回掉函数停止的标志

private:
	static bool callbackflag;//回掉完成的标志，获取完PostResult后应手动将callbackflag设为false
	static size_t postcallback(void *ptr, size_t size, size_t nmemb, void *stream);//curl回掉函数网络通信接收post返回信息
	static Json::Value PostResult;//post回掉接收body函数回掉结果PostResult
};

class TulingRobot :public NetCurl//图灵机器webapi接入类
{
public:
	//TulingRobot() :key(""), userid(""), tulingapi("http://www.tuling123.com/openapi/api"){}//构造函数初始化列表const赋值
	TulingRobot();
	~TulingRobot();
	void InputText(std::string mytext);//打包向图灵机器人post的body数据
	void PostToTuling();//向图灵机器人发出post请求
	std::string strmyword;//对应于myword的string格式
	std::string TulingResult();//获取图灵机器人回复信息
private:
	/*const */std::string key;//需要提交的key
	/*const */std::string userid;//提交的用户id
	/*const */std::string tulingapi;//图灵api接入地址。
};

class Facepp :public NetCurl//face++的webapi接入类
{
public:
	Facepp();
	~Facepp();

	std::string GetFaPostRe();//获取本次请求的所有返回结果

	//这个函数仅作系统维护或第一次使用，创建一个人脸的集合 FaceSet，用于存储人脸标识 face_token
	bool FaceSetCreat(std::string FaSetLogo); //FaSetLogo设置的FaceSet自定义标识

	bool FGetFaceSets();//获取某一 API Key 下的 FaceSet 列表及其 faceset_token、outer_id、display_name 和 tags 等信息。

	//删除一个人脸集合(仅作维护时使用)
	bool FaceSetDelete(std::string DFaceSetToken);//DFaceSetToken需要删除的FaceSet的标识

	//传入图片进行人脸检测和人脸分析返回该人脸的facetoken
	bool FaceDetect(std::string FFileName);//FFileName被post的文件路径包括文件名和后缀
	std::string FDetectToken();//获取本次人脸检测的facetoken
	static bool ErroFlag;//post返回值回掉是否失败标志，没失败返回true

	//为一个已经创建的 FaceSet 添加人脸标识 face_token
	bool FSetAddFace(std::string FtokenToAd, std::string FaceSetId);//FtokenToAd需要被加入人脸标识的facetoken,FaceSetId为facetoken要加入的faceset自设定的id

	//获取一个 FaceSet 的所有信息
	bool FSetDetail(std::string FSetTokenToGet);//FSetTokenToGet需要获取的FaceSet的标识token

	//移除一个FaceSet中的全部face_token
	bool RemoveFFaSet(std::string RemoveFFSetToken);//RemoveFFSetToken需要移除全部face_token的FaceSet的标识faceset_token

	////在一个已有的FaceSet中找出与目标人脸最相似的一张人脸返回facetoken
	bool SearchFFSet(std::string FFNaToSe, std::string SeaFaSetLogo);//FFNaToSe被传入的图片位置（包括文件名和后缀）,SeaFaSetLogo;//被检测的FaceSet的自自定义标识,
	std::string GetSearchRe();//获取本次Seach到的用户自定义的人脸标识

	//设定人脸标识facetoken的自定义标识信息
	bool FaceTokenID(std::string FaceToken,std::string UserId );//FaceToken需要被设置自定义标识的facetoken,UserId将要设定到法测token中的自定义标识

private:
	std::string FaceppKey;//facepp的key
	std::string FaceppSecret;//facepp的secret
	static std::string FaPostResult;//本次请求的所有返回值

	std::string FSetCreatUrl;//创建FaceSet的Url
	static size_t FSetCreatCallback(void *ptr, size_t size, size_t nmemb, void *stream);//回掉函数，创建一个人脸的集合 FaceSet，用于存储人脸标识 face_token

	std::string FGetFaceSetUrl;//获取某一 API Key 下的 FaceSet 列表及其 faceset_token、outer_id、display_name 和 tags 等信息的Url
	static size_t FGetFSetCallback(void *ptr, size_t size, size_t nmemb, void *stream);

	std::string FSetDeleteUrl;//删除一个人脸合集FaceSet的Url;
	static size_t FSetDeleteCallback(void *ptr, size_t size, size_t nmemb, void *stream);

	static std::string FaceToken;//本次的FaceToken
	std::string FaceppDetectUrl;//传入图片进行人脸检测和人脸分析的url
	static size_t FDetectCallback(void *ptr, size_t size, size_t nmemb, void *stream);//Facepp的curl回掉函数网络通信接收post返回信息

	std::string FSetAddFaceUrl;//为一个已经创建的 FaceSet 添加人脸标识 face_token的Url
	static size_t FSetAddFaCallback(void *ptr, size_t size, size_t nmemb, void *stream);//为为一个已经创建的 FaceSet 添加人脸标识 face_token的回掉函数

	std::string FSetDetailUrl;//获取一个 FaceSet 的所有信息的Url
	static size_t FSetDetailCallback(void *ptr, size_t size, size_t nmemb, void *stream);//获取一个 FaceSet 的所有信息的回掉函数

	std::string RemoveFFaSetUrl;//移除一个FaceSet中的全部face_token的Url
	static size_t RemoveFFaSetCallback(void *ptr, size_t size, size_t nmemb, void *stream);//移除一个FaceSet中的全部face_token的回掉函数

	static std::string LogoFace;//本次search到的自定义标识信息
	std::string SearchFFSetUrl;//在一个已有的FaceSet中找出与目标人脸最相似的一张人脸的Url
	static size_t SearchFFSetCallback(void *ptr, size_t size, size_t nmemb, void *stream);//在一个已有的FaceSet中找出与目标人脸最相似的一张人脸的回掉函数

	std::string FaceTokenIDUrl;//设定人脸标识facetoken的自定义标识信息的Url
	static size_t FaceTokenIDCallback(void *ptr, size_t size, size_t nmemb, void *stream);//(若设定为中文则此处解析时需要用基类NetCurl的UTF8ToUnicode转码显示)设定人脸标识facetoken的自定义标识信息的回掉函数
};

#endif // !_NetCurl_H

//TulingResult函数中while循环无意义