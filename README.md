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
   REQUIRE(kffsum == 0); // expected 2.0, but error is BAD!

   // consider 'nfloat64'
   nfloat64 nffsum = 0;
   nffsum += 1;
   nffsum += ::pow(10, 100); 
   nffsum += 1;
   nffsum += -::pow(10, 100); 
   REQUIRE(nffsum == 2);      // expected 2.0, yesss!!!
```

## Install and test

Just copy `src/kahan.hpp` to your project (or also `src/neumaier.hpp` if you prefer that).

To test it here:

- `make` (basic testing)
- `make test` (official tests on [catch2](https://github.com/catchorg/Catch2))
- `make test-coverage` (see `reports/index.html`)

## Learn more

To learn more on IEEE754:

- https://en.wikipedia.org/wiki/IEEE_754
- https://www.h-schmidt.net/FloatConverter/IEEE754.html


## License

MIT License - 2020
