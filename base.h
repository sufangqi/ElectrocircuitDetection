#ifndef _BASE_H_
#define _BASE_H_
#include<opencv2\opencv.hpp>
using namespace cv;
//元器件结构体
typedef struct _electronComponent{
	cv::Point center;
	int radius;
	vector<Point> contours;
	vector<Point> LinkPoint;
}electronComponent;

//电线结构体
struct Cline{
    int numPoint;
    vector<Point> endPoint;
};

struct CSemicircle{
    Point center;
    float radius;
};
//元器件之间的连接性结构体
struct CDeviceConnectinfo{
	int DeviceName;
	int DeviceLinkNumber;
};

#endif