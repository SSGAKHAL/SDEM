#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <deque>

using namespace  std;
typedef unsigned char byte;

struct item{
	byte simbolo;
	unsigned frequenza;

	unsigned nbit;
	unsigned codice;

public:
	item(byte sim, unsigned freq) : simbolo(sim), frequenza(freq), nbit(0), codice(0){}

	void incr(){ frequenza++; }
	void setNbit(unsigned i){ nbit=i; }
	void setCodice(unsigned i){ codice=i; }

	bool operator<(item x){
		return x.frequenza < frequenza;
	}
};

void stampaOccorrenze(vector<item> vec){

	cout << "Simbolo \t Occorrenza" << endl;
	for (auto a : vec){
		cout << a.simbolo << " \t" << a.frequenza << endl;
	}

}

struct node{


	unsigned _valore;
	node* _left;
	node* _right;

	/*Solo per le foglie*/
	unsigned _posizione;

	unsigned nbit;
	unsigned codice;
public:

	node(unsigned valore, node* left, node* right) : _valore(valore), _left(left), _right(right), nbit(0), codice(0), _posizione(0){}
	node(unsigned valore, unsigned posizione) : 
		_valore(valore), 
		nbit(0), 
		codice(0), 
		_left(nullptr), 
		_right(nullptr), 
		_posizione(posizione) {}

	bool isLeaf(){ return (_left == nullptr); }

	bool operator==(unsigned x){
		return x == _valore;
	}

	bool operator<(node x){
		return x._valore >= _valore;
	}
};


void stampaLista(deque<node> list){

	for (auto a : list)
		cout << a._valore << " ";
	cout << endl;

}

/*Il pervchè la uso si capisce quando uso find_if, per ora ignorala*/
struct MyClassComp{

private:
	int n;
public:
	MyClassComp(int i) :  n(i) { }	//il costruttore prendere l'intero che voglio confrontare
	
	bool operator()(node& m) const { return n > m._valore;}	//confronto se il valore di questa classe è uguale al valore del nodo dato
};

void scorriAlberAlContrario(node* nodo, unsigned nbitPadre, unsigned codicePadre, vector<item>& vettTotale){

	/*Se sono una foglia, esci!*/
	if (nodo->isLeaf()){
		cout << "Sono una foglia (posizione=" << nodo->_posizione << ") porca troia! e allora codice " << codicePadre << " però a " << nbitPadre << " bit!" << endl;
		vettTotale[nodo->_posizione].codice = codicePadre;
		vettTotale[nodo->_posizione].nbit = nbitPadre;

		return;
	}

	cout << "Sono il nodo con valore " << nodo->_valore << ", figlioDX= " << nodo->_right->_valore << " e figlio SX=" << nodo->_left->_valore;

	/*A quello a sinistra di il valore 1*/
	unsigned leftCode = codicePadre << 1 | 0;

	/*A quello a destra do il valore 0*/
	unsigned rightCode = codicePadre << 1 | 1;

	unsigned nuovoNumBit = nbitPadre+1;

	scorriAlberAlContrario(nodo->_left, nuovoNumBit, leftCode, vettTotale);

	scorriAlberAlContrario(nodo->_right, nuovoNumBit, rightCode, vettTotale);

}

int main(){

	cout << "Che due palle" << endl;

	ifstream is("provahuf.txt", ios::binary);
	if (!is)
		return -1;

	vector<byte> valoriOriginali;

	char ch;
	while (is.get(ch)){
		valoriOriginali.push_back((byte)ch);
	}

	cout << "caricato il file, contiene " << valoriOriginali.size() << " simboli" << endl;

	vector<byte> tmp(255);	//max valore di un byte

	for (unsigned i = 0; i < valoriOriginali.size(); ++i){
		tmp[valoriOriginali[i]]++;
	}

	/*Ho tutte le occorrenze*/
	vector <item> tmp2;

	/*Popolo il vettore di item con tutti i valori che compaiono almeno una volta*/
	for (unsigned i = 0; i < tmp.size(); ++i){
		if (tmp[i] != 0){
			item it(i, tmp[i]);
			tmp2.push_back(it);
		}
	}

	//stampaOccorrenze(tmp2);

	/*Ordino il vettore*/
	sort(tmp2.begin(), tmp2.end());

	stampaOccorrenze(tmp2);

	deque<node> tree;

	for (unsigned i = 0; i < tmp2.size(); ++i){
		node n(tmp2[i].frequenza, i);
		tree.push_back(n);
	}

	for (unsigned i = 0; i < tmp2.size()-1; ++i){

		/*Nodo che vado a creare come somma dei primi due*/
		
		unsigned somma = 0;
		somma = tree[tree.size() - 2]._valore + tree[tree.size() - 1]._valore;
		cout << "Prendo gli ultimi due: la somma fa: " << somma << endl;
		node n(somma, &tree[tree.size() - 2], &tree[tree.size() - 1]);

		/*Cancello l'ultimo elemento*/
		//tree.pop_back();

		/*E il penultimo*/
		//tree.pop_back();

		tree.emplace_back();
		tree.emplace_back();

		/*Dove lo metto*/
		auto a = find_if(tree.begin(), tree.end(), MyClassComp(somma));

		//cout << "E allora vado ad inserirlo in posozione: " << a << endl;
		tree.insert(a, n);

		tree.push_back(n);
		//sort(tree.begin(),tree.end)

		stampaLista(tree);

		cout << endl;
	}

	/*Bene, ma ora bisogna costruire*/
	scorriAlberAlContrario(&tree.front(),0,0,tmp2);


}