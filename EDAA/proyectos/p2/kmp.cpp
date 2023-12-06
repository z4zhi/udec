// Fuente: https://en.wikipedia.org/wiki/Knuth%E2%80%93Morris%E2%80%93Pratt_algorithm
#include <vector>
#include <string>

using namespace std;

class KMP
{
public:
    KMP(const string &pattern)
    {
        p = pattern;
        lps = kmp_table();
    }

    int count(const string &T)
    {
        int j = 0; // posicion del caracter actual en T
        int k = 0; // posicion del caracter actual en p
        int cnt = 0;

        // recorrer todo el texto caracter por caracter
        while (j < T.length())
        {
            // si coinciden los caracteres
            if (p[k] == T[j])
            {
                j++;
                k++;
                // si se encuentra un patron
                if (k == p.length())
                {
                    cnt++;
                    // k retrocede con ayuda de la tabla kmp
                    k = lps[k];
                }
            }
            else
            {
                // si no coincide, k retrocede con ayuda de la tabla kmp
                // cuantos caracteres se pueden saltar, para no comparar innecesariamente
                k = lps[k];
                // si no puede retroceder (-1), se reinicia a 0 y se avanza j (string original)
                if (k < 0)
                    j++, k++;
            }
        }

        return cnt;
    }
    int space()
    {
        return lps.size() * sizeof(int);
    }

private:
    string p;
    vector<int> lps;
    // Crear la tabla KMP (LPS - Longest Prefix Suffix)
    // utilizado para evitar comparaciones inutiles
    vector<int> kmp_table()
    {
        int pos = 1; // posicion actual en la tabla, se parte desde 1 (para comparar p[1] con p[0])
        int cnd = 0; // longitud del sufijo que coincide con el prefijo en el patron

        vector<int> table(p.length() + 1);
        table[0] = -1; // valor especial, no hay salto si no coincide el primero

        while (pos < p.length())
        {
            // hay sufijo que coincide con el prefijo en el patron en la posicion pos.
            // esto hace que cuando no haya match en la posicion pos
            // el algoritmo debe saltar table[cnd] caracteres
            if (p[pos] == p[cnd])
                table[pos] = table[cnd];
            else
            {
                // esto hace que cuando no haya match en la posicion pos
                // el algoritmo debe saltar cnd caracteres
                table[pos] = cnd;
                // se retrocede en cnd utilizando los valores de la tabla hasta encontrar un sufijo que coincide con el prefijo en el patron.
                while (cnd >= 0 && p[pos] != p[cnd])
                    cnd = table[cnd];
            }
            pos++;
            cnd++;
        }

        table[pos] = cnd; // ultimo
        return table;
    }
};
