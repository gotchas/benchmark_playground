// g++ -Wall -Wextra -I/opt/wandbox/boost-1.64.0/gcc-7.1.0/include -std=c++14

// /proc/sys/kernel/kptr_restrict
// perf report -g 'graph,0.5,caller'
// https://www.youtube.com/watch?v=nXaxk27zwlk

// about escape and clobber: https://youtu.be/nXaxk27zwlk?t=2825

// bjam toolset=gcc-5
// https://wandbox.org/permlink/a6DcJEJ7t0Yc7gk4

#include <benchmark/benchmark.h>

// This file is a "Hello, world!" in C++ language by GCC for wandbox.
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <string>
#include <numeric>
#include <functional>


#define UNLIKELY(x)  __builtin_expect((bool)(x), 0)
// if( UNLIKELY(input[i]>ceil) )

static void escape(void* p) {
   asm volatile("" : : "g"(p) : "memory");
}
static void clobber() { // writes to all memory
   asm volatile("" : : : "memory");
}

#define D(X) // D(X
struct Vector : public std::vector<int> {
  Vector(std::initializer_list<int> il) : std::vector<int>(il){
    D("Vector(std::initializer_list)\n");
  }
  Vector() {
    D("Vector()\n");
  }
  Vector(const Vector &v) : std::vector<int>(v) {
    D("Vector(const Vector &)\n");
  }
  Vector & operator = (const Vector &v) {
    if (this != &v) {
      std::vector<int>::operator=(v);
      D("Vector& Vector::operator=(const Vector &)\n");
    }
    return *this;
  }
  Vector & operator = (Vector && v) {
    if (this != &v) {
      std::vector<int>::operator=(std::move(v));
      D("Vector& Vector::operator=(Vector&&)\n");
    }
    return *this;
  }
  Vector(Vector&& v) : std::vector<int>(std::move(v)) {
     D("Vector(Vector&&)\n");
  }
};

Vector copy_accumulate(Vector &v) {
    return std::accumulate(v.begin(), v.end(), Vector(),
                    [](Vector & v, int i) {
                      v.push_back(i);
                      return v;
                    });
    //D("size of v2 = " << v2.size() << "\n");
}

Vector nocopy_accumulate(Vector &v) {
    Vector init;
    return std::accumulate(v.begin(), v.end(), std::ref(init),
                    [](std::reference_wrapper<Vector> v, int i) {
                      v.get().push_back(i);
                      return v;
                    });
    //D("size of v2 = " << v2.size() << "\n");
}

#if 0
int main()
{
    Vector v { 1, 2, 3, 4 };
    copy_accumulate(v);
    D("=============\n");
    nocopy_accumulate(v);
}
#else
BENCHMARK_MAIN()

static void BM_ca(benchmark::State& state) {
  Vector v { 1, 2, 3, 4 };
  while (state.KeepRunning()) {
    auto res = copy_accumulate(v);
    //escape(&res);
    escape(res.data());
    //clobber();
  }
}
//BENCHMARK_RANGE(BM_ca, 1, 1024 * 1024);
BENCHMARK(BM_ca);

static void BM_noca(benchmark::State& state) {
  Vector v { 1, 2, 3, 4 };
  while ( UNLIKELY(state.KeepRunning()) ) {//UNLIKELY(x) only to test
    auto res = nocopy_accumulate(v);
    //escape(&res);
    escape(res.data());
    //clobber();
  }
}
//BENCHMARK_RANGE(BM_noca, 1, 1024 * 1024);
BENCHMARK(BM_noca);
#endif

