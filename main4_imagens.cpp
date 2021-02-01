/*Autores:
    Anniely Soares Lemos 
    Gustavo Henrique do Nascimento
*/

#include "linhas.h"

int main(int argc, char *argv[]){
    string pref = "_metodo4_saida.png";
    string path = "Imagens/";
    string name;
    string name_arq;
    Camera cam(0,4);
    
    cam.img_fn += "Dataset/";
    cam.img_fn += argv[1];
    cam.img_fn += cam.img_ext;
    
    name_arq += argv[1];
    name_arq += "_metodo4";
    name += path;
    name += argv[1];
    name += pref;

    cam.name += argv[1];
    
    cam.dataLog(name_arq, "Metodo4");
    
    cam.frame = imread(cam.img_fn, IMREAD_COLOR);
    resize(cam.frame, cam.frame, Size(360,480));
    cam.gettingSize(cam.frame.cols,cam.frame.rows);
    
    cam.frame.copyTo(cam.frame_final);
    cam.creatingRoi(cam.frame);
    
    cam.Segmentation2(cam.frame_roi);
    imshow("Seg2",cam.segmented);
    
    cam.Segmentation(cam.frame_roi);
    imshow("Seg",cam.segmented);
    
    cam.erodeConfig(2, 2);
    cam.dilateConfig(7, 7);
    cam.skeletonConfig(3, 3);

    cam.morphOp(cam.segmented);
    //cam.morphologicalOperations(cam.segmented);
    
    imshow("E",cam.erosion);
    imshow("D",cam.dilation);
    imshow("Bin", cam.binarized);
    cam.KMeans(cam.morph, 2, 8);

    cam.eigens(cam.pline);
    cam.eigenLines(cam.eigvect, cam.eigvals, cam.med);
    
    cam.expanding_lines_a(cam.coef_retas, 60, 120);
    cam.drawLines();
    
    cam.nozzles();

    /*for(int i = 0; i < cam.lines_a.size(); i++){
        int xinit = cam.lines_a[i][0];
        int yinit = 0;
        int xf = cam.lines_a[i][2];
        int ex = 4;
        
        if(cam.lines_a[i][2] < xinit){
            if((cam.lines_a[i][2] - 2) > 0){
                xinit = cam.lines_a[i][2] - 2;
                int xf = cam.lines_a[i][0];
            }
            else{
                xinit = cam.lines_a[i][2];
                int xf = cam.lines_a[i][0];
            } 
        }

        int dst = int(abs(cam.lines_a[i][0] - cam.lines_a[i][2]) + ex);

        if((xinit + dst) > cam.width){
            int aux = dst;
            dst = cam.width - xinit - 1; 
        }

        
        int alt = abs(cam.lines_a[i][1] - cam.lines_a[i][3]) - 1; 
        cout << "xi " << xinit << " yi " << yinit << " width " << dst << " height " << alt << endl;
        Rect ROI = Rect(xinit, yinit, dst ,alt);
	    Mat image_roi = cam.morph(ROI);
        
        imshow("Area da reta", image_roi);
        waitKey(0);

    }*/

    imshow("Final", cam.frame_final);
    imshow("Simulacao", cam.frame_simulacao);

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