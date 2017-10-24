#include "Canny.hpp"

Canny::Canny(cv::Mat image, unsigned int min, unsigned int max, bool parallel){
	cv::Mat blurImage = gaussianBlur(image,5);
	//v::namedWindow("Display window", cv::WINDOW_AUTOSIZE);
	//cv::imshow("Display window", blurImage);
	//cv::waitKey(0);
	for(int i = 0; i < blurImage.rows; ++i){
		for(int j = 0; j < blurImage.cols; ++j){
			std::cout << (double)blurImage.at<uchar>(i,j) << " ";
		}
		std::cout << "\n";
	}
	
}

Canny::~Canny(){

}

cv::Mat Canny::gaussianBlur(cv::Mat& image ,unsigned int blurMaskSize){
	std::vector<double> mask;

	int size = blurMaskSize / 2;
	for(int i = -size; i <= size; ++i){
		for(int j = -size; j <= size; ++j){
			mask.push_back(gauss(i,j));
			
		}
	}

	for(int i = 0; i < blurMaskSize; ++i){
		for(int j = 0; j < blurMaskSize; ++j){
			std::cout << mask[i*blurMaskSize+j] << " ";
		}
		std::cout << "\n";
	}

	cv::Mat blurImage(image.rows, image.cols, CV_8UC1, cv::Scalar(0));

	for(int i = 0; i < image.rows; ++i){
		for(int j = 0; j < image.cols; ++j){
			float sum = 0.0;
			for(int k = -size; k <= size; ++k){
				for(int l = -size; l <= size; ++l){
					if(((i+k) > -1) && ((i+k) < image.rows) && ((j+l) > -1) && ((j+l) < image.cols)){
						std::cout << sum << "+=" << mask[(k+size)*blurMaskSize+(l+size)] << "*" << (double)image.at<uchar>(i+k,j+l) << "\n"; 
						sum += mask[(k+size)*blurMaskSize+(l+size)] * image.at<uchar>(i+k,j+l);
					}
				}
			}
			blurImage.at<uchar>(i,j) = sum;	
		}
	}
	return blurImage;
}
