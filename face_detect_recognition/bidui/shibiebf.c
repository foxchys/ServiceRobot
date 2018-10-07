//#include "opencv2/objdetect/objdetect.hpp"
//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
//
//#include <iostream>
//#include <stdio.h>
//#include <ctime>
//
//#include <Python.h>
//
//using cv::Mat;
//using std::string;
//using std::cout;
//using std::endl;
//
//int facebidui(const char a[10]);
//
//
//cv::String face_cascade_name = "haarcascade_frontalface_alt.xml";
//cv::CascadeClassifier face_cascade;
//string window_name = "Capture - Face detection";
//
//
//int main(int argc, const char** argv)
//{
//	std::vector<cv::Rect> faces;
//	cv::VideoCapture capture;
//	capture.open(0);
//	face_cascade.load(face_cascade_name);
//
//	float secs = 2;
//	clock_t delay = secs*CLOCKS_PER_SEC;
//	clock_t lastphoto = clock() - delay;
//	int biduijg;
//
//	Py_Initialize();
//	while (1)
//	{
//		Mat frame;
//		Mat grayImage;
//		capture >> frame;
//		cv::cvtColor(frame, grayImage, CV_BGR2GRAY);
//		face_cascade.detectMultiScale(grayImage, faces, 1.1, 3, 0 | CV_HAAR_DO_ROUGH_SEARCH, cv::Size(100, 100));
//		/*for (size_t i = 0; i < faces.size(); i++)*/
//		size_t i = 0;
//		if (i<faces.size())
//		{
//			cv::Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
//			if (clock() - lastphoto> delay)
//			{
//				imwrite("pic1.jpg", frame);
//				/*Py_Initialize();*/
//				biduijg = facebidui("pic0.jpg");
//				/*Py_Finalize();*/
//				if (biduijg == 1)
//					cout << " see you again " << endl;
//				else
//				{
//					cout << "who are you " << endl;
//				}
//			}
//			ellipse(frame, center, cv::Size(faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, cv::Scalar(0, 255, 0), 4, 8, 0);
//			lastphoto = clock();
//			i++;
//		}
//		imshow(window_name, frame);
//		if (cv::waitKey(10) >= 0) { break; }
//	}
//	Py_Finalize();
//	return 0;
//}
//
//int facebidui(const char a[10])
//{
//	int res;
//
//	PyObject *pModule, *pFunc;
//	PyObject *pValue;
//
//	pModule = PyImport_Import(PyString_FromString("facerecog"));
//
//	pFunc = PyObject_GetAttrString(pModule, "facefounct");
//
//	pValue = PyObject_CallFunction(pFunc, "s", a);
//
//	//Python类型XXX转换为C语言类型YYY要使用PyXXX_AsYYY函数
//	//C类型YYY转换为Python类型XXX要使用PyXXX_FromYYY函数
//	res = PyInt_AsLong(pValue);
//	return res;
//}
