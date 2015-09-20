#include <iostream>
#include "Counter.h"

using namespace std;

int main(int argc, char **argv)
{
  cout << "Hello, world!" << endl;

  UdpCounter counter(INADDR_ANY, 2000);
  counter.Run();

  return 0;
}
  
