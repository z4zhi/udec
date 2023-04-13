#include "MapAVL.h"

MapAVL::MapAVL() {
	raiz = nullptr;
	tam = 0;
}

MapAVL::~MapAVL() {
	borrarArbol(raiz);
}

void MapAVL::borrarArbol(rama *r){
	if(r == nullptr) return;
	borrarArbol(r->izq);
	borrarArbol(r->der);
	delete r;
	r = nullptr;
}

void MapAVL::insert(pair<string, int> p) {
	if(empty()){
		raiz = new rama();
		raiz->str = p.first;
		raiz->n = p.second;
		tam++;
	}else insertRecursivo(p, raiz);
}

void MapAVL::insertRecursivo(pair<string, int> p, rama *r){
	rama *nueva = nullptr;
		
	if(p.first < r->str){
		if(r->izq == nullptr){
			r->izq = new rama();
			nueva = r->izq;
		}else insertRecursivo(p, r->izq);

	}else if(p.first > r->str){
		if(r->der == nullptr){
			r->der = new rama();
			nueva = r->der;
		}else insertRecursivo(p, r->der);

	}else r->n = p.second;

	if(nueva != nullptr){
		nueva->str = p.first;
		nueva->n = p.second;
		nueva->padre = r;
		tam++;	
	}

	int h1 = 0, h2 = 0;
	if(r->der != nullptr) h1 = r->der->h;
	if(r->izq != nullptr) h2 = r->izq->h;
	r->h = 1 + max(h1, h2);

	if(abs(h1 - h2) > 1) rotacion(r);
}

void MapAVL::rotacion(rama *a){
	rama *b, *c;
	int x, y;

	if(a->izq == nullptr) x = 1;
	else if(a->der == nullptr) x = -1;
	else if(a->izq->h > a->der->h) x = -1;
	else x = 1;
	if(x == -1) b = a->izq;
	else b = a->der;

	if(b->izq == nullptr) y = 1;
	else if(b->der == nullptr)  y = -1;
	else if(b->izq->h > b->der->h)  y = -1;
	else y = 1;
	if(y == -1) c = b->izq;
	else c = b->der;

	if(x == y) rotacionSimple(a, b, c, x);
	else{
		if(x == -1){
			a->izq = c;
			c->padre = a;
			b->padre = c;
			b->der = c->izq;
			if(c->izq != nullptr) c->izq->padre = b;
			c->izq = b;
		}else{
			a->der = c;
			c->padre = a;
			b->padre = c;
			b->izq = c->der;
			if(c->der != nullptr) c->der->padre = b;
			c->der = b;
		}

		int h1 = 0, h2 = 0;
		if(b->izq != nullptr) h1 = b->izq->h;
		if(b->der != nullptr) h2 = b->der->h;
		b->h = 1 + max(h1, h2);

		rotacionSimple(a, c, b, x);
	}
}

void MapAVL::rotacionSimple(rama *a, rama *b, rama *c, int x){
	if(a == raiz){
		raiz = b;
		b->padre = nullptr;
	}else{
		b->padre = a->padre;
		if(a->padre->der == a) a->padre->der = b;
		else a->padre->izq = b;
	}

	a->padre = b;
	if(x == 1){
		a->der = b->izq;
		if(a->der != nullptr) a->der->padre = a;
		b->izq = a;	
	}else{
		a->izq = b->der;
		if(a->izq != nullptr) a->izq->padre = a;
		b->der = a;
	}

	rama *aux = a;
	while(aux != nullptr){
		int h1 = 0, h2 = 0;
		if(aux->izq != nullptr) h1 = aux->izq->h;
		if(aux->der != nullptr) h2 = aux->der->h;
		aux->h = 1 + max(h1, h2);
		aux = aux->padre;
	}
}

void MapAVL::erase(string s) {
	if(raiz != nullptr) eraseRecursivo(s, raiz);
}

void MapAVL::eraseRecursivo(string s, rama *r){		
	if(s < r->str && r->izq != nullptr) eraseRecursivo(s, r->izq);
	else if(s > r->str && r->der != nullptr) eraseRecursivo(s, r->der);
	else{
		tam--;
		bool izq = 0, der = 0;
		if(r->der != nullptr) izq = 1;
		if(r->izq != nullptr) der = 1;

		if(!izq && !der){
			if(r != raiz){
				if(r->padre->izq == r) r->padre->izq = nullptr;
				else r->padre->der = nullptr;
			}else raiz = nullptr;
			delete r;
			return;

		}else if(izq && !der){
			if(r != raiz){
				if(r->padre->izq == r){
					r->padre->izq = r->izq;
					r->izq->padre = r->padre;
				}else{
					r->padre->der = r->izq;
					r->izq->padre = r->padre;
				}
			}else{
				raiz = r->izq;
				r->izq->padre = nullptr;
			}
			delete r;
			return;

		}else if(!izq && der){
			if(r != raiz){
				if(r->padre->izq == r){
					r->padre->izq = r->der;
					r->der->padre = r->padre;
				}else{
					r->padre->der = r->der;
					r->der->padre = r->padre;
				}
			}else{
				raiz = r->der;
				r->der->padre = nullptr;
			}
			delete r;
			return;
		}else{
			pair<string,int> p = predecesor(r->izq);
			r->str = p.first;
			r->n = p.second;
		}
	}

	int h1 = 0, h2 = 0;
	if(r->der != nullptr) h1 = r->der->h;
	if(r->izq != nullptr) h2 = r->izq->h;
	r->h = 1 + max(h1, h2);
	if(abs(h1 - h2) > 1) rotacion(r);
}

pair<string,int> MapAVL::predecesor(rama *r){
	pair<string,int> p;

	if(r->der != nullptr) p = predecesor(r->der);
	else{
		p = {r->str, r->n};
		if(r->padre->izq == r) r->padre->izq = nullptr;
		else r->padre->der = nullptr;
		delete r;
		return p;
	}

	int h1 = 0, h2 = 0;
	if(r->der != nullptr) h1 = r->der->h;
	if(r->izq != nullptr) h2 = r->izq->h;
	r->h = 1 + max(h1, h2);

	return p;
}

int MapAVL::at(string s) {
	rama *r = raiz;
	while(r != nullptr){
		if(s < r->str) r = r->izq;
		else if(s > r->str) r = r->der;
		else return r->n;
	}
	throw out_of_range("not in the map");
}

int MapAVL::size() {
	return tam;
}

bool MapAVL::empty() {
	return !tam;
}

void MapAVL::recorrer(rama *r){
	if(r == nullptr) return;
	recorrer(r->izq);

	printf("STR: %5s  N: %2d  |    H: %2d", r->str.c_str(), r->n, r->h);
	if(r->padre != nullptr) printf("   PADRE: %5s\n", r->padre->str.c_str());
	else printf("\n");

	recorrer(r->der);
}
