#include "MiniSet.h"

struct nodoSkip {
	int n;
	nodoSkip *sig = nullptr;
	nodoSkip *sigNiv = nullptr;
};

class SkipList : public MiniSet {
	private:
		static const int niveles = 4;
		struct nodoSkip *nivel[niveles];
		int size;
	public:
		SkipList();
		~SkipList();
		void insert(int n);
		void remove(int n);
		bool search(int n);
};
