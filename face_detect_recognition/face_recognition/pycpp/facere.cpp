#include <Python.h>
#include<iostream>
int great_function_from_python(const char a[5])
{
	int res;

	PyObject *pModule, *pFunc;
	PyObject *pArgs, *pValue;

	pModule = PyImport_Import(PyString_FromString("facerecog"));

	pFunc = PyObject_GetAttrString(pModule, "facefounct");

	pValue = PyObject_CallFunction(pFunc,"s",a);

	//Python类型XXX转换为C语言类型YYY要使用PyXXX_AsYYY函数
	//C类型YYY转换为Python类型XXX要使用PyXXX_FromYYY函数
	res = PyInt_AsLong(pValue);//
	return res;
}
int main()
{
	Py_Initialize();
	std::cout << great_function_from_python("picc.jpg") << "\n";
	Py_Finalize();
	std::cin.get();
	return 0;
}