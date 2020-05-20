/*Autores:
    Anniely Soares Lemos 
    Gustavo Henrique do Nascimento
    //Fixing errors
*/

#include "linhas.h"

int main(int argc, char *argv[]){

    Camera cam(0,2);
    cam.cap.open("3.mp4");
    cam.namingVideo(argv[1]);
    cam.gettingSize(cam.cap.get(CAP_PROP_FRAME_WIDTH),cam.cap.get(CAP_PROP_FRAME_HEIGHT));
    
    while (1){
        cam.cap >> cam.frame;

        if(cam.frame.empty()){
            break;
        }

        cam.frame.copyTo(cam.frame_final);
        imshow("Original",cam.frame);
        cam.creatingRoi(cam.frame);
        imshow("Crop",cam.frame_roi);
        cam.Segmentation(cam.frame_roi);
        imshow("Segmentation",cam.segmented);
        cam.morphologicalOperations(cam.segmented);
        imshow("Morfologica",cam.skeleton);
        cam.houghP(cam.skeleton);
        cam.coefs1(cam.linesP);
        cam.expanding_lines_a(cam.coef_retas);

        sort(cam.lines_a.begin(), cam.lines_a.end(), cmpVecxf);

        cam.retas_med(cam.lines_a);
        cam.expanding_lines_a(cam.coef_med_retas);
        cam.drawLines_a();

        char c = (char)waitKey(100);
			if( c == 27 ){
				break;
			}	
    }
    
    cam.cap.release();
    destroyAllWindows();

	return 0;
}
        



