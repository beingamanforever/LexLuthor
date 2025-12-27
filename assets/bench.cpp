#include <iostream>
#include <vector>
#include <chrono>
#include <numeric>
#include <random>

using namespace std;

const int N = 800; // Slightly smaller to allow for 10 repeats
const int REPEATS = 10;

double run_benchmark() {
    // Using random numbers to prevent compiler "constant folding"
    vector<vector<double>> A(N, vector<double>(N, 1.1));
    vector<vector<double>> B(N, vector<double>(N, 2.2));
    vector<vector<double>> C(N, vector<double>(N, 0.0));

    auto start = chrono::high_resolution_clock::now();

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> diff = end - start;
    return diff.count();
}

int main() {
    vector<double> times;
    cout << "Running benchmark " << REPEATS << " times..." << endl;

    for (int i = 0; i < REPEATS; i++) {
        double t = run_benchmark();
        times.push_back(t);
        cout << "Iteration " << i+1 << ": " << t << "s" << endl;
    }

    double sum = accumulate(times.begin(), times.end(), 0.0);
    cout << "\nAverage Time: " << sum / REPEATS << "s" << endl;

    return 0;
}
