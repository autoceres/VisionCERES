/*Autores:
    Anniely Soares Lemos 
    Gustavo Henrique do Nascimento
*/

#include "linhas.h"

int main(int argc, char *argv[]){
    string pref = "_metodo2_saida.png";
    string path = "Imagens/";
    string name;
    string name_arq;
    Camera cam(0,4);
    
    cam.img_fn += "Dataset/";
    cam.img_fn += argv[1];
    cam.img_fn += cam.img_ext;
    
    name_arq += argv[1];
    name_arq += "_metodo2";
    name += path;
    name += argv[1];
    name += pref;

    cam.name += argv[1];
    
    cam.dataLog(name_arq, "Metodo2");
    
    cam.frame = imread(cam.img_fn, IMREAD_COLOR);
    resize(cam.frame, cam.frame, Size(360,480));
    cam.gettingSize(cam.frame.cols,cam.frame.rows);
   
    cam.frame.copyTo(cam.frame_final);
    cam.creatingRoi(cam.frame);
    cam.Segmentation(cam.frame_roi);
    imshow("Seg2",cam.segmented);
    
    cam.erodeConfig(2, 2);
    cam.dilateConfig(1, 1);
    cam.skeletonConfig(3, 3);
    
    cam.morphOp(cam.segmented);
    //cam.morphologicalOperations(cam.segmented);

    imshow("E",cam.erosion);
    imshow("D",cam.dilation);
    imshow("Morfologica",cam.morph);
    //imshow("Bin", cam.binarized);
    
    cam.findingCenters(cam.morph);
    cam.ordinating(cam.morph, 15);
    cam.eigens(cam.pline);
    cam.eigenLines(cam.eigvect, cam.eigvals, cam.med);
    cam.vanishing_point(cam.coef_retas);
    cam.expanding_lines_a(cam.coef_retas, 60, 120);

    cam.drawLines();
    cam.nozzles();
    cam.expanding_lines_a(cam.coef_retas_wrong, 40, 135);
    cam.drawLines_wrong();
    imshow("Final", cam.frame_final);
    imshow("Simulacao", cam.frame_simulacao);
    
    cam.writingFile(name_arq);
    imwrite(name,cam.frame_final);
    
    waitKey(0);

    cam.tempos << ((double)clock()/CLOCKS_PER_SEC)*1000 << endl;
    cam.all << ((double)clock()/CLOCKS_PER_SEC)*1000 << endl;
    
    cam.datalog.close();
    cam.tempos.close();
    cam.all.close();
    
    cam.cap.release();
    destroyAllWindows();
    
	return 0;
}