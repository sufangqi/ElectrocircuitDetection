#include<opencv2/opencv.hpp>
#include"_getdevice.h"
#include"findLine.h"
#include <iostream>

using namespace cv;
using namespace std;

#define _SHOW_

int main(int argc, char** argv)
{
    Mat cimg = imread("D:/facedata/阿里巴巴开放性项目/电路游戏识别检测/电路游戏识别检测（未连接）/电路游戏5-5.jpg");
	Mat img;
	resize(cimg,cimg,Size(cimg.cols/5,cimg.rows/5));
	
	vector<electronComponent> DeviceSet;
	vector<vector<CDeviceConnectinfo>> DeviceConnectinfoSet;
	vector<CBreakPoint> BreakPointSet;
	vector<Cline> circuitLines;
	getDevice(cimg,DeviceSet);
    getEndPointOfLine(cimg,DeviceSet,circuitLines);
	getDeviceConnectInfo(DeviceSet,circuitLines,DeviceConnectinfoSet,BreakPointSet);
#ifdef _SHOW_
	electronComponent DeviceInfo;
	Point LinkPoint;
    Scalar color(255,255,255);
	Mat SrcCopy= Mat::zeros(cimg.size(),cimg.type());
	for(int i =0;i < DeviceConnectinfoSet.size();i++){
		for(int j = 0;j < DeviceConnectinfoSet[i].size();j++)
		{

				DeviceInfo = DeviceSet[ DeviceConnectinfoSet[i][j].DeviceName];
				LinkPoint  = DeviceInfo.LinkPoint[ DeviceConnectinfoSet[i][j].DeviceLinkNumber];
				circle(SrcCopy,LinkPoint ,5,color,CV_FILLED);
				char str[25];     
				_itoa(i, str,10);
				putText(SrcCopy,string(str),LinkPoint,0,1,Scalar(255,255,255),2);
				circle(SrcCopy,DeviceInfo.center,DeviceInfo.radius,Scalar(0,0,255),1);
			cout<<DeviceConnectinfoSet[i][j].DeviceName<<" ";
		}

		for(int i = 0;i < BreakPointSet.size();i++){
			Point breakPoint = circuitLines[BreakPointSet[i].BreakLineName].endPoint[BreakPointSet[i].BreakPointLabel];
			putText(cimg,string("BreakPoint"),breakPoint ,1,1,Scalar(255,0,255),2);
		}
		imshow("srcCopy", SrcCopy);
	}
#endif
#ifdef _SHOW_

	for(int i = 0;i <DeviceSet.size();i++){
		electronComponent DeviceInfo = DeviceSet[i];
		circle(cimg,DeviceInfo.LinkPoint[0],8,Scalar(0,0,255),CV_FILLED);
		circle(cimg,DeviceInfo.LinkPoint[1],8,Scalar(0,0,255),CV_FILLED);
		circle(cimg,DeviceInfo.center,DeviceInfo.radius,Scalar(0,0,255),3);
	}

	  for(int i = 0;i < circuitLines.size() ;i++){
        for(int j = 0;j < circuitLines[i].endPoint.size();j++){
            circle(cimg,circuitLines[i].endPoint[j],8,Scalar(0,255,0),CV_FILLED);
        }
        
    }
	imshow("Source",cimg);
#endif

	waitKey();
    return 0;
}
