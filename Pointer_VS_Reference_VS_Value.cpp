#include <vector>
#include <ctime>
#include <benchmark/benchmark.h>
#include <execution>
#pragma comment(lib, "Shlwapi.lib")

static auto genData(size_t count)
{
    std::vector<float> a(count);
    std::generate(a.begin(), a.end(), [] { return static_cast<float>(rand()) / RAND_MAX; });
    return a;
}

static void addPointer(float *a, float *b, float *result)
{
    *result = *a + *b;
}

static void addPointerRestrict(float const *__restrict a, float const *__restrict b, float *result)
{
    *result = *a + *b;
}

static void addReference(float const &a, float const &b, float &result)
{
    result = a + b;
}

static float addValue(float const a, float const b)
{
    return a + b;
}

static void BM_RawPointer(benchmark::State &state)
{
    auto a = genData(state.range(0));
    auto b = genData(state.range(0));
    std::vector<float> c(state.range(0));
    for (auto _ : state)
    {
        auto a_iter = a.data();
        auto b_iter = b.data();
        auto c_iter = c.data();
        for (size_t i = 0; i < state.range(0); ++i)
            addPointer(a_iter++, b_iter++, c_iter++);
    }
}

static void BM_RestrictPointer(benchmark::State &state)
{
    auto a = genData(state.range(0));
    auto b = genData(state.range(0));
    std::vector<float> c(state.range(0));
    for (auto _ : state)
    {
        float *__restrict a_iter = a.data();
        float *__restrict b_iter = b.data();
        float *__restrict c_iter = c.data();
        for (size_t i = 0; i < state.range(0); ++i)
            addPointerRestrict(a_iter++, b_iter++, c_iter++);
    }
}

static void BM_Reference(benchmark::State &state)
{
    auto a = genData(state.range(0));
    auto b = genData(state.range(0));
    std::vector<float> c(state.range(0));
    for (auto _ : state)
    {
        const float *a_iter = a.data();
        const float *b_iter = b.data();
        auto c_iter = c.data();
        for (size_t i = 0; i < state.range(0); ++i)
            addReference(*a_iter++, *b_iter++, *c_iter++);
    }
}

static void BM_Value(benchmark::State &state)
{
    auto a = genData(state.range(0));
    auto b = genData(state.range(0));
    std::vector<float> c(state.range(0));
    for (auto _ : state)
    {
        auto a_iter = a.data();
        auto b_iter = b.data();
        auto c_iter = c.data();
        for (size_t i = 0; i < state.range(0); ++i)
            *c_iter++ = addValue(*a_iter++, *b_iter++);
    }
}

static void BM_SIMD(benchmark::State &state)
{
    auto a = genData(state.range(0));
    auto b = genData(state.range(0));
    std::vector<float> c(state.range(0));
    for (auto _ : state)
    {
        std::transform(a.cbegin(), a.cend(), b.cbegin(), c.begin(), std::plus<>{});
    }
}

constexpr size_t start = 8 << 10;
constexpr size_t limit = 8 << 18;

BENCHMARK(BM_RawPointer)->Range(start, limit);
BENCHMARK(BM_RestrictPointer)->Range(start, limit);
BENCHMARK(BM_Reference)->Range(start, limit);
BENCHMARK(BM_Value)->Range(start, limit);
BENCHMARK(BM_SIMD)->Range(start, limit);
BENCHMARK_MAIN();