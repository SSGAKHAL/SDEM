#include <iostream>
#include <fstream>

#include "bitstreams.h"

using namespace std;

typedef unsigned char byte;


string to_binary(unsigned int code, unsigned int len) {
	string s;
	while (len-->0) {
		if ((code >> len) & 1)
			s += '1';
		else
			s += '0';
	}
	return s;
}

int main(){

	cout << "Ciao!";

	ifstream is("applause.wav", ios::binary);
	if (!is){ cerr << "Impossibile aprire file"; return -1;}

	ofstream prova("prova", ios::binary);

	bitreader br(is);
	unsigned int uint;
	unsigned int numeroDiBitDaLeggere = 8;
	char ch;

	/*Stampo i primi 10 byte*/
	for (int i = 0; i<10; i++){
		uint = br(numeroDiBitDaLeggere);
		prova << "--->" << to_binary(uint, numeroDiBitDaLeggere).data() << endl;
	}
}
