#include "findLine.h"
//获取图像中线的端点坐标，端点可能大于2个
//#define _SHOW_
void imageThin(const Mat& src,Mat& dst, int intera){
    if(src.type()!=CV_8UC1){
        printf("only binary image\n");
        return;
    }
    //非原地操作时候，copy src到dst
    if(dst.data!=src.data){
        src.copyTo(dst);
    }
    int i, j, n;
    int width, height;
    width = src.cols -1;
    //之所以减1，是方便处理8邻域，防止越界
    height = src.rows -1;
    int step = (int)src.step;
    int  p2,p3,p4,p5,p6,p7,p8,p9;
    uchar* img;
    bool ifEnd;
    int A1;
    Mat tmpimg;
    //n表示迭代次数
    for(n = 0; n<intera; n++){
        dst.copyTo(tmpimg);
        ifEnd = false;
        img = tmpimg.data;
        for(i = 1; i < height; i++){
            img += step;
            for(j =1; j<width; j++){
                uchar* p = img + j;
                A1 = 0;
                if( p[0] > 0){
                    if(p[-step]==0&&p[-step+1]>0){ //p2,p3 01模式
                        A1++;
                    }
                    if(p[-step+1]==0&&p[1]>0) //p3,p4 01模式
                    {
                        A1++;
                    }
                    if(p[1]==0&&p[step+1]>0) //p4,p5 01模式
                    {
                        A1++;
                    }
                    if(p[step+1]==0&&p[step]>0) //p5,p6 01模式
                    {
                        A1++;
                    }
                    if(p[step]==0&&p[step-1]>0) //p6,p7 01模式
                    {
                        A1++;
                    }
                    if(p[step-1]==0&&p[-1]>0) //p7,p8 01模式
                    {
                        A1++;
                    }
                    if(p[-1]==0&&p[-step-1]>0) //p8,p9 01模式
                    {
                        A1++;
                    }
                    if(p[-step-1]==0&&p[-step]>0) //p9,p2 01模式
                    {
                        A1++;
                    }
                    p2 = p[-step]>0?1:0;
                    p3 = p[-step+1]>0?1:0;
                    p4 = p[1]>0?1:0;
                    p5 = p[step+1]>0?1:0;
                    p6 = p[step]>0?1:0;
                    p7 = p[step-1]>0?1:0;
                    p8 = p[-1]>0?1:0;
                    p9 = p[-step-1]>0?1:0;
                    if((p2+p3+p4+p5+p6+p7+p8+p9)>1 && (p2+p3+p4+p5+p6+p7+p8+p9)<7  &&  A1==1){
                        if((p2==0||p4==0||p6==0)&&(p4==0||p6==0||p8==0)) //p2*p4*p6=0 && p4*p6*p8==0
                        {
                            dst.at<uchar>(i,j) = 0; //满足删除条件，设置当前像素为0
                            ifEnd = true;
                        }
                    }
                }
            }
        }
        dst.copyTo(tmpimg);
        img = tmpimg.data;
        for(i = 1; i < height; i++)
        {
            img += step;
            for(j =1; j<width; j++)
            {
                A1 = 0;
                uchar* p = img + j;
                if( p[0] > 0)
                {
                    if(p[-step]==0&&p[-step+1]>0) //p2,p3 01模式
                    {
                        A1++;
                    }
                    if(p[-step+1]==0&&p[1]>0) //p3,p4 01模式
                    {
                        A1++;
                    }
                    if(p[1]==0&&p[step+1]>0) //p4,p5 01模式
                    {
                        A1++;
                    }
                    if(p[step+1]==0&&p[step]>0) //p5,p6 01模式
                    {
                        A1++;
                    }
                    if(p[step]==0&&p[step-1]>0) //p6,p7 01模式
                    {
                        A1++;
                    }
                    if(p[step-1]==0&&p[-1]>0) //p7,p8 01模式
                    {
                        A1++;
                    }
                    if(p[-1]==0&&p[-step-1]>0) //p8,p9 01模式
                    {
                        A1++;
                    }
                    if(p[-step-1]==0&&p[-step]>0) //p9,p2 01模式
                    {
                        A1++;
                    }
                    p2 = p[-step]>0?1:0;
                    p3 = p[-step+1]>0?1:0;
                    p4 = p[1]>0?1:0;
                    p5 = p[step+1]>0?1:0;
                    p6 = p[step]>0?1:0;
                    p7 = p[step-1]>0?1:0;
                    p8 = p[-1]>0?1:0;
                    p9 = p[-step-1]>0?1:0;
                    if((p2+p3+p4+p5+p6+p7+p8+p9)>1 && (p2+p3+p4+p5+p6+p7+p8+p9)<7  &&  A1==1)
                    {
                        if((p2==0||p4==0||p8==0)&&(p2==0||p6==0||p8==0)) //p2*p4*p8=0 && p2*p6*p8==0
                        {
                            dst.at<uchar>(i,j) = 0; //满足删除条件，设置当前像素为0
                            ifEnd = true;
                        }
                    }
                }
            }
        }
        
        //如果两个子迭代已经没有可以细化的像素了，则退出迭代
        if(!ifEnd) break;
    }
    
}
void findEndPoint(const Mat & src,const vector<Point> & points,vector<Point> & endPoint){
    //  8邻域法 求端点
    for (size_t i = 0; i < points.size();i++) {
        int row = points[i].y;
        int col = points[i].x;
        if(row < 1 || col < 1 || row > src.rows - 1 || col > src.cols - 1 ){
            continue;
        }
        int count = 0;
        if (src.at<uchar>(row - 1,col - 1) > 0) count++;
        if (src.at<uchar>(row - 1,col) > 0) count++;
        if (src.at<uchar>(row - 1,col + 1) > 0) count++;
        if (src.at<uchar>(row ,col - 1) > 0) count++;
        if (src.at<uchar>(row ,col + 1) > 0) count++;
        if (src.at<uchar>(row + 1,col -1) > 0) count++;
        if (src.at<uchar>(row + 1,col) > 0) count++;
        if (src.at<uchar>(row + 1,col + 1) > 0) count++;
        
        if (count == 1) {
            endPoint.push_back(points[i]);
        }
    }
}
void  getEndPointOfLine(const Mat & src,vector<Cline> & lines){
    
    //二值化
    Mat gray;
    cvtColor(src,gray,CV_BGR2GRAY);
    Mat binary;
    double thre = 10;
    double maxval = 255;
    threshold(gray, binary, thre, maxval,CV_THRESH_BINARY_INV);
    medianBlur(binary, binary, 3);
    //细化
    Mat bin;
    int intera = 8;
    imageThin(binary,bin,intera);
    //寻找曲线的端点
    vector<vector<Point> > contours;
    Mat copyBin;
    bin.copyTo(copyBin);
    findContours(copyBin, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    for (size_t i = 0; i < contours.size(); i++) {
        if (contours[i].size() < 10) {
            continue;
        }
        vector<Point> endPoint;
        findEndPoint(bin,contours[i], endPoint);
        if (endPoint.size() > 0) {
            Cline line;
            line.numPoint = (int)endPoint.size();
            for (size_t k = 0;k < endPoint.size();k++) {
                line.endPoint.push_back(endPoint[k]);
            }
            lines.push_back(line);
        }
    }
#ifdef _SHOW_
	Mat SrcCopy=src.clone();
	for(int i = 0;i < lines.size() ;i++)
	{
		for(int j = 0;j < lines[i].endPoint.size();j++){
			circle(SrcCopy,lines[i].endPoint[j],3,Scalar(0,0,255),CV_FILLED);
		}
		
	}
	imshow("LineEndPoint",SrcCopy);
#endif
}
