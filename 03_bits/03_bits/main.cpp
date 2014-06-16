#include <iostream>
#include "bitstream.h"

using namespace std;

int main(){

	/*Lughezza in bit del valore dato*/
	{
		int intero = 8;
		char carattere = 'a';
		unsigned int enorme = 18294948493;

		cout << "intero che vale 8 ha bisogno di " 
			<< getLunghezzaInBit(intero) << " bits" << endl;
		cout << "char che vale a ha bisogno di " 
			<< getLunghezzaInBit(carattere) << "bits" << endl;
		cout << "uint che vale 18294948493 ha bisogno di " 
			<< getLunghezzaInBit(enorme) << "bits" << endl;
	}

	{

		ofstream os("prova", ios::binary);
		os << "a"; os << "b"; os << "c"; os << "8"; os << "1"; os << "88"; os << "a"; os << "c";
		os.close();

		ifstream is("prova", ios::binary);
		
		bitreader br(is);

		//leggo 4 bit!
		unsigned int risultato;
		risultato = br(4);	//il primo byte è una a, cioè 97, cioè 0110.0001
							//leggere 4 bit significa leggere 0110 cioè 6!
		cout << "Ho letto 4 bit, quindi: " << risultato << endl;
		cout << "Leggi altri 4 bit: " << br(4) << endl;	//cioè un uno!
		cout << "Ne leggo 8, e dovrei leggere una b cioè 98" << br(8) << endl;


	}
}