#include <iostream>

#include "lz77.h"

using namespace std;

lz77::lz77(){}

/*Codifica*/
void lz77::encode(istream& is, ostream& os){

	char ch;
	while (is.get(ch)){
		cout << ch;
	}

}

void lz77::decode(istream& is, ostream& os){

}