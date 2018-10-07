#define _CRT_SECURE_NO_WARNINGS
#include "curl/curl.h"
#include <iostream>
#include "json//json.h"    
#pragma comment(lib, "json_vc71_libmtd.lib")    
#pragma comment(lib, "libcurld.lib")

const std::string key = "NULL";
const std::string userid = "NULL";
const char* tulingapi = "http://www.tuling123.com/openapi/api";
char* ANSItoUTF8(const char* strAnsi);
char* UTF8toANSI(const char* strUTF8);
char* UTF8ToEncode(const char* mbcsStr);
char* EncodeToUTF8(const char* mbcsStr);

std::wstring AsciiToUnicode(const std::string& str);
std::string UnicodeToUtf8(const std::wstring& wstr);
std::string AsciiToUtf8(const std::string& str);

int turing(Json::Value rec_result);
static size_t callback(void *ptr, size_t size, size_t nmemb, void *stream);

int main()
{
	Json::Value myword;
	std::string utf8key = AsciiToUtf8(key);
	myword[AsciiToUtf8("key")] = Json::Value(utf8key);
	std::string info = "讲个笑话";
	std::string utf8info = AsciiToUtf8(info);
	myword[AsciiToUtf8("info")] = Json::Value(utf8info);
	std::string utf8userid = AsciiToUtf8(userid);
	myword[AsciiToUtf8("userid")] = Json::Value(utf8userid);

	std::string lins;
	Json::FastWriter fast_writer;
	lins = fast_writer.write(myword);
	int errocod = turing(lins);

	//Json::FastWriter fast_writer;
	//std::cout << fast_writer.write(myword) << std::endl;

	std::cin.get();
	std::cin.get();
	return 0;
}

static size_t callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
	// 获取到的body存放在ptr中，先将其转换为string格式
	std::string s((char *)ptr, size * nmemb);
	std::string* access_token_result = static_cast<std::string*>(stream);
	// 开始获取json中的access token项目
	Json::Reader reader;
	Json::Value root;
	// 使用boost库解析json
	if (reader.parse(s, root))
	{

		//Json::FastWriter fast_writer;
		//std::cout << fast_writer.write(root) << std::endl;

		std::string aaa = root["text"].asString();
		char* turing_back;
		turing_back = UTF8ToEncode(aaa.c_str());

		std::cout << "图灵回复：" << turing_back << std::endl;
		free(turing_back);

		//std::string bbb = root["code"].asString();
		//char* errcod;
		//errcod = UTF8ToEncode(bbb.c_str());
		//std::cout << "错误代码" << errcod << std::endl;
	}

	return size * nmemb;
}


int turing(Json::Value rec_result)
{
	CURL *curl;
	CURLcode result_code;
	struct curl_slist *headers = NULL;
	int error_code = 0;
	//rec_result = ANSItoUTF8(rec_result);
	curl = curl_easy_init();
	headers = curl_slist_append(headers, "Content-Type:application/json;charset=UTF-8");
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, tulingapi);
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_POST, 1);//设置为非0表示本次操作为POST
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, rec_result);
		//curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, sizeof(rec_result));//设置上传json串长度,这个设置可以忽略
		std::string access_token_result;
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &access_token_result);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
		result_code = curl_easy_perform(curl);
		if (result_code != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(result_code));
			return 1;
		}
		curl_easy_cleanup(curl);
		error_code = 0;
	}
	else {
		fprintf(stderr, "curl_easy_init() failed.");
		error_code = 1;
	}
	return error_code;
}



///////////////////////////////////////////////////////////////////////////////////这些转换返回值使用后需要手动释放内存
//UTF8转ANSI
char* UTF8toANSI(const char* strUTF8)
{
	//获取转换为多字节后需要的缓冲区大小，创建多字节缓冲区
	UINT nLen = MultiByteToWideChar(CP_UTF8, NULL, strUTF8, -1, NULL, NULL);
	WCHAR *wszBuffer = new WCHAR[nLen + 2];
	nLen = MultiByteToWideChar(CP_UTF8, NULL, strUTF8, -1, wszBuffer, nLen);
	wszBuffer[nLen] = 0;

	nLen = WideCharToMultiByte(936, NULL, wszBuffer, -1, NULL, NULL, NULL, NULL);
	CHAR *szBuffer = new CHAR[nLen + 1];
	nLen = WideCharToMultiByte(936, NULL, wszBuffer, -1, szBuffer, nLen, NULL, NULL);
	szBuffer[nLen] = 0;

	strUTF8 = szBuffer;
	//清理内存
	delete[]wszBuffer;
	return szBuffer;
}
char* UTF8ToEncode(const char* mbcsStr)
{
	wchar_t*  wideStr;
	char*   unicodeStr;
	int   charLen;

	charLen = MultiByteToWideChar(CP_UTF8, 0, mbcsStr, -1, NULL, 0);
	wideStr = (wchar_t*)malloc(sizeof(wchar_t)*charLen);
	MultiByteToWideChar(CP_UTF8, 0, mbcsStr, -1, wideStr, charLen);

	charLen = WideCharToMultiByte(936, 0, wideStr, -1, NULL, 0, NULL, NULL);
	unicodeStr = (char*)malloc(charLen);
	WideCharToMultiByte(936, 0, wideStr, -1, unicodeStr, charLen, NULL, NULL);

	free(wideStr);
	return unicodeStr;
}

//ANSI转UTF8
char* ANSItoUTF8(const char* strAnsi)
{
	//获取转换为宽字节后需要的缓冲区大小，创建宽字节缓冲区，936为简体中文GB2312代码页
	UINT nLen = MultiByteToWideChar(936, NULL, strAnsi, -1, NULL, NULL);
	WCHAR *wszBuffer = new WCHAR[nLen + 1];
	nLen = MultiByteToWideChar(936, NULL, strAnsi, -1, wszBuffer, nLen);
	wszBuffer[nLen] = 0;
	//获取转为UTF8多字节后需要的缓冲区大小，创建多字节缓冲区
	nLen = WideCharToMultiByte(CP_UTF8, NULL, wszBuffer, -1, NULL, NULL, NULL, NULL);
	CHAR *szBuffer = new CHAR[nLen + 1];
	nLen = WideCharToMultiByte(CP_UTF8, NULL, wszBuffer, -1, szBuffer, nLen, NULL, NULL);
	szBuffer[nLen] = 0;

	strAnsi = szBuffer;
	//内存清理
	delete[]wszBuffer;
	return szBuffer;
}

char* EncodeToUTF8(const char* mbcsStr)
{
	wchar_t*  wideStr;
	char*   utf8Str;
	int   charLen;

	charLen = MultiByteToWideChar(936, 0, mbcsStr, -1, NULL, 0);    ////////936 ----- gb2312
	wideStr = (wchar_t*)malloc(sizeof(wchar_t)*charLen);
	MultiByteToWideChar(936, 0, mbcsStr, -1, wideStr, charLen);

	charLen = WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, NULL, 0, NULL, NULL);

	utf8Str = (char*)malloc(charLen);

	WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, utf8Str, charLen, NULL, NULL);

	free(wideStr);
	return utf8Str;

}
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

std::wstring AsciiToUnicode(const std::string& str)
{
	// 预算-缓冲区中宽字节的长度  
	int unicodeLen = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0);
	// 给指向缓冲区的指针变量分配内存  
	wchar_t *pUnicode = (wchar_t*)malloc(sizeof(wchar_t)*unicodeLen);
	// 开始向缓冲区转换字节  
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, pUnicode, unicodeLen);
	std::wstring ret_str = pUnicode;
	free(pUnicode);
	return ret_str;
}

std::string UnicodeToUtf8(const std::wstring& wstr)
{
	// 预算-缓冲区中多字节的长度  
	int ansiiLen = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
	// 给指向缓冲区的指针变量分配内存  
	char *pAssii = (char*)malloc(sizeof(char)*ansiiLen);
	// 开始向缓冲区转换字节  
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, pAssii, ansiiLen, nullptr, nullptr);
	std::string ret_str = pAssii;
	free(pAssii);
	return ret_str;
}


std::string AsciiToUtf8(const std::string& str)
{
	return UnicodeToUtf8(AsciiToUnicode(str));
}
