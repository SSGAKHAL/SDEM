#include <vector>

template<typename T>

class image<T>{

	unsigned w;
	unsigned h;
	std::vector _data;

public:
	image() : w(0), h(0), _data(0);
	image(unsigned x, unsigned y) : w(x), h(y), _data(x*y){}

	void resize(unsigned x, unsigned y){
		w = x;
		h = y;
		_data()
	}

	operator()(unsigned x, unsigned y){}

};