#ifndef LZW_H
#define LZW_H

class lzw{

public:
	// Comprimi una stringa da una lista di simboli.
	// il risultato viene scritto sull'iteratore
	// partendo da "result"; viene ritornato l'it finale.
	template <typename Iterator>
	Iterator compress(const string &uncompressed, Iterator result) {
		// costruisco il dizionario.
		int dictSize = 256;
		map<string, int> dictionary;
		for (int i = 0; i < 256; i++)
			dictionary[string(1, i)] = i;

		string w;
		for (string::const_iterator it = uncompressed.begin();
			it != uncompressed.end(); ++it) {
			char c = *it;
			std::string wc = w + c;
			if (dictionary.count(wc))
				w = wc;
			else {
				*result++ = dictionary[w];
				// Aggiungo wc al dizionario.
				dictionary[wc] = dictSize++;
				w = string(1, c);
			}
		}

		// ritorna il codice di w.
		if (!w.empty())
			*result++ = dictionary[w];
		return result;
	}

	// decomprimo una lista di output ks in stringa.
	// "begin" e "end" devono stare nel range
	// decomprimo una lista di output ks in stringa.
	// "begin" e "end" devono stare nel range
	template <typename Iterator>
	string decompress(Iterator begin, Iterator end) {
		// Costruisco il dizionario.
		int dictSize = 256;
		map<int, string> dictionary;
		for (int i = 0; i < 256; i++)
			dictionary[i] = string(1, i);

		string w(1, *begin++);
		string result = w;
		string entry;
		for (; begin != end; begin++) {
			int k = *begin;
			if (dictionary.count(k))
				entry = dictionary[k];
			else if (k == dictSize)
				entry = w + w[0];
			else
				throw "Errore compressione k";

			result += entry;

			// Aggiungi w+entry[0] al dizionario.
			dictionary[dictSize++] = w + entry[0];

			w = entry;
		}
		return result;
	};

	string decompress(Iterator begin, Iterator end) {
		// Costruisco il dizionario.
		int dictSize = 256;
		map<int, string> dictionary;
		for (int i = 0; i < 256; i++)
			dictionary[i] = string(1, i);

		string w(1, *begin++);
		string result = w;
		string entry;
		for (; begin != end; begin++) {
			int k = *begin;
			if (dictionary.count(k))
				entry = dictionary[k];
			else if (k == dictSize)
				entry = w + w[0];
			else
				throw "Errore compressione k";

			result += entry;

			// Aggiungi w+entry[0] al dizionario.
			dictionary[dictSize++] = w + entry[0];

			w = entry;
		}
		return result;
	}
};
#endif