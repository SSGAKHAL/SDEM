#ifndef PACKBITS_H
#define PACKBITS_H

#include <vector>
#include <fstream>

class packbits{

	/*Un vector di char che funge da buffer dei caratteri letti diversi*/
	vector<char> _prev;

public:
	/*Costruttore*/
	packbits();
	/*Distuttore*/
	~packbits();

	/*Codifica*/
	void encode(istream &, ostream &);
	/*Decodifica*/
	void decode(istream &, ostream &);

private:
	/*Codice che viene eseguito quando c'� una run*/
	void output_run(ostream &);
	/*Codice che viene eseguito quando c'� una NONrun*/
	void output_nonrun(ostream &);
};

#endif