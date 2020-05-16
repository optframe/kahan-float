#include<iostream>

#include <benchmark/benchmark.h>
#include <vector>

#include <kahan.hpp> // from 'src'

using namespace kahan;

/*
int main()
{
   
   double ff = 0.0;
   int MAXI = 1000000000; // 1B
   for(int i=0; i<MAXI; i++)
   {
        if(i%7 == 0)
          	ff += i;
   }
   std::cout << ff << std::endl;

   return 0;
}
*/