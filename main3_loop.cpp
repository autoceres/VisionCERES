#include "linhas.h"

int main(int argc, char *argv[]){
    for(int i = 1; i <= stoi(argv[1]); i++){
        double init = ((double)clock()/CLOCKS_PER_SEC)*1000;
        string pref = "_metodo3_saida.png";
        string path = "Imagens/";
        string name;
        string name_arq;
        Camera cam(0,1);

        Mat b, g, r, image, RB, I2, I3, BW, H, strip;
        Mat splitChannels[3];
        int thresh = 0;

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
        split(cam.frame, splitChannels);

        add(0.884*splitChannels[2], 0.311*splitChannels[0], RB);
        subtract(1.262*splitChannels[1], RB, image);
        thresh = cv::sum(image)[0];
        thresh = (int)(thresh/(image.cols*image.rows));

        image.convertTo(I2, CV_8U);

        I3 = cam.imquantize(I2, thresh);
        BW = cam.imfill(I3);
        int a = (cam.frame.rows % 10);
        int number_strip = 20;
        int number_pixel = ((cam.frame.rows-a)/number_strip);
        int n = image.cols-1;
        
        Mat strip1 = BW(Rect(0,0,n,number_pixel));
        Mat strip2 = BW(Rect(0,number_pixel+1,n,number_pixel));
        Mat strip3 = BW(Rect(0,number_pixel*2+1,n,number_pixel));
        Mat strip4 = BW(Rect(0,number_pixel*3+1,n,number_pixel));
        Mat strip5 = BW(Rect(0,number_pixel*4+1,n,number_pixel));
        Mat strip6 = BW(Rect(0,number_pixel*5+1,n,number_pixel));
        Mat strip7 = BW(Rect(0,number_pixel*6+1,n,number_pixel));
        Mat strip8 = BW(Rect(0,number_pixel*7+1,n,number_pixel));
        Mat strip9 = BW(Rect(0,number_pixel*8+1,n,number_pixel));
        Mat strip10 = BW(Rect(0,number_pixel*9+1,n,number_pixel));
        Mat strip11 = BW(Rect(0,number_pixel*10+1,n,number_pixel));
        Mat strip12 = BW(Rect(0,number_pixel*11+1,n,number_pixel));
        Mat strip13 = BW(Rect(0,number_pixel*12+1,n,number_pixel));
        Mat strip14 = BW(Rect(0,number_pixel*13+1,n,number_pixel));
        Mat strip15 = BW(Rect(0,number_pixel*14+1,n,number_pixel));
        Mat strip16 = BW(Rect(0,number_pixel*15+1,n,number_pixel));
        Mat strip17 = BW(Rect(0,number_pixel*16+1,n,number_pixel));
        Mat strip18 = BW(Rect(0,number_pixel*17+1,n,number_pixel));
        Mat strip19 = BW(Rect(0,number_pixel*18+1,n,number_pixel));
        Mat strip20 = BW(Rect(0,image.rows-number_pixel-1,n,number_pixel));
        
        strip1 = cam.strips(strip1,number_pixel);
        strip2 = cam.strips(strip2,number_pixel);
        strip3 = cam.strips(strip3,number_pixel);
        strip4 = cam.strips(strip4,number_pixel);
        strip5 = cam.strips(strip5,number_pixel);
        strip6 = cam.strips(strip6,number_pixel);
        strip7 = cam.strips(strip7,number_pixel);
        strip8 = cam.strips(strip8,number_pixel);
        strip9 = cam.strips(strip9,number_pixel);
        strip10 = cam.strips(strip10,number_pixel);
        strip11 = cam.strips(strip11,number_pixel);
        strip12 = cam.strips(strip12,number_pixel);
        strip13 = cam.strips(strip13,number_pixel);
        strip14 = cam.strips(strip14,number_pixel);
        strip15 = cam.strips(strip15,number_pixel);
        strip16 = cam.strips(strip16,number_pixel);
        strip17 = cam.strips(strip17,number_pixel);
        strip18 = cam.strips(strip18,number_pixel);
        strip19 = cam.strips(strip19,number_pixel);
        strip20 = cam.strips(strip20,number_pixel);
        
        strip = cam.concatenate(strip1, strip2, strip3, strip4, strip5, strip6, strip7, strip8, strip9, strip10, 
        strip11, strip12, strip13, strip14, strip15, strip16, strip17, strip18, strip19, strip20);

        cam.houghP(strip, 180, ((strip.rows)*0.65), (strip.rows), 30, 135);
        cam.coefs1(cam.linesP);
        cam.vanishing_point(cam.coef_retas);
        cam.expanding_lines_a(cam.coef_retas, 30, 135);
        sort(cam.lines_a.begin(), cam.lines_a.end(), cmpVecxf);

        cam.retas_med(cam.lines_a);
        cam.expanding_lines_a(cam.coef_med_retas, 30, 135);
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