#include <stdlib.h>

#include "support.h"
#include "pgm.h"
#include "images_operations.h"

using namespace std;

int main(){
	//cout << "Inizio Programma" << endl;
	//apriImmagine("a.png");

	//writeP2("nera50", createBlackImage(50, 50));
	//apriImmagine("nera50");

	//writeP2("bianca50", createWhiteImage(50, 50));
	//apriImmagine("bianca50");

	//writeP2("grad50", createGradientHorizontalBlackToWhite(256, 256));
	//apriImmagine("grad50");

	//writeP2("grad50", createGradientVerticalBlackToWhite(256, 256));
	//apriImmagine("grad50");

	//writeP2("grad50", createGradientWhiteDiagonal(256, 256));
	//apriImmagine("grad50");

	writeP2("grad50", createScacchiera(250, 250));
	apriImmagine("grad50");

}