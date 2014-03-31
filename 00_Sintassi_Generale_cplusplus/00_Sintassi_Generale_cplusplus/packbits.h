#ifndef PACKBITS_H
#define PACKBITS_H

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <set>

using namespace std;

class array{
	
	/*Un vettore che fa da buffer per il programma*/
	vector<char> _prev;

	/*Metodi Privati*/

public:
	void encode(istream &is, ostream &os){

		/*Carattere corrente*/
		char curChar;

		/*Dichiaro un set di 3 valori: start, run e nonrun*/
		enum {start, run, nonrun} state = start;

	}
	


};

#endif