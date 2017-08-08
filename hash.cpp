#include <sstream>

#include "benchmark/benchmark.h"

//#include <iostream>
//#include <climits>
//#include "int_overflow.h"

int NumBuckets;
int hash1(int value) { return value % NumBuckets; }
static void BM_hash1(benchmark::State& state) {
   NumBuckets = 257;
   int h = 0;
   while (state.KeepRunning()) h = hash1(state.range(0));
   std::stringstream ss;
   ss << h;
   state.SetLabel(ss.str());
}
BENCHMARK_RANGE(BM_hash1, 1, 1024 * 1024);

// Hash with constant number of buckets.
const auto ConstNumBuckets = 257;
int hash2(int value) { return value % ConstNumBuckets; }
static void BM_hash2(benchmark::State& state) {
   NumBuckets = 257;
   int h = 0;
   while (state.KeepRunning()) h = hash2(state.range(0));
   std::stringstream ss;
   ss << h;
   state.SetLabel(ss.str());
}
// BENCHMARK(BM_hash2)->RangeMultiplier(4)->Range( 1, 1024 * 1024);
BENCHMARK_RANGE(BM_hash2, 1, 1024 * 1024);

// Hash with constantunsigned number of buckets. One
// of the places where 'auto' might have led to a
// problem as it's easy to forget the trailing 'u'.
const auto uConstNumBuckets = 257u;
int hash3(int value) { return value % uConstNumBuckets; }
static void BM_hash3(benchmark::State& state) {
   NumBuckets = 257;
   int h = 0;
   while (state.KeepRunning()) h = hash3(state.range(0));
   std::stringstream ss;
   ss << h;
   state.SetLabel(ss.str());
}
// BENCHMARK(BM_hash3)->RangeMultiplier(2)->Range( 1, 1024 * 1024);
BENCHMARK_RANGE(BM_hash3, 1, 1024 * 1024);

BENCHMARK_MAIN()
