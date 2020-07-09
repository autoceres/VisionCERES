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
#include <fstream>
#include <string>
#include <cstdlib>
#include <stdlib.h>
#include <math.h>

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

    int np_min = 1;

    int pb;
    
    string img_ext = ".jpg";
    string img_fn;

    VideoCapture cap;
    VideoWriter p;
    VideoWriter c;

    Mat frame;
    Mat frame_final;
    Mat frame_roi;
    Mat otsu;
    Mat segmented;
    Mat erosion;
    Mat dilation;
    Mat binarized;
    Mat skeleton;
    Mat morph;
    Mat canny;
    Mat point;
    Mat init_point;
    

    Mat element1; // = getStructuringElement(MORPH_RECT,Size(3,9)); 		
	Mat element2; // = getStructuringElement(MORPH_RECT,Size(5,5)); 
    Mat element3; // = getStructuringElement(MORPH_CROSS,Size(3,3));
    
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

    vector<int> size_classes;

    vector<Vec2f> coefs; //Primeiro m, segundo d;
    
    vector<Mat> miniROI;
    vector<Mat> clusters;
    vector<Point2f> mmq;
    vector<Point2f> final_coef;

    vector<Vec4f> lines;
    
    vector<Vec4d> lines_a;
    vector<Vec4d> linesP;

    vector<vector<Vec2f> > linesC;
    vector<vector<Point2f> > coefsC;
    
    vector<Vec4f> flinesC;

    vector<pair<double,double > > coef_retas; //Vetor de coeficientes, método antigo
    vector<pair<double,double> > coef_med_retas; //Vetor de coeficientes médios, método antigo
    
    ofstream arquivo;
    //Aqui você coloca os métodos
    Camera(int id_cam, int region);

    void namingVideo(string name);
    void gettingSize(int width, int height);
    void creatingRoi(Mat image);
    
    void Segmentation(Mat image);
    void SegAndCluster(Mat image, int d);
    void verifingClusters(vector<vector<Point2f> > pline);
    void erodeConfig(int m, int n);
    void dilateConfig(int m, int n);
    void skeletonConfig(int m, int n);
    void morphologicalOperations(Mat image);
    void Otsu_first(Mat image, double min, double max);
    void Otsu_second(Mat image, double min, double max);
    void prevision(Mat image);
    
    
    void coefs1(vector<Vec4d> &vv); //Calcula os coeficientes das retas do HoughP
    void retas_med(vector<Vec4d> &pontos); //Calcula as retas médias no método antigo
    void findingCenters(Mat image); //Acha o centroide da imagem
    void ordinating(Mat image); 
    void hough(Mat image, int threshold);
    void houghP(Mat image, int nc, double lineLength, double maxGap);
    void miniROIs(Mat image);
    void ROIsOfClusters(Mat image);
    void houghOnClusters(vector<Mat> image);
    void coefsCluster(vector<vector<Vec2f> > lines);
    void dynamicROI(Mat image);
    void MMQ();
    void MMQ(int np_min);
    void R();
    void R2();
    void estimated_lines();
    

    void expanding_lines(vector<Point2f> coef_retas);
    void expanding_lines_a(vector<pair<double,double>> coef_retas);
    void expanding_lines_c(vector<Point2f> coef_retas);
    void drawLines();
    void drawLines_a();
    void drawLines_c();
    void centroids(Mat image);
    
    void writingFile(string name);
    
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

void Camera::writingFile(string name){
    string path = "arquivos/";
    string extention = ".txt";
    name += extention;
    path += name;
    this->arquivo.open(path, ios::out);
    if(!this->arquivo){
        cout << "Could not open the file"  << endl;
        abort();
    }   

    for(int i = 0; i < this->lines.size(); i++){
        //cout << this->lines[i] << endl;
        if(this->lines[i][2] - this->lines[i][0]){
            double ang = (this->lines[i][3] - this->lines[i][1])/(this->lines[i][2] - this->lines[i][0]);
            double lin = this->lines[i][1] - (ang)*(this->lines[i][0]);
            this->arquivo << " " << this->lines[i][0]
            << " " << this->lines[i][1]
            << " " << this->lines[i][2]
            << " " << this->lines[i][3]
            << " " << ang
            << " " << lin << endl;
        }
    }
    
    this->arquivo.close();
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

void Camera::SegAndCluster(Mat image, int d){
    int i = 0;
    int j = 0;

    Point2f aux;
    vector<Point2f> dot;
    vector<vector<Point2f> > clusters;

    int pb = 0;
    int position = 0;
    int dist = d;
    bool flag = false;
    bool f = false;

    dot.clear();
    clusters.clear();
	//cout<<"Iniciando a busca"<<endl;
    //cout << image << endl;
	for(i = 0; i<image.cols; i++){						
		for(j = (image.rows - 1); j >= 0; j--){
                uchar color = image.at<uchar>(j,i);
                //cout << "i: " << i << " j: " << j;
                if((double)color == 255){
                    /*
                    cout << "i: " << i << " j: " << j;
                    cout << " Valor do color: " << (double)color << endl;
                    */
                    if(flag == false){
                        position = i;
                        aux.x = i;
                        aux.y = j;
                        dot.push_back(aux);
                        //cout << "Ponto encontrado! Quantidade de pontos: " << dot.size() << " i: " << i << " j: " << j << endl;
                        f = true;
                        pb++;
                    }
                    else if(flag == true){
                        position = i;
                        aux.x = i;
                        aux.y = j;
                        f = true;
                    }

                    if((f == true) && (flag == true)){
                        clusters.push_back(dot);
                        //cout << "Numero de pontos na classe: " << dot.size() << endl;
                        dot.clear();
                        dot.push_back(aux);
                        pb++;
                        //cout << "Ponto encontrado! Quantidade de pontos: " << dot.size() << " i: " << i << " j: " << j << endl;
                        position = i;
                        flag = false;
                    }
                    f = false;
                }
                //cout << "else " << endl;
                //cout << "Ultimo branco:  " << position << " Ultimo preto: " << u << " i: " << i << endl;  
        }
        if((i - position) >= dist){
            flag = true;
        }    
	}
    clusters.push_back(dot);
    //cout << "Numero de pontos na classe: " << dot.size() << endl;
    //cout << "Total de pixels brnacos: " << pb << endl; 
    dot.clear();
    this-> pb = pb;
    this->pline = clusters;
    //cout << "Fim" << endl;
}

void Camera::verifingClusters(vector<vector<Point2f> > line){
    bool check = true;
    while(check){
        check = false;
        vector<vector<Point2f> > l;    
        vector<Point2f> split1;
        vector<Point2f> split2;

        l.clear();
        split1.clear();
        split2.clear();

            for(int i = 0; i < this->pline.size(); i++){
                if(this->pline[i].size()!=0){
                    if(this->pline[i].size() > (this->pb)*0.6){
                        check = true;
                        for(int j = 0; j < this->pline[i].size(); j++){
                            if(j > (this->pline[i].size())/2){
                                split1.push_back(this->pline[i][j]);
                            }
                            else{
                                split2.push_back(this->pline[i][j]);
                            }
                        }
                        l.push_back(split1);
                        l.push_back(split2);
                    }
                    else{
                        l.push_back(this->pline[i]);
                    }
                }
            }
        this->pline = l;
    }
}

void Camera::erodeConfig(int m, int n){
    this->element1 = getStructuringElement(MORPH_RECT,Size(m,n));
}

void Camera::dilateConfig(int m, int n){
    this->element2 = getStructuringElement(MORPH_RECT,Size(m,n));
}

void Camera::skeletonConfig(int m, int n){
    this->element3 = getStructuringElement(MORPH_CROSS,Size(m,n));
}

void Camera::morphologicalOperations(Mat image){
    //Opening
    erode(this->segmented,this->erosion,this->element1);
    dilate(this->erosion,this->dilation,this->element2);
    //Image Processing
    cvtColor(this->dilation, this->binarized, COLOR_BGR2GRAY, CV_8UC1);
    
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
    skel.copyTo(this->morph);
}

void Camera::Otsu_first(Mat image, double min, double max){
    Mat gray;
    imshow("Input", image);
    cvtColor(image, gray, CV_BGR2GRAY);
    threshold(gray, this->otsu, min, max, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);
    imshow("Otsu First", this->otsu);
}

void Camera::Otsu_second(Mat image, double min, double max){
    threshold(image, this->otsu, min, max, CV_THRESH_BINARY | CV_THRESH_OTSU);
    imshow("Otsu Second", this->otsu);
}

bool cmpVec2x(Point2f &a, Point2f &b){
		return a.x < b.x;	
}

bool cmpVecxf(Vec4d &a, Vec4d &b){
	return a[2] < b[2];
}

void Camera::coefs1(vector<Vec4d> &vv){
    vector<pair<double,double> > coef_temp;
    this->coef_retas.clear();
	double a = 0;
	double b = 0;	
	for(size_t i = 0; i<vv.size(); i++){
		    //cout << "xi =  " << vv[i][0] << "   xf =  "  << vv[i][2] << endl;
			Vec4f l = vv[i];
			if(l[2] - l[0]){
				a = (l[3] - l[1])/(l[2] - l[0]);
				b = (l[1] - a*l[0]);
				//cout << "coef ang:  " << a << "  coef lin:  " << b << endl;
				coef_temp.push_back(make_pair(a,b));
			}
		}
    this->coef_retas = coef_temp;
}

vector<pair<double,double > > coefs2(vector<vector<Vec4d> > &vv){
	vector<pair<double,double> > coef_temp;	
	double a = 0;
	double b = 0;	
	int j = 0;
	//cout << vv.size() << endl;
	for(size_t j = 0; j < vv.size(); j++){
		for(size_t i = 0; i<vv[j].size(); i++){
			Vec4d l = vv[j][i];
			if((l[2] - l[0])!=0){
				a = (l[3] - l[1])/(l[2] - l[0]);
				b = (l[1] - a*l[0]);
				//cout << "coef ang:  " << a << "  coef lin:  " << b << endl;
				coef_temp.push_back(make_pair(a,b));
			}
		}
	}
    return coef_temp;
}

void Camera::retas_med(vector<Vec4d> &pontos){
	vector<vector<Vec4d> > classes_total;
	//vector<Vec4d> init;
	Vec4d trans;
	
	vector<pair<double,double > > ans;
	
	int j = 0;
	
    this->coef_med_retas.clear();

	trans[0] = 0;
	trans[1] = 0;
	trans[2] = 0;
	trans[3] = 0;

	for(size_t i = 0;i<pontos.size();i++){
	//cout << "xi =  " << pontos[i][0] << "  yi =   " << pontos[i][1] << "  xf =  "  << pontos[i][2] << "  yf = " << pontos[i][3] << endl;
		if(((pontos[i+1][2] - pontos[i][2])>-50)&&((pontos[i+1][2] - pontos[i][2])<50)){
			trans[0] += pontos[i][0];
			trans[1] += pontos[i][1];
			trans[2] += pontos[i][2];
			trans[3] += pontos[i][3];
			j++;
		}
		else{
			Vec4d med;
			vector<Vec4d> classes;
			//cout << "xi =  " << trans[0] << "  yi =  " << trans[1] << "  xf =  "  << trans[2] << " yf = " << trans[3] << endl;
			trans[0] += pontos[i][0];
			trans[1] += pontos[i][1];
			trans[2] += pontos[i][2];
			trans[3] += pontos[i][3];
			j++;
            med[0] = (trans[0]/j);
			med[1] = (trans[1]/j);
			med[2] = (trans[2]/j);
			med[3] = (trans[3]/j);
			//cout << "xi_med =  " << med[0] << "  yi_med =  " << med[1] << "  xf_med =  "  << med[2] << " yf_med = " << med[3] << endl;
			classes.push_back(med);
			classes_total.push_back(classes);
			trans[0] = 0;
			trans[1] = 0;
			trans[2] = 0;
			trans[3] = 0;
			j = 0;
		}
	}
	//cout << "Tamanho:  " << classes_total.size() << endl;
	ans = coefs2(classes_total);
	this->coef_med_retas = ans;
}

void Camera::findingCenters(Mat image){
	this->point = Mat::zeros(Size(image.cols,image.rows),CV_8UC3);
    
    int i = 0;
	int j = 0;
	
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	vector<Point2f> aux(1);
	
    this->points.clear();

	findContours(this->morph, contours, RETR_TREE , CHAIN_APPROX_SIMPLE, Point(0, 0));
  
	vector<Moments> mu(contours.size());
	for( size_t i = 0; i < contours.size(); i++ ){ 
		mu[i] = moments( contours[i], false ); 
	}
	
	vector<Point2f> mc(contours.size() );
	for( size_t i = 0; i < contours.size(); i++ ){ 
		if(mu[i].m00 != 0){
            mc[i] = Point2f(static_cast<float>(mu[i].m10/mu[i].m00),static_cast<float>(mu[i].m01/mu[i].m00)); 
		//cout << mc[i].y << endl;
        }
	}

	//imshow( "Centroides", drawing );
	
	/*for(int i = 0; i<mc.size(); i++){
	cout << mc[i] << endl;
	
	}*/
	
	sort(mc.begin(),mc.end(),cmpVec2x);
	
	//cout << "Quantidade de Centroides   " << mc.size() << endl;
	
	while(i<mc.size()){
		if(((mc[i+1].x - mc[i].x)<(1))&&((mc[i+1].y - mc[i].y)<(1))){
			this->points.push_back(mc[i]);
			//cout << pontos[j].x << "      " << pontos[j].y << endl;
			i = i + 2;
			j++;
			}
		else{
			this->points.push_back(mc[i]);
			//cout << pontos[j].x << "      " << pontos[j].y << endl;
			i++;
			j++;
		}
	}
		
	image.copyTo(this->point);
	for( size_t i = 0; i<j; i++ ){
		Scalar color = Scalar( 255, 0, 0);
        //cout << this->points[i] << endl;
		circle(this->point, this->points[i], 4, color, -1, 8, 0 );
	}
	//imshow("Centroides filtrados", this->point);
	//cout << "Quantidade de Centroides Filtrados  " << points.size() << endl;
}

void Camera::ordinating(Mat image){
	int clss = 1;
	double range = 20;
	double theta = 1;
	vector<vector<Point2f> > pline;
    
    pline.clear();
    this->pline.clear();
    this->size_classes.clear();

	vector<Point2f> inicializacao;
	pline.push_back(inicializacao);

	pline[0].push_back(this->points[0]);
	//cout << "...adicionando..." << pontos[0] << "...na classe..." << (classes.size()-1) << endl;
	//cout << "Quantidade de Centroides Filtrados  " << pontos.size() << endl;
	
	if((this->points.size()>50)){
			range = 25;
			this->np_min = 3;
			//cout << "Definindo range para...   " << range << endl;
		}
	
	for(int i = 1; i<this->points.size(); i++){
		if(!(((this->points[i].x < (this->width)*0.02) && (this->points[i].y < (this->height/this->region)*0.35)) || ((this->points[i].x > (this->width)*0.98) && (this->points[i].y < (this->height/this->region)*0.35)))){
            if((this->points[i].x<=(this->points[i-1].x + (range*theta)))&&(this->points[i].x>=(points[i-1].x - (range*theta))) && (abs(this->points[i].y - (points[i-1].y)) < (this->height/this->region)*0.65)){ 
                pline[(pline.size()-1)].push_back(this->points[i]);
                //cout << ((points[i].x)-(points[i-1].x)) << endl;
                //cout << "...adicionando no if..." << points[i] << "...na classe..." << (pline.size()-1) << endl;
                clss ++;
                }
            else{
                vector<Point2f> temp;
                pline.push_back(temp);
                
                pline[(pline.size()-1)].push_back(this->points[i]);
                //cout << abs(((points[i].y)-(points[i-1].y))) << " " << (this->height/this->region)*0.65 << endl;
                //cout << "...adicionando no else..." << points[i] << "...na classe..." << (pline.size()-1) << endl;
                this->size_classes.push_back(clss);
                //cout << clss << endl;
                clss = 1;
            }
        }
	}
    
	this->size_classes.push_back(clss);
    this->pline = pline;
	//cout << "Quantidade de classes" << this->pline.size() << endl;
}

void Camera::hough(Mat image, int threshold){
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

    HoughLines(this->skeleton, lines, 1, CV_PI/180, threshold, 0, 0);
 
    for(int i = 0; i < lines.size(); i++){
        Vec2f auxc;
        Vec2f auxp1;
        Vec2f auxp2;
        Vec2f auxp3;
        Vec2f auxp4;
        
        if(sin(lines[i][1])){
            auxc[0] = -(cos(lines[i][1])/sin(lines[i][1])); //coef ang
            auxc[1] = lines[i][0]/sin(lines[i][1]); //coef lin
            if(!((auxc[0] > -3) && (auxc[0] < 3))){
                this->coefs.push_back(auxc);  
                auxp1[1] = this->frame_roi.rows;//y
                auxp1[0] = ((this->frame_roi.rows) - auxc[1])/(auxc[0]); //x
                //cout << "primeiro" << auxp1 << endl;
                auxp2[1] = this->frame_roi.rows - h;//y
                auxp2[0] = ((this->frame_roi.rows - h) - auxc[1])/(auxc[0]); //x
                //cout << "segundo" << auxp2 << endl;
                auxp3[1] = this->frame_roi.rows - 2*h;//y
                auxp3[0] = ((this->frame_roi.rows - 2*h) - auxc[1])/(auxc[0]); //x
                //cout << "terceiro" << auxp3 << endl;
                auxp4[1] = this->frame_roi.rows - 3*h;//y
                auxp4[0] = ((this->frame_roi.rows - 3*h) - auxc[1])/(auxc[0]); //x
                //cout << "quarto" << auxp4 << endl;
                if((auxp1[0] > image.cols) || (auxp2[0] > image.cols) || (auxp3[0] > image.cols) || (auxp4[0] > image.cols)){
                    i++;
                }
                else if((auxp1[0] < 0) || (auxp2[0] < 0) || (auxp3[0] < 0) || (auxp4[0] < 0)){
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

                //imshow("Points",this->init_point);
                
            }
        }
    }
}

void Camera::houghP(Mat image, int nc, double lineLength, double maxGap){
    this->linesP.clear();
    HoughLinesP(image, this->linesP, 1, CV_PI/180, nc, lineLength, maxGap); // 100, 100, 100); //32, 65, 100
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
    imshow("1",this->miniROI[0]);
    imshow("2",this->miniROI[1]);
    imshow("3",this->miniROI[2]);
    imshow("4",this->miniROI[3]);
    waitKey(0);
    */
}

void Camera::ROIsOfClusters(Mat image){
    this->clusters.clear();
    int xinit;
    //cout << this->pline.size() << endl;
    for(int i = 0; i < this->pline.size(); i++){
        if((pline[i].size()) > 30){
            //cout << i << endl;
            xinit = (int)pline[i][0].x;
            Rect ROI = Rect(xinit, 0,((pline[i][(pline[i].size() - 1)].x) - pline[i][0].x), image.rows);
            Mat image_roi = image(ROI);
            Mat aux;
            image_roi.copyTo(aux);
            //imshow("ROI", aux);
            //waitKey(0);
            this->clusters.push_back(aux);
        }
    }    
}

void Camera::houghOnClusters(vector<Mat> image){
    vector<vector<Vec2f> > linesC;
    for(int i = 0; i < this->clusters.size(); i++){
        vector<Vec2f> lines;
        HoughLines(image[i], lines, 1, CV_PI/180, 20, 0, 0);
        linesC.push_back(lines);
    }
    
    this-> linesC = linesC;
}

void Camera::coefsCluster(vector<vector<Vec2f> > lines){
    vector<vector<Point2f> > coefs;
    coefs.clear();

    for(int i = 0; i < lines.size(); i++){
        vector<Point2f> coefsC;
        coefsC.clear();
        for(int j = 0; j < lines[i].size(); j++){
            Point2f auxc;
            auxc.x = -(cos(lines[i][j][1])/sin(lines[i][j][1])); //coef ang
            auxc.y = lines[i][j][0]/sin(lines[i][j][1]); //coef lin
            coefsC.push_back(auxc);
        }
        coefs.push_back(coefsC);
    }
    this->coefsC = coefs;
}

int Camera::medianMatrix(Mat image){
    int soma = 0;
    int size = 0;
    for(int i = 0; i < image.rows; i++){
        for(int j = 0; j < image.cols; j++){
            uchar aux = image.at<uchar>(j,i);
            soma += (int)aux;
            //cout << soma << endl;
            size++;
        }
    }
    return (soma/size);
}

void Camera::prevision(Mat image){
    int px = 0;
    vector<int> graph;
    Mat g = Mat::zeros(Size(image.cols,image.rows),CV_8UC3);

    graph.clear();
    for(int i = 0; i<image.cols; i++){						
		for(int j = (image.rows - 1); j >= 0; j--){
            uchar aux = image.at<uchar>(j,i);
            cout << (double)aux << " " << i << " " << j << endl;
            if((double)aux == 255){
                px++;
            }
        }
        cout << px << endl;
        graph.push_back(px);
        circle(g, Point((graph.size() - 1),(image.rows - px)), 3, Scalar(255,0,0),-1,8,0);
        imshow("GRAPH", g);
        waitKey(50);
        px = 0; 
    }
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
                if((xinit1 + 20) < image.cols){
                    Rect ROI1 = Rect(xinit1 - 20, 3*h, 40, h);
                    this->image_roi1 = image(ROI1);
                }
                else{
                    Rect ROI1 = Rect(xinit1 - 20, 3*h, (image.cols - xinit1 + 19), h);
                    this->image_roi1 = image(ROI1);
                }
            }
            else{
                Rect ROI1 = Rect(0, 3*h, 40, h);
                this->image_roi1 = image(ROI1);
            }
            
            if((xinit2 - 20) > 0){
                if((xinit2 + 20) < image.cols){
                    Rect ROI2 = Rect(xinit2 - 20, 2*h, 40, h);
                    this->image_roi2 = image(ROI2);
                }
                else{
                    Rect ROI2 = Rect(xinit2 - 20, 2*h, (image.cols - xinit2 + 19), h);
                    this->image_roi2 = image(ROI2);
                }
            }
            else{
               Rect ROI2 = Rect(0, 2*h, 40, h);
               this->image_roi2 = image(ROI2);
            }

            if((xinit3 - 20) > 0){
                if((xinit3 + 20) < image.cols){
                    Rect ROI3 = Rect(xinit3 - 20, h, 40, h);
                    this->image_roi3 = image(ROI3);
                }
                else{
                    Rect ROI3 = Rect(xinit3 - 20, h, (image.cols - xinit3 + 19), h);
                    this->image_roi3 = image(ROI3);
                }
            }
            else{
               Rect ROI3 = Rect(0, h, 40, h);
               this->image_roi3 = image(ROI3);
            }

            if((xinit4 - 20) > 0){
                if((xinit4 + 20) < image.cols){
                    Rect ROI4 = Rect(xinit4 - 20, 0, 40, h);
                    this->image_roi4 = image(ROI4);
                }
                else{
                    Rect ROI4 = Rect(xinit4 - 20, 0, (image.cols - xinit4 + 19), h);
                    this->image_roi4 = image(ROI4);
                }
            }
            else{
               Rect ROI4 = Rect(0, 0, 40, h);
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
            
            /*
            imshow("Points",this->init_point);
            imshow("Primeira",this->image_roi1);
            imshow("Segunda",this->image_roi2);
            imshow("Terceira",this->image_roi3);
            imshow("Quarta",this->image_roi4);
            waitKey(0); 
            */
            
       
   }
}

void Camera::MMQ(){
    
    this->mmq.clear();
    
    for(int i = 0; i < this->pline.size(); i++){
        //cout << pline[i] << endl;
        Vec2f aux;
        float sx = 0, sy = 0, xx = 0, xy = 0, n = pline[i].size();
        for(int j = 0; j < this->pline[i].size(); j++){
            sx += this->pline[i][j].x;
            sy += this->pline[i][j].y;
            xx += (this->pline[i][j].x)*(this->pline[i][j].x);
            xy += (this->pline[i][j].x)*(this->pline[i][j].y);
        }
        aux[0] = ((sy*xx) - (sx*xy))/((n*xx)-(sx*sx));//((n*xy) - ((sx)*(sy)))/((n*xx) - ((sx)*(sx)));//Coef Ang;
        aux[1] = ((sy - (aux[0]*sx))/n); //Coef lin
        //cout << aux << endl;
        this->mmq.push_back(aux);
    }
}

void Camera::MMQ(int np_min){
    
    this->mmq.clear();
    //cout <<pline.size() << endl;
    for(int i = 0; i < this->pline.size(); i++){
        //cout << this->pline[i].size() << endl;
        if(this->pline[i].size() > np_min){
            Vec2f aux;
            float sx = 0, sy = 0, xx = 0, xy = 0, n = pline[i].size();
            for(int j = 0; j < this->pline[i].size(); j++){
                sx += this->pline[i][j].x;
                sy += this->pline[i][j].y;
                xx += (this->pline[i][j].x)*(this->pline[i][j].x);
                xy += (this->pline[i][j].x)*(this->pline[i][j].y);
            }

            aux[0] = ((sy*xx) - (sx*xy))/((n*xx)-(sx*sx));//((n*xy) - ((sx)*(sy)))/((n*xx) - ((sx)*(sx)));//Coef Ang;
            aux[1] = ((sy - (aux[0]*sx))/n); //Coef lin
        
            this->mmq.push_back(aux);
            
            /*if(abs(aux[0]) < 50){
                cout << aux << endl;
                this->mmq.push_back(aux);
            }*/
        }
    }
    //cout << "Quantidade de linhas: " << mmq.size() << endl;
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
        //cout << "R  " << result << endl;
        if(result < thrs){
            this->final_coef.push_back(mmq[i]);
        }
        else{
            Point2f aux;
            aux.x = this->coefs[i][0];
            aux.y = this->coefs[i][1];
            this->final_coef.push_back(aux);
        }
    }
}

void Camera::R2(){
    this->final_coef.clear();
    for(int i = 0; i < this->pline.size(); i++){
        double sum = 0;
        for(int j = 0; j < this->pline[i].size(); j++){
            int xp = (this->pline[i][j].y - this->mmq[i].y)/(this->mmq[i].x);
            sum += (this->pline[i][j].x - xp)*(this->pline[i][j].x - xp);
        }
        int result = sqrt(sum);
        //cout << "R  " << result << endl;
        if(result < thrs){
            this->final_coef.push_back(mmq[i]);
        }
    }
}

void Camera::expanding_lines(vector<Point2f> coef_retas){
	int xi = 0, yi = 0, xf = 0, yf = 0;
    int aux = 0;
    bool flag = false;
	vector<Vec4f> lines;
    vector<Vec4d> lines_a;

	lines.clear();
    lines_a.clear();
    this->lines.clear();
    this->lines_a.clear();

	for(int i = 0; i < coef_retas.size(); i++){	
		if (!((coef_retas[i].x > -3) && (coef_retas[i].x < 3))){
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
            //cout << "x_ord_trans" << x_ord_trans << endl;
            lines.push_back(x_ord_trans);
            lines_a.push_back(x_ord_trans);
        
    }
    }
	this->lines_a = lines_a;
	this->lines = lines;
}

void Camera::expanding_lines_a(vector<pair<double,double>> coef_retas){
	double xi = 0, yi = 0, xf = 0, yf = 0;
    int aux = 0;
    bool flag = false;
	vector<Vec4f> lines;
    vector<Vec4d> lines_a;

	lines.clear();
    lines_a.clear();
    this->lines.clear();
    this->lines_a.clear();

	for(int i = 0; i < coef_retas.size(); i++){	
		if (!((coef_retas[i].first > -3) && (coef_retas[i].first < 3))){
        xi = (yi - coef_retas[i].second)/coef_retas[i].first;
		yf = (this->height/this->region);
		xf = (yf - coef_retas[i].second)/coef_retas[i].first;
				
		if(xf>this->width){
			xf = this->width;
			yf = (coef_retas[i].first)*xf + coef_retas[i].second;
    	}

        
            Vec4d x_ord_trans;
            x_ord_trans[0] = xi;
            x_ord_trans[1] = yi;
            x_ord_trans[2] = xf;
            x_ord_trans[3] = yf;
            //cout << "x_ord_trans" << x_ord_trans << endl;
            lines.push_back(x_ord_trans);
            lines_a.push_back(x_ord_trans);
    }
    }
    this->lines_a = lines_a;
	this->lines = lines;
    

}

void Camera::expanding_lines_c(vector<Point2f> coef_retas){
	int xi = 0, yi = 0, xf = 0, yf = 0;
    int aux = 0;
    bool flag = false;
	vector<Vec4f> lines;

	lines.clear();
    this->lines.clear();

	for(int i = 0; i < coef_retas.size(); i++){	
		if((pline[i].size()) > 50){
            if (!((coef_retas[i].x > -3) && (coef_retas[i].x < 3))){
                xi = (yi - coef_retas[i].y)/coef_retas[i].x;
                if(xi < this->pline[i][0].x){
                    xi = this->pline[i][0].x;
                    yi = xi*(coef_retas[i].x) + coef_retas[i].y;
                }
                if(xi > this->pline[i][pline[i].size() - 1].x){
                    xi = this->pline[i][pline[i].size() - 1].x;
                    yi = xi*(coef_retas[i].x) + coef_retas[i].y;
                }

                yf = (this->height/this->region);
                xf = (yf - coef_retas[i].y)/coef_retas[i].x;
                        
                if(xf>this->pline[i][pline[i].size() - 1].x){
                    xf = this->pline[i][pline[i].size() - 1].x;
                    yf = (coef_retas[i].x)*xf + coef_retas[i].y;
                }

                if(xf<this->pline[i][0].x){
                    xf = this->pline[i][0].x;
                    yf = (coef_retas[i].x)*xf + coef_retas[i].y;
                }

                
                    Vec4f x_ord_trans;
                    x_ord_trans[0] = xi;
                    x_ord_trans[1] = yi;
                    x_ord_trans[2] = xf;
                    x_ord_trans[3] = yf;
                    lines.push_back(x_ord_trans);
            }
            else{
                Vec4f x_ord_trans;
                x_ord_trans[0] = (pline[i][0].x + pline[i][(pline[i].size() - 1)].x)/2.0;
                x_ord_trans[1] = 0;
                x_ord_trans[2] = x_ord_trans[0];
                x_ord_trans[3] = this->frame_roi.rows;
                lines.push_back(x_ord_trans);
            }  
        }
    }
	this->lines  = lines;
}

void Camera::estimated_lines(){
	int xi = 0, yi = 0, xf = 0, yf = 0;
    int aux = 0;
    bool flag = false;
	vector<Vec4f> lines;

	lines.clear();
    this->lines.clear();
    for(int i = 0; i < this->pline.size(); i++){
        if((pline[i].size()) > 30){
            Vec4f x_ord_trans;
            x_ord_trans[0] = (pline[i][0].x + pline[i][(pline[i].size() - 1)].x)/2.0;
            x_ord_trans[1] = 0;
            x_ord_trans[2] = x_ord_trans[0];
            x_ord_trans[3] = this->frame_roi.rows;
            lines.push_back(x_ord_trans);
        }
    }
    this->lines  = lines;
}

void Camera::drawLines(){
    //cout << "Quantidade de linhas  " << this->lines.size() << endl;
    for(int i = 0; i < this->lines.size(); i++){
		line(this->frame_final, Point(this->lines[i][0], (this->lines[i][1] + (this->height - (this->height/this->region)))), Point(this->lines[i][2], (this->lines[i][3] + (this->height - (this->height/this->region)))), Scalar(255,0,0), 7, LINE_AA);
	}
    //imshow("Resultado",this->frame_final);
}

void Camera::drawLines_a(){
    //cout << "Quantidade de linhas  " << this->lines_a.size() << endl;
    for(int i = 0; i < this->lines_a.size(); i++){
		line(this->frame_final, Point(this->lines_a[i][0], (this->lines_a[i][1] + (this->height - (this->height/this->region)))), Point(this->lines_a[i][2], (this->lines_a[i][3] + (this->height - (this->height/this->region)))), Scalar(255,0,0), 7, LINE_AA);
	}
    //imshow("Resultado",this->frame_final);
}

void Camera::drawLines_c(){
    //cout << "Quantidade de linhas  " << this->flinesC.size() << endl;

    for(int i = 0; i < this->flinesC.size(); i++){
		line(this->frame_final, Point(this->flinesC[i][0], (this->flinesC[i][1] + (this->height - (this->height/this->region)))), Point(this->flinesC[i][2], (this->flinesC[i][3] + (this->height - (this->height/this->region)))), Scalar(255,0,0), 7, LINE_AA);
	}
    //imshow("Resultado",this->frame_final);
}