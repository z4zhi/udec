#include <bits/stdc++.h>
#include "SkipList.h"
using namespace std;

SkipList::SkipList() {
	for(int i=0; i<niveles; i++) nivel[i] = new nodoSkip();
	for(int i=0; i<niveles-1; i++) nivel[i+1]->sigNiv = nivel[i];
	size = 0;

	// //tests
	// struct timespec ts;
	// clock_gettime(CLOCK_MONOTONIC, &ts);
	// srand((time_t)ts.tv_nsec);
	// for(int i=0; i<20; i++) insert(rand()%100);
	//
	// nodoSkip *a;
	// insert(1);
	// insert(53);
	// insert(17);
	// for(int i=niveles-1; i>=0; i--){
	// 	a = nivel[i]->sig;
	// 	while(a != nullptr){
	// 	cout << a->n << " ";
	// 	a = a->sig;
	// 	}
	// 	cout << endl;
	// }
	//
	// remove(1);
	// remove(53);
	// remove(17);
	//
	// for(int i=niveles-1; i>=0; i--){
	// 	a = nivel[i]->sig;
	// 	while(a != nullptr){
	// 	cout << a->n << " ";
	// 	a = a->sig;
	// 	}
	// 	cout << endl;
	// }
	//tests
}

SkipList::~SkipList() {
	nodoSkip *actual, *anterior;
	for(int i=0; i<niveles; i++){
		actual = nivel[i];
		while(actual->sig != nullptr){
			anterior = actual;
			actual = actual->sig;
			delete anterior;
		}
		delete actual;
	}
}

void SkipList::insert(int n) {
	if(!search(n)){
		struct timespec ts;
		clock_gettime(CLOCK_MONOTONIC, &ts);
		srand((time_t)ts.tv_nsec);

		int nivNodo = 0;

		for(int i=0; i<niveles-1; i++){
			if(rand()%2) nivNodo++;
			else break;
		}

		nodoSkip *nodo = new nodoSkip(), *aux;
		nodo->n = n;
		aux = nodo;
		for(int i=0; i<nivNodo; i++){
			nodoSkip *copia = new nodoSkip();
			copia->n = n;
			aux->sigNiv = copia;
			aux = aux->sigNiv;
		}

		nodoSkip *actual = nivel[niveles-1];
		int nivActual = niveles-1;
		while(nivActual > nivNodo){
			if(actual->sig != nullptr && n > actual->sig->n){
				actual = actual->sig;
			}else{
				actual = actual->sigNiv;
				nivActual--;
			}
		}

		while(true){
			if(actual->sig != nullptr){
				if(n > actual->sig->n) actual = actual->sig;
				else{
					nodo->sig = actual->sig;
					actual->sig = nodo;
					if(nodo->sigNiv != nullptr){
						nodo = nodo->sigNiv;
						actual = actual->sigNiv;
					}else break;
				}
			}else{
				actual->sig = nodo;
				if(nodo->sigNiv != nullptr){
					nodo = nodo->sigNiv;
					actual = actual->sigNiv;
				}else break;
			}
		}
		size++;
	}
}

void SkipList::remove(int n) {
	nodoSkip *actual = nivel[niveles-1], *aux;

	while(true){
		if(actual->sig != nullptr){
			if(n == actual->sig->n){
				aux = actual->sig->sig;
				delete actual->sig;
				actual->sig = aux;
				if(actual->sigNiv != nullptr) actual = actual->sigNiv;
			}
			else if(n > actual->sig->n) actual = actual->sig;
			else if(actual->sigNiv != nullptr) actual = actual->sigNiv;
			else break;

		}else if(actual->sigNiv != nullptr) actual = actual->sigNiv;
		else break;
	}
}

bool SkipList::search(int n) {
	nodoSkip *actual = nivel[niveles-1];

	while(true){
		if(actual->sig != nullptr){
			if(n == actual->sig->n) return true;
			else if(n > actual->sig->n) actual = actual->sig;
			else if(actual->sigNiv != nullptr) actual = actual->sigNiv;
			else return false;

		}else if(actual->sigNiv != nullptr) actual = actual->sigNiv;
		else return false;
	}
}
