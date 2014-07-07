#include <fstream>
#include <iostream>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

/*Prendo in input 2N campioni*/
vector<double> trasformata(vector<short> samples, unsigned block_size){

	int dueN = block_size*2;		//per non sbagliarmi, ho N e 2N
	int N = block_size;

	cout << " Sto per iniziare: samples.size()= " << samples.size() << " e quindi dueN=" << dueN << ", e N=" << N << endl;

	vector<double> ret;	//devo tornare N coefficienti di samples.size() - block_size

	/*Per tutti i blocchi di 512 valori in 512 valori alla volta*/
	for (unsigned j = 0; j < samples.size() - block_size; j += block_size){

		/*Qua sono in un blocco di 512 valori, cioè samples[0] fino a samples[511]*/

		/*Calcolo la sommatoria di tutti i Xk*/
		for (unsigned k = 0; k < N - 1; ++k){

			double tmp = 0;

			/*Sommatoria esterna n=0 fino a 2N-1*/
			for (unsigned n = 0; n < dueN - 1; ++n){

				//il coefficiente xkappesimo è:
				tmp += samples[j+n] *					//blocco 1..512 poi 513..1024
					sin((M_PI / dueN) * (n + 0.5)) *
					cos((M_PI / N) * (n + 0.5 + N * 0.5) * (k + 0.5));
			}

			/*tmp contiene il coefficiente k-esimo*/
			ret.push_back(tmp);
		}
	}

	cout << "Alla fine di tutto, ho ottenuto " << ret.size() << " coefficienti. Avevo " << samples.size() << " campioni" << endl;
	return ret;
}

vector<double> anti(vector<double> trasformato, unsigned block_size){

	int dueN = block_size * 2;		//per non sbagliarmi, ho N e 2N
	int N = block_size;
	cout << "anti!" << endl;

	/*c'è + qua perchè nella trasf era un meno!*/
	vector<double> ricostruito(trasformato.size()+block_size);

	/*Scorro per block size alla volta*/
	for (unsigned j = 0; j < ricostruito.size()-block_size; j += block_size){

		cout << "j=" << j << endl;

		/*Il doppio del blocco*/
		for (unsigned n = 0; n < 2 * block_size-1; ++n){

			double tmp = 0;

			/*Per blocco*/
			for (unsigned k = 0; k < block_size - 1; ++k){
				
				//il coefficiente xkappesimo è:
				tmp += trasformato[j + n] *					//blocco 1..512 poi 513..1024
					sin((M_PI / dueN) * (n + 0.5)) *
					cos((M_PI / N) * (n + 0.5 + N * 0.5) * (k + 0.5));
			}

			tmp *= 2.0/N;

			/*tmp contiene 512 valori*/
			ricostruito[j + n] += tmp;

		}
	}

	cout << "ritorno" << endl;

	return ricostruito;
}

int mainnn(){

	const unsigned block_size = 512;

	cout << "inizio programma" << endl;

	//system("ffplay -f s16le audio_originale.raw");

	ifstream is("audio_originale.raw", ios::binary);
	if (!is) return -1;


	/*Prendo dimensione del file*/
	is.seekg(0, ios::end);
	unsigned file_size = unsigned(is.tellg());
	is.seekg(0, ios::beg);

	/*I campioni sono la metà della dimensione del file!*/
	unsigned numeroCampioni = file_size*0.5;
	vector<short> campioni(numeroCampioni);
	vector<double> trasformato;	//il vettore trasformato, ovvero vettore di coefficienti
	vector<double> antitrasformato;

	/*Popolo il vettore campioni leggendo in un botto solo file_size elementi, castati a char puntatore*/
	is.read(reinterpret_cast<char*>(campioni.data()), file_size);

	/*Aggiungo gli zeri*/
	vector<short> zeri(512);
	vector<short> padded(512-(numeroCampioni%block_size));
	campioni.insert(campioni.begin(), zeri.begin(), zeri.end());	//aggiungo all'inizio 512 zeri
	campioni.insert(campioni.end(), padded.begin(), padded.end());
	campioni.insert(campioni.end(), zeri.begin(), zeri.end());		//aggiungo alla fine 512 zeri
	
	cout << "Ok, ho letto il file di dimensione " << file_size << " e ho popolato il vettore di short, che infatti ha " << campioni.size() << " elementi." << endl;

	trasformato = trasformata(campioni, block_size);

	cout << "Il trasformato ha " << trasformato.size() << " elementi" << endl;

	antitrasformato = anti(trasformato, block_size);

	cout << "L'antitrasformato ha " << trasformato.size() << " elementi" << endl;
	vector<short> finale(antitrasformato.size() - 2 * block_size);
	for (unsigned i = 512; i < antitrasformato.size() - block_size; ++i)
		finale[i-block_size] = static_cast<short>(antitrasformato[i]);

	ofstream os("cazzo.raw", ios::binary);
	if (!os) return -1;

	os.write(reinterpret_cast<const char*>(finale.data()),finale.size()*sizeof(short));

	return 0;

	cout << "fine";

	//system("ffplay -f s16le cazzo.raw");

}