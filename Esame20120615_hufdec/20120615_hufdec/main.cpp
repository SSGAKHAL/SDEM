#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

typedef unsigned char byte;

class bitreader{

	byte _buffer;
	istream& _is;
	unsigned _nbit;

private:
	unsigned readbit(){
		
		if (_nbit == 0){
			_is.get(reinterpret_cast<char&>(_buffer));
			_nbit = 8;
		}

		return (_buffer >> --_nbit) & 1;
	}
public:
	bitreader(istream& is) : _is(is), _nbit(0){}

	unsigned operator()(unsigned count){

		unsigned u = 0;
		while (count-- >0){
			u = (u << 1) | (readbit());
		}

		return u;
	}

};

struct canonical{
	unsigned nbit;
	byte valore;
	unsigned code;

	bool operator==(unsigned c){
		return c == code;
	}
};

int main(){
	cout << "ciao" << endl;

	ifstream is("image.huf", ios::binary);
	if (!is)
		return -1;

	unsigned larghezza = 0;
	unsigned altezza = 0;

	is.read(reinterpret_cast<char*>(&larghezza), 2);
	is.read(reinterpret_cast<char*>(&altezza), 2);

	cout << "Dimensione " << larghezza << "x" << altezza << endl;

	vector<canonical> vett;

	unsigned count = 0;

	while (true){

		char ch;

		is.get(ch);
		

		if ((byte)ch == 0xff){
			//finito
			cout << "fine tabella" << endl;

			for (unsigned i = 0; i < vett.size(); ++i){
				cout << "numero:" << i << ": nbit= " << vett[i].nbit << ", valore=" << (unsigned)vett[i].valore << ", codice=" << vett[i].code << endl;
			}

			break;
		}

		count++;

		/*Ok, non sono a fine tabella leggo qualcosa*/

		/*Il carattere corrente è il numero i volte che devo leggere un byte*/
		byte lunghezza = ch;

		/*Il byteCorrente Mi dice quanti valori di bit correnti ho*/
		for (unsigned i = 0; i < lunghezza; ++i){

			/*Leggo il codice i-esimo*/
			is.get(ch);

			/*Mi preparo la struc vuota*/
			canonical tmp;
			tmp.nbit = count;
			tmp.valore = (byte)ch;

			if (vett.empty()){
				tmp.code = 0;
			}
			else {
				/*Il codice del precedente*/
				tmp.code = (vett.back().code + 1);

				/*Con n zeri a destra*/
				tmp.code = tmp.code << (tmp.nbit - vett.back().nbit);
			}

			//tmp.push_back((byte)ch);		}
			vett.push_back(tmp);
		}
	}

	bitreader br(is);

	ofstream os("uscita.pgm", ios::binary);
	os << "P5\n";
	os << "#vaffanculo\n";
	os << larghezza << " " << altezza << "\n";
	os << "255\n";

	cout << "Inizio a leggere i dati dalla posizione " << is.tellg() << endl; 

	while (!is.eof()){

		/*Leggo un bit*/
		unsigned tmp = br(1);
		//cout << "Leggo un " << tmp << endl;
		bool finito = false;

		/*Fintanto che non trovo il corrispondente codice*/
		while (!finito){

			/*Cerco in vett, vettore di struct*/
			auto t = find(vett.begin(), vett.end(), tmp);

			if (t != vett.end()){
				//cout << "trovato il codice: " << tmp << endl;
				os << t->valore;
				finito = true;
			} else {
				/*Faccio nuova ricerca ma con un tmp più grande*/
				tmp = (tmp << 1) | br(1);
			}
		}
	}
}