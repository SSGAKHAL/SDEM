#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

typedef unsigned char byte;

template<typename T>
class image{

	unsigned w;
	unsigned h;
	std::vector<T> _data;

public:
	image() : w(0), h(0), _data(0){};
	image(unsigned x, unsigned y) : w(x), h(y), _data(x*y){}

	void resize(unsigned x, unsigned y){
		w = x;
		h = y;
		_data(x*y);
	}

	unsigned height(){ return h; }
	unsigned width(){ return w; }
	unsigned size(){ return w*h; }
	vector<T> data(){ return _data; }

	T& operator()(unsigned x, unsigned y){ return _data[y*w + x]; }
};

class pixel{
	unsigned r;
	unsigned g;
	unsigned b;

public:
	unsigned& getRed() { return r; }
	unsigned& getGreen() { return g; }
	unsigned& getBlue() { return b; }

	void setRed(unsigned x) { r = x; }
	void setGreen(unsigned x) { g = x; }
	void setBlue(unsigned x) { b = x; }
};

template<typename _T, int _N>
struct vec{
	_T data[_N];

	vec(){}

	/*Se ho tutti e 3*/
	vec(_T& r, _T& g, _T& b){
		data[0] = r;
		data[1] = g;
		data[2] = b;
	}

	/*Se ho solo Y*/
	vec(_T& y){
		data[0] = r;
	}

	/*Se ho solo Y*/
	vec(_T& Cb, _T& Cr){
		data[0] = Cb;
		data[1] = Cr;
	}

	_T& operator[](unsigned i) { return data[i]; }
};

typedef vec<byte,3> vec3b;

byte satura(float valore){

	if (valore > 255)
		return 255;
	else if (valore < 0)
		return 0;
	else
		return round(valore);
}

unsigned RED = 0;
unsigned GREEN = 1;
unsigned BLUE = 2;

image<vec3b> RGB2YCbCr(image<vec3b> cosa){

	image<vec3b> tmp(cosa.width(), cosa.height());

	//Y = 0.299 R + 0.587 G + 0.114 B
	//Cb = -0.1687 R - 0.3313 G + 0.5 B + 128
	//Cr = 0.5 R - 0.4187 G - 0.0813 B + 128

	for (unsigned y = 0; y < tmp.height(); ++y){
		for (unsigned x = 0; x < tmp.width(); ++x){

			byte R = cosa(x, y)[RED];
			byte G = cosa(x, y)[GREEN];
			byte B = cosa(x, y)[BLUE];

			float Y = 0.229*R + 0.587*G + 0.113*B;
			float Cb = -0.1687*R - 0.3313*G + 0.5*B + 128;
			float Cr = 0.5*R - 0.4187*G - 0.0813*B + 128;

			tmp(x, y)[RED] = satura(Y);
			tmp(x, y)[GREEN] = satura(Cb);
			tmp(x, y)[BLUE] = satura(Cr);
		}
	}

	return tmp;
}

void sottocampiona(){

}

void sovracampiona(){

}

int main(){
	
	cout << "ciao" << endl;
	
	ifstream is("rana.ppm", ios::binary);
	if (!is)
		return -1;

	string magic;
	getline(is, magic);

	cout << "Magic number: " << magic.c_str() << endl;

	string commento; 
	getline(is, commento);
	cout << "Commento: " << commento.c_str() << endl;

	string larghezza;
	getline(is, larghezza, ' ');

	unsigned w = 0;
	w = stoi(larghezza);

	string altezza;
	getline(is, altezza);
	
	unsigned h = 0;
	h = stoi(altezza);

	cout << "Dimensione immagine: " << w << "x" << h << endl;

	string maxVal;
	getline(is, maxVal);

	unsigned max = 0;
	max = stoi(maxVal);
	cout << "MaxVal = " << max << endl;


	ofstream os("punto2.bin", ios::binary);
	if (!os)
		return -1;

	//os << "P5\n";
	//os << "#ciao\n";
	//os << w << " " << h << "\n";
	//os << "255\n";

	image<vec3b> img(w,h);
	image<pixel> tmp(w, h);

	char ch;
	//TODO: if MaxVal < di 256, ogni pixel 1 byte, altrimenti ogni pixel 2 byte
	for (unsigned y = 0; y < h; ++y){
		for (unsigned x = 0; x < w; ++x){

			/*Ogni valore letto è parte di una tripletta di valori RGB*/

			/*
			is.get(ch);
			tmp(x, y).setRed((byte)ch);

			is.get(ch);
			tmp(x, y).setGreen((byte)ch);
			os << ch;

			is.get(ch);
			tmp(x, y).setBlue((byte)ch);
			*/

			is.get(ch);
			img(x, y)[RED] = (byte)ch;
			
			is.get(ch);
			img(x, y)[GREEN] = (byte)ch;

			is.get(ch);
			img(x, y)[BLUE] = (byte)ch;


		}
	}

	image<vec3b> convertita = RGB2YCbCr(img);

	for (unsigned y = 0; y < h; ++y){
		for (unsigned x = 0; x < w; ++x){
			os.put(convertita(x, y)[RED]);
			os.put(convertita(x, y)[GREEN]);
			os.put(convertita(x, y)[BLUE]);
			os.put('\n');
		}
	}

	/*Sottocampiono*/

	ofstream os2("sottocampionata.pgm", ios::binary);
	if (!os2)
		return -1;

	os2 << "P5\n";
	os2 << "#ciao\n";
	os2 << convertita.width()/2 << " " << convertita.height()/2 << "\n";
	os2 << "255\n";

	image<byte> tmp2(375,266);
	int y1 = 0;
	int x1 = 0;

	for (unsigned y = 0; y < convertita.height(); y+=2){
		for (unsigned x = 0; x < convertita.width(); x += 2){

			float mediaCb = 0;

			/*Sommo i 4 pixel*/
			for (unsigned j = 0; j < 2; ++j){
				for (unsigned i = 0; i < 2; ++i){


					/*Se non esco dal bordo*/
					if (
						(x + i) >= (convertita.width())
						||
						(y + j) >= (convertita.height())
						)
						continue;

					mediaCb += convertita(x + i, y + j)[2];
				}
			}

			mediaCb /= 4.0;
			//os2 << (byte)mediaCb;
			tmp2(x/2, y/2) = (byte)mediaCb;
			
			//cout << "scrivo in" << mediaCb;

			//cout << "Scrivo media: " << mediaCb;
			//os2 << (byte)round(mediaCb);
		}
	}

	for (unsigned y = 0; y < tmp2.width(); ++y){
		for (unsigned x = 0; x < tmp2.height(); ++x){
			os << tmp2(x, y);
		}
	}

}