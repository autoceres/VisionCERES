import numpy as np
import cv2 as cv
import glob

windowName = 'Imagem'
cv.namedWindow(windowName)
Lista_init = []
Lista_final = []
Lista_coef = []

def getting_points(event, x, y, flags, param):
	if event == cv.EVENT_LBUTTONDOWN:
		Lista_init.append((x,y))

	if event == cv.EVENT_LBUTTONUP:
		Lista_final.append((x, y))

cv.setMouseCallback(windowName, getting_points)

def draw_lines(img):
	for i in range(0,len(Lista_init)):
		cv.line(img, Lista_init[i], Lista_final[i], (255,0,0), 3)

	return img

def calc_coefs():
	for i in range(0,len(Lista_init)):
		ca = (Lista_final[i][1] - Lista_init[i][1])/(Lista_final[i][0] - Lista_init[i][0])	#ca
		cl = Lista_init[i][1] - ((Lista_init[i][0])*ca) #cl
		Lista_coef.append((ca,cl))

def cleaningVar():
	Lista_init.clear()
	Lista_final.clear()
	Lista_coef.clear()
	
	
def main():
	#Comeca a iteração do diretorio
	backup = open("backup.txt", 'r')
	cont = int(backup.read())
	backup.close()
	path = glob.glob("Dataset/*.jpg") 
	print(len(path))
	for i in range(cont,len(path)+1): 
		print(i)
		p = "Dataset/" + str(i) + ".jpg"
		img = cv.imread(p)#nesse espaço tem que ter o nome da variavel da imagem 
		if (img.shape[0] == 2340) and (img.shape[1] == 4160):
			resized = cv.resize(img, (int(img.shape[1]/5), int(img.shape[0]/5)))
		else:
			resized = cv.resize(img, (int(832), int(468)))
		
		backup = open("backup.txt", 'w')
		backup.write("%d" % cont)
		backup.close()

		path_img = "imagens/" + str(cont) + ".jpg"
		path_arq = "arquivos/" + str(cont) + ".txt"
		path_imgo = "original/" + str(cont) + ".jpg"
		
		cont += 1

		cv.imwrite(path_imgo, resized)

		cv.namedWindow(windowName, cv.WINDOW_NORMAL)
		cv.imshow(windowName, resized)
		
		if cv.waitKey(0) == ord('s'):
			break 
		
		img_retas = draw_lines(resized)
		cv.imshow("Retas", img_retas)
		cv.imwrite(path_img, img_retas) #Colocar no primeiro parametro o caminho para salvar a imagem 
								#e o repectivo nome
		calc_coefs()
			
		#Criar arquivo TXT
		arquivo = open(path_arq, 'w+')
		for i in range(0,len(Lista_init)):
			texto = arquivo.readlines()
			texto.append('xi: ')
			texto.append(str(Lista_init[i][0]))
			texto.append(' yi: ')
			texto.append(str(Lista_init[i][1]))
			texto.append(' xf: ')
			texto.append(str(Lista_final[i][0]))
			texto.append(' yf: ')
			texto.append(str(Lista_final[i][1]))
			texto.append(' coeficiente Angular: ')
			texto.append(str(Lista_coef[i][0]))
			texto.append(' coeficiente Linear: ')
			texto.append(str(Lista_coef[i][1]))
			texto.append("\n")
			arquivo.writelines(texto)

		arquivo.close()
		cleaningVar()

	print("Terminou!!!")
	cv.destroyAllWindows()

if __name__ == '__main__':
	main()
