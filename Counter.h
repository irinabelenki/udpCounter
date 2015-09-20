#ifndef UDP_COUNTER_H_
#define UDP_COUNTER_H_

#include <netinet/ip.h> 
#include <sys/timeb.h>
#include <pthread.h>
#include "PreciseTime.h"

class UdpCounter {
public:
  UdpCounter(int fd);
  UdpCounter(uint32_t addr, uint16_t port);
  ~UdpCounter();

  void Run(void);

  void OnPacketReceived(uint32_t packetSerialNumber);

  int GetCurrentLoss(void);
  int GetAverageLoss(void);
private:
  int fd;
  
  bool firstPacket;
  uint32_t counter, startCounter;

  uint64_t totalLost;

  pthread_rwlock_t rwLock;

  struct {
    uint64_t counter, loss;
    PreciseTime time;
  } lossHistory[16];
  int historyHead, historyTail;

  void setupLossStats();
  void updateLossStats(int32_t loss);

  class ReadLock {
  public:
    ReadLock(pthread_rwlock_t *_lock): lock(_lock) { pthread_rwlock_rdlock(lock); }
    ~ReadLock() { pthread_rwlock_unlock(lock); }
  protected:
    pthread_rwlock_t *lock;
  };

  class WriteLock {
  public:
    WriteLock(pthread_rwlock_t *_lock): lock(_lock) { pthread_rwlock_wrlock(lock); }
    ~WriteLock() { pthread_rwlock_unlock(lock); }
  protected:
    pthread_rwlock_t *lock;
  };
};


#endif
