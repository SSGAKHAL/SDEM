//
//#include <fstream>
//#include <iostream>
//#include <vector>
//
//using namespace std;
//
//class image{
//	unsigned _w, _h;
//	vector<int> _puntatore;
//
//public:
//
//	/*Costruttore con parametri*/
//	image(unsigned w = 0, unsigned h = 0) : _w(w), _h(h), _puntatore(new int[w*h]) {}
//
//	unsigned getAltezza(){
//		return _h;
//	}
//
//	unsigned getLarghezza(){
//		return _w;
//	}
//
//	string toString(){
//
//		string tmp;
//		tmp.append("Immagine di");
//		tmp.append("" + _w);
//		tmp.append("x");
//		tmp.append("" + _h);
//		tmp.append(" pixel\n");
//
//		return tmp;
//	}
//
//	void creaScalaDiGrigi(){
//
//		unsigned x = 0;
//		for (unsigned i = 0; i < _h; ++i){
//
//			for (unsigned j = 0; j < _w; ++j){
//
//				/*Non c'è un modo per scrivere this(i,j)??*/
//				_puntatore[i*_w + j] = x;
//
//			}
//			x++;
//		}
//	}
//
//	void salvaInPGN(const string nomeFile){
//
//		fstream fs(nomeFile);
//
//		/*Magic number e a capo*/
//		fs << "P5" << endl;
//
//		/*Altezza spazio larghezza e a capo*/
//		fs << _w << " " << _h << endl;
//
//		/*Massimo livello di grigio e a capo*/
//		fs << "255" << endl;
//
//		/*Valori*/
//		//TODO: fallo con iterator
//
//		for (unsigned i = 0; i < _h; ++i){
//			for (unsigned j = 0; j < _w; ++j){
//				fs.write((const char *) _puntatore[i*_w + j], 1);
//			}
//		}
//	}
//
//	const int& operator()(unsigned x, unsigned y){
//		return _puntatore[y*_w + x];
//	}
//};
//
//int main(){
//
//	//immagine vuota
//	image img(2,2);
//	//cout << img.toString();
//	img.creaScalaDiGrigi();
//	img.salvaInPGN("scalaGrigi.pgm");
//}