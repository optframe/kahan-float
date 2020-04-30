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


## License

MIT License