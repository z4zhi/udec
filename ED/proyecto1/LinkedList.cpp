#include <bits/stdc++.h>
#include "LinkedList.h"

LinkedList::LinkedList() {
  head = new nodo();
	head->siguiente = NULL; // head NO contiene el primer elemento
	size = 0;
  espacio = sizeof(head);
}

LinkedList::~LinkedList() {
  nodo *aux = head;
	while(head != NULL){
		aux = head->siguiente;
		delete head;
		head = aux;
	}
  size = 0;
}

void LinkedList::insert(int n) {
  // desordenado
  // if (!search(n)) {
  //   nodo *node = new nodo();
  // 	node->n = n;
  // 	node->siguiente = head->siguiente;
  // 	head->siguiente = node;
  // 	size++;
  // }
  // ordenado
  nodo *sig = head->siguiente;
  nodo *anterior = head;
  while(sig != NULL && sig->n < n) {
    anterior = sig;
    sig = sig->siguiente;
  }
  if (((sig != NULL && sig->n > n) || sig == NULL)  && anterior->n < n) {
    nodo *node = new nodo();
    node->n = n;
    node->siguiente = sig;
    anterior->siguiente = node;
    size++;
    espacio += sizeof(node);
  }
}

void LinkedList::remove(int n) {
  nodo *node = head;
	for (int i = 0; i < size; i++) {
    if (node->siguiente->n == n) {
      espacio -= sizeof(node);
      nodo *aux = node->siguiente->siguiente;
      delete node->siguiente;
      node->siguiente = aux;
      size--;
      break;
    }
		node = node->siguiente;
  }
}

bool LinkedList::search(int n) {
  nodo *node = head;
	for (int i = 0; i < size; i++) {
		node = node->siguiente;
    if (node->n == n) return true;
  }
	return false;
}

int LinkedList::space() {
  return espacio;
}
