include<iostream>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>


using namespace std;
using namespace cv;

int main(){

Mat image = imread("map.PNG", CV_LOAD_IMAGE_GRAYSCALE);

//Finde ud af hvad classer skal indeholde
  //Robotclass indeholder: Points: start + now og vector<Points> objekter der er detekteret
//Laves bruschfire på map
//Følg vej rundt rundt i map
//Lav en form for laser scanner
//Find en rute på vej tilbage
  
//forslag til laserscanner
//tjek de nærmeste naboer, er der nogle af dem der er sorte, eller et objekt?
//Hvis ikke, så tjek alle naboernes naboer.
//Fortsæt til et objekt er ramt (eller mulighvis skal man også her fortsætte), eller til der er sort hele vejen rundt.
//OBS, vær opmærksom på, at den ikke kommer til at dreje om hjørner.

//Mine noter omkring bruschfire algoritmen

/*Hvis man skulle implementere det kan man lave en kode hvor man løber gennem billedet, indtil alle værdier er forskellige fra baggrundsfarven. Hver gang, man støder ind i noget undervejs, farver man det pixel man står på en værdi højere.
Denne er O(nm*max(m,n)) (den kan potentielt løbe gennem billedet mange gange)

Der er også en mindre computional tung måde at gøre det på.
Her starter man med at oprette en naboliste. Så løber man hele biledet igennem, og lægger coordinaterne til alle naboerne i listen. Derefter har den en løkke, indtil der ikke er flere naboer. Der laver man en ny liste med naboer. Så løber man alle de første naboer igennem, og lægger alle deres naboer i den nye liste.
Denne er O(nm+nm) (den løber højest billede igennem 1 gang. */

  return 0;

}
