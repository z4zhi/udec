#ifndef MAPH_H
#define MAPH_H
#include "Map.h"
#include <bits/stdc++.h>

using namespace std;

class MapH : public Map {

private:
	int n,capacidad; // n y N
	double a; // alfa
	vector<pair<string, int>> table;
	int hash(string);
	void rehash();
	int dh(string);

public:
	MapH(int);
	~MapH();
	void insert(pair<string, int>);
	void erase(string);
	int at(string);
	int size();
	bool empty();
};
#endif
