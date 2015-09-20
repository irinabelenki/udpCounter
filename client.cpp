#include <netinet/ip.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <iostream>
using namespace std;

int main(int argc, char **argv)
{
  int fd;

  char buf[16];

  fd = socket(AF_INET, SOCK_DGRAM, 0);

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(2000);
  inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

  for (int i = 0; i < 16; i++) {
    uint32_t c = htonl(i);
    memcpy(buf, &c, 4);
    int rc = sendto(fd, buf, sizeof(buf), 0, (const struct sockaddr *)&addr, sizeof(addr));
    if (rc < 0) {
      cerr << "sendto: " << strerror(errno) << endl;
      return -1;
    }
    usleep(125000);
  }
}

  
  
