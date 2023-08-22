#include "yolov8.h"
#include "imageprocess.h"

Detect* Detect::detect = nullptr;

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s [imagepath]\n", argv[0]);
		return -1;
	}

	const char* imagepath = argv[1];

	cv::Mat m = cv::imread(imagepath, 1);
	if (m.empty())
	{
		fprintf(stderr, "cv::imread %s failed\n", imagepath);
		return -1;
	}
	cv::imshow("原图", m); // 展示原图
	GreyScale(m);
	cv::Mat canny, dst;
	cv::Canny(m, canny, 50, 150); // 配合canny算法使用
	cv::cvtColor(canny, dst, cv::COLOR_GRAY2BGR); //灰度图转换为彩色图
	cv::imshow("edge", canny);

	std::vector<cv::Vec4f> plines;
	cv::HoughLinesP(canny, plines, 1, CV_PI / 180.0, 5, 50, 10);
	cv::Scalar color = cv::Scalar(0, 0, 255);
	for (size_t i = 0; i < plines.size(); i++) {
		cv::Vec4f hline = plines[i];
		cv::line(dst, cv::Point(hline[0], hline[1]), cv::Point(hline[2], hline[3]), color, 3, cv::LINE_AA);
	}
	cv::imshow("output", dst);


	std::vector<Object> objects;
	Detect::GetIstance()->DetectYolov8(m, objects);
	Detect::GetIstance()->DrawObjects(m, objects);
	Detect::GetIstance()->DeleteDetect();

	return 0;
}