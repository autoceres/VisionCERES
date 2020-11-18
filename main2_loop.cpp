/*Autores:
    Anniely Soares Lemos 
    Gustavo Henrique do Nascimento
*/

#include "linhas.h"

int main(int argc, char *argv[]){
    for(int i = 1; i <= stoi(argv[1]); i++){
            double init = ((double)clock()/CLOCKS_PER_SEC)*1000;
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
    
            cam.dataLog(name_arq, "Metodo3");

            cam.frame = imread(cam.img_fn, IMREAD_COLOR);
            resize(cam.frame, cam.frame, Size(360,480));
            cam.gettingSize(cam.frame.cols,cam.frame.rows);
        
            cam.frame.copyTo(cam.frame_final);
            cam.creatingRoi(cam.frame);
            
            cam.Segmentation(cam.frame_roi);
            //imshow("Segmentation", cam.segmented);
            
            cam.erodeConfig(3, 1);
            cam.dilateConfig(7, 7);
            cam.skeletonConfig(3, 3);
            
            cam.morphOp(cam.segmented);
            //cam.morphologicalOperations(cam.segmented);

            cam.findingCenters(cam.morph);
            cam.ordinating(cam.morph, 15);
            cam.eigens(cam.pline);
            cam.eigenLines(cam.eigvect, cam.eigvals, cam.med);
            cam.vanishing_point(cam.coef_retas);
            cam.expanding_lines_a(cam.coef_retas, 60, 120);
            cam.drawLines();

            cam.writingFile(name_arq);
            imwrite(name,cam.frame_final);
            
            cam.tempos << ((double)clock()/CLOCKS_PER_SEC)*1000 << endl;
            cam.all << (((double)clock()/CLOCKS_PER_SEC)*1000 - init) << endl;

            //cout << i << endl;
        
            cam.datalog.close();
            cam.tempos.close();
            cam.all.close();
        
            cam.cap.release();
        
    }
    cout << "Done! " << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
    destroyAllWindows();
	return 0;
}