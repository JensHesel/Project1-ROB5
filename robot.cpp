#include "robot.h"



robot::robot()
{
	start = Point(10, 10);
	now = start;
}

robot::robot(Point begin, Point pos, vector<Point> dots)
{
	start = begin;
	now = pos;
	found_objects = dots;
}

robot::~robot()
{
}

void robot::followRute(Mat &input, Mat &output)
{

}

Point robot::laserScan(Mat &map, Scalar color)
{
	return Point(0,0);
}

void robot::getBack(Mat &input, Mat &output)
{

}
