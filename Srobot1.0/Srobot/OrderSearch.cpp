#include"stdafx.h"
#include"OrderSearch.h"

OrderSearch::OrderSearch()
{
	Init();
}

OrderSearch::~OrderSearch()
{
	order_dic.clear();
}
void OrderSearch::Init()
{
	order_dic["山东科技大学。"] = { PhizModule,18 };
	order_dic["前进。"] = { MoveModule,1 }; order_dic["后退。"] = { MoveModule,2 }; order_dic["左转。"] = { MoveModule,3 };
	order_dic["右转。"] = { MoveModule,4 }; order_dic["停止。"] = { MoveModule,0 };
	order_dic["可爱。"] = { PhizModule,1 }; order_dic["可怜。"] = { PhizModule,2 }; order_dic["纠结。"] = { PhizModule,3 }; order_dic["偷笑。"] = { PhizModule,4 };
	order_dic["惊讶。"] = { PhizModule,5 }; order_dic["笑脸。"] = { PhizModule,6 }; order_dic["囧。"] = { PhizModule,7 }; order_dic["俏皮。"] = { PhizModule,8 };
	order_dic["悲伤。"] = { PhizModule,9 }; order_dic["心仪。"] = { PhizModule,10 }; order_dic["奔跑。"] = { PhizModule,12 }; order_dic["愤怒。"] = { PhizModule,11 };
	order_dic["广播体操。"] = { ArmModule,GymSw }; order_dic["原地踏步。"] = { ArmModule,PiaffeSw }; order_dic["复位。"] = { ArmModule,ActionRes };
	order_dic["停止动作。"] = { ArmModule,StopAll }; order_dic["拥抱。"] = { ArmModule,HugArm }; order_dic["发出邀请。"] = { ArmModule,InviteArm };
}
bool OrderSearch::Search(std::string recorder, int &Device, int &order)
{
	std::map<std::string, device_order>::iterator result;
	result = order_dic.find(recorder);
	if (result == order_dic.end())//没找到
	{
		return false;
	}
	else//找到 
	{
		Device = result->second.device;
		order = result->second.order;
	}
	return true;
}