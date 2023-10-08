#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
#include <chrono>
#include <numeric>
#include <cmath>

using namespace std;

struct BinaryHeap
{
    vector<int> heap;

    void insert(const int val)
    {
        heap.push_back(val);                 // Insertar el elemento
        push_heap(heap.begin(), heap.end()); // Arreglar el heap con el elemento insertado
    }

    void remove(const int val)
    {
        auto it = find(heap.begin(), heap.end(), val); // Buscar elemento
        if (it != heap.end())                          // si lo encuentra
        {
            heap.erase(it);                      // lo elimina del heap
            make_heap(heap.begin(), heap.end()); // Luego, se arregla el heap
        }
    }

    static BinaryHeap merge(const BinaryHeap &heap1, const BinaryHeap &heap2)
    {
        BinaryHeap new_heap = heap1;

        // Insertar los elementos del heap2 al nuevo heap que contiene elementos del heap1
        for (int i = 0; i < heap2.heap.size(); i++)
            new_heap.heap.push_back(heap2.heap[i]);

        make_heap(new_heap.heap.begin(), new_heap.heap.end()); // arreglar el heap
        return new_heap;
    }

    int getMax()
    {
        return heap.empty() ? -1 : heap.front();
    }

    void removeMax()
    {
        if (!heap.empty())
        {
            pop_heap(heap.begin(), heap.end()); // Poner el elemento mayor al final y arreglar el heap
            heap.pop_back();                    // y luego pop_back para quitar ese elemento
        }
    }
};

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        BinaryHeap heap1, heap2;
        srand(static_cast<unsigned>(time(nullptr)));

        for (int i = 0; i < 10; i++)
            heap1.insert(rand() % 1000),
                heap2.insert(rand() % 1000);
        heap1.insert(0), heap2.insert(0);
        heap1.insert(500), heap2.insert(500);
        heap1.insert(1000), heap2.insert(1000);

        cout << "Heap1: ";
        for (int num : heap1.heap)
            cout << num << " ";
        cout << endl;

        cout << "Heap2: ";
        for (int num : heap2.heap)
            cout << num << " ";
        cout << endl;

        BinaryHeap merged_heap = BinaryHeap::merge(heap1, heap2);
        cout << "Merged: ";
        for (int num : merged_heap.heap)
            cout << num << " ";
        cout << endl;

        cout << "getMax, removeMax: ";
        while (!merged_heap.heap.empty())
            cout << merged_heap.getMax() << " ",
                merged_heap.removeMax();
        cout << endl;
        heap1.remove(0), heap2.remove(0);
        heap1.remove(500), heap2.remove(500);
        heap1.remove(1000), heap2.remove(1000);

        cout << "Heap1: ";
        for (int num : heap1.heap)
            cout << num << " ";
        cout << endl;

        cout << "Heap2: ";
        for (int num : heap2.heap)
            cout << num << " ";
        cout << endl;
    }
    else
    {
        const int reps = 40; // reps totales incluyendo cold runs
        const int cold = 10; // cold runs, numero de iteraciones que no cuentan (menor a reps)
        string s = "binary.csv";
        ofstream output(s);
        output << "n,tInsert,vInsert,tRemove,vRemove,tUnion,vUnion \n";

        const int N = 100000000;
        for (int i = 100; i <= N; i *= 10)
        {
            BinaryHeap heap1;
            for (int k = 0; k < i; k++)
                heap1.insert(k);
            vector<double> i_times, r_times, u_times;
            for (int j = 0; j < reps; j++)
            {
                auto start = chrono::high_resolution_clock::now();
                heap1.insert(i);
                auto end = chrono::high_resolution_clock::now();
                double i_time = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

                start = chrono::high_resolution_clock::now();
                heap1.removeMax();
                end = chrono::high_resolution_clock::now();
                double r_time = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

                start = chrono::high_resolution_clock::now();
                BinaryHeap merged = BinaryHeap::merge(heap1, heap1);
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