#include "lz77.h"
#include "support.h"
#include <algorithm>
#include <iterator>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

/*Integer to String*/
string i2s(int num){
	stringstream s;
	s << num;
	return s.str();
}

string scriviTerna(int pos, int lun, char car){

	//suppongo che un byte basti per memorizzare posizione e lunghezza. Quindi max 255!
	cout << "Scrivo (" << pos << "," << lun << "," << car << ")" << endl;
	string res;
	res.append("(");
	res.append(i2s(pos));	//posizione
	res.append(",");
	res.append(i2s(lun));	//lunghezza
	res.append(",");
	res.append(i2s(car));	//carattere
	res.append(")");
	return res;
}

lz77_coder::lz77_coder(){}

lz77_coder::lz77_coder(std::string file){
	std::fstream fs;
	fs.open(file, std::fstream::in /*| std::fstream::binary*/);
	fs >> std::noskipws;
	std::istream_iterator<unsigned char> stream_it(fs);
	std::istream_iterator<unsigned char> eof;

	std::copy(stream_it, eof, back_inserter(_data));

	std::cout << "testo originale: " << _data.data() << endl << endl;
}

lz77_coder::lz77_coder(std::vector<unsigned char> data) : _data(data) {}

void lz77_coder::read_data_from_file(std::string file){
	std::fstream fs;
	fs.open(file, std::fstream::in | std::fstream::binary);
	std::istream_iterator<unsigned char> stream_it(fs);
	std::istream_iterator<unsigned char> eof;

	std::copy(stream_it, eof, back_inserter(_data));
}

void lz77_coder::codify_data(){
	std::string tmp;
	std::string dictionary;
	unsigned actual_pos;
	unsigned effective_actual_pos;
	int L = 0;
	int P = 0;
	unsigned char C;
	bool string_maxed = 0;
	bool dictionary_maxed = 0;

	//Ciclo di codifica
	for (unsigned i = 0; i<_data.size(); i++){
		if (!string_maxed){
			tmp.push_back(_data[i]);
			if (tmp.size() == 1){
				actual_pos = i;
				effective_actual_pos = i;
				if (dictionary_maxed)
					actual_pos = 256;
			}
			size_t pos = dictionary.rfind(tmp);
			if (i - effective_actual_pos >= 256)
				string_maxed = 1;
			if ((pos == std::string::npos) || (i == _data.size() - 1)){
				string_maxed = 1;
			}
			else{
				P = actual_pos - pos;

				//Gestione del caso in cui si arrivi alla fine del dizionario e si riparta con il carattere puntato da P
				if (P == tmp.size()){							//Entra soltanto se la stringa trovata nel dizionario e' alla fine del dizionario
					tmp.push_back(_data[i + 1]);					//Inserisco il valore successivo e controllo che la stringa cosi creata non sia gia presente nel dizionario
					if (dictionary.rfind(tmp) == std::string::npos){	//Se non e' presente allora
						tmp.pop_back();							//Tolgo l'elemento che ho inserito per il controllo precedente
						bool repeated = 1;
						std::string sub_dictionary = tmp;			//Creo un sottodizionario composto dalla sola sottostringa che dovrebbe potersi ripetere
						int sub_count = 0;						//Contatore che mi permette di tenere conto del punto in cui mi trovo nella sottostringa
						while (repeated){									//Ciclo
							i++;
							if (i - actual_pos >= 256)
								string_maxed = 1;
							tmp.push_back(_data[i]);							//Inserisco il valore successivo
							if (tmp.back() != sub_dictionary[sub_count]){	//Se il valore appena aggiunto e' diverso dall'elemento che dovrebbe corrispondere nel sottodizionario, allora la stringa e' completa e posso mandarla il output
								repeated = 0;
								string_maxed = 1;
							}
							if (i == _data.size() - 1){						//Controllo di non essere arrivato alla fine dei miei dati, e in tal caso setto i flag cosi da poter uscire dal ciclo e da mandare l'ultimo output
								repeated = 0;
								string_maxed = 1;
							}
							sub_count++;									//Incremento il contatore del sottodizionario cosi che punti all'elemento su cui dovro' fare il controllo al prossimo ciclo
							if (sub_count == sub_dictionary.size())			//Se serve azzero il contatore del sottodizionario cosi che questo possa ripetersi senza sforare oltre alla sua dimensione possibile
								sub_count = 0;
						}
					}
					else
						tmp.pop_back();				//Nel caso in cui il controllo non vada a buon fine e che percio' nel dizionario sia presente una stringa formato dalla stringa analizzata piu il carattere successivo, tolgo comunque il carattere inserito temporaneamente per quel controllo
				}
				//Fine gestione caso eccezionale
			}
		}
		//Gestione della stringa da mandare in output e da aggiungere al dizionario
		if (string_maxed){
			C = tmp.back();
			L = tmp.size() - 1;
			dictionary.append(tmp);
			if (dictionary.size() >256){
				//Se il dizionario supera i 256 caratteri aggiungendo la parte nuova viene shiftato cancellando le parti piu lontane e lasciando la dimensione a 256
				dictionary_maxed = 1;
				std::string dictionary_new;

				for (unsigned i = dictionary.size() - 256; i<dictionary.size(); i++){
					dictionary_new.push_back(dictionary[i]);
				}

				dictionary = dictionary_new;
			}

			tmp.clear();
			string_maxed = 0;
			//std::string result;
			//Composizione della stringa di output
			//result.append("(");
			//result.append(convert_int_to_string(P));
			//result.append(",");
			//result.append(convert_int_to_string(L));
			//result.append(",");
			//result.push_back(C);
			//result.append(")\n");
			//Inserisco la stringa in un vector cosi da poterlo mandare in output in modo diverso

			_codified.push_back(scriviTerna(P,L,C));
		}
	}
}

void lz77_coder::codify_in_console(){
	codify_data();
	std::vector<std::string>::iterator it = _codified.begin();
	while (it != _codified.end()){
		std::cout << (*it);
		++it;
	}
	cout << endl;
}

void lz77_coder::codify_in_file(std::string file){
	std::fstream fout;
	codify_data();
	fout.open(file, std::fstream::out);
	if (fout.fail()){
		std::cout << "Errore creazione file per scrittura\n";
	}
	std::vector<std::string>::iterator it = _codified.begin();
	while (it != _codified.end()){
		fout << (*it);
		++it;
	}
	fout.close();
}

