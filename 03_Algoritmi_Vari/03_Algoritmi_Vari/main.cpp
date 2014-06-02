/*Il programma deve calcolare le occorrenze dei caratteri della bibbia*/


#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include "coppia.h"

typedef unsigned char byte;

using namespace std;

/*Compilalo in RELEASE! è inefficiente ma chissene, funziona*/
int main(){

	ifstream is("bibbia.txt", ios::binary);
	is.unsetf(ios::skipws);	//salta i whitespace, non ho capito a che minchia server

	vector<Coppia> coppie;

	char ch;
	while (is.get(ch)){

		bool found = false;

		/*Cerco se ce l'avevo già*/
		for (auto &a : coppie){			//usando & mi assicuro che le modifiche fatte ad a vengano fatte anche a quello "vero"!
			if (ch == a.getChar()){
				//avevo già questa coppia!
				a.add();
				found = true;
				break;
			}
		}

		/*Se non l'ho trovata*/
		if (!found){
			coppie.push_back(Coppia(ch));
		}
	}

	/*Ordino il vettore, usando l'operatore < di default --> quindi lo ordino in ordine crescente*/
	sort(coppie.begin(), coppie.end());

	/*Scambio l'ordine di tutto, per averlo decrescente --> tricky? fai che l'operatore< sia...l'operatore >!*/
	reverse(coppie.begin(), coppie.end());

	ofstream os("output.txt");
	for (auto a : coppie){
		os << (int)a.getChar() << "\t" << a.getNum() << endl;
		//os << a.getChar() << "\t" << a.getNum() << endl; se vuoi vedere il carattere
	}
}