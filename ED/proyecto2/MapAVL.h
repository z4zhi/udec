#ifndef MAPAVL_H
#define MAPAVL_H

#include "Map.h"

struct rama{
	string str;
	int n, h = 1;
	rama *padre = nullptr;
	rama *izq = nullptr;
	rama *der = nullptr;
};

class MapAVL : public Map{
	private:
		rama *raiz;
		int tam;
		
		void rotacion(rama *r);
		void rotacionSimple(rama *a, rama *b, rama *c, int x);
		void insertRecursivo(pair<string, int>, rama *z);
		void borrarArbol(rama *r);
		void recorrer(rama *r);
		void eraseRecursivo(string s, rama *r);
		pair<string,int> predecesor(rama *r);

	public:
		MapAVL();
		~MapAVL();
		void insert(pair<string, int>);
		void erase(string);
		int at(string);
		int size();
		bool empty();

};

#endif
