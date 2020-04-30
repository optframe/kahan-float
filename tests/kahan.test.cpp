#include <iostream>

#define CATCH_CONFIG_MAIN // This tells Catch to provide a main()
#include "../libs/catch2/catch.hpp"

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