/*Autores:
    Anniely Soares Lemos 
    Gustavo Henrique do Nascimento
*/

#include "linhas.h"

int main(int argc, char *argv[]){
    string pref = "_metodo3_saida.png";
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

    cam.erodeConfig(1, 11);
    cam.dilateConfig(3, 3);
    cam.skeletonConfig(3, 3);

    cam.morphologicalOperations(cam.segmented);
    //imshow("Dilation", cam.dilation);
    //cam.prevision(cam.binarized);
    cam.findingCenters(cam.morph);
    cam.ordinating(cam.morph);
    cam.MMQ(7);
    //cam.R2();
    cam.expanding_lines(cam.mmq);
    cam.drawLines();
    name_arq += argv[1];
    name_arq += "_metodo3";
    name += path;
    name += argv[1];
    name += pref;
    //imshow("Final", cam.frame_final);
    cam.writingFile(name_arq);
    imwrite(name,cam.frame_final);
    cam.cap.release();
    //waitKey(0);
    destroyAllWindows();

	return 0;
}