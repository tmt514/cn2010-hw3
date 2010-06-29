#ifndef _SERV_H
#define _SERV_H
#include<vector>
#include"log.h"
#include"mysock.h"
using namespace std;
class Server {
public:
  bool Load(const char file[]);
  bool Init(unsigned i, unsigned port);
  bool Send();
  void Update(unsigned id, unsigned cost);
  bool Refresh();
  bool Refresh(unsigned id, const vector<unsigned>& dv);
  virtual bool DV_algo();
  void Display();
  void PrintDVs();
  bool SetPort(unsigned port);
  void Wait();
  vector<unsigned>& getDis();
private:
  unsigned id, num_serv, refresh_num;
  MySock serv_sock;
  LogWriter w;
  vector<MySock> serv;
  vector<vector<unsigned> > dv;
  vector<unsigned> dis, cost, next;
};
#endif
