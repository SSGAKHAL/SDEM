#ifndef IMAGE_OPERATIONS_H
#define IMAGE_OPERATIONS_H

#include <algorithm>
using namespace std;

#include "image.h"

template<typename T>
void flip(image<T>& img) {
	for (unsigned y = 0; y < img.height() / 2; ++y) {
		for (unsigned x = 0; x < img.width(); ++x) {
			swap(img(x, y), img(x, img.height() - 1 - y));
		}
	}
}

template<typename T>
void rotateLeft(image<T>& img){
	//TODO
}

template<typename T>
void rotateRight(image<T>& img){

}

void schiarisci(){}
void scurisci(){}
void invertiColori(){}
void contaPixel(int colore);


#endif // IMAGE_OPERATIONS_H