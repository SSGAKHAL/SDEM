#include <fstream>

#include "pgm.h"
#include "ppm.h"

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


int radiantiInGradi(double radianti){

}

double gradiInRadianti(int gradi){
	return gradi * 3.1415 / 180.0;
}

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
		//readP2("rana_asc.pgm", img);
		//apriImmagine("rana_asc.pgm");
		//img = blackANDWhite(img, 127); 
		//img = ruota90(img);

		//writeP2("out.pgm", img);
		//apriImmagine("out.pgm");

	}

	double PI = M_PI;
	double ta = atan(1) * 4;

	cout << "90 gradi sono " << gradiInRadianti(90) << " radianti" << endl;
	cout << "valore sin(_90_) " << sin(ta)*90/180.0 << endl;

	cout << endl;



	{
		image<vec3b> imgc;
		/*Leggo immagine a colori*/
		cout << readP6("landscape.ppm", imgc);

		cout << "ho letto un'immagine ppm! dimensioni: " << imgc.width() << "x" << imgc.height() << endl;
		//vector<image<byte>> t = separateRGBGrayScale(imgc);
		vector<image<vec3b>> t = separateRGB(imgc);
		cout << "separato: ho ottenuto " << t.size() << " immagini di " << t[0].width() << "x" << t[0].height() << endl;
		writeP6("r.ppm", t[0]);
		writeP6("g.ppm", t[1]);
		writeP6("b.ppm", t[2]);
		cout << "ho scritto r" << endl;
		apriImmagine("r.ppm");
		apriImmagine("g.ppm");
		apriImmagine("b.ppm");

	}
}