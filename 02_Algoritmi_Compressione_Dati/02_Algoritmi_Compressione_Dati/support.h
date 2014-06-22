#ifndef SUPPORT_H
#define SUPPORT_H

#include <vector>
#include <fstream>
#include <algorithm>	//std:sort

typedef unsigned char byte;

struct Coppia{

	unsigned occorrenze;
	short valore;

	Coppia(unsigned a, short b) : occorrenze(a), valore(b){};
};

//float entropia(std::vector<short>& sample){
//
//	/*Se fgaccio una sort. Volendo l'algoritmo si può migliorare e
//	interrompere la ricerca non appena ho valore diverso*/
//	std::sort(sample.begin(), sample.end());
//
//	float h = 0.0;
//
//	std::vector<Coppia> coppie;
//	for (size_t i = 0; i < sample.size(); ++i){
//
//		unsigned tot = 0;
//		short tmp = sample[i];
//
//		for (int j = i; j < sample.size(); ++j){
//			if (tmp == sample[j])
//				tot++;
//		}
//		i += tot - 1;
//
//		//cout << "per " << sample[i] << " ci sono " 
//		//<< tot << " occorrenze " << endl;
//		coppie.push_back(Coppia(tot, sample[i]));
//
//	}
//
//	/*Ho il vettore di valore-occorrenze*/
//	//for (auto a : coppie){
//	//	cout << "(" << a.valore << ", " << a.occorrenze << ")" << endl;
//	//}
//
//	/*Calolco entropia!*/
//	for (unsigned i = 0; i < coppie.size(); ++i){
//
//		float num = (coppie[i].occorrenze);
//		float den = sample.size();
//		float px = num / den;
//
//		h += px * log2(px);
//	}
//
//	h = -h;
//
//	/*Calcolo tutte le occorrenze*/
//	return h;
//}

#endif

