#include <iostream>
#include <vector>
#include <algorithm>
#include <omp.h>
#include <cstdlib>
using namespace std;

void merge(vector<int>& arr, int left, int mid, int right) {
    vector<int> leftPart(arr.begin() + left, arr.begin() + mid + 1);
    vector<int> rightPart(arr.begin() + mid + 1, arr.begin() + right + 1);

    int i = 0, j = 0, k = left;
    while (i < leftPart.size() && j < rightPart.size()) {
        if (leftPart[i] <= rightPart[j])
            arr[k++] = leftPart[i++];
        else
            arr[k++] = rightPart[j++];
    }

    while (i < leftPart.size()) arr[k++] = leftPart[i++];
    while (j < rightPart.size()) arr[k++] = rightPart[j++];
}

void sequentialMergeSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;
        sequentialMergeSort(arr, left, mid);
        sequentialMergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

void parallelMergeSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;

        if (abs(mid - left) >= 100) {  // if array is large enough(atleast 100 elements), use parallelism
            #pragma omp parallel sections
            {
                #pragma omp section
                parallelMergeSort(arr, left, mid);
                #pragma omp section
                parallelMergeSort(arr, mid + 1, right);
            }
        } else { // for smaller array use sequential 
            sequentialMergeSort(arr, left, mid);
            sequentialMergeSort(arr, mid + 1, right);
        }

        merge(arr, left, mid, right);
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

    vector<int> original(N);
    generate(original.begin(), original.end(), rand);
    vector<int> a1(N);
    copy(original.begin(), original.end(), a1.begin());
    vector<int> a2(N);
    copy(original.begin(), original.end(), a2.begin());
    double start,end;
    
    start = omp_get_wtime();
    sequentialMergeSort(a1, 0, N - 1);
    end = omp_get_wtime();
    cout << "Sequential Merge Sort:  " << (end - start)*1000 << " ms\n";

    start = omp_get_wtime();
    parallelMergeSort(a2, 0, N - 1);
    end = omp_get_wtime();
    cout << "Parallel Merge Sort:    " << (end - start)*1000 << " ms\n";

    return 0;
}

// Compile with: g++ -fopenmp hpc2_merge_sort.cpp
// Run with: ./a.out or ./a.exe

// input:
// 15000

// 15000- number of elements
















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

// // ---------- Merge ----------
// void merge(vector<int>& arr, int l, int m, int r) {
//     vector<int> temp;
//     int i = l, j = m + 1;

//     while (i <= m && j <= r) {
//         if (arr[i] < arr[j])
//             temp.push_back(arr[i++]);
//         else
//             temp.push_back(arr[j++]);
//     }

//     while (i <= m) temp.push_back(arr[i++]);
//     while (j <= r) temp.push_back(arr[j++]);

//     for (int k = l; k <= r; k++)
//         arr[k] = temp[k - l];
// }

// // ---------- Sequential Merge Sort ----------
// void mergeSort(vector<int>& arr, int l, int r) {
//     if (l >= r) return;

//     int m = (l + r) / 2;
//     mergeSort(arr, l, m);
//     mergeSort(arr, m + 1, r);
//     merge(arr, l, m, r);
// }

// // ---------- Parallel Merge Sort ----------
// void parallelMergeSort(vector<int>& arr, int l, int r, int depth = 0) {
//     if (l >= r) return;

//     int m = (l + r) / 2;

//     if (depth < 4) {
//         #pragma omp parallel sections
//         {
//             #pragma omp section
//             parallelMergeSort(arr, l, m, depth + 1);

//             #pragma omp section
//             parallelMergeSort(arr, m + 1, r, depth + 1);
//         }
//     } else {
//         mergeSort(arr, l, m);
//         mergeSort(arr, m + 1, r);
//     }

//     merge(arr, l, m, r);
// }

// // ---------- Main ----------
// int main() {
//     int choice;
//     int n = 10000;

//     vector<int> original(n);
//     for (int i = 0; i < n; i++)
//         original[i] = rand() % 100000;

//     do {
//         cout << "\n--- MERGE SORT MENU ---\n";
//         cout << "1. Sequential Merge Sort\n";
//         cout << "2. Parallel Merge Sort\n";
//         cout << "3. Compare Sequential vs Parallel\n";
//         cout << "4. Exit\n";
//         cout << "Enter choice: ";
//         cin >> choice;

//         vector<int> arr = original;

//         auto start = high_resolution_clock::now();

//         switch (choice) {
//             case 1:
//                 mergeSort(arr, 0, n - 1);
//                 break;

//             case 2:
//                 parallelMergeSort(arr, 0, n - 1);
//                 break;

//             case 3: {
//                 vector<int> a1 = original;
//                 vector<int> a2 = original;

//                 auto s1 = high_resolution_clock::now();
//                 mergeSort(a1, 0, n - 1);
//                 auto e1 = high_resolution_clock::now();

//                 auto s2 = high_resolution_clock::now();
//                 parallelMergeSort(a2, 0, n - 1);
//                 auto e2 = high_resolution_clock::now();

//                 auto t1 = duration_cast<milliseconds>(e1 - s1);
//                 auto t2 = duration_cast<milliseconds>(e2 - s2);

//                 cout << "\nSequential Time: " << t1.count() << " ms\n";
//                 cout << "Parallel Time: " << t2.count() << " ms\n";

//                 cout << "Speedup: "
//                      << (double)t1.count() / t2.count() << "x\n";
//                 continue;
//             }

//             case 4:
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

//     } while (choice != 4);

//     return 0;
// }
