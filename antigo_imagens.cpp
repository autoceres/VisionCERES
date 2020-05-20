/*Autores:
    Anniely Soares Lemos 
    Gustavo Henrique do Nascimento
*/

#include "linhas.h"

int main(int argc, char *argv[]){
    string pref = "_metedo1_sainda.png";
    string path = "imagens/";
    string name;
    string name_arq;
    Camera cam(0,2);
    cam.img_fn += argv[1];
    cam.img_fn += cam.img_ext;
    cam.frame = imread(cam.img_fn, IMREAD_COLOR);
    cam.gettingSize(cam.frame.cols,cam.frame.rows);
   
    cam.frame.copyTo(cam.frame_final);
   
    cam.creatingRoi(cam.frame);
    cam.Segmentation(cam.frame_roi);
    cam.morphologicalOperations(cam.segmented);
    cam.houghP(cam.skeleton);
    cam.coefs1(cam.linesP);
    cam.expanding_lines_a(cam.coef_retas);

    sort(cam.lines_a.begin(), cam.lines_a.end(), cmpVecxf);

    cam.retas_med(cam.lines_a);
    cam.expanding_lines_a(cam.coef_med_retas);
    cam.drawLines_a();
    name_arq += argv[1];
    name_arq += "_metodo1"; 
    name += path;
    name += argv[1];
    name += pref;
    cam.writingFile(name_arq);
    imwrite(name,cam.frame_final);
    cam.cap.release();
    destroyAllWindows();

	return 0;
}