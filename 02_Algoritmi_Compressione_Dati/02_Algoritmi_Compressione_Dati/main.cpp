#include <fstream>
#include <iostream>

#include "support.h"
#include "lz77.h"
#include "lz78.h"
#include "lzw.h"
#include "packbits.h"


using namespace std;


class bitreader {
	// La classe ha il compito di scrivere gli n bit (n<8) 
	//meno significativi di ogni byte letto dal file specificato.


	istream& _is;
	byte _buffRem;		//Parte rimasta del byte utilizzato precedentemente
	byte _nBuffRem;	//Lunghezza buffRem
	unsigned int _data;
	int _bitsData;

	// Aggiunge il bit meno significativo di u al buffer
	void read_bit(unsigned int u, int c) {
		_data = (_data << 1) | (u & 1);
		if (++_bitsData == c) {
			unsigned int temp = 0;
			for (unsigned int i = 0; i < _nBuffRem; ++i)
				temp = (temp << 1) | 1;

			_buffRem = _buffRem & ((byte)temp);
		}
	}

	// Non si deve né copiare né assegnare!
	bitreader(const bitreader&);
	bitreader& operator= (const bitreader&);

public:
	bitreader(istream& is) : _is(is), _bitsData(0), _buffRem(0), 
		_nBuffRem(0), _data(0){}


	//
	void read(int count) {
		char c;

		if (count <= _nBuffRem){
			_data = 0;
			_bitsData = 0;
			int decrease = count;
			while (decrease-- > 0)
				read_bit(_buffRem >> (--_nBuffRem), count);
		}
		else{
			_data = _buffRem;
			_bitsData = _nBuffRem;
			flush();

			while ((count - _bitsData - 8) >= 0){
				_is.get(c);
				//cout << "Sono elias, sto per fare: " << c <<endl;
				_data = (_data << 8) + ((byte)c);
				_bitsData += 8;
				flush();
			}

			if (count > _bitsData){
				int t = count - _bitsData;
				_nBuffRem = 8;
				_is.get(c);
				_buffRem = (byte)c;
				while (t-- > 0)
					read_bit(_buffRem >> --_nBuffRem, count);
			}
		}

	}

	unsigned int getData(){ return _data; }
	unsigned int getBitsData(){ return _bitsData; }

	void flush(){ _buffRem = 0; _nBuffRem = 0; }

};


char LeggiConElias(ifstream& is){
	char ch;
	unsigned int count = 0;
	bitreader br(is);

	bool trovato = false;
	unsigned char ris = 1;

	unsigned int nzeri = 0;
	while (!trovato){
		br.read(1);
		if (br.getData() == 0)++nzeri;
		else {
			trovato = true;
			br.read(nzeri);
			ris = (ris << nzeri) + (byte)br.getData();
		}
	}

	return ch;
}








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

	if (_lzw){

		lzw l;
		vector<int> compressed;
		l.compress("aabaacabcabcb", back_inserter(compressed));
		copy(compressed.begin(), compressed.end(), 
			ostream_iterator<int>(cout, ", "));
		cout << endl;
		string decompressed = l.decompress(compressed.begin(), 
			compressed.end());
		cout << decompressed << endl;
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