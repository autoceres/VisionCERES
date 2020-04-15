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
#include<math.h>

#define thrs 10


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
    
    VideoCapture cap;

    Mat frame;
    Mat frame_final;
    Mat frame_roi;
    Mat segmented;
    Mat erosion;
    Mat dilation;
    Mat binarized;
    Mat skeleton;
    Mat prune;
    Mat init_point;
    

    Mat element1 = getStructuringElement(MORPH_RECT,Size(1,7)); 		
	Mat element2 = getStructuringElement(MORPH_RECT,Size(5,5)); 
    Mat element3 = getStructuringElement(MORPH_CROSS,Size(3,3));
    
    Mat image_roi1;
    Mat image_roi2;
    Mat image_roi3;
    Mat image_roi4;

    string original;
    string processed;

    vector<Point2f> points;
    vector<Point2f> points2;
    vector<Point2f> points3;
    vector<Point2f> points4;
    
    vector<Point2f> center;
    vector<Point2f> center2;
    vector<Point2f> center3;
    vector<Point2f> center4;

    vector<vector<Point2f> > pline;

    vector<Vec2f> coefs; //Primeiro m, segundo d;
    vector<Mat> miniROI;

    vector<Point2f> mmq;
    vector<Point2f> final_coef;

    vector<Vec4d> lines;
    //Aqui você coloca os métodos
    Camera(int id_cam, int region);

    void namingVideo(string name);
    void gettingSize(int width, int height);
    void creatingRoi(Mat image);
    void Segmentation(Mat image);
    void morphologicalOperations(Mat image);
    void hough(Mat image);
    void miniROIs(Mat image);
    void dynamicROI(Mat image);
    void MMQ();
    void R();
    void expanding_lines(vector<Point2f> coef_retas);
    void drawLines();

    int medianMatrix(Mat image);

    Point2f centroid(Mat image);
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
    
    //Skeletonization
    Mat skel(this->binarized.size(), CV_8UC1, Scalar(0));
    Mat temp;
    Mat aux;

    this->binarized.copyTo(aux);
    bool done;		
    do{
        morphologyEx(aux, temp, MORPH_OPEN, this->element3);        
        bitwise_not(temp, temp);
        bitwise_and(aux, temp, temp);
        bitwise_or(skel, temp, skel);
        //this->skeleton.copyTo(this->binarized);
        erode(aux,aux,this->element3);

        double max;
        minMaxLoc(aux, 0, &max);
        done = (max == 0);
        //cout << max << endl;
    } while (!done);
    //Pruning
    skel.copyTo(this->skeleton);
}

void Camera::hough(Mat image){
    vector<Vec2f> lines;
    int count = 0;
    
    this->init_point = Mat::zeros(Size(this->frame_roi.cols,this->frame_roi.rows),CV_8UC3);    
    int h = image.rows/4;
    
    lines.clear();
    this->coefs.clear();
    this->points.clear();
    this->points2.clear();
    this->points3.clear();
    this->points4.clear();

    HoughLines(this->skeleton, lines, 1, CV_PI/180, 50, 0, 0);
 
    for(int i = 0; i < lines.size(); i++){
        Vec2f auxc;
        Vec2f auxp1;
        Vec2f auxp2;
        Vec2f auxp3;
        Vec2f auxp4;

        auxc[0] = -(cos(lines[i][1])/sin(lines[i][1]));
        auxc[1] = lines[i][0]/sin(lines[i][1]);
        if(auxc[0] > 0){
            this->coefs.push_back(auxc);  
            auxp1[1] = this->frame_roi.rows;//y
            auxp1[0] = ((this->frame_roi.rows) - auxc[1])/auxc[0]; //x
            auxp2[1] = this->frame_roi.rows - h;//y
            auxp2[0] = ((this->frame_roi.rows - h) - auxc[1])/auxc[0]; //x
            auxp3[1] = this->frame_roi.rows - 2*h;//y
            auxp3[0] = ((this->frame_roi.rows - 2*h) - auxc[1])/auxc[0]; //x
            auxp4[1] = this->frame_roi.rows - 3*h;//y
            auxp4[0] = ((this->frame_roi.rows - 3*h) - auxc[1])/auxc[0]; //x
            if((auxp1[0] > image.rows) || (auxp2[0] > image.rows) || (auxp3[0] > image.rows) || (auxp4[0] > image.rows)){
                i++;
            }
            else{
                this->points.push_back(auxp1); //Pontos da Quarta Parte
                circle(this->init_point, this->points[count], 4, Scalar(0,255,0),-1,8,0);
                this->points2.push_back(auxp2); //Pontos da Terceira Parte
                circle(this->init_point, this->points2[count], 4, Scalar(0,255,0),-1,8,0);
                this->points3.push_back(auxp3); //Pontos da Segunda Parte
                circle(this->init_point, this->points3[count], 4, Scalar(0,255,0),-1,8,0);
                this->points4.push_back(auxp4); // Pontos da Primeira parte
                circle(this->init_point, this->points4[count], 4, Scalar(0,255,0),-1,8,0);
                count++;
            
            }

            imshow("Points",this->init_point);
            
        }
    }
}

void Camera::miniROIs(Mat image){
    int xinit = 0;  
    int h = (image.rows/4);
    this->miniROI.clear();
    for(int i = 0; i < 4; i++){
        int yinit = (i*h);
        Rect ROI = Rect(xinit, yinit, image.cols, h);
        Mat image_roi = image(ROI);
        Mat aux;
        image_roi.copyTo(aux);
        this->miniROI.push_back(aux);
    }
    /*
    imshow("Primeira",this->miniROI[0]);
    imshow("Segunda",this->miniROI[1]);
    imshow("Terceira",this->miniROI[2]);
    imshow("Quarta",this->miniROI[3]);*/
}

int Camera::medianMatrix(Mat image){
    int soma = 0;
    int size = 0;
    for(int i = 0; i < image.rows; i++){
        for(int j = 0; j < image.cols; j++){
            Vec3b aux = image.at<Vec3b>(Point(i,j));
            soma += aux[0];
            size++;
        }
    }
    return (soma/size);
}

Point2f Camera::centroid(Mat image){
    int x = 0;
    int y = 0;
    int size = 0;
    Point2f result;
    for(int i = 0; i < image.rows; i++){
        for(int j = 0; j < image.cols; j++){
            Vec3b aux = image.at<Vec3b>(Point(i,j));
            if(aux[0] == 255){
                x += j;
                y += i;
                size++;
            }
        }
    }
    //cout << "x = " << x << " y = " << y << " size = " << size << endl;
    result.x = x/size;
    result.y = y/size;

    return result;
}

void Camera::dynamicROI(Mat image){
   int h = (image.rows/4);
   int k = 0;
   this->pline.clear();
   vector<vector<Point2f> > plines;
   plines.clear();
   for(int i = 0; i < this->points.size(); i++){
            int t = 0;

            Vec2f aux;
            vector<Point2f> vaux;

            vaux.clear();

            int xinit1 = this->points[i].x;  
            int xinit2 = this->points2[i].x;
            int xinit3 = this->points3[i].x;
            int xinit4 = this->points4[i].x;
            
            /*
            cout << "Primeiro ROI:  " << this->points[i].x << "..." << this->points[i].y << endl;
            cout << "Segundo ROI:  " << this->points2[i].x << "..." << this->points2[i].y << endl;
            cout << "Terceiro ROI:  " << this->points3[i].x << "..." << this->points3[i].y << endl;
            cout << "Quarto ROI:  " << this->points4[i].x << "..." << this->points4[i].y << endl;
            */
            
            if((xinit1 - 20) > 0){
                if((xinit1 + 40) < image.cols){
                    Rect ROI1 = Rect(xinit1 - 20, 3*h, 40, h);
                    this->image_roi1 = image(ROI1);
                }
                else{
                    Rect ROI1 = Rect(xinit1 - 20, 3*h, (image.cols - xinit1 - 1), h);
                    this->image_roi1 = image(ROI1);
                }
            }
            else{
                Rect ROI1 = Rect(1, 3*h, 40, h);
                this->image_roi1 = image(ROI1);
            }
            
            if((xinit2 - 20) > 0){
                if((xinit2 + 40) < image.cols){
                    Rect ROI2 = Rect(xinit2 - 20, 2*h, 40, h);
                    this->image_roi2 = image(ROI2);
                }
                else{
                    Rect ROI2 = Rect(xinit2 - 20, 2*h, (image.cols - xinit2 - 1), h);
                    this->image_roi2 = image(ROI2);
                }
            }
            else{
               Rect ROI2 = Rect(1, 2*h, 40, h);
               this->image_roi2 = image(ROI2);
            }

            if((xinit3 - 20) > 0){
                if((xinit3 + 40) < image.cols){
                    Rect ROI3 = Rect(xinit3 - 20, h, 40, h);
                    this->image_roi3 = image(ROI3);
                }
                else{
                    Rect ROI3 = Rect(xinit3 - 20, h, (image.cols - xinit3 - 1), h);
                    this->image_roi3 = image(ROI3);
                }
            }
            else{
               Rect ROI3 = Rect(1, h, 40, h);
               this->image_roi3 = image(ROI3);
            }

            if((xinit4 - 20) > 0){
                if((xinit4 + 40) < image.cols){
                    Rect ROI4 = Rect(xinit4 - 20, 0, 40, h);
                    this->image_roi4 = image(ROI4);
                }
                else{
                    Rect ROI4 = Rect(xinit4 - 20, 0, (image.cols - xinit4 - 1), h);
                    this->image_roi4 = image(ROI4);
                }
            }
            else{
               Rect ROI4 = Rect(1, 0, 40, h);
               this->image_roi4 = image(ROI4);
            }
                            
            if(medianMatrix(this->image_roi1) != 0){
                Point2f c = centroid(this->image_roi1);
                aux[0] = xinit1 - 20 + c.x;
                aux[1] = 3*h + c.y;
                vaux.push_back(aux);
                t++;
                circle(this->init_point, vaux[t-1], 4, Scalar(255,0,0),-1,8,0);
            }

            else{
                aux[0] = this->points[i].x;
                aux[1] = this->points[i].y - (h/2);
                vaux.push_back(aux);
                t++;
                circle(this->init_point, vaux[t-1], 4, Scalar(255,0,0),-1,8,0);  
            }
                    
            if(medianMatrix(this->image_roi2) != 0){
                Point2f c = centroid(this->image_roi2);

                aux[0] = (xinit2 - 20) + c.x;
                aux[1] = 2*h + c.y;
                vaux.push_back(aux);
                t++;
                circle(this->init_point, vaux[t-1], 4, Scalar(255,0,0),-1,8,0);
            }

            else{
                aux[0] = xinit2;
                aux[1] = this->points2[i].y - (h/2);
                vaux.push_back(aux);
                t++;
                circle(this->init_point, vaux[t-1], 4, Scalar(255,0,0),-1,8,0);
            }

            if(medianMatrix(this->image_roi3) != 0){
                Point2f c = centroid(this->image_roi3);
                        
                aux[0] = (xinit3 - 20) + c.x;
                aux[1] = h + c.y;
                vaux.push_back(aux);
                t++;
                circle(this->init_point, vaux[t-1], 4, Scalar(255,0,0),-1,8,0);
            }
                    
            else{
                aux[0] = xinit3;
                aux[1] = this->points3[i].y - (h/2);
                vaux.push_back(aux);
                t++;
                circle(this->init_point, vaux[t-1], 4, Scalar(255,0,0),-1,8,0);
            }

            if(medianMatrix(this->image_roi4) != 0){
                Point2f c = centroid(this->image_roi4);

                aux[0] = (xinit4 - 20) + c.x;
                aux[1] = c.y;
                vaux.push_back(aux);
                t++;
                circle(this->init_point, vaux[t-1], 4, Scalar(255,0,0),-1,8,0);
            }
                    
            else{
                aux[0] = xinit4;
                aux[1] = this->points4[i].y - (h/2);
                vaux.push_back(aux);
                t++;
                circle(this->init_point, vaux[t-1], 4, Scalar(255,0,0),-1,8,0);
            }

            plines.push_back(vaux);
            this->pline = plines;
            
            imshow("Points",this->init_point);
            /*imshow("Primeira",this->image_roi1);
            imshow("Segunda",this->image_roi2);
            imshow("Terceira",this->image_roi3);
            imshow("Quarta",this->image_roi4);*/
              
            
       
   }
}

void Camera::MMQ(){
    
    this->mmq.clear();
    
    for(int i = 0; i < this->pline.size(); i++){
        Vec2f aux;
        float sx = 0, sy = 0, xx = 0, xy = 0, n = pline[i].size();
        for(int j = 0; j < this->pline[i].size(); j++){
            sx += this->pline[i][j].x;
            sy += this->pline[i][j].y;
            xx += (this->pline[i][j].x)*(this->pline[i][j].x);
            xy += (this->pline[i][j].x)*(this->pline[i][j].y);
        }
        aux[0] = ((xy - ((sy*sx)/n))/(xx - ((sx*sx)/n))); //Coef Ang;
        aux[1] = ((sy - (aux[0]*sx))/n); //Coef lin
        //cout << aux << endl;
        this->mmq.push_back(aux);
    }
}

void Camera::R(){
    this->final_coef.clear();
    for(int i = 0; i < this->pline.size(); i++){
        double sum = 0;
        for(int j = 0; j < this->pline[i].size(); j++){
            int xp = (this->pline[i][j].y - this->mmq[i].y)/(this->mmq[i].x);
            sum += (this->pline[i][j].x - xp)*(this->pline[i][j].x - xp);
        }
        int result = sqrt(sum);
        //cout << result << endl;
        if(result < thrs){
            this->final_coef.push_back(mmq[i]);
        }
    }
}

void Camera::expanding_lines(vector<Point2f> coef_retas){
	int xi = 0, yi = 0, xf = 0, yf = 0;

	vector<Vec4d> lines;

	lines.clear();
    this->lines.clear();

	for(int i = 0; i < coef_retas.size(); i++){	
		xi = (yi - coef_retas[i].y)/coef_retas[i].x;
		yf = (this->height/this->region);
		xf = (yf - coef_retas[i].y)/coef_retas[i].x;
				
		if(xf>this->width){
			xf = this->width;
			yf = (coef_retas[i].x)*xf + coef_retas[i].y;
    	}
			
        Vec4d x_ord_trans;
		x_ord_trans[0] = xi;
		x_ord_trans[1] = yi;
		x_ord_trans[2] = xf;
		x_ord_trans[3] = yf;
		lines.push_back(x_ord_trans);
		
    }
	this->lines  = lines;
}

void Camera::drawLines(){
    for(int i = 0; i < this->lines.size(); i++){
		line(this->frame_final, Point(this->lines[i][0], (this->lines[i][1] + (this->height - (this->height/this->region)))), Point(this->lines[i][2], (this->lines[i][3] + (this->height - (this->height/this->region)))), Scalar(255,0,0), 7, LINE_AA);
	}
    imshow("Resultado",this->frame_final);
}