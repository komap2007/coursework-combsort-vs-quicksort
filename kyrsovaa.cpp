#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>
using namespace std;

unsigned long long comb_sort(vector<int>& a) {
    unsigned long long op_counter = 0;
    int n = a.size();
    int gap = n;
    bool swapped = true;
    while (gap > 1 || swapped) {
        gap = int(gap / 1.247);
        if (gap < 1) gap = 1;
        swapped = false;
        for (int i = 0; i + gap < n; i++) {
            op_counter++;
            if (a[i] > a[i + gap]) {
                swap(a[i], a[i + gap]);
                swapped = true;
            }
        }
    }
    return op_counter;
}

unsigned long long quick_sort(vector<int>& a, int left, int right) {
    unsigned long long op_counter = 0;
    if (left >= right) return 0;

    int i = left, j = right;
    int pivot = a[(left + right) / 2];

    while (i <= j) {
        while (a[i] < pivot) { op_counter++; i++; }
        while (a[j] > pivot) { op_counter++; j--; }
        if (i <= j) {
            swap(a[i], a[j]);
            i++;
            j--;
        }
    }

    op_counter += quick_sort(a, left, j);
    op_counter += quick_sort(a, i, right);
    return op_counter;
}

vector<int> generate_random_array(int n, mt19937& gen) {
    vector<int> arr(n);
    uniform_int_distribution<int> dist(-1000000, 1000000);
    for (int i = 0; i < n; i++) {
        arr[i] = dist(gen);
    }
    return arr;
}

int main() {
    setlocale(LC_ALL, "Russian");

    vector<int> sizes = { 100, 500, 1000, 2000, 5000, 10000, 20000, 50000 };
    const int REPEATS = 10000;

    random_device rd;
    mt19937 gen(rd());

    ofstream out("results.csv");
    out << "Size;CombSort_Ops;QuickSort_Ops\n";

    cout << "Size\tComb(ops)\tQuick(ops)\n";
    cout << "------------------------------------\n";

    for (int n : sizes) {
        unsigned long long comb_ops_total = 0;
        unsigned long long quick_ops_total = 0;

        for (int rep = 0; rep < REPEATS; rep++) {
            vector<int> original = generate_random_array(n, gen);

            // --- Comb Sort ---
            vector<int> arr1 = original;
            unsigned long long ops1 = comb_sort(arr1);
            comb_ops_total += ops1;

            // --- Quicksort ---
            vector<int> arr2 = original;
            unsigned long long ops2 = quick_sort(arr2, 0, n - 1);
            quick_ops_total += ops2;
        }

        unsigned long long comb_avg_ops = comb_ops_total / REPEATS;
        unsigned long long quick_avg_ops = quick_ops_total / REPEATS;

        cout << n << "\t"
            << comb_avg_ops << "\t\t"
            << quick_avg_ops << "\n";

        out << n << ";"
            << comb_avg_ops << ";"
            << quick_avg_ops << "\n";
    }

    out.close();
    cout << "\nРезультаты сохранены в файл results.csv\n";

    return 0;
}