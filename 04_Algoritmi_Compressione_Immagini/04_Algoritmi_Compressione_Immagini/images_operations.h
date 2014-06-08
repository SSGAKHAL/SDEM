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

/*Colora un blocco di pixel dell'immagine data! Si da il pixel più in alto a 
sinistra e quello più in basso a destra*/
void coloraBlocco(uint startx, uint starty, uint endx, uint endy, uint colore, 
	image<byte>& img){

	uint npixelx = (endx - startx);
	uint npixely = (endy - starty);

	for (uint i = npixelx; i < npixelx; ++i){
		for (uint j = npixely; j < npixely; ++j){
			img(npixelx-j, npixely-i) = colore;
		}
	}
}

void coloraBlocchi(uint start, uint end, uint colore, image<byte>& img){

	uint nblocchi = (end-start);

	for (uint i = start; i < nblocchi; ++i){
			//coloraBlocco(i, i+10,colore,img);
	}

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





#endif