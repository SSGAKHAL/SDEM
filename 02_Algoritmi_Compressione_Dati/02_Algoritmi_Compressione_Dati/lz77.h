#ifndef LZ77_H
#define LZ77_H

#include <vector>
#include <istream>
#include <ostream>

#include "support.h"

class lz77{

	/*Dizionario di caratteri che memorizzo mano a mano*/
	std::string _searchBuffer;

	/*LookAhead Buffer*/
	std::string _lb;

	void scriviTerna(std::ostream&, int, int, char);

public:
	
	/*Costruttore. Prende un inputstream e un outputstream*/
	lz77();

	/*Codifica l'inputstream nell'outputstream*/
	void encode(std::istream& is, std::ostream& os);

	/*Codifica l'outputstream nell'inputstream*/
	void decode(std::istream& is, std::ostream& os);

};
#endif