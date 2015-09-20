#include <iostream>
#include "PreciseTime.h"
#include <assert.h>

using namespace std;

int main(int, char **)
{
  PreciseTime t1(2,0);
  PreciseTime t2(3,500);

  PreciseTime t3 = t2 - t1;

  assert(t3.time == 1 && t3.millitm == 500);
  assert(t3 == PreciseTime(1, 500));

  assert(t2 > t1);

  t3 = PreciseTime(2, 200) - PreciseTime(1, 800);
  assert(t3 == PreciseTime(0, 400));

  t3 = PreciseTime(3, 700) + PreciseTime(1, 800);
  assert(t3 == PreciseTime(5, 500));

  cout << "Test passed" << endl;
}

  

    
