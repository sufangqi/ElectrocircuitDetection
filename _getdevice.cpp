#include"_getdevice.h"
using namespace std;
//#define _SHOW_
void SortLocation(vector<double> T,vector<int> &label)
{
	for(int i = 0;i < T.size();i++)
		for(int j= i;j < T.size();j++)
		{
			if(T[i] < T[j])
			{
				int temp = T[i];
				T[i] = T[j];
				T[j] = temp;

				int tempLabel = label[i];
				label[i] = label[j];
				label[j] = tempLabel;	
			}
		}
}
void EnhanceEdge(Mat &img,Mat &EnhancedImage)
{
	Mat ImgX,ImgY;
	Mat GrayImg;
	Sobel(img,ImgX,CV_16S,1,0,3);
	Sobel(img,ImgY,CV_16S,0,1,3);
	convertScaleAbs(ImgX,ImgX);
	convertScaleAbs(ImgY,ImgY);
	EnhancedImage = abs(ImgX)+abs(ImgY);
}
void GetHist(Mat &img,Mat &mask, MatND &hist)
{
  int h_bins = 30; int s_bins = 32;
  int histSize[] = { h_bins, s_bins };
  float h_range[] = { 0, 179 };
  float s_range[] = { 0, 255 };
  const float* ranges[] = { h_range, s_range };

  int channels[] = { 0, 1 };
  Mat hsv;
  cvtColor(img,hsv,CV_BGR2HSV);
  /// Get the Histogram and normalize it
  calcHist( &hsv, 1, channels, mask, hist, 2, histSize, ranges, true, false );
  normalize( hist, hist, 0, 255, NORM_MINMAX, -1, Mat() );
}
void calcProject(Mat &img,MatND hist,Mat&backproj)
{
	 int channels[] = { 0, 1 };
	 float h_range[] = { 0, 179 };
     float s_range[] = { 0, 255 };
     const float* ranges[] = { h_range, s_range };
	 Mat hsv;
	 cvtColor(img,hsv,CV_BGR2HSV);
	 calcBackProject( &hsv, 1, channels, hist, backproj, ranges, 1, true );
#ifdef _SHOW_
	 imshow("backproj",backproj);
#endif
	 threshold(backproj,backproj,200,255,CV_THRESH_BINARY);
	 dilate(backproj,backproj,Mat());
}
void ExtractColor(Mat img,MatND &hist)
{
	Mat MaskImg ;
	cvtColor(img,MaskImg,CV_BGR2GRAY);
	adaptiveThreshold(MaskImg,MaskImg,255,ADAPTIVE_THRESH_MEAN_C,CV_THRESH_BINARY_INV,71,10);
	GetHist(img,MaskImg,hist);
#ifdef _SHOW_
	imshow("Gray",MaskImg);
#endif
}
void findDeviceLinkPoint(Mat backProjectImg,Point &centre,Point &LinkPointOne,Point &LinkPointTwo,vector<Point> &DeviceContours)
{
    vector<vector<Point>> contours;
#ifdef _SHOW_
	Mat DisplayImg = Mat::zeros(backProjectImg.size(),CV_8UC3);;
#endif
	findContours(backProjectImg,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);

	double TmpArea = 0;
	for(int i = 0;i < contours.size();i++)
	{
		double Tmp = contourArea(contours[i]);
		if(Tmp > TmpArea)
		{
			TmpArea = Tmp;
			DeviceContours = contours[i];
		}
	}

	//计算中心
	Moments mom = moments(DeviceContours);
	centre = Point(mom.m10 /mom.m00,mom.m01 / mom.m00);

	//找断点的连接线
	vector<int> label;
	vector<double> distance;
	for(int i =0 ;i < DeviceContours.size();i++){
		distance.push_back(norm(centre - DeviceContours[i]));
		label.push_back(i);
	}
	SortLocation(distance,label);
	Point EndPoint_1 = DeviceContours[label[0]];
	Point EndPoint_2 = DeviceContours[label[1]];
	int labe2 = -1;
	
	Point EndPoint_3 = DeviceContours[label[2]];
	int label3 = -1;
	Point EndPoint_4 = DeviceContours[label[3]];

	//距离最远点距离大于30个像素的点，作为第二个最远点
	for(int i = 0;i < label.size();i++){
		if(norm(DeviceContours[label[i]] - EndPoint_1) > 30){
			EndPoint_2 = DeviceContours[label[i]];
			labe2 = i;
			break;
		}
    }
	//距离前两个点距离大于30个像素的点，作为第三个最远点
   for(int i = labe2 + 1;i < label.size();i++){
	   if(norm(DeviceContours[label[i]] - EndPoint_1) >30 && norm(DeviceContours[label[i]] - EndPoint_2) >30){
          EndPoint_3 = DeviceContours[label[i]];
		  label3 = i;
		  break;
	   }
	}
   //距离前三个点距离大于30个像素的点，最为第四个最远点
   for(int i = label3 + 1;i < label.size();i++){
	   if(norm(DeviceContours[label[i]] - EndPoint_1) >30 && norm(DeviceContours[label[i]] - EndPoint_2) >30 && norm(DeviceContours[label[i]] - EndPoint_3) >30){
          EndPoint_4 = DeviceContours[label[i]];
		  break;
	   }
	}
	
	
	//判断哪两个点为一组
	double Dis1_2 = norm(EndPoint_1 - EndPoint_2);
	double Dis1_3 = norm(EndPoint_1 - EndPoint_3);
	double Dis1_4 = norm(EndPoint_1 - EndPoint_4);
	
	if(Dis1_2 < Dis1_3 && Dis1_2 < Dis1_4){
		LinkPointOne = Point((EndPoint_1.x + EndPoint_2.x) / 2,(EndPoint_1.y + EndPoint_2.y) / 2);
		LinkPointTwo = Point((EndPoint_3.x + EndPoint_4.x) / 2,(EndPoint_3.y + EndPoint_4.y) / 2);
	}else if(Dis1_3 < Dis1_2 && Dis1_3 < Dis1_4){
		LinkPointOne = Point((EndPoint_1.x + EndPoint_3.x) / 2,(EndPoint_1.y + EndPoint_3.y) / 2);
		LinkPointTwo = Point((EndPoint_2.x + EndPoint_4.x) / 2,(EndPoint_2.y + EndPoint_4.y) / 2);
	}else if(Dis1_4 < Dis1_2 && Dis1_4 < Dis1_3){
		LinkPointOne = Point((EndPoint_1.x + EndPoint_4.x) / 2,(EndPoint_1.y + EndPoint_4.y) / 2);
		LinkPointTwo = Point((EndPoint_2.x + EndPoint_3.x) / 2,(EndPoint_2.y + EndPoint_3.y) / 2);
	}


#ifdef _SHOW_
	vector<vector<Point>> TmpContour;
	TmpContour.push_back(DeviceContours);
	drawContours(DisplayImg,TmpContour,0,Scalar(255,0,255));
	circle(DisplayImg,DeviceContours[label[0]],3,Scalar(255),CV_FILLED);
	circle(DisplayImg,DeviceContours[label[1]],3,Scalar(255),CV_FILLED);
	circle(DisplayImg,DeviceContours[label[2]],3,Scalar(255),CV_FILLED);
	circle(DisplayImg,DeviceContours[label[3]],3,Scalar(255),CV_FILLED);
    circle(DisplayImg,centre,3,Scalar(255),CV_FILLED);
	line(DisplayImg,Point(LinkLineOne[0],LinkLineOne[1]),Point(LinkLineOne(2),LinkLineOne[3]),Scalar(255,0,0));
	line(DisplayImg,Point(LinkLineTwo[0],LinkLineTwo[1]),Point(LinkLineTwo[2],LinkLineTwo[3]),Scalar(255,0,0));
	imshow("LinkPoint", DisplayImg);
#endif
}
void ExtractDeviceComponentInfo(Mat img,Vec3i centre,electronComponent &DeviceInfo)
{
	
	Vec3i c = centre;
	Rect CircleRect;

	if(max(c[0] - c[2],0) + c[2]*2 < img.cols && max(c[1] - c[2],0) + c[2]*2 <img.rows)
		 CircleRect = Rect(max(c[0] - c[2],0),max(c[1] - c[2],0),c[2]*2,c[2]*2);

	Mat DeviceTmp = img(CircleRect).clone();
	MatND hist;
	Mat backProject;
	ExtractColor(DeviceTmp,hist);
	Point ROIorigin(max(int(c[0] - c[2]*1.5),0),max(int(c[1] - c[2]*1.5),0));
	int ROIWidth = c[2]*3;
	int ROIHeght = c[2]*3;
	Mat DeviceROI = img(Rect(ROIorigin.x,ROIorigin.y,ROIWidth,ROIHeght));
	calcProject(DeviceROI,hist,backProject);

#ifdef _SHOW_
	Mat DeviceComponent(Size(DeviceROI.cols *2,DeviceROI.rows),CV_8UC3);
	Mat TmpImg;
	cvtColor(backProject,TmpImg,CV_GRAY2BGR);
	DeviceROI.copyTo(DeviceComponent(Rect(0,0,DeviceROI.cols,DeviceROI.rows)));
	TmpImg.copyTo(DeviceComponent(Rect(DeviceROI.cols,0,DeviceROI.cols,DeviceROI.rows)));
	imshow("DeviceComponent",DeviceComponent);
#endif
	Point centreInbackProject = Point(c[2]*1.5,c[2]*1.5);//在ROI图像圆心的位置
	Point LinkPoint_1,linkPoint_2;
	vector<Point> DeviceContours;
	findDeviceLinkPoint(backProject,centreInbackProject, LinkPoint_1,linkPoint_2,DeviceContours);

	DeviceInfo.LinkPoint.push_back(LinkPoint_1 + ROIorigin);
	DeviceInfo.LinkPoint.push_back(linkPoint_2 + ROIorigin);

	DeviceInfo.center = centreInbackProject + ROIorigin;
	DeviceInfo.radius =centre[2];
	
	for(int i = 0;i < DeviceContours.size();i++){
		DeviceContours[i] = DeviceContours[i] + ROIorigin;
	}
	DeviceInfo.contours = DeviceContours;
}
void getDevice(Mat img,vector<electronComponent> &DeviceSet)
{ 
	Mat cimg = img.clone();
	if(img.channels() == 3){
		cvtColor(img,img,CV_BGR2GRAY);
	}
	EnhanceEdge(img,img);
	vector<Vec3f> circles;
    HoughCircles(img, circles, CV_HOUGH_GRADIENT, 2, 30,
                 250, 180, 20,100// change the last two parameters
                                // (min_radius & max_radius) to detect larger circles
                 );
	for(int i =0;i<circles.size();i++){
		Vec3i c = circles[i];
		electronComponent DeviceInfo;
		ExtractDeviceComponentInfo(cimg,c,DeviceInfo);
		DeviceSet.push_back(DeviceInfo);
	}
#ifdef _SHOW_
	cvtColor(img,img,CV_GRAY2BGR);
	for(int i = 0;i <DeviceSet.size();i++){
		electronComponent DeviceInfo = DeviceSet[i];
	    circle(img,Point(DeviceInfo.LinkLineOne[0],DeviceInfo.LinkLineOne[1]),5,Scalar(0,0,255),CV_FILLED);
		circle(img,Point(DeviceInfo.LinkLineOne[2],DeviceInfo.LinkLineOne[3]),5,Scalar(0,0,255),CV_FILLED);
		circle(img,Point(DeviceInfo.LinkLineTwo[0],DeviceInfo.LinkLineTwo[1]),5,Scalar(0,255,255),CV_FILLED);
		circle(img,Point(DeviceInfo.LinkLineTwo[2],DeviceInfo.LinkLineTwo[3]),5,Scalar(0,255,255),CV_FILLED);
		circle(img,DeviceInfo.center,DeviceInfo.radius,Scalar(0,0,255),3);
		imshow("Result",img);
	}
	imshow("SourceImg",cimg);
#endif
}

void getDeviceConnectInfo(vector<electronComponent> DeviceSet,vector<Cline> lines,vector<vector<CDeviceConnectinfo>> &DeviceConnectinfoSet)
{
	int thresOfConnect = 10;
	for(int i = 0;i < lines.size();i++){
		vector<CDeviceConnectinfo> EveryLineDeviceConnectinfo;
		for(int j = 0;j < lines[i].numPoint;j++){
			CDeviceConnectinfo DeviceConnectinfo = {-1,-1};
			double Dist_min = 10000000000000000;
			for(int k = 0;k < DeviceSet.size();k++){   
				double Dist = 0;
				int Label = -1;
				double Dist_0 = norm(lines[i].endPoint[j] - DeviceSet[k].LinkPoint[0]);//和元器件的第一个连接点的距离
				double Dist_1 = norm(lines[i].endPoint[j] - DeviceSet[k].LinkPoint[1]);//和元器件的第二个连接点的距离
				
					if(Dist_0 < Dist_1){
						Dist = Dist_0;
						Label = 0;
					}else{
						Dist = Dist_1;
						Label = 1;
					}
					//如果存在距离小于十个像素的链接点，则认为与该器件的一个端点相连
					if(Dist < Dist_min && Dist < thresOfConnect){
						Dist_min = Dist;
						DeviceConnectinfo.DeviceName = k;
						DeviceConnectinfo.DeviceLinkNumber = Label;
					}
			}
			EveryLineDeviceConnectinfo.push_back(DeviceConnectinfo);
		}
		DeviceConnectinfoSet.push_back(EveryLineDeviceConnectinfo);
	}
}
