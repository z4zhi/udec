#ifndef MAP_H
#define MAP_H
#include <bits/stdc++.h>
using namespace std;

class Map{

public:
	virtual void insert(pair<string, int>) = 0;
	virtual void erase(string)= 0;
	virtual int at(string) = 0;
	virtual int size() = 0;
	virtual bool empty() = 0;
};

#endif
