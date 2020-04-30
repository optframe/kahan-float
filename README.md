# kahan-float
## Kahan Floating-Point (in C++)

This project provides simple implementation of a float type with Kahan Summation strategy.


## How to use?

Just `#include "kahan.hpp"`, a single header library.
There are two basic types:

- `kahan::kfloat32`, equivalent to `float`
- `kahan::kfloat64`, equivalent to `double`

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

## Install and test

Just copy `src/kahan.hpp` to your project.

To test it here:

- `make` (basic testing)
- `make test` (official tests on [catch2](https://github.com/catchorg/Catch2))


## License

MIT License - 2020