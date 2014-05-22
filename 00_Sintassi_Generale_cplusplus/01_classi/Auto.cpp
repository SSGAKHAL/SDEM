#include "Auto.h"
#include <iostream>

using namespace std;

/*Implementazione del costruttore dichiarato nel .h*/
Auto::Auto(size_t a, std::string b, float c) : _cilindrata(a), _nome(b), _consumoLitriPerCentoKilometri(c){
	cout << "Costruttore Chiamato!" << endl;
	_colore = "no_colore";
}

/*Implementazione del distruttore dichiarato nel .h*/
Auto::~Auto(){
	cout << "Distruttore di Auto invocato!" << endl;
}

float Auto::quantiKmConLitri(size_t litri){
	/*Ritorno quanti km faccio con un litro * numero litri dati*/
	return 100/_consumoLitriPerCentoKilometri * litri;
}

float Auto::quantiEuroCostanoKm(float km, float costoAlLitro){
	/*Quanto mi costa fare un chilometro? questo * numeroKm*/
	return _consumoLitriPerCentoKilometri / 100 * costoAlLitro * km;
}