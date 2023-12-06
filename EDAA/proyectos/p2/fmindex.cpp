// Fuente BWT: https://en.wikipedia.org/wiki/Burrows%E2%80%93Wheeler_transform
// Fuente FM-index: https://en.wikipedia.org/wiki/FM-index
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

// para construir el FM-index, necesitamos el string original
// luego, se hace transforma el string a BWT
// tambien, se crea una tabla C que necesita el alfabeto del string y que guarda valores
// se crea tambien la funcion occ que necesita la BWT, en este caso se implemento como tabla que guarda valores
// finalmente, teniendo C y Occ, se puede hacer el calculo LF(i) o count(p)
class FM_index
{
public:
    FM_index(const string &input)
    {
        bwt = BWT(input);
        C = CTable();
        occ = OccTable();
    }

    // mapea la posicion del bwt con el string original
    // para la funcion LF se necesita la tabla C, Occ y L[i]
    // solo basta con sacar la informacion de las tablas y sumarlas
    // en este caso estamos con indice desde 0, por eso se resta i-1
    int LF(const int i)
    {
        string L = bwt;
        char c = L[i - 1];
        int C_Li = C.at(L[i - 1]);
        int Occ_Li = occ.at(c)[i - 1];
        return C_Li + Occ_Li;
    }

    // como esta ordenado lexicograficamente, basta con encontrar el rango donde se encuentra el patron
    // no es necesario ir "saltando"
    // para contar las ocurrencias necesitamos el patron y las 2 tablas
    // necesitamos encontrar el rango donde se encuentra las ocurrencias del patron, ya que esta todo ordenado
    // para ello, se recorre el patron de atras hacia adelante
    // se inicializa el rango con el (valor C) + 1 del caracter actual (ultimo del patron)
    // y el caracter presente en el alfabeto que le sigue lexicograficamente a este caracter actual
    // esto obtiene el rango de todas las combinaciones que empiezan con el ultimo caracter del patron
    // luego en cada iteracion se algo similar al LF, peron con valor C del caracter actual del patron
    // y con el occ del start anterior -1 y occ del end anterior
    int count(const string &p)
    {
        int psize = p.length();
        int start, end; // rango

        for (int i = psize - 1; i >= 0; i--)
        {
            char c = p[i];
            int C_c = C.at(c);
            // inicializacion
            if (i == psize - 1)
            {
                start = C_c + 1;
                // obtener siguiente caracter que pertenece al alfabeto y le sigue lexicograficamente al actual
                auto nextCharIterator = C.upper_bound(c);
                if (nextCharIterator != C.end())
                {
                    char next = nextCharIterator->first;
                    end = C.at(next);
                }
                else
                    end = bwt.size();
            }
            else
            {
                start = C_c + occ.at(c)[start - 2] + 1;
                end = C_c + occ.at(c)[end - 1];
            }

            if (start > end)
                return 0;
        }

        return end - start + 1;
    }

    int space()
    {
        int size_C = C.size() * (sizeof(char) + sizeof(int));
        int size_occ = occ.size() * (sizeof(char) + sizeof(int) * bwt.length());

        int total_space = size_C + size_occ;

        return total_space;
    }

private:
    string bwt;
    map<char, int> C;
    map<char, vector<int>> occ;

    // para construir el bwt se necesita el string original y agregar el signo $ (terminacion) al final
    // luego se crean todas las rotaciones y se ordena
    // finalmente, se toma el ultimo caracter de cada rotacion
    string BWT(const string &str)
    {
        vector<string> rotations;
        string s = str + '$';
        int n = s.length();

        for (int i = 0; i < n; i++)
            rotations.push_back(s.substr(i) + s.substr(0, i));

        sort(rotations.begin(), rotations.end());

        string bwt;
        for (const string &rotation : rotations)
            bwt.push_back(rotation.back());

        return bwt;
    }

    // para crear la tabla C, se necesita el bwt o el string
    // luego se ordena el string
    // para cada caracter del string
    // verificar si el caracter actual es distinto al anterior
    // si es asi, registrar en el mapa y actualizar prev
    // finalmente, aumentar el count
    map<char, int> CTable()
    {
        map<char, int> C;
        string sorted = bwt;
        sort(sorted.begin(), sorted.end());

        char prev = '\0';
        int count = 0;
        for (char c : sorted)
        {
            if (c != prev)
            {
                C[c] = count;
                prev = c;
            }
            count++;
        }

        return C;
    }

    // para construir la funcion Occ hay 2 formas:
    // con la tabla, que ocupa tiempo y espacio para construir, pero busqueda constante
    // o con una funcion que recorre el bwt a 0...k (ineficiente si son muchas llamadas)
    // para construir la tabla se necesita el alfabeto y el bwt
    // para esta implementacion se usa un mapa que guarda el caracter y un vector con los valores
    // para cada caracter, se recorre todo el bwt y se cuenta
    map<char, vector<int>> OccTable()
    {
        map<char, vector<int>> occ;
        int ssize = bwt.size();

        for (auto &entry : C)
        {
            char c = entry.first;
            occ[c] = vector<int>(ssize, 0);
        }

        for (auto &entry : C)
        {
            char c = entry.first;
            int count = 0;

            for (int i = 0; i < ssize; i++)
            {
                if (bwt[i] == c)
                    count++;
                occ[c][i] = count;
            }
        }

        return occ;
    }
};
