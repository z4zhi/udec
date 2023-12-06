// Fuente: https://www.geeksforgeeks.org/kmp-algorithm-for-pattern-searching/
#include <bits/stdc++.h>
using namespace std;

// la idea principal es tener un arreglo (tabla) lps que indica
// la longitud del prefijo mas largo que tambien es sufijo (lps) en cada posicion del patron.
// por ejemplo, lps[i] nos dice la longitud del prefijo mas largo que es tambien un sufijo de pat[0..i].
// esto se puede usar para evitar comparaciones innecesarias durante la busqueda del patron en el texto.

int KMPSearch(const string &pat, const string &txt, const vector<int> &lps)
{
    int M = pat.length();
    int N = txt.length();
    int cnt = 0;

    int i = 0; // index for txt[]
    int j = 0; // index for pat[]
    // mientras haya caracteres en el texto para continuar comparando
    while ((N - i) >= (M - j))
    {
        // comparar
        if (pat[j] == txt[i])
        {
            j++;
            i++;
        }

        // si alcanza el size del pattern
        if (j == M)
        {
            cnt++;
            // actualizar el lps
            j = lps[j - 1];
        }

        // si hay un mismatch despues de j matches
        else if (i < N && pat[j] != txt[i])
        {
            // Do not match lps[0..lps[j-1]] characters, they will match anyway

            // actualizar j con el lps para no hacer comparaciones innecesarias
            if (j != 0)
                j = lps[j - 1];
            // si es 0 es porque no hay match anterior
            else
                i = i + 1;
        }
    }
    return cnt;
}

// calcular el arreglo LPS (Longest Prefix Suffix) para un patron
// cuantos caracteres se puede avanzar en el pattern despues de un mismatch
void computeLPSArray(const string &pat, vector<int> &lps)
{
    int M = pat.length();
    // tamaño del prefijo que es sufijo anterior mas largo
    int len = 0;
    lps[0] = 0;

    int i = 1;
    while (i < M)
    {
        // si son iguales, hay un LPS
        if (pat[i] == pat[len])
        {
            len++;
            lps[i] = len;
            i++;
        }
        else
        {
            // This is tricky. Consider the example.
            // AAACAAAA and i = 7. The idea is similar to search step.

            // se esta construyendo el lps, retroceder
            if (len != 0)
            {
                len = lps[len - 1];
            }
            else
            // no se ha encontrado un LPS todavia
            {
                lps[i] = 0;
                i++;
            }
        }
    }
}
int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        string txt = "ABABDABACDABABCABAB";
        string pat = "ABABCABAB";
        int M = pat.length();
        vector<int> lps(M);
        computeLPSArray(pat, lps);
        int count1 = KMPSearch(pat, txt, lps);
        cout << count1 << '\n';

        string txt2 = "banana";
        string pat2 = "ana";
        int M2 = pat2.length();
        vector<int> lps2(M2, 0);
        computeLPSArray(pat2, lps2);
        int count2 = KMPSearch(pat2, txt2, lps2);
        cout << count2 << '\n';
    }
    else if (argc == 2)
    {
        const int reps = 40; // reps totales incluyendo cold runs
        const int cold = 10; // cold runs, numero de iteraciones que no cuentan (menor a reps)
        string s = "kmp1.csv";
        ofstream output(s);
        output << "n,tkmp,vkmp \n";

        ifstream file("dna", std::ios::binary);

        string pattern;
        pattern.resize(1000);
        file.read(&pattern[0], 100);
        vector<int> lps(1000);
        computeLPSArray(pattern, lps);

        const int N = 1000000;
        for (int i = N / 10; i <= N; i += N / 10)
        {
            string txt;
            txt.resize(i);
            file.read(&txt[0], i);

            vector<double> s_times;
            for (int j = 0; j < reps; j++)
            {
                auto start = chrono::high_resolution_clock::now();
                int cnt = KMPSearch(pattern, txt, lps);
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
        string s = "kmp2.csv";
        ofstream output(s);
        output << "n,tkmp,vkmp \n";

        ifstream file("dna", std::ios::binary);
        const int txtsize = 1000000;
        string txt;
        txt.resize(txtsize);
        file.read(&txt[0], txtsize);

        for (int i = txtsize / 10; i <= txtsize; i += txtsize / 10)
        {
            string pattern;
            pattern.resize(i);
            file.read(&pattern[0], i);
            vector<int> lps(i);
            computeLPSArray(pattern, lps);

            vector<double> s_times;
            for (int j = 0; j < reps; j++)
            {
                auto start = chrono::high_resolution_clock::now();
                int cnt = KMPSearch(pattern, txt, lps);
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