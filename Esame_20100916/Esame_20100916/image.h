#ifndef IMAGE_H
#define IMAGE_H

#include <vector>
#include <algorithm>

template<typename T>
class image{
	
	unsigned _w;
	unsigned _h;

	std::vector<T> _data;

public :
	image() : _w(0), _h(0) {};
	image(unsigned w, unsigned h) : _w(w), _h(h), _data(w*h) {}

	void resize(unsigned x, unsigned y){
		_w = x;
		_h = y;
		_data.resize(_w*_h);
	}

	char* getData(){ return _data; }
	unsigned getWidth(){ return _w; }
	unsigned getHeight(){ return _h; }
	unsigned getSize(){ return _w*_h; }

	char* data() { return reinterpret_cast<char*>(_data.data()); }
	//todo versione const

	T& operator()(unsigned x, unsigned y){ return _data[y*_w + x]; }

	unsigned getDataSize(){ return _w*_h*sizeof(T); }

	void flip(image<T>& img){
		for (unsigned y = 0; y < _h/2; ++y){
			for (unsigned x = 0; x < _w; ++x){
				std::swap(img(x,y), img(x, _h - y - 1));
			}
		}
	}
};


#endif