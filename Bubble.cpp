#include <iostream>
#include <vector>
#include <omp.h>
#include <chrono>
#include <cstdlib>

using namespace std;
using namespace chrono;

// ---------- Utility ----------
void printArray(vector<int>& arr) {
    for (int i = 0; i < min(20, (int)arr.size()); i++)
        cout << arr[i] << " ";
    cout << "... \n";
}

// ---------- Sequential Bubble Sort ----------
void bubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1])
                swap(arr[j], arr[j + 1]);
        }
    }
}

// ---------- Parallel Bubble Sort ----------
void parallelBubbleSort(vector<int>& arr) {
    int n = arr.size();

    for (int i = 0; i < n; i++) {
        if (i % 2 == 0) {
            #pragma omp parallel for
            for (int j = 0; j < n - 1; j += 2) {
                if (arr[j] > arr[j + 1])
                    swap(arr[j], arr[j + 1]);
            }
        } else {
            #pragma omp parallel for
            for (int j = 1; j < n - 1; j += 2) {
                if (arr[j] > arr[j + 1])
                    swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// ---------- Main ----------
int main() {
    int choice;
    int n = 10000;

    vector<int> original(n);
    for (int i = 0; i < n; i++)
        original[i] = rand() % 100000;

    do {
        cout << "\n--- BUBBLE SORT MENU ---\n";
        cout << "1. Sequential Bubble Sort\n";
        cout << "2. Parallel Bubble Sort\n";
        cout << "3. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        vector<int> arr = original;

        auto start = high_resolution_clock::now();

        switch (choice) {
            case 1:
                bubbleSort(arr);
                break;

            case 2:
                parallelBubbleSort(arr);
                break;

            case 3:
                return 0;

            default:
                cout << "Invalid choice\n";
                continue;
        }

        auto end = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end - start);

        cout << "Sorted (first 20 elements): ";
        printArray(arr);

        cout << "Time Taken: " << duration.count() << " ms\n";

    } while (choice != 3);

    return 0;
}
