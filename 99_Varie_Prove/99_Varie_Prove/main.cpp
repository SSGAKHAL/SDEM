#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <map>
#include <algorithm>

#include "bitstreams.h"

#define _USE_MATH_DEFINES
#include <math.h>






using namespace std;

typedef unsigned char byte;
typedef short campione;

/*Per eseguire il file*/
// -f specifica il file, size 16 bit little endian
//ffplay -f s16le


string to_binary(unsigned int code, unsigned int len) {
	string s;
	while (len-->0) {
		if ((code >> len) & 1)
			s += '1';
		else
			s += '0';
	}
	return s;
}

void provamappe(){
	
	map<char, int> mappa;
	mappa['a'] = 97;
	mappa['b'] = 98;
	mappa['a'] += 2;

	
	unsigned i = 0;
	for (auto it = mappa.cbegin(); it != mappa.cend(); ++it, ++i) {
		cout << "mappa[" << it->first << "] = " << it->second << endl;
	}

	auto a = mappa.find('b');
	if (a != mappa.end()){
		cout << "trovato, con valore: " << a->second;
	}

	auto au = mappa.find('z');
	if (au != mappa.end()){
		cout << "trovato, con valore: " << au->second;
	}
	else {
		cout << "non trovato" << endl;
	}
}

bool maggioreDi15(int n){
	return (n >= 3);
}

void provafind(){

	vector<int> vec;

	for (int i = 0; i < 20;++i)
		vec.push_back(i);

	auto a = find_if(vec.begin(), vec.end(), maggioreDi15);
	if (a != vec.end()){
		int dove = *a;
		cout << "trovato in posizione" << dove << "cioè " << vec[*a] << endl;
	}

}

void provasearch(){

	vector<int> uno;
	vector<int> due;

	for (int i = 0; i < 20; ++i) uno.push_back(i);

	due.push_back(4);
	due.push_back(5);
	due.push_back(6);
	due.push_back(7);

	for (auto x : uno) cout << x << " ";
	cout << endl;
	for (auto x : due) cout << x << " ";

	auto dove = search(uno.begin(), uno.end(), due.begin(), due.end());
	if (dove != uno.end()){
		cout << "trovata la sequenza alla posizione " << (dove-uno.begin()) << endl;
	}
	else {
		cout << "Sequenza non trovata";
	}

}

void provacopia(){

	vector<int> uno;
	vector<int> due(3);

	for (int i = 0; i < 20; ++i) uno.push_back(i);

	//copy(uno.begin() + 4, (uno.begin() + 4 + 3),due.begin()); fa la stessa cosa di 
	copy_n(uno.begin() + 4, 3, due.begin());

	for (auto x : due) cout << x << " ";

}

void scrivibit(){

	ofstream os("nomefile", ios::binary);

	bitwriter bw(os);

	unsigned uno, due, tre;
	uno = 10;
	due = 5;
	tre = 9;

	{
		bw(uno, 4);	//1010		-->per ora 4 bit	//non scrive ancora nulla!
		bw(due, 5);	//00101		-->per ora 9 bit	//al 4 elemento (cioè ha 8 bit), scrive il primo byte poi si ricorda dell'ultimo bit
		bw(tre, 7);	//0001001	-->per ora 16bit	//al 
					//RISULTATO:
					//1010.0010 1000.1001

		cout << "Ho scritto il numero " << uno << " in 4 bit" << endl;
		cout << "Ho scritto il numero " << due << " in 5 bit" << endl;
		cout << "Ho scritto il numero " << tre << " in 7 bit" << endl;
	}

	ifstream is("nomefile", ios::binary);

	bitreader br(is);

	unsigned one, two, three;
	one = br(4);
	two = br(5);
	three = br(7);

	cout << "Ho letto il numero: " << one << " a 4 bit" << endl;
	cout << "Ho letto il numero: " << two << " a 5 bit" << endl;
	cout << "Ho letto il numero: " << three << " a 7 bit" << endl;

}

int main(){
	//provafind();
	//provasearch();
	//provacopia();
	scrivibit();
}

