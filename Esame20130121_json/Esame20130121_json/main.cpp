#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

typedef unsigned char byte;

unsigned R = 0;
unsigned G = 1;
unsigned B = 2;


/*
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

	//operator[]

};
*/

template <typename T>
class image{

	unsigned w;
	unsigned h;
	vector<T> data;

public:
	image(){}
	image(unsigned largh, unsigned alt): w(largh), h(alt), data(largh*alt) {}

	void resize(unsigned ww, unsigned hh){
		w = ww;
		h = hh;
		data.resize(w*h);
	}

	T& operator()(unsigned x, unsigned y){ return data[y*w + x]; }

	unsigned altezza(){ return h; }
	unsigned larghez(){ return h; }

};

template <typename T, int N>
struct vec{

	T _N[N];

	vec(){}
	vec(T& r, T& g, T& b){
		_N[R] = r;
		_N[G] = g;
		_N[B] = b;
	}
	
	T& operator[](unsigned i) { return _N[i]; }

	//T getRed(){ return _N[R]; }
	//T getGreen(){ return _N[G]; }
	//T getBlue(){ return _N[B]; }

};


vector<byte> encodePackibits(image<byte> img){

	vector<byte> vettimg;

	//for (unsigned y = 0; y < img.altezza(); ++y){
		//for (unsigned x = 0; x < img.larghez(); ++x){
			//vettimg.push_back(img(x, y));
		//}
	//}

	vettimg.push_back(97);
	vettimg.push_back(97);
	vettimg.push_back(97);
	vettimg.push_back(98);
	vettimg.push_back(99);
	vettimg.push_back(100);
	vettimg.push_back(97);
	vettimg.push_back(97);
	vettimg.push_back(98);
	vettimg.push_back(98);


	/*Arrivato qua ho il vettore vettimg che ha tutti gli elementi uno dopo l'altro*/

	int i = 0;

	vector<byte> ret;
	unsigned countuguali = 0;
	unsigned countdiversi = 0;

	byte tmp1 = vettimg[0];
	//byte tmp2;
	

	return ret;
}


typedef vec<byte, 3> vec3b;

int main(){
	
	cout << "ciao" << endl;

	ifstream is("test.ppm", ios::binary);
	if (!is)
		return -1;

	image<vec3b> img;

	//unsigned magic = 0;
	string m;
	//is.read(reinterpret_cast<char*>(&magic), 2);
	getline(is, m);
	cout << "Ho letto il magic number: " << m << endl;

	unsigned larghezza = 0;
	unsigned altezza = 0;

	/*Prendo il commento opzionale*/
	string commento;
	getline(is, commento);
	cout << "Il commento e': " << commento << endl;

	/*Prendo altezza e larghezza*/
	is >> larghezza >> altezza;
	
	cout << "Immagine " << larghezza << "x" << altezza << endl;

	unsigned maxval = 0;
	is >> maxval;
	cout << "Massimo valore: " << maxval << endl;

	/*Mangio l'acapo*/
	char cdh;
	is.get(cdh);

	img.resize(larghezza, altezza);
	
	image<byte> vectR(larghezza, altezza);
	image<byte> vectG(larghezza, altezza);
	image<byte> vectB(larghezza, altezza);

	for (unsigned y = 0; y < altezza; ++y){
		for (unsigned x = 0; x < larghezza; ++x){
			char r;
			char g;
			char b;

			is.get(r);
			is.get(g);
			is.get(b);

			img(x, y)[R] = r;
			img(x, y)[G] = g;
			img(x, y)[B] = b;

			vectR(x, y) = r;
			vectG(x, y) = g;
			vectB(x, y) = b;

		}
	}

	cout << "ho caricato l'immagine che è di " << img.larghez() << "x" << img.altezza() << endl;

	ofstream os1("r.bin", ios::binary);
	ofstream os2("g.bin", ios::binary);
	ofstream os3("b.bin", ios::binary);

	if (!os1 || !os2 || !os3){
		cerr << "fanculo" << endl;
		return -1;
	}

	/*os1 << "P5\n";
	os1 << "#ciao\n";
	os1 << altezza << " " << larghezza << "\n";
	os1 << "255\n";

	os2 << "P5\n";
	os2 << "#ciao\n";
	os2 << altezza << " " << larghezza << "\n";
	os2 << "255\n";

	os3 << "P5\n";
	os3 << "#ciao\n";
	os3 << altezza << " " << larghezza << "\n";
	os3 << "255\n";
	*/

	for (unsigned y = 0; y < altezza; ++y){
		for (unsigned x = 0; x < larghezza; ++x){
			os1 << vectR(x, y);
			os2 << vectG(x, y);
			os3 << vectB(x, y);
		}
	}

	cout << "Ho salvato le tre imamgini" << endl;

	/*Packbits*/
	vector<byte> Rpack = encodePackibits(vectR);
	vector<byte> Gpack;
	vector<byte> Bpack;

}