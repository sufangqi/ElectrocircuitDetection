
#ifndef _findLine__
#define _findLine__

#include <opencv2/opencv.hpp>
using namespace cv;


struct Cline{
    int numPoint;
    vector<Point> endPoint;
};

void getEndPointOfLine(const Mat &src,vector<Cline> & lines);
void imageThin(const Mat & src,Mat & dst, int intera);
void findEndPoint(const Mat & src,const vector<Point> & points,vector<Point> &endPoint);
#endif 
