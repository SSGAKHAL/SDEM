#ifndef DCT_H
#define DCT_H

#define _USE_MATH_DEFINES
#include <math.h>

#include "image.h"
#include "support.h"

/*Prende un vectore di coefficienti, e crea un'immagine*/
image<byte> dct(std::vector<double>){

	image<byte> img;

	for (uint y = 0; y < 8; ++y){
		for (uint x = 0; x < 8; ++x){

		}
	}

	return img;
}


/*Ritorna un vector di double, con in ordine dal 0 al 63-esimo 
coefficiente del blocco relativo all'immagine data*/
std::vector<double> idct(image<byte>& img){

	std::vector<double> bloccoAnti;

	/*Per 8 y*/
	for (uint y = 0; y < 8; ++y){

		/*per 8 x*/
		for (uint x = 0; x < 8; ++x){

			/*calcolo il valore di Sxy come somma di valori v*u*/
			double Sxy = 0.0;

			/*Per 8 v*/
			for (uint v = 0; v < 8; ++v){

				/*Per 8 u*/
				for (uint u = 0; u < 8; ++u){

					/*Calcolo coeff Cu*/
					double Cu = (u == 0) ? 0.7071 : 1;

					/*Calcolo coeff Cv*/
					double Cv = (v == 0) ? 0.7071 : 1;

					/*Sommo a Sxy il valore di Cu*Cv * img(u,v) * cos...*/
					Sxy += Cu * Cv * img(u, v)
						* cos((2 * x + 1)*(u * M_PI) / 16)
						* cos((2 * y + 1)*(v * M_PI) / 16);
				}
			}

			bloccoAnti.push_back(Sxy*1.0/4.0);
		}
	}

	return bloccoAnti;
}

#endif