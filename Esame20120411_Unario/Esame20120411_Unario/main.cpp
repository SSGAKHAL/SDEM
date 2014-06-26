#include <iostream>
#include <fstream>

using namespace std;

typedef unsigned char byte;

/*




*/

class bitwriter{

	ostream& _os;
	int n_bits;
	byte _buffer;

private:
	void writeBit(unsigned u){

		_buffer = (_buffer << 1) | (u & 1);

		if (++n_bits == 8){
			_os.put(_buffer);
			n_bits = 0;
		}

	}

public:
	bitwriter(ofstream &os) : _os(os), n_bits(0){};

	void operator()(unsigned u, unsigned count){

		while (count > 0){
			writeBit(u >> --count);
		}
	}

	void flush(unsigned fill = 1){
		while (n_bits > 0)
			writeBit(fill);
	}

	~bitwriter(){
		flush();
	}

};

unsigned SwapEndian(unsigned val){
	return 
		(val << 24) | 
		((val << 8) & 0x00ff0000) | 
		((val >> 8) & 0x0000ff00) | 
		(val >> 24);
}

int main(){
	cout << "Ciao" << endl;

	ifstream is("esempio.bin", ios::binary | ios::ate);
	if (!is)
		return -1;

	unsigned lunghezza = is.tellg();
	is.seekg(is.beg);

	ofstream os("esempio.bin.mio", ios::binary);
	if (!os)
		return -1;

	os << "UNARIO";
	
	/*
		byte 1 | byte 2 | byte 3 | byte 4
	*/
	cout << "lunghezza vale " << lunghezza << endl << "Che in big endian = " << SwapEndian(lunghezza) << endl << "Che di nuovo in little = " << SwapEndian(SwapEndian(lunghezza)) << endl;
	unsigned lungBIG = SwapEndian(lunghezza);

	os.write(reinterpret_cast<char*>(&lungBIG), sizeof(unsigned));

	char ch;

	bitwriter bw(os);

	while (is.get(ch)){

		/*Og
		ni carattere lo trasformo in unario*/
		for (unsigned i = 0; i < ch; ++i){
			bw(1, 1);
		}

		bw(0, 1);
	}

}
