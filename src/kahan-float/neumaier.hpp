#pragma once

// neumaier.hpp: alternative to kahan.hpp

#include <cmath> // isnan
#include <iostream>
#include <limits>  // numeric_limits (will extend this below)
#include <utility> // declval

namespace kahan {

template<class T>
struct tneumaier
{
private:
   // "real" value
   T val{ 0 };
   // pending correction
   T c{ 0 };

public:
   // this constructor allows promotion of kahan types
   template<class T2>
   constexpr tneumaier(tneumaier<T2> kother)
     : val(kother.getValue())
   {
   }

   // build with T value (not 'explicit', may be automatic!)
   constexpr tneumaier(T _val)
     : val(_val)
   {
   }

   // build with T value and T error
   constexpr tneumaier(T _val, T _c)
     : val(_val)
     , c(_c)
   {
   }

   // empty
   constexpr tneumaier()
   {
   }

   T getValue() const
   {
      //this->val += this->c;
      //this->c = 0;
      return this->val + this->c;
   }

private:
   // TODO: do we need this?
   T getC() const
   {
      return this->c;
   }

public:
   // IMPORTANT: this CHANGES value! lazy operation.
   explicit operator T() const
   {
      return this->val + this->c;
   }

   // copy assignment (for any valid element)
   template<class X>
   tneumaier<T>& operator+=(const X& _add)
   {
      T add = _add; // converting to correct type
      //
      // naive solution
      // this->val += add;  // will accumulate errors easily
      //
      // neumaier operation
      // THANKS: https://en.wikipedia.org/wiki/Kahan_summation_algorithm
      // # Kahan and Babuska summation, Neumaier variant
      // # A. Neumaier.
      // # Rundungsfehleranalyse einiger Verfahren zur Summation endlicher Summen.
      // # Math. Mechanik, 54:39–51, 1974.
      //
      T t = this->val + add;
      if (::fabs(this->val) >= ::fabs(add)) {
         this->c += ((this->val - t) + add); // If sum is bigger, low-order digits of input[i] are lost.
      } else {
         this->c += ((add - t) + this->val); // Else low-order digits of sum are lost.
      }
      this->val = t;

      // we must ensure that 'c' is never 'contaminated' by 'nan'
      // TODO: verify that this is REALLY safe... looks like.
      this->c = std::isnan(this->c) ? 0.0 : this->c; // TODO:
      //
      return *this;
   }

   // reverse (unary minus)
   tneumaier<T> operator-() const
   {
      return tneumaier<T>(-this->val, -this->c);
   }

   // ------------------

   // copy assignment (for any valid element)
   template<class X>
   tneumaier<T>& operator-=(const X& add)
   {
      (*this) += -add; // reuse '+='
      return *this;
   }

   // ------------------

   // copy return (for any valid element)
   template<class X>
   friend tneumaier<T> operator+(tneumaier<T> lhs, const X& rhs)
   {
      lhs += rhs; // reuse '+='
      return lhs;
   }

   // copy return (for any valid element)
   template<class X>
   friend tneumaier<T> operator-(tneumaier<T> lhs, const X& rhs)
   {
      lhs += -rhs; // reuse '+='
      return lhs;
   }

   // ==================

   bool operator==(const tneumaier<T>& other) const
   {
      // since do not cache updated values, we should compare sums here
      return (this->val + this->c == other.val + other.c);
   }

   bool operator!=(const tneumaier<T>& other) const
   {
      return !((*this) == other);
   }

   bool operator<(const tneumaier<T>& other) const
   {
      // time to use accumulator 'c'
      return (this->val + this->c) < (other.val + other.c);
   }

   bool operator>(const tneumaier<T>& other) const
   {
      // time to use accumulator 'c'
      return (this->val + this->c) > (other.val + other.c);
   }

   bool operator<=(const tneumaier<T>& other) const
   {
      return (this->val + this->c) <= (other.val + other.c);
   }

   bool operator>=(const tneumaier<T>& other) const
   {
      return (this->val + this->c) >= (other.val + other.c);
   }

   // ==================

   friend std::ostream& operator<<(std::ostream& os, const tneumaier<T>& k)
   {
      os << (k.val + k.c);
      return os;
   }
};

// =========================================================

using nfloat32 = tneumaier<float>;
using nfloat64 = tneumaier<double>;
using nfloat128 = tneumaier<long double>;

// =========================================================

// Testing type sizes

struct neumaier_helper
{
   static_assert(sizeof(nfloat32) == 8, "Expected 8 bytes on nfloat32");
   static_assert(sizeof(nfloat64) == 16, "Expected 16 bytes on nfloat64");
   static_assert(sizeof(nfloat128) == 32, "Expected 32 bytes on nfloat64");
};

} // namespace kahan
