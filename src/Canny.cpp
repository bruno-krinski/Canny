#include "Canny.hpp"

Canny::Canny(cv::Mat image, unsigned int minThreshold, unsigned int maxThreshold, bool parallel){
	cv::Mat blurImage = gaussianBlur(image, 5, 1.4);
	/*cv::namedWindow("Input Image", cv::WINDOW_AUTOSIZE);
	cv::imshow("Input Image", image);
	cv::namedWindow("Blur Image", cv::WINDOW_AUTOSIZE);
	cv::imshow("Blur  Image", blurImage);
	cv::waitKey(0);*/
	/*for(int i = 0; i < blurImage.rows; ++i){
		for(int j = 0; j < blurImage.cols; ++j){
			std::cout << (double)blurImage.at<uchar>(i,j) << " ";
		}
		std::cout << "\n";
	}*/
	//std::cout << "=========================================\n";
	std::vector<double> gradients;
	std::vector<int> angles;
	cv::Mat gradImage = findGradient(blurImage, gradients, angles);

	/*for(int i = 0; i < gradients.size();++i){
		std::cout << gradients[i] << "\n";
	}*/


	/*for(int i = 0; i < gradImage.rows; ++i){
		for(int j = 0; j < gradImage.cols; ++j){
			std::cout << (double)gradImage.at<uchar>(i,j) << " ";
		}
		std::cout << "\n";
	}*//*
	cv::namedWindow("Input Image", cv::WINDOW_NORMAL);
	cv::resizeWindow("Input Image", 600,600);
	cv::imshow("Input Image", image);
	
	cv::namedWindow("Blur Image", cv::WINDOW_NORMAL);
	cv::resizeWindow("Blur Image", 600,600);
	cv::imshow("Blur Image", blurImage);
	
	//cv::namedWindow("Grad Image", cv::WINDOW_AUTOSIZE);
	//cv::imshow("Grad Image", gradImage);
	
	nonMaximumSuppression(gradImage,gradients,angles);
	/*for(int i = 0; i < gradImage.rows; ++i){
		for(int j = 0; j < gradImage.cols; ++j){
			std::cout << (double)gradImage.at<uchar>(i,j) << " ";
		}
		std::cout << "\n";
	}*/
	/*
	cv::namedWindow("Grad Image", cv::WINDOW_NORMAL);
	cv::resizeWindow("Grad Image", 600,600);
	cv::imshow("Grad Image", gradImage);

	std::vector<int> strength(gradients.size());
	doubleThreshold(gradImage, minThreshold, maxThreshold, gradients, strength);
	

	cv::namedWindow("Threshold Image", cv::WINDOW_NORMAL);
	cv::resizeWindow("Threshold Image", 600,600);
	cv::imshow("Threshold Image", gradImage);

	hysteresis(gradImage, strength);
	cv::namedWindow("Final Image", cv::WINDOW_NORMAL);
	cv::resizeWindow("Final Image", 600,600);
	cv::imshow("Final Image", gradImage);

	cv::waitKey(0);
	*/
}

Canny::~Canny(){

}

std::vector<double> Canny::createKernel(unsigned int blurMaskSize, double sigma){
	std::vector<double> kernel;
	double constant = 2.0 * sigma * sigma;	
	int meanMask = blurMaskSize / 2;

	for(int i = -meanMask; i <= meanMask; ++i){
		for(int j = -meanMask; j <= meanMask; ++j){
			int coordSum = (i*i + j*j);
			double result = std::exp(-(coordSum)/constant)/(M_PI*constant);
			kernel.push_back(result);
		}
	}
	return kernel;
}

cv::Mat Canny::gaussianBlur(const cv::Mat& image, unsigned int blurMaskSize, double sigma){
	std::vector<double> kernel = createKernel(blurMaskSize, sigma);
	
	cv::Mat blurImage(image.rows, image.cols, CV_8UC1, cv::Scalar(0));
	
	int size = blurMaskSize / 2;
	for(int i = 0; i < image.rows; ++i){
		for(int j = 0; j < image.cols; ++j){
			float sum = 0.0;
			for(int k = -size; k <= size; ++k){
				for(int l = -size; l <= size; ++l){
					if(((i+k) > -1) && ((i+k) < image.rows) && ((j+l) > -1) && ((j+l) < image.cols)){
						//std::cout << sum << "+=" << mask[(k+size)*blurMaskSize+(l+size)] << "*" << (double)image.at<uchar>(i+k,j+l) << "\n"; 
						sum += kernel[(k+size)*blurMaskSize+(l+size)] * (double)image.at<uchar>(i+k,j+l);
					}
				}
			}
			blurImage.at<uchar>(i,j) = sum;	
		}
	}
	return blurImage;
}

cv::Mat Canny::findGradient(cv::Mat image, std::vector<double>& gradient, std::vector<int>& angles){
	std::vector<double> Gx(9,0);
	std::vector<double> Gy(9,0);

	Gx[0*3+0] = -1;
	Gx[1*3+0] = -2;
	Gx[2*3+0] = -1;

	Gx[0*3+2] = 1;
	Gx[1*3+2] = 2;
	Gx[2*3+2] = 1;

	Gy[0*3+0] = 1;
	Gy[0*3+1] = 2;
	Gy[0*3+2] = 1;

	Gy[2*3+0] = -1;
	Gy[2*3+1] = -2;
	Gy[2*3+2] = -1;

	/*
	for(int i = 0; i < 3; ++i){
		for(int j = 0; j < 3; ++j){
			std::cout << Gx[i*3+j] << " ";
		}
		std::cout << "\n";
	}

	for(int i = 0; i < 3; ++i){
		for(int j = 0; j < 3; ++j){
			std::cout << Gy[i*3+j] << " ";
		}
		std::cout << "\n";
	}*/

	for(int i = 1; i < image.rows-1; ++i){
		for(int j = 1; j < image.cols-1; ++j){
			float gx = 0.0;
			float gy = 0.0;
			//std::cout << "==================================================" << "\n";
			for(int k = -1; k <= 1; ++k){
				for(int l = -1; l <= 1; ++l){
					//std::cout << Gx[(k+1)*3+(l+1)] << "*" << (double)image.at<uchar>(i+k,j+l) << "\n";
					//std::cout << Gy[(k+1)*3+(l+1)] << "*" << (double)image.at<uchar>(i+k,j+l) << "\n";
					gx += Gx[(k+1)*3+(l+1)] * (double)image.at<uchar>(i+k,j+l);
					gy += Gy[(k+1)*3+(l+1)] * (double)image.at<uchar>(i+k,j+l);
				}
			}
			//std::cout << "gx:" << gx << std::endl;
			//std::cout << "gy:" << gy << std::endl;
			//std::cout << "==================================================" << "\n";
			gradient.push_back(std::sqrt(std::pow(gx,2)+std::pow(gy,2)));
			double angle = (std::atan2(gx,gy)/M_PI)*180;
			if(((angle < 22.5) && (angle > -22.5)) || (angle > 157.5) || (angle < -157.5)){
				angles.push_back(0);
			}
			if(((angle > 22.5) && (angle < 67.5)) || ((angle < -112.5) && (angle > -157.5))){
				angles.push_back(45);
			}
			if (((angle > 67.5) && (angle < 112.5)) || ((angle < -67.5) && (angle > -112.5))){
				angles.push_back(90);
			}
			if (((angle > 112.5) && (angle < 157.5)) || ((angle < -22.5) && (angle > -67.5))){
				angles.push_back(135);
			}
			//angles.push_back(angle);
		}
	}

	cv::Mat gradImage(image.rows-2, image.cols-2, CV_8UC1, cv::Scalar(0));
	for(int i = 1; i < image.rows-1; ++i){
		for(int j = 1; j < image.cols-1; ++j){
			gradImage.at<uchar>((i-1),(j-1)) = image.at<uchar>(i,j);
		}
	}
	return gradImage;
}

void Canny::nonMaximumSuppression(cv::Mat& image, const std::vector<double>& gradients, const std::vector<int>& angles){

	//std::cout << "Image size: " << image.cols*image.rows << "\n";
	//std::cout << "gradients size:" << gradients.size() << "\n";
	//std::cout << "angles size:" << angles.size() << "\n";

	for(int i = 0; i < image.rows; ++i){
		for(int j = 0; j < image.cols; ++j){
			int angle = angles[i*image.cols+j];
			double pixelGradient = gradients[i*image.cols+j];
			//std::cout << pixelGradient << std::endl;
			//std::cout << angle << std::endl;
			//std::cout << "Pixel: " << (double)image.at<uchar>(i,j) << "\n";
			switch(angle){
				case 0: {
					double eastPixelGradient;
					double westPixelGradient;
					if((i+1) >= image.rows){
						eastPixelGradient = 0;	
					} else {
						eastPixelGradient = gradients[(i+1)*image.cols+j];
					}
					if((i-1) < 0){
						westPixelGradient = 0;
					} else {
						westPixelGradient = gradients[(i-1)*image.cols+j];
					}
					//std::cout << "Angle 0:" << std::endl;
					/*std::cout << "Gradient:" << pixelGradient << std::endl;
					std::cout << "eastPixelGradient:" << eastPixelGradient << std::endl;
					std::cout << "westPixelGradient:" << westPixelGradient << std::endl;*/
					if((pixelGradient < eastPixelGradient) || (pixelGradient < westPixelGradient)){
						//std::cout << "Suprimido!" << std::endl;
						image.at<uchar>(i,j) = 0;
					}
					break;
				}
				case 45: {
					double northEastPixelGradient;
					double southWestPixelGradient;
					if((i+1) >= image.rows || (j-1) < 0){
						northEastPixelGradient = 0;	
					} else {
						northEastPixelGradient = gradients[(i+1)*image.cols+j-1];	
					}

					if((i-1) < 0 || (j+1) >= image.cols){
						southWestPixelGradient = 0;	
					} else {
						southWestPixelGradient = gradients[(i-1)*image.cols+j+1];
					}
					//std::cout << "Angle 45:" << std::endl;
					/*std::cout << "Gradient:" << pixelGradient << std::endl;
					std::cout << "northEastPixelGradient:" << northEastPixelGradient << std::endl;
					std::cout << "southWestPixelGradient:" << southWestPixelGradient << std::endl;*/
					if((pixelGradient < northEastPixelGradient) || (pixelGradient < southWestPixelGradient)){
						//std::cout << "Suprimido!" << std::endl;
						image.at<uchar>(i,j) = 0;
					}
					break; 
				}
				case 90: {
					double northPixelGradient;
					double southPixelGradient;
					if((j-1) < 0){
						northPixelGradient = 0;	
					} else {
						northPixelGradient = gradients[i*image.cols+j-1];
					}
					if ((j+1) >= image.cols){
						southPixelGradient = 0;	
					} else {
						southPixelGradient = gradients[i*image.cols+j+1];
					}
					//std::cout << "Angle 90:" << std::endl;
					/*std::cout << "Gradient:" << pixelGradient << std::endl;
					std::cout << "northPixelGradient:" << northPixelGradient << std::endl;
					std::cout << "southPixelGradient:" << southPixelGradient << std::endl;*/
					if((pixelGradient < northPixelGradient) || (pixelGradient < southPixelGradient)){
						//std::cout << "Suprimido!" << std::endl;
						image.at<uchar>(i,j) = 0;
					}
					break;
				}
				case 135: {
					//std::cout << "i" << i << "\n";
					//std::cout << "j" << j << "\n";
					double northWestPixelGradient;
					double southEastPixelGradient;
					if((i-1) < 0 || (j-1) < 0){
						northWestPixelGradient = 0;	
					} else {
						northWestPixelGradient = gradients[(i-1)*image.cols+j-1];
					}
					if((i+1) >= image.rows || (j+1) >= image.cols){
						southEastPixelGradient = 0;	
					} else {
						southEastPixelGradient = gradients[(i+1)*image.cols+j+1];
					}
					//std::cout << "Angle 135:" << std::endl;
					/*std::cout << "Gradient:" << pixelGradient << std::endl;
					std::cout << "northWestPixelGradient:" << northWestPixelGradient << std::endl;
					std::cout << "southEastPixelGradient:" << southEastPixelGradient << std::endl;*/
					if((pixelGradient < northWestPixelGradient) || (pixelGradient < southEastPixelGradient)){
						//std::cout << "Suprimido!" << std::endl;
						image.at<uchar>(i,j) = 0;
					}
					break;
				}
			}
		}
	}
}

void Canny::doubleThreshold(cv::Mat& image, unsigned int minThreshold, unsigned int maxThreshold, const std::vector<double>& gradients, std::vector<int>& strength){
	for(int i = 0; i < image.rows; ++i){
		for(int j = 0; j < image.cols; ++j){
			int index = i*image.cols+j;
			double pixelGradient = gradients[index];
			if(pixelGradient > maxThreshold){
				strength[index] = 2;
			} else if(pixelGradient < maxThreshold && pixelGradient > minThreshold){
				strength[index] = 1;
			} else {
				strength[index] = 0;
				image.at<uchar>(i,j) = 0;
			}
		}
	}
}

void Canny::hysteresis(cv::Mat& image, const std::vector<int>& strength){
	for(int i = 0; i < image.rows; ++i){
		for(int j = 0; j < image.cols; ++j){
			int index = i * image.cols + j;
			if(strength[index] == 1){
				bool supress = true;
				for(int k = -1; k <= 1; ++k){
					for(int l = -1; l <= 1; ++l){
						if(((i+k) > -1) && ((i+k) < image.rows) && ((j+l) > -1) && ((j+l) < image.cols)){
							int index2 = (i+k) * image.cols + (j+l);
							if(strength[index2] == 2){
								supress = false;
							}
						}
					}
				}
				if(supress){
					image.at<uchar>(i,j) = 0;
				}
			}
		}
	}
}