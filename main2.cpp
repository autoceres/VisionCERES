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
    
    cam.p.open(cam.processed, CV_FOURCC('M','J','P','G'), 20, Size(cam.width,cam.height));
    cam.c.open(cam.original, CV_FOURCC('M','J','P','G'), 20, Size(cam.width,cam.height));
    
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

        cam.erodeConfig(1, 11);
        cam.dilateConfig(3, 3);
        cam.skeletonConfig(3, 3);

        cam.morphologicalOperations(cam.segmented);
        imshow("Morfologica",cam.skeleton);
        cam.findingCenters(cam.morph);
        cam.ordinating(cam.morph, 20);
        cam.MMQ(7);
        //cam.R2();
        cam.expanding_lines(cam.mmq, 0.87, -0.7);
        cam.drawLines();
        imshow("Resultado",cam.frame_final);
        
        cam.p.write(cam.frame_final);
        cam.c.write(cam.frame);
        char c = (char)waitKey(100);
			if( c == 27 ){
				break;
			}	
    }
    
    cam.cap.release();
    cam.p.release();
    cam.c.release();
    destroyAllWindows();

	return 0;
}
