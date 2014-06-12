#ifndef PGM_H
#define PGM_H

/*
	Definizioni di metodi per leggere e scrivere immagini PGM
				Portable PixMap Format
						A COLORI

		p3 --> plain
		p6 --> normale
*/

#include <string>

#include "support.h"
#include "image.h"

bool writeP3(const std::string& sFileName, const image<vec3b>& img);
bool writeP6(const std::string& sFileName, const image<vec3b>& img);

bool readP3(const std::string& sFileName, image<vec3b>& img);
bool readP6(const std::string& sFileName, image<vec3b>& img);

#endif // PGM_H