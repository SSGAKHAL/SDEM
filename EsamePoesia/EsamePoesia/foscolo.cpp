#include <vector>
#include <fstream>

using namespace std;

typedef unsigned char byte;

class bitwriter{

	byte _buffer;
	int nbit;
	ostream& _os;

	void writebit(unsigned u){

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
			writebit(val >> --count);
		}
	}

	void flush(unsigned fill_bit = 0){
		while (nbit > 0){
			writebit(fill_bit);
		}
	}

	~bitwriter(){
		flush();
	}
};

