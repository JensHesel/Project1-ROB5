#include<opencv2/opencv.hpp>
#include"robot.h"

using namespace std;
using namespace cv;

void brushfire(Mat &img); //laver bruchfire på mappet
void drawVoronoi(Mat &img); //tegner streger ud fra brushfire på mappet (som voronoi)
void lapning(Mat &img); //bruges i drawVoronoi, da der ellers opstår en masse huller i linjen der skal køres på
void lapningLige(Mat &img); //bruges som lapning i drawVoronoi

int main()
{
	robot a;
	Mat map = imread("modimap.png");
	namedWindow("Original", WINDOW_AUTOSIZE);
	imshow("Original", map);
	brushfire(map); 
	drawVoronoi(map); 
	namedWindow("map", WINDOW_NORMAL);
	imshow("map", map);

	//Disse to har været brugt for at der ikke skulle laves brushfire og voroni igen hver gang robotten har skulle testes
	//imwrite("brushfiredmap.png", map);
	//Mat map = imread("brushfiredmap.png");

	a.followRute(map); //Får robotten til at køre rundt i mappet

	waitKey(0);

}

void brushfire(Mat &img)
{
	
	vector<Point> neighbors; //Bruges til at indsætte naboerne til de punkter der ligger rundt om dem der kigges på 

	for (int i = 1; i < img.cols - 1; i++) //Gennemgår x-koordinater 
	{
		for (int j = 1; j < img.rows - 1; j++) //Gennemgår y-koordinater
		{
			if (img.at<Vec3b>(Point(i, j)) == Vec3b(0, 0, 0)) //Hvis det pixel vi står på er sort
			{
				for (int k = -1; k < 2; k++) //all neighbors n
				{
					for (int l = -1; l < 2; l++)
					{
						int x = i + k;
						int y = j + l;
						if (img.at<Vec3b>(Point(x, y)) == Vec3b(255, 255, 255)) //Hvis den nabo er hvid (så ikke en mur)
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
	double B(2), G(2), R(2); //Størrelsen på farverne (de er ens så det bliver gråt)
	while (neighbors.size() != 0) //Så længe der er naboer (så man ikke er kommet helt ind til midten)
	{
		Vec3b color(B, G, R); //Farven i den runde
		vector<Point> newneighbors; //skal bruges til naboerne til naboerne
		for (int i = 0; i < neighbors.size(); i++)//all pixel in neighbors
		{
			if (img.at<Vec3b>(neighbors[i]) == Vec3b(255, 255, 255)) //Hvis den ikke er blevet givet en værdi endnu
				img.at<Vec3b>(neighbors[i]) = color;
			for (int k = -1; k < 2; k++) //all neighbors to pixel
			{
				for (int l = -1; l < 2; l++)
				{
					Point now = neighbors[i]; //den nabo der ses på nu

					//Det sted rundt om naboen der ses på nu
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

		//Øg farverne så man i næste ømgang farver det en tand lysere
		B+=2;
		G+=2;
		R+=2;

		neighbors = newneighbors; //Overflyt newneighbors til neighbors, så de er klar til næste runde
		cout << "in progress " << neighbors.size() << " color: " << B << endl; //Udskriv for at kunne følge med i, hvor langt man er
	}
}

void drawVoronoi(Mat &img)
{
	Mat compare = img.clone(); //Bruges til at tjekke farverne på, da der løbende overskrives på det originale billede. 
	for (int i = 1; i < img.cols - 1; i++) //Gennemgår x-koordinater
	{
		for (int j = 1; j < img.rows - 1; j++) //Gennemgår y-koordinater
		{
			if (img.at<Vec3b>(Point(i, j)) == Vec3b(0, 0, 0) || ( (int)img.at<Vec3b>(Point(i, j))[2] > 230 && (int)img.at<Vec3b>(Point(i, j))[0] < 50)) //Hvis man står på et sort eller et rødt pixel
			{
				continue; //Så skal den ikke gøre noget
			} 
			else
			{
				int countb(0); //Hvor mange omkring er større
				int counte(0); //Hvor mange omkring er ens
				for (int k = -1; k < 2; k++) //Se rundt om det sted i billedet der er nået til
				{
					for (int l = -1; l < 2; l++)
					{
						if (k == 0 && l == 0) //Tjek ikke det punkt man står på
						{ 
							continue;
						}
						else if (compare.at<Vec3b>(Point(i, j))[2] > compare.at<Vec3b>(Point(i + k, j + l))[2]) //Hvis midten er større end den ved siden af
						{
							countb++;
						}
						else if (compare.at<Vec3b>(Point(i, j))[2] == img.at<Vec3b>(Point(i + k, j + l))[0]) //Hvis midten har samme størrelse som den ved siden af
						{
							counte++;
						}
						if (countb > 3 || counte > 4) //I de to tilfælde, skal punktet være en del af en linje
						{
							img.at<Vec3b>(Point(i, j)) = Vec3b(255, 0, 0);
						}

					}
				}
			}
		}
	}

	//Lapning. Det er nødvendigt, da der her er en masse huller i linjen.
	//Der kunne fra start godt laves en linje uden en masse huller, men så bliver den tykkere end et pixel nogle steder. 
	lapning(img); //Sætter punkter sammen i flere retninger, hvor der kun er et pixel som mellemrum imellem dem. 
	lapningLige(img); //Sætter punkter sammen der er lige overfor hinanden, med et pixel imellem
}

void lapning(Mat &img)
{
	
	for (int i = 1; i < img.cols - 1; i++) //løber x-koordinater igennem
	{
		for (int j = 1; j < img.rows - 1; j++) //løber y-koordinater igennem
		{
			int countblue(0);
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

			if (countblue == 2) //indsætter punkter, hvis der kun er 2 blå rundt om
			{
				// Tjekker (-1,-1) og (1,1) - Diagonalt
				if (img.at<Vec3b>(Point(i - 1, j - 1)) == Vec3b(255, 0, 0) && img.at<Vec3b>(Point(i + 1, j + 1)) == Vec3b(255, 0, 0))
					img.at<Vec3b>(Point(i, j)) = Vec3b(255, 0, 0);

				// Tjekker (-1,1) og (1,-1) - Diagonalt
				if (img.at<Vec3b>(Point(i - 1, j + 1)) == Vec3b(255, 0, 0) && img.at<Vec3b>(Point(i + 1, j - 1)) == Vec3b(255, 0, 0))
					img.at<Vec3b>(Point(i, j)) = Vec3b(255, 0, 0);

				for (int k = -1; k < 2; k++) //Her sammenligner man i tre forskellige retninger. 
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
		}
	}
}

void lapningLige(Mat &img)
{
	for (int i = 1; i < img.rows - 1; i++) //Gennemgår billedet
	{
		for (int j = 1; j < img.cols - 1; j++)
		{
			int count(0);
			vector<Point> rute{ Point(0, -1), Point(1, 0), Point(0, 1), Point(-1, 0) }; //de pixels man gerne vil tjekke, omkring det man er nået til
			for (int k = 0; k < rute.size(); k++) //Tjek alle punkterne omkring
			{
				if (img.at<Vec3b>(Point(j, i) + rute[k]) == Vec3b(255, 0, 0))
				{
					count++;
				}
			}
			if (count == 2) //Man vil kun gøre det, hvis count er 2, ellers kommer man til at lave klumper i ens streg
			{
				// Tjekker (-1,0) og (1,0) - Over under
				if (img.at<Vec3b>(Point(j - 1, i)) == Vec3b(255, 0, 0) && img.at<Vec3b>(Point(j + 1, i)) == Vec3b(255, 0, 0))
					img.at<Vec3b>(Point(j, i)) = Vec3b(255, 0, 0);

				// Tjekker (0,-1) og (0,1) - Venstre højre
				if (img.at<Vec3b>(Point(j, i - 1)) == Vec3b(255, 0, 0) && img.at<Vec3b>(Point(j, i + 1)) == Vec3b(255, 0, 0))
					img.at<Vec3b>(Point(j, i)) = Vec3b(255, 0, 0);
			}
		}
	}
}
