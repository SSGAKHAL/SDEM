#include <string>

class Auto{
private:
	
	size_t _cilindrata;
	std::string _nome;
	float _consumoLitriPerCentoKilometri;

	std::string _colore;

public:
	Auto(size_t, std::string, float);
	~Auto();

	/*Getter*/
	size_t getCilindrata(){ return _cilindrata; }					//inline = dichiarata e implementata
	std::string getNome(){ return _nome; }							//inline = dichiarata e implementata
	float getConsumo(){ return _consumoLitriPerCentoKilometri; }	//inline = dichiarata e implementata

	/*Setter*/
	void setColore(std::string col){ _colore = col; }				//inline = dichiarata e implementata

	/*Altre funzioni*/
	float quantiKmConLitri(size_t);

	float quantiEuroCostanoKm(float, float);
};