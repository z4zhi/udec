#include <iostream>
#include <cstring>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <vector>
#include <cmath>
#include <numeric>

#include "kmp.cpp"
#include "sa.cpp"
#include "fmindex.cpp"

void read_inpt(string T, string p)
{

    KMP kmp(p);
    int countKMP = kmp.count(T);
    cout << "KMP: '" << p << "' en '" << T << "': " << countKMP << " ocurrencias" << endl;

    SuffixArray sa(T);
    int countSA = sa.count(p);
    cout << "Suffix Array: '" << p << "' en '" << T << "': " << countSA << " ocurrencias" << endl;

    FM_index index(T);
    int countFM = index.count(p);
    cout << "FM Index: '" << p << "' en '" << T << "': " << countFM << " ocurrencias" << endl;
}

void experiment()
{
    const int reps = 40; // reps totales incluyendo cold runs
    const int cold = 10; // cold runs, numero de iteraciones que no cuentan (menor a reps)
    string s = "1.csv";
    ofstream output(s);
    output << "n,t1kmp,v1kmp,t2kmp,v2kmp,skmp,t1sa,v1sa,t2sa,v2sa,ssa,t1fmi,v1fmi,t2fmi,v2fmi,sfmi \n";

    ifstream file("dna", std::ios::binary);
    string pattern;
    pattern.resize(1000);
    file.read(&pattern[0], 1000);

    const int N = 100000;
    for (int i = N / 10; i <= N; i += N / 10)
    {
        file.clear();
        file.seekg(0);
        string txt;
        txt.resize(i);
        file.read(&txt[0], i);

        KMP kmp(pattern);
        SuffixArray sa(txt);
        FM_index index(txt);

        vector<double> kmp1_times, kmp2_times, sa1_times, sa2_times, fmi1_times, fmi2_times;
        size_t size_kmp = kmp.space();
        size_t size_sa = sa.space();
        size_t size_index = index.space();

        for (int j = 0; j < reps; j++)
        {
            auto start = chrono::high_resolution_clock::now();
            int countKMP = kmp.count(txt);
            auto end = chrono::high_resolution_clock::now();
            double kmp1_time = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

            start = chrono::high_resolution_clock::now();
            int countSA = sa.count(pattern);
            end = chrono::high_resolution_clock::now();
            double sa_time = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

            start = chrono::high_resolution_clock::now();
            int countFMI = index.count(pattern);
            end = chrono::high_resolution_clock::now();
            double fmi_time = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

            if (j < cold)
                continue;
            kmp1_times.push_back(kmp1_time);
            sa1_times.push_back(sa_time);
            fmi1_times.push_back(fmi_time);
        }

        for (int j = 0; j < reps; j++)
        {
            auto start = chrono::high_resolution_clock::now();
            KMP kmp2(pattern);
            int countKMP2 = kmp2.count(txt);
            auto end = chrono::high_resolution_clock::now();
            double kmp2_time = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

            if (j < cold)
                continue;
            kmp2_times.push_back(kmp2_time);
        }
        for (int j = 0; j < reps; j++)
        {
            auto start = chrono::high_resolution_clock::now();
            SuffixArray sa2(txt);
            int countSA2 = sa2.count(pattern);
            auto end = chrono::high_resolution_clock::now();
            double sa2_time = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

            if (j < cold)
                continue;
            sa2_times.push_back(sa2_time);
        }

        for (int j = 0; j < reps; j++)
        {
            auto start = chrono::high_resolution_clock::now();
            FM_index index2(txt);
            int countFMI2 = index2.count(pattern);
            auto end = chrono::high_resolution_clock::now();
            double fmi2_time = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

            if (j < cold)
                continue;
            fmi2_times.push_back(fmi2_time);
        }
        double avg_kmp1_time = accumulate(kmp1_times.begin(), kmp1_times.end(), 0.0) / (reps - cold);
        double var_kmp1_time = 0.0;
        for (double t : kmp1_times)
            var_kmp1_time += pow(t - avg_kmp1_time, 2);
        var_kmp1_time /= (reps - cold);

        double avg_kmp2_time = accumulate(kmp2_times.begin(), kmp2_times.end(), 0.0) / (reps - cold);
        double var_kmp2_time = 0.0;
        for (double t : kmp2_times)
            var_kmp2_time += pow(t - avg_kmp2_time, 2);
        var_kmp2_time /= (reps - cold);
        double avg_sa1_time = accumulate(sa1_times.begin(), sa1_times.end(), 0.0) / (reps - cold);
        double var_sa1_time = 0.0;
        for (double t : sa1_times)
            var_sa1_time += pow(t - avg_sa1_time, 2);
        var_sa1_time /= (reps - cold);

        double avg_sa2_time = accumulate(sa2_times.begin(), sa2_times.end(), 0.0) / (reps - cold);
        double var_sa2_time = 0.0;
        for (double t : sa2_times)
            var_sa2_time += pow(t - avg_sa2_time, 2);
        var_sa2_time /= (reps - cold);

        double avg_fmi1_time = accumulate(fmi1_times.begin(), fmi1_times.end(), 0.0) / (reps - cold);
        double var_fmi1_time = 0.0;
        for (double t : fmi1_times)
            var_fmi1_time += pow(t - avg_fmi1_time, 2);
        var_fmi1_time /= (reps - cold);

        double avg_fmi2_time = accumulate(fmi2_times.begin(), fmi2_times.end(), 0.0) / (reps - cold);
        double var_fmi2_time = 0.0;
        for (double t : fmi2_times)
            var_fmi2_time += pow(t - avg_fmi2_time, 2);
        var_fmi2_time /= (reps - cold);

        cout << i << ","
             << avg_kmp1_time << "," << var_kmp1_time << "," << avg_kmp2_time << "," << var_kmp2_time << "," << size_kmp << ","
             << avg_sa1_time << "," << var_sa1_time << "," << avg_sa2_time << "," << var_sa2_time << "," << size_sa << ","
             << avg_fmi1_time << "," << var_fmi1_time << "," << avg_fmi2_time << "," << var_fmi2_time << "," << size_index << "\n";
        output << i << ","
               << avg_kmp1_time << "," << var_kmp1_time << "," << avg_kmp2_time << "," << var_kmp2_time << "," << size_kmp << ","
               << avg_sa1_time << "," << var_sa1_time << "," << avg_sa2_time << "," << var_sa2_time << "," << size_sa << ","
               << avg_fmi1_time << "," << var_fmi1_time << "," << avg_fmi2_time << "," << var_fmi2_time << "," << size_index << "\n";
    }

    string s2 = "2.csv";
    ofstream output2(s2);
    output2 << "n,t1kmp,v1kmp,t2kmp,v2kmp,skmp,t1sa,v1sa,t2sa,v2sa,ssa,t1fmi,v1fmi,t2fmi,v2fmi,sfmi \n";
    
    const int txtsize = 100000;
    file.clear();
    file.seekg(0);
    string txt;
    txt.resize(txtsize);
    file.read(&txt[0], txtsize);

    for (int i = txtsize / 10; i <= txtsize; i += txtsize / 10)
    {
        file.clear();
        file.seekg(0);
        string pat;
        pat.resize(i);
        file.read(&pat[0], i);

        KMP kmp(pat);
        SuffixArray sa(txt);
        FM_index index(txt);

        vector<double> kmp1_times, kmp2_times, sa1_times, sa2_times, fmi1_times, fmi2_times;
        size_t size_kmp = kmp.space();
        size_t size_sa = sa.space();
        size_t size_index = index.space();

        for (int j = 0; j < reps; j++)
        {
            auto start = chrono::high_resolution_clock::now();
            int countKMP = kmp.count(txt);
            auto end = chrono::high_resolution_clock::now();
            double kmp1_time = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

            start = chrono::high_resolution_clock::now();
            int countSA = sa.count(pat);
            end = chrono::high_resolution_clock::now();
            double sa_time = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

            start = chrono::high_resolution_clock::now();
            int countFMI = index.count(pat);
            end = chrono::high_resolution_clock::now();
            double fmi_time = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

            if (j < cold)
                continue;
            kmp1_times.push_back(kmp1_time);
            sa1_times.push_back(sa_time);
            fmi1_times.push_back(fmi_time);
        }

        for (int j = 0; j < reps; j++)
        {
            auto start = chrono::high_resolution_clock::now();
            KMP kmp2(pat);
            int countKMP2 = kmp2.count(txt);
            auto end = chrono::high_resolution_clock::now();
            double kmp2_time = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

            if (j < cold)
                continue;
            kmp2_times.push_back(kmp2_time);
        }
        for (int j = 0; j < reps; j++)
        {
            auto start = chrono::high_resolution_clock::now();
            SuffixArray sa2(txt);
            int countSA2 = sa2.count(pat);
            auto end = chrono::high_resolution_clock::now();
            double sa2_time = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

            if (j < cold)
                continue;
            sa2_times.push_back(sa2_time);
        }

        for (int j = 0; j < reps; j++)
        {
            auto start = chrono::high_resolution_clock::now();
            FM_index index2(txt);
            int countFMI2 = index2.count(pat);
            auto end = chrono::high_resolution_clock::now();
            double fmi2_time = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

            if (j < cold)
                continue;
            fmi2_times.push_back(fmi2_time);
        }
        double avg_kmp1_time = accumulate(kmp1_times.begin(), kmp1_times.end(), 0.0) / (reps - cold);
        double var_kmp1_time = 0.0;
        for (double t : kmp1_times)
            var_kmp1_time += pow(t - avg_kmp1_time, 2);
        var_kmp1_time /= (reps - cold);

        double avg_kmp2_time = accumulate(kmp2_times.begin(), kmp2_times.end(), 0.0) / (reps - cold);
        double var_kmp2_time = 0.0;
        for (double t : kmp2_times)
            var_kmp2_time += pow(t - avg_kmp2_time, 2);
        var_kmp2_time /= (reps - cold);
        double avg_sa1_time = accumulate(sa1_times.begin(), sa1_times.end(), 0.0) / (reps - cold);
        double var_sa1_time = 0.0;
        for (double t : sa1_times)
            var_sa1_time += pow(t - avg_sa1_time, 2);
        var_sa1_time /= (reps - cold);

        double avg_sa2_time = accumulate(sa2_times.begin(), sa2_times.end(), 0.0) / (reps - cold);
        double var_sa2_time = 0.0;
        for (double t : sa2_times)
            var_sa2_time += pow(t - avg_sa2_time, 2);
        var_sa2_time /= (reps - cold);

        double avg_fmi1_time = accumulate(fmi1_times.begin(), fmi1_times.end(), 0.0) / (reps - cold);
        double var_fmi1_time = 0.0;
        for (double t : fmi1_times)
            var_fmi1_time += pow(t - avg_fmi1_time, 2);
        var_fmi1_time /= (reps - cold);

        double avg_fmi2_time = accumulate(fmi2_times.begin(), fmi2_times.end(), 0.0) / (reps - cold);
        double var_fmi2_time = 0.0;
        for (double t : fmi2_times)
            var_fmi2_time += pow(t - avg_fmi2_time, 2);
        var_fmi2_time /= (reps - cold);

        cout << i << ","
             << avg_kmp1_time << "," << var_kmp1_time << "," << avg_kmp2_time << "," << var_kmp2_time << "," << size_kmp << ","
             << avg_sa1_time << "," << var_sa1_time << "," << avg_sa2_time << "," << var_sa2_time << "," << size_sa << ","
             << avg_fmi1_time << "," << var_fmi1_time << "," << avg_fmi2_time << "," << var_fmi2_time << "," << size_index << "\n";
        output2 << i << ","
               << avg_kmp1_time << "," << var_kmp1_time << "," << avg_kmp2_time << "," << var_kmp2_time << "," << size_kmp << ","
               << avg_sa1_time << "," << var_sa1_time << "," << avg_sa2_time << "," << var_sa2_time << "," << size_sa << ","
               << avg_fmi1_time << "," << var_fmi1_time << "," << avg_fmi2_time << "," << var_fmi2_time << "," << size_index << "\n";
    }
}

int main(int argc, char *argv[])
{
    if (argc == 3)
        read_inpt(argv[1], argv[2]);
    else
        experiment();

    return 0;
}