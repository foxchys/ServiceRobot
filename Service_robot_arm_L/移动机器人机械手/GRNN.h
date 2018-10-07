#pragma once

#include "time.h"
#include <cmath>
#include <fstream>
#include <float.h>

using namespace std;

#define pi 3.1415926
#define Input_Neurons      3	//输入层神经元个数，其值等于输入样本的维数
#define Hidden_Neurons  1030301	//隐含层神经元个数，其值等于输入样本的个数
#define Output_Neurons     3	//输出层神经元个数，其值等于输出样本的维数
typedef float Matrix[4][4];
typedef float Vector[4];

class GRNNNetwork
{
private:

	HWND hWnd;					//专用表示窗口的句柄
	//float **w1;
	//float **w2;
	//float *U;
	//float V[3];					//输出层某一节点输出
	//float Input_Buf[3];			//输入向量缓冲区
	//float Output_Buf[3];		//输出向量缓冲区
	//float alpha[3];				//需要输出的关节变量
	//float distance;				//距离
	////////////////////////////////////////////////////
	float **Lw1;
	float **Lw2;
	float *LU;
	float LV[3];					//输出层某一节点输出
	float LInput_Buf[3];			//输入向量缓冲区
	float LOutput_Buf[3];			//输出向量缓冲区
	float Lalpha[3];				//需要输出的关节变量
	////////////////////////////////////////////////////
	float **Rw1;
	float **Rw2;
	float *RU;
	float RV[3];					//输出层某一节点输出
	float RInput_Buf[3];			//输入向量缓冲区
	float ROutput_Buf[3];			//输出向量缓冲区
	float Ralpha[3];				//需要输出的关节变量
	bool GRNNIsOpened;				//网络初始化标志位
	
public:
	 GRNNNetwork();//声明构造函数
	~GRNNNetwork();//声明析构函数
	
	void Initcenter();//网络初始化
	bool GRNNNetworkStart();//启动网络
	bool Calculate_Left(float a, float b, float c);
	bool Calculate_Right(float a, float b, float c);
	float Euclidean_distance(float *a, float *b);//求输入样本与权值矩阵的欧氏距离
	//BP神经网络函数，目的是校正目标定位系统的误差
	//void mapminmaxIn(float* t);//输入样本归一化
	//void unmapminmaxOut(float* t);//输出样本反归一化
	//void correction(float* p);

protected:
	//CWinThread* pGRNNThread;
	//static UINT GRNNThreadFunc(LPVOID pParam);//线程函数，作为类的成员要加上static
};

void MMMul(Matrix a, Matrix b, Matrix c, int m, int r, int s, int n);//矩阵相乘
void MVMul(Matrix a, Vector b, Vector c, int m, int r, int n);//矩阵向量相乘
