#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <fstream>
#include <chrono>
#include <array>

using namespace std;

int main() {
    cin.tie(nullptr)->sync_with_stdio(false);

    int N, step_interval;
    double J, T;
    long long STEPS;
    
    if (!(cin >> N >> J >> T >> STEPS >> step_interval)) return 1;

    auto start_time = chrono::high_resolution_clock::now();

    random_device rd;
    mt19937 gen(rd());
    
    uniform_int_distribution<int> dis_grid(0, N * N - 1);
    uniform_real_distribution<double> dis_rand(0.0, 1.0);

    array<double, 9> exp_table{};
    exp_table[2 + 4] = exp(-2.0 * J * 2 / T);
    exp_table[4 + 4] = exp(-2.0 * J * 4 / T);

    vector<int> grid(N * N);
    long long total_spin = 0;

    for (int i = 0; i < N * N; ++i) {
        int spin = (gen() % 2 == 0) ? 1 : -1;
        grid[i] = spin;
        total_spin += spin;
    }

    ofstream outFile{"ising_data.txt"};
    if (!outFile.is_open()) {
        cerr << "Error opening file!\n";
        return 1;
    }
    outFile << N << " " << J << " " << T << " " << STEPS << " " << step_interval << "\n";

    for (long long step = 0; step < STEPS; ++step) {
        int idx = dis_grid(gen);
        int i = idx / N;
        int j = idx % N;
        int s = grid[idx];

        int top    = grid[(i == 0 ? N - 1 : i - 1) * N + j];
        int bottom = grid[(i == N - 1 ? 0 : i + 1) * N + j];
        int left   = grid[i * N + (j == 0 ? N - 1 : j - 1)];
        int right  = grid[i * N + (j == N - 1 ? 0 : j + 1)];

        int s_neighbors = s * (top + bottom + left + right);

        bool flip = false;
        if (s_neighbors <= 0) {
            flip = true;
        } else {
            if (dis_rand(gen) < exp_table[s_neighbors + 4]) {
                flip = true;
            }
        }

        if (flip) {
            grid[idx] = -s;
            total_spin += 2 * (-s);
        }

        if (step % step_interval == 0) {
            double magnetization = static_cast<double>(total_spin) / (N * N);
            
            outFile << step << " " << magnetization << "\n";
            for (int spin : grid) {
                outFile << spin << " ";
            }
            outFile << "\n";

            cout << "Step: " << step << ", Magnetization: " << magnetization << "\n";
        }
    }

    outFile.close();

    auto end_time = chrono::high_resolution_clock::now();
    chrono::duration<double, std::milli> duration = end_time - start_time;

    cout << "Time taken: " << duration.count() << " ms\n";
    cout << "Results saved to ising_data.txt\n";

    return 0;
}