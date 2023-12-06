// https://github.com/stevenhalim/cpbook-code/blob/master/ch2/ourown/segmenttree_ds.cpp
#include <bits/stdc++.h>
using namespace std;

typedef vector<int> vi;

// se llama build para crear el st. 
// dividir el rango en segmentos mas pequeños, hasta llegar a nodos hoja y guardar elementos de A
// se usa conquer para crear el nodo padre
// para rmq, primero se propaga info pendiente con propagate
// luego, si no esta en el rango, se divide el rango y se consulta ambos hijos. despues se combinan con conquer
// update es similar, actualizando hijos y luego se actualiza el padre con la info de hijos  
class SegmentTree
{
private:
  int n;          // n = (int)A.size()
  vi A, st, lazy; // the arrays, arreglo original, segment tree, nodos

  // calcular indices de los nodos hijos
  int l(int p) { return p << 1; }       // go to left child
  int r(int p) { return (p << 1) + 1; } // go to right child

  // combinacion, retornar el minimo
  int conquer(int a, int b)
  {
    if (a == -1)
      return b; // corner case
    if (b == -1)
      return a;
    return min(a, b); // RMQ
  }

  // construir el st recursivamente
  // dividir el rango/2 y construir los nodos
  void build(int p, int L, int R) // O(n)
  {
    if (L == R)
      st[p] = A[L]; // base case (hoja)
    else
    {
      int m = (L + R) / 2;
      build(l(p), L, m);                   // hijo izquierdo
      build(r(p), m + 1, R);               // hijo derecho
      st[p] = conquer(st[l(p)], st[r(p)]); // combinar resultados de ambos hijos
    }
  }

  // propagar actualizaciones pendientes
  // si no es nodo hoja, propagar a hijos
  void propagate(int p, int L, int R)
  {
    if (lazy[p] != -1)
    {                                      // has a lazy flag
      st[p] = lazy[p];                     // [L..R] has same value
      if (L != R)                          // not a leaf
        lazy[l(p)] = lazy[r(p)] = lazy[p]; // propagate downwards
      else                                 // L == R, a single index
        A[L] = lazy[p];                    // time to update this
      lazy[p] = -1;                        // erase lazy flag
    }
  }

  // si esta dentro del rango, return
  // sino, dividir el rango y consultar a hijos
  int RMQ(int p, int L, int R, int i, int j) // O(log n)
  {
    propagate(p, L, R); // lazy propagation
    if (i > j)
      return -1; // infeasible
    if ((L >= i) && (R <= j))
      return st[p]; // found the segment
    int m = (L + R) / 2;
    return conquer(RMQ(l(p), L, m, i, min(m, j)),
                   RMQ(r(p), m + 1, R, max(i, m + 1), j));
  }

  // update a un rango, similar a rmq
  void update(int p, int L, int R, int i, int j, int val) // O(log n)
  {
    propagate(p, L, R); // lazy propagation
    if (i > j)
      return;
    if ((L >= i) && (R <= j))
    {                     // found the segment
      lazy[p] = val;      // update this
      propagate(p, L, R); // lazy propagation
    }
    else
    {
      int m = (L + R) / 2;
      update(l(p), L, m, i, min(m, j), val);
      update(r(p), m + 1, R, max(i, m + 1), j, val);
      // ver si hay lazy pendiente, si hay actualizar con ese sino, con el hijo izq/der
      int lsubtree = (lazy[l(p)] != -1) ? lazy[l(p)] : st[l(p)];
      int rsubtree = (lazy[r(p)] != -1) ? lazy[r(p)] : st[r(p)];
      st[p] = (lsubtree <= rsubtree) ? st[l(p)] : st[r(p)]; // actualizar el st
    }
  }

public:
  // inicializar segment tree, con los arreglos
  SegmentTree(int sz) : n(sz), st(4 * n), lazy(4 * n, -1) {}

  // inicializar con un arreglo existente
  SegmentTree(const vi &initialA) : SegmentTree((int)initialA.size())
  {
    A = initialA;
    build(1, 0, n - 1);
  }

  // update en rango [i, j]
  void update(int i, int j, int val) { update(1, 0, n - 1, i, j, val); }

  // rmq en rango [i, j]
  int RMQ(int i, int j) { return RMQ(1, 0, n - 1, i, j); }
};

int main(int argc, char *argv[])
{
  if (argc == 1)
  {
    vi A = {18, 17, 13, 19, 15, 11, 20, 99}; // make n a power of 2
    SegmentTree st(A);

    printf("              idx    0, 1, 2, 3, 4, 5, 6, 7\n");
    printf("              A is {18,17,13,19,15,11,20,oo}\n");
    printf("RMQ(1, 3) = %d\n", st.RMQ(1, 3)); // 13
    printf("RMQ(4, 7) = %d\n", st.RMQ(4, 7)); // 11
    printf("RMQ(3, 4) = %d\n", st.RMQ(3, 4)); // 15

    st.update(5, 5, 77); // update A[5] to 77
    printf("              idx    0, 1, 2, 3, 4, 5, 6, 7\n");
    printf("Now, modify A into {18,17,13,19,15,77,20,oo}\n");
    printf("RMQ(1, 3) = %d\n", st.RMQ(1, 3)); // remains 13
    printf("RMQ(4, 7) = %d\n", st.RMQ(4, 7)); // now 15
    printf("RMQ(3, 4) = %d\n", st.RMQ(3, 4)); // remains 15

    st.update(0, 3, 30); // update A[0..3] to 30
    printf("              idx    0, 1, 2, 3, 4, 5, 6, 7\n");
    printf("Now, modify A into {30,30,30,30,15,77,20,oo}\n");
    printf("RMQ(1, 3) = %d\n", st.RMQ(1, 3)); // now 30
    printf("RMQ(4, 7) = %d\n", st.RMQ(4, 7)); // remains 15
    printf("RMQ(3, 4) = %d\n", st.RMQ(3, 4)); // remains 15

    st.update(3, 3, 7); // update A[3] to 7
    printf("              idx    0, 1, 2, 3, 4, 5, 6, 7\n");
    printf("Now, modify A into {30,30,30, 7,15,77,20,oo}\n");
    printf("RMQ(1, 3) = %d\n", st.RMQ(1, 3)); // now 7
    printf("RMQ(4, 7) = %d\n", st.RMQ(4, 7)); // remains 15
    printf("RMQ(3, 4) = %d\n", st.RMQ(3, 4)); // now 7
  }
  else
  {
    const int reps = 40; // reps totales incluyendo cold runs
    const int cold = 10; // cold runs, numero de iteraciones que no cuentan (menor a reps)
    string s = "segment.csv";
    ofstream output(s);
    output << "n,t1,v1,t2,v2 \n";

    const int N = 11000000;
    for (int i = 1000000; i <= N; i += 2000000)
    {
      random_device rd;
      mt19937 gen(rd());
      uniform_int_distribution<int> dist(0, i);
      vi A(i);
      for (int &num : A)
        num = dist(gen);

      vector<double> con_times, cnt_times;
      for (int j = 0; j < reps; j++)
      {
        auto start = chrono::high_resolution_clock::now();
        SegmentTree st(A);
        auto end = chrono::high_resolution_clock::now();
        double con_time = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

        uniform_int_distribution<int> query_dist(0, i - 1);
        int x = query_dist(gen);
        int y = query_dist(gen);
        if (x > y)
          swap(x, y);
        start = chrono::high_resolution_clock::now();
        st.RMQ(x, y);
        end = chrono::high_resolution_clock::now();
        double cnt_time = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

        if (j < cold)
          continue;
        con_times.push_back(con_time);
        cnt_times.push_back(cnt_time);
      }

      double avg_con_time = accumulate(con_times.begin(), con_times.end(), 0.0) / (reps - cold);
      double var_con_time = 0.0;
      for (double t : con_times)
        var_con_time += pow(t - avg_con_time, 2);
      var_con_time /= (reps - cold);

      double avg_cnt_time = accumulate(cnt_times.begin(), cnt_times.end(), 0.0) / (reps - cold);
      double var_cnt_time = 0.0;
      for (double t : cnt_times)
        var_cnt_time += pow(t - avg_cnt_time, 2);
      var_cnt_time /= (reps - cold);

      cout << i << ","
           << avg_con_time << "," << var_con_time << ","
           << avg_cnt_time << "," << var_cnt_time << "\n";
      output << i << ","
             << avg_con_time << "," << var_con_time << ","
             << avg_cnt_time << "," << var_cnt_time << "\n";
    }
  }

  return 0;
}
