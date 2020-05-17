//#include<iostream>
#include <benchmark/benchmark.h>
#include <vector>

// CppCon 2015: Chandler Carruth "Tuning C++: Benchmarks, and CPUs, and Compilers! Oh My!"
// https://www.youtube.com/watch?v=nXaxk27zwlk
static void escape(void *p)
{
   asm volatile("": : "g"(p) : "memory");
}
//
static void clobber()
{
   asm volatile("": : : "memory");
}
// ===============================
//  library seems to have now: 
//    benchmark::DoNotOptimize(x)
//    benchmark::ClobberMemory()
// -------------------------------
// some info on asm (2017 talk cppcon): 
//    CppCon 2017: Matt Godbolt “What Has My Compiler Done for Me Lately? Unbolting the Compiler's Lid”
//    https://www.youtube.com/watch?v=bSkpMdDe4g4&feature=youtu.be
// rdi, rsi, rdx ... args
// rax ... result
//-----------------------------------------
// http://quick-bench.com/    (see online)
// ----------------------------------------
static void bench_create(benchmark::State &state)
{
   //while(state.KeepRunning())
    for (auto _ : state) 
   {
      std::vector<int> v;
      escape(&v);
      (void)v;
   }
}
BENCHMARK(bench_create);


static void bench_reserve(benchmark::State &state)
{
   //while(state.KeepRunning())
    for (auto _ : state) 
   {
      std::vector<int> v;
      v.reserve(1);
      escape(v.data());
   }
}
BENCHMARK(bench_reserve);

static void bench_push_back(benchmark::State &state)
{
   //while(state.KeepRunning())
   for (auto _ : state) 
   {
      std::vector<int> v;
      v.reserve(1);
      escape(v.data());
      v.push_back(42);
      clobber();
   }
}
BENCHMARK(bench_push_back);

static void bench_norev_push_back(benchmark::State &state)
{
   //while(state.KeepRunning())
   for (auto _ : state) 
   {
      std::vector<int> v;
      v.push_back(42);
      clobber();
   }
}
BENCHMARK(bench_norev_push_back);

