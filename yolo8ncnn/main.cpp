// 预测代码demo
//#include "yolov8.h"
//#include "imageprocess.h"
//
//Detect* Detect::detect = nullptr;
//
//int main(int argc, char** argv)
//{
//	cv::Mat m = cv::imread("1.jpg", 1);
//	if (m.empty())
//	{
//		fprintf(stderr, "cv::imread failed\n");
//		return -1;
//	}
//	std::vector<Object> objects;
//	Detect::GetIstance()->DetectYolov8(m, objects);
//	Detect::GetIstance()->DrawObjects(m, objects);
//	Detect::GetIstance()->DeleteDetect();
//
//	return 0;
//}



// 偏航检测demo - 方法一
//#include <windows.h>
//#include<iostream>
//#include <opencv2/opencv.hpp>
//#include"ColorDetector.h"
//#include <opencv2/highgui/highgui_c.h>
//using namespace cv;
//using namespace std;
//int main()
//{
//	ColorDetector* CD = ColorDetector::getInstance();
//	CD->setInputImage("4.png");
//	//Mat img = CD->getInputImage();
//	CD->setTargetColor(255, 128, 192);
//	CD->setTarget1Color(0, 0, 0);
//	CD->setColorDistanceThreshold(10);
//	CD->process();
//	Mat out = CD->getResult();
//	namedWindow("out", CV_WINDOW_NORMAL);
//	resizeWindow("out", 600, 600);
//	imshow("out", out);
//	waitKey();
//	return 0;
//}


#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;
int hough_value = 55;
Mat src, gray;
void hough_change(int, void*) {
    vector<Vec3f>circles;
    HoughCircles(gray, circles, HOUGH_GRADIENT, 1, 50, 100, 100, 20, 200);
    Mat show = src.clone();
    double center_x = 0;
    double center_y = 0;
    for (int i = 0; i < circles.size(); i++) {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        circle(show, Point(circles[i][0], circles[i][1]), circles[i][2], Scalar(0, 0, 255), 2);
        for (int x = 0; x < show.cols; x++) {
            for (int y = 0; y < show.rows; y++) {
                int r = sqrt((x - center.x) * (x - center.x) + (y - center.y) * (y - center.y));
                if (r >= radius - 5) {
                    show.at<Vec3b>(Point(x, y))[0] = 0;
                    show.at<Vec3b>(Point(x, y))[1] = 0;
                    show.at<Vec3b>(Point(x, y))[2] = 0;
                }
            }
        }
        center_x = center.x;
        center_y = center.y;
    }

    cv::Mat canny, dst;
    cv::Canny(show, canny, 50, 150); // 配合canny算法使用
    imshow("huidu", show);
    cv::cvtColor(canny, dst, cv::COLOR_GRAY2BGR); //灰度图转换为彩色图

    std::vector<cv::Vec4f> plines;
    cv::HoughLinesP(canny, plines, 1, CV_PI / 180.0, 5, 100, 5);
    cv::Scalar color = cv::Scalar(0, 0, 255);
    double maxDistance = 1920;
    for (size_t i = 0; i < plines.size(); i++) {
        cv::Vec4f hline = plines[i];
        cv::line(dst, cv::Point(hline[0], hline[1]), cv::Point(hline[2], hline[3]), color, 2, cv::LINE_AA);
        int x1 = hline[0];
        int x2 = hline[2];
        int y1 = hline[1];
        int y2 = hline[3];
        cout << "x1 =  " << hline[0] << "   x2 = " << hline[2] << endl;
        if (x2 != x1) {
            for (int x = hline[0]; x <= hline[2]; x++) {
                int y = (x - x1) * (y2 - y1) / (x2 - x1) + y1;
                //cout << "x = " << x << "   y= " << y << endl;
                int temp = sqrt((x - center_x) * (x - center_x) + (y - center_y) * (y - center_y));
                if (temp < maxDistance) {
                    maxDistance = temp;
                }
            }
        }
    }

    if (maxDistance > 10) {
        cout << "***************偏航****************" << endl;
    } else {
        cout << "***************正常****************" << endl;
    }
    imshow("show", dst);
}


int main() {
    src = imread("6.png");
    GaussianBlur(src, src, Size(3, 3), 0, 0);
    imshow("src", src);
    cvtColor(src, gray, COLOR_BGR2GRAY);
    namedWindow("show");
    createTrackbar("hough_value", "show", &hough_value, 200, hough_change);
    hough_change(0, 0);
    waitKey(0);
}


 //测试代码
//#include "yolov8.h"
//#include "imageprocess.h"
//
//Detect* Detect::detect = nullptr;
//
//int main(int argc, char** argv)
//{
//	cv::Mat m = cv::imread("6.png", 1);
//	if (m.empty())
//	{
//		fprintf(stderr, "cv::imread failed\n");
//		return -1;
//	}
//	cv::imshow("原图", m); // 展示原图
//	cv::Mat canny, dst;
//	cv::Canny(m, canny, 50, 150); // 配合canny算法使用
//	cv::cvtColor(canny, dst, cv::COLOR_GRAY2BGR); //灰度图转换为彩色图
//
//	std::vector<cv::Vec4f> plines;
//	cv::HoughLinesP(canny, plines, 1, CV_PI / 180.0, 5, 50, 10);
//	cv::Scalar color = cv::Scalar(0, 0, 255);
//	for (size_t i = 0; i < plines.size(); i++) {
//		cv::Vec4f hline = plines[i];
//		cv::line(dst, cv::Point(hline[0], hline[1]), cv::Point(hline[2], hline[3]), color, 3, cv::LINE_AA);
//	}
//	cv::imshow("output", dst);
//	waitKey(0);
//	return 0;
//}