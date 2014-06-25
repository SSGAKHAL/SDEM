#include <iostream>
#include <fstream>

using namespace std;

typedef unsigned char byte;

class bitreader{

	istream &_is;
	unsigned n_bits;
	byte _buffer;

private:
	unsigned leggiSingoloBit(){

		if (n_bits == 0){
			_is.get(reinterpret_cast<char&>(_buffer));
			n_bits = 8;
		}

		/*Leggo il singolo bit*/
		return (_buffer >> --n_bits) & 1;

	}

public:
	bitreader(istream& is) : _is(is), n_bits(0) {};

	unsigned operator()(unsigned quantiBit){
	
		unsigned ret = 0;;

		while (quantiBit-- > 0){
			ret = (ret<<1) | leggiSingoloBit();
		}
		return ret;
	}
};


unsigned BIANCO = 255;
unsigned NERO = 0;

int main(){
	
	ifstream is("pagina.rl", ios::binary);
	if (!is)
		return -1;

	/*Leggo MAGIC NUMBER*/
	char ch;

	//R
	is.get(ch);
	cout << "Formato file: " << ch;
	//L
	is.get(ch);
	cout << ch;
	//E
	is.get(ch);
	cout << ch << endl;

	/*Leggo 4 Byte*/
	unsigned larghezza = 0;
	unsigned altezza = 0;
	is.read(reinterpret_cast<char*>(&larghezza), sizeof(unsigned));
	is.read(reinterpret_cast<char*>(&altezza), sizeof(unsigned));

	cout << "Dimensione immagine: " << larghezza << "x" << altezza << endl;

	unsigned coloreCorrente = 0;
	is.read(reinterpret_cast<char*>(&coloreCorrente), 1);

	if (coloreCorrente == BIANCO){
		cout << "Primo colore è BIANCO!"<< endl;
	}
	else {
		cout << "Primo colore è NERO!"<< endl;
	}

	ofstream os("immagine1.pgm", ios::binary);
	os << "P5" << endl;
	os << "#ciao" << endl;
	os << larghezza << " " << altezza << endl;
	os << "255" << endl;
	
	bitreader br(is);

	unsigned tmp = 0;		
	unsigned bo = 0;

	while (!is.eof()){

		unsigned count = 0;

		while (br(1)==1){ //fintanto che leggo degli uno

			bo = br(7); //prendo i 7 bit di dati
			//cout << "Ho letto un 1, mi preparo a leggere i prossimi 7 bit" << endl;
			bo = (bo << (7 * count));	//mi aggiungo 7, 14, 21 bit a destra
			tmp = tmp | bo;	//aggiungo a bo appena shiftato il valore tmp
			count++;
		}

		/*Significa che ho letto uno zero*/
		bo = br(7); //prendo i 7 bit di dati
		bo = (bo << (7 * count));	//mi aggiungo 7, 14, 21 bit a destra
		tmp = tmp | bo;	//aggiungo a bo appena shiftato il valore tmp

		tmp++;

		//cout << "risultato finale: " << tmp << endl;
		count = 0;

		/*Scrivo l'immagine*/
		for (unsigned i = 0; i < tmp; ++i){
			
			os << (byte)coloreCorrente;
		}

		/*E cambio colore*/
		if (coloreCorrente == NERO)
			coloreCorrente = BIANCO;
		else
			coloreCorrente = NERO;

		tmp = 0;
	}

	return 0;
}