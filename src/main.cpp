#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "Canny.hpp"

int main(int argc, char** argv){

	/*if(argc != 2){
		std::cout << "Usage: ./canny <image>" << std::endl;
		return -1;
	}

	cv::Mat image = cv::imread(argv[1],0);

	if(!image.data){
		std::cout << "Could not open or find the image" << std::endl;
		return -1;
	}*/

	cv::Mat image(5, 5, CV_8UC1, cv::Scalar(100));
	for(int i = 0; i < image.rows; ++i){
		for(int j = 0; j < image.cols; ++j){
			std::cout << (double)image.at<uchar>(i,j) << " ";
		}
		std::cout << "\n";
	}
	Canny canny(image,100,200);

	/*cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE);
	cv::imshow("Display window", image);
	cv::waitKey(0);
	*/

	return 0;
}