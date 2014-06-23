/*


Esercizio
Si scriva un programma che caricata un’immagine a livelli di grigio di dimensioni variabili, ma con
larghezza e altezza comunque multiple di 8, in formato BMP esegua le seguenti operazioni:
1) Generazione di un file contenente il numero di occorrenze dei valori da 0 a 255 e calcolo dell’entropia
dell’immagine. Il file deve essere nel formato
[valore] TAB [numero occorrenze] A CAPO
Ad esempio:
0 150
1 174
2 1202
...
3) Scorrimento dell’immagine a blocchi 8x8 e trasformazione con la trasformata DCT di tutti i blocchi
scalati di -128. L’output deve essere una matrice di interi a 32 bit.
4) Generazione di un file contenente il numero di occorrenze dei coefficienti da -1024 a 1024 e calcolo
dell’entropia dei coefficienti. Il filedeve essere nel formato precedente.
5) Quantizzazione dei coefficienti con valore di quantizzazione 16 per tutti i coefficienti. Generazione di
un file contenente il numero dioccorrenze dei coefficienti quantizzati da -1024 a 1024 e calcolo
dell’entropia. Il file deve essere nel formato precedente.
6) Ricostruzione dei coefficienti e trasformata IDCT. L’output, una volta riscalato di +128, deve essere
una immagine a livelli di grigio salvata inun nuovo file.

*/

#include <fstream>
#include <iostream>
#include "bmp.h"


using namespace std;

int main(){


	image<byte> img;

	readBMP("lena.bmp", img);


}