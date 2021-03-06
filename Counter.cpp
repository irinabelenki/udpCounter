#include "Counter.h"
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <iostream>

UdpCounter::UdpCounter(int fd_) : fd(fd_) {
  pthread_rwlock_init(&rwLock, nullptr);
  setupLossStats();
}

UdpCounter::UdpCounter(uint32_t s_addr, uint16_t  port) {
  pthread_rwlock_init(&rwLock, nullptr);

  sockaddr_in addr;

  addr.sin_family = AF_INET;  // Supporting only IPv4
  addr.sin_port = htons(port); 
  addr.sin_addr.s_addr = s_addr;
  
  fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (fd < 0)
    throw errno;
  
  if (bind(fd, (const struct sockaddr *)&addr, sizeof(addr)) < 0)
    throw errno;

  setupLossStats();
}

UdpCounter::~UdpCounter() 
{
  pthread_rwlock_destroy(&rwLock);
}

void UdpCounter::Run(void) {
  char buf[1024]; // larger buffer should not be placed on stack, but 1K is fine
  int rc;
  uint32_t incomingCounter;

  for (;;) {
    rc = read(fd, buf, sizeof(buf));
    if (rc < 0) 
      throw errno;

    if (rc < 4)
      throw EBADMSG;

    memcpy(&incomingCounter, buf, 4);
    incomingCounter = ntohl(incomingCounter);

    OnPacketReceived(incomingCounter);
  }
}

void UdpCounter::OnPacketReceived(uint32_t incomingCounter)
{
    if (firstPacket) {
      startCounter = counter = incomingCounter;
      firstPacket = false;
    } else {
      if (incomingCounter != (counter + 1 ))  {
	updateLossStats(incomingCounter - counter);
      }
      counter = incomingCounter;
    }
}

void UdpCounter::setupLossStats()
{
  historyHead = historyTail = 0;
  totalLost = 0;
}

#define HISTORY_SIZE (sizeof(lossHistory)/sizeof(lossHistory[0]))

void UdpCounter::updateLossStats(int32_t loss)
{
  WriteLock wLock(&rwLock);

  totalLost += loss;
  
  lossHistory[historyHead].counter = counter;
  lossHistory[historyHead].loss = loss;
  ftime(&lossHistory[historyHead].time);

  historyHead += (historyHead + 1) % HISTORY_SIZE;
  if (historyHead == historyTail)
    historyTail += (historyTail + 1) % HISTORY_SIZE;
}

  
int UdpCounter::GetCurrentLoss(void) 
{ 
  ReadLock rLock(&rwLock);

  if (historyTail == historyHead)
    return 0;

  PreciseTime currentTime;
  ftime(&currentTime);

  uint32_t historyStartCounter = lossHistory[historyTail].counter;
  uint32_t historyLossSum = 0;

  for (int i = historyTail; i != historyHead; i = (i + 1) % HISTORY_SIZE) {
    if ((currentTime - lossHistory[i].time) > PreciseTime(2, 0))
      continue;

    historyLossSum += lossHistory[i].loss;
  }

  return ((counter - historyStartCounter) * 100) / historyLossSum;
}

int UdpCounter::GetAverageLoss(void) 
{
  ReadLock rLock(&rwLock);

  return ((counter - startCounter) * 100) / totalLost;
}
  


