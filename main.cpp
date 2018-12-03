#include<opencv2/opencv.hpp>
#include"robot.h"

using namespace std;
using namespace cv;

void brushfire(Mat &img);
void drawVoronoi(Mat &img);
void onMouse(int event, int x, int y, int, void* param);
void lapning(Mat &img);
void lapningLige(Mat &img);
void fjernTrekant(Mat &img);

int main()
{
	//Mat map = imread("map2.PNG");
	//namedWindow("Original", WINDOW_AUTOSIZE);
	//imshow("Original", map);
	//robot b;
	//cout << "found point: " << b.laserScan(map, Vec3b(0, 0, 230), Vec3b(50, 50, 255), Point(100, 100));

	///*double time = clock();
	//brushfire(map);
	//time = clock() - time;
	//cout << "It took this many seconds: " << time / CLOCKS_PER_SEC;
	//namedWindow("Brushfired map", WINDOW_AUTOSIZE);
	//imshow("Brushfired map", map);
	//drawVoronoi(map);
	//imwrite("linedmap2.png", map);
	//namedWindow("map", WINDOW_NORMAL);
	//imshow("map", map);
	//Rect roi(120, 90, 200, 170);
	//Mat temp = map(roi);
	//namedWindow("temp", WINDOW_NORMAL);
	//imshow("temp", temp);
	//Mat map = imread("linedmap.png");*/
	//robot a;
	//a.followRute(map);
	//setMouseCallback("Brushfired map", onMouse, &map);
	//waitKey(0);

	robot a;
	Mat map = imread("map2.png");
	namedWindow("Original", WINDOW_AUTOSIZE);
	imshow("Original", map);
	//double time = clock();
	brushfire(map);
	//time = clock() - time;
	//cout << "It took this many seconds: " << time / CLOCKS_PER_SEC;
	imwrite("brushfiredmap2.png", map);
	//Mat map = imread("brushfiredmap.png");
	namedWindow("Brushfired map", WINDOW_NORMAL);
	imshow("Brushfired map", map);
	drawVoronoi(map);
	namedWindow("map", WINDOW_NORMAL);
	imshow("map", map);
	a.followRute(map);
	imshow("map", map);
	waitKey(0);

}

void onMouse(int event, int x, int y, int, void* param)
{
	Mat map = *((Mat*)param);
	if (event == EVENT_LBUTTONDOWN)
	{
		cout << "x: " << x << ", y: " << y << endl;
		cout << map.at<Vec3b>(Point(x, y)) << endl;
	}
}

void brushfire(Mat &img)
{
	
	vector<Point> neighbors;

	for (int i = 1; i < img.cols - 1; i++)
	{
		for (int j = 1; j < img.rows - 1; j++)
		{
			if (img.at<Vec3b>(Point(i, j)) == Vec3b(0, 0, 0)) //Hvis det pixel vi står på er sort
			{
				for (int k = -1; k < 2; k++) //all neighbors n
				{
					for (int l = -1; l < 2; l++)
					{
						int x = i + k;
						int y = j + l;
						if (img.at<Vec3b>(Point(x, y)) == Vec3b(255, 255, 255)) //Hvis den nabo er hvid
						{
							if (find(neighbors.begin(), neighbors.end(), Point(x, y)) == neighbors.end()) //Hvis den ikke allerede er i listen
							{
								neighbors.push_back(Point(x, y)); //sæt den ind som en nabo
							}
						}
						
					}
				}
			}
		}
	}
	double B(3), G(3), R(3);
	while (neighbors.size() != 0)
	{
		Vec3b color(B, G, R);
		vector<Point> newneighbors;
		for (int i = 0; i < neighbors.size(); i++)//all pixel in neighbors
		{
			if (img.at<Vec3b>(neighbors[i]) == Vec3b(255, 255, 255)) //Hvis den ikke er blevet givet en værdi endnu
				img.at<Vec3b>(neighbors[i]) = color;
			for (int k = -1; k < 2; k++) //all neighbors n to pixel
			{
				for (int l = -1; l < 2; l++)
				{
					Point now = neighbors[i];
					int x = now.x + k;
					int y = now.y + l;
					if (img.at<Vec3b>(Point(x, y)) == Vec3b(255, 255, 255)) //Hvis den nabo er hvid
					{
						if (find(newneighbors.begin(), newneighbors.end(), Point(x, y)) == newneighbors.end()) //Hvis den ikke allerede er i listen
						{
							newneighbors.push_back(Point(x, y)); //sæt den ind som en nabo
						}
					}
				}
			}
		}
		B+=2;
		G+=2;
		R+=2;
		neighbors = newneighbors;
		cout << "in progress " << neighbors.size() << " round: " << B << endl;
	}
}

void drawVoronoi(Mat &img)
{
	Mat compare = img.clone();
	for (int i = 1; i < img.cols - 1; i++)
	{
		for (int j = 1; j < img.rows - 1; j++)
		{
			if (img.at<Vec3b>(Point(i, j)) == Vec3b(0, 0, 0) || ( (int)img.at<Vec3b>(Point(i, j))[2] > 230 && (int)img.at<Vec3b>(Point(i, j))[0] < 50)) //Hvis man står på et sort eller et rødt pixel
			{ }
			else
			{
				int countb(0);
				int counte(0);
				for (int k = -1; k < 2; k++)
				{
					for (int l = -1; l < 2; l++)
					{
						if (k == 0 && l == 0)
						{ }
						else if (compare.at<Vec3b>(Point(i, j))[2] > compare.at<Vec3b>(Point(i + k, j + l))[2])
						{
							countb++;
						}
						else if (compare.at<Vec3b>(Point(i, j))[2] == img.at<Vec3b>(Point(i + k, j + l))[0])
						{
							counte++;
						}
						if (countb > 3 || counte > 4)
						{
							img.at<Vec3b>(Point(i, j)) = Vec3b(255, 0, 0);
						}

					}
				}
			}
		}
	}

	//Lapning
	lapning(img);
	lapningLige(img);
	fjernTrekant(img);
}

void lapning(Mat &img)
{
	int countblue(0);
	for (int i = 1; i < img.cols - 1; i++) //løber x-koordinater igennem
	{
		for (int j = 1; j < img.rows - 1; j++) //løber y-koordinater igennem
		{
			for (int k = -1; k < 2; k++) //3 x 3 maske
			{
				for (int l = -1; l < 2; l++)
				{
					if (k == 0 && l == 0) //Hvis man er i midten
					{ }
					else if (img.at<Vec3b>(Point(i + k, j + l)) == Vec3b(255, 0, 0)) //Hvis den man tjekker er blå
						countblue++;
				}
			}

			if (countblue == 2) //indsætter punkter
			{
				// Tjekker (-1,-1) og (1,1) - Diagonalt
				if (img.at<Vec3b>(Point(i - 1, j - 1)) == Vec3b(255, 0, 0) && img.at<Vec3b>(Point(i + 1, j + 1)) == Vec3b(255, 0, 0))
					img.at<Vec3b>(Point(i, j)) = Vec3b(255, 0, 0);

				// Tjekker (-1,1) og (1,-1) - Diagonalt
				if (img.at<Vec3b>(Point(i - 1, j + 1)) == Vec3b(255, 0, 0) && img.at<Vec3b>(Point(i + 1, j - 1)) == Vec3b(255, 0, 0))
					img.at<Vec3b>(Point(i, j)) = Vec3b(255, 0, 0);

				for (int k = -1; k < 2; k++)
				{
					if (img.at<Vec3b>(Point(i - 1, j)) == Vec3b(255, 0, 0) && img.at<Vec3b>(Point(i + 1, j + k)) == Vec3b(255, 0, 0))
						img.at<Vec3b>(Point(i, j)) = Vec3b(255, 0, 0);

					if (img.at<Vec3b>(Point(i + 1, j)) == Vec3b(255, 0, 0) && img.at<Vec3b>(Point(i - 1, j + k)) == Vec3b(255, 0, 0))
						img.at<Vec3b>(Point(i, j)) = Vec3b(255, 0, 0);

					if (img.at<Vec3b>(Point(i, j - 1)) == Vec3b(255, 0, 0) && img.at<Vec3b>(Point(i + k, j + 1)) == Vec3b(255, 0, 0))
						img.at<Vec3b>(Point(i, j)) = Vec3b(255, 0, 0);

					if (img.at<Vec3b>(Point(i, j + 1)) == Vec3b(255, 0, 0) && img.at<Vec3b>(Point(i + k, j - 1)) == Vec3b(255, 0, 0))
						img.at<Vec3b>(Point(i, j)) = Vec3b(255, 0, 0);
				}
			}
			countblue = 0;
		}
	}
}

void lapningLige(Mat &img)
{
	for (int i = 1; i < img.cols - 1; i++)
	{
		for (int j = 1; j < img.rows - 1; j++)
		{
			// Tjekker (-1,0) og (1,0) - Over under
			if (img.at<Vec3b>(Point(i - 1, j)) == Vec3b(255, 0, 0) && img.at<Vec3b>(Point(i + 1, j)) == Vec3b(255, 0, 0))
				img.at<Vec3b>(Point(i, j)) = Vec3b(255, 0, 0);

			// Tjekker (0,-1) og (0,1) - Venstre højre
			if (img.at<Vec3b>(Point(i, j - 1)) == Vec3b(255, 0, 0) && img.at<Vec3b>(Point(i, j + 1)) == Vec3b(255, 0, 0))
				img.at<Vec3b>(Point(i, j)) = Vec3b(255, 0, 0);

		}
	}
}

void fjernTrekant(Mat &img)
{
	for (int i = 1; i < img.rows - 1; i++)
	{
		for (int j = 1; j < img.cols - 1; j++)
		{
			int count(0);
			vector<Point> rute{Point(0, -1), Point(1, 0), Point(0, 1), Point(-1, 0) };
			if (img.at<Vec3b>(Point(j, i)) == Vec3b(255, 0, 0)) //hvis man er på et blåt pixel
			{
				for (int k = 0; k < rute.size(); k++)
				{
					if (img.at<Vec3b>(Point(j, i) + rute[k]) == Vec3b(255, 0, 0))
					{
						count++;
					}
				}
				if (count > 2)
				{
					img.at<Vec3b>(Point(j, i)) = Vec3b(100, 100, 100);
				}
			}
		}
	}
}
