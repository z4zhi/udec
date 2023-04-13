#include <bits/stdc++.h>
#include "MapSV.h"
using namespace std;

MapSV::MapSV() {

}

MapSV::~MapSV() {

}

void MapSV::insert(pair<string, int> p) {
  // elemento repetido
  int i = binariaRecursiva(p.first, 0, v.size()-1); //buscar si esta el elemento
  if (i != -1) {
    v[i] = p;
    return;
  }
  // si no es elemento repetido
  v.push_back(p); // insertar al final
  int j = v.size() - 1; // posicion ultimo elemento
  // el ciclo recorre desde el final hasta el indice donde deba ser insertado
  while (j > 0 && v[j].first < v[j-1].first) { // mientras el anterior sea mayor al siguiente
    v[j] = v[j-1];
    v[j-1] = p;
    j--;
  }
}

void MapSV::erase(string s) {
  int i = binariaRecursiva(s, 0, v.size()-1); //buscar indice del elemento
  if (i == -1) return;
  // el ciclo mueve todos los elementos desde "i" hasta "size-1" a la izquierda
  for ( i; i < v.size()-1; i++)
    v[i] = v[i+1];
  v.erase(v.begin() + v.size()-1);
}

int MapSV::at(string s) {
  int i = binariaRecursiva(s, 0, v.size()-1); //buscar indice del elemento
  if (i == -1) return i;
  return v[i].second;
}

int MapSV::size() {
  return v.size();
}

bool MapSV::empty() {
  return v.empty();
}

int MapSV::binariaRecursiva(string s, int l, int r){
	if (r >= l){
		int m = (r + l) / 2;
		if (v[m].first == s) return m;
		else if (v[m].first < s) return binariaRecursiva(s, m + 1, r);
		else if (v[m].first > s) return binariaRecursiva(s, l, m - 1);
	}
	return -1;
}
