#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

const unsigned finestra = 512;

const double PI = 3.14159265358979323846;

void trasformata(vector<short>& input, vector<double>& trasformato){

	/*Devo calcolare prima quelli da 0 a 1024, poi da 512 a 1536 etc
	
	0		  512	     1024      1536
	|__________|__________|

			   |__________|__________|

			              |__________|__________|
	*/

	/*For più esterno che cicla su tutti i blocchi da 512 elementi. Mi devo fermare un blocco prima,
	perchè l'ultimo blocchetto non posso farlo con qualcosa che non esiste!*/
	for (size_t j = 0; j < input.size() - finestra; j += finestra){

		/*Per 512 volte corrente*/
		for (size_t i = 0; i < finestra; ++i){

			double Xk = 0;

			/*Prendo i valori da 0 a 1024!( due blocchi. E parto da dove era j)*/
			for (size_t k = 0; k < finestra * 2; ++k){

				Xk += (input[j+k]						//parto da j (0,512,1024 etc e vado avanti per 1024 volte)
					* sin((PI / (2.0 * 512))*(k + 0.5))
					* cos((PI / 512)*(k + 0.5 + 512 / 2.0)*(i + 0.5)));
			}

			/*Terminato tutto quando, popolo il vettore trasformato*/
			

			trasformato.push_back(Xk);

		}
	}

	/*Se divido per migliaia, abbassa  il volume! Se moltiplico (per valori bassi) è come se aumentasse il gain e si sente a cazzo*/
	for (auto &a : trasformato)
		a = a * 3.0;

}

void antitrasformata(vector<double>& trasformato, vector<double>& output){

	/*For esterno come per la trasformata*/
	for (size_t j = 0; j < trasformato.size() - finestra; j+=finestra){

		/*Il for più interno ora viene fatto per primo!*/
		for (size_t k = 0; k < finestra * 2 - 1; ++k){	//Se avessi solo due blocchi (da 0 a 1024) il for non deve far si che acceda all'elemento 1024 (l'ultimo è il 1023!)

			double xn = 0.0;

			/*Per i 512 valori che ho (nell'antitrasformata ho un blocco di 512 cazzi e devo farne uno da 1024)*/
			for (size_t i = 0; i < finestra -1 ; ++i){ //sempre meno 1!
				
				xn += (trasformato[j+i] 
					* sin((PI / (2. * 512))*(k + 0.5))
					* cos((PI / 512)*(k + 0.5 + 512 / 2.)*(i + 0.5)));
			}

			/*La formula prevede una divisione per il numero di elementi in una finestra*/
			xn *= 2.0 / finestra;

			/*Antiquantizzo*/
			//xn = xn * 2.0;

			/*Ci devo mettere la sommatoria!*/
			output[k + j] += xn;	//sono dentro il ciclo 0 ... 1024.
		}
	}
}


int main(){

	

	/*Il mio file di ad esempio 100 byte elementi viene preso due byte alla volta (short)

	quindi se il file originale erano 100 byte,
	il vettore che devo elaborarare è 50 short


	
	*/

	/*Apro il file*/
	ifstream is("audio_originale.raw", ios::binary);
	if (!is) return -1;

	/*Prendo dimensione del file*/
	is.seekg(0, ios::end);
	unsigned file_size = unsigned(is.tellg());
	is.seekg(0, ios::beg);

	/*I campioni sono la metà della dimensione del file!*/
	unsigned numeroCampioni = file_size*0.5;

	/**************************************/
	/*				Vettori               */
	/**************************************/

	/*Uno short è 16 bit quindi è la dimensione del file diviso due! Se erano 4 byte, diviso 4*/
	vector<short> input(numeroCampioni);

	/*Il vettore che conterrà i coefficienti trasformati*/
	vector<double> trasformato;

	/*Il vettore che conterrà i coefficienti antitrasformati*/
	vector<double> antitrasformato;

	/*Per finire, dovrò scrivere sempre a gruppi di due byte*/
	//vector<short> output(numeroCampioni);


	/*Popolo il vettore*/
	is.read(reinterpret_cast<char*>(input.data()), file_size);

	/*E gli Aggiungo gli zeri*/
	vector<short> zeri(512);
	vector<short> padded(512 - (numeroCampioni%finestra));
	input.insert(input.begin(), zeri.begin(), zeri.end());	//aggiungo all'inizio 512 zeri
	input.insert(input.end(), padded.begin(), padded.end());
	input.insert(input.end(), zeri.begin(), zeri.end());		//aggiungo alla fine 512 zeri

	/*Ora sono pronto a fare la trasformata!*/
	trasformata(input, trasformato);
	cout << "trasformata fatta\a" << endl;

	/*Faccio anche l'antitrasfromata, che è grande 512 elementi in più!*/
	antitrasformato.resize(trasformato.size() + finestra);
	antitrasformata(trasformato, antitrasformato);
	cout << "antitrasformata fatta\a" << endl;

	/*Scrivo su file, due byte alla volta (short) --> un meraviglio cast da double (64 bit) a short (16 bit)*/
	vector<short> output(antitrasformato.size() - 2 * finestra);	//tolgo i due di padding
	for (unsigned i = finestra; i < antitrasformato.size() - finestra; ++i) {	//e infatti parto da 512 e termino 512 prima
		
		double d = antitrasformato[i]; //prendo il valore i-esimo

		/*E lo sparo in 512-512, poi 513-512, poi 514-513 etc */
		output[i - finestra] = static_cast<short>(d>0 ? d + 0.5 : d - 0.5);	//l'ultima parte è per fare approssimazione
	}

	cout << "fine!\a" << endl;
	ofstream os("vediamoAnziSentiamo.raw", ios::binary);
	if (!os)
		return false;

	os.write(reinterpret_cast<const char*>(output.data()), output.size() * sizeof(output[0]));

}