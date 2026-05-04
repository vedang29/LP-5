#include <omp.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <cstdlib>

using namespace std;

void sequentialBubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (arr[j] > arr[j + 1]) swap(arr[j], arr[j + 1]);
}

void parallelBubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n; i++) {
        if (i % 2 == 0) { // for i 0 2 4 6 ... even
#pragma omp parallel for
            for (int j = 0; j < n - 1; j += 2) {
                if (arr[j] > arr[j + 1]) swap(arr[j], arr[j + 1]);
            }
        } else { // for i 1 3 5 7 ... odd
#pragma omp parallel for
            for (int j = 1; j < n - 1; j += 2) {
                if (arr[j] > arr[j + 1]) swap(arr[j], arr[j + 1]);
            }
        }
    }
}

int main() {
    int N;
    cout << "Please enter number of elements: ";
    cin >> N;
    if (N <= 0) {
        cout << "Invalid input. Exiting." << endl;
        return 1;
    }
    omp_set_num_threads(4);

    vector<int> original(N);
    generate(original.begin(), original.end(), rand);
    vector<int> a1(N);
    copy(original.begin(), original.end(), a1.begin());
    vector<int> a2(N);
    copy(original.begin(), original.end(), a2.begin());
    double start, end;

    start = omp_get_wtime();
    sequentialBubbleSort(a1);
    end = omp_get_wtime();
    cout << "Sequential Bubble Sort: " << (end - start)*1000 << " ms\n";

    a1 = original;
    start = omp_get_wtime();
    parallelBubbleSort(a2);
    end = omp_get_wtime();
    cout << "Parallel Bubble Sort:   " << (end - start)*1000 << " ms\n";

    return 0;
}

// Compile with: g++ -fopenmp hpc2_bubble_sort.cpp
// Run with: ./a.out or ./a.exe

// input: 
// 15000

// 15000- number of elements
// output: Sequential Bubble Sort: 1814.24 ms
//         Parallel Bubble Sort:   839.567 ms




















// #include <iostream>
// #include <vector>
// #include <omp.h>
// #include <chrono>
// #include <cstdlib>

// using namespace std;
// using namespace chrono;

// // ---------- Utility ----------
// void printArray(vector<int>& arr) {
//     for (int i = 0; i < min(20, (int)arr.size()); i++)
//         cout << arr[i] << " ";
//     cout << "... \n";
// }

// // ---------- Sequential Bubble Sort ----------
// void bubbleSort(vector<int>& arr) {
//     int n = arr.size();
//     for (int i = 0; i < n - 1; i++) {
//         for (int j = 0; j < n - i - 1; j++) {
//             if (arr[j] > arr[j + 1])
//                 swap(arr[j], arr[j + 1]);
//         }
//     }
// }

// // ---------- Parallel Bubble Sort ----------
// void parallelBubbleSort(vector<int>& arr) {
//     int n = arr.size();

//     for (int i = 0; i < n; i++) {
//         if (i % 2 == 0) {
//             #pragma omp parallel for
//             for (int j = 0; j < n - 1; j += 2) {
//                 if (arr[j] > arr[j + 1])
//                     swap(arr[j], arr[j + 1]);
//             }
//         } else {
//             #pragma omp parallel for
//             for (int j = 1; j < n - 1; j += 2) {
//                 if (arr[j] > arr[j + 1])
//                     swap(arr[j], arr[j + 1]);
//             }
//         }
//     }
// }

// // ---------- Main ----------
// int main() {
//     int choice;
//     int n = 10000;

//     vector<int> original(n);
//     for (int i = 0; i < n; i++)
//         original[i] = rand() % 100000;

//     do {
//         cout << "\n--- BUBBLE SORT MENU ---\n";
//         cout << "1. Sequential Bubble Sort\n";
//         cout << "2. Parallel Bubble Sort\n";
//         cout << "3. Exit\n";
//         cout << "Enter choice: ";
//         cin >> choice;

//         vector<int> arr = original;

//         auto start = high_resolution_clock::now();

//         switch (choice) {
//             case 1:
//                 bubbleSort(arr);
//                 break;

//             case 2:
//                 parallelBubbleSort(arr);
//                 break;

//             case 3:
//                 return 0;

//             default:
//                 cout << "Invalid choice\n";
//                 continue;
//         }

//         auto end = high_resolution_clock::now();
//         auto duration = duration_cast<milliseconds>(end - start);

//         cout << "Sorted (first 20 elements): ";
//         printArray(arr);

//         cout << "Time Taken: " << duration.count() << " ms\n";

//     } while (choice != 3);

//     return 0;
// }
