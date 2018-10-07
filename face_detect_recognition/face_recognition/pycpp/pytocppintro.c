//#include <Python.h>
//#include<iostream>
//int great_function_from_python(int a) {
//	int res;
//
//	//所有Python元素，module、function、tuple、string等等，
//	//实际上都是PyObject。C语言里操纵它们，一律使用PyObject *
//	PyObject *pModule, *pFunc;
//	PyObject *pArgs, *pValue;
//
//	/* import */
//	pModule = PyImport_Import(PyString_FromString("great_module"));
//
//	/* great_module.great_function */
//	pFunc = PyObject_GetAttrString(pModule, "great_function");
//
//	/* build args */
//	/*也可以创建Python类型的变量，使用PyXXX_New可以创建类型为XXX的变量。*/
//	pArgs = PyTuple_New(1);
//	/*若a是Tuple，则a[i] = b对应于 PyTuple_SetItem(a, i, b)，有理由相信还有一个函数PyTuple_GetItem完成取得某一项的值。*/
//	PyTuple_SetItem(pArgs, 0, PyInt_FromLong(a));
//
//	/* call */
//	pValue = PyObject_CallObject(pFunc, pArgs);
//
//	//Python类型XXX转换为C语言类型YYY要使用PyXXX_AsYYY函数
//	//C类型YYY转换为Python类型XXX要使用PyXXX_FromYYY函数
//	res = PyInt_AsLong(pValue);//
//	return res;
//}
//int main()
//{
//	Py_Initialize();
//	std::cout << great_function_from_python(2) << "\n";
//	Py_Finalize();
//	std::cin.get();
//	return 0;
//}