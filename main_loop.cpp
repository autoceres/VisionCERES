/*Autores:
    Anniely Soares Lemos 
    Gustavo Henrique do Nascimento
    //Fixing errors
*/

#include "linhas.h"

int main(int argc, char *argv[]){
    for(int i = 1; i <= stoi(argv[1]); i++){
        string pref = "_metodo2_saida.jpg";
        string path = "imagens/";
        string name;
        string name_arq;
        Camera cam(0,2);
        cam.img_fn += to_string(i);
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
        cam.hough(cam.skeleton, 30);
        cam.miniROIs(cam.skeleton);
        cam.dynamicROI(cam.skeleton);
        cam.MMQ();
        cam.R();
        cam.expanding_lines(cam.final_coef);

        sort(cam.lines_a.begin(), cam.lines_a.end(), cmpVecxf);

        cam.retas_med(cam.lines_a);
        cam.expanding_lines_a(cam.coef_med_retas);
        cam.drawLines();
        name_arq += to_string(i);
        name_arq += "_metodo2"; 
        name += path;
        name += to_string(i);
        name += pref;
        //imshow("Final", cam.frame_final);
        cam.writingFile(name_arq);
        imwrite(name,cam.frame_final);
        cam.cap.release();
        //waitKey(0);
    }
    cout << "Done!" << endl;
    destroyAllWindows();

	return 0;
}
