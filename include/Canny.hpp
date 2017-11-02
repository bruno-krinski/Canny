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
	cv::Mat gaussianBlur(const cv::Mat& image, unsigned int blurMaskSize, double sigma);
	cv::Mat findGradient(cv::Mat image, std::vector<double>& gradient, std::vector<int>& angle);
	void nonMaximumSuppression(cv::Mat& image, const std::vector<double>& gradients, const std::vector<int>& angles);
	void doubleThreshold(cv::Mat& image, unsigned int minThreshold, unsigned int maxThreshold,const std::vector<double>& gradients, std::vector<int>& strength);
	void hysteresis(cv::Mat& image, const std::vector<int>& strength);
	std::vector<double> createKernel(unsigned int blurMaskSize, double sigma);

public:
	Canny(cv::Mat image, unsigned int minThreshold, unsigned int maxThreshold, bool parallel = false);
	~Canny();
};

#endif