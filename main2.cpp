/*Autores:
    Anniely Soares Lemos 
    Gustavo Henrique do Nascimento
*/

#include "linhas.h"

int main(int argc, char *argv[]){

    Camera cam(0,4);
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
        cam.findingCenters(cam.morph);
        cam.ordinating(cam.morph);
        cam.MMQ(cam.np_min);
        //cam.R();
        cam.expanding_lines(cam.mmq);
        cam.drawLines();
        
        char c = (char)waitKey(0);
			if( c == 27 ){
				break;
			}	
    }
    
    cam.cap.release();
    destroyAllWindows();

	return 0;
}
