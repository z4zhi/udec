#include <bits/stdc++.h>
#include <iostream>
#include "MapSV.h"
#include "MapH.h"
#include "MapAVL.h"

using namespace std;

string palabra() {
	string s = "hola";
	int ssize = rand() % 8 + 2;
	s.resize(ssize);

	for (int i = 0; i < ssize; i++) {
		int letramin = rand() % (122 - 97 + 1) + 97; // letra miniscula desde "a" hasta "z"
		s[i] = letramin;
	}
	return s;
}

int main(){
	srand(time(NULL));
	int n;
	cout << "n:" << " ";
	cin >> n;
	vector<pair<string, int>> v;
	for (int i = 0; i < n; i++) {
		string str = palabra();
		int aux = rand() % n;
		pair<string, int> tmp(str, aux);
		v.push_back(tmp);
		std::cout << tmp.second << ' ';
	}
	int aux,i;
	std::cout << '\n' << "insert" << '\n';
	MapSV *mapsv = new MapSV();
	for (i = 0; i < n; i++)  mapsv->insert(v[i]);
	for (i = 0; i < n; i++)  {
		aux = mapsv->at(v[i].first);
		std::cout << aux << ' ';
	}

	std::cout << '\n'<< "erase" << '\n';
	for (i = 0; i < n; i++)  mapsv->erase(v[i].first);
	for (i = 0; i < n; i++)  {
		aux = mapsv->at(v[i].first);
		std::cout << aux << ' ';
	}
	delete mapsv;

	std::cout << '\n'<< "insert" << '\n';
	MapH *maph = new MapH(n);
	for (i = 0; i < n; i++)  maph->insert(v[i]);
	for (i = 0; i < n; i++)  {
		aux = maph->at(v[i].first);
		std::cout << aux << ' ';
	}

	std::cout << '\n'<< "erase" << '\n';
	for (i = 0; i < n; i++)  maph->erase(v[i].first);
	for (i = 0; i < n; i++)  {
		aux = maph->at(v[i].first);
		std::cout << aux << ' ';
	}
	delete maph;

	std::cout << '\n'<< "insert" << '\n';
	MapAVL *mapavl = new MapAVL();
	for (i = 0; i < n; i++)  mapavl->insert(v[i]);
	for (i = 0; i < n; i++)  {
		aux = mapavl->at(v[i].first);
		std::cout << aux << ' ';
	}

	std::cout << '\n'<< "erase" << '\n';
	for (i = 0; i < n; i++)  mapavl->erase(v[i].first);
	for (i = 0; i < n; i++)  {
		aux = mapavl->at(v[i].first);
		std::cout << aux << ' ';
	}
	delete mapavl;
	return 0;
}
