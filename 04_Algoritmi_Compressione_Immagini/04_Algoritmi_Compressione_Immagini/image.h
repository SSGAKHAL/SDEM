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
	la classe � template e lascia che un pixel sia di dimensione
	arbitraria.

	L'immagine � sempre memorizzata per righe.
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

	/*Il metodo ridefinisce la grandezza dell'immagine. � come fare tre setter*/
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
	
	/*Non capisco cosa cambi rispetto a quello di prima. Pi� performante?*/
	const T& operator() (unsigned x, unsigned y) const { return _data[y*_w + x]; }

	/*Ritorna i dati puri di questa immagine*/
	char* data() { return reinterpret_cast<char*>(_data.data()); }
	/*Non capisco cosa cambi rispetto a quello di prima. Pi� performante?*/
	const char* data() const { return reinterpret_cast<const char*>(_data.data()); }

	/*Quanto � grande l'immagine (escluso header). Ovvero altezza*larghezza*dimensionePixel*/
	unsigned data_size() const { return _w*_h*sizeof(T); }

};

#endif // IMAGE_H