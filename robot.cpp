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
	found_targets = dots;
}

robot::~robot()
{
}

void robot::followRute(Mat &input, Mat &output)
{

}

Point robot::laserScan(Mat &map, Vec3b color, Point start) //Med neighbors metode
{
	int diameter(100); //Hvor mange pixels ud fra midten skal den scanne
	vector<Point> neighbors;

	//Find alle naboer til startpunktet der ikke er sorte

	//Find alle de udadgående naboer til naboerne i en for lykke, indtil den er kørt igennem et vis antal gange.

	for (int k = -1; k < 2; k++) //all neighbors til startpunktet
	{
		for (int l = -1; l < 2; l++)
		{
			Point look(start.x + k, start.y + l);
			if (map.at<Vec3b>(look) == color) //Hvis man ser den farve pixel der søges efter
			{
				return look;
			}
			else if (map.at<Vec3b>(look) != Vec3b(0, 0, 0))  //Hvis naboen ikke er sort. Her vil startpunktet også komme ind som nabo
			{
					neighbors.push_back(look); //sæt den ind som en nabo
			}
		}
	}

	vector<Point> newneighbors(neighbors);

	for (int i = 0; i < diameter; i++)
	{
		vector<Point> newnewneighbors;
		for (int j = 0; j < newneighbors.size(); j++)//all pixel in neighbors
		{
			for (int k = -1; k < 2; k++) //all neighbors n to pixel
			{
				for (int l = -1; l < 2; l++)
				{
					Point look(newneighbors[j].x + k, newneighbors[j].y + l);
					if (find(neighbors.begin(), neighbors.end(), look) == neighbors.end()) //Hvis den ikke allerede er i listen
					{
						if (map.at<Vec3b>(look) == color) //Hvis man er på den farve pixel der søges efter
						{
							return look;
						}
						else if (map.at<Vec3b>(look) != Vec3b(0, 0, 0))  //Hvis det ikke er et nul
						{
							newnewneighbors.push_back(look); //sæt den ind som en ny nabo
							neighbors.push_back(look);
						}
					}
				}
			}
		}
		newneighbors = newnewneighbors;
	}
	return Point(0, 0); //Hvis den ikke finder et rødt punkt, så returner 0,0
}

void robot::getBack(Mat &input, Mat &output)
{

}

void robot::getToTarget(Mat &map)
{

}
