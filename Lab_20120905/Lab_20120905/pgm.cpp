#include "pgm.h"

#include <fstream>
using namespace std;

/*Scrive il plain PGM ovvero una versione umanamente leggibile con un a capo per ogni info dell'header*/
bool writeP2(const string& sFileName, const image<byte>& img) {

	/*Apro lo stream di output*/
	ofstream os(sFileName, ios::binary);
	if (!os) return false;


	/*Genero l'header*/
	os << "P2\n";										//Formato e ACAPO
	os << "#plain PGM creato nel corso di SdEM\n";		//Commento e ACAPO
	os << img.width() << " " << img.height() << "\n";	//larghezza spazio altezza
	os << "255\n";										//255, richiesto dallo standard

	/*Inserisco i dati! Per ogni riga...*/
	for (unsigned y = 0; y < img.height(); ++y) {

		/*Scrivo il primo pixel della riga*/
		os << static_cast<int>(img(0, y));				//static cast perchè...non lo so. Forse per via del riferimento

		/*E scrivo tutti gli altri della riga*/
		for (unsigned x = 1; x < img.width(); ++x) {
			os << " " << static_cast<int>(img(x, y));	//spazio + 2nd pixel + spazio + 3rd pixel...etc
		}
		os << "\n";
	}

	/*Se sono qua, tutto bene*/
	return true;
}



/*Leggo la versione semplificata!*/
bool readP2(const string& sFileName, image<byte>& img) {

	/*Apro il file!*/
	ifstream is(sFileName, ios::binary);
	if (!is) return false;

	/*Prendo il magic number!*/
	string sMagic;
	if (!getline(is, sMagic) || sMagic != "P2") //con la prima istruzione prendo la prima linea (fino a /n) e la memorizzo in sMagic
		//getLine torna un istream& quindi deve essere diverso da false altrimenti esci
		// (è come aprire il file!). Con la seconda istruzione, se sMagic non è "P2", errore!
		return false;

	/*Prendo l'eventuale commento!*/
	string sComment;
	if (!getline(is, sComment) || sComment.size() == 0 || sComment[0] != '#')	//getLine come sopra. sComment.size()==0 perchè se non
		//c'è il commento non c'è nemmeno il relativo \n quindi
		//la getline legge larghezza spazio altazza. Quindi controllo
		//che cosa????
		return false;

	unsigned w, h, max_val;

	if (!(is >> w >> h >> max_val) || max_val != 255)							//Leggo, IN BINARIO con l'operatore estrattore >>. Leggo in ordine larghezza, altezza e valore massimo.
		//Controllo inoltre che valore massimo sia 255!
		return false;

	/*Ultimo a capo!*/
	char lastLF;
	if (!is.get(lastLF) || lastLF != '\n')	//Leggo l'ultimo carattere dell'header, che è un \n. Se non lo è, esco
		return false;

	/*Creo lo spazio per w*h elementi*/
	img.resize(w, h);

	/*Prendo tutti i dati, per righe*/
	for (unsigned y = 0; y < img.height(); ++y) {		//per tutte le righe
		for (unsigned x = 0; x < img.width(); ++x) {	//per tutte le colonne

			/*Prendo il valore del pixel, di nuovo, in binario!*/
			unsigned val;
			if (!(is >> val) || val>255)	//fino alla fine del file o se ho un valore troppo grande
				return false;

			/*Se ho preso bene il valore, lo metto castandolo a byte in img(x,y)*/
			img(x, y) = static_cast<byte>(val);
		}
	}

	return true;
}

bool writeP5(const string& sFileName, const image<byte>& img) {
	ofstream os(sFileName, ios::binary);

	// Header
	os << "P5\n";
	os << "#PGM creato nel corso di SdEM\n";
	os << img.width() << " " << img.height() << "\n";
	os << "255\n";

	// Dati
	os.write(img.data(), img.data_size());

	return true;
}

bool readP5(const string& sFileName, image<byte>& img) {
	ifstream is(sFileName, ios::binary);
	if (!is)
		return false;

	string sMagic;
	if (!getline(is, sMagic) || sMagic != "P5")
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

	return is.read(img.data(), img.data_size()).good();
}