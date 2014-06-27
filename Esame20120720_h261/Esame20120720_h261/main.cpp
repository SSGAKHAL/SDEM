#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

typedef unsigned char byte;

class bitreader{

	byte _buffer;
	istream& _is;
	unsigned nbit;
	unsigned long bitletti;

	unsigned readBit(){

		bitletti++;
		if (nbit == 0){
			_is.get(reinterpret_cast<char&>(_buffer));
			nbit = 8;
		}

		return (_buffer >> --nbit) & 1;
	}

public:
	bitreader(istream& is) : _is(is), nbit(0), bitletti(0){}

	unsigned operator()(unsigned count){
		unsigned u = 0;
		while (count-- > 0){
			u = (u << 1) | readBit();
		}

		return u;
	}

	unsigned getNumBitLetti(){
		return bitletti;
	}
};

struct elemento{

	unsigned _code;
	byte _valore;
	unsigned _nbit;

	bool operator==(unsigned c){
		return c == _code;
	}

	elemento(byte valore, unsigned code, unsigned nbit) : _valore(valore), _code(code), _nbit(nbit){}

};

int main(){



	/*NON FUNZIONA IL PUNTO 3 MA CI SI VA VICINO
	
	NON VA IL RICONOSCIMENTO DEL CODICE SE MANCA UN GOB E INIZIA UN NUOVO GOB O UN PSC
	
	*/

	cout << "ciao" << endl;

	ifstream is("video_test.h261", ios::binary);
	if (!is)
		return -1;

	ofstream os("picture.txt", ios::binary);
	if (!os)
		return -1;

	ofstream os2("gob.txt", ios::binary);
	if (!os2)
		return -1;




	ofstream os3("mb.txt", ios::binary);
	if (!os3)
		return -1;

	bitreader br(is);

	unsigned numeroProgressivo = 0;
	unsigned posizioneInBit = 0;
	unsigned temporalReference = 0; //in decimale
	unsigned formato = 0;


	/*Per il punto 3*/
	/*genero la tabella*/
	vector<elemento> tabella;

	// il costruttore è elemento(byte valore, unsigned code, unsigned nbit);
	tabella.push_back(elemento(1, 1, 1));

	tabella.push_back(elemento(2, 3, 3));
	tabella.push_back(elemento(3, 2, 3));

	tabella.push_back(elemento(4, 3, 4));
	tabella.push_back(elemento(5, 2, 4));

	tabella.push_back(elemento(6, 3, 5));
	tabella.push_back(elemento(7, 2, 5));

	tabella.push_back(elemento(8, 7, 7));
	tabella.push_back(elemento(9, 6, 7));

	tabella.push_back(elemento(10, 11, 8));
	tabella.push_back(elemento(11, 10, 8));
	tabella.push_back(elemento(12, 9, 8));
	tabella.push_back(elemento(13, 8, 8));
	tabella.push_back(elemento(14, 7, 8));
	tabella.push_back(elemento(15, 6, 8));

	tabella.push_back(elemento(16, 23, 10));
	tabella.push_back(elemento(17, 22, 10));
	tabella.push_back(elemento(18, 21, 10));
	tabella.push_back(elemento(19, 20, 10));
	tabella.push_back(elemento(20, 19, 10));
	tabella.push_back(elemento(21, 18, 10));

	tabella.push_back(elemento(22, 35, 11));
	tabella.push_back(elemento(23, 34, 11));
	tabella.push_back(elemento(24, 33, 11));
	tabella.push_back(elemento(25, 32, 11));
	tabella.push_back(elemento(26, 31, 11));
	tabella.push_back(elemento(27, 30, 11));
	tabella.push_back(elemento(28, 29, 11));
	tabella.push_back(elemento(29, 1, 11));
	tabella.push_back(elemento(30, 1, 11));
	tabella.push_back(elemento(31, 1, 11));
	tabella.push_back(elemento(32, 1, 11));
	tabella.push_back(elemento(33, 1, 11));




	/*Temporanea per trovare il PSC*/
	unsigned tmp = 0;

	/*Temporanea per trovate il GSC*/
	unsigned tmp2;

	bool trovato = false;

	/*Picture Start Code*/
	unsigned PSC = 0;

	while (!is.eof()){

		tmp = (tmp << 1) | br(1);
		
		/*20 bit*/
		PSC = tmp & 0x000FFFFF;

		if (PSC == 16){
			
			numeroProgressivo++;

			posizioneInBit = br.getNumBitLetti()-20;

			/*I prossimi 5 bit sono la temporal reference*/
			temporalReference = br(5);

			/*Scarto 3 bit*/
			br(3);

			/*Prendo il formato*/
			formato = br(1);

			/*Scarto altri 2 bit*/
			br(2);

			/*Leggo il bit dopo. Se ho l'exta information, salta un byte*/
			while (br(1) == 1){
				br(8);
			}

			os << numeroProgressivo << "\t" << posizioneInBit << "\t" << temporalReference << "\t" << formato << "\n";

			/********************************************************************/

			unsigned numeroBlocchi = 0;
			unsigned posizioneBloccoInBit = 0;
			unsigned GBSC = 0;

			if (formato == 1){
				numeroBlocchi = 12;
			}
			else {
				numeroBlocchi = 3;
			}

			for (unsigned i = 0; i < numeroBlocchi; ++i){

				while (GBSC != 1){

					tmp2 = (tmp2 << 1) | br(1);

					GBSC = tmp2 & 0x0000FFFF;

				}

				GBSC = 0;	//forse non serve
				posizioneBloccoInBit = br.getNumBitLetti()-16;

				unsigned numberGOB = br(4);
				unsigned GQUANT = br(5);
				
				/*Leggo il bit dopo. Se ho l'exta information, salta un byte*/
				while (br(1) == 1){
					br(8);
				}

				os2 << numeroProgressivo << "\t" << posizioneBloccoInBit << "\t" << numberGOB << "\t" << GQUANT << "\n";

				/**********************************************************/

				/*Inizio a leggere il macroblock address*/

				byte macroBlockAddress = 0;
				unsigned numbit = 0;
				unsigned dovebit = 0;

				trovato = false;

				unsigned codice = 0;

				while (!trovato){

					codice = (codice << 1) | br(1);

					if (codice == GBSC){

						trovato = true;
						cout << "Stavo cercando il codice" << codice << " ma mi sono accorto che è lo stesso di un GOB! - o anche un  PSC" << endl;
						tmp2 = codice;
						tmp = codice;
					}

					auto a = find(tabella.begin(), tabella.end(), codice);
					if (a != tabella.end()){
						
						/*Trovato!*/
						dovebit = br.getNumBitLetti() - 1;
						numbit = a->_nbit;
						macroBlockAddress = a->_valore;
						trovato = true;
					}
					else {
						cout << "Non ho trovato il codice: " << codice << " di " << numbit << " bit. Continuo" << endl;
					}
				}

				if (trovato)
					os3 << numeroProgressivo << "\t" << dovebit << "\t" << numberGOB << "\t" << (unsigned)macroBlockAddress << "\t" << numbit << "\n";

				trovato = false;

			}
		}


	}
}