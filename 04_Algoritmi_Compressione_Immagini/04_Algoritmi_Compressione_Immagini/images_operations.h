#ifndef IMAGES_OPERATIONS_H
#define IMAGES_OPERATIONS_H

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

	/*Devo fare 10 blocchi, se dimensione 250 avrò 10 blocchi da 25 pixel PER RIGA. 
	//Se il numero non è multiplo di 10...sticazzi*/
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

/*Fa dei blocchi w/10 bianchi e neri in sequenza*/
image<byte> createScacchiera(uint w, uint h){

	image<byte> tmp(w, h);

	/*Devo fare 10 blocchi, se dimensione 250 avrò 10 blocchi da 25 pixel PER RIGA. 
	Se il numero non è multiplo di 10...sticazzi*/
	uint dimBlocco = w / 10;

	coloraBlocco(20,20, 40,40, 100, tmp);
		
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
void colorablocco(image<byte>& img, unsigned x1, unsigned y1, byte colore){

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



#endif