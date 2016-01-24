#ifndef _BASE_H_
#define _BASE_H_
#include<opencv2\opencv.hpp>
using namespace cv;
//Ԫ�����ṹ��
typedef struct _electronComponent{
	cv::Point center;
	int radius;
	vector<Point> contours;
	vector<Point> LinkPoint;
}electronComponent;

//���߽ṹ��
struct Cline{
    int numPoint;
    vector<Point> endPoint;
};

struct CSemicircle{
    Point center;
    float radius;
};
//Ԫ����֮��������Խṹ��
struct CDeviceConnectinfo{
	int DeviceName;
	int DeviceLinkNumber;
};

#endif