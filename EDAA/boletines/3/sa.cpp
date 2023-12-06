// Fuente: https://www.geeksforgeeks.org/suffix-array-set-2-a-nlognlogn-algorithm/
#include <iostream>
#include <cstring>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <vector>
#include <cmath>
#include <numeric>

using namespace std;

// la idea principal es construir un suffix array, esto se puede hacer de varias formas.
// en esta implementacion se usan ranks, donde se comparan por los primeros 2 caracteres, luego 4, 8, etc. y se ordena.
// para la busqueda del patron, se hace con una busqueda binaria.
// se compara el patron con el sufijo del medio y luego ver si es menor o mayor e ir moviendose (busqueda binaria)
// si lo encuentra, revisa atras y adelante para ver si hay sufijos que tambien contienen el patron

// struct para guardar informacion del sufijo, ranks e indice
// el segundo rank es para casos donde hay 2 con igual rank (desempatar)
struct suffix
{
    int index;
    int rank[2];
};

// funcion para comparacion de los ranks
int cmp(struct suffix a, struct suffix b)
{
    return (a.rank[0] == b.rank[0]) ? (a.rank[1] < b.rank[1] ? 1 : 0) : (a.rank[0] < b.rank[0] ? 1 : 0);
}

// para crear el suffix array, se necesita el string
vector<int> buildSuffixArray(string &txt)
{
    int n = txt.length();
    // se crea un arreglo que guardara los n sufijos
    vector<suffix> suffixes(n);

    // guardar el indice del sufijo y su rank para 2 caracteres
    // si no es el ultimo sufijo guardar, sino -1
    for (int i = 0; i < n; i++)
    {
        suffixes[i].index = i;
        suffixes[i].rank[0] = txt[i] - 'a';
        suffixes[i].rank[1] = ((i + 1) < n) ? (txt[i + 1] - 'a') : -1;
    }

    // ordenar los sufijos con la funcion
    sort(suffixes.begin(), suffixes.end(), cmp);

    int ind[n]; // arreglo para obtener el indice en suffixes[] a partir del índice original.

    // hasta ahora, los sufijos estan ordenados por los primeros 2 caracteres
    // ahora hay que hacerlo con 4, 8 caracteres y asi
    for (int k = 4; k < 2 * n; k = k * 2)
    {
        // asignacion al primer sufijo
        int rank = 0;
        int prev_rank = suffixes[0].rank[0];
        suffixes[0].rank[0] = rank;
        ind[suffixes[0].index] = 0;

        // asignar rank a sufjos
        for (int i = 1; i < n; i++)
        {
            // Para cada sufijo, se compara su rank y los siguientes ranks con el sufijo anterior en el arreglo
            if (suffixes[i].rank[0] == prev_rank &&
                suffixes[i].rank[1] == suffixes[i - 1].rank[1])
            {
                // Si los ranks coinciden, al sufijo actual se le asigna el mismo rank que al anterior
                prev_rank = suffixes[i].rank[0];
                suffixes[i].rank[0] = rank;
            }
            else // Si no coinciden, al sufijo actual se le asigna un nuevo rank y se incrementa el contador de ranks
            {
                prev_rank = suffixes[i].rank[0];
                suffixes[i].rank[0] = ++rank;
            }
            ind[suffixes[i].index] = i;
        }

        // para cada sufijo en suffixes
        for (int i = 0; i < n; i++)
        {
            // Se calcula la proxima posicion en el arreglo de sufijos sumando k/2 a la posicion actual
            int nextindex = suffixes[i].index + k / 2;
            // Se verifica si lo calculado esta dentro del tamaño (n).
            // Si es asi, se obtiene el primer rank del sufijo y se asigna como el segundo rank al sufijo actual. sino -1.
            suffixes[i].rank[1] = (nextindex < n) ? suffixes[ind[nextindex]].rank[0] : -1;
        }

        // Sort the suffixes according to first k characters
        // ordenar por los primeros k caracteres
        sort(suffixes.begin(), suffixes.end(), cmp);
    }

    // despues de ordenar los sufijos, se guardan en el suffix array, pero no el sufijo, sino que los indices
    vector<int> suffixArr(n);
    for (int i = 0; i < n; i++)
        suffixArr[i] = suffixes[i].index;

    return suffixArr;
}

// Fuente: https://www.cs.yale.edu/homes/aspnes/pinewiki/SuffixArrays.html
// funcion para encontrar patron utilizando busqueda binaria
int search(vector<int> &suffixArr, const string &txt, const string &pattern)
{
    int m = pattern.length();
    int count = 0;
    int n = suffixArr.size();

    // busqueda binaria
    int l = 0, r = n - 1;
    while (l <= r)
    {
        int mid = (r + l) / 2;

        // comparar el patron de tamaño m con el sufijo del "medio"
        // se comparan los primeros m caracteres del sufijo con el pattern
        int compare = strncmp(pattern.c_str(), txt.c_str() + suffixArr[mid], m);

        // si devuelve 0 es porque encontro
        // luego buscar adelante y atras para ver si hay mas ocurrencias
        if (compare == 0)
        {
            count++;
            int left = mid - 1;
            int right = mid + 1;

            // revisar atras
            while (left >= 0 && strncmp(pattern.c_str(), txt.c_str() + suffixArr[left], m) == 0)
            {

                count++;
                left--;
            }

            // revisar adelante
            while (right < n && strncmp(pattern.c_str(), txt.c_str() + suffixArr[right], m) == 0)
            {
                count++;
                right++;
            }

            return count;
        }
        // si es menor a 0, es lexicograficamente menor, mid es extremo derecho
        else if (compare < 0)
            r = mid - 1;
        else
            l = mid + 1;
    }

    return count;
}

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        string txt = "banana";
        vector<int> suffixArr = buildSuffixArray(txt);
        string pattern = "ana";
        int patternCount = search(suffixArr, txt, pattern);
        cout << pattern << " " << patternCount << " veces" << endl;

        string txt2 = "ABABDABACDABABCABAB";
        vector<int> suffixArr2 = buildSuffixArray(txt2);
        string pattern2 = "ABABCABAB";
        int patternCount2 = search(suffixArr2, txt2, pattern2);
        cout << pattern2 << " " << patternCount2 << " veces" << endl;
    }
    else if (argc == 2)
    {
        const int reps = 40; // reps totales incluyendo cold runs
        const int cold = 10; // cold runs, numero de iteraciones que no cuentan (menor a reps)
        string s = "sa1.csv";
        ofstream output(s);
        output << "n,tsa,vsa \n";

        ifstream file("dna", std::ios::binary);
        string pattern;
        pattern.resize(1000);
        file.read(&pattern[0], 1000);

        const int N = 1000000;
        for (int i = N / 10; i <= N; i += N / 10)
        {
            string txt;
            txt.resize(i);
            file.read(&txt[0], i);
            vector<int> suffixArr = buildSuffixArray(txt);

            vector<double> s_times;
            for (int j = 0; j < reps; j++)
            {
                auto start = chrono::high_resolution_clock::now();
                int cnt = search(suffixArr, txt, pattern);
                auto end = chrono::high_resolution_clock::now();
                double s_time = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

                if (j < cold)
                    continue;
                s_times.push_back(s_time);
            }
            double avg_s_time = accumulate(s_times.begin(), s_times.end(), 0.0) / (reps - cold);
            double var_s_time = 0.0;
            for (double t : s_times)
                var_s_time += pow(t - avg_s_time, 2);
            var_s_time /= (reps - cold);

            cout << i << "," << avg_s_time << "," << var_s_time << "\n";
            output << i << "," << avg_s_time << "," << var_s_time << "\n";
        }
    }

    else if (argc == 3)
    {
        const int reps = 40; // reps totales incluyendo cold runs
        const int cold = 10; // cold runs, numero de iteraciones que no cuentan (menor a reps)
        string s = "sa2.csv";
        ofstream output(s);
        output << "n,tsa,vsa \n";

        ifstream file("dna", std::ios::binary);

        const int txtsize = 1000000;
        string txt;
        txt.resize(txtsize);
        file.read(&txt[0], txtsize);
        vector<int> suffixArr2 = buildSuffixArray(txt);

        for (int i = txtsize / 10; i <= txtsize; i += txtsize / 10)
        {
            string pat;
            pat.resize(i);
            file.read(&pat[0], i);

            vector<double> s_times;
            for (int j = 0; j < reps; j++)
            {
                auto start = chrono::high_resolution_clock::now();
                int cnt = search(suffixArr2, txt, pat);
                auto end = chrono::high_resolution_clock::now();
                double s_time = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

                if (j < cold)
                    continue;
                s_times.push_back(s_time);
            }
            double avg_s_time = accumulate(s_times.begin(), s_times.end(), 0.0) / (reps - cold);
            double var_s_time = 0.0;
            for (double t : s_times)
                var_s_time += pow(t - avg_s_time, 2);
            var_s_time /= (reps - cold);

            cout << i << "," << avg_s_time << "," << var_s_time << "\n";
            output << i << "," << avg_s_time << "," << var_s_time << "\n";
        }
    }
    return 0;
}
