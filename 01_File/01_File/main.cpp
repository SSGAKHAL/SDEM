
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>

using namespace std;

typedef unsigned char byte;


int leggiUnByteAllaVolta(string inputFile){

	cout << "Leggo un byte alla volta:" << endl << endl;

	ifstream is(inputFile);
	if (!is) return -1;

	char tmp;
	while (is.get(tmp)){
		byte L = tmp;
		cout << L; //L contiene un numero. se viene letto un carattere, viene letto il numero ASCII corrispondente
		if (L >= 48 && L <= 57){
			cout << endl << "Ho letto un numero!!! -> " << L - 48 << endl;
		}
	}

	return 0;
}

int leggiHeaderFile(const string& nomefile, const string& formatoSupposto){

	ifstream is(nomefile, ios::binary);
	if (!is) return -1;

	cout << "inizio a leggere file " << nomefile << endl;
	string magic;
	if (!getline(is, magic) || magic != "P2")
		return -1;
	else
		cout << "ho letto: " << magic;
	
}

int main(){
	leggiHeaderFile("prova1.pgm", "P2");
}