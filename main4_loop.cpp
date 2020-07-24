/*Autores:
    Anniely Soares Lemos 
    Gustavo Henrique do Nascimento
*/

#include "linhas.h"

int main(int argc, char *argv[]){
    for(int i = 1; i <= stoi(argv[1]); i++){
        string pref = "_metodo4_saida.png";
        string path = "Imagens/";
        string name;
        string name_arq;
        Camera cam(0,3);

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
        cam.Segmentation(cam.frame_roi);

        cam.erodeConfig(9, 9);
        cam.dilateConfig(3, 3);
        cam.skeletonConfig(3, 3);
        
        cam.morphOp(cam.segmented);
        //cam.morphologicalOperations(cam.segmented);
        cam.SegAndCluster(cam.morph,15);
        
        cam.verifingClusters(cam.pline);
        cam.ROIsOfClusters(cam.morph);
        cam.MMQ();
        cam.expanding_lines_c(cam.mmq, 0.87, -0.7);
        cam.drawLines();

        //imshow("Final", cam.frame_final);
        
        cam.writingFile(name_arq);
        imwrite(name,cam.frame_final);
        
        cam.tempos << ((double)clock()/CLOCKS_PER_SEC)*1000 << endl;
        cam.all << ((double)clock()/CLOCKS_PER_SEC)*1000 << endl;
        
        cam.datalog.close();
        cam.tempos.close();
        cam.all.close();
        
        cam.cap.release();      
    }
    cout << "Done!" << endl;
    destroyAllWindows();

	return 0;
}