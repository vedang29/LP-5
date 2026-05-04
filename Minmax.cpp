// g++ -fopenmp Minmax.cpp -o Minmax
// ./Minmax


#include <iostream>
#include <vector>
#include <cstdlib>
#include <omp.h>

using namespace std;

int main() {

    int n = 10000;
    vector<int> nums(n);

    for (int i = 0; i < n; i++) {
        nums[i] = rand() % 10000;
    }

    long long sum_par = 0;
    int min_par = nums[0], max_par = nums[0];
    double start, end;

    start = omp_get_wtime();

    #pragma omp parallel for reduction(min:min_par) reduction(max:max_par) reduction(+:sum_par)
    for (int i = 0; i < n; i++) {

        if (nums[i] < min_par)
            min_par = nums[i];

        if (nums[i] > max_par)
            max_par = nums[i];

        sum_par += nums[i];
    }

    end = omp_get_wtime();

    double avg_par = (double)sum_par / n;

    cout << "Minimum : " << min_par << endl;
    cout << "Maximum : " << max_par << endl;
    cout << "Sum     : " << sum_par << endl;
    cout << "Average : " << avg_par << endl;
    cout << "Time    : " << (end - start) << " sec" << endl;

    return 0;
}
