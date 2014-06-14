#ifndef DCT_H
#define DCT_H

#include "image.h"

int idct(image<byte>& img){

	for (uint y = 0; y < 8; ++y){
		for (uint x = 0; x < 8; ++x){

			double Sxy = 0.0;

			for (uint v = 0; v < 8; ++v){
				for (uint u = 0; u < 8; ++u){

					double Cu = (u == 0) ? 0.7071 : 1;
					double Cv = (v == 0) ? 0.7071 : 1;
					//v*8+u
					Sxy += Cu * Cv * img(u, v) * cos((2 * x + 1)*(u * M_PI) / 16) * cos((2 * y + 1)*(v * M_PI) / 16);
				}
			}
		}
	}

	return 1;
}

#endif