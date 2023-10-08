// C++ program to implement different operations on Binomial Heap
#include <bits/stdc++.h>
using namespace std;

// A Binomial Tree node.
// Definicion de un nodo en un arbol binomial.
// Contiene su valor, su grado actual y los nodos vecinos (hijos, hermanos y padres)
struct Node
{
	int data, degree;
	Node *child, *sibling, *parent;
};

// Crear un nodo
// Para crear un nodo se necesita un valor (key)
// Como el nodo es nuevo, no tiene vecinos y el grado es 0
Node *newNode(int key)
{
	Node *temp = new Node;
	temp->data = key;
	temp->degree = 0;
	temp->child = temp->parent = temp->sibling = NULL;
	return temp;
}

// This function merge two Binomial Trees.
// Merge de 2 arboles binomiales
// Es solo hacer que el menor sea hijo del mayor
// El grado tambien aumenta
Node *mergeBinomialTrees(Node *b1, Node *b2)
{
	// Make sure b1 is smaller
	if (b1->data > b2->data)
		swap(b1, b2);

	// We basically make larger valued tree a child of smaller valued tree
	b2->parent = b1;
	b2->sibling = b1->child;
	b1->child = b2;
	b1->degree++;

	return b1;
}

// This function perform union operation on two binomial heap i.e. l1 & l2
// Union de 2 binomial heaps
// Se crea un heap para almacenar el merge de los 2 heaps
// Se comparan los grados de los nodos de cada heap y se almacena primero el de menor grado
// Si quedan nodos sin guardar, se almacenan
list<Node *> unionBionomialHeap(list<Node *> l1,
								list<Node *> l2)
{
	// _new to another binomial heap which contain new heap after merging l1 & l2
	list<Node *> _new;
	list<Node *>::iterator it = l1.begin();
	list<Node *>::iterator ot = l2.begin();
	while (it != l1.end() && ot != l2.end())
	{
		// if D(l1) <= D(l2)
		if ((*it)->degree <= (*ot)->degree)
		{
			_new.push_back(*it);
			it++;
		}
		// if D(l1) > D(l2)
		else
		{
			_new.push_back(*ot);
			ot++;
		}
	}

	// if there remains some elements in l1 binomial heap
	while (it != l1.end())
	{
		_new.push_back(*it);
		it++;
	}

	// if there remains some elements in l2 binomial heap
	while (ot != l2.end())
	{
		_new.push_back(*ot);
		ot++;
	}
	return _new;
}

// adjust function rearranges the heap so that heap is in increasing order of degree and
// no two binomial trees have same degree in this heap
// Ajustar el heap para que arboles tengan grado creciente estricto
list<Node *> adjust(list<Node *> _heap)
{
	if (_heap.size() <= 1)
		return _heap;
	list<Node *> new_heap;
	list<Node *>::iterator it1, it2, it3;
	it1 = it2 = it3 = _heap.begin();

	// Cuando hay solo 2 heaps, no se necesita el tercer iterador
	if (_heap.size() == 2)
	{
		it2 = it1;
		it2++;
		it3 = _heap.end();
	}
	// Sino, se inicializa con 1, 2, 3
	else
	{
		it2++;
		it3 = it2;
		it3++;
	}
	// while que recorre todos los arboles
	while (it1 != _heap.end())
	{
		// if only one element remains to be processed
		if (it2 == _heap.end())
			it1++;

		// If D(it1) < D(it2) i.e.
		// merging of Binomial Tree pointed by it1 & it2 is not possible then move next in heap
		// Si el grado del arbol1 es menor al a arbol2, seguir al prox. heap
		else if ((*it1)->degree < (*it2)->degree)
		{
			it1++;
			it2++;
			if (it3 != _heap.end())
				it3++;
		}

		// if D(it1),D(it2) & D(it3) are same i.e.
		// degree of three consecutive Binomial Tree are same in heap
		// Si los 3 arboles tienen el mismo grado, avanzar
		else if (it3 != _heap.end() &&
				 (*it1)->degree == (*it2)->degree &&
				 (*it1)->degree == (*it3)->degree)
		{
			it1++;
			it2++;
			it3++;
		}

		// if degree of two Binomial Tree are same in heap
		// Si el grado del arbol1 === arbol2, merge
		// eliminar arbol2, arbol3 avanza
		else if ((*it1)->degree == (*it2)->degree)
		{
			Node *temp;
			*it1 = mergeBinomialTrees(*it1, *it2);
			it2 = _heap.erase(it2);
			if (it3 != _heap.end())
				it3++;
		}
	}
	return _heap;
}

// inserting a Binomial Tree into binomial heap
// Insertar arbol binomial al heap
// Se crea un heap nuevo con el arbol y se hace union con el heap viejo
list<Node *> insertATreeInHeap(list<Node *> _heap,
							   Node *tree)
{
	// creating a new heap i.e temp
	list<Node *> temp;

	// inserting Binomial Tree into heap
	temp.push_back(tree);

	// perform union operation to finally insert Binomial Tree in original heap
	temp = unionBionomialHeap(_heap, temp);

	return adjust(temp);
}

// removing minimum key element from binomial heap
// this function take Binomial Tree as input and return binomial heap after removing head of that tree i.e. minimum element
// Remover el elemento minimo
// Se hace una copia del hijo del arbol
// Para cada hermano del hijo, se separan y luego se insertan al principio del heap
list<Node *> removeMinFromTreeReturnBHeap(Node *tree)
{
	list<Node *> heap;
	Node *temp = tree->child;
	Node *lo;

	// making a binomial heap from Binomial Tree
	while (temp)
	{
		lo = temp;
		temp = temp->sibling;
		lo->sibling = NULL;
		heap.push_front(lo);
	}
	return heap;
}

// inserting a key into the binomial heap
// Insertar un nodo nuevo a un heap = insertar un arbol
list<Node *> insert(list<Node *> _head, int key)
{
	Node *temp = newNode(key);
	return insertATreeInHeap(_head, temp);
}

// return pointer of minimum value Node present in the binomial heap
// Buscar minimo recorriendo
Node *getMin(list<Node *> _heap)
{
	list<Node *>::iterator it = _heap.begin();
	Node *temp = *it;
	while (it != _heap.end())
	{
		if ((*it)->data < temp->data)
			temp = *it;
		it++;
	}
	return temp;
}

// Extraer minimo
// Buscar el minimo con la funcion getMin
// Luego se insertan todos los arboles al nuevo heap excepto el arbol con el minimo
// Se remueve el minimo del arbol
// Luego el arbol sin el minimo se une al hep nuevo y se ajusta
list<Node *> extractMin(list<Node *> _heap)
{
	list<Node *> new_heap, lo;
	Node *temp;

	// temp contains the pointer of minimum value element in heap
	temp = getMin(_heap);
	list<Node *>::iterator it;
	it = _heap.begin();
	while (it != _heap.end())
	{
		if (*it != temp)
		{
			// inserting all Binomial Tree into new binomial heap except the Binomial Tree contains minimum element
			new_heap.push_back(*it);
		}
		it++;
	}
	lo = removeMinFromTreeReturnBHeap(temp);
	new_heap = unionBionomialHeap(new_heap, lo);
	new_heap = adjust(new_heap);
	return new_heap;
}

// print function for Binomial Tree
void printTree(Node *h)
{
	while (h)
	{
		cout << h->data << " ";
		printTree(h->child);
		h = h->sibling;
	}
}

// print function for binomial heap
void printHeap(list<Node *> _heap)
{
	list<Node *>::iterator it;
	it = _heap.begin();
	while (it != _heap.end())
	{
		printTree(*it);
		it++;
	}
}

// Driver program to test above functions
int main(int argc, char *argv[])
{
	if (argc == 1)
	{
		int ch, key;
		list<Node *> _heap;

		// Insert data in the heap
		for (int i = 0; i < 5; i++)
			_heap = insert(_heap, i);
		cout << "Heap elements after insertion:\n";
		printHeap(_heap);

		Node *temp = getMin(_heap);
		cout << "\nMinimum element of heap "
			 << temp->data << "\n";

		// Delete minimum element of heap
		_heap = extractMin(_heap);
		cout << "Heap after deletion of minimum element\n";
		printHeap(_heap);
	}
	else
	{
		const int reps = 40; // reps totales incluyendo cold runs
		const int cold = 10; // cold runs, numero de iteraciones que no cuentan (menor a reps)
		string s = "binomial.csv";
		ofstream output(s);
		output << "n,tInsert,vInsert,tRemove,vRemove,tUnion,vUnion \n";

		const int N = 100000000;
		for (int i = 100; i <= N; i *= 10)
		{
			list<Node *> heap1;
			for (int k = 0; k < i; k++)
				heap1 = insert(heap1, k);
			vector<double> i_times, r_times, u_times;
			for (int j = 0; j < reps; j++)
			{
				auto start = chrono::high_resolution_clock::now();
				heap1 = insert(heap1, -1);
				auto end = chrono::high_resolution_clock::now();
				double i_time = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

				start = chrono::high_resolution_clock::now();
				heap1 = extractMin(heap1);
				end = chrono::high_resolution_clock::now();
				double r_time = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

				start = chrono::high_resolution_clock::now();
				list<Node *> merged = unionBionomialHeap(heap1, heap1);
				end = chrono::high_resolution_clock::now();
				double u_time = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

				if (j < cold)
					continue;
				i_times.push_back(i_time);
				r_times.push_back(r_time);
				u_times.push_back(u_time);
			}
			double avg_i_time = accumulate(i_times.begin(), i_times.end(), 0.0) / (reps - cold);
			double var_i_time = 0.0;
			for (double t : i_times)
				var_i_time += pow(t - avg_i_time, 2);
			var_i_time /= (reps - cold);

			double avg_r_time = accumulate(r_times.begin(), r_times.end(), 0.0) / (reps - cold);
			double var_r_time = 0.0;
			for (double t : r_times)
				var_r_time += pow(t - avg_r_time, 2);
			var_r_time /= (reps - cold);

			double avg_u_time = accumulate(u_times.begin(), u_times.end(), 0.0) / (reps - cold);
			double var_u_time = 0.0;
			for (double t : u_times)
				var_u_time += pow(t - avg_u_time, 2);
			var_u_time /= (reps - cold);

			cout << i << "," << avg_i_time << "," << var_i_time << "," << avg_r_time << "," << var_r_time << "," << avg_u_time << "," << var_u_time << "\n";
			output << i << "," << avg_i_time << "," << var_i_time << "," << avg_r_time << "," << var_r_time << "," << avg_u_time << "," << var_u_time << "\n";
		}
	}
	return 0;
}
