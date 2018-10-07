#include <iostream>    
#include "json//json.h"    
#include <iostream>  
#pragma comment(lib, "json_vc71_libmtd.lib")    


int main()
{
	Json::Value val;
	Json::StyledWriter style_write;

	val["name"] = Json::Value("xiaoli");

	std::cout << style_write.write(val) << std::endl;
	getchar();


	return 0;
}