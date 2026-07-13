## 이징 모델

단순한 이징 모델 시뮬레이션

> 2026/07/13 물리학 발표

### Usage

You'll need Python (used 3.12.11 during development) and C++ compiler (used g++ 16.1.1 during development)

```bash
g++ -std=c++17 -O3 main.cpp -o ising
./ising <N> <J> <T> <steps> <step_interval>

python visual.py # use -h for more options
```
