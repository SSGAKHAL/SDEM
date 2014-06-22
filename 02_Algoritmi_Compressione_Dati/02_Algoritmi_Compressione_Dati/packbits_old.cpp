#include <iterator>

#include "support.h"
#include "packbits.h"

using namespace std;


void Packbits::output_run(ostream& os){

	/*Calcolo quante ripetizioni*/
	int L = 257 - _prev.size();

	os.put(L);				//scrivo il numero di ripetizioni
	os.put(_prev.back());	//scrivo il carattere: _prev contiene tutti valori uguali
	_prev.clear();			//pulisco il vettore
};

void Packbits::output_nonrun(ostream& os){

	/*Calcolo quanti caratteri non ripetuti ho trovato*/
	int L = _prev.size() - 1;

	os.put(L);				//scrivo il numero delle non ripetizioni
	copy(_prev.begin(), _prev.end(), ostream_iterator<char>(os));	//copio il vettore così com'è
	_prev.clear();	//pulisco il vettore
};

void Packbits::encode(istream &is, ostream &os) {

	char chCur;

	enum { start, run, nonrun } state = start;

	_prev.clear();
	_prev.push_back(is.get());	//butto nel vettore il primo carattere

	/*Scorro tutto il file char per char*/
	while (is.get(chCur)) {

		switch (state) {
			case start:
				/*Se il carattere nuovo è uguale all'ultimo che ho messo nel vector, parte la run*/
				state = (chCur == _prev.back()) ? run : nonrun;
				break;

			case run:
				/*Se il carattere corrente è diverso dalla serie che stavo scrivendo sul vettore*/
				if (chCur != _prev.back()) {
					/*Ho finito la run, scrivo e ricomincio*/
					output_run(os);
					state = start;
				}
				break;

			case nonrun:
				/*Se il carattere corrente è uguale allì'ultimo inserito*/
				if (chCur == _prev.back()) {

					_prev.resize(_prev.size() - 1);	//scarto l'ultimo valore
					output_nonrun(os);				//mando in output
					_prev.push_back(chCur);			//metto il carattere
					state = run;					//e inizio una run
				}
				break;
		}

		/*In qualunque stato fossi, metto nel vettore il carattere corrente*/
		_prev.push_back(chCur);

		/*Se il vetore che sto popolando è più grande di 129, devo svuotare il vettore*/
		if (_prev.size() == 129) {
			_prev.resize(_prev.size() - 1);
			switch (state) {
				case run: output_run(os); break;	//svuoto il vettore in output_run
				case nonrun: output_nonrun(os); break;//svuot il vettore in output_nonrun
			}

			_prev.push_back(chCur); //poi ricomincio! inserendo il carattere nel vettore
			state = start;
		}
	}

	/*Qua ho finito di leggere tutto il file*/

	/*Prima di uscire, scrivo eventuali valori presenti nel vettore*/
	switch (state) {
		case run: output_run(os); break;
		case start:	//sia se ero start o nonrun, fai output nonrun
		case nonrun: output_nonrun(os); break;
	}

	/*Per finire, scrivi 128*/
	os.put((char)128);
};

void Packbits::decode(std::istream &is, std::ostream &os){

	char tmp;

	/*Leggo il file un char alla volta*/
	while (is.get(tmp)) {

		/*Prendo il numero L*/
		byte L = tmp;

		/*Se il numero è sotto 128*/
		if (L < 128) {
			/*Scrivo i prossimi L+1 char così come sono stati scritti sul file*/
			for (int i = 0; i < L + 1; ++i)
				os.put(is.get());

		} else if (L>128) {
			/*Devo scrivere 257-L volte il carattere!*/
			L = 257 - L;

			is.get(tmp);
			for (int i = 0; i < L; ++i)
				os.put(tmp);
		} else
			//per uscire se sono alla fine del file!
			break;
	}
};