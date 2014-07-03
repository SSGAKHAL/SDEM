#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

typedef unsigned char byte;

class bitreader{

	byte _buffer;
	istream& _is;
	int nbit;

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

class bitwriter{

	ostream& _os;
	int nbit;
	byte _buffer;

	void writeBit(unsigned u){

		_buffer = (_buffer << 1) | (u & 1);

		if (++nbit == 8){
			_os.put(_buffer);
			nbit = 0;
		}
	}

public:
	bitwriter(ostream& os) : _os(os), nbit(0){}

	void operator()(unsigned val, unsigned count){

		while (count > 0){
			writeBit(val >> --count);
		}
	}

	void flush(unsigned fill_bit=0){
		while (nbit > 0){
			writeBit(fill_bit);
		}
	}

	~bitwriter(){
		flush();
	}
};

///*Da un campione di 16 bit mi da un byte encodato con la mulaw*/
//byte ApplicaMuLaw(short campione){

	//byte rit = 0;
	//byte segno = 0;
	//if (campione > 0) segno = 1;

	/*Mi servono solo i 13 bit meno significativi, ne butto via 3*/
	//br(3);

	//unsigned count = 0;

	/*I 13 bit successivi sono quello che mi interessa*/
	//while (br(1) == 0){
		//count++;

		/*Se ne ho trovati 8, esco*/
		//if (count == 8)
			//break;
	//}

	//cout << "Ho trovato un uno e quindi ho ben " << count << "zeri davanti" << endl;
	//if (count == 0) return 0;

	/*Prendo i 4 valori dopo*/
	//byte valori = br(4);

//}

int main(){

	ifstream is("audio_originale.raw", ios::binary);
	if (!is){
		cerr << "Coglione" << endl;
		return -1;
	}

	ofstream os("figata.raw", ios::binary);
	if (!os){
		cerr << "Coglione" << endl;
		return -1;
	}

	bitreader br(is);
	bitwriter bw(os);


	//byte zeri=0;
	//byte temp = 0;
	//byte valori = 0;
	//byte sign = 0;
	//
	//while (true){

	//	zeri = 0;

	//	for (size_t i = 0; i < 8; ++i){
	//		if (br(1) == 0) zeri++;
	//		else break;
	//	}

	//	if (zeri == 8){
	//		temp = 0;
	//	}
	//	else {

	//		if (zeri == 7)
	//			valori = br(4);
	//		else {
	//			br(1);
	//			valori = br(4);
	//		}
	//	}

	//	temp = sign;
	//	temp = temp << 3;
	//	temp = temp | (7 - zeri);
	//	temp = temp << 4;
	//	temp = temp | valori;

	//}


	/**************************************************************************************/

	vector<byte> finale;

	while (!is.eof()){

		/*Leggo 16 bit alla volta*/

		unsigned count = 0;
		byte temp = 0;

		/*Il primo bit è il segno*/
		byte segno = br(1);

		/*I due bit dopo me ne sbatto i coglioni*/
		br(2);


		/*I 13 bit successivi sono quello che mi interessa. Conto gli zeri*/
		while (br(1) == 0){
			count++;

			/*Se ne ho trovati 8, esco*/
			if (count == 8)
				break;
		}

		//cout << "Ho trovato un uno e quindi ho ben " << count << "zeri davanti" << endl;
		if (count == 0) return 0;

		/*Prendo i 4 valori dopo*/
		byte valori = br(4);

		temp = segno;
		temp = temp << 3;
		temp = temp | (7 - count);
		temp = temp << 4;
		temp = temp | valori;


		/*Salto i bit rimanenti*/
		br(8 - count);

		/*Salvo*/
		finale.push_back(temp);
		bw(temp, 8);

		/*Dopo di questo, leggerò altri 16 bit*/

	}

	cout << "finale contiene" << finale.size() << " cose" << endl;
}
