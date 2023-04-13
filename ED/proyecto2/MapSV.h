#ifndef MAPSV_H
#define MAPSV_H
#include "Map.h"
#include <bits/stdc++.h>
using namespace std;
class MapSV : public Map {

private:
	vector<pair<string, int>> v;
	int binariaRecursiva(string s, int l, int r);
public:
	MapSV();
	~MapSV();
	void insert(pair<string, int>);
	void erase(string);
	int at(string);
	int size();
	bool empty();
};
#endif
