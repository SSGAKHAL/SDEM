#include <fstream>
#include "pgm.h"
#include "images_operations.h"

using namespace std;

void apriImmagine(std::string nomeFileImmagine){

	/*Dove è memorizzato il programma*/
	std::string tmp = "xnview\\XnViewPortable.exe ";

	/*Concateno il nome del file*/
	tmp += nomeFileImmagine;

	/*Eseguo script*/
	std::cout << "Inizio esecuzione programma esterno" << std::endl;
	system(tmp.c_str()); //stdlib.h, e uso c_str di string per avere il const char
};

int main(){
	//cout << "Inizio Programma" << endl;
	//apriImmagine("a.png");

	image<byte> img;

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

	//writeP2("grad50", createScacchiera(250, 250));
	//apriImmagine("grad50");
	{
		//readP2("rana_asc.pgm", img);
		//img = specchiaOrizzontale(img);
		//img = specchiaVerticale(img);
		//writeP2("out.pgm", img);
		//apriImmagine("out.pgm");
	}

	{
		readP2("rana_asc.pgm", img);
		//apriImmagine("rana_asc.pgm");
		//img = blackANDWhite(img, 127); 
		img = ruota90(img);

		writeP2("out.pgm", img);
		apriImmagine("out.pgm");

	}
}