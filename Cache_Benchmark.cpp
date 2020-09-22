#include <benchmark/benchmark.h>
#include <vector>
#include "Range.hpp"

#ifdef _WIN32
#pragma comment ( lib, "Shlwapi.lib" )
#endif

static void cacheBench(benchmark::State& s)
{
    auto const bytes = 1 << s.range(0);
    auto const counts = bytes / sizeof(int) / 2;

    std::vector<int> v(counts);
    Range(0, std::numeric_limits<int>::max()).fillRand(v);

    std::vector<size_t> index(counts);
    Range({ 0 }, counts).fillRand(index);

    for (auto _ : s)
    {
        uint64_t sum = 0;
        for (auto i : index)
            sum += v[i];
        benchmark::DoNotOptimize(sum);
    }

    s.SetBytesProcessed((s.iterations()) * bytes);
    s.SetLabel(std::to_string(bytes / 1024) + " kb");
}

BENCHMARK(cacheBench)->DenseRange(13, 26);
BENCHMARK_MAIN();