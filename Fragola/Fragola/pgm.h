#ifndef PGM_H
#define PGM_H



/*
Definizioni di metodi per leggere e scrivere immagini PGM
Portable GrayMap Format
solo a tonalità di grigio
*/

#include <string>

typedef unsigned char byte;

#include "image.h"

bool writeP2(const std::string& sFileName, const image<byte>& img);
bool writeP5(const std::string& sFileName, const image<byte>& img);

bool readP2(const std::string& sFileName, image<byte>& img);
bool readP5(const std::string& sFileName, image<byte>& img);

#endif // PGM_H