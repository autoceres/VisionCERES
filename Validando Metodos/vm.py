from pathlib import Path
import cv2 as cv
import glob 
import math

xi1, yi1, xf1, yf1, ca1 = [], [], [], [], []
xi2, yi2, xf2, yf2, ca2 = [], [], [], [], []
area = []

windowName = 'Imagem'
cv.namedWindow(windowName)

def areaPoligono1(X, Y, n):  
    area = 0.0 
    j = n - 1
    for i in range(0,n): 
        area += (X[j]+X[i]) * (Y[j]-Y[i]) 
        j = i  
    return int(abs(area/2.0)) 

def areaPoligono2(X, Y, n):  
    area = 0.0 
    j = n - 1
    for i in range(0,n): 
        area += (X[j]+X[i]) * (Y[j]-Y[i]) 
        j = i  
    return int(abs(area/2.0)) 

def limpaVar1():
        xi1.clear()
        yi1.clear()
        xf1.clear()
        yf1.clear()
        ca1.clear()

def limpaVar2():
        xi2.clear()
        yi2.clear()
        xf2.clear()
        yf2.clear()
        ca2.clear()

def main():
        path = glob.glob("Dataset/*.png")
        path_R = "Resultados/1.txt"
        result = open(path_R, 'w+')
        
        for i in range(1, len(path)+1):
          
                print('Imagem', i)
                image =  "Dataset/" + str(i) + ".png"
                img = cv.imread(image)
                if (i>=1 & i<=70):
                        a = 25
                if (i>70 & i<=171):
                        a = 18
                if (i>171 & i<=242):
                        a = 30
                if (i>242 & i<=577):
                        a = 25
                if (i>577 & i<=633):
                        a = 30


                path_M = "Automatico/" + str(i) + "_metodo3.txt" 

                path_A = "Manual/" + str(i) + ".txt"

                f1 = open(path_A, "r")
                f2 = open(path_M, "r")

                data1 = (f1.readlines())
                data2 = (f2.readlines())
                for line in data1:
                        l = line.split(" ")
                        xi1.append(int(float(l[0])))
                        yi1.append(int(float(l[1])))
                        xf1.append(int(float(l[2])))
                        yf1.append(int(float(l[3])))
                        ca1.append(float(l[4]))
                for line in data2:
                        l = line.split(" ")
                        xi2.append(int(float(l[0])))
                        yi2.append(int(float(l[1])))
                        xf2.append(int(float(l[2])))
                        yf2.append(int(float(l[3])))
                        ca2.append(float(l[4]))


                #print(len(xi1))
                p = 0
                for x in range(0, len(xi1)):
                        cv.line(img, (xi1[x], yi1[x]), (xf1[x], yf1[x]), (255,0,0), 2)
                        for j in range(0, len(xi2)):  
                                cv.line(img, (xi2[j], yi2[j]), (xf2[j], yf2[j]), (0,0,255), 2) 
                                if (((xf1[x]-a) < xf2[j] < (xf1[x]+a)) & ((xi1[x]-a) < xi2[j] < (xi1[x]+a))):
                                
                                        det = (xf2[j]-xi2[j])*(234) - (234)*(xf1[x]-xi1[x]) 
                                        distEucli = (abs(xf2[j]-xf1[x]))
                                        angRetas = round(math.degrees(math.atan(abs((ca1[x]-ca2[j])/(1+(ca1[x]*ca2[j]))))), 2)

                                        if (ca2[j] < 0):
                                                ang = round((math.degrees(math.atan(abs(ca2[j])))), 2)
                                                #print('Angulo ', ang, "\n")
                                        else:
                                                ang = round(180 - (math.degrees(math.atan(ca2[j]))), 2)
                                                #print('Angulo ', ang, "\n")

                                        if (det == 0.0):
                                                X1 = [xf1[x], xf2[j], xi1[x]]

                                                Y1 = [468, 468, 351]
                                                n1 = len(X1)
                                                X2 = [xi1[x], xi2[j], xf2[j]]
                                                Y2 = [351, 351, 468]

                                                n2 = len(X2)  
                                                areaTotal = int(areaPoligono1(X1, Y1, n1) + areaPoligono2(X2, Y2, n2))
                                                #print('Area', areaTotal, "\n")
                                                area.append(areaTotal)
                                        else:

                                                s = ((xf2[j]-xi2[j])*(0) - (351)*(xi2[j]-xi1[x]))/det
                                                t = ((xf1[x]-xi1[x])*(0) - (351)*(xi2[j]-xi1[x]))/det
                                                px = xi1[x] + (xf1[x]-xi1[x])*s
                                                py = 351 + (351)*s
                                                if (351 < py and py < 468) & (0 < px and px < 832):
                                                        X1 = [xi1[x], xi2[j], px]
                                                        Y1 = [351, 351, py]

                                                        n1 = len(X1) 
                                                        X2 = [xf1[x], xf2[j], px]
                                                        Y2 = [468, 468, py]
                                                        n2 = len(X2) 
                                                        areaTotal = int(areaPoligono1(X1, Y1, n1) + areaPoligono2(X2, Y2, n2))
                                                        #print('Area', areaTotal, "\n")
                                                        area.append(areaTotal)
                                                        #print(px, py)
                                                else:
                                                        X1 = [xf1[x], xf2[j], xi1[x]]

                                                        Y1 = [468, 468, 351]
                                                        n1 = len(X1)
                                                        X2 = [xi1[x], xi2[j], xf2[j]]
                                                        Y2 = [351, 351, 468]

                                                        n2 = len(X2)
                                                        areaTotal = int(areaPoligono1(X1, Y1, n1) + areaPoligono2(X2, Y2, n2))
                                                        #print('Area', areaTotal, "\n")
                                                        area.append(areaTotal)

                                        	
                                        if (ang > 130) | (ang < 55) | (areaTotal > 5000) | (distEucli > 50) | (angRetas > 30):
                                        	p += 1


                                        txt = result.readlines()
                                        txt.append(str(i))
                                        txt.append(' ')
                                        txt.append(str(areaTotal))
                                        txt.append(' ')
                                        txt.append(str(angRetas))
                                        txt.append(' ')
                                        txt.append(str(ang))
                                        txt.append(' ')
                                        txt.append(str(distEucli))
                                        txt.append("\n")
                                        result.writelines(txt)


                #print(len(area)-p)
                acc = round(((len(area)-p)/len(xi1))*100, 2)
                if (acc > 100.0):
                        acc = 100.0      
                if (acc == 0.0):
                	result.write(str(i) + ' 0 0 0 0 \n') 
                	        
                #print(acc)
                	        

                result.write('Total 0 0 0 0 ' + str(acc) + '\n')

                area.clear()
                limpaVar1()
                limpaVar2()

                #cv.imshow('Imagem', img)
                #cv.waitKey(0)

                #if cv.waitKey(0) == ord('q'):
                #        break

        #cv.destroyAllWindows()

if __name__ == '__main__':
	main()


