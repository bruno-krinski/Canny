#ifndef Canny_HPP_
#define Canny_HPP_

#include <string>
#include <vector>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "functions.hpp"

class Canny{

private:
	cv::Mat gaussianBlur(cv::Mat& image, unsigned int blurMaskSize);
	void findGradient();
	void nonMaximumSuppression();
	void doubleThreshold();
	void hysteresis();

public:
	Canny(cv::Mat image, unsigned int min, unsigned int max, bool parallel=false);
	~Canny();
};

#endif