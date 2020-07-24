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
    Camera cam(0,2);
    
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
    cam.gettingSize(cam.frame.cols,cam.frame.rows);
   
    cam.frame.copyTo(cam.frame_final);
    cam.creatingRoi(cam.frame);
    cam.Segmentation(cam.frame_roi);
    
    //imshow("Seg",cam.segmented);

    cam.erodeConfig(7, 7);
    cam.dilateConfig(3, 3);
    cam.skeletonConfig(3, 3);
    
    cam.morphologicalOperations(cam.segmented);
    //cam.morphOp(cam.segmented);
    
    //imshow("E",cam.erosion);
    //imshow("D",cam.dilation);
    //imshow("Morfologica",cam.morph);
    //imshow("Bin", cam.binarized);

    cam.hough(cam.morph, 20);
    
    cam.miniROIs(cam.morph);
    cam.dynamicROI(cam.morph);
    cam.MMQ();
    cam.R();
    cam.expanding_lines(cam.final_coef, 3, -3);

    sort(cam.lines_a.begin(), cam.lines_a.end(), cmpVecxf);

    cam.retas_med(cam.lines_a);
    cam.expanding_lines_a(cam.coef_med_retas, 3, -3);
    cam.drawLines();
    
    //imshow("Final", cam.frame_final);
    
    cam.writingFile(name_arq);
    imwrite(name,cam.frame_final);
    
    //waitKey(0);

    cam.tempos << ((double)clock()/CLOCKS_PER_SEC)*1000 << endl;
    cam.all << ((double)clock()/CLOCKS_PER_SEC)*1000 << endl;
    
    cam.datalog.close();
    cam.tempos.close();
    cam.all.close();
    
    cam.cap.release();
    destroyAllWindows();
    
	return 0;
}
