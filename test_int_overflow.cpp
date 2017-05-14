#include <sstream>
//#include <iostream>
#include <climits>

#include "benchmark/benchmark.h"

#include "int_overflow.h"

constexpr int ITERS = 1000;

extern "C" int subtract_overflow; //TODO header with destrctive read functions readr_add_overflows() read-reset
extern "C" int int_add_overflow;
extern "C" int long_add_overflow;

/*

static void check_add_overflow(benchmark::State& state) {
   int n_overflowed=0;
  while (state.KeepRunning()) {
     for (auto j = 0; j != ITERS; ++j) {
    auto const x = state.range(0);
    auto const y = state.range(0);
    benchmark::DoNotOptimize(n_overflowed += is_add_overflow( x, y, 0));
     }
  }
  std::stringstream ss;
  ss << n_overflowed;
  state.SetLabel(ss.str());
}
BENCHMARK(check_add_overflow)->RangeMultiplier(2)->Range(128 * 1024 * 1024L, INT_MAX);
*/

static void add(benchmark::State& state) {
   long long last_sum;
  while (state.KeepRunning()) {
     for (auto j = 0; j != ITERS; ++j)
     {
       auto const x = state.range(0);
       auto const y = state.range(0)+j;
       benchmark::DoNotOptimize(last_sum = (x + y));
     }
  }
  std::stringstream ss;
  ss << "last_sum=" << last_sum;;
  if(int_add_overflow) ss << " int_add_overflow=" << int_add_overflow;
  int_add_overflow = 0;
  if(long_add_overflow) ss << " long_add_overflow=" << long_add_overflow;
  long_add_overflow = 0L;
  state.SetLabel(ss.str());
}
BENCHMARK(add)->RangeMultiplier(2)->Range(128 * 1024 * 1024L, INT_MAX);

template <class FLOAT>
static void add_float(benchmark::State& state) {
   FLOAT last_sum;
  while (state.KeepRunning()) {
     for (auto j = 0; j != ITERS; ++j)
     {
       FLOAT x = state.range(0); //TODO Conversion may afftect timings - use predefined array of float nums
       FLOAT y = state.range(0)+j;
       benchmark::DoNotOptimize(last_sum = (x + y));
     }
  }
  std::stringstream ss;
  ss << "last_sum=" << last_sum;;/*
  if(int_add_overflow) ss << " int_add_overflow=" << int_add_overflow;
  int_add_overflow = 0;
  if(long_add_overflow) ss << " long_add_overflow=" << long_add_overflow;
  long_add_overflow = 0L;*/
  state.SetLabel(ss.str());
}
static void add_float_long_double(benchmark::State& state) { add_float<long double>(state); }
static void add_float_double(benchmark::State& state) { add_float<double>(state); }
BENCHMARK(add_float_long_double)->RangeMultiplier(2)->Range(128 * 1024 * 1024L, INT_MAX);
BENCHMARK(add_float_double)->RangeMultiplier(2)->Range(128 * 1024 * 1024L, INT_MAX);

static void x_mul_y_plus_x_plus_y_div_x_minus_2_mul_y(benchmark::State& state) {
   long long last_res;
  while (state.KeepRunning()) {
     //state.PauseTiming();
     for (auto j = 0; j != ITERS; ++j) {
    double const x = state.range(0);
    double const y = state.range(0)+j;
    //state.ResumeTiming();
    benchmark::DoNotOptimize(last_res = (x*y + 3*x + y) / (x + 5*y) );
     }
  }
  std::stringstream ss;
  ss << "last_res=" << last_res;
  if(int_add_overflow) ss << " int_add_overflow=" << int_add_overflow;
  int_add_overflow = 0;
  if(long_add_overflow) ss << " long_add_overflow=" << long_add_overflow;
  long_add_overflow = 0L;
  state.SetLabel(ss.str());
}
BENCHMARK(x_mul_y_plus_x_plus_y_div_x_minus_2_mul_y)->RangeMultiplier(2)->Range(128 * 1024 * 1024L, INT_MAX);

/*
static void add_with_overflow_check(benchmark::State& state) {
   long long res;
   int n_overflowed=0;
  while (state.KeepRunning()) {
     for (auto j = 0; j != ITERS; ++j) {
    //is_add_overflow( state.range(0), state.range(0), 0);
    auto const x = state.range(0);
    auto const y = state.range(0);
    benchmark::DoNotOptimize(res = (n_overflowed +=is_add_overflow(x,y,0), x+y));
     }
  }
  std::stringstream ss;
  ss << n_overflowed << " " << res;
  state.SetLabel(ss.str());
}
BENCHMARK(add_with_overflow_check)->RangeMultiplier(2)->Range(128 * 1024 * 1024L, INT_MAX);
*/
BENCHMARK_MAIN()
