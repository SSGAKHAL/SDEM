#include <iostream>

#include "lz77.h"

using namespace std;

lz77::lz77(){}

void lz77::scriviTerna(ostream& os, int pos, int lun, char car){

	//suppongo che un byte basti per memorizzare posizione e lunghezza. Quindi max 255!
	cout << "Scrivo (" << pos << "," << lun << "," << car << ")" << endl;

	os.put('(');
	os.put(pos);	//posizione
	os.put(',');
	os.put(lun);	//lunghezza
	os.put(',');
	os.put(car);	//carattere
	os.put(')');
}

/*Codifica*/
void lz77::encode(istream& is, ostream& os){
	
	//cout << "Inizio la codifica!" << _lb << " " << _searchBuffer << endl;
	
	char ch;

	/*Elementi della terna*/
	int lung = 0;
	int pos_cursore = 0;

	while (is.get(ch)){	//volendo essere molto pignoli, non funziona se ho un file con un solo carattere
		cout << "===== Sto leggendo il carattere: " << ch << " =====" << endl;

		/*Cosa devo cercare nel _searchBuffer?*/
		_lb += ch;
		
		cout << " Cerco '" << _lb << "' dentro " << _searchBuffer << endl;

		if (_searchBuffer.find(_lb) != string::npos){
			cout << "  L'ho trovato, continuo..." << endl;

			lung++;
			
		} else {
			/*Calcolo pos*/
			string tmp = _lb.substr(0, _lb.size() - 1);
			int pos = (tmp.empty()) ? 0 : pos = _searchBuffer.find(tmp) +1; //dove l'ho trovato
			
			cout << "  Non ho trovato '" << _lb << "', ma tutto il resto prima dell'ultima lettera (" << tmp << ") si, " << pos << "caratteri indietro" << endl;
			scriviTerna(os, pos_cursore, lung, ch);

			/*Dopo aver scritto, aumento la distanza del cursore: è dove ero prima + lunghezza + 1 carattere nuovo*/
			pos_cursore += lung + 1;
			/*Azzero lunghezza e buffer di ricerca*/
			lung = 0;
			_lb.clear();

					
		}

		/*In ogni caso, prima di leggere il carattere dopo, aggiungo al buffer qualcosa*/

		/*Se la finestra di ricerca è troppo grande, la azzero*/
		if (_searchBuffer.size() == 255){
			_searchBuffer.clear();
		}

		_searchBuffer += ch;
	}
}

void lz77::decode(istream& is, ostream& os){
	
	char ch;
	
	while (is.get(ch)){
		os.put(ch);
	}
}