import numpy as np
import cv2 as cv
import glob

windowName = 'Imagem'
cv.namedWindow(windowName)
Lista_init_temp = []
Lista_final_temp = []
Lista_init = []
Lista_final = []
Lista_coef = []

def getting_points(event, x, y, flags, param):
	if event == cv.EVENT_LBUTTONDOWN:
		Lista_init_temp.append((x,y))

	if event == cv.EVENT_LBUTTONUP:
		Lista_final_temp.append((x, y))

cv.setMouseCallback(windowName, getting_points)

def draw_lines(img):
	for i in range(0,len(Lista_coef)):
		yi = 234
		xi = int((yi - Lista_coef[i][1])/Lista_coef[i][0]) 
		
		if (Lista_coef[i][0] >= 0) and (int((468 - Lista_coef[i][1])/Lista_coef[i][0]) > 831):
			xf = 831
			yf = int(xf*Lista_coef[i][0] + Lista_coef[i][1])
		elif (Lista_coef[i][0] < 0) and (int((468 - Lista_coef[i][1])/Lista_coef[i][0]) < 0):
			xf = 0
			yf = int(xf*Lista_coef[i][0] + Lista_coef[i][1])
		else:
			yf = 468
			xf = int((yf - Lista_coef[i][1])/Lista_coef[i][0])
		
		Lista_init.append((xi,yi))
		Lista_final.append((xf,yf))
		cv.line(img, (xi,yi), (xf,yf), (255,0,0), 3)

	return img

def calc_coefs():
	for i in range(0,len(Lista_init_temp)):
		ca = (Lista_final_temp[i][1] - Lista_init_temp[i][1])/(Lista_final_temp[i][0] - Lista_init_temp[i][0])	#ca
		cl = Lista_init_temp[i][1] - ((Lista_init_temp[i][0])*ca) #cl
		Lista_coef.append((ca,cl))

def cleaningVar():
	Lista_init_temp.clear()
	Lista_final_temp.clear()
	Lista_init.clear()
	Lista_final.clear()
	Lista_coef.clear()
	
	
def main():
	#Comeca a iteração do diretorio
	backup = open("backup.txt", 'r')
	cont = int(backup.read())
	backup.close()
	path = glob.glob("Dataset/*.png") 
	print(len(path))
	for i in range(cont,len(path)+1): 
		print(i)
		p = "Dataset/" + str(i) + ".png"
		img = cv.imread(p)
		if (img.shape[0] == 2340) and (img.shape[1] == 4160):
			resized = cv.resize(img, (int(img.shape[1]/5), int(img.shape[0]/5)))
		else:
			resized = cv.resize(img, (int(832), int(468)))
		
		backup = open("backup.txt", 'w')
		backup.write("%d" % cont)
		backup.close()

		path_img = "Imagens/" + str(cont) + ".png"
		path_arq = "Arquivos/" + str(cont) + ".txt"
		path_imgo = "Original/" + str(cont) + ".png"
		
		cont += 1

		cv.imwrite(path_imgo, resized)

		cv.namedWindow(windowName, cv.WINDOW_NORMAL)
		cv.imshow(windowName, resized)
		
		if cv.waitKey(0) == ord('s'):
			print("Saindo...")
			break

		
		calc_coefs()

		img_retas = draw_lines(resized)
		cv.imshow("Retas", img_retas)
		cv.imwrite(path_img, img_retas)  
		
			
		#Criar arquivo TXT
		arquivo = open(path_arq, 'w+')
		for i in range(0,len(Lista_init)):
			texto = arquivo.readlines()
			texto.append(str(Lista_init[i][0]))
			texto.append(' ')
			texto.append(str(Lista_init[i][1]))
			texto.append(' ')
			texto.append(str(Lista_final[i][0]))
			texto.append(' ')
			texto.append(str(Lista_final[i][1]))
			texto.append(' ')
			texto.append(str(Lista_coef[i][0]))
			texto.append(' ')
			texto.append(str(Lista_coef[i][1]))
			texto.append("\n")
			arquivo.writelines(texto)

		arquivo.close()
		cleaningVar()

	print("Done!")
	cv.destroyAllWindows()

if __name__ == '__main__':
	main()
