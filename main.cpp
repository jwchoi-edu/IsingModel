#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <fstream>
#include <chrono>
#include <array>
#include <string>
#include <iomanip>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 6) {
        cerr << "Usage: " << argv[0] << " <N> <J> <T> <steps> <step_interval>\n";
        return 1;
    }

    int N = stoi(argv[1]);
    double J = stod(argv[2]);
    double T = stod(argv[3]);
    long long STEPS = stoll(argv[4]);
    int step_interval = stoi(argv[5]);

    cin.tie(nullptr)->sync_with_stdio(false);

    auto start_time = chrono::high_resolution_clock::now();

    random_device rd;
    mt19937 gen(rd());
    
    uniform_int_distribution<int> dis_coord(0, N - 1);
    uniform_real_distribution<double> dis_rand(0.0, 1.0);

    array<double, 9> exp_table{};
    exp_table[2 + 4] = exp(-4.0 * J / T);
    exp_table[4 + 4] = exp(-8.0 * J / T);

    vector<int> grid(N * N);
    long long total_spin = 0;

    for (int i = 0; i < N * N; ++i) {
        int spin = (gen() % 2 == 0) ? 1 : -1;
        grid[i] = spin;
        total_spin += spin;
    }
auto now = chrono::system_clock::now();
    auto time_t_now = chrono::system_clock::to_time_t(now);
    
    stringstream ss;
    ss << "outputs/" << put_time(localtime(&time_t_now), "%Y-%m-%d-%H-%M-%S") << ".txt";
    string filename = ss.str();
    ofstream outFile{filename};
    if (!outFile.is_open()) {
        cerr << "Error opening file!\n";
        return 1;
    }
    outFile << N << " " << J << " " << T << " " << STEPS << " " << step_interval << "\n";

    for (long long step = 0; step < STEPS; ++step) {
        int i = dis_coord(gen);
        int j = dis_coord(gen);
        int idx = i * N + j;
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
    cout << "Results saved to outputs/" << filename << "\n";

    return 0;
}
