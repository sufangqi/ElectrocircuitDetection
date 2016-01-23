#include<opencv2\opencv.hpp>
#include"_getdevice.h"
#include"findLine.h"
#include <iostream>

using namespace cv;
using namespace std;

static void help()
{
    cout << "\nThis program demonstrates circle finding with the Hough transform.\n"
            "Usage:\n"
            "./houghcircles <image_name>, Default is ../data/board.jpg\n" << endl;
}

int main(int argc, char** argv)
{
    /*const char* filename = argc >= 2 ? argv[1] : "../data/board.jpg";*/

    Mat cimg = imread("D:/facedata/阿里巴巴开放性项目/电路游戏识别检测/电路游戏识别检测（未连接）/电路游戏8-8.jpg", 1);
	Mat img;
	resize(cimg,cimg,Size(cimg.cols/5,cimg.rows/5));

	vector<electronComponent> DeviceSet;
	getDevice(cimg,DeviceSet);
	Mat Wirelines;
	vector<Cline> lines;
	getEndPointOfLine(cimg,lines);

	Mat SrcCopy=cimg.clone();
	for(int i = 0;i < lines.size() ;i++)
	{
		for(int j = 0;j < lines[i].endPoint.size();j++){
			circle(SrcCopy,lines[i].endPoint[j],5,Scalar(0,0,255),CV_FILLED);
		}
		
	}

	for(int i = 0;i <DeviceSet.size();i++){
		electronComponent DeviceInfo = DeviceSet[i];
	    circle(SrcCopy,Point(DeviceInfo.LinkLineOne[0],DeviceInfo.LinkLineOne[1]),5,Scalar(0,0,255),CV_FILLED);
		circle(SrcCopy,Point(DeviceInfo.LinkLineOne[2],DeviceInfo.LinkLineOne[3]),5,Scalar(0,0,255),CV_FILLED);
		circle(SrcCopy,Point(DeviceInfo.LinkLineTwo[0],DeviceInfo.LinkLineTwo[1]),5,Scalar(0,0,255),CV_FILLED);
		circle(SrcCopy,Point(DeviceInfo.LinkLineTwo[2],DeviceInfo.LinkLineTwo[3]),5,Scalar(0,0,255),CV_FILLED);
		circle(SrcCopy,DeviceInfo.center,DeviceInfo.radius,Scalar(0,0,255),3);
	}
	imshow("LineEndPoint",SrcCopy);
    waitKey();

    return 0;
}
