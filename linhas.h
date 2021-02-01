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
#include <time.h>
#include <math.h>

#define thrs 10

using namespace std;
using namespace cv;

class Camera{
    private:
    //Aqui você coloca as variáveis que a classe irá usar, os atributos
    float k = 0.65;
    float t = 20.0;
    
    public:
    int id;
    int region;
    int height;
    int width;

    int np_min = 1;
    int nNozzles = 4;
    int pb;
    
    int number_pixel;
    int thresh;

    string img_ext = ".png";
    string img_fn;

    VideoCapture cap;
    VideoWriter p;
    VideoWriter c;

    Mat bwImage;
    Mat frame;
    Mat frame_final;
    Mat frame_roi;
    Mat frame_simulacao;
    Mat otsu;
    Mat segmented;
    Mat erosion;
    Mat dilation;
    Mat binarized;
    Mat skeleton;
    Mat morph;
    Mat canny;
    Mat mcanny;
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
    string name;
    
    vector<vector<Point>> contours;

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

    //Eigens
    vector<Mat> eigvect;
    vector<Mat> eigvals;
    vector<Point2f> med;
    
    vector<pair<double,double > > coef_retas; //Vetor de coeficientes, método antigo
    vector<pair<double,double > > coef_retas_wrong; //Vetor de coeficientes, método antigo
    
    vector<pair<double,double> > coef_med_retas; //Vetor de coeficientes médios, método antigo
    
    ofstream arquivo;
    ofstream datalog;
    ofstream tempos;
    ofstream all;

    clock_t last_time;

    //Aqui você coloca os métodos
    Camera(int id_cam, int region);

    void namingVideo(string name);
    void gettingSize(int width, int height);
    void creatingRoi(Mat image);
    
    //Métodos de Segmentação
    void Segmentation(Mat image);
    void Segmentation2(Mat image);
    void Segmentation3(Mat image);
    void Segmentation4(Mat image);
    void Segmentation5(Mat image);
    void limiarSeg(Mat image);

    void SegAndCluster(Mat image, int d);
    void verifingClusters(vector<vector<Point2f> > pline);
    void Clusters(Mat image, vector<vector<Point2f> > line);
    
    //Operações Morfológica
    void erodeConfig(int m, int n);
    void dilateConfig(int m, int n);
    void skeletonConfig(int m, int n);
    void morphOp(Mat image);
    void morphologicalOperations(Mat image);
    void medianCanny(Mat image);
    void Otsu_first(Mat image, double min, double max);
    void Otsu_second(Mat image, double min, double max);
    void prevision(Mat image);
    
    void eigens(vector<vector<Point2f> > line);
    void eigenLines(vector<Mat> eigvect, vector<Mat> eigvals, vector<Point2f> med);
    void KMeans(Mat image, int min, int max);

    Mat imfill(Mat image);
    Mat strips(Mat strip, int number_pixel);
    Mat imquantize(Mat image, int thresh);
    Mat concatenate(Mat strip1, Mat strip2, Mat strip3, Mat strip4, Mat strip5, Mat strip6, Mat strip7, Mat strip8, Mat strip9, 
                    Mat strip10, Mat strip11, Mat strip12, Mat strip13, Mat strip14, Mat strip15, Mat strip16, Mat strip17,
                    Mat strip18, Mat strip19, Mat strip20);
    void coefs1(vector<Vec4d> &vv); //Calcula os coeficientes das retas do HoughP
    void intersections(vector<pair<double,double>> coef_retas);
    void retas_med(vector<Vec4d> &pontos); //Calcula as retas médias no método antigo
    void findingCenters(Mat image); //Acha o centroide da imagem
    void ordinating(Mat image, double range); 
    void hough(Mat image, int threshold, double mn, double mx);
    void houghP(Mat image, int nc, double lineLength, double maxGap, double mn, double mx);
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
    void vanishing_point(vector<pair<double,double>> coef_retas);

    void expanding_lines(vector<Point2f> coef_retas, double p, double n);
    void expanding_lines_a(vector<pair<double,double>> coef_retas, double mn, double mx);
    void expanding_lines_c(vector<Point2f> coef_retas, double p, double n);
    void drawLines();
    void drawLines_wrong();
    void drawLines_a();
    void drawLines_c();
    void centroids(Mat image);
    void nozzles();
    
    void writingFile(string name);
    void dataLog(string name, string met);
    
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
    //this->datalog << "gettingSize" << endl;
    //this->datalog << "Width: " << width << " Height: " << height << endl;
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
    string path = "Arquivos/";
    string extention = ".txt";
    name += extention;
    path += name;
    this->arquivo.open(path, ios::out);
    if(!this->arquivo){
        cout << "Não conseguir abrir o arquivo final"  << endl;
        abort();
    }   

    for(int i = 0; i < this->lines.size(); i++){
        //cout << this->lines[i] << endl;
        if(this->lines[i][2] - this->lines[i][0]){
            double ang = (this->lines[i][3] - this->lines[i][1])/(this->lines[i][2] - this->lines[i][0]);
            double lin = this->lines[i][1] - (ang)*(this->lines[i][0]);
            this->arquivo << this->lines[i][0]
            << " " << this->lines[i][1]
            << " " << this->lines[i][2]
            << " " << this->lines[i][3]
            << " " << ang
            << " " << lin << endl;
        }
    }
    
    this->arquivo.close();
}

void Camera::dataLog(string name, string met){
    string path = "DataLog/";
    string path_t = "Tempos/";
    string path_a = "All/";
    string extension = ".txt";
    name += extension;
    path += name;
    path_t += name;

    met += extension;
    path_a += met;

    //this->datalog.open(path, ios::out);
    this->tempos.open(path_t, ios::out);
    this->all.open(path_a, ios::app);

    /*if(!this->datalog){
        cout << "Falha ao iniciar o DataLog"  << endl;
        abort();
    }*/

    if(!this->tempos){
        cout << "Falha ao iniciar o DataLog"  << endl;
        abort();
    }

    this->tempos << "Iniciando o txt...: " << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;

    if(!this->all){
        cout << "Falha ao iniciar o DataLog"  << endl;
        abort();
    }
}

void Camera::creatingRoi(Mat image){
    this->tempos << "Iniciando Creating ROI: " << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
    int xinit = 0;  
	int yinit = ((this->height) - (this->height/this->region));
    
    Rect ROI = Rect(xinit, yinit,this->width,(this->height/this->region));
	Mat image_roi = image(ROI);
    image_roi.copyTo(this->frame_roi);
    //this->datalog << "creatingROI" << endl;
    //this->datalog << "ROI " << xinit << " " << yinit << " "
    //<< this->width << " " << (this->height/this->region) << endl;

    this->tempos << "Finalizando Creating ROI: " << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
}

void Camera::Segmentation(Mat image){
    //cout << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
    this->tempos << "Iniciando a Segmentação: " << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
    
    vector<Mat> chanels;
    Mat RB;
    Mat M1, M2;
    ofstream arq;
    string path = "Segmentation/";
    string extention = ".txt";
    
    this->name += extention;
    path += this->name;
    /*arq.open(path, ios::out);
    
    if(!arq){
        cout << "Falha ao abrir o DataLog da Segmentação"  << endl;
        abort();
    }*/
    
    split(image, chanels);
    add(chanels[2], chanels[0], RB);
    threshold(RB,M1,this->t,255,0);
    imshow("M1",M1); 
    for(int i = 0; i<image.cols; i++){						//imagem.size().width = imagem.cols -> dependendo de como a imagem está
		for(int j = 0; j<image.rows; j++){					//imagem.size().height = imagem.rows -> dependendo de como a imagem está
			Vec3b color = image.at<Vec3b>(Point(i,j));
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
			image.at<Vec3b>(Point(i,j)) = color; //Granvando os novos pixeis na matriz
		}
	}
    //Mat aux1 = (RB*(this->k));
    //Mat aux2;
    //aux1.convertTo(aux2, CV_8UC1);
    //M2 = (chanels[1]) > aux2;
    image.copyTo(M2);
    cvtColor(image,M2, COLOR_BGR2GRAY);
    //imshow("M2",M2);
    bitwise_and(M1,M2,this->segmented);
    //cvtColor(this->segmented,this->segmented, COLOR_GRAY2BGR);
    this->tempos << "Finalizando Segmentação: " << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;

    //arq.close();
    //cout << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;

}

void Camera::Segmentation2(Mat image){
    //cout << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
    this->tempos << "Iniciando a Segmentação: " << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
    
    vector<Mat> chanels;
    Mat RB;
    
    split(image, chanels);
    add(2.4*chanels[2], 2.4*chanels[0], RB);
    subtract(4*chanels[1],RB,this->segmented);
    threshold(this->segmented,this->segmented, 0, 255, CV_THRESH_BINARY);
    //cvtColor(this->segmented,this->segmented, COLOR_GRAY2BGR);
    this->tempos << "Finalizando Segmentação: " << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
    //cout << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
}

void Camera::Segmentation3(Mat image){
    //cout << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
    this->tempos << "Iniciando a Segmentação: " << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
    
    vector<Mat> chanels;
    Mat RB;
    Mat BGR;
    Mat R, G, B;

    split(image, chanels);
    add(chanels[2], chanels[0], RB);
    add(RB, chanels[1], BGR);

    divide(chanels[0], BGR, B);
    divide(chanels[1], BGR, G);
    divide(chanels[2], BGR, R);

    imshow("B", B);
    imshow("G", G);
    imshow("R", R);
    
    add(R, B, RB);
    
    subtract(2*G,RB,this->segmented);
    threshold(this->segmented,this->segmented, 0, 255, CV_THRESH_BINARY + CV_THRESH_OTSU);
    //cvtColor(this->segmented,this->segmented, COLOR_GRAY2BGR);
    this->tempos << "Finalizando Segmentação: " << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
    //cout << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
}

void Camera::Segmentation4(Mat image){
    //cout << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
    this->tempos << "Iniciando a Segmentação: " << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
    
    vector<Mat> chanels;
    Mat RB;
    Mat R, G, B;

    double mB, mG, mR;

    split(image, chanels);

    minMaxLoc(chanels[0],NULL,&mB);
    minMaxLoc(chanels[1],NULL,&mG);
    minMaxLoc(chanels[2],NULL,&mR);

    cout << "mB: " << mB << endl;
    cout << "mG: " << mG << endl;
    cout << "mR: " << mR << endl;

    divide(chanels[0], mB, B);
    divide(chanels[1], mG, G);
    divide(chanels[2], mR, R);
    /*
    imshow("B", B);
    imshow("G", G);
    imshow("R", R);

    minMaxLoc(B,NULL,&mB);
    minMaxLoc(G,NULL,&mG);
    minMaxLoc(R,NULL,&mR);
    
    cout << "mB: " << mB << endl;
    cout << "mG: " << mG << endl;
    cout << "mR: " << mR << endl;
    */

    add(R, B, RB);
    subtract(2*G,RB,this->segmented);
    //threshold(this->segmented,this->segmented, 50, 255, CV_THRESH_OTSU);
    //cvtColor(this->segmented,this->segmented, COLOR_GRAY2BGR);
    this->tempos << "Finalizando Segmentação: " << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
    //cout << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
}

void Camera::Segmentation5(Mat image){
    Mat gray;
    cvtColor(image, gray, CV_BGR2GRAY);
    threshold(gray,this->segmented,0,255,CV_THRESH_OTSU);
}

void Camera::limiarSeg(Mat image){
    Mat hsv, gray;
    Mat mask1;
    
    //cout << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
    cvtColor(image, hsv, COLOR_BGR2HSV);
    inRange(hsv, Scalar(25, 100, 60), Scalar(95, 255,255), mask1);
    bitwise_and(image, image, this->segmented, mask1);
    //cvtColor(this->segmented, gray, COLOR_BGR2GRAY);
    //threshold(gray,this->segmented, 0, 255, 0);
    //cout << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;

}

void Camera::erodeConfig(int m, int n){
    this->element1 = getStructuringElement(MORPH_RECT,Size(m,n));
    //this->datalog << "ErodeConfig: " << endl;
    //this->datalog << "m: " << m << " n: " << n << endl;
}

void Camera::dilateConfig(int m, int n){
    this->element2 = getStructuringElement(MORPH_RECT,Size(m,n));
    //this->datalog << "DilateConfig: " << endl;
    //this->datalog << "m: " << m << " n: " << n << endl;
}

void Camera::skeletonConfig(int m, int n){
    this->element3 = getStructuringElement(MORPH_CROSS,Size(m,n));
    //this->datalog << "SkeletonConfig: " << endl;
    //this->datalog << "m: " << m << " n: " << n << endl;
}

void Camera::morphOp(Mat image){
    this->tempos << "Iniciando a Erosão e Dilatação: " << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
    //Opening
    erode(image,this->erosion,this->element1);
    dilate(this->erosion,this->dilation,this->element2);
    //Image Processing
    //cvtColor(this->dilation, this->binarized, COLOR_BGR2GRAY, CV_8UC1);
    threshold(this->dilation,this->binarized, 0, 255, 0);
    Canny(this->binarized, this->canny, 50, 150, 3);
    binarized.copyTo(this->morph);
    this->tempos << "Finalizando a Erosão e Dilatação: " << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
}

void Camera::morphologicalOperations(Mat image){
    this->tempos << "Iniciando Operações Morfológicas: " << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
    //Opening
    erode(this->segmented,this->erosion,this->element1);
    dilate(this->erosion,this->dilation,this->element2);
    //Image Processing
    //cvtColor(this->dilation, this->binarized, COLOR_BGR2GRAY, CV_8UC1);
    threshold(this->dilation,this->binarized, 0, 255, 0);
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
    
    Canny(skel, this->canny, 0, 100, 3);
    skel.copyTo(this->skeleton);
    skel.copyTo(this->morph);
    this->tempos << "Finalizando Operações Morfológicas: " << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
}

void Camera::medianCanny(Mat image){
    Mat dst = Mat::zeros(Size(image.cols,image.rows),CV_8UC1);
    Canny(image, this->canny, 50, 150, 3);
    int x1 = 0, d = 0;
    bool fx1 = false;
    for(int i = (this->canny.rows - 1); i >= 0; i--){
        for(int j = 0; j < this->canny.cols; j++){
            uchar aux = this->canny.at<uchar>(i,j);
            if(d<20){
                if(((double)aux == 255) && (fx1 == false)){
                    x1 = j;
                    fx1 = true;
                }
                else if(((double)aux == 255) && (fx1 == true)){
                    int md = (j + x1)/2; 
                    //cout << "media " << i << " " << x1 << " " << (j-x1) << " " << md << " " << d << endl;
                    dst.at<uchar>(i,md) = (uchar)255;//circle(dst,Point(md,i),1,Scalar(255),FILLED,LINE_AA);
                    x1 = 0;
                    fx1 = false;
                    d = 0;
                }
            }
            else if(d==20){
                    //cout << "single " << i << " " << j << " " << x1 << " " << (j-x1) << " " << d << endl;
                    dst.at<uchar>(i,x1) =  (uchar)255;//circle(dst,Point(x1,i),1,Scalar(255),FILLED,LINE_AA);
                    x1 = 0; 
                    fx1 = false;
                    d=0;
            } 
            
            if(fx1 == true){
                d++;
            }
            
            //imshow("Canny process", this->canny);
            //imshow("Process", dst);
            //waitKey(1);
        }
    }
    dst.copyTo(this->mcanny);
}

void Camera::SegAndCluster(Mat image, int d){
    this->tempos << "Iniciando Clusterização pixel-a-pixel: " << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
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
    //this->datalog << "SegAndCluster" << endl;
    //this->datalog << "Distancia: " << dist << endl;
    //this->datalog << "Iniciando a busca" << endl;
    //cout << image << endl;
	for(i = 0; i<image.cols; i++){						
		for(j = (image.rows - 1); j >= 0; j--){
                uchar color = image.at<uchar>(j,i);
                //cout << "i: " << i << " j: " << j;
                if((double)color == 255){
                    
                    //this->datalog << "i: " << i << " j: " << j
                    //<< " Valor do color: " << (double)color << endl;
                    
                    if(flag == false){
                        position = i;
                        aux.x = i;
                        aux.y = j;
                        dot.push_back(aux);
                        //this->datalog << "Ponto encontrado! Quantidade de pontos: " << dot.size() << " i: " << i << " j: " << j << endl;
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
                        //this->datalog << "Numero de pontos na classe: " << dot.size() << endl;
                        dot.clear();
                        dot.push_back(aux);
                        pb++;
                        //this->datalog << "Ponto encontrado! Quantidade de pontos: " << dot.size() << " i: " << i << " j: " << j << endl;
                        position = i;
                        flag = false;
                    }
                    f = false;
                }
                //cout << "else " << endl;
                //this->datalog << "Ultimo branco:  " << position << endl;  
        }
        if((i - position) >= dist){
            flag = true;
        }    
	}
    clusters.push_back(dot);
    //this->datalog << "Numero de pontos na classe: " << dot.size() << endl;
    //this->datalog << "Total de pixels brnacos: " << pb << endl; 
    dot.clear();
    this-> pb = pb;
    this->pline = clusters;
    //this->datalog << "Fim" << endl;

    this->tempos << "Finalizando Clusterização pixel-a-pixel: " << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
}

void Camera::verifingClusters(vector<vector<Point2f> > line){
    this->tempos << "Iniciando a verificação dos clusters: " << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
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

    this->tempos << "Finalizando a verificação dos clusters: " << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
}

void Camera::Clusters(Mat image, vector<vector<Point2f> > line){
    int x1 = 0, x2 = 0, y1 = 0, y2 = 0, size1 = 0, size2 = 0;
    int xi = 0, xf = 0, yi = 0, yf = 0;
    vector<Vec4f> lines;
    lines.clear(); 
    for(int c = 0; c<line.size(); c++){
        //cout << c << " " << line.size() <<  endl; 
        //cout << c << " " << line[c].size() <<  endl;        
        for(int i = line[c][0].x; i<line[c][(line[c].size() - 1)].x; i++){						
            //cout << "Hello2" << endl;
            for(int j = (image.rows - 1); j >= 0; j--){
                //cout << "Hello3" << endl;
                uchar color = image.at<uchar>(j,i);
                if((double)color == 255){
                    //cout << "Hello" << endl;
                    if(j < ((this->height)/(this->region))*0.5){
                        x1 += i;
                        y1 += j;
                        size1++;
                        //cout << "Primeira " << (x1/size1) << " " << (y1/size1) << endl;
                    }
                    else{
                        x2 += i;
                        y2 += j;
                        size2++;
                        //cout << "Segunda " <<  (x2/size2) << " " << (y2/size2) << endl;
                    }
                }
                 
            }
        }
        if((size1 != 0) && (size2 != 0)){
            Vec4f temp;
            temp[0] = (x1/size1);
            temp[1] = (y1/size1);
            temp[2] = (x2/size2);
            temp[3] = (y2/size2);
            
            double a = (temp[3] - temp[1])/(temp[2] - temp[0]);
            double b = (temp[1] - a*temp[0]);

            xi = (yi - b)/a;
		    yf = (this->height/this->region);
		    xf = (yf - b)/a;
				
		    if(xf>this->width){
			    xf = this->width;
			    yf = (a)*xf + b;
    	    }
            
            //cout << xi << " " << yi << " " << xf << " " << yf << endl;
            temp[0] = xi;
            temp[1] = yi;
            temp[2] = xf;
            temp[3] = yf;
            x1 = 0;
            x2 = 0;
            y1 = 0;
            y2 = 0;
            size1 = 0;
            size2 = 0;
            lines.push_back(temp);
        }
    }
    this->lines = lines;
}

void Camera::eigenLines(vector<Mat> eigvect, vector<Mat> eigvals, vector<Point2f> med){
    int x1 = 0, x2 = 0, y1 = 0, y2 = 0, size1 = 0, size2 = 0;
    int xi = 0, xf = 0, yi = 0, yf = 0;
    double lb1 = 0, lb2 = 0;
    double eivecx1 = 0, eivecx2 = 0, eivecy1 = 0, eivecy2 = 0;
    vector<Vec4f> lines;
    vector<pair<double,double> > cf;

    cf.clear();
    lines.clear(); 
    
    //cout << "O numero de clusters e: " << eigvect.size() << endl;
    for(int c = 0; c<eigvect.size(); c++){        
        eivecx1 = eigvect[c].at<double>(0,0);
        eivecy1 = eigvect[c].at<double>(0,1);
        lb1 = sqrt(eigvals[c].at<double>(0,0));
        Vec4d temp1;
        temp1[0] = med[c].x; 
        temp1[1] = ((this->height) - ((this->height)/4)) + med[c].y;
        temp1[2] = med[c].x + (eivecx1*lb1);
        temp1[3] = ((this->height) - ((this->height)/4)) + med[c].y + (eivecy1*lb1);
        //cout << eivecx1 << " " << eivecy1 << " " << lb1 << endl;
        //cout << temp1[0] << " " << temp1[1] << " " << temp1[2] << " " << temp1[3] << endl;
        double ang = (atan((temp1[3] - temp1[1])/(temp1[2] - temp1[0]))*180)/CV_PI;
        if((60 <= abs(ang)) && (abs(ang) <= 120)){
            double a = (temp1[3] - temp1[1])/(temp1[2] - temp1[0]);
            double lin = (temp1[1] - (a*temp1[0]));
            cf.push_back(make_pair(a,lin));
            //lines.push_back(temp1);
            //cout << "Aceito, angulo: " << ang << endl;
        }
        else{
            //cout << "Rejeitado, angulo: " << ang << endl;
        }  
    }
    this->coef_retas = cf;
    this->lines = lines;
}

void Camera::eigens(vector<vector<Point2f> > line){
    double dx = 0, dy = 0, c = 0, n = 0;
    double xb = 0, yb = 0;
    Mat src;
    vector<Mat> eigvect;
    vector<Mat> eigvals;
    vector<Point2f> med;

    eigvect.clear();
    eigvals.clear();
    med.clear();
    this->eigvect.clear();
    this->eigvals.clear();
    this->med.clear();
    
    for(int i = 0; i<line.size(); i++){
        for(int j = 0; j<line[i].size(); j++){
            xb += line[i][j].x;
            yb += line[i][j].y;
            n++;
        }
        
        
        if((n-1) != 0){

            xb = (xb/n);
            yb = (yb/n);

            //cout << "xb: " << xb << " yb: " << yb << endl;
            for(int j = 0; j<line[i].size(); j++){
                dx += (line[i][j].x - xb)*(line[i][j].x - xb);
                dy += (line[i][j].y - yb)*(line[i][j].y - yb);
                c += (line[i][j].x - xb)*(line[i][j].y - yb);
            }
            //cout << "dx: " << dx << " dy: " << dy << " cov: " << c << " n - 1: " << (n-1) << endl;
            dx = (dx/(n - 1));
            dy = (dy/(n-1));
            c = (c/(n-1));
            
            src = Mat::zeros(Size(2,2),CV_64F);
            
            src.at<double>(0,0) = dx;
            src.at<double>(0,1) = c;
            src.at<double>(1,0) = c;
            src.at<double>(1,1) = dy;
            
            //cout << src << endl;
            Mat eigenvalues, eigenvectors;
            eigen(src, eigenvalues, eigenvectors);

            eigvals.push_back(eigenvalues);
            eigvect.push_back(eigenvectors);

            Point2f p;
            p.x = xb;
            p.y = yb;
            med.push_back(p);

            xb = 0;
            yb = 0;
            n = 0;
            c = 0;
            dx = 0;
            dy = 0;
            //cout << eigenvalues << endl;
            //cout << eigenvectors << endl;
            //cout << "Next" << endl;
        }
        else{
            //cout << "excluindo..." << endl;
        }
    }
    this->eigvect = eigvect;
    this->eigvals = eigvals;
    this->med = med;
}

void Camera::KMeans(Mat image, int min, int max){
    Mat img;
    vector<Point2f> dados;
    vector<Point2f> centro;
    vector<vector<Point2f> > centrs;
    vector<Mat> labels;
    vector<double> elbow;
    vector<vector<Point2f> > p;

    int cls = this->pline.size();

    p.clear();
    elbow.clear();
    labels.clear();
    centrs.clear();
    centro.clear();
    dados.clear();
    this->pline.clear();

    for(int i = 0; i<image.cols; i++){						
		for(int j = (image.rows - 1); j >= 0; j--){
            uchar aux = image.at<uchar>(j,i);
            if((double)aux == 255){
                Point2f temp;
                temp.x = i;
                temp.y = j;
                dados.push_back(temp);
            }
        } 
    }

    for (int k = min; k<=max; k++){
        Mat label;
        double compactness = kmeans(dados, k, label, TermCriteria(TermCriteria::EPS+TermCriteria::COUNT, 50, 1.0),3, KMEANS_PP_CENTERS,centro);
        elbow.push_back(compactness/dados.size());
        labels.push_back(label);
        centrs.push_back(centro);
    }

    int clus = 0;
    double a = (elbow[(elbow.size() - 1)] - elbow[0])/(max - min);
    double c = (elbow[0] - a*min);
    vector<double> distancia;
    distancia.clear();

    for(int i = 0; i < elbow.size(); i++){
        double d = abs(a*(i+min) - elbow[i] + (c))/(sqrt((a*a) + 1));
        distancia.push_back(d);
        //cout << "d " << d << endl;
    }
    
    double mx = 0;
    for(int i = 0; i < distancia.size(); i++){
        if(mx < distancia[i]){
            clus = i;
            mx = distancia[i];
        }
    }

    /*if(clus < cls){
        clus = cls - 3;
        cout << "Mudando para o valor do SegAndCluster" << endl;
    }
    cout << "Numero de cluster: " << (clus + 3) << " labels " << labels.size() << endl;
    */
    p.resize(clus + min);
    //cout << " Tamanho: " << p.size() << endl;

    img = Mat::zeros(Size(image.cols,image.rows), CV_8UC3);
    for(int j = 0; j < p.size(); j++){
        for(int i = 0; i < dados.size(); i++ ){
            if(labels[clus].at<int>(i) == j){
                p[j].push_back(dados[i]);
                circle( img, dados[i], 2, Scalar(255,255,255), FILLED, LINE_AA );
            }
        }
        
    }
   
   for (int i = 0; i < (int)centrs[clus].size(); i++){
        Point2f c = centrs[clus][i];
        circle( img, c, 5, Scalar(0,255,0), -1, LINE_AA );
    }
    //cout << "Compactness: " << compactness << endl;
    /*imshow("clusters", img);
    waitKey(1000);
*/
    this->pline = p;
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

bool cmpVecxff(Vec4f &a, Vec4f &b){
	return a[2] < b[2];
}

void Camera::coefs1(vector<Vec4d> &vv){
    this->tempos << "Iniciando Coefs1: " << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
    vector<pair<double,double> > coef_temp;
    this->coef_retas.clear();
	double a = 0;
	double b = 0;	
	for(size_t i = 0; i<vv.size(); i++){
		    //this->datalog << "Coefs1" << endl;
            //this->datalog <<"xi =  " << vv[i][0] << "   xf =  "  << vv[i][2] << endl;
			Vec4f l = vv[i];
			if(l[2] - l[0]){
				a = (l[3] - l[1])/(l[2] - l[0]);
				b = (l[1] - a*l[0]);
				//cout << "coef ang:  " << a << "  coef lin:  " << b << endl;
				coef_temp.push_back(make_pair(a,b));
			}
		}
    this->coef_retas = coef_temp;
    this->tempos << "Finalizando Coefs1: " << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
}

void Camera::intersections(vector<pair<double,double>> coef_retas){
    bool find = false, p = true;
    float x =0 , y = 0;
    vector<pair<double,double>> coef_retas_f;
    vector<pair<double,double>> coef_retas_temp;
    vector<Vec4f> intersection;

    coef_retas_f.clear();
    coef_retas_temp.clear();

    
    for(int i = 0; i < coef_retas.size(); i++){
        for(int j = 0; j < coef_retas.size(); j++){
            if((j != i) && (coef_retas[i].first != coef_retas[j].first)){
                Vec4f temp;
                x = (coef_retas[i].second - coef_retas[j].second)/((-coef_retas[i].first) - (-coef_retas[j].first));
                y = (((coef_retas[j].first)*(coef_retas[i].second)) + ((-coef_retas[i].first)*(coef_retas[j].second)))/((-coef_retas[i].first) - (-coef_retas[j].first));
                temp[0] = x;
                temp[1] = y;
                temp[2] = i;
                temp[3] = j;
                cout << temp << endl;
            }
            
        }
    }
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
	this->tempos << "Iniciando Retas Médias: " << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
    vector<vector<Vec4d> > classes_total;
	//vector<Vec4d> init;
	Vec4d trans;
	
	vector<pair<double,double > > ans;
	double ta = 0, tb = 0;
	int j = 0;
	
    this->coef_med_retas.clear();

    //this->datalog << "Retas Med" << endl;
	for(size_t i = 0;i<pontos.size();i++){
	//this->datalog << "xi =  " << pontos[i][0] << "  yi =   " << pontos[i][1] << "  xf =  "  << pontos[i][2] << "  yf = " << pontos[i][3] << endl;
		if(((pontos[i+1][2] - pontos[i][2])>-30)&&((pontos[i+1][2] - pontos[i][2])<30)){
            if(pontos[i][2] - pontos[i][0]){
			int a = (pontos[i][3] - pontos[i][1])/(pontos[i][2] - pontos[i][0]);
			int b = (pontos[i][1] - a*pontos[i][0]);
            //cout << " a: " << a << " b: " << b << " dx: " << pontos[i][2] - pontos[i][0] << " dy: " << pontos[i][3] - pontos[i][1] << endl;
            ta += a;
            tb += b; 
            j++;
            //cout << " ta: " << ta << " tb: " << tb << " j: " << j << endl;
            }
		}
		else{
			Vec4d med;
			vector<Vec4d> classes;
			//this->datalog << "xi =  " << trans[0] << "  yi =  " << trans[1] << "  xf =  "  << trans[2] << " yf = " << trans[3] << endl;
			if(pontos[i][2] - pontos[i][0]){
            int a = (pontos[i][3] - pontos[i][1])/(pontos[i][2] - pontos[i][0]);
			int b = (pontos[i][1] - a*pontos[i][0]);
            
            ta += a;
            tb += b;
            j++;
            
            //cout << " ta: " << ta << " tb: " << tb << " j: " << j << endl;
            ans.push_back(make_pair((ta/j),(tb/j)));
            }
            //cout << "a: " << (ta/j) << " b: " << (tb/j) << endl;
            ta = 0;
            tb = 0; 
			j = 0;
		}
	}
	//this->datalog << "Tamanho:  " << classes_total.size() << endl;
	//ans = coefs2(classes_total);
	this->coef_med_retas = ans;

    this->tempos << "Finalizando Retas Médias: " << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
}

void Camera::findingCenters(Mat image){
    this->tempos << "Iniciando Finding Centers: " << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
    //this->datalog << "Finding Centers" << endl;
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
            mc[i] = Point2f(static_cast<float>(mu[i].m10/mu[i].m00), static_cast<float>(mu[i].m01/mu[i].m00)); 
		//this->datalog << "Moments" << endl;
        //this->datalog << mc[i] << endl;
        }
	}

	//imshow( "Centroides", drawing );
	
	/*for(int i = 0; i<mc.size(); i++){
	cout << mc[i] << endl;
	
	}*/
	
	sort(mc.begin(),mc.end(),cmpVec2x);
	
	//this->datalog << "Quantidade de Centroides   " << mc.size() << endl;
	
	while(i<mc.size()){
        if((mc[i].x == 0) && (mc[i].y == 0)){
            i++;
        }
        else{
            if(((mc[i+1].x - mc[i].x)<(1))&&((mc[i+1].y - mc[i].y)<(1))){
                this->points.push_back(mc[i]);
                //this->datalog << "if: " << points[j].x << "      " << points[j].y << endl;
                i = i + 2;
                j++;
                }
            else{
                this->points.push_back(mc[i]);
                //this->datalog << "else: " << points[j].x << "      " << points[j].y << endl;
                i++;
                j++;
            }
        }
	}
		
	//image.copyTo(this->point);
	for( size_t i = 0; i<j; i++ ){
		Scalar color = Scalar( 255, 0, 0);
        //this->datalog << this->points[i] << endl;
		circle(this->point, this->points[i], 4, color, -1, 8, 0 );
	}
	imshow("Centroides filtrados", this->point);
	//this->datalog << "Quantidade de Centroides Filtrados  " << points.size() << endl;
    this->tempos << "Finalizando Finding Centers: " << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
}

void Camera::ordinating(Mat image, double range){
    this->tempos << "Inicializando Ordinating: " << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
	//this->datalog << "Ordinating" << endl;
    
    int clss = 1;
	double theta = 1;
	vector<vector<Point2f> > pline;
    
    pline.clear();
    this->pline.clear();
    this->size_classes.clear();

	vector<Point2f> inicializacao;
	pline.push_back(inicializacao);

    if(this->points.size() != 0){
	    pline[0].push_back(this->points[0]);
    }
	
	for(int i = 1; i<this->points.size(); i++){
		//if(!(((this->points[i].x < (this->width)*0.02) && (this->points[i].y < (this->height/this->region)*0.35)) || ((this->points[i].x > (this->width)*0.98) && (this->points[i].y < (this->height/this->region)*0.35)))){
            if((this->points[i].x<=(this->points[i-1].x + (range*theta)))){ //&& (abs(this->points[i].y - (points[i-1].y)) < (this->height/this->region)*0.65)){ 
                pline[(pline.size()-1)].push_back(this->points[i]);
                //this->datalog << ((points[i].x)-(points[i-1].x)) << endl;
                //this->datalog << "...adicionando no if..." << points[i] << "...na classe..." << (pline.size()-1) << endl;
                clss ++;
                }
            else{
                vector<Point2f> temp;
                pline.push_back(temp);
                
                pline[(pline.size()-1)].push_back(this->points[i]);
                //cout << abs(((points[i].y)-(points[i-1].y))) << " " << (this->height/this->region)*0.65 << endl;
                //this->datalog << "...adicionando no else..." << points[i] << "...na classe..." << (pline.size()-1) << endl;
                this->size_classes.push_back(clss);
                //this->datalog << clss << endl;
                clss = 1;
            }
        //}
	}
    
	this->size_classes.push_back(clss);
    this->pline = pline;
	//this->datalog << "Quantidade de classes" << this->pline.size() << endl;
    this->tempos << "Finalizando Ordinating: " << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
}

void Camera::hough(Mat image, int threshold, double mn, double mx){
    this->tempos << "Iniciando Hough Tradicional: " << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
    //this->datalog << "Hough" << endl;
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
    //this->datalog << "Threshold: " << threshold << endl;
    HoughLines(image, lines, 1, CV_PI/180, threshold, 0, 0);
 
    for(int i = 0; i < lines.size(); i++){
        Vec2f auxc;
        Vec2f auxp1;
        Vec2f auxp2;
        Vec2f auxp3;
        Vec2f auxp4;
        
        if(sin(lines[i][1])){
            auxc[0] = -(cos(lines[i][1])/sin(lines[i][1])); //coef ang
            auxc[1] = lines[i][0]/sin(lines[i][1]); //coef lin
            //cout << (atan(auxc[0])*180)/CV_PI << endl;
            if((mn <= ((atan(auxc[0])*180)/CV_PI)) && (((atan(auxc[0])*180)/CV_PI) <= mx)){
                this->coefs.push_back(auxc);  
                auxp1[1] = this->frame_roi.rows;//y
                auxp1[0] = ((this->frame_roi.rows) - auxc[1])/(auxc[0]); //x
                //this->datalog << "primeiro" << auxp1 << endl;
                auxp2[1] = this->frame_roi.rows - h;//y
                auxp2[0] = ((this->frame_roi.rows - h) - auxc[1])/(auxc[0]); //x
                //this->datalog << "segundo" << auxp2 << endl;
                auxp3[1] = this->frame_roi.rows - 2*h;//y
                auxp3[0] = ((this->frame_roi.rows - 2*h) - auxc[1])/(auxc[0]); //x
                //this->datalog << "terceiro" << auxp3 << endl;
                auxp4[1] = this->frame_roi.rows - 3*h;//y
                auxp4[0] = ((this->frame_roi.rows - 3*h) - auxc[1])/(auxc[0]); //x
                //this->datalog << "quarto" << auxp4 << endl;
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

                //imshow("Points1",this->init_point);
                
            }
        }
    }
    this->tempos << "Finalizando Hough Tradicional: " << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
}

void Camera::houghP(Mat image, int nc, double lineLength, double maxGap, double mn, double mx){
    Mat houghResult;
    this->frame_roi.copyTo(houghResult);
    this->tempos << "Iniciando HoughP: " << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
    //this->datalog << "HoughP" << endl;
    //this->datalog << "NC: " << nc << " lineLength: " << lineLength << " maxGap: " << maxGap << endl;
    vector<Vec4d> lines;
    lines.clear();
    this->linesP.clear(); 
    HoughLinesP(image, lines, 1, CV_PI/180, nc, lineLength, maxGap); // 100, 100, 100); //32, 65, 100
    for(int i = 0; i < lines.size(); i++){	
        int ang = (atan((lines[i][3] - lines[i][1])/(lines[i][2] - lines[i][0]))*180)/CV_PI;
		if ((mn <= abs(ang)) && (abs(ang) <= mx)){
            Vec4d temp;
            temp[0] = lines[i][0];
            temp[1] = lines[i][1];
            temp[2] = lines[i][2];
            temp[3] = lines[i][3];
            line(houghResult,Point(lines[i][0],lines[i][1]),Point(lines[i][2],lines[i][3]),Scalar(30,125,66),2);
            this->linesP.push_back(temp);
        }
    }
    //imshow("Hough", houghResult);
    this->tempos << "Finalizando HoughP: " << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
}

void Camera::miniROIs(Mat image){
    this->tempos << "Iniciando mini ROIs: " << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
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
    
    
    /*imshow("1",this->miniROI[0]);
    imshow("2",this->miniROI[1]);
    imshow("3",this->miniROI[2]);
    imshow("4",this->miniROI[3]);
    waitKey(0);*/
    this->tempos << "Finalizando mini ROIs: " << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
}

void Camera::ROIsOfClusters(Mat image){
    this->tempos << "Iniciando ROIsOfClusters: " << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
    //this->datalog << "ROIsOfClusters" << endl;
    this->clusters.clear();
    int xinit;
    //this->datalog << this->pline.size() << endl;
    for(int i = 0; i < this->pline.size(); i++){
        if((pline[i].size()) > 50){
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
    this->tempos << "Finalizando ROIsOfClusters: " << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;    
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
    //this->tempos << "Iniciando MedianMatrix: " << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
    //this->datalog << "MedianMatrix" << endl;
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
    //this->datalog << (soma/size) << endl;
    //this->tempos << "Finalizando MedianMatrix: " << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
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
            //cout << (double)aux << " " << i << " " << j << endl;
            if((double)aux == 255){
                px++;
            }
        }
        cout << px << endl;
        graph.push_back(px);
        circle(g, Point((graph.size() - 1),(image.rows - px)), 3, Scalar(255,0,0),-1,8,0);
        imshow("GRAPH", g);
        waitKey(10);
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
    ////cout << "x = " << x << " y = " << y << " size = " << size << endl;
    result.x = x/size;
    result.y = y/size;

    return result;
}

void Camera::dynamicROI(Mat image){
    this->tempos << "Iniciando dynamicROI: " << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
    //this->datalog << "dynamicROI" << endl;
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
            
            
            //this->datalog << "Primeiro ROI:  " << this->points[i].x << "..." << this->points[i].y << endl;
            //this->datalog << "Segundo ROI:   " << this->points2[i].x << "..." << this->points2[i].y << endl;
            //this->datalog << "Terceiro ROI:  " << this->points3[i].x << "..." << this->points3[i].y << endl;
            //this->datalog << "Quarto ROI:    " << this->points4[i].x << "..." << this->points4[i].y << endl;
            
            
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
            
            
            imshow("Points",this->init_point);
            imshow("Primeira",this->image_roi1);
            imshow("Segunda",this->image_roi2);
            imshow("Terceira",this->image_roi3);
            imshow("Quarta",this->image_roi4);
            waitKey(0); 
           
            
       
   }
   this->tempos << "Finalizando dynamicROI: " << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
}

void Camera::MMQ(){
    this->tempos << "Iniciando MMQ 1: " << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
    //this->datalog << "MMQ 1" << endl;
    this->mmq.clear();
    
    for(int i = 0; i < this->pline.size(); i++){
        //this->datalog << pline[i] << endl;
        Vec2f aux;
        float sx = 0, sy = 0, xx = 0, xy = 0, n = pline[i].size();
        for(int j = 0; j < this->pline[i].size(); j++){
            sx += this->pline[i][j].x;
            sy += this->pline[i][j].y;
            xx += pow(this->pline[i][j].x,2); //*(this->pline[i][j].x);
            xy += (this->pline[i][j].x)*(this->pline[i][j].y);
        }
        aux[0] = ((n*xy) - (sx*sy))/((n*xx) - (sx*sx)); //((sy*xx) - (sx*xy))/((n*xx)-(sx*sx));//((n*xy) - ((sx)*(sy)))/((n*xx) - ((sx)*(sx)));//Coef Ang;
        aux[1] = ((sy*xx) - (sx*xy))/((n*xx)-(sx*sx));//((sy - (aux[0]*sx))/n); //Coef lin
        //this->datalog << aux << endl;
        this->mmq.push_back(aux);
    }
    this->tempos << "Finalizando MMQ 1: " << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
}

void Camera::MMQ(int np_min){
    this->tempos << "Iniciando MMQ 2: " << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
    //this->datalog << "MMQ 2" << endl;
    this->mmq.clear();
    //this->datalog << pline.size() << endl;
    //this->datalog << "np: " << np_min << endl;
    for(int i = 0; i < this->pline.size(); i++){
        //this->datalog << this->pline[i].size() << endl;
        if(this->pline[i].size() > np_min){
            Vec2f aux;
            float sx = 0, sy = 0, xx = 0, xy = 0, n = pline[i].size();
            for(int j = 0; j < this->pline[i].size(); j++){
                sx += this->pline[i][j].x;
                sy += this->pline[i][j].y;
                xx += (this->pline[i][j].x)*(this->pline[i][j].x);
                xy += (this->pline[i][j].x)*(this->pline[i][j].y);
            }

            aux[0] = ((sy*xx) - (sx*xy))/((n*xx)-(sx*sx)); //((sy*xx) - (sx*xy))/((n*xx)-(sx*sx));//((n*xy) - ((sx)*(sy)))/((n*xx) - ((sx)*(sx)));//Coef Ang;
            aux[1] = ((sy - (aux[0]*sx))/n); //((sy*xx) - (sx*xy))/((n*xx)-(sx*sx));//((sy - (aux[0]*sx))/n); //Coef lin
            //this->datalog << aux << endl;
            this->mmq.push_back(aux);
            
            /*if(abs(aux[0]) < 50){
                cout << aux << endl;
                this->mmq.push_back(aux);
            }*/
        }
    }
    //cout << "Quantidade de linhas: " << mmq.size() << endl;
    this->tempos << "Finalizando MMQ 2: " << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
}

void Camera::R(){
    this->tempos << "Iniciando R: " << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
    //this->datalog << "R" << endl;
    this->final_coef.clear();
    for(int i = 0; i < this->pline.size(); i++){
        double sum = 0;
        for(int j = 0; j < this->pline[i].size(); j++){
            int xp = (this->pline[i][j].y - this->mmq[i].y)/(this->mmq[i].x);
            sum += (this->pline[i][j].x - xp)*(this->pline[i][j].x - xp);
        }
        int result = sqrt(sum);
        //this->datalog << "R  " << result << endl;
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
    this->tempos << "Finalizando R: " << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
}

void Camera::R2(){
    this->tempos << "Iniciando R2: " << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
    //this->datalog << "R2" << endl;
    this->final_coef.clear();
    for(int i = 0; i < this->pline.size(); i++){
        double sum = 0;
        for(int j = 0; j < this->pline[i].size(); j++){
            int xp = (this->pline[i][j].y - this->mmq[i].y)/(this->mmq[i].x);
            sum += (this->pline[i][j].x - xp)*(this->pline[i][j].x - xp);
        }
        int result = sqrt(sum);
        //this->datalog << "R  " << result << endl;
        if(result < thrs){
            this->final_coef.push_back(mmq[i]);
        }
    }
    this->tempos << "Finalizando R2: " << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
}

void Camera::vanishing_point(vector<pair<double,double>> coef_retas){
    double x = 0, y = 0;
    vector<vector<Point2f> > pontos;
    vector<pair<double,double>> coef_retas_f;
    vector<pair<double,double>> coef_retas_w;
    
    pontos.clear();
    coef_retas_f.clear();
    coef_retas_w.clear();

    for(int i = 0; i < coef_retas.size(); i++){
        vector<Point2f> ponto;
        ponto.clear();
        for(int j = 0; j < coef_retas.size(); j++){
            if((j != i) && (coef_retas[i].first != coef_retas[j].first)){
                Point2f xy;
                xy.x = (coef_retas[i].second - coef_retas[j].second)/((-coef_retas[i].first) - (-coef_retas[j].first));
                xy.y = (((coef_retas[j].first)*(coef_retas[i].second)) + ((-coef_retas[i].first)*(coef_retas[j].second)))/((-coef_retas[i].first) - (-coef_retas[j].first));
                ponto.push_back(xy);
            }
        }
        pontos.push_back(ponto);
    }
    
    for(int i = 0; i < pontos.size(); i++){
        int check = 0;
        //cout << pontos[i] << endl;
        for(int j = 0; j < pontos[i].size(); j++){
            
            if((pontos[i][j].y < (this->height/3) && (pontos[i][j].x >= 0) && (pontos[i][j].x <= (this->width)))/* && (0 < pontos[i][j].x) && (pontos[i][j].x < this->width)*/){
                check++;
            }
        }
        //cout << check << endl;
        if(check != 0){
            coef_retas_f.push_back(coef_retas[i]);
            //cout << "Aceito " << coef_retas[i].first << endl;
        }
        else{
            coef_retas_w.push_back(coef_retas[i]);
            //cout << "Rejeitado " << coef_retas[i].first << endl;
        }
    }

    this->coef_retas = coef_retas_f;
    this->coef_retas_wrong = coef_retas_w;
}

void Camera::expanding_lines(vector<Point2f> coef_retas, double mn, double mx){
	this->tempos << "Iniciando Expanding Lines: " << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
    //this->datalog << "Expanding Lines" << endl;
    //this->datalog << "Minimo: " << mn << " Maximo: " << mx << endl;
    int xi = 0, yi = ((this->height) - ((this->height)/4)), xf = 0, yf = 0;
    int aux = 0;
    float x =0 , y = 0;
    bool find = false;
    bool flag = false;
	vector<Vec4f> lines;
    vector<Vec4d> lines_a;
    vector<Point2f> coef_retas_f;


	lines.clear();
    lines_a.clear();
    coef_retas_f.clear();
    this->lines.clear();
    this->lines_a.clear();

    /*for(int i = 0; i < coef_retas.size(); i++){
        for(int j = 0; j < coef_retas.size(); j++){
            if((j != i) && (coef_retas[i].x != coef_retas[j].x)){
                x = (coef_retas[i].y - coef_retas[j].y)/((-coef_retas[i].x) - (-coef_retas[j].x));
                y = (((coef_retas[j].x)*(coef_retas[i].y)) + ((-coef_retas[i].x)*(coef_retas[j].y)))/((-coef_retas[i].x) - (-coef_retas[j].x));
                if((x > 0) && (x < this->width) && (y > ((this->height) - ((this->height)/4))) && (y < (this->height))){
                    cout << "Reta " << (i +1) << " " << " Reta " << (j+1) << " x: " << x << " y: " << y << endl;
                    find = true;
                }
            }
        }
        if(!find){
            cout << "Sem interseccao Reta " << (i +1) << endl;
            coef_retas_f.push_back(coef_retas[i]);
        }
        else{
            find = false;
        }
    }*/
    

    coef_retas_f = coef_retas;
    //cout << "Numero de retas: " << coef_retas_f.size() << endl;
	for(int i = 0; i < coef_retas_f.size(); i++){	
		//this->datalog << "coefs: " << coef_retas_f[i] << endl;
        //cout << coef_retas_f[i].x << " " << coef_retas_f[i].y << endl;
		if((mn <= abs((atan(coef_retas_f[i].x)*180)/CV_PI)) && (abs((atan(coef_retas_f[i].x)*180)/CV_PI) <= mx)){
            xi = (yi - coef_retas_f[i].y)/coef_retas_f[i].x;
            yf = (this->height);
            xf = (yf - coef_retas_f[i].y)/coef_retas_f[i].x;
                    
            if(xf>this->width){
                xf = this->width;
                yf = (coef_retas_f[i].x)*xf + coef_retas_f[i].y;
            }
            else if(xf<0){
                xf = 0;
                yf = (coef_retas_f[i].x)*xf + coef_retas_f[i].y;
            }

                Vec4d x_ord_trans;
                x_ord_trans[0] = xi;
                x_ord_trans[1] = yi;
                x_ord_trans[2] = xf;
                x_ord_trans[3] = yf;
                //this->datalog << "x_ord_trans" << x_ord_trans << endl;
                lines.push_back(x_ord_trans);
                lines_a.push_back(x_ord_trans);
            
        }
        else{
            //cout << "Out of range" << endl;
        }
        
    }
    //cout << lines.size() << endl;
	this->lines_a = lines_a;
	this->lines = lines;
    this->tempos << "Finalizando Expanding Lines: " << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
}

void Camera::expanding_lines_a(vector<pair<double,double>> coef_retas, double mn, double mx){
	this->tempos << "Iniciando Expanding Lines A: " << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
    //this->datalog << "Expanding Lines A" << endl;
    //this->datalog << "Minimo: " << mn << " Maximo: " << mx << endl;
    double xi = 0, yi = ((this->height) - ((this->height)/(4))), xf = 0, yf = this->height;
    int aux = 0;
    float x =0 , y = 0;
    bool find = false;
    bool flag = false;
	vector<Vec4f> lines;
    vector<Vec4f> lines_f;
    vector<Vec4d> lines_a;
    vector<pair<double,double>> coef_retas_f;

	lines.clear();
    lines_a.clear();
    lines_f.clear();
    coef_retas_f.clear();
    this->lines.clear();
    this->lines_a.clear();
    /*for(int i = 0; i < coef_retas.size(); i++){
        for(int j = 0; j < coef_retas.size(); j++){
            if((j != i) && (coef_retas[i].first != coef_retas[j].first)){
                x = (coef_retas[i].second - coef_retas[j].second)/((-coef_retas[i].first) - (-coef_retas[j].first));
                y = (((coef_retas[j].first)*(coef_retas[i].second)) + ((-coef_retas[i].first)*(coef_retas[j].second)))/((-coef_retas[i].first) - (-coef_retas[j].first));
                if((x > 0) && (x < this->width) && (y > ((this->height) - ((this->height)/(this->region)))) && (y < this->height)){
                    //cout << "Reta " << (i +1) << " " << " Reta " << (j+1) << " x: " << x << " y: " << y << " height " << ((this->height) - ((this->height)/4)) << endl;
                    find = true;
                }
            }
        }
        if(!find){
            cout << "Sem interseccao Reta " << (i +1) << endl;
            coef_retas_f.push_back(coef_retas[i]);
        }
        else{
            find = false;
        }
    }*/

    coef_retas_f = coef_retas;
    //cout << "Numero de retas: " << coef_retas_f.size() << endl;
	for(int i = 0; i < coef_retas_f.size(); i++){	
        //cout << coef_retas_f[i].first << " " << coef_retas_f[i].second << endl;
		if ((mn <= abs((atan(coef_retas_f[i].first)*180)/CV_PI)) && (abs((atan(coef_retas_f[i].first)*180)/CV_PI) <= mx)){
        //this->datalog << "coefs - first: " << coef_retas_f[i].first << " second: " << coef_retas_f[i].second << endl;
        xi = (yi - coef_retas_f[i].second)/coef_retas_f[i].first;
		yf = (this->height);
		xf = (yf - coef_retas_f[i].second)/coef_retas_f[i].first;
				
            if(xf>this->width){
                xf = this->width;
                yf = (coef_retas_f[i].first)*xf + coef_retas_f[i].second;
            }
            else if(xf<0){
                xf = 0;
                yf = (coef_retas_f[i].first)*xf + coef_retas_f[i].second;
            }
            
            Vec4d x_ord_trans;
            x_ord_trans[0] = xi;
            x_ord_trans[1] = yi;
            x_ord_trans[2] = xf;
            x_ord_trans[3] = yf;
            //cout << "x_ord_trans" << x_ord_trans << endl;
            //this->datalog << "x_ord_trans" << x_ord_trans << endl;
            lines.push_back(x_ord_trans);
            lines_a.push_back(x_ord_trans);
        
        }
    }

    sort(lines.begin(), lines.end(), cmpVecxff);
    sort(lines_a.begin(), lines_a.end(), cmpVecxf);
    
    this->lines_a = lines_a;
	this->lines = lines;
    
    this->tempos << "Finalizando Expanding Lines A: " << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
}

void Camera::expanding_lines_c(vector<Point2f> coef_retas, double p, double n){
	this->tempos << "Iniciando Expanding Lines C: " << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
    //this->datalog << "Expanding Lines C" << endl;
    //this->datalog << "Positivo: " << p << " Negativo: " << n << endl;
    int xi = 0, yi = (this->height/2), xf = 0, yf = this->height;
    int aux = 0;
    float x =0 , y = 0;
    bool find = false;
    bool flag = false;
	vector<Vec4f> lines;
    vector<Point2f> coef_retas_f;

	lines.clear();
    coef_retas_f.clear();
    this->lines.clear();

    for(int i = 0; i < coef_retas.size(); i++){
        for(int j = 0; j < coef_retas.size(); j++){
            if((j != i) && (coef_retas[i].x != coef_retas[j].x)){
                x = (coef_retas[i].y - coef_retas[j].y)/((coef_retas[i].x) - (coef_retas[j].x));
                y = (((-coef_retas[j].x)*(coef_retas[i].y)) + ((coef_retas[i].x)*(coef_retas[j].y)))/((coef_retas[i].x) - (coef_retas[j].x));
                if((x < this->width)){
                    //cout << "Reta " << (i +1) << " " << " Reta " << (j+1) << " x: " << x << " y: " << y << endl;
                }
            }
        }
        if(!find){
            //cout << "Sem interseccao Reta " << (i +1) << endl;
            coef_retas_f.push_back(coef_retas[i]);
        }
        else{
            find = false;
        }
    }

	for(int i = 0; i < coef_retas_f.size(); i++){	
		if((pline[i].size()) > 50){
            if (!((coef_retas_f[i].x > n) && (coef_retas_f[i].x < p))){
                xi = (yi - coef_retas_f[i].y)/coef_retas_f[i].x;
                if(xi < this->pline[i][0].x){
                    xi = this->pline[i][0].x;
                    yi = xi*(coef_retas_f[i].x) + coef_retas_f[i].y;
                }
                if(xi > this->pline[i][pline[i].size() - 1].x){
                    xi = this->pline[i][pline[i].size() - 1].x;
                    yi = xi*(coef_retas_f[i].x) + coef_retas_f[i].y;
                }

                yf = (this->height/this->region);
                xf = (yf - coef_retas_f[i].y)/coef_retas_f[i].x;
                        
                if(xf>this->pline[i][pline[i].size() - 1].x){
                    xf = this->pline[i][pline[i].size() - 1].x;
                    yf = (coef_retas_f[i].x)*xf + coef_retas_f[i].y;
                }

                if(xf<this->pline[i][0].x){
                    xf = this->pline[i][0].x;
                    yf = (coef_retas_f[i].x)*xf + coef_retas_f[i].y;
                }

                
                    Vec4f x_ord_trans;
                    x_ord_trans[0] = xi;
                    x_ord_trans[1] = yi;
                    x_ord_trans[2] = xf;
                    x_ord_trans[3] = yf;
                    //this->datalog << "x_ord_trans" << x_ord_trans << endl;
                    lines.push_back(x_ord_trans);
            }
            else{
                Vec4f x_ord_trans;
                x_ord_trans[0] = (pline[i][0].x + pline[i][(pline[i].size() - 1)].x)/2.0;
                x_ord_trans[1] = 0;
                x_ord_trans[2] = x_ord_trans[0];
                x_ord_trans[3] = this->frame_roi.rows;
                //this->datalog << "x_ord_trans" << x_ord_trans << endl;
                lines.push_back(x_ord_trans);
            }  
        }
    }
	this->lines  = lines;
    this->tempos << "Finalizando Expanding Lines C: " << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
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

void Camera::nozzles(){
    this->frame_simulacao = Mat::zeros(Size(this->width, (this->height + 60)), CV_8UC3);
    this->frame_final.copyTo(frame_simulacao(Rect(0,0,this->width, this->height)));
    vector<bool> regions;


    regions.clear();
    for(int i = 0; i< this->nNozzles; i++){
        regions.push_back(false);
    }

    for(int i = 0; i < this->lines.size(); i++){
        for(int j = 1; j <= this->nNozzles; j++){
            if((lines[i][0] >= ((this->width)/(this->nNozzles))*(j-1)) && (lines[i][0] < ((this->width)/(this->nNozzles))*(j))){
                regions[j-1] = true;
            }
        }
    }

    for(int i = 0; i< this->nNozzles; i++){
        if(regions[i] == true){
            int x = (((this->width)/(this->nNozzles))*(i))+(((this->width)/(this->nNozzles))*0.5);
            int y = ((this->height) + 30);
            circle(this->frame_simulacao, Point(x,y), 20, Scalar(0, 255, 0), -1);
        }
        else{
            int x = (((this->width)/(this->nNozzles))*(i))+(((this->width)/(this->nNozzles))*0.5);
            int y = ((this->height) + 30);
            circle(this->frame_simulacao, Point(x,y), 20, Scalar(0, 0, 255), -1);
        }
    }    
}

void Camera::drawLines(){
    this->tempos << "Quantidade de linhas  " << this->lines.size() << endl;
    for(int i = 0; i < this->lines.size(); i++){
		line(this->frame_final, Point(this->lines[i][0], this->lines[i][1]), Point(this->lines[i][2], this->lines[i][3]), Scalar(255,0,0), 3, LINE_AA);
        //imshow("Resultado",this->frame_final);
        //waitKey(1000);
    }
    //imshow("Resultado",this->frame_final);
}

void Camera::drawLines_a(){
    this->tempos << "Quantidade de linhas  " << this->lines_a.size() << endl;
    for(int i = 0; i < this->lines_a.size(); i++){
		line(this->frame_final, Point(this->lines_a[i][0], this->lines_a[i][1]), Point(this->lines_a[i][2], this->lines_a[i][3]), Scalar(255,0,0), 7, LINE_AA);
	}
    //imshow("Resultado",this->frame_final);
}

void Camera::drawLines_c(){
    this->tempos << "Quantidade de linhas  " << this->flinesC.size() << endl;

    for(int i = 0; i < this->flinesC.size(); i++){
		line(this->frame_final, Point(this->flinesC[i][0], this->flinesC[i][1]), Point(this->flinesC[i][2], this->flinesC[i][3]), Scalar(255,0,0), 7, LINE_AA);
	}
    //imshow("Resultado",this->frame_final);
}

void Camera::drawLines_wrong(){
    this->tempos << "Quantidade de linhas  " << this->lines.size() << endl;
    for(int i = 0; i < this->lines.size(); i++){
		line(this->frame_final, Point(this->lines[i][0], this->lines[i][1]), Point(this->lines[i][2], this->lines[i][3]), Scalar(0,0,255), 3, LINE_AA);
        //imshow("Resultado",this->frame_final);
        //waitKey(1000);
    }
    //imshow("Resultado",this->frame_final);
}

Mat Camera::imfill(Mat image){
    vector<vector<Point>> contours;

    threshold(image,image,0,255,THRESH_BINARY+THRESH_OTSU);
    Mat bwImage = image.clone();
    
    findContours(bwImage, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    drawContours(bwImage, contours, -1, Scalar(255), CV_FILLED);

    return bwImage;
}

Mat Camera::strips(Mat strip, int number_pixel){
    int vet = 0;
    int limite = (number_pixel/4);
    int v[strip.cols];

    //Soja
    dilate(strip, strip, getStructuringElement(MORPH_RECT, Size(3,3)));
    //Cebola
    //dilate(strip, strip, getStructuringElement(MORPH_ELLIPSE, Size(3,3)));
    vector<vector<Point>> contours;
    threshold(strip,strip,0,255,THRESH_BINARY+THRESH_OTSU);
    Mat bwImage = strip.clone();
    findContours(bwImage, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    drawContours(bwImage, contours, -1, Scalar(255), CV_FILLED);
    int n1 = bwImage.cols;
    int m1 = bwImage.rows;
    
    for (int j = 0; j < n1; j++){
        for (int i = 0; i < m1; i++){
            if(bwImage.at<uchar>(i, j) == 0){
                vet += 1;
            }
        }   
        v[j] = vet;
        vet = 0;             
    }
    
    for (int i = 0; i < n1; i++){
        if (v[i] < limite){
            for (int j = 0; j < m1; ++j){
            bwImage.at<uchar>(j, i) = 255;   
            } 
        }
        if (v[i] >= limite){
            for (int j = 0; j < m1; ++j){
            bwImage.at<uchar>(j, i) = 0;   
            } 
        }
    }
    return bwImage;
}

Mat Camera::imquantize(Mat image, int thresh){
	Mat Result = Mat::zeros(image.rows, image.cols, CV_8UC1);
	for (int i = 0; i < image.rows; i++)
			for (int j = 0; j < image.cols; j++)
			{
				if (image.at<uchar>(i, j) <= thresh)
					Result.at<uchar>(i, j) = 0;
				if (image.at<uchar>(i, j) > thresh)
					Result.at<uchar>(i, j) = 255;
			}
	return Result;
}

Mat Camera::concatenate(Mat strip1, Mat strip2, Mat strip3, Mat strip4, Mat strip5, Mat strip6, Mat strip7, Mat strip8,
    Mat strip9, Mat strip10, Mat strip11, Mat strip12, Mat strip13, Mat strip14, Mat strip15, Mat strip16, Mat strip17,
    Mat strip18, Mat strip19, Mat strip20){

    Mat H;

    vconcat(strip1, strip2, H); vconcat(H, strip3, H); vconcat(H, strip4, H); vconcat(H, strip5, H);
    vconcat(H, strip6, H); vconcat(H, strip7, H); vconcat(H, strip8, H); vconcat(H, strip9, H);
    vconcat(H, strip10, H); vconcat(H, strip11, H); vconcat(H, strip12, H); vconcat(H, strip13, H);
    vconcat(H, strip14, H); vconcat(H, strip15, H); vconcat(H, strip16, H); vconcat(H, strip17, H);
    vconcat(H, strip18, H); vconcat(H, strip19, H); vconcat(H, strip20, H);

    return H;

}