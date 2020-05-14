#pragma once

// kahan.hpp: implementation of Kahan Summation in C++
// Author: Igor Machado Coelho
// Date: 29-04-2020 (first proposed, see official release info in beggining of file)
// License: MIT License

#include <iostream>
#include <limits>  // numeric_limits (will extend this below)
#include <utility> // declval

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
   constexpr tkahan(T _val)
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
      std::cout << "ADDING '" << add << "' TO '" << this->val << "'" << std::endl;
      std::cout << "this->c: '" << this->c << "'" << std::endl;
      if ((this->val == std::numeric_limits<T>::infinity()) || (add == std::numeric_limits<T>::infinity()))
         ;
      float y = add - this->c;
      std::cout << "y: '" << y << "'" << std::endl;
      float t = this->val + y;
      std::cout << "t: '" << y << "'" << std::endl;
      this->c = (t - this->val) - y;
      std::cout << "this->c: '" << this->c << "'" << std::endl;
      this->val = t;
      std::cout << "FINAL VAL: '" << this->val << "'" << std::endl;
      //
      return *this;
   }

   /*
   friend tkahan<T> operator-(tkahan<T> lhs, const X& rhs)
   {

   }
*/

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

// =========================================================

using kfloat32 = tkahan<float>;
using kfloat64 = tkahan<double>;
using kfloat128 = tkahan<long double>;

// =========================================================

// Testing type sizes

struct kahan_helper
{
   static_assert(sizeof(kfloat32) == 8, "Expected 8 bytes on kfloat32");
   static_assert(sizeof(kfloat64) == 16, "Expected 16 bytes on kfloat64");
   static_assert(sizeof(kfloat128) == 32, "Expected 32 bytes on kfloat64");
};

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

// ===============================
// setting up std::numeric_limits
// -------------------------------

template<>
struct std::numeric_limits<kahan::kfloat64>
{
   static constexpr bool has_infinity = std::numeric_limits<double>::has_infinity;
   static constexpr bool has_quiet_NaN = std::numeric_limits<double>::has_quiet_NaN;
   static constexpr bool has_signaling_NaN = has_quiet_NaN;
   static constexpr float_denorm_style has_denorm = bool(__DBL_HAS_DENORM__) ? denorm_present : denorm_absent;
   static constexpr bool has_denorm_loss = std::numeric_limits<double>::has_denorm_loss;

   static constexpr kahan::kfloat64
   infinity() noexcept { return std::numeric_limits<double>::infinity(); };

   static constexpr kahan::kfloat64
   quiet_NaN() noexcept { return std::numeric_limits<double>::quiet_NaN(); };

   static constexpr kahan::kfloat64
   signaling_NaN() noexcept { return std::numeric_limits<double>::signaling_NaN(); };

   static constexpr kahan::kfloat64
   denorm_min() noexcept { return std::numeric_limits<double>::denorm_min(); };

   static constexpr bool is_iec559 = has_infinity && has_quiet_NaN && has_denorm == denorm_present;
};
