#include <fstream>
#include <iostream>

#include "lz77.h"
#include "lz78.h"
#include "packbits.h"

using namespace std;

int main(){
	
	/*3 tipi di algoritmi: LZ77, LZ78 e LZW*/
	bool _77 = false;
	bool _78 = false;
	bool _lzw = false;
	bool _pac = true;

	/************************************************************************/
	/*							Codice di LZ77								*/
	/*			In output una terna (pos, lun, ch successivo)				*/
	/************************************************************************/
	if (_77){
		lz77_coder coder("input.bin");
		coder.codify_in_console();
		coder.codify_in_file("output.txt");
	}

	/************************************************************************/
	/*							Codice di LZ78								*/
	/*	Ha un dizionario, output djhfjkdhkjfhd								*/
	/************************************************************************/
	if (_78){

		fstream f("testo.bin", fstream::out);
		//aacaacabcabaaac
		if (f.fail())
			cout << "errore" << endl;
		f << "aabaacabcabcb";


		f.close();

		lz78 lz;
		lz.codificalz78("testo.bin", 255);
		lz.decodificalz78("coded.bin", 255);
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