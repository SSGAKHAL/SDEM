#ifndef IMAGES_OPERATIONS_H
#define IMAGES_OPERATIONS_H

#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include "image.h"
#include "support.h"

template<typename T>

/*Rovescia una immagine di 180 gradi*/
void flip(image<T>& img) {
	for (unsigned y = 0; y < img.height() / 2; ++y) {
		for (unsigned x = 0; x < img.width(); ++x) {
			swap(img(x, y), img(x, img.height() - 1 - y));
		}
	}
}

/*Crea immagine specchiata orizzontalmente*/
template<typename T>
image<T> specchiaOrizzontale(image<T>& img){

	unsigned w = img.width();
	unsigned h = img.height();

	/*Creo un'immagine di copia identica*/
	image<T> tmp = img;

	/*E poi sovrascrivo le righe necessarie*/
	for (unsigned y = 0; y < h / 2; ++y){
		for (unsigned x = 0; x < w; ++x){
			tmp(x, h - y - 1) = img(x, y);
		}
	}

	return tmp;
}

/*Crea immagine specchiata orizzontalmente*/
template<typename T>
image<T> specchiaVerticale(image<T>& img){

	unsigned w = img.width();
	unsigned h = img.height();

	/*Creo un'immagine di copia identica*/
	image<T> tmp = img;

	/*E poi sovrascrivo le righe necessarie*/
	for (unsigned y = 0; y < h; ++y){
		for (unsigned x = 0; x < w / 2; ++x){
			tmp(w-x-1, y) = img(x, y);
		}
	}

	return tmp;
}

/*Operazione di saturazione, che ritorna un byte dato un double*/
byte saturate(double d) {
	int i = static_cast<int>(d + 0.5);	//intanto casto a + 0,5
	
	if (i<0)	//se sono sotto 0, torno 0
		return 0;
	else if (i>255)	//se sono sopra 255, torno 255
		return 255;
	else
		return i;	//se sono tra 0 e 255, torno quel valore
}

/*Crea immagine con solo bianco o solo nero*/
template<typename T>
image<T> blackANDWhite(image<T> &img, unsigned int media){

	image<T> tmp = img;

	for (unsigned y = 0; y < img.height(); ++y) {
		for (unsigned x = 0; x < img.width(); ++x) {
			if (tmp(x, y) < media){
				tmp(x, y) = 0;
			} else {
				tmp(x, y) = 255;
			}
		}
	}

	return tmp;
}


void cyclic_roll(int &a, int &b, int &c, int &d)
{
	int temp = a;
	a = b;
	b = c;
	c = d;
	d = temp;
}

template<typename T>
image<T> ruota90(image<T> &img){

	

	/*L'immagine risultante � lunga quanto alta prima!*/
	
//	for (int i = 0; i<n / 2; i++)
//		for (int j = 0; j<(n + 1) / 2; j++)
//			cyclic_roll(m[i][j], m[n - 1 - j][i], m[n - 1 - i][n - 1 - j], m[j][n - 1 - i]);

//	unsigned int r, c;
//	for (r = 0; r < tmp.height(); r++)
//	{
//		for (c = 0; c < tmp.width(); c++)
//		{
//			tmp(c*y + (y - r - 1)) = img(r*x + c);
//			//*(pD + c * row + (row - r - 1)) = *(pS + r * col + c);
//		}
//	}

	
	/*
	//double teta = M_PI*0.5;
	float teta = (90 * 3,1415) / 180.0;
	cout << "teta vale: " << teta << endl;
	double dovex = 0;
	double dovey = 0;
	byte pxl = 0;

	for (unsigned y = 0; y < img.height(); ++y) {
		for (unsigned x = 0; x < img.width(); ++x) {

			pxl = img(x, y);
			dovex = x*cos(teta) + y*sin(teta);
			dovey = x*(-sin(teta)) + y*cos(teta);

			//dovex = y;
			//dovey = -x + y;
			//if (!(dovey <= tmp.height() || dovex >= tmp.width()))
			tmp(x1,y1) = pxl;
			//tmp(dovex, dovey) = pxl;
		}
	}
	*/
	
	int teta = 90;
	const double PI = atan(1) * 4;

	int r0, c0;		//dove eseguire la rotazione, ovvero sul centro
	int r1, c1;		//dove mettere il pixel x,y
	int rows, cols;
	rows = img.width();
	cols = img.height();

	//float rads = (teta * 3.14159265) / 180.0;	//trasformo in radianti
	//double rads = M_PI*90/180.0;
	double rads = PI/2;
	//int rads = teta;

	r0 = rows / 2;
	c0 = cols / 2;

	image<T> tmp(img.height(), img.width());

	for (int y = 0; y < rows; ++y){
		for (int x = 0; x < cols; ++x){

			/*Se cambio i + o i - si gira dall'altra parte!*/
			r1 = static_cast<int>((r0 + ((y-r0) * cos(rads)) + ((x-c0) * -sin(rads))));
			c1 = static_cast<int>((c0 + ((y-r0) * sin(rads)) - ((x-c0) * cos(rads))));

			//r1 = static_cast<int>((r0 + ((y-r0) * 0) - ((x-c0)*1)));
			//c1 = static_cast<int>((c0 + ((y-r0) * 1) - ((x-c0)*0)));

			//r1 = x*cos(rads) + y*cos(rads);
			//c1 = -x*sin(rads) + y*cos(rads);

			if (tmp.isInBounds(r1, c1)){
				//cout << "sto per fare: " << r1 << "," << c1 << endl;
				tmp(r1, c1) = img(y, x);
			}
			else {
				cout << "!";
			}
		}
	}

	return tmp;

}

std::vector<image<byte>> separateRGBGrayScale(image<vec3b> img){

	unsigned w, h;
	w = img.width();
	h = img.height();

	std::cout << "sto per creare le 3 immagini" << std::endl;
	image<byte> r(w,h);
	image<byte> g(w,h);
	image<byte> b(w,h);

	for (unsigned y = 0; y < h; ++y){
		for (unsigned x = 0; x < w; ++x){
			r(x, y) = img(x, y)[0];
			g(x, y) = img(x, y)[1];
			b(x, y) = img(x, y)[2];
		}
	}

	std::vector<image<byte>> tmp;
	tmp.push_back(r);
	tmp.push_back(g);
	tmp.push_back(b);
	return tmp;
}

std::vector<image<vec3b>> separateRGB(image<vec3b> img){

	unsigned w, h;
	w = img.width();
	h = img.height();

	std::cout << "sto per creare le 3 immagini" << std::endl;
	image<vec3b> r(w, h);
	image<vec3b> g(w, h);
	image<vec3b> b(w, h);

	for (unsigned y = 0; y < h; ++y){
		for (unsigned x = 0; x < w; ++x){
			r(x, y)[0] = img(x, y)[0];	//metto nell'immagine r solo il rosso (il resto � gi� 0)
			g(x, y)[1] = img(x, y)[1];	//etc
			b(x, y)[2] = img(x, y)[2];
		}
	}

	std::vector<image<vec3b>> tmp;
	tmp.push_back(r);
	tmp.push_back(g);
	tmp.push_back(b);
	return tmp;
}

vec3b RGB2YCbCr(const vec3b& in) {

	double R = in[0];
	double G = in[1];
	double B = in[2];

	double Y = 16. + 1. / 256. * (65.738  * R + 129.057  * G + 25.064  * B);
	double Cb = 128. + 1. / 256. * (-37.945  * R - 74.494  * G + 112.439  * B);
	double Cr = 128. + 1. / 256. * (112.439  * R - 94.154  * G - 18.285  * B);

	return vec3b(saturate(Y), saturate(Cb), saturate(Cr));
}

vec3b YCbCr2RGB(const vec3b& in) {
	double Y = in[0];
	double Cb = in[1];
	double Cr = in[2];

	double R = (298.082 * Y + 408.583 * Cr) / 256. - 222.921;
	double G = (298.082 * Y - 100.291 * Cb - 208.120 * Cr) / 256. + 135.576;
	double B = (298.082 * Y + 516.412 * Cb) / 256. - 276.836;

	return vec3b(saturate(R), saturate(G), saturate(B));
}

image<byte> createBlackImage(uint w, uint h){
	image<byte> tmp(w, h);
	for (uint i = 0; i < tmp.height(); i++){	//per tutte le righe                 
		for (uint j = 0; j < tmp.width(); j++){	//per tutte le colonne
			tmp(i, j) = 0;						//nero = 0
		}
	}

	return tmp;
}

image<byte> createWhiteImage(uint w, uint h){
	image<byte> tmp(w, h);
	for (uint i = 0; i < tmp.height(); i++){	//per tutte le righe
		for (uint j = 0; j < tmp.width(); j++){	//per tutte le colonne
			tmp(i, j) = 255;					//bianco = 255
		}
	}

	return tmp;
}

image<byte> createGradientHorizontalBlackToWhite(uint w, uint h){
	image<byte> tmp(w, h);
	for (uint i = 0; i < tmp.height(); i++){	//per tutte le righe
		for (uint j = 0; j < tmp.width(); j++){	//per tutte le colonne
			tmp(i, j) = i;						
		}
	}

	return tmp;
}

image<byte> createGradientVerticalBlackToWhite(uint w, uint h){
	
	image<byte> tmp(w, h);
	for (uint i = 0; i < tmp.height(); i++){	//per tutte le righe
		for (uint j = 0; j < tmp.width(); j++){	//per tutte le colonne
			tmp(i, j) = j;
		}
	}

	return tmp;
}

image<byte> createGradientWhiteDiagonal(uint w, uint h){
	image<byte> tmp(w, h);
	for (uint i = 0; i < tmp.height(); i++){	//per tutte le righe
		for (uint j = 0; j < tmp.width(); j++){	//per tutte le colonne
			if (i>j)
				//triangolo sopra la diagonale
				tmp(i, j) = 255 + j - i;		//prendo il bianco (255) sommo il n. 
												//colonna e sottraggo il numero riga
			else
				//triangolo sotto la diagonale
				tmp(i, j) = 255 - j + i;
			
		}
	}

	return tmp;
}

image<byte> createFoglioCartaQuadrettata(uint w, uint h){

	image<byte> tmp(w, h);

	/*Devo fare 10 blocchi, se dimensione 250 avr� 10 blocchi da 25 pixel PER RIGA. 
	//Se il numero non � multiplo di 10...sticazzi*/
	uint dimBlocco = w / 10;

	for (uint i = 0; i < tmp.height(); i++){	//per tutte le righe
		for (uint j = 0; j < tmp.width(); ++j){	//per tutte le colonne
			if (i % dimBlocco > 0 && j % dimBlocco > 0)
				tmp(i, j) = 255;
			else
				tmp(i, j) = 0;
		}
	}

	return tmp;
}

/*Ritorna un carattere secondo lo standard JPEG
0	1
1	2
-1	3
2	4
-2	5
3	6
-3	7
...
*/
char mappatura(byte c){
	if (c == 1) return 0;
	else {
		if (c % 2 == 0)return c / 2;
		else return -(c / 2);
	}
}

/*Scrive un blocco 8x8 con il colore dato alle coordinate specificate.
Le coordinate indicano il pixel in alto a sinistra
*/
void coloraBlocco(image<byte>& img, unsigned x1, unsigned y1, byte colore){

	unsigned f = 0;
	unsigned y = 0;

	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++){
			img(x1 + j, y1 + i) = colore;
			f++;
		}
		y++;
	}
}

/*Fa dei blocchi w/10 bianchi e neri in sequenza*/
image<byte> createScacchiera(uint w, uint h){

	image<byte> tmp(w, h);

	/*Devo fare 10 blocchi, se dimensione 250 avr� 10 blocchi da 25 pixel PER RIGA.
	Se il numero non � multiplo di 10...sticazzi*/
	uint dimBlocco = w / 10;

	coloraBlocco(tmp, 10, 10,20);

	return tmp;
}




#endif