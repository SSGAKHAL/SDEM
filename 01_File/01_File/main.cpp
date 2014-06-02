
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

void leggiConEstrattore(const string& nomefile){

	ifstream is(nomefile, ios::binary);
	if (!is) return;

	/*Il file è fatto così
	
			1)	1			//UN SOLO numero di una cifra
			2)	abc			//TRE CARATTERI diversi
			3)	defgh		//UNA SOLA STRINGA
			4)	1234		//UN SOLO numero di una cifra
			5)	parola 5	//Una stringa, uno spazio e un numero
			6)	123 456		//numero spazio numero
	*/

	int uno;
	char a, b, c;
	string stringa;
	int bignumero;
	string tutto;
	int larghezza, altezza;

	is >> uno;

	is >> a;
	is >> b;
	is >> c;

	is >> stringa;

	is >> bignumero;

	is >> tutto;
	is >> larghezza >> altezza;

	
	cout << "int:\t\t" << uno << endl;
	cout << "3 chars\t\t" << a << b << c << endl;
	cout << "stringa:\t\t" << stringa << endl;
	cout << "int:\t\t" << bignumero << endl;
	cout << "stringa:\t\t" << tutto << endl;  //parola si mangia tutto, anche lo spazio!!
	cout << larghezza << altezza << endl;		//non ha senso! O meglio, dipende dall'input. Deve essere scritto non tradotto insomma!
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

/*La classe nasce per poter SCRIVERE in uno stream qualunque n bit. Tuttavia, non si può
scrivere un bit alla volta quindi bisogna prima aggreggare i bit da scrivere e non appena
se ne hanno 8, si procede a scrivere in output*/
class bitwriter {

	ostream& _os;			//steam di output
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
	bitwriter(ostream& os) : _os(os), _bits(0) {}

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
	istream& _is;	//l'inputstream!
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
	bitreader(istream& is) : _is(is), _bits(0) {}

	operator istream&() { return _is; }

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

/*********************************************************************************************/

int main(){

	//decommenta!

	/*================================= Leggere un byte alla volta ================================*/
	//leggiUnByteAllaVolta("file");

	/*====================================== Leggere una linea alla volta   =======================*/
	//leggiLineeFile("immagine.pgm"); //immagine in formato P2

	/*=============================== Ottenere dimensione del file  ===============================*/
	//cout << "la lunghezza del file e' di: " << lunghezzaFile("file_con_solo_una_parola") << " byte." << endl;

	/*==================================== Stampare i bit di un byte ==========================*/
	//printByteOnConsole('a'); cout << endl;			//stampa 0110.0001 che è 61exa, e 97 decimale == lettera 'a' ASCII
	//printByteOnConsole(0x61); cout << endl;			//stampa 0110.0001 che è 61exa, e 97 decimale == lettera 'a' ASCII
	//printByteOnConsole('0x61'); cout << endl;		//stampa 0011.0001 che è 31exa, e 49 decimale == lettera '1' ASCII
	//printByteOnConsole('0x69'); cout << endl;		//stampa 0011.0001 che è 39exa, e 57 decimale == lettera '1' ASCII

	//printFileOnConsole(ifstream("file_con_solo_una_parola"));

	//ofstream os("vediamo", ios::binary);
	//bitwriter bw(os);
	//cout << "Le seguenti istruzioni producono dei bit. Vanno letti x alla volta, dove x è il secondo parametro" << endl;
	//bw('z', 2);
	//bw('z', 7);
	//bw('z', 6);
	//bw('z', 5);

	//ifstream is("file_con_solo_lettera_U", ios::binary);
	//bitreader br(is);
	//printByteOnConsole(br(0)); cout << endl;
	//printByteOnConsole(br(1)); cout << endl;
	//printByteOnConsole(br(2)); cout << endl;	
	//printByteOnConsole(br(3)); cout << endl;
	//printByteOnConsole(br(4)); cout << endl;
	///printByteOnConsole(br(5)); cout << endl;
	//printByteOnConsole(br(6)); cout << endl;
	//printByteOnConsole(br(7)); cout << endl;
	//printByteOnConsole(br(8)); cout << endl;

	leggiConEstrattore("file_estrattore");
}