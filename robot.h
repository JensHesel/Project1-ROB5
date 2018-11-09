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
	void followRute(Mat &input); //Jens
	void getBack(Mat &input, Mat &output); //Asger
	void getToTarget(Mat &map); //Asger

private: 
	Point now, start;
	vector<Point> found_targets;
	Point laserScan(Mat &map, Vec3b color, Point start); //Lærke kigger på den
	void moveOne(Mat &map, int &moved, Point &old, Vec3b color, bool checkold);

};

