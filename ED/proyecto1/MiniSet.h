#ifndef MINISET_H
#define MINISET_H

class MiniSet {
	public:
		virtual void insert(int) = 0;
		virtual void remove(int) = 0;
		virtual bool search(int) = 0;
};

#endif
