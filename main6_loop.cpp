//metodo felipe
#include "linhas.h"
#include <opencv2/opencv.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp" //Processamento de Imagem
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define k 0.68 
#define t 20.0

using namespace std;
using namespace cv;

int main(int argc, char *argv[]){
	for (int i=1;i<=stoi(argv[1]);i++){
		string nome="Dataset/";
		nome+=to_string(1);
		nome+=".png";
		string pref = "_metodo6_saida.png";
        string path = "Imagens/";
        string name;
        string name_arq;
        Camera cam(0,2);

        cam.img_fn += "Dataset/";
        cam.img_fn += to_string(i);
        cam.img_fn += cam.img_ext;
        
        name_arq += to_string(i);
        name_arq += "_metodo6"; 
        name += path;
        name += to_string(i);
        name += pref;
        
        cam.name += to_string(i);

		Mat ent,frame,src, erosion_dst, dilation_dst, canny_output,framegray,exi, RB, M1,M2;
		Mat element1 = getStructuringElement(MORPH_RECT, Size(3,3));
		Mat element2 = getStructuringElement(MORPH_RECT, Size(7,7));
		vector<Mat> chanels;
		ent = imread(nome, 	IMREAD_COLOR); 
		//imshow("Original", ent); 
		//waitKey(0);

		resize(ent,frame, Size(360,480));
		//imshow("Resize", frame); 
		//waitKey(0);
		if(frame.empty()){
			printf(" Erro ao abrir a imagem\n");
			return -1;
		}
		else {
			frame.copyTo(exi); 
			split(frame, chanels);
			add(chanels[2], chanels[0], RB);
			threshold(RB,M1,t,255,0);
			for(int i = 0; i<frame.cols; i++){					
				for(int j = 0; j<frame.rows; j++){			
					Vec3b color = frame.at<Vec3b>(Point(i,j));
					//Condições para o pixel ser considerado verde funções 2.2 e 2.3 TCC do Felipe
					if((((color[0] + color[2])*k)<color[1])){ 
					//Muda o pixel para branco caso seja verde
					color[0] = 255; 
					color[1] = 255;
					color[2] = 255;
					}
					else{
					//Muda o pixel para preto
					color[0] = 0; 
					color[1] = 0;
					color[2] = 0;
					}
					frame.at<Vec3b>(Point(i,j)) = color; //Granvando os novos pixeis na matriz
				}
			}
		}
		//imshow("Segmentacao", frame); 	
		//waitKey(0);
		frame.copyTo(src); 
		erode(src, erosion_dst, element1);
		//imshow("Erosao", erosion_dst);
		//waitKey(0);
		dilate( erosion_dst, dilation_dst, element2 );
		//imshow( "Dilatacao", dilation_dst );
		//waitKey(0);

		int thresh = 100;
		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;
		Canny( dilation_dst, canny_output, thresh, thresh*2, 3 );
		findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
		Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
		for( int i = 0; i< contours.size(); i++ )
			{
			Scalar color = Scalar( 255,255,255 );
			drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
		}
		//imshow( "Contornos", drawing );
		//waitKey(0);
		vector<Moments> mu(contours.size());
		for( size_t i = 0; i < contours.size(); i++ ){ 
			mu[i] = moments( contours[i], false ); 
		}
		vector<Point2f> mc(contours.size());
		for( size_t i = 0; i < contours.size(); i++ ){ 
			mc[i] = Point2f(static_cast<float>(mu[i].m10/mu[i].m00),static_cast<float>(mu[i].m01/mu[i].m00)); 
		}
		Mat drawi = Mat::zeros( canny_output.size(), CV_8UC3 );
		for( size_t i = 0; i< contours.size(); i++ ){
			Scalar color = Scalar(0,0,255);
			circle(drawi, mc[i], 4, color,-1,8,0);
		}
		//imshow( "Centros", drawi );
		//waitKey(0);
		vector<Vec4i> linesP; 
		HoughLinesP(canny_output, linesP, 1, CV_PI/180, 75, 300,650 );
		for( size_t i = 0; i < linesP.size(); i++ )
		{
			Vec4i l = linesP[i];
			line( exi, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, LINE_AA); 
		}
		//imshow("Hough", exi);
		//waitKey(0); 
		cam.writingFile(name_arq);
        //imwrite(name,cam.frame_final);
		destroyAllWindows();
		//cout << i << endl;
	}
	return 0;
}