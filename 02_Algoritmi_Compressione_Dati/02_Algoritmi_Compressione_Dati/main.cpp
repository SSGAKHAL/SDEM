#include <fstream>
#include <iostream>

#include "lz77.h"

using namespace std;

int main(){
	
	/*3 tipi di algoritmi: LZ77, LZ78 e LZW*/
	//ifstream ifs("lz77_testo_originale_da_comprimere.txt");
	ifstream ifs("prova");
	ofstream ofs("prova_compresso");

	if (!ifs){ cerr << "Impossibile aprire file" << endl; return -1; }
	if (!ofs){ cerr << "Impossibile scrivere file" << endl; return -1; }

	lz77 lz;
	lz.encode(ifs,ofs);

	//ofs.close();
	//lz.decode(ifstream("lz77_compresso"),ofstream("prova"));

}