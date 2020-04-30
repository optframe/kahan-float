#include <iostream>

#include "kahan.hpp"

using namespace std;
//using namespace kahan;

using kahan::kfloat32;
using kahan::kfloat64;

// ==========================================
//           very basic testing
//
// Kahan Summation for
//    IEEE 754 floating point representaion
// ==========================================

int
main()
{
   kfloat32 kf(0.1f);
   cout << kf << endl;

   kf += 0.2f;
   cout << kf << endl;

   kf -= 0.1;
   cout << kf << endl;

   float vf_1 = 1.0;
   kahan::print_IEEE754(vf_1);
   float vf_2 = -2.0;
   kahan::print_IEEE754(vf_2);
   float vf_875 = 0.875;
   kahan::print_IEEE754(vf_875);

   // ======= 30 tests ========

   float x = 0;
   int k = 30;
   while (k--)
      x += 0.1f;
   printf("x=%f\n", x); // 2.999999

   kfloat32 y = 0;
   k = 30;
   while (k--)
      y += 0.1f;
   printf("y=%f\n", (float)y); // 3.000000

   // ======= 1000 tests ========

   double x2 = 0;
   k = 1000;
   while (k--)
      x2 += 0.1f;
   printf("x2=%f\n", x2); // 100.000001

   kfloat32 y2 = 0;
   k = 1000;
   while (k--)
      y2 += 0.1f;
   printf("y2=%f\n", (float)y2); // 100.000000

   kfloat64 y3 = 0;
   k = 1000;
   while (k--)
      y3 += 0.1f;
   printf("y3=%f\n", (double)y3); // 100.000000

   // ======= 1000000 tests ========

   kfloat32 z = 0;
   k = 1000000;
   while (k--)
      z += 0.1f;
   printf("z=%f\n", (float)z); // 100000.000000

   return 0;
}
