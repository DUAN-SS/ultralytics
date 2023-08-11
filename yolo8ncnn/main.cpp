#include "yolov8.h"
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
	std::vector<Object> objects;
	Detect::GetIstance()->DetectYolov8(m, objects);
	Detect::GetIstance()->DrawObjects(m, objects);
	Detect::GetIstance()->DeleteDetect();
	return 0;
}