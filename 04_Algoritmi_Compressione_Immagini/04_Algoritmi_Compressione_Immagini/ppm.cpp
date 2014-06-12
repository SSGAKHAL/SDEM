#include "ppm.h"

#include <fstream>

using namespace std;

bool writeP3(const string& nome, const image<vec3b>& img){

	ofstream os(nome, ios::binary);

	/*Scrivo l'header*/
	os << "P3\n";
	os << "# Codificato in PLAIN PPM\n";
	os << img.width() << " " << img.height() << "\n";
	os << "255\n";	//suppongo sia il massimo valore per ogni canale!

	/*Metto tutti i dati*/
	for (unsigned y = 0; y < img.height(); ++y){

		/*Il primo*/
		os << static_cast<int>(img(0, y)[0]) << " "
			<< static_cast<int>(img(0, y)[1]) << " "
			<< static_cast<int>(img(0, y)[2]);

		/*Tutti gli altri*/
		for (unsigned x = 1; x < img.width(); ++x){	//NON PARTE DA 0!!
			os << static_cast<int>(img(x, y)[0]) << " "
				<< static_cast<int>(img(x, y)[1]) << " "
				<< static_cast<int>(img(x, y)[2]);
		}

		/*Finito riga*/
		os << "\n";
	}

	return true;
}

bool writeP6(const string& nome, const image<vec3b>& img){
	
	ofstream os(nome, ios::binary);

	/*Scrivo l'header*/
	os << "P3\n";
	os << "# Codificato in PLAIN PPM\n";
	os << img.width() << " " << img.height() << "\n";
	os << "255\n";	//suppongo sia il massimo valore per ogni canale!

	/*Metto tutti i dati, fregandomene della formattazione*/
	os.write(img.data(), img.data_size());
	
	return true;
}

/*Leggo formato P3, PLAIN*/
bool readP3(const string& nome, image<vec3b>& img) {
	ifstream is(nome, ios::binary);
	if (!is)
		return false;

	string sMagic;
	if (!getline(is, sMagic) || sMagic != "P3")		//non è un P3
		return false;

	string sComment;
	if (!getline(is, sComment) || sComment.size() == 0 || sComment[0] != '#')	//c'è qualcosa ma non inizia per #
		return false;

	unsigned w, h, max_val;
	if (!(is >> w >> h >> max_val) || max_val != 255)	//leggo w h e maxnum
		return false;

	char lastLF;
	if (!is.get(lastLF) || lastLF != '\n')	//leggo ultimo \n
		return false;

	img.resize(w, h);	//"alloco" spazio per w*h valori

	for (unsigned y = 0; y < img.height(); ++y) {	//per righe
		for (unsigned x = 0; x < img.width(); ++x) {
			
			unsigned val;
			
			/*Prendo primo canale, deve essere < di 255*/
			if (!(is >> val) || val>255)
				return false;
			
			/*E lo salvo*/
			img(x, y)[0] = static_cast<byte>(val);	//tutti cast statici

			/*Prendo secondo canale, deve essere minore di 255*/
			if (!(is >> val) || val>255)
				return false;
			
			/*E lo salvo*/
			img(x, y)[1] = static_cast<byte>(val);
			
			/*Prendo il terzo canale, < 255*/
			if (!(is >> val) || val>255)
				return false;
			
			/*E lo salvo*/
			img(x, y)[2] = static_cast<byte>(val);
		}
	}

	return true;
}

bool readP6(const string& nome, image<vec3b>& img) {
	ifstream is(nome, ios::binary);
	if (!is)
		return false;

	string sMagic;
	if (!getline(is, sMagic) || sMagic != "P6")	//se non è P6, esci
		return false;

	string sComment;
	if (!getline(is, sComment) || sComment.size() == 0 || sComment[0] != '#')
		return false;

	unsigned w, h, max_val;
	if (!(is >> w >> h >> max_val) || max_val != 255)
		return false;

	char lastLF;
	if (!is.get(lastLF) || lastLF != '\n')
		return false;

	img.resize(w, h);

	/*Ritorno se ho letto tutto bene o meno! prima fa una read in img.data() di data_size() elementi. Ritorno il bit good()*/
	return is.read(img.data(), img.data_size()).good();
}