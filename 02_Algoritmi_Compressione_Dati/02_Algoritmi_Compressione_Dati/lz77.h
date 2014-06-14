#ifndef LZ77_H
#define LZ77_H

#include <vector>
#include <string>

class lz77_coder{
	std::vector<unsigned char> _data;
	std::vector<std::string> _codified;

public:

	lz77_coder();

	lz77_coder(std::string file);

	lz77_coder(std::vector<unsigned char> data);

	void read_data_from_file(std::string file);

private:

	void codify_data();

public:
	void codify_in_console();

	void codify_in_file(std::string file);
};

#endif