#ifndef COPPIA_H
#define COPPIA_H

class Coppia{

	unsigned _numero;
	char _char;

public:

	/*Costruisce una coppia con il char dato, e mette il contatore a 1*/
	Coppia(char c) : _char(c), _numero(1){};

	/*Definendolo, posso usare la funzione std::sort*/
	bool operator<( Coppia c) const { 
		return (_numero < c.getNum()); 
	}

	char getChar(){ return _char; };
	unsigned getNum(){ return _numero; };
	void add(){ _numero++; }
};
#endif