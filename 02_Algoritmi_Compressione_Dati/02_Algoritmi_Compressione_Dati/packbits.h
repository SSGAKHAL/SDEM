#ifndef PACKBITS_H
#define PACKBITS_H

#include <vector>
#include <ostream>
#include <istream>

class Packbits {

	/*Memorizza il numero delle ripetizioni*/
	std::vector<char> _prev;

	/*cosa mandare in output se ho ripetizioni. Poi svuota il vettore*/
	void output_run(std::ostream &os);

	/*Cosa mandare in output se non ho ripetizioni. Poi svuota il vettore*/
	void output_nonrun(std::ostream &os);

public:
	/*codifica l'istream dato dell'ostream dato*/
	void encode(std::istream &is, std::ostream &os);

	/*decodifica l'istream dato dell'ostream dato*/
	void decode(std::istream &is, std::ostream &os);
};

#endif