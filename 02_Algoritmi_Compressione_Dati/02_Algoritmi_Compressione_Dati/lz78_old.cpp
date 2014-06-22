#include "lz78.h"
#include <iostream>
#include <vector>
#include "support.h"
#include <map>

using namespace std;

void put(int diz, int next, fstream &out)
{
	unsigned d = (unsigned)diz;
	unsigned n = (unsigned)next;

	out.put(diz);
	out.put(next);

	cout << (int)diz << " " << (char)next << endl;
}

void codificalz78(string name, int lunghezzadiz)
{
	vector<byte> dati;

	fstream is(name, ios::in);
	if (!is){cout << "errore"; return;}

	while (true)
	{
		if (is.eof())
			break;
		byte a = is.get();
		int p = a;
		dati.push_back(p);

	}
	if (dati.back() == 255)
		dati.pop_back();

	is.close();

	fstream out("coded.bin", ios::out);

	int nelemdiz = 0;
	map<int, vector<byte>> mappa;

	for (int i = 0; i<dati.size();)
	{

		vector<byte> testo;
		//copio il resto della stringa in un nuovo vector
		for (int j = i; j<dati.size(); j++)
		{
			testo.push_back(dati[j]);
		}

		if (nelemdiz>lunghezzadiz)
		{
			nelemdiz = 0;
			mappa.clear();
		}

		if (i>0)
		{
			int maxcount = 0; int indocc = 0;
			int j = 1;
			while (j <= nelemdiz)
			{
				//x ogni elem del dizionario
				int p = 0; int finito = 1; int q = i; int count = 0;
				while (p<mappa[j].size() && finito &&p<testo.size())
				{
					if (mappa[j][p++] == dati[q++])
					{
						count++;
					}
					else
					{
						finito = 0;
					}
				}

				if (count>0 & count>maxcount)
				{
					maxcount = count;
					indocc = j;
				}
				j++;


			}

			if (maxcount == 0)
			{
				mappa[++nelemdiz].push_back(dati[i]);
				put(0, dati[i], out);
				i++;
			}
			else
			{
				mappa[++nelemdiz] = mappa[indocc];
				mappa[nelemdiz].push_back(dati[i + mappa[indocc].size()]);
				put(indocc, dati[i + mappa[indocc].size()], out);
				i += mappa[nelemdiz].size();
			}

		}
		else
		{
			mappa[++nelemdiz].push_back(dati[i]);
			put(nelemdiz - 1, dati[i + 1], out);
			i++;
		}
	}

	out.close();
}


void decodificalz78(string name, int n)
{
	vector<unsigned char> dati;

	fstream is(name, ios::in);

	if (is.fail())
		cout << "errore";

	while (true)
	{
		if (is.eof())
			break;
		byte a = is.get();
		int p = a;
		dati.push_back(p);
	}

	if (dati.back() == 255)
		dati.pop_back();

	is.close();

	vector<unsigned char> decoded;
	vector<unsigned char> olddecoded;

	map<int, vector<unsigned char>> mappa;

	int nelemdiz = 0;
	for (int i = 0; i<dati.size(); i += 2)
	{
		if (nelemdiz>n)
		{
			nelemdiz = 0;
			mappa.clear();
		}
		if (dati[i] != 0)
		{
			int p = dati[i];
			for (int j = 0; j<mappa[p].size(); j++) // copio la roba nel vettore di decodifica
			{
				decoded.push_back(mappa[p][j]);
			}
			// faccio nuovo elem della mappa
			mappa[++nelemdiz] = mappa[p];
			mappa[nelemdiz].push_back(dati[i + 1]);
			decoded.push_back(dati[i + 1]);
		}
		if (dati[i] == 0) // è un nuovo car da mettere nel diz
		{
			mappa[++nelemdiz].push_back(dati[i + 1]);
			decoded.push_back(dati[i + 1]);
		}
	}

}