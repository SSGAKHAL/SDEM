#include <iostream>
using namespace std;

#include "pgm.h"

#include "image_operation.h"

int main(int argc, char **argv) {
	// Esercizio 1
	if (false) {
		image<byte> img(256, 256);

		for (unsigned y = 0; y < img.height(); ++y) {
			for (unsigned x = 0; x < img.width(); ++x) {
				img(x, y) = y;
			}
		}

		writeP2("provaP2.pgm", img);
		writeP5("provaP5.pgm", img);
	}

	// Esercizio 2
	if (false) {
		image<byte> img;
		if (!readP2("rana_asc.pgm", img)) {
			cout << "Errore durante il caricamento del file.\n";
			return EXIT_FAILURE;
		}

		flip(img);
		writeP2("flipped_rana_P2.pgm", img);
	}

	// Esercizio 3
	if (false) {
		image<byte> img;
		if (!readP5("rana_bin.pgm", img)) {
			cout << "Errore durante il caricamento del file.\n";
			return EXIT_FAILURE;
		}

		flip(img);
		writeP5("flipped_rana_P5.pgm", img);
	}

	//Esercizio 4
	if (true){
		cout << "Mi hai passato ben " << argc << " parametri" << endl;

		if (argc < 3){
			cout << "Errore numero parametri: serve nomefileinuput e nomefileoutput" << endl;
			return EXIT_FAILURE;
		}

		string nome_file_input = argv[1];
		cout << "Nome file input è argv[1]= " << nome_file_input << endl;

		string nome_file_output = argv[2];
		cout << "Nome file output è argv[2]= " << nome_file_output << endl;

		image<byte> img;
		if (!readP2(nome_file_input, nome_file_output, img)){
			cout << "Errore lettura file input";
			return EXIT_FAILURE;
		}



	}


}