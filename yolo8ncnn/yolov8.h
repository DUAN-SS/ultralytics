#ifndef _YOLOV8_H_
#define _YOLOV8_H_
#include "layer.h"
#include "net.h"
#include "opencv2/opencv.hpp"
#include <float.h>
#include <stdio.h>
#include <iostream>
#include <vector>

using namespace std;

#define MAX_STRIDE 32 // if yolov8-p6 model modify to 64

struct Object
{
	cv::Rect_<float> rect;
	int label;
	float prob;
};


class Detect
{
public:


	// 删除拷贝函数，防止拷贝
	Detect(const Detect& other) = delete;
	void operator = (const Detect& other) = delete;
	// 单例模式接口
	static Detect* GetIstance() {
		if(detect == nullptr) {
			detect = new Detect();
			cout << "new detect" << endl;
		}
		return detect;
	}

public:
	int DetectYolov8(const cv::Mat& bgr, vector<Object>& objects);
	void DrawObjects(const cv::Mat& bgr, const vector<Object>& objects);
	void DeleteDetect(void) {
		delete detect;
		detect = nullptr;
	}
private:
	//构造函数
	static Detect* detect;
	Detect() {
		cout << "create Detect success!" << endl;
	}

	// 析构函数
	~Detect() {
		cout << "free Detect success!!" << endl;
	}

	float SoftMax(const float* src, float* dst, int length);
	void GenerateProposals(int stride, const ncnn::Mat& feat_blob, const float prob_threshold, vector<Object>& objects);
	float Clamp(float val, float min = 0.f, float max = 1280.f);
	void NonMaxSuppression(vector<Object>& proposals, vector<Object>& results,
							int orin_h, int orin_w, float dh = 0, float dw = 0, float ratio_h = 1.0f,
							float ratio_w = 1.0f, float conf_thres = 0.25f, float iou_thres = 0.65f);
};


#endif // !
