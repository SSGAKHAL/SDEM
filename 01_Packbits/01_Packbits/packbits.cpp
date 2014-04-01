#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <set>

using namespace std;
/*
Classe Packibits: permette di eseguire l'encoding e decoding di uno stream (es: un file)
	encode() esegue la codifica
	decode() esegue la decodifica

	1-127 ==> valori non compressi
	128 ==> fine del file
	129-257 ==> valori compressi

	es:	
		sequenza bacdaaa
			deve diventare (gli spazi non ci vanno)
		3bacd 254a 128

		ovvero 3+1 caratteri non compressi ("bacd")
		poi 257-254=3 caratteri compressi, cioè "a"

	Per eseguire la codifica si hanno i seguenti casi
		run : sto riempiendo il buffer con dei valori tutti uguali:
			mi fermo appena ne trovo uno diverso e scrivo la coppia (numero_ripetizioni, carattere)
		nonrun : sto riempiendo il buffer con dei valori tutti diversi:
			mi fermo appena ne trovo uno uguale all'ultimo, e scrivo non compresso
			tutto il contenuto del buffer, carattere per carattere


*/
class packbits{

	/*Un vettore che fa da buffer per il programma*/
	vector<char> _prev;

private:

	/*Costruttore*/
	packbits();

	/*Quando ho finito una run, devo scrivere su file!*/
	void output_run(ostream &os){

		/*Sono nel caso in cui devo comprimere! - scrivo 257 meno la grandezza del buffer!*/
		int L = 257 - _prev.size();

		/*Scrivo il numero*/
		os.put(L);

		/*Scrivo il carattere: il buffer è pieno di valori uguali quindi prendo l'ultimo*/
		os.put(_prev.back());

		/*Pulisco il buffer!*/
		_prev.clear();
	}

	void output_nonrun(ostream &os){

		/*Non devo comprire una sega:*/
		int L = _prev.size() - 1;

		/*Metto il numero*/
		os.put(L);

		/*Copio l'intero contenuto del buffer, carattere per carattere (non ci sono caratteri uguali consecutivi)*/
		/*Copio dal primo all'ultimo, usando l'interatore di char sullo stream!*/
		copy(_prev.begin(), _prev.end(), ostream_iterator<char>(os));

		/*Infine, pulisco il buffer*/
		_prev.clear();
	}

public:
	void encode(istream &is, ostream &os){

		/*Spazio per il carattere corrente*/
		char curChar;

		/*Dichiarazione di un enum con i 3 stati*/
		enum {start, run, nonrun} state = start;

		/*Pulisco il buffer se per caso era pieno*/
		_prev.clear();

		/*Metto in buffer il carattere (non importa quale sia) corrente nello stream ricevuto*/
		_prev.push_back(is.get());

		/*Comincio a scorrere tutto lo stream, dal secondo carattere fino alla fine!*/
		while (is.get(curChar)){

			switch (state){
			case start:
				/*Alla prima iterazione parto da qua. Se il carattere letto è uguale a quello che ho nel buffer*/
				if (_prev.back() == curChar)
					state = run;
				else
					state = nonrun;
				break;

			case run:
				/*Sono in una run: mi fermo solo se il corrente è diverso!*/
				if (curChar != _prev.back()){
					/*Scrivo in output il pezzo di buffer!*/
					output_run(os);

					/*E ricomincio!*/
					state = start;
				}
				break;
			case nonrun:
				if (curChar == _prev.back()){
					_prev.resize(_prev.size() - 1);
					output_nonrun(os);
					_prev.push_back(curChar);
					state = run;
					break;
				}
			}

			/*Fuori da case: in ogni caso, prendo il carattere corrente!*/
			_prev.push_back(curChar);

		} //fine del while che scorre tutto il file!


	}

	void decode(istream &is, ostream &os){

	}
};