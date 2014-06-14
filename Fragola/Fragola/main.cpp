#include <fstream>
#include <iostream>
#include "image.h"
#include "pgm.h"


typedef unsigned char byte;

using namespace std;


/*Ritorna un carattere secondo lo standard JPEG
0	1
1	2
-1	3
2	4
-2	5
3	6
-3	7
...
*/
char mappatura(byte c){

	if (c == 1) return 0;
	else {
		if (c % 2 == 0)return c / 2;
		else return -(c / 2);
	}
}

typedef unsigned char uchar;
typedef unsigned int uint;

uint getLog2(uint code){
	uint log = 1;
	while (code >>= 1) ++log;

	return log;
}

class bitreader {
	// La classe ha il compito di scrivere gli n bit (n<8) meno significativi di ogni byte letto dal file specificato.


	istream& _is;
	uchar _buffRem;		//Parte rimasta del byte utilizzato precedentemente
	uchar _nBuffRem;	//Lunghezza buffRem
	uint _data;
	int _bitsData;

	// Aggiunge il bit meno significativo di u al buffer
	void read_bit(uint u, int c) {
		_data = (_data << 1) | (u & 1);
		if (++_bitsData == c) {
			uint temp = 0;
			for (uint i = 0; i < _nBuffRem; ++i)
				temp = (temp << 1) | 1;

			_buffRem = _buffRem & ((uchar)temp);
		}
	}

	// Non si deve né copiare né assegnare!
	bitreader(const bitreader&);
	bitreader& operator= (const bitreader&);

public:
	bitreader(istream& is) : _is(is), _bitsData(0), _buffRem(0), _nBuffRem(0), _data(0){}


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
				_data = (_data << 8) + ((uchar)c);
				_bitsData += 8;
				flush();
			}

			if (count > _bitsData){
				int t = count - _bitsData;
				_nBuffRem = 8;
				_is.get(c);
				_buffRem = (uchar)c;
				while (t-- > 0)
					read_bit(_buffRem >> --_nBuffRem, count);
			}
		}

	}

	uint getData(){ return _data; }
	uint getBitsData(){ return _bitsData; }

	void flush(){ _buffRem = 0; _nBuffRem = 0; }

};

vector<byte> leggiConElias(ifstream& is, unsigned quanto){
	
	vector<byte> tmp;

	unsigned count = 0;
	bitreader br(is);
	while (count < quanto){
		bool trovato = false;
		byte ris = 1;

		unsigned int nzeri = 0;
		while (!trovato){
			br.read(1);
			if (br.getData() == 0)++nzeri;

			else {
				trovato = true;
				br.read(nzeri);
				ris = (ris << nzeri) + (uchar)br.getData();

				cout << ris;
				
				tmp.push_back(mappatura((byte)ris));
				
			}
		}

		count++;
	}

	//mappatura del risultato
	return tmp;
}

/*Scrive un blocco 8x8 con il colore dato alle coordinate specificate.
Le coordinate indicano il pixel in alto a sinistra
*/
void colorablocco(image<byte>& img, unsigned x1, unsigned y1, byte colore){

	unsigned f = 0;
	unsigned y = 0;

	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++){
			img(x1 + j, y1 + i) = colore;
			f++;
		}
		y++;
	}
}




int main(){

	ifstream is("fragola.sdem", ios::binary);
	if (!is) return -1;


	char ch;
	is.get(ch);
	if (ch != 'S')
		return -1;

	is.get(ch);
	if (ch != 'D')
		return -1;

	is.get(ch);
	if (ch != 'E')
		return -1;

	is.get(ch);
	if (ch != 'M')
		return -1;

	unsigned w = 0;
	unsigned h = 0;

	for (int i = 0; i < 4; ++i){
		is.get(ch);
		w = (w << 8) + (unsigned char)ch;
	}


	for (int i = 0; i < 4; ++i){
		is.get(ch);
		h = (h << 8) + (unsigned char)ch;
	}




	ofstream os("out.pgm", ios::binary);
	if (!os) return -1;



	os << "P5" << endl;
	os << "#siamofortiammerdissima!!!" << endl;
	os << w / 8 << " " << h / 8 << endl;
	os << "255" << endl;


	//for (int i = 0; i < h / 8; ++i){ //colonne
	//	for (int j = 0; j < w / 8; ++j){ //righe
	//		is.get(ch);
	//		//elias

	//		//mappatura del risultato
	//		ch = mappatura((unsigned char)ch);
	//		//moltiplicare per 8 e sommare 128
	//		ch = (ch * 8) + 128;
	//		os.put(ch);
	//		is.seekg(8, is.cur);
	//	}
	//	is.seekg(7*w, is.cur);
	//}

	//unsigned int nDC = w*h / 64;

	//unsigned int count = 0;
	//bitreader br(is);
	//while (count < nDC){
	//	bool trovato = false;
	//	unsigned char ris = 1;

	//	unsigned int nzeri = 0;
	//	while (!trovato){
	//		br.read(1);
	//		if (br.getData() == 0)++nzeri;
	//		else {
	//			trovato = true;
	//			br.read(nzeri);
	//			ris = (ris << nzeri) + (uchar)br.getData();
	//		}
	//	}
	//	++count;

	//	//mappatura del risultato
	//	ch = mappatura((unsigned char)ris);
	//	//moltiplicare per 8 e sommare 128

	vector<byte> dc_coeff(w*h / 64);



	vector<byte> tmp;

	cout << w << "x" << h << endl;

	unsigned num_blocchi = w*h / 64;

	tmp = leggiConElias(is, num_blocchi);
	cout << "tmp è grande " << tmp.size() << endl;

	for (int i = 0; i < tmp.size(); ++i){
		dc_coeff[i] = tmp[i] + 128;
		os.put((char)dc_coeff[i]);
	}

	cout << "veidamo";
	/*Stampoa i primi 10 per degub*/
	//for (int i = 0; i < 10;i++)
	//cout << dc_coeff[i];

	/*Qua ho ottenuto i DC*/


	//vector coi dc
	//}


	image<byte> img(w, h);
	unsigned i = 0;
	for (size_t y = 0; y < h; y += 8){
		for (size_t x = 0; x < w; x += 8){
			img(x, y) = dc_coeff[i];
			i++;
		}
	}

	/*Stampa i coeff DC*/
	//for (auto a : dc_coeff)
	//cout << a << endl;
	cout << "siamo alla posizione:" << is.tellg() << endl;




	/*Leggiamo tutto gli AC*/
	vector < vector<byte>> ac_vect;

	/*Ciclo per scrorrete tutti gli AC*/

	cout << endl << endl << endl;
	for (int i = 0; i < num_blocchi; i++){

		vector<byte> _tmp(63);
		//tmp = leggiConElias(is, 63);
		for (unsigned j = 0; j < 63; ++j){
			is.get(ch);

			//tmp.push_back(static_cast<byte>(ch));
			_tmp[j] = static_cast<byte>(ch);
			//cout << "tmp[ = " << i << "] =" << tmp[i] << endl;
		}

		//cout << "siamo al blocco: " << i << endl;


		ac_vect.push_back(_tmp);
	}

	cout << "Il vettore dei coefficienti AC contiene " << ac_vect.size() << " elementi da " << ac_vect[0].size() << endl;
	cout << "inoltre ho finito di leggere il file, infatti " << is.tellg() << endl;

	unsigned k = 0;
	unsigned q = 0;
	unsigned riga = 0;

	unsigned t = 0;

	for (int r = 0; r <= num_blocchi; ++r){


		if (r < w / 8){
			colorablocco(img, r * 8, k, ac_vect);
		}
		else {
			++k;
		}
	}
	/*
	for (unsigned cazzo = 0; cazzo < h; ++cazzo){

			for (unsigned x = 0; x < w; x+=8){
				if (riga % 8 == 0){	//se non sono in una colonna multipla di 8

					
					img(k*8+1, riga) = ac_vect[cazzo][k];
					k++;
					//cout << "Abbiamo messo (" << x + 1 << ", " << riga << ") !!!" << endl;

				}
				else {
					img(k*8, riga) = ac_vect[cazzo][k];
					k++;
				}
			}
			k = 0;



			cout << "fine blocco " << cazzo << " da 63 elementi" << "di riga cazzo" << endl;
		

		riga++;
	}
	*/
		


	int a = 7364874;

	/*
	for (int j = 0; j < num_blocchi; ++j){
		for (size_t y = 0 + j; y < h; ++y){
			for (size_t x = 1 + j; x < w; x += 8){

				img(x, y) = ac_vect[k][p];
				//img(x, y) = ac_vect.at(y).at(x);
				cout << "scrivo a (" << x << "," << y << ")" << endl;
				p++;
			}

			k++;
		}
	}
	*/
	



	//system("xnview\\XnViewPortable.exe out.pgm");

	writeP5("finale.pgm", img);

	cout << "aprimao";
	system("xnview\\XnViewPortable.exe finale.pgm");
	

}