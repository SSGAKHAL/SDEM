#ifndef BITSTREAMS_H
#define BITSTREAMS_H

#include <fstream>

typedef unsigned char byte;

/*La classe nasce per poter SCRIVERE in uno stream qualunque n bit.Tuttavia, non si può
scrivere un bit alla volta quindi bisogna prima aggreggare i bit da scrivere e non appena
se ne hanno 8, si procede a scrivere in output*/
class bitwriter {

	std::ostream& _os;			//steam di output
	byte _buffer;	//buffer che si riempie delle micro operazioni che voglio fare, bit per bit
	int _bits;				//conta quante micro operazioni ho fatto. appena sono 8, scrivo!


	/*Scrivo un singolo bit nel _buffer*/
	void writeBit(unsigned u) {
		_buffer = (_buffer << 1) | (u & 1); //Aggiungo il bit meno significativo di u al buffer
		if (++_bits == 8) {					//non appena ho 8 scritture, sono pronto a
			_os.put(_buffer);				//scrivere sullo stream il buffer!
			_bits = 0;						//riazzero il conto
		}
	}

	// Non si deve né copiare né assegnare!
	bitwriter(const bitwriter&);
	bitwriter& operator= (const bitwriter&);

public:
	/*Costruttore. Prende un ostream generico. Inizializza osstream, e mette conteggio dei bit a zero*/
	bitwriter(std::ostream& os) : _os(os), _bits(0) {}

	/*Scrive un byte (8bit) i cui count bit meno significativi sono quelli di u*/
	/*Esempio:
	'z' = 122dec = 7Aexa = 0111.1010
	bw('z',8)              0111.1010

	In successione, fare bw('z,8'); bw('z',7); ...; bw('z',0), scrive:
	01111010 0111101 011110 10110 1010 010 1 000000

	*/
	void operator() (unsigned u, unsigned count) {
		while (count > 0)
			writeBit(u >> --count);
	}

	/*Se ho scritto 2 bit, in ostream non ci va niente perchè aspetta 8 bit per scrivere
	Questo assicura che i rimanenti 6 bit vengano scritto (tutti a 0).*/
	void flush(unsigned fill_bit = 0) {
		while (_bits > 0)
			writeBit(fill_bit);
	}

	/*Quando si distrugge un bitwriter...*/
	~bitwriter() {
		flush(); //flusha!
	}
};

class bitreader {
	std::istream& _is;	//l'inputstream!
	byte _buffer;	//il buffer - ovvero il byte poi da ritornare
	int _bits;		//il numero di bit letti

	// Aggiunge il bit meno significativo di u al buffer
	unsigned readBit() {

		//come prima cosa (costruttore: :bits=0) leggo il char. Poi dico che devo leggere 8 bit
		if (_bits == 0) {
			_is.get(reinterpret_cast<char&>(_buffer)); //casto per dire che _buffer è un char&
			_bits = 8;		//e poi mi preparo a leggere altri 8 bit						   	
		}
		return (_buffer >> --_bits) & 1; //ritorno shiftato di bits 
	}

	// Non si deve né copiare né assegnare!
	bitreader(const bitreader&);
	bitreader& operator= (const bitreader&);

public:
	/*Costruttore*/
	bitreader(std::istream& is) : _is(is), _bits(0) {}

	operator std::istream&() { return _is; }

	/*Legge dal bitstream i count bit richiesti e li mette nei bit meno significativi del risultato
	Ovvero: br(3) del byte 11001100 legge i 3 bit meno significativi 100
	*/
	unsigned operator() (unsigned count) {
		unsigned u = 0;
		while (count-- > 0) //faccio count volte
			u = (u << 1) | readBit(); //lo shift di u con readbit()
		return u;	//per ritornare il byte corrispondente!
	}
};

/*****************************************************************************/

/*Ritorna la lunghezza in bit di un elemento di classe T
NON FUNZIONA CON IL TIPO DOUBLE!*/
template <class T>
unsigned getLunghezzaInBit(T el){

	unsigned cont = 0;
	while (el != 0){
		el = el >> 1;
		++cont;
	}
	return cont;
}

#endif