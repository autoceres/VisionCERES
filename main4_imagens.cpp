/*Autores:
    Anniely Soares Lemos 
    Gustavo Henrique do Nascimento
*/

#include "linhas.h"

int main(int argc, char *argv[]){
    string pref = "_metodo4_saida.png";
    string path = "Imagens/";
    string name;
    string name_arq;
    Camera cam(0,4);
    
    cam.img_fn += "Dataset/";
    cam.img_fn += argv[1];
    cam.img_fn += cam.img_ext;
    
    name_arq += argv[1];
    name_arq += "_metodo4";
    name += path;
    name += argv[1];
    name += pref;

    cam.name += argv[1];
    
    cam.dataLog(name_arq);
    
    cam.frame = imread(cam.img_fn, IMREAD_COLOR);
    cam.gettingSize(cam.frame.cols,cam.frame.rows);
    
    cam.frame.copyTo(cam.frame_final);
    cam.creatingRoi(cam.frame);
    cam.Segmentation(cam.frame_roi);

    imshow("Seg",cam.segmented);

    cam.erodeConfig(9, 9);
    cam.dilateConfig(3, 3);
    cam.skeletonConfig(3, 3);

    cam.morphOp(cam.segmented);
    //cam.morphologicalOperations(cam.segmented);
    cam.SegAndCluster(cam.morph,10);

    imshow("E",cam.erosion);
    imshow("D",cam.dilation);
    imshow("Bin", cam.binarized);

    cam.verifingClusters(cam.pline);
    cam.ROIsOfClusters(cam.morph);
    cam.MMQ();
    cam.expanding_lines_c(cam.mmq, 0.87, -0.7);
    cam.drawLines();

    imshow("Final", cam.frame_final);
    
    cam.writingFile(name_arq);
    imwrite(name,cam.frame_final);
    
    waitKey(0);

    cam.cap.release();
    destroyAllWindows();
    cam.datalog.close();

	return 0;
}