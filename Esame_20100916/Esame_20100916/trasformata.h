#ifndef TRASFORMATA_H
#define TRASFORMATA_H

#include "image.h"

#include <iostream>
#include <vector>

#define _USE_MATH_DEFINES
#include <math.h>
typedef unsigned char byte;

image<int> DCT(image<byte>& img, float livelloQuantizzazione){
	image<int> coeff(img.getWidth(), img.getHeight());
	
	//Scalo tutti i coefficenti di -128
	for (size_t y = 0; y < img.getHeight(); y++){
		for (size_t x = 0; x < img.getWidth(); x++){
			img(x, y) -= 128;
		}
	}

	int offset = 0;
	int blocchi_letti = 0;
	int count = 0;
	unsigned num_blocco = img.getWidth()*img.getHeight() / 64;
	for (size_t i = 0; i <num_blocco; i++){
		if (blocchi_letti == 64){
			offset = offset + 7;
			blocchi_letti = 0;
		}
		for (unsigned v = 0; v < 8; v++){
			for (unsigned u = 0; u < 8; u++){
				double Suv = 0;
				double Cu = (u == 0) ? 0.7071 : 1;
				double Cv = (v == 0) ? 0.7071 : 1;
				for (unsigned y = 0; y < 8; y++){
					for (unsigned x = 0; x < 8; x++){
						Suv += img(x + i * 8, y + offset)*cos(((2 * x + 1)*u*M_PI) / 16.0)*cos(((2 * y + 1)*v*M_PI) / 16.0);

					}
				}

				Suv = 0.25*Cu*Cv*Suv;
				//Suv -= 128;
				coeff(u + i * 8, v + offset) = (int)Suv;
				//std::cout << '\r' << count++;
			}
		}

		++blocchi_letti;
	}

	/*quantizzo*/
	for (unsigned y = 0; y < coeff.getHeight(); ++y){
		for (unsigned x = 0; x < coeff.getWidth(); ++x){
			coeff(x,y) /= livelloQuantizzazione;
		}
	}

	return coeff;
}

image<byte> IDCT(image<int>& img, float livelloQuantizzazione){
	image<byte> imgant(img.getWidth(), img.getHeight());

	for (unsigned y = 0; y < img.getHeight(); ++y){
		for (unsigned x = 0; x < img.getWidth(); ++x){
			img(x, y) *= livelloQuantizzazione;
		}
	}

	int offset = 0;
	int blocchi_letti = 0;
	int count = 0;
	unsigned num_blocco = img.getWidth()*img.getHeight() / 64;
	for (size_t i = 0; i <num_blocco; i++){
		if (blocchi_letti == 64){
			offset = offset + 7;
			blocchi_letti = 0;
		}
		for (unsigned y = 0; y < 8; y++){
			for (unsigned x = 0; x < 8; x++){
				double Sxy = 0.0;

				for (unsigned v = 0; v < 8; v++){
					for (unsigned u = 0; u < 8; u++){
						double Cu = (u == 0) ? 0.7071 : 1;
						double Cv = (v == 0) ? 0.7071 : 1;
						Sxy += Cu*Cv*img(u + i * 8, v + offset)*cos(((2 * x + 1)*u*M_PI) / 16.0)*cos(((2 * y + 1)*v*M_PI) / 16.0);
					}
				}

				Sxy *= 0.25;
				//Sxy += 128;
				imgant(x + i * 8, y + offset) = (byte)Sxy;

				//std::cout << '\r' << count++;

			}

		}
		++blocchi_letti;
	}

	//Aggiungo 128 a tutti i coefficenti
	for (size_t y = 0; y < imgant.getHeight(); y++){
		for (size_t x = 0; x < imgant.getWidth(); x++){
			imgant(x, y) += 128;
		}
	}

	return imgant;
}

#endif // TRASFORMATADCT_H