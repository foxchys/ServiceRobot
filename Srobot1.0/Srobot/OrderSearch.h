#pragma once
#ifndef _OrderSearch_H_
#define _OrderSearch_H_
//用于查找语音命令
#include<map>
#include<string>

class OrderSearch
{
public:
	OrderSearch();
	~OrderSearch();
	bool Search(std::string recorder, int &Device, int &order);
private:
	void Init();
	enum DeviceList { PhizModule, MoveModule, ArmModule };//模块列表
	enum ArmOrderList { GymSw, PiaffeSw, ActionRes, StopAll, HugArm, InviteArm };//手臂动作列表
	struct device_order
	{
		int device;
		int order;
	};
	std::map<std::string, device_order> order_dic;//命令词，模块，发送数据
};
#endif