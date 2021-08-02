#include <benchmark/benchmark.h>
#include <composer/melody.h>
#include <vector>
#include <algorithm>

void create_melody(benchmark::State &state) {
    for (auto _ : state) {
        std::vector<int> v(state.range(0));
        //composer::melody m;
        //m.notes(v);
        benchmark::DoNotOptimize(v);
    }
}

BENCHMARK(create_melody)->Range(2,2000);

BENCHMARK_MAIN();
