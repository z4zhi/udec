// Fuente para suffix array: https://www.geeksforgeeks.org/suffix-array-set-2-a-nlognlogn-algorithm/
// Fuente para search: https://en.wikipedia.org/wiki/Suffix_array#Applications
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// la idea principal es construir un suffix array, esto se puede hacer de varias formas.
// en esta implementacion se usan ranks, donde se comparan por los primeros 2 caracteres, luego 4, 8, etc. y se ordena.
// para la busqueda del patron, se hace con una busqueda binaria.

class SuffixArray
{
public:
    SuffixArray(const string &input)
    {
        T = input;
        suffixArr = buildSuffixArray();
    }

    int count(const string &p)
    {
        int l = 0;
        int r = suffixArr.size();

        // busqueda binaria para encontrar el indice donde empieza el intervalo que contiene el patron
        while (l < r)
        {
            int mid = (l + r) / 2;
            // comparar substring T que empieza en suffixArr[mid] con el patron
            int comparison = T.compare(suffixArr[mid], p.length(), p);
            // si p es mayor, mid es limite izquierdo
            comparison < 0 ? l = mid + 1 : r = mid;
        }
        int s = l; // indice donde comienza el patron en el suffix array

        r = suffixArr.size();
        // busqueda binaria para encontrar el indice donde termina el intervalo que contiene el patron
        while (l < r)
        {
            int mid = (l + r) / 2;
            int comparison = T.compare(suffixArr[mid], p.length(), p);
            // si hay match, seguir buscando
            comparison == 0 ? l = mid + 1 : r = mid;
        }
        int e = l; // donde termina

        return e - s; // la resta de el numero de ocurrencias
    }

    int space()
    {
        return suffixArr.size() * sizeof(int);
    }

private:
    string T;
    vector<int> suffixArr;

    // struct para guardar informacion del sufijo, ranks e indice
    // el segundo rank es para casos donde hay 2 con igual rank (desempatar)
    struct suffix
    {
        int index;
        int rank[2];
    };

    // funcion para comparacion de los ranks
    static int cmp(struct suffix a, struct suffix b)
    {
        return (a.rank[0] == b.rank[0]) ? (a.rank[1] < b.rank[1] ? 1 : 0) : (a.rank[0] < b.rank[0] ? 1 : 0);
    }

    // para crear el suffix array, se necesita el string
    vector<int> buildSuffixArray()
    {
        int n = T.length();
        // se crea un arreglo que guardara los n sufijos
        vector<suffix> suffixes(n);

        // guardar el indice del sufijo y su rank para 2 caracteres
        // si no es el ultimo sufijo guardar, sino -1
        for (int i = 0; i < n; i++)
        {
            suffixes[i].index = i;
            suffixes[i].rank[0] = T[i] - 'a';
            suffixes[i].rank[1] = ((i + 1) < n) ? (T[i + 1] - 'a') : -1;
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
};
