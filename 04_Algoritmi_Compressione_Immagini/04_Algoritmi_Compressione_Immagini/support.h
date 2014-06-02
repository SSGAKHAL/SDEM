#ifndef SUPPORT_H
#define SUPPORT_H

#include <string>
#include <stdlib.h>
#include <iostream>

typedef unsigned char byte;

void apriImmagine(std::string nomeFileImmagine){
	
	/*Dove è memorizzato il programma*/
	std::string tmp = "xnview\\XnViewPortable.exe ";

	/*Concateno il nome del file*/
	tmp+=nomeFileImmagine;

	/*Eseguo script*/
	std::cout << "Inizio esecuzione programma esterno" << endl;
	system(tmp.c_str()); //stdlib.h, e uso c_str di string per avere il const char
};

#endif

