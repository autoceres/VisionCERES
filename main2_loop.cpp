/*Autores:
    Anniely Soares Lemos 
    Gustavo Henrique do Nascimento
*/

#include "linhas.h"

int main(int argc, char *argv[]){
    for(int i = 1; i <= stoi(argv[1]); i++){
        if(!((i == 12) || (i==32) || (i==239))){
            string pref = "_metodo3_saida.png";
            string path = "imagens/";
            string name;
            string name_arq;
            Camera cam(0,4);
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
            //imshow("Dilation", cam.dilation);
            //cam.prevision(cam.binarized);
            cam.findingCenters(cam.morph);
            cam.ordinating(cam.morph);
            cam.MMQ(7);
            //cam.R2();
            cam.expanding_lines(cam.mmq);
            cam.drawLines();
            name_arq += to_string(i);
            name_arq += "_metodo3";
            name += path;
            name += to_string(i);
            name += pref;
            //imshow("Final", cam.frame_final);
            cam.writingFile(name_arq);
            imwrite(name,cam.frame_final);
            cam.cap.release();
        }
    }
    cout << "Done!" << endl;
    destroyAllWindows();
	return 0;
}