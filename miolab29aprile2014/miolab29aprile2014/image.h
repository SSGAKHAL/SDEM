#ifndef IMAGE_H
#define IMAGE_H

#include <vector>

using namespace std;

template <typename T>
class image {

	/*Valori per altezza e larghezza*/
	unsigned _w, _h;

	/*il vettore dei pixel*/
	vector<T> _data;

public:

	image(unsigned w = 0, unsigned h = 0) : _w(w), _h(h), _data(w*h) {}

	void resize(unsigned w, unsigned h) {
		_w = w;
		_h = h;
		_data.resize(w*h);
	}

	unsigned width() const { return _w; }
	unsigned height() const { return _h; }
	unsigned size() const { return _w*_h; }

	T& operator() (unsigned x, unsigned y) { return _data[y*_w + x]; }
	const T& operator() (unsigned x, unsigned y) const { return _data[y*_w + x]; }

	char* data() { return reinterpret_cast<char*>(_data.data()); }
	const char* data() const { return reinterpret_cast<const char*>(_data.data()); }
	unsigned data_size() const { return _w*_h*sizeof(T); }

	/*Iteratori*/
	typedef typename std::vector<T>::iterator iterator;
	typedef typename std::vector<T>::const_iterator const_iterator;

	/*Il begin*/
	iterator begin() { return _data.begin(); }

	/*Il begin const*/
	const_iterator begin() const { return _data.begin(); }

	/*Iterator end*/
	iterator end() { return _data.end(); }

	/*Iterator end const*/
	const_iterator end() const { return _data.end(); }
};

#endif // IMAGE_H
