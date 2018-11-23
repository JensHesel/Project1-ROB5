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

	
	

	

private: 
	Point now, start;
	vector<Point> found_targets;
	list<Point> path;

	void moveOne(Mat &map, int &moved, Point &old, Vec3b color, bool checkold);
	void colvec(Mat &map, vector<Point> &points, vector<Vec3b> &color, vector<Vec3b> &prevcolor);
	bool clearline(Mat &map, Point A, Point B); //returnerer false hvis der er noget sort mellem de to punkter og ellers true
	void getToTarget(Mat &map, Point dotPos); //Asger
	void colorTarget(Mat map, Vec3b minColor, Vec3b maxColor); //Funktion der farver hele det fundende objekt til en anden farve
	Point laserScan(Mat &map, Vec3b mincolor, Vec3b maxcolor); //Lærke kigger på den, Den skal være i private
															   //Lav søgeområdet rundt i stedet for firkantet
	void getBack(Mat &map, Point robot_leave); //Asger
};

