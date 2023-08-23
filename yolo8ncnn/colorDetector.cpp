#include "ColorDetector.h"

ColorDetector* ColorDetector::singleton = NULL;

ColorDetector::ColorDetector() :minDist(500) {
    target[0] = target[1] = target[2] = 0;
}

ColorDetector* ColorDetector::getInstance() {
    if (singleton == NULL) {
        singleton = new ColorDetector;
    }
    return singleton;
}

void ColorDetector::destory() {
    if (singleton != 0) {
        delete singleton;
    }
    singleton = 0;
}

void ColorDetector::setColorDistanceThreshold(int distance) {
    if (distance < 0) {
        distance = 0;
    }
    minDist = distance;
}

int ColorDetector::getColorDistanceThreshold() const {
    return minDist;
}

void ColorDetector::setTargetColor(unsigned char red,
    unsigned char green, unsigned char blue) {
    target[2] = red;
    target[1] = green;
    target[0] = blue;
}

void ColorDetector::setTarget1Color(unsigned char red,
    unsigned char green, unsigned char blue) {
    target1[2] = red;
    target1[1] = green;
    target1[0] = blue;
}

void ColorDetector::setTargetColor(cv::Vec3b color) {
    target = color;
}

cv::Vec3b ColorDetector::getTargetColor() const {
    return target;
}

int ColorDetector::getDistance(const cv::Vec3b& color) const {
    return abs(color[0] - target[0]) + abs(color[1] - target[1]) + abs(color[2] - target[2]);
}

int ColorDetector::getDistance1(const cv::Vec3b& color) const {
    return abs(color[0] - target1[0]) + abs(color[1] - target1[1]) + abs(color[2] - target1[2]);
}

bool ColorDetector::judgeTargetColor(const cv::Vec3b& color, int area) const
{
    if (abs(color[0] - target[0]) + abs(color[1] - target[1]) + abs(color[2] - target[2]) < area)
        return true;
    return false;
}

void ColorDetector::process() {
    result.create(image.rows, image.cols, CV_8U);
    cv::Mat_<cv::Vec3b>::const_iterator it = image.begin<cv::Vec3b>();
    cv::Mat_<cv::Vec3b>::const_iterator itend = image.end<cv::Vec3b>();
    cv::Mat_<uchar>::iterator itout = result.begin<uchar>();
    bool ret = false;
    for (; it != itend; ++it, ++itout) {
        if (getDistance(*it) < minDist) {
            *itout = 255;
        } else {
            if (getDistance1(*it) < minDist) {
                *itout = 255;
                cv::Mat_<cv::Vec3b>::const_iterator temp = it;
                for (int i = 0; i < 5; i++) {
                    ret |= judgeTargetColor(*(temp - i), 10);
                    ret |= judgeTargetColor(*(temp + i), 10);
                    ret |= judgeTargetColor(*(temp + i * image.cols), 10);
                    ret |= judgeTargetColor(*(temp - i * image.cols), 10);
                }
            } else {
                *itout = 0;
            }
        }
    }
    if (ret) {
        printf("**************Õý³£*****************");
    }
    else {
        printf("**************Æ«º½*****************");
    }
}

cv::Mat ColorDetector::getResult() const {
    return result;
}

bool ColorDetector::setInputImage(std::string filename) {
    image = cv::imread(filename);
    if (!image.data) {
        return false;
    }
    return true;
}

cv::Mat ColorDetector::getInputImage() const {
    return image;
}