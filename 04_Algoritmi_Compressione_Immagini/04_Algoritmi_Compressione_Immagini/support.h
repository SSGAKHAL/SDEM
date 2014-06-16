#ifndef SUPPORT_H
#define SUPPORT_H

#include <string>
#include <stdlib.h>
#include <iostream>

typedef unsigned char byte;
typedef unsigned int uint;

/*Classe template per RGB con 4 costruttori: vuoto, un parametro, due o tre*/
template <typename tipo, int n>
struct vec{

	/*Vettore di n elementi*/
	tipo _data[n];

	/*Costuttore vuoto*/
	vec(){};

	/*Costruttore con un solo valore*/
	vec(const tipo& uno){
		_data[0] = uno;
	}

	/*Costruttore con due valori*/
	vec(const tipo& uno, const tipo& due){
		_data[0] = uno;
		_data[1] = due;
	}

	/*Costruttore con tre valori*/
	vec(const tipo& uno, const tipo& due, const tipo& tre){
		_data[0] = uno;
		_data[1] = due;
		_data[2] = tre;
	}

	/*Per accedere all'iesmo. Ritorno l'iesimo elemento del vettore*/
	tipo& operator[](unsigned i){ return _data[i]; }
	const tipo& operator[](unsigned i) const { return _data[i]; } //versione const

	bool operator==(const vec& due) const {
		for (int i = 0; i < n; i++){
			if (_data[i] != due._data[i])
				return false;	//non appena ne trovo uno diverso, è diverso tutto!
		}
		return false;
	}
};

typedef vec<byte, 3> vec3b;

#endif