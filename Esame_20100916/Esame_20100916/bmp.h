#ifndef BMP_H
#define BMP_H

#include <fstream>
#include <iostream>
#include <sstream>
#include "image.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include "trasformata.h"

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

float calcolaEntropia(image<byte> img){

	std::ofstream os("occorrenze", std::ios::binary);

	std::vector<unsigned> occ(256);
	for (unsigned y = 0; y < img.getHeight(); ++y){
		for (unsigned x = 0; x < img.getWidth(); ++x){
				occ[img(x, y)]++;
		}
	}

	for (unsigned i = 0; i < occ.size(); ++i){
		os << i << "\t" << occ[i] << "\n";
	}

	float h=0;

	float den = img.getWidth()*img.getHeight();
	
	for (unsigned i = 0; i < occ.size(); ++i){
		
		if (occ[i] == 0)
			continue;

		unsigned num = occ[i];
				
		float px = num / den;

		h += px*log2f(px);
	}

	return -h;
}

std::vector<int> dct_blocco(image<byte> img, unsigned x1, unsigned y1){

	std::vector<int> tmp;

	/*Prendo tutti e */
	for (unsigned v = 0; v < 8; ++v){
		for (unsigned u = 0; u < 8; ++u){

			int Suv = 0;

			int Cu = (u == 0) ? 0.7071 : 1;
			int Cv = (v == 0) ? 0.7071 : 1;

			for (unsigned y = y1; y < 8; ++y){
				for (unsigned x = x1; x < 8; ++x){
					Suv += img(x, y)
						*cos((2 * x + 1 * u*M_PI) / 16.0)
						*cos((2 * y + 1 * v*M_PI) / 16.0);
				}
			}

			//y*_h + x

			Suv *= Cu*Cv;
			Suv -= 128;
			tmp.push_back(static_cast<int>(Suv)); //controlla
		}
	}

	std::cout << "Blocco dal pixel " << x1 << ", " << y1 << "Terminato" << std::endl;

	return tmp;
}

std::vector<int> dct(image<byte> img){

	std::vector<int> tmp;

	unsigned num_blocchi = img.getWidth()*img.getHeight() / 64;

	/*Per tutti i blocchi*/

		/*Faccio la trasformata del blocco 8x8 che inizia all'indice 8*/

	unsigned rigablocchi = img.getWidth() / 64;
	unsigned colblocchi = img.getHeight() / 64;

	for (unsigned y = 0; y < rigablocchi; ++y){
		for (unsigned x = 0; x < colblocchi; ++x){
			std::cout << "Faccio blocco " << x << ", " << y << std::endl;
			std::vector<int> tmp2 = dct_blocco(img, x*8, y*8);

			for (unsigned t = 0; t < tmp2.size(); ++t)
				tmp.push_back(tmp2[t]);
		}
	}



	std::cout << "terminato" << std::endl;
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

	//vediamo se va bene
	img.flip(img);
	writeP5("originale.pgm", img);

	std::cout << calcolaEntropia(img) << std::endl;

	

	//for (float x = 2; x < 10; ++x){

		image<int> trasformata = DCT(img, 16.0);
		image<byte> antitrasformata = IDCT(trasformata, 16.0);
		writeP5("finale.pgm", antitrasformata);

		//std::cout << std::pow(x, 2);

		//std::ostringstream  oss;
		//oss << "antitrasformata";
		//oss << std::pow(x, 2);

		//std::cout << "fine per " << nome.c_str() << std::endl;

		//writeP5(oss, antitrasformata);
	//}
}

#endif