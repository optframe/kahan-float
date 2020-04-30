#include <iostream>

#include "kahan.hpp"

using namespace std;
using namespace kahan;

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
   print_IEEE754(vf_1);
   float vf_2 = -2.0;
   print_IEEE754(vf_2);
   float vf_875 = 0.875;
   print_IEEE754(vf_875);

   return 0;
}
