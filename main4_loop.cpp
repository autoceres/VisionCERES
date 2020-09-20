/*Autores:
    Anniely Soares Lemos 
    Gustavo Henrique do Nascimento
*/

#include "linhas.h"

int main(int argc, char *argv[]){
    for(int i = 1; i <= stoi(argv[1]); i++){
            double init = ((double)clock()/CLOCKS_PER_SEC)*1000;
            string pref = "_metodo4_saida.png";
            string path = "Imagens/";
            string name;
            string name_arq;
            Camera cam(0,4);

            cam.img_fn += "Dataset/";
            cam.img_fn += to_string(i);
            cam.img_fn += cam.img_ext;
            
            name_arq += to_string(i);
            name_arq += "_metodo4";
            name += path;
            name += to_string(i);
            name += pref;

            cam.name += to_string(i);
        
            cam.dataLog(name_arq, "Metodo4");
            
            cam.frame = imread(cam.img_fn, IMREAD_COLOR);
            cam.gettingSize(cam.frame.cols,cam.frame.rows);
            
            cam.frame.copyTo(cam.frame_final);
            cam.creatingRoi(cam.frame);
            
            cam.Segmentation2(cam.frame_roi);
            //imshow("Seg2",cam.segmented);
            
            cam.erodeConfig(3, 3);
            cam.dilateConfig(5, 5);
            cam.skeletonConfig(3, 3);
            
            cam.morphOp(cam.segmented);
            //cam.morphologicalOperations(cam.segmented);
            
            //imshow("E",cam.erosion);
            //imshow("D",cam.dilation);
            //imshow("Bin", cam.binarized);
            //waitKey(1000);
            
            cam.KMeans(cam.morph, 10);

            cam.eigens(cam.pline);
            cam.eigenLines(cam.eigvect, cam.eigvals, cam.med);

            cam.expanding_lines_a(cam.coef_retas, 30, 135);
            cam.drawLines();

            //imshow("Final", cam.frame_final);
            
            cam.writingFile(name_arq);
            imwrite(name,cam.frame_final);
            
            cam.tempos << ((double)clock()/CLOCKS_PER_SEC)*1000 << endl;
            cam.all << (((double)clock()/CLOCKS_PER_SEC)*1000 - init) << endl;
            
            cam.datalog.close();
            cam.tempos.close();
            cam.all.close();
            
            cam.cap.release();      
        
            cout << i << endl;
        
    }
    cout << "Done! " << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
    destroyAllWindows();

	return 0;
}