#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>


using namespace std;

typedef unsigned char byte;

struct coppia{
	unsigned indice;
	vector<byte> valore;

	coppia(unsigned _indice, vector<byte> _valore) : indice(_indice), valore(_valore){}
		
	bool operator==(vector<byte> t){
		return t == valore;
	}
};

void stampaDizionario(vector<coppia> dizio){
	for (int i = 0; i < dizio.size(); ++i){
		cout << "dizionario(" << dizio[i].indice << "," << dizio[i].valore.data() << ")" << endl;
	}
}

int main(){

	ifstream is("prova_lzw", ios::binary);
	if (!is)
		return -1;

	vector<byte> input;
	char ch;
	while (is.get(ch)){
		input.push_back((byte)ch);
	}

	/*Precarico il dizionario*/
	vector<coppia> dizionario;
	
	for (unsigned i = 0; i < 255; ++i){
	
		vector<byte> t;
		t.push_back((byte)i);
		coppia tmp(i, t);

		dizionario.push_back(tmp);
	}

	vector<byte> valore;
	unsigned ultimaPos = 0;

	ofstream os("ciao.txt", ios::binary);

	/*Scorro tutto il vettore di valori da codificare*/
	for (unsigned i = 0; i < input.size(); ++i){

		/*Per interrompere la ricerca*/
		bool trovato = false;
		
		/*Finche non trovo...*/
		while (!trovato){

			/*Cerco il valore i-esimo nel vettore*/
			valore.push_back(input[i]);

			/*C'è nel dizionario?*/
			auto a = find(dizionario.begin(), dizionario.end(), valore);
			if (a != dizionario.end()){
				/*Trovato*/
				//cout << "Trovato " << valore.data() << " nel dizionario alla posizione " << a->indice << endl;
		
				/*Con questo esce dal while, si incrementa la i e continua a cercare*/
				trovato = true;

				/*Mi memorizzo che valore è stato trovato in questo indice*/
				ultimaPos = a->indice;

			} else{
				/*Non trovato. Allora lo mando in output e aggiungo al dizionario!*/
				os << ultimaPos << "\n";

				/*Faccio coppia temporanea, con nuovo indice nel dizionario (+1), e la stringa appena NON trovata*/
				coppia c((unsigned)dizionario.size() + 1, valore);
				dizionario.push_back(c);

				/*Riazzero il mio vettore di ricerca*/
				valore.clear();

			}
		}
	}

	cout << "Dizionario alla fine:" << endl;
	stampaDizionario(dizionario);

	return 0;

}