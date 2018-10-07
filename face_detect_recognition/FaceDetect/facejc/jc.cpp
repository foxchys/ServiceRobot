#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <stdio.h>

using cv::Mat;
using std::string;
using std::cout;
using std::endl;

cv::String face_cascade_name = "haarcascade_frontalface_alt.xml";
cv::CascadeClassifier face_cascade;//这个类在opencv2.4.9加vs15的情况下析构时会出错
string window_name = "Capture - Face detection";


int main(int argc, const char** argv)
{
	std::vector<cv::Rect> faces;
	cv::VideoCapture capture;
	capture.open(0);
	face_cascade.load(face_cascade_name);
	while (1)
	{
		Mat frame;
		Mat grayImage;
		capture >> frame;
		grayImage.create(frame.size(), frame.type());
		cv::cvtColor(frame, grayImage, CV_BGR2GRAY);
		face_cascade.detectMultiScale(grayImage, faces, 1.1, 3, 0 | CV_HAAR_DO_ROUGH_SEARCH, cv::Size(100, 100));
		for (size_t i = 0; i < faces.size(); i++)
		{
			//cv::Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);//计算脸部圆心
			//ellipse(frame, center, cv::Size(faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, cv::Scalar(0, 255, 0), 4, 8, 0);//在脸部画圆

			cv::Rect select;//声明矩形  
			select.x = faces[i].x;
			select.y = faces[i].y;
			select.width = faces[i].width;
			select.height = faces[i].height;
			rectangle(frame, select, cv::Scalar(0, 255, 0), 2, 8, 0);//在脸部画矩形：：参数形式void rectangle(Mat& img, Rect rec, const Scalar& color, int thickness=1, int lineType=8, int shift=0 )

		}
		imshow(window_name, frame);
		if (cv::waitKey(30) >= 0) { break; }
	}
	return 0;
}

