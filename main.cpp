#include <iostream>
#include <tbb/blocked_range.h>
#include <tbb/parallel_for.h>
#include <tbb/parallel_reduce.h>
#include <vector>

using namespace std::chrono;
using namespace std;
const int VECTOR_SIZE = 10000000;

vector<long> v(VECTOR_SIZE);
vector<long> u(VECTOR_SIZE);
vector<long> w(VECTOR_SIZE);



int main() {
    const long VAL_U = 7;
    const long VAL_V = 9;

    auto start = high_resolution_clock::now();

    tbb::parallel_for(
        tbb::blocked_range<size_t>(0, VECTOR_SIZE),
        [&](const tbb::blocked_range<size_t>& range) {
            for (size_t i = range.begin(); i != range.end(); ++i) {
                u[i] = VAL_U;
                v[i] = VAL_V;
            }
        }
    );

    tbb::parallel_for(
        tbb::blocked_range<size_t>(0, VECTOR_SIZE),
        [&](const tbb::blocked_range<size_t>& range) {
            for (size_t i = range.begin(); i != range.end(); ++i) {
                w[i] = u[i] * v[i];
            }
        }
    );

    long total = tbb::parallel_reduce(
        tbb::blocked_range<size_t>(0, VECTOR_SIZE),
        0L,
        [&](const tbb::blocked_range<size_t>& range, long sum) {
            for (size_t i = range.begin(); i != range.end(); ++i) {
                sum += w[i];
            }
            return sum;
        },
        [](long a, long b) { return a + b; }
    );

    auto end = high_resolution_clock::now();
    auto elapsed_ms = duration_cast<milliseconds>(end - start).count();

    cout << "Suma total: " << total << endl;
    cout << "Tiempo: " << elapsed_ms << " ms" << endl;

    return 0;
}
