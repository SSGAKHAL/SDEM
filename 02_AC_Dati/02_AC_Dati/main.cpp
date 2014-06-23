#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

typedef unsigned char byte;
typedef vector<byte> vecb;

/*Stampa tutti gli elementi e poi un acapo*/
string stampa(vecb v){
	string tmp;
	for (auto a : v)
		tmp+=a;
	return tmp;
}


class lz77{

	unsigned _dimD;
	unsigned _dimLAB;

	vecb _dizionario;
	vecb _lab;

private:

	/*Ritorna l'iteratore alla prima posizione in cui si trova la sequenza cercata*/
	vecb::const_iterator searchInDizionario(vecb vec){	//deve essere const e per reference, altrimenti le chiamate a .begin() e .end() sono di due vettore diversi per il c++

		/*Se lo trova --> iteratore alla posizione di dizionario a cui c'è tutta la sequenza
		  Se non trova -> iteratore a vec.end()*/

		/*Cerca dadove adove / cosa cosa*/
		vecb::const_iterator it = search(_dizionario.begin(), _dizionario.end(), vec.begin(), vec.end());

		if (it != _dizionario.end()){
			cout << "...non trovato!";
		} else{
			cout << "...trovato!";
		}

		return it;
	}

void stampaTerna(unsigned pos, unsigned lung, char ch){
	cout << "(" << pos << ", " << lung << ", " << ch << ")" << endl;
}

public:
	lz77(unsigned dimDizionario, unsigned dimLookAheadBuffer) : _dimD(dimDizionario), _dimLAB(dimLookAheadBuffer){
		
		//TODO: per ora lascio dimensione infinita a tutte e due
		// a a c a a c a b c a b a b a c
	};

public:
	void encode(istream& is, ostream& os){

		vecb dati;

		/*Prendo tuttu i dati, un byte alla volta*/
		char ch;
		while (is.get(ch)){
			dati.push_back(static_cast<byte>(ch));
		}

		/*Inizializzo il lookahead buffer*/
		vecb::const_iterator first = dati.begin();
		vecb::const_iterator last = dati.begin() + _dimLAB;
		_lab = vecb(first, last);	//metto dentro i primi n valori dati!

		unsigned i = 0; //indice che scorre tutto il testo dato

		cout << "Ok iniziamo. Dimensione dizionario: " << _dimD << ", dimLookAheadBuffer: " << _dimLAB << endl;

		/*Scrivo il primo e incremento la i*/
		//stampaTerna(0, 0, dati[i]);	//1°--> indice dell'occorrenza più lunga trovata 2°-->lunghezza, 3°-->prossimo carattere

		/*Metto nel dizionario il primo elemento*/
		//_dizionario.push_back(dati[i++]);

		/*Scorro tutto il vettore*/
		while (i != dati.size()){

			//sono all'elemento i-esimo

			/*Cerco nel "dizionario"*/
			//vecb tosearch(1, dati[i]);												//comincio a cercare l'elemento i-esimo
			vecb tosearch;
			tosearch.push_back(dati[i]);

			unsigned lung = 0;					//lunghezza della sequenza massima trovata
			bool flag = true;					//se ho finito di cercare									
			vecb::iterator it;
			int pos = 0;

			/*Determino la lunghezza massima della stringa nel dizionario. 0 se non trovata!*/
			while (flag){
				cout << "Cerco '" << stampa(tosearch).c_str() << "' nel dizionario" << endl;

				/*Ad ogni ricerca, incremento pos*/
				pos++;

				/*cerco l'elemento i-esimo nel dizionario*/
				it = search(_dizionario.begin(), _dizionario.end(), tosearch.begin(), tosearch.end());

				if (it!=_dizionario.end()){

					/*Se lo trovo*/
					cout << "-->'" << stampa(tosearch).c_str() << "' trovata nel dizionario. Continuo..." << endl;
					lung++;
				
					/*Metto il corrente nel dizionario*/
					_dizionario.push_back(dati[i]);
					

					/*E quello successivo, se c'è, nel tosearch!*/
					i++;
					if (i != dati.size()){
						tosearch.push_back(dati[i]);
					}
					else {
						stampaTerna(69, lung-1, dati[i-1]);	//e incremento anche la i
						flag = false; //fine file
					}
				}
				else {
					/*Non ho trovato più questa stringa (magari il ciclo prima si! Quindi stringa massima trovata, quella ultima appena cercata-1)*/
					cout << "-->'" << stampa(tosearch).c_str() << "' non trovata. E allora aggiungo al diz dati[" << i << "]" << endl;
					flag = false;

					/*In it ho l'iteratore del vettore dizionario che dice dove c'è quello che ho trovato*/
					//int quantoIndietro = i - distance(_dizionario.begin(), it2) - lung;

					/*Metto comunque il corrente nel dizionario*/
					_dizionario.push_back(dati[i]);

					/*Quindi ora dati[i] contiene quello che devo sparare in output*/
					stampaTerna(i-pos, lung, dati[i++]);	//e incremento anche la i
				}

				if (_dizionario.size() == _dimD){
					/*Il dizionario ha superato la dimensione massima!*/
					flag = false;	//fermo la ricerca
				}

			} //fine while

			/*Prima di fare una nuova ricerca, sistemo il dizionario*/
			if (_dizionario.size() >= _dimD){

				cout << "Dizionario troppo grande: dimensione " << _dizionario.size() << endl;
				_dizionario.clear();

			}
		}

	} //end encoding

	void decode(ostream& os, istream& is);

};


int main(){
	cout << "Algoritmi di compressione DATI:" << endl << endl;
	cout << "LZ77 - a finestra scorrevole. OUTPUT: terna (pos,lung,char)" << endl;
	cout << "LZSS - simile. OUTPUT terna (0,pos,lung) oppure (1, char)" << endl;
	cout << "LZ78 - a dizionario. OUTPUT: coppie (id, nextchar)" << endl;
	cout << "LZW  - simile. OUTPUT: id" << endl;

	ifstream ifs("prova_in_lz77.txt", ios::binary);
	ofstream ofs("prova_out_lz77", ios::binary);

	if (!ifs){ cerr << "Errore apertura file input" << endl; return -1; }
	if (!ofs){ cerr << "Errore apertura file output" << endl; return -1; }

	lz77 lz(5,5);
	lz.encode(ifs, ofs);
}