#ifndef IMAGE_H
#define IMAGE_H

#include <vector>

/*
	Questa classe rapprenseta una generica immagine
	Qualunque immagine ha 3 valori fondamentali:
		larghezza
		altezza
		dati

	Generalmente un'immagine ha i "pixel" da un byte, ma
	la classe è template e lascia che un pixel sia di dimensione
	arbitraria.

	L'immagine è sempre memorizzata per righe.
	Es immagine 10x4 (w*h)
		xxxxxxxxxx
		xxxxxxxxxx
		xxxxxxxxOx
		xxxxxxxxxx
	Per accedere a O, e si scrive image(3,9)
	si scende di 3 righe e si va avanti di 9 pixel
	Quindi 
	
	
*/

/*Immagine di...qualcosa: in genere byte*/
template <typename T>
class image {
	/*Larghezza e altezza*/
	unsigned _w, _h;

	/*I dati veri e propri*/
	std::vector<T> _data;

public:
	/*Di default, l'immagine ha dimensione 0x0*/
	image() : _w(0), _h(0) {}

	/*Crea un oggetto image con larghezza e alterzza specificata*/
	image(unsigned w, unsigned h) : _w(w), _h(h), _data(w*h) {}

	/*Il metodo ridefinisce la grandezza dell'immagine. È come fare tre setter. Il vettore dati inizializzato a 0*/
	void resize(unsigned w, unsigned h) {
		_w = w;
		_h = h;
		_data.resize(w*h);
	}

	unsigned width() const { return _w; }
	unsigned height() const { return _h; }
	unsigned size() const { return _w*_h; }

	/*Ritorna il dato (byte) alla posizione x,y data. Fa y*w + x*/
	T& operator() (unsigned x, unsigned y) { return _data[y*_w + x]; }
	
	/*Non capisco cosa cambi rispetto a quello di prima. Più performante?*/
	const T& operator() (unsigned x, unsigned y) const { return _data[y*_w + x]; }

	/*Ritorna i dati puri di questa immagine*/
	char* data() { return reinterpret_cast<char*>(_data.data()); }
	/*Non capisco cosa cambi rispetto a quello di prima. Più performante?*/
	const char* data() const { return reinterpret_cast<const char*>(_data.data()); }

	/*Quanto è grande l'immagine (escluso header). Ovvero altezza*larghezza*dimensionePixel*/
	unsigned data_size() const { return _w*_h*sizeof(T); }

	bool isInBounds(int row, int col){
		if (row >= _w || row < 0 || col >= _h || col < 0){
			return false;
		} else{
			return true;
		}
	}

	typedef typename std::vector<T>::iterator iterator;
	typedef typename std::vector<T>::const_iterator const_iterator;

	iterator begin(){ return _data.begin(); }
	const_iterator begin() const { return _data.begin();}	//è come un normale _data.begin(), ma è const

	iterator end(){ return _data.end(); }
	const_iterator end() const { return _data.end(); }




};

#endif // IMAGE_H