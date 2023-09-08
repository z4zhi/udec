#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <random>
#include <numeric>
#include <cmath>
#include <string>

using namespace std;

// Busqueda Secuencial
// Recorre el arreglo secuencialmente hasta encontrar el elemento buscado.
// Si no lo encuentra, retorna -1
int secuencial(const vector<int> &v, int n)
{
    int vsize = v.size();
    for (int i = 0; i < vsize; i++)
        if (v[i] == n)
            return i;
    return -1;
}

// Busqueda Binaria
// Requiere dos variables: 'l' y 'r', que son los indices que delimitan el rango donde se puede encontrar el elemento.
// En cada iteracion, se examina el elemento en el medio del rango (indice 'm').
// Si el elemento en el medio es igual al elemento buscado, se retorna el indice 'm'.
// Si el elemento no es igual al buscado, se determina si es mayor o menor al elemento del medio.
// Si es mayor, significa que esta en el intervalo desde el medio+1 hasta r.
// Si es menor, significa que esta en el intervalo desde l hasta el medio-1.
int binaria(const vector<int> &v, int n, int l, int r)
{
    while (l <= r)
    {
        int m = (r + l) >> 1;

        if (v[m] == n)
            return m;
        else if (v[m] < n)
            l = m + 1;
        else
            r = m - 1;
    }
    return -1;
}

// Busqueda Galopante (Exponencial)
// Se "divide" el arreglo en intervalos de potencias de 2: 1, 2, 4, 8...
// Luego recorre cada intervalo uno por uno y realiza lo siguiente:
// En cada intervalo, se establece una variable 'i' como el limite derecho del intervalo actual.
// Si el elemento en la posicion 'i' (limite derecho del intervalo actual) es menor,
// esto indica que el elemento buscado se encuentra en otro intervalo y es necesario continuar recorriendo intervalos de mayor tamaño.
// Si es mayor o igual, significa que esta dentro de este intervalo.
// Finalmente, se realiza una búsqueda binaria dentro de este intervalo para encontrar el elemento.
int galopante(const vector<int> &v, int n)
{
    int vsize = v.size();
    if (!vsize) // Si el tamaño es 0, no sirve
        return -1;

    int i = 1; // limite derecho

    // Si el limite derecho es menor al size del arreglo
    // y el valor del elemento en el limite derecho es menor al elemento buscado,
    // significa que esta en otro intervalo mas adelante. Multiplicar x2 el limite.
    while (i < vsize && v[i] < n)
        i <<= 1;

    // Busqueda binaria en el intervalo reducido
    // Parte desde i/2 (limite derecho anterior) hasta i (limite derecho actual)
    return binaria(v, n, i >> 1, min(i + 1, vsize));
}

int main(int argc, char *argv[])
{
    const int reps = 150; // reps totales incluyendo cold runs
    const int cold = 50;  // cold runs, numero de iteraciones que no cuentan (menor a reps)
    string s;
    s = argc > 1 ? "resultadospos.csv" : "resultadostam.csv";
    ofstream output(s);
    output << "n,TiempoSec,VarSec,TiempoBin,VarBin,TiempoExp,VarExp \n";

    if (argc == 1)
    {
        for (int i = 100; i <= 100000000; i *= 10)
        {
            vector<int> v(i);
            iota(v.begin(), v.end(), 0);
            vector<double> sec_times, bin_times, exp_times;
            int n = i + 1;
            for (int j = 0; j < reps; j++)
            {
                auto start = chrono::high_resolution_clock::now();
                int sec = secuencial(v, n);
                auto end = chrono::high_resolution_clock::now();
                double sec_time = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

                start = chrono::high_resolution_clock::now();
                int bin = binaria(v, n, 0, i - 1);
                end = chrono::high_resolution_clock::now();
                double bin_time = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

                start = chrono::high_resolution_clock::now();
                int exp = galopante(v, n);
                end = chrono::high_resolution_clock::now();
                double exp_time = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

                if (sec != bin || bin != exp || sec != exp)
                    break;

                if (j < cold)
                    continue;
                sec_times.push_back(sec_time);
                bin_times.push_back(bin_time);
                exp_times.push_back(exp_time);
                //     cout << "size: " << i << " rep: " << j << " n: " << n << " sec: " << sec << " bin: " << bin << " exp: " << exp << '\n';
            }
            double avg_sec_time = accumulate(sec_times.begin(), sec_times.end(), 0.0) / (reps - cold);
            double var_sec_time = 0.0;
            for (double t : sec_times)
                var_sec_time += pow(t - avg_sec_time, 2);
            var_sec_time /= (reps - cold);

            double avg_bin_time = accumulate(bin_times.begin(), bin_times.end(), 0.0) / (reps - cold);
            double var_bin_time = 0.0;
            for (double t : bin_times)
                var_bin_time += pow(t - avg_bin_time, 2);
            var_bin_time /= (reps - cold);

            double avg_exp_time = accumulate(exp_times.begin(), exp_times.end(), 0.0) / (reps - cold);
            double var_exp_time = 0.0;
            for (double t : exp_times)
                var_exp_time += pow(t - avg_exp_time, 2);
            var_exp_time /= (reps - cold);

            cout << i << "," << avg_sec_time << "," << var_sec_time << "," << avg_bin_time << "," << var_bin_time << "," << avg_exp_time << "," << var_exp_time << "\n";
            output << i << "," << avg_sec_time << "," << var_sec_time << "," << avg_bin_time << "," << var_bin_time << "," << avg_exp_time << "," << var_exp_time << "\n";
        }
    }
    else
    {
        const int vsize = 100000000;
        vector<int> v(vsize + 1);
        iota(v.begin(), v.end(), 0);
        for (int i = 0; i <= vsize; i += vsize / 10)
        {
            vector<double> sec_times, bin_times, exp_times;
            int n = i;
            for (int j = 0; j < reps; j++)
            {
                auto start = chrono::high_resolution_clock::now();
                int sec = secuencial(v, n);
                auto end = chrono::high_resolution_clock::now();
                double sec_time = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

                start = chrono::high_resolution_clock::now();
                int bin = binaria(v, n, 0, vsize);
                end = chrono::high_resolution_clock::now();
                double bin_time = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

                start = chrono::high_resolution_clock::now();
                int exp = galopante(v, n);
                end = chrono::high_resolution_clock::now();
                double exp_time = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

                if (sec != bin || bin != exp || sec != exp)
                {
                    cout << "error"
                         << " p: " << n << " sec: " << sec << " bin: " << bin << " exp: " << exp << '\n';
                    break;
                }

                if (j < cold)
                    continue;
                sec_times.push_back(sec_time);
                bin_times.push_back(bin_time);
                exp_times.push_back(exp_time);

                // cout << "pos: " << i << " rep: " << j << " p: " << n << " sec: " << sec << " bin: " << bin << " exp: " << exp << '\n';
            }
            double avg_sec_time = accumulate(sec_times.begin(), sec_times.end(), 0.0) / (reps - cold);
            double var_sec_time = 0.0;
            for (double t : sec_times)
                var_sec_time += pow(t - avg_sec_time, 2);
            var_sec_time /= (reps - cold);

            double avg_bin_time = accumulate(bin_times.begin(), bin_times.end(), 0.0) / (reps - cold);
            double var_bin_time = 0.0;
            for (double t : bin_times)
                var_bin_time += pow(t - avg_bin_time, 2);
            var_bin_time /= (reps - cold);

            double avg_exp_time = accumulate(exp_times.begin(), exp_times.end(), 0.0) / (reps - cold);
            double var_exp_time = 0.0;
            for (double t : exp_times)
                var_exp_time += pow(t - avg_exp_time, 2);
            var_exp_time /= (reps - cold);

            cout << i << "," << avg_sec_time << "," << var_sec_time << "," << avg_bin_time << "," << var_bin_time << "," << avg_exp_time << "," << var_exp_time << "\n";
            output << i << "," << avg_sec_time << "," << var_sec_time << "," << avg_bin_time << "," << var_bin_time << "," << avg_exp_time << "," << var_exp_time << "\n";
        }
    }
    output.close();
    return 0;
}
