#include "functions.hpp"

double gauss(int x, int y){
	double pi = 3.1415;
	double a = 2*1.4*1.4;
	double b = 1 / (a*pi);
	double c = (x*x+y*y)/a;
	return b*std::exp(-c);
}

