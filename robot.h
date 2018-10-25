#pragma once
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class robot
{
public:
	robot();
	robot(Point begin, Point pos, vector<Point> dots);
	~robot();
	void followRute(Mat &input, Mat &output);
	Point laserScan(Mat &map, Scalar color);
	void getBack(Mat &input, Mat &output);

private: 
	Point now, start;
	vector<Point> found_objects;

};

