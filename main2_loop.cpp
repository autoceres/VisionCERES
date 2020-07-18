/*Autores:
    Anniely Soares Lemos 
    Gustavo Henrique do Nascimento
*/

#include "linhas.h"

int main(int argc, char *argv[]){
    for(int i = 1; i <= stoi(argv[1]); i++){
        if(!((i == 12) || (i==32) || (i==239))){
            string pref = "_metodo3_saida.png";
            string path = "Imagens/";
            string name;
            string name_arq;
            Camera cam(0,4);

            cam.img_fn += "Dataset/";
            cam.img_fn += to_string(i);
            cam.img_fn += cam.img_ext;
            
            name_arq += to_string(i);
            name_arq += "_metodo3";
            name += path;
            name += to_string(i);
            name += pref;

            cam.name += to_string(i);
    
            cam.dataLog(name_arq);

            cam.frame = imread(cam.img_fn, IMREAD_COLOR);
            cam.gettingSize(cam.frame.cols,cam.frame.rows);
        
            cam.frame.copyTo(cam.frame_final);        
            cam.creatingRoi(cam.frame);
            cam.Segmentation(cam.frame_roi);

            cam.erodeConfig(7, 7);
            cam.dilateConfig(3, 3);
            cam.skeletonConfig(3, 3);

            //cam.morphOp(cam.segmented);
            cam.morphologicalOperations(cam.segmented);

            cam.findingCenters(cam.morph);
            cam.ordinating(cam.morph, 20);
            cam.MMQ(2);
            //cam.R2();
            cam.expanding_lines(cam.mmq, 0.3, -0.3);
            cam.drawLines();
            
            //imshow("Final", cam.frame_final);
            
            cam.writingFile(name_arq);
            imwrite(name,cam.frame_final);
            
            cam.cap.release();
            cam.datalog.close();
        }
    }
    cout << "Done!" << endl;
    destroyAllWindows();
	return 0;
}