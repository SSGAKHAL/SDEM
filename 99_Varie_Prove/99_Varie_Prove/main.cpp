#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>

#define _USE_MATH_DEFINES
#include <math.h>




#include "bitstreams.h"



using namespace std;

typedef unsigned char byte;
typedef short campione;

/*Per eseguire il file*/
// -f specifica il file, size 16 bit little endian
//ffplay -f s16le


string to_binary(unsigned int code, unsigned int len) {
	string s;
	while (len-->0) {
		if ((code >> len) & 1)
			s += '1';
		else
			s += '0';
	}
	return s;
}


/*samples ha già gli zeri all'inizio, alla fine e il padding*/
vector<double> calcolaXk(vector<short> samples){

	double Xk = 0;
	vector<double> finale;

	cout << "iniziamo" << endl;

	cout << "Faccio ciclo da 0 a " << (samples.size()) << endl;

	vector<double> tot(samples.size()/512);	//perfetto!!!! cit Alagggggia

	for (unsigned  j = 0; j < samples.size()/512; ++j){		//ciclo di 512 blocchi alla volta

		double cazzo=0;	//la somma di tutti i 512 valori di UNA finestra
		for (unsigned int i = 0; i < 512; ++i){	//scorro tutti il primo blocco

			cazzo += (samples[i] * sin(M_PI / samples.size()*(i + 0.5)) * cos((M_PI / samples.size()*0.5) * (i + 0.5 + samples.size()*0.5)*(i / 2 + 0.5)));
		}

		//il risultato nel valore j-esimo
		tot[j] = cazzo;
	}

	cout << "Ho " << tot.size() << " coefficienti relativi alle finestre";

	/*0 con 1, 2 con 3 etc etc*/
	for (unsigned int i = 0; i < samples.size()*0.5; i += 2){
		finale.push_back(tot[i] + tot[i + 1]);
	}

	cout << "finale contiene " << finale.size() << " cazzi" << endl;

	return finale;
}

//TODO: ricordati di aggiungere un blocco alla fine e all'inizio
int main(){

	cout << "Ciao!";

	ifstream is("prova_mdct.raw", ios::binary);
	if (!is){ cerr << "Impossibile aprire file"; return -1;}

	is.seekg(0, ios::end);
	unsigned file_size = unsigned(is.tellg());
	is.seekg(0, ios::beg);

	unsigned nSamples = file_size / 2;
	double wn;


	vector<short> samples(nSamples);

	/*Vettore di 512 zeri*/
	vector<short> zeri(512);

	/*Leggo*/
	is.read(reinterpret_cast<char*>(samples.data()), file_size);

	cout << "sample all'inizio: " << samples.size() << endl;
	/*Aggiungo all'inzio*/
	samples.insert(samples.begin(), zeri.begin(), zeri.end());
	cout << "sample dopo 512 zeri all'inizio: " << samples.size() << endl;
	/*Aggiungo dopo*/
	samples.insert(samples.end(), zeri.begin(), zeri.end());
	cout << "sample dopo 512 zeri alla fine: " << samples.size() << endl;
	/*Aggiungo Padding*/
	unsigned padding = nSamples % 512;
	cout << "padding: " << 512-padding << endl;
	vector<short> zeris(512-padding);

	samples.insert(samples.end(), zeris.begin(), zeris.end());
	cout << "sample con tutta la roba ha dimensione: " << samples.size() << endl;

	/*Calcolo coefficienti*/
	vector<double> coefficienti = calcolaXk(samples);;

	cout << "Alla fine di tutto, ho " << coefficienti.size() << " coefficienti";

	/*Aggiungo blocco all'inizio e alla fine*/
	//for (per 512)
	//coefficienti.push_back()



	//system("ffplay.exe -f prova_mdct.raw s16le");



	//ofstream prova("prova_mdct.raw", ios::binary);
	//if (!prova){ cerr << "errore" << endl; return - 1; }

	//xn --> vector
	//wn ..> usa la formula

		// -f specifica il file, size 16 bit little endian
		//ffplay -f s16le

}
