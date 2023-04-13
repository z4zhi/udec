#include <bits/stdc++.h>
#include "MapH.h"
using namespace std;

MapH::MapH(int tam) {
  capacidad = tam/2;
  n = 0;
  a = n/capacidad;
  pair<string, int> p("#", -1); // # y -1 = vacio
  table.clear(); table.resize(capacidad, p);
}

MapH::~MapH() {
  table.clear();
  table.shrink_to_fit();
}

void MapH::insert(pair<string, int> p) {
  int hk = hash(p.first);
  int j = 0;
  int dk = dh(p.first);
  int hx = (hk + j*dk) % capacidad;

  int pos = INT_MAX, posflag = 0; // primera posicion vacia (borrada)
  // double hashing
  // el ciclo recorre hasta una posicion vacia, en caso de que se repita el elemento
  while (table[hx].second != -1) { // buscar hasta un espacio vacio

    if (table[hx].first == p.first) { // clave repetida
      table[hx].second = p.second; //actualizar valor;
      return;
    }

    // la llave se guardara en la primera posicion vacia (borrada) si es posible
    if (table[hx].second == -2 && posflag == 0) {
      pos = hx; // guardar la posicion del primer espacio vacio (borrado)
      posflag = 1;
    }

    j++;
    hx = (hk + j*dk) % capacidad;
  }

  if (pos < hx) hx = pos;
  table[hx] = p;
  n++;
  a = n/capacidad;
  if (a >= 0.5) rehash();
}

void MapH::erase(string s) {
  int hk = hash(s);
  int j = 0;
  int dk = dh(s);
  int hx = (hk + j*dk) % capacidad;

  while (table[hx].first != s) { // recorrer hasta la clave
    if (table[hx].second == -1) return; // si hay un espacio vacio -> no esta la clave
    j++;
    hx = (hk + j*dk) % capacidad;
  }
  if (table[hx].first == s) {
    pair<string, int> p("$", -2); // $ y -2 = borrado
    table[hx] = p;
    n--;
  }
}

int MapH::at(string s) {
  int hk = hash(s);
  int j = 0;
  int dk = dh(s);
  int hx = (hk + j*dk) % capacidad;

  while (table[hx].first != s) {
    if (table[hx].second == -1) return -1; // si hay un espacio vacio -> no esta la clave
    j++;
    hx = (hk + j*dk) % capacidad;
  }
  return table[hx].second;
}

int MapH::size() {
  return n;
}

bool MapH::empty() {
  if (n) return false;
  return true;
}

int MapH::hash(string x) {
  int hx = 0;
  int b = 127;
  for (int i = 0; i < x.size(); i++) {
    int v = (int)x[i];
    hx = (b * hx + v) % capacidad;
  }
  return hx;
}

int MapH::dh(string k) {
  unsigned long long dk = 0;
  for (int i = 0; i < k.size(); i++) {
    unsigned long long tmp = (int)k[i] * pow(33, i);
    dk += tmp;
  }
  //dk = dk % capacidad;
  dk = (3*dk + 7) % capacidad;
  int q = 2;
  dk = q - (dk % q);
  return dk;
}

void MapH::rehash() {
  vector<pair<string, int>> temp;
  temp = table;
  capacidad *= 2;
  if (capacidad % 2 == 0) capacidad++;
  pair<string, int> p("#", -1);
  table.clear(); table.resize(capacidad, p);

  for (int i = 0; i < temp.size(); i++) {
    if (temp[i].second > -1) { // si no es una posicion vacia o borrada
      int hk = hash(temp[i].first);
      int j = 0;
      int dk = dh(temp[i].first);
      int hx = (hk + j*dk) % capacidad;

      // double hashing
      while (table[hx].second > -1) {
        j++;
        hx = (hk + j*dk) % capacidad;
      }
      table[hx] = temp[i];
    }
  }
}
