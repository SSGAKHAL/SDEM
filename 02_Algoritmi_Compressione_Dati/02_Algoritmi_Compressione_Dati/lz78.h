#ifndef LZ_78_H
#define LZ_78_H

#include<fstream>
#include<string>

class lz78{
private:
	void put(int diz, int next, std::fstream &out);
public:
	void codificalz78(string name, int lung);
	void decodificalz78(string name, int lung);
};

#endif