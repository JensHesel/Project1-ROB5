#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void brushfire(Mat &img);
void drawVoronoi(Mat &img);
void onMouse(int event, int x, int y, int, void* param);
void findBlindLine(Mat &img);

int main()
{
	Mat map = imread("map2.png");
	namedWindow("Original", WINDOW_AUTOSIZE);
	imshow("Original", map);
	double time = clock();
	brushfire(map);
	time = clock() - time;
	cout << "It took this many seconds: " << time / CLOCKS_PER_SEC;
	namedWindow("Brushfired map", WINDOW_AUTOSIZE);
	imshow("Brushfired map", map);
	drawVoronoi(map);
	namedWindow("map", WINDOW_NORMAL);
	imshow("map", map);
	setMouseCallback("Brushfired map", onMouse, &map);
	waitKey(0);
}

void onMouse(int event, int x, int y, int, void* param)
{
	Mat map = *((Mat*)param);
	if (event == EVENT_LBUTTONDOWN)
	{
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
						else if (compare.at<Vec3b>(Point(i, j))[2] == compare.at<Vec3b>(Point(i + k, j + l))[2])
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
	for (int i = 0; i < compare.cols; i++)
	{
		for (int j = 127; j < 131; j++)
		{
			cout << (int)compare.at<Vec3b>(Point(i, j))[1] << " ";
		}

		cout << "            ";

		for (int j = 127; j < 131; j++)
		{
			cout << (int)img.at<Vec3b>(Point(i, j))[1] << " ";
		}

		cout << endl;
	}
}
