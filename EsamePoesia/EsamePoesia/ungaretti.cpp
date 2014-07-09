#include <iostream>
#include <vector>


using namespace std;

typedef unsigned char byte;

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

class bitwriter{

	byte _buffer;
	ostream& _os;
	int nbit;

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