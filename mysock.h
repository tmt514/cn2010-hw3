#ifndef _MYSOCK_H
#define _MYSOCK_H
#include<netinet/in.h>
class MySock {
public:
  MySock();
  bool Bind(unsigned port);
  bool SetDest(const char ip[],unsigned port);
  int Send(unsigned len, const void *buf);
  int Recv(unsigned len, void *buf);
  bool operator==(const MySock& s)const;
  bool Ready(int t);
  sockaddr_in from;
private:
  int sd;
  sockaddr_in dest;
};

inline long long Wait(long long t);
inline long long Timer(int type);
#endif
