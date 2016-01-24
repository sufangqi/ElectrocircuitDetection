
#ifndef _findLine__
#define _findLine__

#include <opencv2/opencv.hpp>
#include"base.h"
using namespace cv;


void getEndPointOfLine(const Mat &src,vector<Cline> & lines);
void getEndPointOfLine(const Mat src,const vector<electronComponent> DeviceSet,vector<Cline> & lines);
void imageThin(const Mat & src,Mat & dst, int intera);
void findEndPoint(const Mat & src,const vector<Point> & points,vector<Point> &endPoint);
void  binaryImage(const Mat & src,Mat & binary,double thre,int mode);
#endif 
