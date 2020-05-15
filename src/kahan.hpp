#pragma once

// kahan.hpp: implementation of Kahan Summation in C++
// Author: Igor Machado Coelho
// Date: 29-04-2020 (first proposed, see official release info in beggining of file)
// License: MIT License

#include <cmath> // isnan
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
   // this constructor allows promotion of kahan types
   template<class T2>
   constexpr tkahan(tkahan<T2> kother)
     : val(kother.getValue())
   {
   }

   // build with T value (not 'explicit', may be automatic!)
   constexpr tkahan(T _val)
     : val(_val)
   {
   }

   // build with T value and T error
   constexpr tkahan(T _val, T _c)
     : val(_val)
     , c(_c)
   {
   }

   T getValue() const
   {
      return this->val;
   }

   T getC() const
   {
      return this->c;
   }

   explicit operator T() const
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
      T y = add - this->c;
      T t = this->val + y;
      this->c = (t - this->val) - y;
      this->val = t;
      // we must ensure that 'c' is never 'contaminated' by 'nan'
      // TODO: verify that this is REALLY safe... looks like.
      this->c = std::isnan(this->c) ? 0.0 : this->c;
      //
      return *this;
   }

   // reverse (unary minus)
   tkahan<T> operator-() const
   {
      return tkahan<T>(-this->val, -this->c);
   }

   // ------------------

   // copy assignment (for any valid element)
   template<class X>
   tkahan<T>& operator-=(const X& add)
   {
      (*this) += -add; // reuse '+='
      return *this;
   }

   // ------------------

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

   // ==================

   bool operator==(const tkahan<T>& other) const
   {
      // strict check if both parts are the same (value and error 'c')
      // if you want weaker tests, cast to 'double' or 'float' before
      return (this->val == other.val) && (this->c == other.c);
   }

   bool operator!=(const tkahan<T>& other) const
   {
      return !((*this) == other);
   }

   bool operator<(const tkahan<T>& other) const
   {
      // note that this ignores error 'this->c'
      return this->val < other.val;
   }

   bool operator>(const tkahan<T>& other) const
   {
      // note that this ignores error 'this->c'
      return this->val > other.val;
   }

   bool operator<=(const tkahan<T>& other) const
   {
      return ((*this) < other) || ((*this) == other);
   }

   bool operator>=(const tkahan<T>& other) const
   {
      return ((*this) > other) || ((*this) == other);
   }

   // ==================

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
struct std::numeric_limits<kahan::kfloat32>
{
   static constexpr bool is_specialized = true;

   static constexpr kahan::kfloat32
   min() noexcept { return std::numeric_limits<float>::min(); }

   static constexpr kahan::kfloat32
   max() noexcept { return std::numeric_limits<float>::max(); }

   static constexpr kahan::kfloat32
   lowest() noexcept { return std::numeric_limits<float>::lowest(); }

   static constexpr int digits = std::numeric_limits<float>::digits;
   static constexpr int digits10 = std::numeric_limits<float>::digits10;

   static constexpr int max_digits10 = std::numeric_limits<float>::max_digits10;
   static constexpr bool is_signed = std::numeric_limits<float>::is_signed;
   static constexpr bool is_integer = std::numeric_limits<float>::is_integer;
   static constexpr bool is_exact = std::numeric_limits<float>::is_exact;
   static constexpr int radix = std::numeric_limits<float>::radix;

   static constexpr kahan::kfloat32
   epsilon() noexcept { return std::numeric_limits<float>::epsilon(); }

   static constexpr kahan::kfloat32
   round_error() noexcept { return std::numeric_limits<float>::round_error(); }

   static constexpr int min_exponent = std::numeric_limits<float>::min_exponent;
   static constexpr int min_exponent10 = std::numeric_limits<float>::min_exponent10;
   static constexpr int max_exponent = std::numeric_limits<float>::max_exponent;
   static constexpr int max_exponent10 = std::numeric_limits<float>::max_exponent10;

   static constexpr bool has_infinity = std::numeric_limits<float>::has_infinity;
   static constexpr bool has_quiet_NaN = std::numeric_limits<float>::has_quiet_NaN;
   static constexpr bool has_signaling_NaN = std::numeric_limits<float>::has_signaling_NaN;
   static constexpr float_denorm_style has_denorm = std::numeric_limits<float>::has_denorm;
   static constexpr bool has_denorm_loss = std::numeric_limits<float>::has_denorm_loss;

   static constexpr kahan::kfloat32
   infinity() noexcept { return std::numeric_limits<float>::infinity(); };

   static constexpr kahan::kfloat32
   quiet_NaN() noexcept { return std::numeric_limits<float>::quiet_NaN(); };

   static constexpr kahan::kfloat32
   signaling_NaN() noexcept { return std::numeric_limits<float>::signaling_NaN(); };

   static constexpr kahan::kfloat32
   denorm_min() noexcept { return std::numeric_limits<float>::denorm_min(); };

   static constexpr bool is_iec559 = has_infinity && has_quiet_NaN && has_denorm == denorm_present;

   static constexpr bool is_bounded = std::numeric_limits<float>::is_bounded;
   static constexpr bool is_modulo = std::numeric_limits<float>::is_modulo;

   static constexpr bool traps = std::numeric_limits<float>::traps;
   static constexpr bool tinyness_before = std::numeric_limits<float>::tinyness_before;
   static constexpr float_round_style round_style = std::numeric_limits<float>::round_style;
};

template<>
struct std::numeric_limits<kahan::kfloat64>
{
   static constexpr bool is_specialized = true;

   static constexpr kahan::kfloat64
   min() noexcept { return std::numeric_limits<double>::min(); }

   static constexpr kahan::kfloat64
   max() noexcept { return std::numeric_limits<double>::max(); }

   static constexpr kahan::kfloat64
   lowest() noexcept { return std::numeric_limits<double>::lowest(); }

   static constexpr int digits = std::numeric_limits<double>::digits;
   static constexpr int digits10 = std::numeric_limits<double>::digits10;

   static constexpr int max_digits10 = std::numeric_limits<double>::max_digits10;
   static constexpr bool is_signed = std::numeric_limits<double>::is_signed;
   static constexpr bool is_integer = std::numeric_limits<double>::is_integer;
   static constexpr bool is_exact = std::numeric_limits<double>::is_exact;
   static constexpr int radix = std::numeric_limits<double>::radix;

   static constexpr kahan::kfloat64
   epsilon() noexcept { return std::numeric_limits<double>::epsilon(); }

   static constexpr kahan::kfloat64
   round_error() noexcept { return std::numeric_limits<double>::round_error(); }

   static constexpr int min_exponent = std::numeric_limits<double>::min_exponent;
   static constexpr int min_exponent10 = std::numeric_limits<double>::min_exponent10;
   static constexpr int max_exponent = std::numeric_limits<double>::max_exponent;
   static constexpr int max_exponent10 = std::numeric_limits<double>::max_exponent10;

   static constexpr bool has_infinity = std::numeric_limits<double>::has_infinity;
   static constexpr bool has_quiet_NaN = std::numeric_limits<double>::has_quiet_NaN;
   static constexpr bool has_signaling_NaN = has_quiet_NaN;
   static constexpr float_denorm_style has_denorm = std::numeric_limits<double>::has_denorm;
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

   static constexpr bool is_bounded = std::numeric_limits<double>::is_bounded;
   static constexpr bool is_modulo = std::numeric_limits<double>::is_modulo;

   static constexpr bool traps = std::numeric_limits<double>::traps;
   static constexpr bool tinyness_before = std::numeric_limits<double>::tinyness_before;
   static constexpr float_round_style round_style = std::numeric_limits<double>::round_style;
};

template<>
struct std::numeric_limits<kahan::kfloat128>
{
   static constexpr bool is_specialized = true;

   static constexpr kahan::kfloat128
   min() noexcept { return std::numeric_limits<long double>::min(); }

   static constexpr kahan::kfloat128
   max() noexcept { return std::numeric_limits<long double>::max(); }

   static constexpr kahan::kfloat128
   lowest() noexcept { return std::numeric_limits<long double>::lowest(); }

   static constexpr int digits = std::numeric_limits<long double>::digits;
   static constexpr int digits10 = std::numeric_limits<long double>::digits10;

   static constexpr int max_digits10 = std::numeric_limits<long double>::max_digits10;
   static constexpr bool is_signed = std::numeric_limits<long double>::is_signed;
   static constexpr bool is_integer = std::numeric_limits<long double>::is_integer;
   static constexpr bool is_exact = std::numeric_limits<long double>::is_exact;
   static constexpr int radix = std::numeric_limits<long double>::radix;

   static constexpr kahan::kfloat128
   epsilon() noexcept { return std::numeric_limits<long double>::epsilon(); }

   static constexpr kahan::kfloat128
   round_error() noexcept { return std::numeric_limits<long double>::round_error(); }

   static constexpr int min_exponent = std::numeric_limits<long double>::min_exponent;
   static constexpr int min_exponent10 = std::numeric_limits<long double>::min_exponent10;
   static constexpr int max_exponent = std::numeric_limits<long double>::max_exponent;
   static constexpr int max_exponent10 = std::numeric_limits<long double>::max_exponent10;

   static constexpr bool has_infinity = std::numeric_limits<long double>::has_infinity;
   static constexpr bool has_quiet_NaN = std::numeric_limits<long double>::has_quiet_NaN;
   static constexpr bool has_signaling_NaN = has_quiet_NaN;
   static constexpr float_denorm_style has_denorm = bool(__DBL_HAS_DENORM__) ? denorm_present : denorm_absent;
   static constexpr bool has_denorm_loss = std::numeric_limits<long double>::has_denorm_loss;

   static constexpr kahan::kfloat128
   infinity() noexcept { return std::numeric_limits<long double>::infinity(); };

   static constexpr kahan::kfloat128
   quiet_NaN() noexcept { return std::numeric_limits<long double>::quiet_NaN(); };

   static constexpr kahan::kfloat128
   signaling_NaN() noexcept { return std::numeric_limits<long double>::signaling_NaN(); };

   static constexpr kahan::kfloat128
   denorm_min() noexcept { return std::numeric_limits<long double>::denorm_min(); };

   static constexpr bool is_iec559 = has_infinity && has_quiet_NaN && has_denorm == denorm_present;

   static constexpr bool is_bounded = std::numeric_limits<long double>::is_bounded;
   static constexpr bool is_modulo = std::numeric_limits<long double>::is_modulo;

   static constexpr bool traps = std::numeric_limits<long double>::traps;
   static constexpr bool tinyness_before = std::numeric_limits<long double>::tinyness_before;
   static constexpr float_round_style round_style = std::numeric_limits<long double>::round_style;
};
