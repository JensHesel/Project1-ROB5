#include "robot.h"

bool operator>(const Vec3b &a, const Vec3b &b); //gør så alle tre værdier skal være større for at returnere true
bool operator<(const Vec3b &a, const Vec3b &b); //gør så alle tre værdier skal være mindre for at returnere true

robot::robot()
{
	start = Point(10, 10); //det punkt man starter i, i opgaven
	now = start; 
}

robot::robot(Point begin, Point pos, vector<Point> dots)
{
	start = begin;
	now = pos;
	found_targets = dots;
}

void robot::followRute(Mat &input)
{
	int search(0), total(0); //Bruges til at tælle hvor langt man har gået
	Point old(0, 0), oldold(0,0); //Bruges til at se hvilket punkt man var på sidst og inden da
	namedWindow("map", WINDOW_NORMAL);
	int movement(0), movementprev(0); //Hvor mange har man rykket siden sidste laserscan, og har man rykket sig siden sidst der blev tjekket
	laserScan(input, Vec3b(0, 0, 230), Vec3b(50, 50, 255)); //lav laserscan
	while (found_targets.size() < 9) { //Indtil man har fundet alle punkterne

		Vec3b color(0, 255, 0); //Den farve stregen får når den er blevet gået på
		Point target;
		bool blue(true); //Hvis man rykker hen på et blå pixel, forbliver den true

		oldold = old; 
		moveOne(input, movement, old, Vec3b(255, 0, 0)); //man rykker et pixel frem i denne funktion
		while (movement == movementprev)
		{
			blue = false; //Hvis man er kommet herind, er man ikke rykket hen på et blåt pixel
			moveOne(input, movement, old, color); //man rykker et pixel frem i denne funktion
			color -= Vec3b(0, 20, 0);
		}
		if (movement > 15 && blue && oldold != now) { //Man vil kun have at den laver laserscan hver 15. gang og på de blå pixels (oldold != now er med fordi den ellers også kommer til at scanne i spidserne af grenene)
			target = laserScan(input, Vec3b(0, 0, 230), Vec3b(50,50,255)); //Laver laserScan og returnerer koodinaterne på punktet eller 0,0 hvis der ikke er fundet noget
			movement = 0;
			if (target != Point(0, 0)) //Hvis man har fundet et punkt
			{
				getToTarget(input, target); //I denne kører den både til target og tilbage til start
				cout << "length traveled while search for target " << found_targets.size() << ": " << path.size() << endl;
				search += path.size();
				total = search * 2; //Den kører samme vej tilbage, og derfor er den totale længde bare dobbelt så lang som søgelængden
				path.clear(); //Man starter fra start igen, og tager en ny rute ud
			}
		}
		movementprev = movement; //Viser at man har rykket sig

	}
	cout << "The total search length is: " << search << endl;
	cout << "The total length traveled is: " << total << endl;
	cout << "Targets found on these points: " << found_targets << endl;
}

void robot::moveOne(Mat &map, int &moved, Point &old, Vec3b color)
{
	bool greenOption(false); //bliver true hvis det er muligt at gå hen på en grøn i en eller anden nuance, så man ved at man ikke skal gå hen på old
	vector<Point> rute{ Point(-1, -1), Point(0, -1), Point(1, -1), Point(1, 0), Point(1, 1), Point(0, 1), Point(-1, 1), Point(-1, 0) }; //Man tjekker rundt fra venstre hjørne og med uret
	for (int i = 0; i < rute.size(); i++)
	{
		if (map.at<Vec3b>(Point(now) + rute[i]) == color) //Hvis den farve man tjekker for er på det pixel der tjekkes på
		{
			if ((Point(now) + rute[i]) != old)  //Hvis det ikke er det pixel man var på lige før
			{
				old = now; 
				now = Point(now) + rute[i];
				
				colorRobot(map); //Vi tegner robotten

				if (color == Vec3b(255, 0, 0)) //hvis den er blå
					map.at<Vec3b>(now) = Vec3b(0, 255, 0); //farv den grøn
				else
				{
					Vec3b oldcolor = map.at<Vec3b>(now); //hvis den er grøn
					map.at<Vec3b>(now) = oldcolor - Vec3b(0, 20, 0); //gør den mørkere
				}

				moved++;
				path.push_back(now); //sæt ind at man har gået på now på vej hen mod målet
				return;
			}
		}
		if (map.at<Vec3b>(Point(now) + rute[i])[2] == 0 && map.at<Vec3b>(Point(now) + rute[i])[1] > 0 && Point(now) + rute[i] != old) //Hvis der er en grøn mulighed, som ikke er old
		{
			greenOption = true; //så lav den her true, så man ikke hopper hen til old
		}
	}
	if (greenOption == false) //Hvis ikke der er en grøn mulighed
	{
		//Farv spidsen en tand mørkere, så man ikke rykker derud igen lige efter
		Vec3b oldcolor = map.at<Vec3b>(now); 
		map.at<Vec3b>(now) = oldcolor - Vec3b(0, 20, 0);

		//byt rundt på now og old
		Point temp = old;
		old = now;
		now = temp;

		
		colorRobot(map); //Vi tegner robotten
	
		map.at<Vec3b>(now) = oldcolor - Vec3b(0, 20, 0); //Feltet gøres mørkere

		moved++;
		path.push_back(now);
		return;
	}
		

}

Point robot::laserScan(Mat &map, Vec3b mincolor, Vec3b maxcolor) //Med neighbors metode
{
	//Dette sker i denne funktion:
	//Find alle naboer til startpunktet der ikke er sorte
	//Find alle de udadgående naboer til naboerne i en for lykke, indtil den er kørt igennem et vis antal gange.

	int radius(20); //Hvor mange pixels ud fra midten skal den scanne
	vector<Point> neighbors; //Den der kommer til at indeholde alle de pixels der er blevet testet

	for (int k = -1; k < 2; k++) //all neighbors til startpunktet
	{
		for (int l = -1; l < 2; l++)
		{
			Point look(now.x + k, now.y + l); //Det punkt man kigger på lige nu
			if (map.at<Vec3b>(look) < maxcolor && map.at<Vec3b>(look) > mincolor) //Hvis man ser den farve pixel der søges efter
			{
				found_targets.push_back(look); //Sæt target ind her
				return look; //returner punktet, og springer ud af funktionen
			}
			else if (map.at<Vec3b>(look) != Vec3b(0, 0, 0))  //Hvis naboen ikke er sort. Her vil startpunktet også komme ind som nabo
			{
				neighbors.push_back(look); //sæt den ind som en nabo
			}
		}
	}

	vector<Point> newneighbors(neighbors); //Den kommer til at indeholde alle de pixels vi er i gang med at teste

	for (int i = 0; i < radius; i++)
	{
		//Her tegnes den firkant, som er i gang med at blive scannet
		vector<Vec3b> colornow(newneighbors.size(), Vec3b(100, 0, 100)); //Opretter en vector med den farve, som man gerne vil have scannet har
		vector<Vec3b> colorprev(newneighbors.size(), 0); //Opretter en vector, som kommer til at indeholde de farver som pixelsene der bliver scannet havde, før de blev lilla (så de kan blive farvet tilbage i den farve)
		colvec(map, newneighbors, colornow, colorprev); //Farver det område der scannes i lilla, og gemmer den gamle farve
		colorRobot(map); //Tegner robotten (og printer scannet + robotten)
		colvec(map, newneighbors, colorprev, colornow); //Farver området til den gamle farve igen



		vector<Point> newnewneighbors; //De naboer der skal testes i næste runde
		for (int j = 0; j < newneighbors.size(); j++)//all pixel in newneighbors
		{
			for (int k = -1; k < 2; k++) //all neighbors to pixel
			{
				for (int l = -1; l < 2; l++)
				{
					Point look(newneighbors[j].x + k, newneighbors[j].y + l); //Det punkt man kigger på
					if (find(neighbors.begin(), neighbors.end(), look) == neighbors.end()) //Hvis den ikke allerede er i listen som indeholder alle punkter der er søgt i
					{
						if (map.at<Vec3b>(look) < maxcolor && map.at<Vec3b>(look) > mincolor) //Hvis man er på den farve pixel der søges efter
						{
							found_targets.push_back(look); //Sæt target ind her
							return look; //returner punktet, og springer ud af funktionen
						}
						else if (map.at<Vec3b>(look) != Vec3b(0, 0, 0) && clearline(map, look, now))  //Hvis det ikke er et nul, og der ikke er et sort pixel imellem punktet og starten
						{
							newnewneighbors.push_back(look); //sæt den ind som en ny nabo (de naboer vi skal teste i næste runde)
							neighbors.push_back(look); //Sæt ind at den er blevet tilføjet som nabo
						}
					}
				}
			}
		}
		newneighbors = newnewneighbors; //Rykker de naboer der skal testes i næste omgang over
	}
	return Point(0, 0); //Hvis den ikke finder et rødt punkt, så returner 0,0
}

void robot::colvec(Mat &map, vector<Point> &points, vector<Vec3b> &color, vector<Vec3b> &prevcolor)
{
	for (int i = 0; i < points.size(); i++) //På alle de koordinater der er kommet med ind
	{
		prevcolor[i] = map.at<Vec3b>(points[i]); //Gemmer den gamle farve
		map.at<Vec3b>(points[i]) = color[i]; //Farver det den nye farve
	}
}

bool robot::clearline(Mat &map, Point A, Point B)
{
	if (A.x != B.x) //hvis ikke det er en lodret linje
	{
		double a = ((double)A.y - (double)B.y) / ((double)A.x - (double)B.x); //udregner hælningen på linjen
		double b = (double)A.y - a * (double)A.x; //Udregner skæringen på y-aksen
		if (a < 1 && a > -1) //Hvis det er x der ændres hurtigst på linjen
		{
			int x_start = min(A.x, B.x);
			int x_end = max(A.x, B.x);
			int x_now = x_start;
			while (x_now != x_end) //Indtil alle punkterne imellem dem er tjekket
			{
				int y_now = a * (double)x_now + b; //find y-værdien på linjen der passer på x-værdien
				if (map.at<Vec3b>(Point(x_now, y_now)) == Vec3b(0, 0, 0)) //Hvis man støder ind i noget sort
					return false;
				x_now++; //Gå videre til næste x-værdi
			}
		}
		else //Hvis det er y der ændres hurtigst på linjen
		{
			int y_start = min(A.y, B.y);
			int y_end = max(A.y, B.y);
			int y_now = y_start;
			while (y_now != y_end) //Indtil alle punkterne imellem dem er tjekket
			{
				int x_now = ((double)y_now - b) / a; //find x-værdien på linjen der passer på y-værdien
				if (map.at<Vec3b>(Point(x_now, y_now)) == Vec3b(0, 0, 0)) //Hvis man støder ind i noget sort
					return false;
				y_now++; //Gå videre til næste y-værdi
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
			if (map.at<Vec3b>(Point(A.x, y_now)) == Vec3b(0, 0, 0)) //Hvis man støder ind i noget sort
				return false;
			y_now++; //Gå videre til næste y-værdi
		}
	}
	return true; //Hvis man kommer herud, er der ikke nogen mur imellem de to punkter
}

void robot::getBack(Mat &map)
{
	path.reverse(); //vender ruten der er gået ud til punktet om, så vi kan følge den bagfra
	
	for (list<Point>::iterator it = path.begin(); it != path.end(); it++) //På hver placering i ruten der er gået
	{
		now = *it; //ryk robottens placering 

		
		colorRobot(map); //Tegn robotten på mappet
	}
}

void robot::getToTarget(Mat &map, Point dotPos)
{
	
	while (dotPos != now) //Indtil man er på target
	{
		//Bevæg x en tand mod target
		if (dotPos.x > now.x)
		{
			now.x++;
		}
		else if (dotPos.x < now.x)
		{
			now.x--;
		}
		
		//Bevæg y en tand med target
		if (dotPos.y > now.y)
		{
			now.y++;
		}
		else if (dotPos.y < now.y)
		{
			now.y--;
		}

		map.at<Vec3b>(now) = Vec3b(0, 255, 255); //Farv linjen gul
		imshow("map", map);
		waitKey(10);
		path.push_back(now); //indsætter at den også gemmer punkterne ud til target
	}


	colorTarget(map, Vec3b(0, 0, 230), Vec3b(50, 50, 255)); //Farv hele target grøn (det er min og max farver for target før der sendes med)
	

	getBack(map); //kom tilbage til start

}

void robot::colorTarget(Mat map, Vec3b minColor, Vec3b maxColor)
{
	for (int k = -4; k < 5; k++) //all neighbors til startpunktet 4 pixels ud
	{
		for (int l = -4; l < 5; l++) {
			if (map.at<Vec3b>(Point(now.x + k, now.y + l)) > minColor && map.at<Vec3b>(Point(now.x + k, now.y + l)) < maxColor) //hvis det pixel er rødt
				map.at<Vec3b>(Point(now.x + k, now.y + l)) = Vec3b(0, 255, 0); //farv det grønt
		}
	}
}

void robot::colorRobot(Mat &map)
{
	vector<Point> points; //Vector der skal indeholde de punkter hvor robotten er, og lige omkring
	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			points.push_back(Point(now.x + i, now.y + j)); //Punkterne til robotten indsættes
		}
	}
	vector<Vec3b> robotcolor(9, Vec3b(230, 0, 255)), oldcolor(9); //Farver på robotplacering (før og efter)
	colvec(map, points, robotcolor, oldcolor); //Farv robotten
	imshow("map", map);
	waitKey(1);
	colvec(map, points, oldcolor, robotcolor); //Lav området tilbage til den gamle farve (men uden at vise det)
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
