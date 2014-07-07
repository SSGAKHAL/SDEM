#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <vector>
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>
float quant = 1000000000;
using namespace std;
void trasformata(vector<short>& sample, vector<double>& trasformato){
	for (size_t j = 0; j < sample.size() - 512; j += 512)
	{
		//vector<short> tmp;
		//tmp.insert(sample.begin(), sample.begin() + (j+512), sample.begin() )

		for (size_t i = 0; i < 512; i++)
		{
			double xk = 0.0;
			for (size_t k = 0; k < 1024; k++)
			{
				xk += (sample[k + j] * sin((M_PI / (2. * 512))*(k + 0.5))*cos((M_PI / 512)*(k + 0.5 + 512 / 2.)*(i + 0.5)));
			}
			trasformato.push_back(xk / quant);
		}
	}
}

void antitrasformata(vector<double>& trasformato, vector<double>& antitrasformato){

	for (size_t j = 0; j < antitrasformato.size() - 512; j += 512)
	{
		for (size_t i = 0; i < 1024 - 1; i++)
		{
			double xn = 0.0;
			for (size_t k = 0; k < 512 - 1; k++)
			{
				xn += (trasformato[k + j] * sin((M_PI / (2. * 512))*(i + 0.5))*cos((M_PI / 512)*(i + 0.5 + 512 / 2.)*(k + 0.5)));
			}
			xn *= 2.0 / 512;
			xn = xn*quant;
			antitrasformato[i + j] += xn;
		}
	}
}

int main(){



}


int main2(){

	ifstream is("prova_mdct.raw", ios::binary);
	if (!is)
		return false;

	is.seekg(0, ios::end);
	unsigned file_size = unsigned(is.tellg());
	is.seekg(0, ios::beg);
	unsigned nSamples = file_size / 2;
	vector<short> sample(nSamples);
	is.read(reinterpret_cast<char *>(sample.data()), file_size);


	// Aggiungo zeri per riempire vettore

	const int padd = nSamples % 512;
	vector<short> padding(512);
	vector<short> padded(512 - padd);
	sample.insert(sample.begin(), padding.begin(), padding.end());
	sample.insert(sample.end(), padded.begin(), padded.end());
	sample.insert(sample.end(), padding.begin(), padding.end());
	vector<double> trasformato;


	trasformata(sample, trasformato);

	vector<double> antitrasformato(trasformato.size() + 512);

	antitrasformata(trasformato, antitrasformato);

	vector<short> output(antitrasformato.size() - 2 * 512);
	for (unsigned i = 512; i < antitrasformato.size() - 512; ++i) {
		double d = antitrasformato[i];
		output[i - 512] = static_cast<short>(d>0 ? d + 0.5 : d - 0.5);
	}

	ofstream os("mdctquant.raw", ios::binary);
	if (!os)
		return false;
	os.write(reinterpret_cast<const char*>(output.data()), output.size() * sizeof(output[0]));
}