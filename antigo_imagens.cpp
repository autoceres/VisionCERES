/*Autores:
    Anniely Soares Lemos 
    Gustavo Henrique do Nascimento
*/

#include "linhas.h"

int main(int argc, char *argv[]){
    string pref = "_metodo1.png";
    string path = "Imagens/";
    string name;
    string name_arq;
    Camera cam(0,1);
    
    cam.img_fn += "Dataset/";
    cam.img_fn += argv[1];
    cam.img_fn += cam.img_ext;
    
    name_arq += argv[1];
    name_arq += "_metodo1"; 
    name += path;
    name += argv[1];
    name += pref;

    cam.name += argv[1];
    
    cam.dataLog(name_arq, "Metodo1");
    
    cam.frame = imread(cam.img_fn, IMREAD_COLOR);
    resize(cam.frame, cam.frame, Size(360,480));
    cam.gettingSize(cam.frame.cols,cam.frame.rows);
    
    cam.frame.copyTo(cam.frame_final);
    cam.creatingRoi(cam.frame);

    imshow("original", cam.frame_roi); 
    
    //cam.limiarSeg(cam.frame_roi);
    //imshow("lSeg",cam.segmented);
    
    cam.Segmentation(cam.frame_roi);
    imshow("Seg2", cam.segmented);
    
    
    cam.erodeConfig(3, 3);
    cam.dilateConfig(5, 5);
    cam.skeletonConfig(3, 3);

    

    cam.morphOp(cam.segmented);
    //cam.morphologicalOperations(cam.segmented);

    //cam.medianCanny(cam.morph);

    imshow("E",cam.erosion);
    imshow("D",cam.dilation);
    imshow("Morfologica",cam.morph);
    imshow("Canny", cam.canny);
    //imshow("Median Canny", cam.mcanny);
    //waitKey(0);

    cam.houghP(cam.canny, 60, ((cam.height)*0.65),(cam.height), 60, 120); //50 - 60 
    cam.coefs1(cam.linesP);
    //cam.intersections(cam.coef_retas);
    cam.vanishing_point(cam.coef_retas);
    cam.expanding_lines_a(cam.coef_retas, 30, 135);
    sort(cam.lines_a.begin(), cam.lines_a.end(), cmpVecxf);

    cam.retas_med(cam.lines_a);
    //cam.vanishing_point(cam.coef_retas);
    cam.expanding_lines_a(cam.coef_med_retas, 30, 135);
    
    cam.drawLines();
    cam.expanding_lines_a(cam.coef_retas_wrong, 40, 135);

    cam.drawLines_wrong();
    imshow("Final", cam.frame_final);
    
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