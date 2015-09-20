#ifndef PRECISE_TIME_H_
#define PRECISE_TIME_H_

#include <sys/timeb.h>

class PreciseTime : public timeb {
public:
  PreciseTime(time_t t = 0, unsigned short m = 0)  {
    time = t;
    millitm = m;
  }

  PreciseTime(timeb &arg)  {
    time = arg.time;
    millitm = arg.millitm;
  }

  PreciseTime operator+(const PreciseTime &arg) {
    struct timeb res;
    
    res.time = this->time + arg.time;
    res.millitm = this->millitm + arg.millitm;
    res.time += res.millitm / 1000;
    res.millitm %= 1000;

    return res;
  }

  PreciseTime operator-(const PreciseTime &arg)
  {
    struct timeb res;
    
    res.time = this->time - arg.time;
    short n = this->millitm - arg.millitm;

    if (n < 0) {
      n += 1000;
      res.time--;
    }
    res.millitm = n;

    return res;
  }
		
  bool operator>(const PreciseTime &arg) {
    if (this->time < arg.time)
      return false;
    else if (this->time > arg.time)		  
      return true;
    else
      return this->millitm > arg.millitm;
  }

  bool operator==(const PreciseTime &arg) {
    return this->time == arg.time && this->millitm == arg.millitm;
  }
};

#endif
