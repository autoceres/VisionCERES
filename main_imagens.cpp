/*Autores:
    Anniely Soares Lemos 
    Gustavo Henrique do Nascimento
    //Fixing errors
*/

#include "linhas.h"

int main(int argc, char *argv[]){
    string pref = "_metedo2_sainda.png";
    string path = "imagens/";
    string name;
    Camera cam(0,2);
    cam.img_fn += argv[1];
    cam.img_fn += cam.img_ext;
    cam.frame = imread(cam.img_fn, IMREAD_COLOR);
    cam.gettingSize(cam.frame.cols,cam.frame.rows);
   
    cam.frame.copyTo(cam.frame_final);
   
    cam.creatingRoi(cam.frame);
    cam.Segmentation(cam.frame_roi);
    cam.morphologicalOperations(cam.segmented);
    cam.hough(cam.skeleton);
    cam.miniROIs(cam.skeleton);
    cam.dynamicROI(cam.skeleton);
    cam.MMQ();
    cam.R();
    cam.expanding_lines(cam.final_coef);
    cam.drawLines();
        
    name += path;
    name += argv[1];
    name += pref;
    cam.writingFile(argv[1]);
    imwrite(name,cam.frame_final);
    cam.cap.release();
    destroyAllWindows();

	return 0;
}
