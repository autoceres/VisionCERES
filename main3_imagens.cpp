/*Autores:
    Anniely Soares Lemos 
    Gustavo Henrique do Nascimento
*/

#include "linhas.h"

int main(int argc, char *argv[]){
    string pref = "_metedo4_sainda.png";
    string path = "imagens/";
    string name;
    string name_arq;
    Camera cam(0,4);
    cam.img_fn += argv[1];
    cam.img_fn += cam.img_ext;
    cam.frame = imread(cam.img_fn, IMREAD_COLOR);
    cam.gettingSize(cam.frame.cols,cam.frame.rows);
   
    cam.frame.copyTo(cam.frame_final);
   
    cam.creatingRoi(cam.frame);
    cam.Segmentation(cam.frame_roi);
    cam.morphologicalOperations(cam.segmented);
    cam.SegAndCluster(cam.skeleton,30);
    cam.estimated_lines();
    cam.drawLines();
    name_arq += argv[1];
    name_arq += "_metodo3";
    name += path;
    name += argv[1];
    name += pref;
    cam.writingFile(name_arq);
    imwrite(name,cam.frame_final);
    cam.cap.release();
    destroyAllWindows();

	return 0;
}