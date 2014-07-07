#include <iostream>
#include <vector>
#include <fstream>

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
		while (count > 0)
			writebit(val >> --count);
	}

	void flush(unsigned fill_bit = 0){
		while (nbit > 0)
			writebit(fill_bit);
	}

	~bitwriter(){
		flush();
	}
};

template <typename T>
class image{

	unsigned w;
	unsigned h;
	vector<T> data;
public:
	image() : w(0), h(0){}
	image(unsigned largh, unsigned alt) : w(largh), h(alt), data(largh*alt)){}

	void resize(unsigned largh, unsigned alt){
		w = largh;
		h = alt;
		data.resize(w*h);
	}

	T& operator()(unsigned x, unsigned y) {return data[y*w + x];}

	unsigned getWidth(){ return w; }
	unsigned getHeight(){ return h; }

};

template <typename T, int N>
struct vec{

	T _DATA[N];

	vec(){}
	vec(T& r, T& g, T& b){
		_DATA[0] = r;
		_DATA[1] = g;
		_DATA[2] = b;
	}

	T& operator[](unsigned i){
		return _DATA[i];
	}
};

typedef vec<byte, 3> vec3b;

int main(){
	
	image <byte> biancoNero;
	image<vec3b> immagineColori;

	unsigned ROSSO = 0;
	unsigned VERDE = 1;
	unsigned BLU = 2;

	biancoNero(3, 2) = 4;
	immagineColori(3, 2)[1] = 4;


	ifstream is("prova.elias", ios::binary);
	if (!is)
		return -1;

	bitreader br(is);

	unsigned tmp = 1;
	unsigned numeroZeri = 0;

	while (br(1) == 0){
		numeroZeri++;
	}

	tmp = (tmp << numeroZeri) | br(numeroZeri);

}