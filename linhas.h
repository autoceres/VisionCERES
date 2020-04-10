/*Autores:
    Anniely Soares Lemos 
    Gustavo Henrique do Nascimento
*/

//OpenCv stuff
#include <opencv2/opencv.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp" //Processamento de Imagem
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"

//C++ stuff
#include <iostream>
#include <stdlib.h>


using namespace std;
using namespace cv;

class Camera{
    private:
    //Aqui você coloca as variáveis que a classe irá usar, os atributos
    float k = 0.62;
    float t = 18.0;
    
    
    public:
    int id;
    int region;
    int height;
    int width;
    
    Mat frame;
    Mat frame_roi;
    Mat segmented;
    Mat erosion;
    Mat dilation;
    Mat binarized;
    Mat skeleton;
    Mat prune;

    Mat element1 = getStructuringElement(MORPH_RECT,Size(1,7)); 		
	Mat element2 = getStructuringElement(MORPH_RECT,Size(5,5)); 
    Mat element3 = getStructuringElement(MORPH_CROSS,Size(3,3));
    
    string original;
    string processed;

    
    //Aqui você coloca os métodos
    Camera(int id_cam, int region);

    void namingVideo(string name);
    void gettingSize(int width, int height);
    void creatingRoi(Mat image);
    void Segmentation(Mat image);
    void morphologicalOperations(Mat image);

}; //<---Nunca esqueça desse ponto e vírgula, super importante!!

Camera::Camera(int id_cam, int region){
    this->id = id_cam;
    this->region = region;
}

void Camera::gettingSize(int width, int height){
    this->width = width;
    this->height = height;
}

void Camera::namingVideo(string name){
    string pvideo = "PVideo_";
    string cam = "Camera_";
	string formato = ".avi";
	pvideo += name;
	pvideo += formato;
	cam += name;
	cam += formato;

    this->original = cam;
    this->processed = pvideo;
}

void Camera::creatingRoi(Mat image){
    int xinit = 0;  
	int yinit = ((this->height) - (this->height/this->region));
    
    Rect ROI = Rect(xinit, yinit,this->width,(this->height/this->region));
	Mat image_roi = frame(ROI);
    image_roi.copyTo(this->frame_roi);
}

void Camera::Segmentation(Mat image){
    int i = 0;
    int j = 0;
	//cout<<"Iniciando a busca"<<endl;
	for(i = 0; i<image.cols; i++){						//imagem.size().width = imagem.cols -> dependendo de como a imagem está
		for(j = 0; j<image.rows; j++){					//imagem.size().height = imagem.rows -> dependendo de como a imagem está
			Vec3b color = image.at<Vec3b>(Point(i,j));
			//Condições para o pixel ser considerado verde funções 2.2 e 2.3 TCC do Felipe
			if((((color[0] + color[2])*(this->k))<color[1]) && ((color[0] + color[2])>(this->t))){ 
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
			image.at<Vec3b>(Point(i,j)) = color; //Granvando os novos pixeis na matriz
		}
	}
	this->segmented = image;
}

void Camera::morphologicalOperations(Mat image){
    //Opening
    erode(this->segmented,this->erosion,this->element1);
    dilate(this->erosion,this->dilation,this->element2);
    //Image Processing
    cvtColor(this->erosion, this->binarized, COLOR_BGR2GRAY, CV_8UC1);
    //threshold(this->dilation, this->binarized, 127, 255, THRESH_BINARY); 
    //Skeletonization
    /*Mat skel(this->binarized.size(), CV_8UC1, Scalar(0));
    Mat temp;
    
    bool done;		
    do{
        morphologyEx(this->binarized, temp, MORPH_OPEN, this->element3);        subtract(this->binarized, temp, temp);
        bitwise_not(temp, temp);
        bitwise_and(this->binarized, temp, temp);
        bitwise_or(skel, temp, skel);
        //this->skeleton.copyTo(this->binarized);

        double max;
        minMaxLoc(this->binarized, 0, &max);
        done = (max == 0);
    } while (!done);
    //Pruning
    skel.copyTo(this->skeleton);*/
}