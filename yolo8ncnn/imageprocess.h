#ifndef __IMAGEPROCESS_H__
#define __IMAGEPROCESS_H__
#include <iostream>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

using namespace std;
using namespace cv;


void ContrastStretch(Mat mat, double k, double b);

void LogTransformation(Mat srcImage, double c);

Mat GammaTrans(Mat srcImag, float parameter);

void GrayScaleStretch(Mat srcImage);

void GreyScale(Mat srcImage);

#endif

