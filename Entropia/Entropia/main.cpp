#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Coppia{

	unsigned occorrenze;
	short valore;

	Coppia(unsigned a, short b) : occorrenze(a), valore(b){};
};

//template <typename T1>
//struct greater{
//	
//	bool operator()(type const& a, type const& b){
//		return a > b;
//	}
//};

float entropia(vector<short>& sample){

	sort(sample.begin(), sample.end());

	float h =  0.0;

	/*Vettore è ordinato*/
	//for (auto a : sample)
		//dcout << a << endl;


	vector<Coppia> coppie;
	for (size_t i = 0; i < sample.size(); ++i){

		unsigned tot=0;
		short tmp = sample[i];

		for (int j = i; j < sample.size(); ++j){
			if (tmp == sample[j])
				tot++;
		}
		i += tot - 1;

		//cout << "per " << sample[i] << " ci sono " << tot << " occorrenze " << endl;
		coppie.push_back(Coppia(tot, sample[i]));

	}

	/*Ho il vettore di valore-occorrenze*/
	//for (auto a : coppie){
	//	cout << "(" << a.valore << ", " << a.occorrenze << ")" << endl;
	//}

	/*Calolco entropia!*/
	for (unsigned i = 0; i < coppie.size(); ++i){

		float num = (coppie[i].occorrenze);
		float den = sample.size();

		float px = num / den;
		//cout << px << endl; 
		h += px * log2(px);
	}

	h = -h;
	//cout << h << endl;

	/*Calcolo tutte le occorrenze*/
	return h;

}

int main(){

	/*Apro il file*/
	ifstream is("audio_originale.raw", ios::binary | ios::ate);
	if (!is) return -1;

	/*Prendo dimensione del file*/
	unsigned file_size = unsigned(is.tellg());
	is.seekg(0, ios::beg);

	/*I campioni sono la metà della dimensione del file!*/
	unsigned numeroCampioni = file_size*0.5;

	vector<short> campioni(numeroCampioni);
	
	/*Popolo il vettore campioni leggendo in un botto solo file_size elementi, castati a char puntatore*/
	is.read(reinterpret_cast<char*>(campioni.data()), file_size);

	//vector<short> campioni;
	//campioni.push_back(3);
	//campioni.push_back(3);
	//campioni.push_back(4);
	//campioni.push_back(5);
	//campioni.push_back(6);
	//campioni.push_back(4);
	//campioni.push_back(4);
	//campioni.push_back(1);

	cout << "campioni ha" << campioni.size() << "elementi" << endl;

	cout << "CAZZO: " << entropia(campioni) << endl;

}