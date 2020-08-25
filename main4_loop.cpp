/*Autores:
    Anniely Soares Lemos 
    Gustavo Henrique do Nascimento
*/

#include "linhas.h"

int main(int argc, char *argv[]){
    for(int i = 1; i <= stoi(argv[1]); i++){
        if(!((i == 92) || (i== 93) || (i== 94) || (i== 95) || (i== 96)
                       || (i== 97) || (i==110) || (i==114) || (i==132)
                       || (i==165) || (i==177) || (i==178) || (i==179)
                       || (i==182) || (i==183) || (i==184) || (i==185)
                       || (i==191) || (i==193))){
            //cout << i << endl;
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
            //cam.Segmentation(cam.frame_roi);
            cam.limiarSeg(cam.frame_roi);
            //imshow("Seg",cam.segmented);
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
        }
        else{
            string name;
            string name_arq;
            Camera cam(0,4);

            cam.img_fn += "Dataset/";
            cam.img_fn += to_string(i);
            cam.img_fn += cam.img_ext;
            
            name_arq += to_string(i);
            name_arq += "_metodo4";

            cam.dataLog(name_arq, "Metodo4");
            cam.all << 0 << endl;
        }
    }
    cout << "Done! " << ((double)clock()/CLOCKS_PER_SEC)*1000 << " ms" << endl;
    destroyAllWindows();

	return 0;
}