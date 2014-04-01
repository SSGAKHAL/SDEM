#include <iostream>
#include <fstream>

#include "packbits.h"

using namespace std;

int main(){
	cout << "Inizio Programma";

	ifstream is("input.txt", ios::binary);
	if (!is) return -1;

	ofstream os("output.pkb", ios::binary);
	if (!os) return -1;
}