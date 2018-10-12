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
  


  return 0;

}
