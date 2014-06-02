#ifndef IMAGE_OPS_H
#define IMAGE_OPS_H

#include <algorithm>
#include "image.h"


template<typename T>
/*Rovescia una immagine di 180 gradi*/
void flip(image<T>& img) {
	for (unsigned y = 0; y < img.height() / 2; ++y) {
		for (unsigned x = 0; x < img.width(); ++x) {
			swap(img(x, y), img(x, img.height() - 1 - y));
		}
	}
}

#endif