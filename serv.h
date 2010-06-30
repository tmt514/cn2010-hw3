#ifndef _SERV_H
#define _SERV_H
#include"log.h"
#include"vector.h"
#include"mysock.h"
#include<netinet/in.h>
const unsigned inf = 257;
class Server {
public:
  void Wait();
  bool Send();
  bool Send(int server_id);
  bool Refresh();
  void Display();
  void PrintDVs();
  virtual bool DV_algo();
  bool SetPort(unsigned port);
  bool Load(const char file[]);
  bool Init(unsigned i, unsigned port);
  void Update(unsigned id, unsigned cost);
  bool Refresh(unsigned id);
private:
  LogWriter w;
  MySock serv_sock;
  vector<sockaddr_in> serv;
  vector<vector<unsigned> > dv;
  vector<unsigned> dis, cost, next;
  unsigned id, num_serv, refresh_num;
};
#endif
