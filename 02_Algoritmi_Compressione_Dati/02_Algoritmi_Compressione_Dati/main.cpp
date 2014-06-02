#include <fstream>
#include <iostream>

#include "lz77.h"
#include "packbits.h"

using namespace std;

int main(){
	
	/*3 tipi di algoritmi: LZ77, LZ78 e LZW*/
	bool _77 = false;
	bool _78 = false;
	bool _lzw = false;
	bool _pac = true;

	if (_77){
		//ifstream ifs("lz77_testo_originale_da_comprimere.txt");
		ifstream ifs("file_input\\prova");
		ofstream ofs("file_output\\prova_compresso");

		if (!ifs){ cerr << "Impossibile aprire file" << endl; return -1; }
		if (!ofs){ cerr << "Impossibile scrivere file" << endl; return -1; }

		lz77 lz;
		lz.encode(ifs, ofs);

		//ofs.close();
		//lz.decode(ifstream("lz77_compresso"),ofstream("prova"));
	}

	/************************************************************************/
	/************************************************************************/
	/* Packbits è un algoritmo Run Lenght. È fatto da coppie (num,car)      */
	/* dove num:															*/
	/*		1-127	= non compressi											*/
	/*		128		= EOF													*/
	/*		129-257	= compressi												*/
	/*																		*/
	/*	Es: b a c d a a a													*/
	/*		3 bacd 254 a 128												*/
	/*		3=4 caratteri "puri" -1;										*/
	/*		254=257-3 perchè ho 3 caratteri che si ripetono					*/
	/*																		*/
	/************************************************************************/
	/************************************************************************/
	if (_pac){
		//metto le graffe perchè così si aprono/chiudono correttamente tutti gli stream
		{
			ifstream is("file_input\\pack_file", ios::binary);
			if (!is) { cerr << "Impossibile aprire file input"; return -1; }
			ofstream os("file_output\\output.pkb", ios::binary);
			if (!os) { cerr << "Impossibile aprire file output"; return -1; }

			Packbits pack;
			pack.encode(is, os);
		}
		{
			ifstream is("file_output\\output.pkb", ios::binary);
			if (!is) { cerr << "Impossibile aprire file input"; return -1; }
			ofstream os("file_output\\pack_file", ios::binary);
			if (!os) { cerr << "Impossibile aprire file output"; return -1; }

			Packbits pack1;
			pack1.decode(is, os);
		}
	}
}