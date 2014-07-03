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

	int cazzo = 0;
	vector<byte> valore;
	unsigned ultimaPos = 0;

	ofstream os("ciao.txt", ios::binary);

	for (unsigned i = 0; i < input.size(); ++i){

		bool trovato = false;
		
		while (!trovato){

			valore.push_back(input[i]);

			auto a = find(dizionario.begin(), dizionario.end(), valore);
			if (a != dizionario.end()){
				/*Trovato*/
				cout << "Trovato " << valore.data() << " nel dizionario alla posizione " << a->indice << endl;
		
				trovato = true;
				ultimaPos = a->indice;

			} else{
				/*Non trovato*/
				os << ultimaPos << "\n";

				coppia c((unsigned)dizionario.size() + 1, valore);
				dizionario.push_back(c);

				i += cazzo;
				cazzo = 0;
				valore.clear();

			}
		}


	}


	cout << "Dizionario alla fine:" << endl;
	stampaDizionario(dizionario);



	return 0;

}