#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void brushfire(Mat &img);

int main()
{
	Mat map = imread("Map.png");
	namedWindow("Original", WINDOW_AUTOSIZE);
	imshow("Original", map);
	double time = clock();
	brushfire(map);
	time = clock() - time;
	cout << "It took this many seconds: " << time / CLOCKS_PER_SEC;
	namedWindow("Brushfired map", WINDOW_AUTOSIZE);
	imshow("Brushfired map", map);
	waitKey(0);
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
		B+=3.7;
		G+=3.7;
		R+=3.7;
		neighbors = newneighbors;
		cout << "in progress " << neighbors.size() << " round: " << B << endl;
	}
}
