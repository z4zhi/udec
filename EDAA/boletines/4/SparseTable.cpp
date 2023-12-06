// https://github.com/stevenhalim/cpbook-code/blob/master/ch9/SparseTable.cpp
#include <bits/stdc++.h>
using namespace std;

typedef vector<int> vi;

// se calcula el numero de bits para representar el tamaño
// se crean y llenan 2 arreglos que guardan potencias y logaritmos de 2
// se crea la spt (bidimensional), con un tamaño (L2[n] + 1) * n y se inicializa la primera fila
// luego, se usa un doble bucle for anidado, en i se recorre las potencias de 2 hasta n
// j recorre los indices del arreglo A y se calcula x e y, que son extremos del subarray rango j
// el valor en la spt es el indice del elemento que tiene el minimo entre A[x] y A[y]
// para rmq, se calcula k (limites de los subarrays) para calcular x e y que son los limites del subarray
// finalemnte devolver x o y dependiendo de cual tenga el minimo A[x] o A[y]
class SparseTable
{
private:
  vi A, P2, L2;   // arreglo original, potencias de 2 y logaritmos de 2
  vector<vi> SpT; // the Sparse Table
public:
  SparseTable() {} // default constructor

  SparseTable(vi &initialA) // pre-processing routine
  {
    A = initialA;
    int n = (int)A.size();

    // bits para representar n (tamaño)
    int L2_n = (int)log2(n) + 1;
    // asignar tamaño
    P2.assign(L2_n, 0);
    L2.assign(1 << L2_n, 0);
    // preprocesar y asignar las potencias y logaritmos
    for (int i = 0; i <= L2_n; ++i)
    {
      P2[i] = (1 << i); // to speed up 2^i
      L2[(1 << i)] = i; // to speed up log_2(i)
    }
    // llenar valores faltantes
    for (int i = 2; i < P2[L2_n]; ++i)
      if (L2[i] == 0)
        L2[i] = L2[i - 1]; // to fill in the blanks

    // the initialization phase
    // construir la SpT
    SpT = vector<vi>(L2[n] + 1, vi(n));
    // inicializar primera fila con valores originales del indice
    for (int j = 0; j < n; ++j)
      SpT[0][j] = j; // RMQ of sub array [j..j]

    // doble bucle anidado para calcular y guardar los valores
    // the two nested loops below have overall time complexity = O(n log n)
    for (int i = 1; P2[i] <= n; ++i)          // for all i s.t. 2^i <= n
      for (int j = 0; j + P2[i] - 1 < n; ++j) // for all valid j
      {
        // calcular 2 indices que representan los indices de los elementos en el rango j...
        int x = SpT[i - 1][j];             // [j..j+2^(i-1)-1]
        int y = SpT[i - 1][j + P2[i - 1]]; // [j+2^(i-1)..j+2^i-1]
        SpT[i][j] = A[x] <= A[y] ? x : y;
      }
  }

  int RMQ(int i, int j)
  {
    int k = L2[j - i + 1];         // 2^k <= (j-i+1)
    int x = SpT[k][i];             // covers [i..i+2^k-1]
    int y = SpT[k][j - P2[k] + 1]; // covers [j-2^k+1..j]
    return A[x] <= A[y] ? x : y;
  }
};

int main(int argc, char *argv[])
{
  if (argc == 1)
  {
    // cntme example as in Chapter 2: Segment Tree
    vi A = {18, 17, 13, 19, 15, 11, 20};
    SparseTable SpT(A);
    int n = (int)A.size();
    for (int i = 0; i < n; ++i)
      for (int j = i; j < n; ++j)
        printf("RMQ(%d, %d) = %d\n", i, j, SpT.RMQ(i, j));
  }
  else
  {
    const int reps = 40; // reps totales incluyendo cold runs
    const int cold = 10; // cold runs, numero de iteraciones que no cuentan (menor a reps)
    string s = "sparse.csv";
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
        SparseTable SpT(A);
        auto end = chrono::high_resolution_clock::now();
        double con_time = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

        uniform_int_distribution<int> query_dist(0, i - 1);
        int x = query_dist(gen);
        int y = query_dist(gen);
        if (x > y)
          swap(x, y);

        start = chrono::high_resolution_clock::now();
        SpT.RMQ(x, y);
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
