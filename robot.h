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
	Point laserScan(Mat &map, Vec3b mincolor, Vec3b maxcolor, Point from); //Lærke kigger på den, Den skal være i private
	//Lav søgeområdet rundt i stedet for firkantet

	

private: 
	Point now, start;
	vector<Point> found_targets;
	//Point laserScan(Mat &map, Vec3b mincolor, Vec3b maxcolor, Point start); //Lærke kigger på den
	void moveOne(Mat &map, int &moved, Point &old, Vec3b color, bool checkold);
	void colvec(Mat &map, vector<Point> points, Vec3b color);
	bool clearline(Mat &map, Point A, Point B); //returnerer false hvis der er noget sort mellem de to punkter og ellers true
	//Lav funktion der farver hele det fundende objekt til en anden farve

};

