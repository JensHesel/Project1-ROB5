#pragma once
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class robot
{
public:
	robot(); //Default constructor
	robot(Point begin, Point pos, vector<Point> dots); //Constructor til hvis man selv vil vælge sine værdier
	void followRute(Mat &input); //Den får robotten til at køre fra start til den har fundet alle punkterne
	
private: 
	Point now, start; //startpunktet og det sted robotten står nu
	vector<Point> found_targets; //koordinater på de punkter der er blevet fundet
	list<Point> path; //Kommer til at indeholde alle koordinaterne fra start og ud til de enkelte punkter

	void moveOne(Mat &map, int &moved, Point &old, Vec3b color); //Flytter robotten en placering når den søger efter targets
	void colvec(Mat &map, vector<Point> &points, vector<Vec3b> &color, vector<Vec3b> &prevcolor); //Farver en vector i farver efter en anden vector, og gemmer de tidligere farver i en tredje vektor
	bool clearline(Mat &map, Point A, Point B); //returnerer false hvis der er noget sort mellem de to punkter og ellers true
	void getToTarget(Mat &map, Point dotPos); //sørger for, at man kommer fra linjen hen til target, når target er fundet
	void colorTarget(Mat map, Vec3b minColor, Vec3b maxColor); //Funktion der farver hele det fundende objekt til en anden farve
	Point laserScan(Mat &map, Vec3b mincolor, Vec3b maxcolor); //Scanner en firkant omkring det punkt man står på
	void getBack(Mat &map); //følger path tilbage til start
	void colorRobot(Mat &map); //Funktion der tegner robotten der hvor den er
};

