#include "pgm.h"

#include <fstream>
#include <iostream>
using namespace std;

/*Il metodo scrive in formato P2 cioè PLAIN PGM*/
bool writeP2(const string& sFileName, const image<byte>& img) {

	/*Apro lo stream*/
	ofstream os(sFileName, ios::binary);

	/*Inizializzo l'header*/
	os << "P2" << endl;
	os << "#plain PGM creato da Me. Questo è un commento!" << endl;
	os << img.width() << " " << img.height() << endl;
	os << "255" << endl;

	/*Scrivo l'immagine vera e propria nell'OS.*/
	for (unsigned y = 0; y < img.height(); ++y) {
		os << static_cast<int>(img(0, y));

		for (unsigned x = 1; x < img.width(); ++x) {
			os << " " << static_cast<int>(img(x, y));
		}

		os << endl;
	}

	cout << "Tutto ok, ho salvato l'immagine in PLAIN PGM";
	return true;
}

bool writeP5(const string& sFileName, const image<byte>& img) {

	/*Apro il file*/
	ofstream os(sFileName, ios::binary);

	/*Inizializzo l'header*/
	os << "P5" << endl;
	os << "#PGM creato da me!. Questo è un commento" << endl;
	os << img.width() << " " << img.height() << endl;
	os << "255" << endl;

	/*Scrivo i dati con un'unica write!*/
	os.write(img.data(), img.data_size());

	return true;
}

bool writeHUF(const string& sFileName, const string& sFileOutput, const image<byte>& img){

	/*Leggo nella variabile img_input l'immagine*/
	image<byte> img_input;
	if (!readP2(sFileName, img_input)){
		return EXIT_FAILURE;
	}

	ofstream os(sFileOutput, ios::binary);

	/*Header*/

	/*Scrivo il magic number*/
	os.write("HUF", 3);

	/*LARGHEZZA Intero senza segno a 32 bit little endian*/
	os.write((const char *)img_input.width(), 4);

	/*ALTEZZA Intero senza segno a 32 bit little endian*/
	os.write((const char *)img_input.height(), 4);

	os.write(, 280);

}

bool readP2(const string& sFileName, image<byte>& img) {
	ifstream is(sFileName, ios::binary);

	if (!is)
		return false;

	string sMagic;
	if (!getline(is, sMagic) || sMagic != "P2")
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

	for (unsigned y = 0; y < img.height(); ++y) {
		for (unsigned x = 0; x < img.width(); ++x) {
			unsigned val;
			if (!(is >> val) || val>255)
				return false;
			img(x, y) = static_cast<byte>(val);
		}
	}

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