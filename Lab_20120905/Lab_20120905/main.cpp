#include <fstream>
#include <iostream>
#include "pgm.h"
#include "support.h"
#include "bitstream.h"

using namespace std;

int bigToLittle(int valore){

	/*Ho 32 bit, cioè 4 byte*/
	int tmp = valore;

	byte uno = (valore & 0x000000ff) << 24;
	byte due = (valore & 0x0000ff00) << 8u;;
	byte tre = (valore & 0x00ff0000) >> 8u;;
	byte quattro = (valore & 0xff000000) >> 24u;

	return uno | due | tre | quattro;
}

/*Leggo 4 caratteri */
unsigned bigToLittle(ifstream& is, unsigned &x){
	char ch;

	for (int i = 0; i < 4; i++){
		is.get(ch);
		x = (x << 8) + (byte)ch;
	}

	return x;
}

char mappatura(byte c){
	
	if (c == 1){
		return 0;
	} else {
		if (c % 2 == 0){
			/*Se è pari*/
			return c / 2;
		} else {
			return -(c / 2);
		}
	}
}

bool readSDEM(const std::string& sFileName, image<byte>& img){

	int a = 32678;
	cout << bigToLittle(a) << endl;


	ifstream is(sFileName, ios::binary);
	if (!is) return false;

	char ch;
	string magic;
	/*I primi 4 byte sono il magic number*/
	for (unsigned i = 0; i < 4; ++i){
		is.get(ch);
		magic += ch;
	}

	cout << "Magic del file: " << magic << endl;

	unsigned w;
	unsigned h;

	bigToLittle(is, w);
	bigToLittle(is, h);

	cout << "Quindi l'immagine è una " << w << "x" << h << endl << endl;
	//img.resize(w, h);

	if (w*h % 8 != 0){
		cerr << "L'immagine non è multipla di 8!";
		return false;
	}

	unsigned tot_blocchi_8x8 = (w / 8)*(h / 8);

	cout << "Quindi " << w / 8 << " blocchi in orizzontale" << endl;
	cout << "Per " << h / 8 << " blocchi in verticale" << endl;
	cout << "Totale: " << tot_blocchi_8x8 << " blocchi" << endl << endl;

	/*Dovrò quindi leggere 203 blocchi da 8x8 byte*/

	/*Prima leggo i 203 DC*/
	/*Poi leggo i w*h-203 AC*/

	bitreader br(is);
	
	vector<byte> dc_coeff;

	for (size_t i = 0; i < tot_blocchi_8x8; ++i){
		/*Per tutti i 203 valori DC*/

		/*Conto quanti zeri trovo*/
		unsigned n_zeri=0;
		while (br(1)==0){
			//cout << "Ho trovato uno zero!" << endl;
			n_zeri++;
		}

		//cout << "Blocco[" << i << "] ha " << n_zeri << " zeri!" << endl;

		//cout << "Quindi valore[" << i <<"]= " << br(n_zeri + 1) << endl;
		unsigned val = br(n_zeri + 1);

		dc_coeff.push_back(static_cast<byte>(mappatura(static_cast<byte>(val))+128));
		
	}

	int i = 0;
	img.resize(w/8, h/8);

	/*Popolo l'immagine*/
	for (unsigned y = 0; y < h/8; ++y){			//ogni 8
		for (unsigned x = 0; x < w/8; ++x){	//ogni 8!
			img(x, y) = dc_coeff[i];
			i++;
		}
	}

	writeP5("xnview\\vediamo.pgm", img);
	system("xnview\\XnViewPortable.exe xnview\\vediamo.pgm");

	return true;
}

int maindue(){

	cout << "Inizio prova di laboratorio" << endl;
	image<byte> img;
	readSDEM("fragola.sdem", img);
	return 0;
}