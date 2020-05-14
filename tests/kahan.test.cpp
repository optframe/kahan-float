#include <iostream>
#include <limits> // numeric_limits

#define CATCH_CONFIG_MAIN // This tells Catch to provide a main()
#include "thirdparty/catch2/catch.hpp"

// Some instructions for Catch2
// https://github.com/catchorg/Catch2/blob/master/docs/test-cases-and-sections.md

#include "../src/kahan.hpp"

using namespace std;
using namespace kahan;

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
   REQUIRE(std::numeric_limits<float>::has_infinity);
   REQUIRE(std::numeric_limits<double>::has_infinity);
   REQUIRE(std::numeric_limits<kfloat64>::has_infinity);

   REQUIRE(std::numeric_limits<double>::is_iec559);
   REQUIRE(std::numeric_limits<kfloat64>::is_iec559);

   float f1 = std::numeric_limits<float>::infinity();
   std::cout << f1 << std::endl;
   REQUIRE(f1 == f1);

   double ff1 = std::numeric_limits<double>::infinity();
   std::cout << ff1 << std::endl;
   REQUIRE(ff1 == ff1);

   kfloat64 kff1 = std::numeric_limits<kahan::kfloat64>::infinity();
   std::cout << kff1 << std::endl;
   REQUIRE(kff1 == ff1);

   // float inf === double inf
   REQUIRE(f1 == ff1);

   // =============== REVERSE =================
   {
      float fm1 = -f1;
      double ffm1 = -ff1;
      kfloat64 kffm1 = -kff1;

      REQUIRE(fm1 == -std::numeric_limits<float>::infinity());
      REQUIRE(ffm1 == -std::numeric_limits<double>::infinity());
      REQUIRE(kffm1 == -std::numeric_limits<kfloat64>::infinity());
   }
   // =============== ONE MINUS REVERSE =================
   {
      float fm1 = 1 - f1;
      double ffm1 = 1 - ff1;
      kfloat64 kffm1 = 1 - kff1;

      REQUIRE(fm1 == -std::numeric_limits<float>::infinity());
      REQUIRE(ffm1 == -std::numeric_limits<double>::infinity());
      REQUIRE(kffm1 == -std::numeric_limits<kfloat64>::infinity());
   }
   // ============== ADD ONE TO INFINITY ======= (TODO: section)

   f1 += 1;   // add 1 to infinity
   ff1 += 1;  // add 1 to infinity
   kff1 += 1; // add 1 to infinity

   REQUIRE(f1 == ff1);
   REQUIRE(ff1 == kff1);

   std::cout << INFINITY << std::endl;
   std::cout << f1 << std::endl;
   std::cout << ff1 << std::endl;
   std::cout << kff1 << std::endl;

   // ============== SUB ONE TO INFINITY ======= (TODO: section)

   f1 -= 1;   // add 1 to infinity
   ff1 -= 1;  // add 1 to infinity
   kff1 -= 1; // add 1 to infinity

   REQUIRE(f1 == std::numeric_limits<float>::infinity());
   REQUIRE(f1 == ff1);
   REQUIRE(ff1 == kff1); // inf == -nan (if nothing is done about it...)

   // ================== INFINITY constant ================

   float f2 = INFINITY;
   double ff2 = INFINITY;
   kfloat64 kff2 = INFINITY;

   REQUIRE(f2 == std::numeric_limits<float>::infinity());
   REQUIRE(f2 == ff2);
   REQUIRE(ff2 == kff2);

   // ================== ADD INFS ================

   float f3 = f1 + f2;
   double ff3 = ff1 + ff2;
   kfloat64 kff3 = kff1 + kff2;

   REQUIRE(f3 == std::numeric_limits<float>::infinity());
   REQUIRE(f3 == ff3);
   REQUIRE(ff3 == kff3);

   REQUIRE(false);
   /*
   kfloat64 f = 0.0;
   for (unsigned i = 0; i < 20; i++)
      f += 0.1;
   REQUIRE((double)f == 2.0);
*/
}
