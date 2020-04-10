/*Autores:
    Anniely Soares Lemos 
    Gustavo Henrique do Nascimento
*/

#include "linhas.h"

int main(int argc, char *argv[]){

    Camera cam(0,2);
    VideoCapture cap("3.mp4");
    cam.namingVideo(argv[1]);
    cam.gettingSize(cap.get(CAP_PROP_FRAME_WIDTH),cap.get(CAP_PROP_FRAME_HEIGHT));
    
    while (1){
        cap >> cam.frame;

        if(cam.frame.empty()){
            break;
        }

        imshow("Original",cam.frame);
        cam.creatingRoi(cam.frame);
        imshow("Crop",cam.frame_roi);
        cam.Segmentation(cam.frame_roi);
        imshow("Segmentation",cam.segmented);
        cam.morphologicalOperations(cam.segmented);
        imshow("Morfologica",cam.binarized);
        
        char c = (char)waitKey(100);
			if( c == 27 ){
				break;
			}	
    }
    
    cap.release();
    destroyAllWindows();

	return 0;
}
