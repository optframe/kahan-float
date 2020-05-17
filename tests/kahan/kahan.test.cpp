#include <iostream>
#include <limits> // numeric_limits

#define CATCH_CONFIG_MAIN // This tells Catch to provide a main()
#include "../thirdparty/catch2/catch.hpp"

// Some instructions for Catch2
// https://github.com/catchorg/Catch2/blob/master/docs/test-cases-and-sections.md

#include <kahan.hpp>    // 'src' included
#include <neumaier.hpp> // 'src' included

using namespace std;
using namespace kahan;


TEST_CASE("Kahan Tests kfloat32 empty == 0.0")
{
   kfloat32 kf32;
   REQUIRE((float)kf32 == 0.0);
}

TEST_CASE("Kahan Tests nfloat32 empty == 0.0")
{
   nfloat32 nf32;
   REQUIRE((float)nf32 == 0.0);
}

TEST_CASE("Kahan Tests kfloat32(1.0) == 1.0")
{
   float f = 1.0;
   kfloat32 kf32(f);
   REQUIRE((float)kf32 == 1.0);
}

TEST_CASE("Kahan Tests kfloat32(1.0 + 1.0) == 2.0")
{
   float f = 1.0;
   kfloat32 kf32(f);
   REQUIRE((float)kf32 == 1.0);
   kf32 += f;
   REQUIRE((float)kf32 == 2.0);
}

TEST_CASE("Kahan Tests 20x float32 0.1 <> 2.0")
{
   float f = 0.0f;
   for (unsigned i = 0; i < 20; i++)
      f += 0.1f;
   REQUIRE(f != 2.0);
}

TEST_CASE("Kahan Tests 20x kfloat32 0.1 == 2.0")
{
   kfloat32 f = 0.0f;
   for (unsigned i = 0; i < 20; i++)
      f += 0.1f;
   REQUIRE((float)f == 2.0);
}

TEST_CASE("Kahan Tests 20x double 0.1 <> 2.0")
{
   double f = 0.0;
   for (unsigned i = 0; i < 20; i++)
      f += 0.1;
   REQUIRE(f != 2.0);
}

TEST_CASE("Kahan Tests 20x kfloat64 0.1 == 2.0")
{
   kfloat64 f = 0.0;
   for (unsigned i = 0; i < 20; i++)
      f += 0.1;
   REQUIRE((double)f == 2.0);
}

TEST_CASE("Kahan Tests infinity kfloat32 kfloat64")
{
   float fone = 1.0;
   float fzero = 0.0;
   double ffone = 1.0;
   double ffzero = 0.0;
   kfloat64 kffone = 1.0;
   kfloat64 kffzero = 0.0;

   // --------

   REQUIRE(std::numeric_limits<float>::has_infinity);
   REQUIRE(std::numeric_limits<double>::has_infinity);
   REQUIRE(std::numeric_limits<kfloat64>::has_infinity);

   REQUIRE(std::numeric_limits<double>::is_iec559);
   REQUIRE(std::numeric_limits<kfloat64>::is_iec559);

   float f1 = std::numeric_limits<float>::infinity();
   REQUIRE(f1 == f1);

   double ff1 = std::numeric_limits<double>::infinity();
   REQUIRE(ff1 == f1);

   kfloat64 kff1 = std::numeric_limits<kahan::kfloat64>::infinity();
   REQUIRE((double)kff1 == f1);

   // ================== INFINITY constant ================

   float f2 = INFINITY;
   double ff2 = INFINITY;
   kfloat64 kff2 = INFINITY;

   REQUIRE(f2 == std::numeric_limits<float>::infinity());
   REQUIRE(f2 == ff2);
   REQUIRE(ff2 == (double)kff2);

   // ================== NAN constant ================
   {
      float fn = NAN;
      double ffn = NAN;
      kfloat64 kffn = NAN;

      REQUIRE(std::isnan(fn));
      REQUIRE(std::isnan(ffn));
      REQUIRE(std::isnan((double)kffn));
   }

   // ================== SNAN constant ================
   {
      float fn = SNANF;
      double ffn = SNAN;
      long double lffn = SNANL;
      kfloat64 kffn = SNAN;

      REQUIRE(std::isnan(fn));
      REQUIRE(std::isnan(ffn));
      REQUIRE(std::isnan(lffn));
      REQUIRE(std::isnan((double)kffn));
   }

   // ================== 1/0 ================
   {
      float fx = fone / (fone - fone + fzero);
      double ffx = ffone / (ffone - ffone + ffzero);
      kfloat64 kffx = (double)kffone / (double)(kffone - kffone + kffzero);

      REQUIRE(fx == std::numeric_limits<float>::infinity());
      REQUIRE(fx == ffx);
      REQUIRE(ffx == (double)kffx);
   }

   // =============== REVERSE =================
   {
      float fm1 = -f1;
      double ffm1 = -ff1;
      kfloat64 kffm1 = -kff1;

      REQUIRE(fm1 == -std::numeric_limits<float>::infinity());
      REQUIRE(ffm1 == -std::numeric_limits<double>::infinity());
      REQUIRE(kffm1 == -std::numeric_limits<kfloat64>::infinity());
   }
   // =============== STANDARD NAN =================
   {
      REQUIRE(std::isnan(0.0 / 0.0));
      REQUIRE(std::isnan(INFINITY - INFINITY));
   }
   // =============== ONE PLUS INF =================
   {
      float fm1 = 1.0 + f1;
      double ffm1 = 1.0 + ff1;
      kfloat64 kffm1 = 1.0 + kff1;

      REQUIRE(fm1 == std::numeric_limits<float>::infinity());
      REQUIRE(ffm1 == std::numeric_limits<double>::infinity());
      REQUIRE(kffm1.getValue() == (double)std::numeric_limits<kfloat64>::infinity());
      REQUIRE(!std::isnan(kffm1.getC()));
      REQUIRE(kffm1 == std::numeric_limits<kfloat64>::infinity());
   }
   // =============== ONE MINUS REVERSE =================
   {
      float fm1 = 1.0 - f1;
      double ffm1 = 1.0 - ff1;
      kfloat64 kffm1 = 1.0 - kff1;

      REQUIRE(fm1 == -std::numeric_limits<float>::infinity());
      REQUIRE(ffm1 == -std::numeric_limits<double>::infinity());
      REQUIRE(kffm1 == -std::numeric_limits<kfloat64>::infinity());
   }
   // ============== ADD ONE TO INFINITY ======= (TODO: section)

   f1 += 1;   // add 1 to infinity
   ff1 += 1;  // add 1 to infinity
   kff1 += 1; // add 1 to infinity

   REQUIRE(f1 == ff1);
   REQUIRE(ff1 == (double)kff1);

   // ============== SUB ONE TO INFINITY ======= (TODO: section)

   f1 -= 1;   // add 1 to infinity
   ff1 -= 1;  // add 1 to infinity
   kff1 -= 1; // add 1 to infinity

   REQUIRE(f1 == std::numeric_limits<float>::infinity());
   REQUIRE(f1 == ff1);
   REQUIRE(ff1 == (double)kff1); // inf == -nan (if nothing is done about it...)

   // ================== ADD INFS ================
   {
      float f3 = f1 + f2;
      double ff3 = ff1 + ff2;
      kfloat64 kff3 = kff1 + kff2;

      REQUIRE(f3 == std::numeric_limits<float>::infinity());
      REQUIRE(f3 == ff3);
      REQUIRE(ff3 == (double)kff3);
   }
   // ================== SUB INFS... ================
   {
      float f3 = f1 - f2;
      double ff3 = ff1 - ff2;
      kfloat64 kff3 = kff1 - kff2;

      REQUIRE(std::isnan(f3));
      REQUIRE(std::isnan(ff3));
      REQUIRE(std::isnan((double)kff3));
   }
}

TEST_CASE("Kahan Tests basic compare kfloat")
{
   kfloat64 f1 = 0.0;
   kfloat64 f2 = 1.0;
   REQUIRE(f1 == f1);
   REQUIRE(f2 == 1.0);
   REQUIRE(f1 <= f1);
   REQUIRE(f1 >= f1);
   REQUIRE(f1 < f2);
   REQUIRE(f1 <= f2);
   REQUIRE(f2 > f1);
   REQUIRE(f2 >= f1);
   REQUIRE(f2 != f1);
}

TEST_CASE("Kahan Tests basic some numeric_limits")
{
   REQUIRE(std::numeric_limits<kfloat32>::lowest() == std::numeric_limits<float>::lowest());
   REQUIRE(std::numeric_limits<kfloat32>::min() == std::numeric_limits<float>::min());
   REQUIRE(std::numeric_limits<kfloat32>::max() == std::numeric_limits<float>::max());

   REQUIRE(std::numeric_limits<kfloat64>::lowest() == std::numeric_limits<double>::lowest());
   REQUIRE(std::numeric_limits<kfloat64>::min() == std::numeric_limits<double>::min());
   REQUIRE(std::numeric_limits<kfloat64>::max() == std::numeric_limits<double>::max());

   REQUIRE(std::numeric_limits<kfloat128>::lowest() == std::numeric_limits<long double>::lowest());
   REQUIRE(std::numeric_limits<kfloat128>::min() == std::numeric_limits<long double>::min());
   REQUIRE(std::numeric_limits<kfloat128>::max() == std::numeric_limits<long double>::max());

   // max 32 < max 64 < max 128
   REQUIRE(std::numeric_limits<kfloat32>::max() < std::numeric_limits<kfloat64>::max());
   REQUIRE(std::numeric_limits<kfloat64>::max() < std::numeric_limits<kfloat128>::max());
}

TEST_CASE("Kahan Tests basic operator<<")
{
   kfloat32 kf{ 1.1 };
   std::stringstream ss;
   ss << kf;
   REQUIRE(ss.str() == "1.1");
}

TEST_CASE("Kahan Tests neumeier basic [1,10^100, 1, -10^100]")
{
   double ffbig = ::pow(10, 100);
   float fbig = ::pow(10, 100);
   //
   REQUIRE(fbig == std::numeric_limits<float>::infinity()); // out of precision
   REQUIRE(ffbig < std::numeric_limits<float>::infinity()); // in precision

   // consider 'double'
   double ffsum = 0;
   ffsum += 1;
   ffsum += ::pow(10, 100);
   ffsum += 1;
   ffsum += -::pow(10, 100);
   REQUIRE(ffsum == 0); // expected 2.0, but error is BAD!

   // consider 'kfloat32'
   kfloat32 kfsum = 0;
   kfsum += 1;
   kfsum += ::pow(10, 100); // breaks precision (inf)
   kfsum += 1;
   kfsum += -::pow(10, 100); // inf - inf = nan
   REQUIRE(std::isnan((float)kfsum));

   // consider 'kfloat64'
   kfloat64 kffsum = 0;
   kffsum += 1;
   kffsum += ::pow(10, 100);
   kffsum += 1;
   kffsum += -::pow(10, 100);
   REQUIRE(kffsum == 0); // expected 2.0, but error is BAD!

   // ==================================================
   // kahan summation could not save us... try neumeier!
   // --------------------------------------------------

   // THANKS Tim Peters, for the following example
   //
   // Raymond Hettinger, Recipe 393090: Binary floating point summation accurate to full precision,
   // Python implementation of algorithm from Shewchuk (1997) article (28 March 2005).

   // consider 'nfloat64'
   nfloat64 nffsum = 0;
   REQUIRE(nffsum == 0); // starts empty
   nffsum += 1;
   nffsum += ::pow(10, 100); // or just '1e100'
   nffsum += 1;
   nffsum += -::pow(10, 100); // or just '-1e100'
   REQUIRE(nffsum == 2);      // expected 2.0, yesss!!!
}

TEST_CASE("Kahan Tests basic compare nfloat")
{
   nfloat64 f1 = 0.0;
   nfloat64 f2 = 1.0;
   REQUIRE(f1 == f1);
   REQUIRE(f2 == 1.0);
   REQUIRE(f1 <= f1);
   REQUIRE(f1 >= f1);
   REQUIRE(f1 < f2);
   REQUIRE(f1 <= f2);
   REQUIRE(f2 > f1);
   REQUIRE(f2 >= f1);
   REQUIRE(f2 != f1);
}

TEST_CASE("Kahan Tests basic operator<< neumaier")
{
   nfloat64 nffsum = 0;
   REQUIRE(nffsum == 0); // starts empty
   nffsum += 1;
   nffsum += ::pow(10, 100); // or just '1e100'
   nffsum += 1;
   nffsum += -::pow(10, 100); // or just '-1e100'
   std::stringstream ss;
   ss << nffsum;
   REQUIRE(ss.str() == "2");
}

TEST_CASE("Kahan Tests inf nan neumaier")
{
   nfloat64 kff1 = std::numeric_limits<double>::infinity();
   kff1 += 1;
   REQUIRE(kff1 == std::numeric_limits<double>::infinity());
   kff1 -= 1;
   REQUIRE(kff1 == std::numeric_limits<double>::infinity());
   nfloat64 kff2 = 1 / 0.0;
   nfloat64 kff3 = kff1 - kff2;

   REQUIRE(std::isnan((double)kff3));
}
