#include "vec.hpp"
#include <benchmark/benchmark.h>
#include <cstring>

constexpr int SIZE = 1024;

struct Movable {
    std::unique_ptr<int[]> data;
    Movable() : data(std::make_unique<int[]>(SIZE)) {}
    Movable(const Movable& other) : data(std::make_unique<int[]>(SIZE)) {
        std::copy(other.data.get(), other.data.get() + SIZE, data.get());
    }
    Movable(Movable&& other) noexcept = default;
};

static void BM_VecPushBack_Movable(benchmark::State& state) {
    for (auto _ : state) {
        Vec<Movable> v;
        v.Reserve(state.range(0));
        for (int i = 0; i < state.range(0); ++i) {
            v.PushBack({});
        }
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(BM_VecPushBack_Movable)->Arg(1000)->Arg(10000)->Arg(100000)->Arg(1000000);

BENCHMARK_MAIN();
