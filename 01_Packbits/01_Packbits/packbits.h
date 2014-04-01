#ifndef PACKBITS_H
#define PACKBITS_H

#include <vector>

class packbits{

	vector<char> _prev;

public:
	packbits();
	~packbits();

	void encode(istream &, ostream &);
	void decode(istream &, ostream &);

private:
	void output_run(ostream &);
	void output_nonrun(ostream &);
};

#endif