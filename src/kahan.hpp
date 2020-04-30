#pragma once

// kahan.hpp: implementation of Kahan Summation in C++
// Author: Igor Machado Coelho
// Date: 29-04-2020
// License: MIT License

#include <iostream>

namespace kahan {

// T must be typically double or float (TODO: use concepts here)
template<class T>
struct tkahan
{
private:
   // "real" value
   T val{ 0 };
   // pending correction
   T c{ 0 };

public:
   tkahan(T _val)
     : val(_val)
   {
   }

   operator T() const
   {
      return val;
   }

   // copy assignment (for any valid element)
   template<class X>
   tkahan<T>& operator+=(const X& add)
   {
      // naive solution
      // this->val += add;  // will accumulate errors easily
      //
      // kahan operation
      float y = add - this->c;
      float t = this->val + y;
      this->c = (t - this->val) - y;
      this->val = t;
      //
      return *this;
   }

   // copy assignment (for any valid element)
   template<class X>
   tkahan<T>& operator-=(const X& add)
   {
      (*this) += -add; // reuse '+='
      return *this;
   }

   // copy return (for any valid element)
   template<class X>
   friend tkahan<T> operator+(tkahan<T> lhs, const X& rhs)
   {
      lhs += rhs; // reuse '+='
      return lhs;
   }

   // copy return (for any valid element)
   template<class X>
   friend tkahan<T> operator-(tkahan<T> lhs, const X& rhs)
   {
      lhs += -rhs; // reuse '+='
      return lhs;
   }

   friend std::ostream& operator<<(std::ostream& os, const tkahan<T>& k)
   {
      os << k.val;
      return os;
   }
};

using kfloat32 = tkahan<float>;
using kfloat64 = tkahan<double>;

// UTILS

// print variable bits (MSB to LSB) -> Big Endian bitstring format
void
kprint_bin(int val, int sz)
{
   for (int k = sz - 1; k >= 0; k--) {
      if ((val >> k) & 1)
         printf("1");
      else
         printf("0");
   }
}

// printing details of float IEEE 754
typedef union
{
   float f; // 32-bits
   struct   // same 32-bits here
   {
      // components here follow LSB to MSB (little-endian rep)

      // MSB  --------------->  LSB
      // SIGN | EXPONENT | MANTISSA
      // READS                  <-
      // READS          <-
      // READS  <-
      unsigned int mantissa : 23;
      unsigned int exponent : 8;
      unsigned int sign : 1;
   } data_f;
} union_float32;

void
print_IEEE754(float f32)
{
   union_float32 var;
   var.f = f32;
   //
   // un_f32.data_f contains data in parts
   //
   // FORMAT: (-1)^SIGN * 2^EXP * MANTISSA
   // MANTISSA \in [1.0, 2.0]
   // EXP part is (EXPONENT - 127)
   // ---

   printf("%d | ", var.data_f.sign);
   printf("(2^%d) ", var.data_f.exponent - 127);
   kprint_bin(var.data_f.exponent, 8);
   printf(" | ");
   kprint_bin(var.data_f.mantissa, 23);
   printf("\n");
}

} // namespace kahan