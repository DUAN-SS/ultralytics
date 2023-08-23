#pragma once
#ifndef COLORDETECTOR_H_
#define COLORDETECTOR_H_

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>

class ColorDetector {
private:
    int minDist;
    cv::Vec3b target;
    cv::Vec3b target1;
    cv::Mat result;
    cv::Mat image;
    ColorDetector();
    static ColorDetector* singleton;

public:
    static ColorDetector* getInstance();
    static void destory();
    void setColorDistanceThreshold(int);
    int getColorDistanceThreshold() const;
    void setTargetColor(unsigned char, unsigned char, unsigned char);
    void setTarget1Color(unsigned char, unsigned char, unsigned char);
    void setTargetColor(cv::Vec3b);
    cv::Vec3b getTargetColor() const;
    void process();
    bool judgeTargetColor(const cv::Vec3b&, int) const;
    int getDistance(const cv::Vec3b&) const;
    int getDistance1(const cv::Vec3b&) const;
    cv::Mat getResult() const;
    bool setInputImage(std::string);
    cv::Mat getInputImage() const;
};


#endif /* COLORDETECTOR_H_ */