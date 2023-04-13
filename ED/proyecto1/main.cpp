#include <bits/stdc++.h>
#include "LinkedList.h"
#include "SkipList.h"
using namespace std;

int main() {
	srand(time(NULL));
	LinkedList *ll = new LinkedList;
	SkipList *sl = new SkipList;
	for (int n = 1000; n < 10001; n+=1000) {
		vector<int> v(n);
		for (int i = 0; i < n; i++)
			v[i] = i+1;
		random_shuffle(v.begin(), v.end());
		double time;
		// arreglo para analisis exp
		vector<int> arreglo(101);
		for (int i = 0; i < 100; i++)
			arreglo[i] = rand() % n;

		// insert
		clock_t start = clock();
		for(int i = 0; i < n; i++) ll->insert(v[i]);
		time = ((double)clock() - start) / CLOCKS_PER_SEC;
		//printf("Insert LL: %f \n",(pow(10,6))*time/(double)n);

		start = clock();
		for(int i = 0; i < n; i++) sl->insert(v[i]);
		time = ((double)clock() - start) / CLOCKS_PER_SEC;
		//printf("Insert SL: %f \n",(pow(10,6))*time/(double)n);

		 int spcll = ll->space();
		std::cout << spcll << '\n';

	}
	delete ll; delete sl;
	return 0;
}
