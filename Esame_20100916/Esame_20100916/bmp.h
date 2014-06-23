#ifndef BMP_H
#define BMP_H

#include <fstream>
#include <iostream>
#include "image.h"

typedef unsigned char byte;



bool writeP5(const std::string& sFileName, image<byte>& img) {
	std::ofstream os(sFileName, std::ios::binary);

	// Header
	os << "P5\n";
	os << "#PGM creato nel corso di SdEM\n";
	os << img.getWidth() << " " << img.getHeight() << "\n";
	os << "255\n";

	// Dati
	os.write(img.data(), img.getDataSize());

	return true;
}

std::string getBytes(std::istream& is, unsigned n){
	
	char ch;
	std::string magic;
	/*I primi 4 byte sono il magic number*/
	for (unsigned i = 0; i < n; ++i){
		is.get(ch);
		magic += ch;
	}

	return magic;
}

std::vector<byte> readBytes(std::istream& is, unsigned n){

	std::vector<byte> tmp(n);
	char ch;
	
	for (unsigned i = 0; i < n; ++i){
		is.get(ch);
		tmp[i] = ch;
	}

	return tmp;
	

}

bool readBMP(const std::string& nomeFile, image<byte> img){
	
	std::ifstream is(nomeFile, std::ios::binary);
	if (!is){
		std::cerr << "Impossibile aprire file" << std::endl;
		return false;
	}

	/*Magic number*/
	std::cout << "formato file: " << getBytes(is,2).c_str() << std::endl;
	char ch;

	/*Dimensione file*/
	unsigned size;
	is.read(reinterpret_cast<char*>(&size), sizeof(unsigned));
	std::cout << "Dimensione file: " << size << std::endl;

	/*Magio 4 byte*/
	unsigned salta;
	is.read(reinterpret_cast<char*>(&salta), sizeof(unsigned));

	/*Offset da dove inizia l'array di pixel*/
	unsigned offset;
	is.read(reinterpret_cast<char*>(&offset), sizeof(unsigned));
	std::cout << "I pixel iniziano all'indirizzo " << offset << std::endl;

	unsigned dimensioneDIB;
	is.read(reinterpret_cast<char*>(&dimensioneDIB), sizeof(unsigned));
	std::cout << "Dimensione DIB " << dimensioneDIB << std::endl;

	unsigned w, h;
	is.read(reinterpret_cast<char*>(&w), sizeof(unsigned));
	is.read(reinterpret_cast<char*>(&h), sizeof(unsigned));
	std::cout << "Immagine " << w << "x" << h << std::endl;

	/*Saltiamo*/
	is.read(reinterpret_cast<char*>(&salta), 2);

	/*Bit per pixel*/
	unsigned bpp=0;
	is.read(reinterpret_cast<char*>(&bpp), 2);
	std::cout << "bit per pixel " << bpp << std::endl;

	/*Vado dove iniziano i pixel*/
	is.seekg(offset);

	img.resize(w, h);
	char ch2;
	for (unsigned y = 0; y < h; ++y){
		for (unsigned x = 0; x < w; ++x){
			is.get(ch2);
			img(x, y) = static_cast<byte>(ch2);
		}
	}

	img.flip(img);

	writeP5("vediamo", img);


}



#endif