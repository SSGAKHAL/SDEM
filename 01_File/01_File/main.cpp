
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

using namespace std;

typedef unsigned char byte;

/*Leggo il file un byte alla volta e stampo ogni byte in console*/
int leggiUnByteAllaVolta(string inputFile){

	ifstream is(inputFile);
	if (!is) return -1;

	char tmp;
	while (is.get(tmp)){
		byte L = tmp;
		cout << L; //L contiene un numero. se viene letto un carattere, viene letto il numero ASCII corrispondente
		if (L >= 48 && L <= 57){
			cout << endl << "Ho letto un numero!!! -> " << L - 48 << endl;
		}
	}

	return 0;
}

/*Questa funzione legge la prima riga di un file (fino al \n). leggo il magic number di un file pgm P2*/
int leggiLineeFile(const string& nomefile){

	ifstream is(nomefile, ios::binary);
	if (!is) {
		cerr << "Errore apertura file"; 
		return -1;
	}

	cout << "inizio a leggere file " << nomefile << endl;
	string magic;
	if (!getline(is, magic) || magic != "P2"){
		cerr << "Estensione File non trovata" << nomefile;
		return -1;
	}
		
	cout << "ho letto: " << magic;

	//la testina ora è appena dopo il /n. Posso continuare a leggere il file per righe
	return 0;
}

/*Un modo rapido per sapere quanto è lungo un file*/
int lunghezzaFile(const string& nomefile){
	ifstream is(nomefile, ios::ate); //ate lascia la testina alla fine del file
	return is.tellg();
}


/**********************************   Operazioni sui Bit    **********************************/

/*Shifto il byte di n posizioni e faccio l'AND bit a bit. In pratica estraggo il bit richiesto!*/
bool getBit(byte byte, int position){
	return (byte >> position) & 0x1;
}

/*stampa su stdout il byte dato*/
void printByteOnConsole(byte b){
	
	for (int i = 7; i >= 0; --i){
		cout << getBit(b, i);
	}

	cout << " "; //separo ogni 8 byte
}

/*Stampa l'ifstream dato!*/
void printFileOnConsole(ifstream& is){

	char ch;
	int tmp = 8;
	while (is.get(ch)){
		printByteOnConsole(ch);
		tmp--;
		if (tmp == 0){
			cout << endl;
			tmp = 8;
		}
	}

	cout << endl;
}

class bitwriter {
	std::ostream& _os;
	unsigned char _buffer;
	int _bits;

	// Aggiunge il bit meno significativo di u al buffer
	void write_bit(unsigned u) {
		_buffer = (_buffer << 1) | (u & 1);
		if (++_bits == 8) {
			_os.put(_buffer);
			_bits = 0;
		}
	}

	// Non si deve né copiare né assegnare!
	bitwriter(const bitwriter&);
	bitwriter& operator= (const bitwriter&);

public:
	bitwriter(std::ostream& os) : _os(os), _bits(0) {}

	// Scrive sul bitstream i count bit meno significativi di u
	void operator() (unsigned u, unsigned count) {
		while (count > 0)
			write_bit(u >> --count);
	}

	// Svuota il buffer sul file
	void flush(unsigned fill_bit = 0) {
		while (_bits > 0)
			write_bit(fill_bit);
	}

	~bitwriter() {
		flush();
	}
};

class bitreader {
	std::istream& _is;
	unsigned char _buffer;
	int _bits;

	// Aggiunge il bit meno significativo di u al buffer
	unsigned read_bit() {
		if (_bits == 0) {
			_is.get(reinterpret_cast<char&>(_buffer));
			_bits = 8;
		}
		return (_buffer >> --_bits) & 1;
	}

	// Non si deve né copiare né assegnare!
	bitreader(const bitreader&);
	bitreader& operator= (const bitreader&);

public:
	bitreader(std::istream& is) : _is(is), _bits(0) {}

	operator std::istream&() { return _is; }

	// Legge dal bitstream i count bit richiesti e li mette nei bit meno significativi del risultato
	unsigned operator() (unsigned count) {
		unsigned u = 0;
		while (count-- > 0)
			u = (u << 1) | read_bit();
		return u;
	}
};

/*********************************************************************************************/

int main(){

	//decommenta!

	//Leggo un byte alla volta il file e lo stampo in console
	//leggiUnByteAllaVolta("file");

	//Leggo a linee il file
	//leggiLineeFile("immagine.pgm"); //immagine in formato P2

	//Dimensione del file!
	//cout << "la lunghezza del file e' di: " << lunghezzaFile("file_con_solo_una_parola") << " byte." << endl;

	//stampa il valore di un bit
	printByteOnConsole('a'); cout << endl;			//stampa 0110.0001 che è 61exa, e 97 decimale == lettera 'a' ASCII
	printByteOnConsole(0x61); cout << endl;			//stampa 0110.0001 che è 61exa, e 97 decimale == lettera 'a' ASCII
	printByteOnConsole('0x61'); cout << endl;		//stampa 0011.0001 che è 31exa, e 49 decimale == lettera '1' ASCII
	printByteOnConsole('0x69'); cout << endl;		//stampa 0011.0001 che è 39exa, e 57 decimale == lettera '1' ASCII
	
	
	printFileOnConsole(ifstream("file_con_solo_una_parola"));

}