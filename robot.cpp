#include "robot.h"

bool operator>(const Vec3b &a, const Vec3b &b);
bool operator<(const Vec3b &a, const Vec3b &b);

robot::robot()
{
	start = Point(10, 11);
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

void robot::followRute(Mat &input)
{
	Point old(0, 0);
	namedWindow("map", WINDOW_NORMAL);
	int movement(0), movementprev(0);
	laserScan(input, Vec3b(0, 0, 230), Vec3b(50, 50, 255));
	while (found_targets.size() < 9) {

		imshow("map", input);
		waitKey(10);
		Vec3b color(0, 255, 0);
		Point target;
		bool blue(true);
		moveOne(input, movement, old, Vec3b(255, 0, 0), false);
		while (movement == movementprev)
		{
			blue = false;
			moveOne(input, movement, old, color, false);
			color -= Vec3b(0, 20, 0);
		}
		/*if (movement == movementprev)
		{
			moveOne(input, movement, old, Vec3b(0,255,0), true);
		}
		if (movement == movementprev)
		{
			moveOne(input, movement, old, Vec3b(0, 255, 0), false);
		}
		if (movement == movementprev)
		{
			moveOne(input, movement, old, Vec3b(0, 105, 0), false);
		}*/
		if (movement > 15 && blue) {
			target = laserScan(input, Vec3b(0, 0, 230), Vec3b(50,50,255));
			movement = 0;
			if (target != Point(0, 0)) //Hvis man har fundet et punkt
			{
				getToTarget(input, target); //I denne kører den både til target og tilbage til start
				path.clear();
			}
		}
		movementprev = movement;

	}
}

void robot::moveOne(Mat &map, int &moved, Point &old, Vec3b color, bool checkold)
{
	vector<Point> rute{ Point(-1, -1), Point(0, -1), Point(1, -1), Point(1, 0), Point(1, 1), Point(0, 1), Point(-1, 1), Point(-1, 0) };
	for (int i = 0; i < rute.size(); i++)
	{
		if (map.at<Vec3b>(Point(now) + rute[i]) == color)
		{
			if (checkold && (Point(now) + rute[i]) != old)
			{
				old = now;
				now = Point(now) + rute[i];
				Vec3b oldcolor = map.at<Vec3b>(now);
				map.at<Vec3b>(now) = Vec3b(230, 0, 255);
				imshow("map", map);
				waitKey(2);
				map.at<Vec3b>(now) = oldcolor - Vec3b(0, 20, 0);
				moved++;
				path.push_back(now);
				return;
			}
			else if (!checkold)
			{
				old = now;
				now = Point(now) + rute[i];
				Vec3b oldcolor = map.at<Vec3b>(now);
				map.at<Vec3b>(now) = Vec3b(230, 0, 255);
				imshow("map", map);
				waitKey(2);
				map.at<Vec3b>(now) = oldcolor;
				if (color == Vec3b(255, 0, 0))
					map.at<Vec3b>(now) = Vec3b(0, 255, 0);
				else
				{
					Vec3b oldcolor = map.at<Vec3b>(now);
					map.at<Vec3b>(now) = oldcolor - Vec3b(0, 20, 0);
				}
				moved++;
				path.push_back(now);
				return;
			}
		}
	}
}

Point robot::laserScan(Mat &map, Vec3b mincolor, Vec3b maxcolor) //Med neighbors metode
{
	int diameter(20); //Hvor mange pixels ud fra midten skal den scanne
	vector<Point> neighbors; //Den der kommer til at indeholde alle de pixels der er blevet testet

	//Find alle naboer til startpunktet der ikke er sorte

	//Find alle de udadgående naboer til naboerne i en for lykke, indtil den er kørt igennem et vis antal gange.

	for (int k = -1; k < 2; k++) //all neighbors til startpunktet
	{
		for (int l = -1; l < 2; l++)
		{
			Point look(now.x + k, now.y + l);
			if (map.at<Vec3b>(look) < maxcolor && map.at<Vec3b>(look) > mincolor) //Hvis man ser den farve pixel der søges efter
			{
				found_targets.push_back(look);
				return look;
			}
			else if (map.at<Vec3b>(look) != Vec3b(0, 0, 0))  //Hvis naboen ikke er sort. Her vil startpunktet også komme ind som nabo
			{
				neighbors.push_back(look); //sæt den ind som en nabo
			}
		}
	}

	vector<Point> newneighbors(neighbors); //Den kommer til at indeholde alle de pixels vi er i gang med at teste

	for (int i = 0; i < diameter; i++)
	{
		vector<Vec3b> colornow(newneighbors.size(), Vec3b(100, 0, 100));
		vector<Vec3b> colorprev(newneighbors.size(), 0);
		colvec(map, newneighbors, colornow, colorprev); //Farver det område der scannes i lilla
		imshow("map", map); //Viser billedet med scanningen på
		colvec(map, newneighbors, colorprev, colornow); //Farver området hvidt igen til næste runde
		waitKey(1);
		vector<Point> newnewneighbors;
		for (int j = 0; j < newneighbors.size(); j++)//all pixel in neighbors
		{
			for (int k = -1; k < 2; k++) //all neighbors n to pixel
			{
				for (int l = -1; l < 2; l++)
				{
					Point look(newneighbors[j].x + k, newneighbors[j].y + l);
					if (find(neighbors.begin(), neighbors.end(), look) == neighbors.end() /*&& find(newneighbors.begin(), newneighbors.end(), look) == newneighbors.end()*/) //Hvis den ikke allerede er i listen
					{
						if (map.at<Vec3b>(look) < maxcolor && map.at<Vec3b>(look) > mincolor) //Hvis man er på den farve pixel der søges efter
						{
							found_targets.push_back(look);
							return look;
						}
						else if (map.at<Vec3b>(look) != Vec3b(0, 0, 0) && clearline(map, look, now))  //Hvis det ikke er et nul, og der ikke er et sort åixel imellem punktet og starten
						{
							newnewneighbors.push_back(look); //sæt den ind som en ny nabo (de noboer vi skal teste i næste runde)
							neighbors.push_back(look);
						}
					}
				}
			}
		}
		//neighbors = newneighbors;
		newneighbors = newnewneighbors;
	}
	return Point(0, 0); //Hvis den ikke finder et rødt punkt, så returner 0,0
}

void robot::colvec(Mat &map, vector<Point> &points, vector<Vec3b> &color, vector<Vec3b> &prevcolor)
{
	for (int i = 0; i < points.size(); i++)
	{
		prevcolor[i] = map.at<Vec3b>(points[i]);
		map.at<Vec3b>(points[i]) = color[i];
	}
}

bool robot::clearline(Mat &map, Point A, Point B)
{
	if (A.x != B.x) //hvis ikke det er en lodret linje
	{
		if (A == Point(48, 22) && B == Point(20, 20))
			int u = 0;
		//double A_x(A.x), A_y(A.y), B_x(B.x), B_y(B.y);
		//double a = (A_y - B_y) / (A_x / B_x);
		double a = ((double)A.y - (double)B.y) / ((double)A.x - (double)B.x); //udregner hælningen på linjen
		double b = (double)A.y - a * (double)A.x; //Udregner skæringen på y-aksen
		if (a < 1 && a > -1) //Hvis det er x der ændres hurtigst på linjen
		{
			int x_start = min(A.x, B.x);
			int x_end = max(A.x, B.x);
			int x_now = x_start;
			while (x_now != x_end) //Indtil alle punkterne imellem dem er tjekket
			{
				int y_now = a * (double)x_now + b;
				if (map.at<Vec3b>(Point(x_now, y_now)) == Vec3b(0, 0, 0))
					return false;
				x_now++;
			}
		}
		else //Hvis det er y der ændres hurtigst på linjen
		{
			int y_start = min(A.y, B.y);
			int y_end = max(A.y, B.y);
			int y_now = y_start;
			while (y_now != y_end) //Indtil alle punkterne imellem dem er tjekket
			{
				int x_now = ((double)y_now - b) / a;
				if (map.at<Vec3b>(Point(x_now, y_now)) == Vec3b(0, 0, 0))
					return false;
				y_now++;
			}
		}
	}
	else //Hvis det er en lodret linje
	{
		int y_start = min(A.y, B.y);
		int y_end = max(A.y, B.y);
		int y_now = y_start;
		while (y_now != y_end) //Indtil alle punkterne imellem dem er tjekket
		{
			if (map.at<Vec3b>(Point(A.x, y_now)) == Vec3b(0, 0, 0))
				return false;
			y_now++;
		}
	}
	return true;
}

void robot::getBack(Mat &map, Point robot_leave)
{
	while (robot_leave != now)
	{
		map.at<Vec3b>(now) = Vec3b(0, 255, 255);
		imshow("map", map);
		waitKey(10);
		if (robot_leave.x > now.x)
		{
			now.x++;
		}
		else if (robot_leave.x < now.x)
		{
			now.x--;
		}

		if (robot_leave.y > now.y)
		{
			now.y++;
		}
		else if (robot_leave.y < now.y)
		{
			now.y--;
		}
		imshow("map", map);
		waitKey(10);
	}

	path.reverse();
	
	for (list<Point>::iterator it = path.begin(); it != path.end(); it++)
	{
		now = *it;
		Vec3b oldcolor = map.at<Vec3b>(now);
		map.at<Vec3b>(now) = Vec3b(230, 0, 255);
		imshow("map", map);
		waitKey(2);
		map.at<Vec3b>(now) = oldcolor;
	}

	/*vector<Point> rute{ Point(-1, -1), Point(0, -1), Point(1, -1), Point(1, 0), Point(1, 1), Point(0, 1), Point(-1, 1), Point(-1, 0) };
	Vec3b color(0, 255, 0);
	Point old = now, oldold = old;
	while (now != start)
	{
		bool moved(false);
		for (int i = 0; i < rute.size(); i++)
		{
			if (map.at<Vec3b>(Point(now) + rute[i]) == color)
			{
				if ((now + rute[i]) != old && (now + rute[i]) != oldold)
				{
					oldold = old;
					old = now;
					now = Point(now) + rute[i];
					moved = true;
					Vec3b oldcolor = map.at<Vec3b>(now);
					map.at<Vec3b>(now) = Vec3b(230, 0, 255);
					imshow("map", map);
					waitKey(100);
					map.at<Vec3b>(now) = oldcolor;

				}
			}
		}
		if (!moved) //Hvis ikke den har rykket sig
		{
			color -= Vec3b(0, 20, 0);
		}
		else if (moved)
		{
			color = Vec3b(0, 255, 0);
		}
	}*/
}

void robot::getToTarget(Mat &map, Point dotPos)
{
	Point robot_leave = now;
	
	while (dotPos != now)
	{
		if (dotPos.x > now.x)
		{
			now.x++;
		}
		else if (dotPos.x < now.x)
		{
			now.x--;
		}
		
		if (dotPos.y > now.y)
		{
			now.y++;
		}
		else if (dotPos.y < now.y)
		{
			now.y--;
		}
		map.at<Vec3b>(now) = Vec3b(0, 255, 255);
		imshow("map", map);
		waitKey(10);
	}


	colorTarget(map, Vec3b(0, 0, 230), Vec3b(50, 50, 255));
	

	getBack(map, robot_leave);

}

void robot::colorTarget(Mat map, Vec3b minColor, Vec3b maxColor)
{
	for (int k = -4; k < 5; k++) //all neighbors til startpunktet
	{
		for (int l = -4; l < 5; l++) {
			if (map.at<Vec3b>(Point(now.x + k, now.y + l)) > minColor && map.at<Vec3b>(Point(now.x + k, now.y + l)) < maxColor) //rød
				map.at<Vec3b>(Point(now.x + k, now.y + l)) = Vec3b(0, 255, 0);
		}
	}
}

bool operator<(const Vec3b &a, const Vec3b &b)
{
	return a[0] < b[0]
		&& a[1] < b[1]
		&& a[2] < b[2];
}

bool operator>(const Vec3b &a, const Vec3b &b)
{
	return a[0] > b[0]
		&& a[1] > b[1]
		&& a[2] > b[2];
}
