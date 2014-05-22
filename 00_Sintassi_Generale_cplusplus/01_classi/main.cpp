#include <iostream>
#include "Auto.h"

using namespace std;

int main(){
	cout << "Inizio del programma!" << endl;

	/*Creo oggetto Auto con i valori dati. L'oggetto si chiama a*/
	Auto a(1800, "Honda Civic", 7.0);

	
	/*For con passo 10. Invece di i, uso direttamente float*/
	cout << "Ho creato la macchina " << a.getNome() << " di cilindrata " << a.getCilindrata() << endl;

	float costoLitroBenzina = 1.625;
	for (float litri = 0; litri < 80; litri = litri + 10){
		float kmConLitri = a.quantiKmConLitri(litri);
		cout << "Con " << litri << " litri fa ben " << kmConLitri << " chilometri, cioè spendo " << a.quantiEuroCostanoKm(kmConLitri, costoLitroBenzina) << " euro!" << endl;
	}

	/*Il programma sta per finire: verrà invocato il distruttore di Auto a */
	cout << "Fine programma!" << endl;
}