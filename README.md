# kahan-float
## Kahan Floating-Point (in C++)

This project provides simple implementation of a float type with [Kahan Summation](https://en.wikipedia.org/wiki/Kahan_summation_algorithm) strategy.


## How to use?

Just `#include "kahan.hpp"`, a single header library. Just get it from [src/kahan.hpp](src/kahan.hpp).
There are three basic types:

- `kahan::kfloat32`, equivalent to `float`
- `kahan::kfloat64`, equivalent to `double`
- `kahan::kfloat128`, equivalent to `long double`

One example is to add `30` times the value `0.1`, result should be `3.0` (but it's only correct on `kfloat32`):

```cpp
#include "kahan.hpp"
using kahan::kfloat32;

int main() {
    // add 30 times 0.1 (should be 3.0)
    float x = 0;
    int k = 30;
    while (k--)
        x += 0.1f;
    printf("x=%f\n", x);        // outputs 2.999999

    kfloat32 y = 0;
    k = 30;
    while (k--)
        y += 0.1f;
    printf("y=%f\n", (float)y); // outputs 3.000000
    // ...
```

## Space and Time

Type `kfloat32` occuppies 64 bits in fact (two internal variables), and more processing on every operation. It should be more stable than `double`, trying to avoid error propagation. Precision should still be bigger on `double`, as long as one [properly use floating-point](https://en.wikipedia.org/wiki/Loss_of_significance).

One can also do this for `kfloat64` (which occupies 128 bits) and should be definitely more stable than `double`.

Finally, one can use `kfloat128` (that occupies 32 bytes = 256 bits) and should be more stable than `long double`.

Benchmarking is not properly done yet, but it is definitely expected to consume more time. Beware of aggressive code optimizations, that may perhaps remove kahan strategy completely (such as in `icc` compiler).
For this reason, *always test* your code, to ensure operations are performing as expected.

## Comparison with Neumaier

Sometimes even kahan summation may fail, and it may be useful to `#include "neumaier.hpp"`.

Neumaier strategy includes three floating points (namespace is still `kahan::`):

- `kahan::nfloat32` (similar to `kahan::kfloat32`)
- `kahan::nfloat64` (similar to `kahan::kfloat64`)
- `kahan::nfloat128` (similar to `kahan::kfloat128`)

See example from Tim Peters (sum `1 + 10^100 + 1 - 10^100`):

```cpp
   // consider 'kfloat64'
   kfloat64 kffsum = 0;
   kffsum += 1;
   kffsum += ::pow(10, 100);
   kffsum += 1;
   kffsum += -::pow(10, 100);
   assert(kffsum == 0); // expected 2.0, but error is BAD!

   // consider 'nfloat64'
   nfloat64 nffsum = 0;
   nffsum += 1;
   nffsum += ::pow(10, 100); 
   nffsum += 1;
   nffsum += -::pow(10, 100); 
   assert(nffsum == 2);      // expected 2.0, yesss!!!
```

## Install and test

Just copy `src/kahan.hpp` to your project (or also `src/neumaier.hpp` if you prefer that).

To test it here:

- `make` (basic testing)
- `make test` (official tests on [catch2](https://github.com/catchorg/Catch2))
- `make test-coverage` (see `reports/index.html`)

## Benchmarks

On `tests` folder you can find [benchmark](https://github.com/google/benchmark) tools. To install and run:
- `make deps`
- `make bench`
- `make perf` (will require `sudo` and `perf` installed)
- `make perf-report`

A basic sample indicates that `kfloat` and `nfloat` are 3 to 4 times (around `~3.5x`) slower than raw versions.
Experiments varied from a range of attributions, including `1`, `16` and `64` values.

```
2020-05-17 13:57:00
Running ./build/kahan_bench
Run on (4 X 3100 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 3072 KiB (x1)
Load Average: 0.88, 0.71, 0.71
------------------------------------------------------------------------
Benchmark                              Time             CPU   Iterations
------------------------------------------------------------------------
t_plus_assign<float>/1/0            1.77 ns         1.77 ns    401515125
t_plus_assign<float>/16/0           23.4 ns         23.3 ns     29884745
t_plus_assign<float>/64/0            155 ns          155 ns      4287115
t_plus_assign<double>/1/0           1.76 ns         1.75 ns    368454731
t_plus_assign<double>/16/0          20.3 ns         20.3 ns     34340593
t_plus_assign<double>/64/0           129 ns          129 ns      5369049
t_plus_assign<kfloat32>/1/0         2.88 ns         2.88 ns    242359192
t_plus_assign<kfloat32>/16/0         106 ns          106 ns      6638273
t_plus_assign<kfloat32>/64/0         547 ns          545 ns      1244372
t_plus_assign<kfloat64>/1/0         2.77 ns         2.77 ns    264395636
t_plus_assign<kfloat64>/16/0        95.8 ns         95.8 ns      7120360
t_plus_assign<kfloat64>/64/0         465 ns          465 ns      1473925
t_plus_assign<nfloat32>/1/0         3.01 ns         3.01 ns    230304808
t_plus_assign<nfloat32>/16/0         105 ns          105 ns      6659415
t_plus_assign<nfloat32>/64/0         483 ns          483 ns      1447780
t_plus_assign<nfloat64>/1/0         2.98 ns         2.98 ns    234288433
t_plus_assign<nfloat64>/16/0         101 ns          101 ns      6912502
t_plus_assign<nfloat64>/64/0         474 ns          474 ns      1477161
```

Regarding `kfloat128`, there's some weird results... for longer runs, it is incredibly slow (`50x`) compared to `long double` version:
```
t_plus_assign<long double>/1/0        9.08 ns         9.08 ns     75946379
t_plus_assign<long double>/16/0        152 ns          152 ns      4579032
t_plus_assign<long double>/64/0        619 ns          619 ns      1115649
t_plus_assign<kfloat128>/1/0          6.92 ns         6.92 ns     97725530
t_plus_assign<kfloat128>/16/0         6947 ns         6948 ns        99394
t_plus_assign<kfloat128>/64/0        29144 ns        29142 ns        23936
t_plus_assign<nfloat128>/1/0          7.46 ns         7.46 ns     91103858
t_plus_assign<nfloat128>/16/0         8931 ns         8931 ns        77510
t_plus_assign<nfloat128>/64/0        37538 ns        37538 ns        18629
```

Don't know why it behaves like this, because applying clobbering only at a final stage, looks to "solve the issue" (one must deeply verify how benchmarking interferes on real code via `perf`):

So, final overhead looks like around `4x` on practice (and `nfloat` looks faster than `kfloat` afterall):
```
t_plus_assign_final_clobber<double>/1/0             1.32 ns         1.32 ns    465124556
t_plus_assign_final_clobber<double>/16/0            9.73 ns         9.73 ns     70145529
t_plus_assign_final_clobber<double>/64/0            54.3 ns         54.3 ns     12427363
t_plus_assign_final_clobber<kfloat64>/1/0           2.31 ns         2.31 ns    299287996
t_plus_assign_final_clobber<kfloat64>/16/0          75.2 ns         75.2 ns      9148860
t_plus_assign_final_clobber<kfloat64>/64/0           423 ns          423 ns      1621581
t_plus_assign_final_clobber<nfloat64>/1/0           2.77 ns         2.77 ns    254712235
t_plus_assign_final_clobber<nfloat64>/16/0          38.4 ns         38.4 ns     18207250
t_plus_assign_final_clobber<nfloat64>/64/0           190 ns          190 ns      3674940
t_plus_assign_final_clobber<long double>/1/0        2.36 ns         2.36 ns    300934858
t_plus_assign_final_clobber<long double>/16/0       11.5 ns         11.5 ns     59995851
t_plus_assign_final_clobber<long double>/64/0       45.3 ns         45.3 ns     15438976
t_plus_assign_final_clobber<kfloat128>/1/0          5.26 ns         5.26 ns    128193936
t_plus_assign_final_clobber<kfloat128>/16/0         43.9 ns         43.9 ns     15936900
t_plus_assign_final_clobber<kfloat128>/64/0          239 ns          239 ns      2925523
t_plus_assign_final_clobber<nfloat128>/1/0          6.08 ns         6.08 ns    109617390
t_plus_assign_final_clobber<nfloat128>/16/0         54.0 ns         54.0 ns     12548218
t_plus_assign_final_clobber<nfloat128>/64/0          212 ns          212 ns      3294921
```

Please benchmark your own code when using this library.

## Learn more

To learn more on IEEE754:

- https://en.wikipedia.org/wiki/IEEE_754
- https://www.h-schmidt.net/FloatConverter/IEEE754.html


## License

MIT License - 2020
