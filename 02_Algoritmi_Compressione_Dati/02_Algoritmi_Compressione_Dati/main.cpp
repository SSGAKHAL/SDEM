#include <fstream>
#include <iostream>

//#include "support.h"

#include "lz77_old.h"

using namespace std;




int main(){

	/**************************************************
							LZ77
	***************************************************/
	lz77_coder prova("file.txt");

	prova.codify_in_file("output.txt");
}