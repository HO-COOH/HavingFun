#include <benchmark/benchmark.h>
#include <immintrin.h>
#include <ctime>
#include <vector>
#include <algorithm>
#include <functional>
#include <execution>
#include <iostream>
#include <string>
#include <fstream>
#include <valarray>
#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl2.hpp>
#include "Range.hpp"

#ifdef _WIN32
#pragma comment ( lib, "Shlwapi.lib" )
#endif

#ifdef _DEBUG
#define End 8<<22
#else
#define End 8<<24
#endif

static void BM_Forloop(benchmark::State& st)
{
    for(auto _ :st)
    {
        uint64_t sum{};
        for (auto i = 0; i < st.range(0); ++i)
        {
            (sum += i);
        }
    }
}


static void BM_Range(benchmark::State& st)
{
    for (auto _ : st)
    {
        uint64_t sum{};
        for (auto i : ::Range{ {0}, st.range(0) })
            (sum += i);
    }
}


class AddFixture:public benchmark::Fixture
{
public:
    std::vector<float> v1;
    std::vector<float> v2;
    std::vector<float> result;
    void SetUp(const benchmark::State& state)
    {
        srand(time(nullptr));
        v1.resize(state.range(0));
        v2.resize(state.range(0));
        std::generate(v1.begin(), v1.end(), rand);
        std::generate(v2.begin(), v2.end(), rand);
        result.resize(state.range(0));
    }
};

BENCHMARK_DEFINE_F(AddFixture, ScalarSingleThread)(benchmark::State& st)
{
    for(auto _:st)
    {
        std::transform(std::execution::seq, v1.cbegin(), v1.cend(), v2.cbegin(), result.begin(), std::plus<>{});
    }
}


BENCHMARK_DEFINE_F(AddFixture, ScalarParallelExecution)(benchmark::State& st)
{
    for(auto _ : st)
    {
        std::transform(std::execution::par_unseq, v1.cbegin(), v1.cend(), v2.cbegin(), result.begin(), std::plus<>{});
    }
}


BENCHMARK_DEFINE_F(AddFixture, ScalarMultithread)(benchmark::State& st)
{
    const static auto threadLimit = std::thread::hardware_concurrency() - 1;
    for (auto _ : st)
    {
        parallel(::Range{ {0}, v1.size() }, [this](auto range)
        {
            for (auto index : range)
                result[index] = v1[index] + v2[index];
        });
    }
}


static void BM_ValarraySingleThread(benchmark::State& st)
{
    std::valarray<float> v1(st.range(0));
    std::valarray<float> v2(st.range(0));
    std::generate(std::begin(v1), std::end(v1), rand);
    std::generate(std::begin(v2), std::end(v2), rand);
    for(auto _:st)
    {
        benchmark::DoNotOptimize(v1 + v2);
    }
}


//static void BM_ValarrayMultiThread(benchmark::State& st)
//{
//    std::valarray<float> v1(st.range(0));
//    std::valarray<float> v2(st.range(0));
//    std::generate(std::begin(v1), std::end(v1), rand);
//    std::generate(std::begin(v2), std::end(v2), rand);
//
//}

BENCHMARK_DEFINE_F(AddFixture, SIMDSingleThread)(benchmark::State& st)
{
    std::vector<__m256> result_simd(st.range(0)/8);
    for(auto _:st)
    {
        auto iter = result_simd.begin();
        for(auto pt1=v1.data(), pt2=v2.data(); pt1!=v1.data()+v1.size(); pt1+=8, pt2+=8)
        {
            *iter = _mm256_add_ps(_mm256_loadu_ps(pt1), _mm256_loadu_ps(pt2));
            ++iter;
        }
    }
}

static void BM_SIMDNative(benchmark::State& st)
{
    std::vector<__m256> v1(st.range(0) / 8);
    std::vector<__m256> v2(st.range(0) / 8);
    std::vector<__m256> result(st.range(0) / 8);
    for(auto _ :st)
    {
        auto iter_r = result.begin();
        for(auto iter1 = v1.cbegin(), iter2 = v2.cbegin(); iter1!=v1.cend(); ++iter1, ++iter2, ++iter_r)
            *iter_r = _mm256_add_ps(*iter1, *iter2);
    }
}


static auto getSource(std::string const& fileName)
{
    std::ifstream f{ fileName };
    if (!f.is_open())
    {
        std::cerr << "Cannot open file: " << fileName << '\n';
        throw std::runtime_error{ "Cannot open file" };
    }
    return cl::Program::Sources{ std::string{ std::istreambuf_iterator<char>{f}, std::istreambuf_iterator<char>{} } };
}
static auto getKernel(cl::Device const& device, cl::Context const& context, std::string const& source, bool optimize = true)
{
    cl::Program program{ context, getSource(source) };
    if (optimize)
    {
        try {
            program.build("-O3 -cl-fast-relaxed-math");
        }
        catch (cl::Error& err) {
            std::cerr << "Build program with -O3 error, trying without\n" << err.what() << '\n';
        }
        try {
            program.build();
        }
        catch (cl::Error& err) {
            std::cerr << "Build program error\n" << err.what() << '\n';
            const auto buildLog = program.getBuildInfo<CL_PROGRAM_BUILD_LOG>();
            for (const auto& log : buildLog)
                std::cerr << log.first.getInfo<CL_DEVICE_NAME>() << ":\t" << log.second << '\n';
        }
    }
    else
    {
        try {
            program.build();
        }
        catch (cl::Error& err) {
            std::cerr << "Build program error\n" << err.what() << '\n';
            const auto buildLog = program.getBuildInfo<CL_PROGRAM_BUILD_LOG>();
            for (const auto& log : buildLog)
                std::cerr << log.first.getInfo<CL_DEVICE_NAME>() << ":\t" << log.second << '\n';
        }
    }
    std::vector<cl::Kernel> kernels;
    program.createKernels(&kernels);
    std::cerr << "Kernel creation success!\n";
    return kernels;
}


BENCHMARK_DEFINE_F(AddFixture, GPUNaive)(benchmark::State& st)
{
    static std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    static std::vector<cl::Device> devices;
    for (auto& platform : platforms)
    {
        std::vector<cl::Device> platformDevice;
        platform.getDevices(CL_DEVICE_TYPE_ALL, &platformDevice);
        std::copy(platformDevice.cbegin(), platformDevice.cend(), std::back_inserter(devices));
    }
    auto& device = devices[0];
    static cl::Context context{ device };
    static auto kernels = getKernel(device, context, "kernel.cl");
    auto& kernel = kernels[0];

    static cl::CommandQueue queue{ context, device };
    for(auto _:st)
    {
        cl::Buffer l_buf{ context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, v1.size() * sizeof(float), v1.data() };
        cl::Buffer r_buf{ context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, v1.size() * sizeof(float), v2.data() };
        cl::Buffer result_buf{ context, CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR, v1.size() * sizeof(float), result.data() };
        kernel.setArg(0, l_buf);
        kernel.setArg(1, r_buf);
        kernel.setArg(2, result_buf);

        queue.enqueueNDRangeKernel(kernel, { 0 }, { v1.size() });
        queue.finish();

    }
}


BENCHMARK_DEFINE_F(AddFixture, GPUWorkGroup)(benchmark::State& st)
{
    static std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    static std::vector<cl::Device> devices;
    for (auto& platform : platforms)
    {
        std::vector<cl::Device> platformDevice;
        platform.getDevices(CL_DEVICE_TYPE_ALL, &platformDevice);
        std::copy(platformDevice.cbegin(), platformDevice.cend(), std::back_inserter(devices));
    }
    auto& device = devices[0];
    static cl::Context context{ device };
    static auto kernels = getKernel(device, context, "kernel.cl");
    auto& kernel = kernels[0];

    static cl::CommandQueue queue{ context, device };
    for (auto _ : st)
    {
        cl::Buffer l_buf{ context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, v1.size() * sizeof(float), v1.data() };
        cl::Buffer r_buf{ context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, v1.size() * sizeof(float), v2.data() };
        cl::Buffer result_buf{ context, CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR, v1.size() * sizeof(float), result.data() };
        kernel.setArg(0, l_buf);
        kernel.setArg(1, r_buf);
        kernel.setArg(2, result_buf);

        queue.enqueueNDRangeKernel(kernel, { 0 }, { v1.size() }, { 64 });
        queue.finish();

    }
}

//BENCHMARK(BM_Forloop)->Range(2ull << 10, 2ull << 24);
//BENCHMARK(BM_Range)->Range(2ull << 10, 2ull << 24);

BENCHMARK_REGISTER_F(AddFixture, ScalarSingleThread)->Range(8, End);
//BENCHMARK_REGISTER_F(AddFixture, ScalarParallelExecution)->Range(8, End);
//BENCHMARK_REGISTER_F(AddFixture, ScalarMultithread)->Range(8, End);
BENCHMARK_REGISTER_F(AddFixture, SIMDSingleThread)->Range(8, End);
BENCHMARK(BM_SIMDNative)->Range(8, End);

//BENCHMARK(BM_ValarraySingleThread)->Range(8, End);
//BENCHMARK_REGISTER_F(AddFixture, GPUNaive)->Range(8, End);
//BENCHMARK_REGISTER_F(AddFixture, GPUWorkGroup)->Range(8, End);

BENCHMARK_MAIN();
