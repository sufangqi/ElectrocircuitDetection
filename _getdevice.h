#ifndef _GETDEVICE__H
#define _GETDEVICE__H
#include<opencv2\opencv.hpp>
#include"base.h"
using namespace cv;
using namespace std;


void SortLocation(vector<double> T,vector<int> &label);

void EnhanceEdge(Mat &img,Mat &EnhancedImage);

void GetHist(Mat &img,Mat &mask, MatND &hist);

void calcProject(Mat &img,MatND hist,Mat&backproj);

void ExtractColor(Mat img,MatND &hist);

void findDeviceLinkPoint(Mat backProjectImg,Point &centre,Point &LinkPointOne,Point &LinkPointTwo,vector<Point> &DeviceContours);

void ExtractDeviceComponentInfo(Mat img,Vec3i centre,electronComponent &DeviceInfo);

void getDevice(Mat img,vector<electronComponent> &DeviceSet);

void getDeviceConnectInfo(vector<electronComponent> DeviceSet,vector<Cline> lines,vector<vector<CDeviceConnectinfo>> &DeviceConnectinfoSet,vector<CBreakPoint> &BreakPointSet);
#endif