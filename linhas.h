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
    int id;
    int region;

    float k = 0.62;
    float t = 18.0;
    
    Mat frame;
    Mat frame_roi;
    Mat 
    
    public:
    //Aqui você coloca os métodos
    Camera(int id_cam, int region);

    void namingVideo(string name);

    void creatingRoi();
    void Segmentation(Mat image);

}; //<---Nunca esqueça desse ponto e vírgula, super importante!!

Camera::Camera(int id_cam, int region){
    this->id = id_cam;
    this->region = region;
}

Camera::creatingRoi(){

}