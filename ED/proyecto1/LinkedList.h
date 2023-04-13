#include "MiniSet.h"

struct nodo {
	int n;
	nodo *siguiente;
};

class LinkedList: public MiniSet {
	private:
		struct nodo *head;
		int size;
		int espacio;
	public:
		LinkedList();
		~LinkedList();
		void insert(int n);
		void remove(int n);
		bool search(int n);
		int space();
};
