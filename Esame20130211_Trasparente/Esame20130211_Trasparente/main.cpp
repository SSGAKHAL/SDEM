#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


typedef unsigned char byte;


using namespace std;


class bitreader{

	byte _buffer;
	int nbit;
	istream& _is;

	unsigned readbit(){

		if (nbit == 0){
			_is.get(reinterpret_cast<char&>(_buffer));
			nbit = 8;
		}
		return (_buffer >> --nbit) & 1;
	}

public:
	bitreader(istream& is) : _is(is), nbit(0){}

	unsigned operator()(unsigned count){
		unsigned u = 0;
		while (count-- > 0){
			u = (u << 1) | readbit();
		}

		return u;
	}
};

unsigned swapEndian(unsigned val){
	return
		(val << 24) |
		((val << 8) & 0x00ff0000) |
		((val >> 8) & 0x0000ff00) |
		(val >> 24);
}

unsigned swapEndian16(short val){
	return
		((val << 8) & 0x00ff0000) |
		((val >> 8) & 0x0000ff00);
}

unsigned leggiNumeroDelCazzo(bitreader &br){

	
	unsigned count = 0;

	/*Leggo il primo bit*/
	while (br(1) == 0){
		count++;
	}

	//cout << "Ho trovato " << count << " zeri (e poi un uno)" << endl;

	if(count == 0) 
		return br(7);
	else {
		/*Se ho trovato 001, (3 bit), count=2, devo leggere 3 byte cioè 3*8 bit (24bit), meno i 3 appena letti per capirlo (24-3 = 21)*/
		count++;
		//cout << "count : " << count << " e Sto per leggere " << count * 8 - count << " bit" << endl;
		return swapEndian((br((count * 8) - count)));

	}
}

int main(){

	ifstream is("test.pim", ios::binary);
	if (!is)
		return -1;

	bitreader br(is);

	//unsigned magic= leggiNumeroDelCazzo(br);
	unsigned magic = 0;
	is.read(reinterpret_cast<char*>(&magic), 4);
	//cout << "leggo " << swapEndian(magic) << endl;
	/*Bitreader*/

	unsigned size = leggiNumeroDelCazzo(br);
	cout << "la size è :" << size << endl;

	unsigned tmp = leggiNumeroDelCazzo(br);
	//cout << "element id è: " << tmp << endl;

	unsigned altraSize = leggiNumeroDelCazzo(br);
	cout << "l'altra size è " << altraSize << endl;

	/*Salto 8 caratteri a cazzo*/
	is.read(reinterpret_cast<char*>(&magic), 4);
	is.read(reinterpret_cast<char*>(&magic), 4);

	/*Salto il prossimo numero*/
	tmp = leggiNumeroDelCazzo(br);

	unsigned terzaSize = leggiNumeroDelCazzo(br);
	cout << "terzaSize= " << terzaSize << endl;

	char primoVal = leggiNumeroDelCazzo(br);
	is.get(primoVal);
	cout << "primoVal= " << (unsigned)primoVal << endl;


}