#include"serv.h"
bool Server::DV_algo() {
  unsigned i,j,d;
  bool send = false;
  for (i = 0; i < num_serv; ++i)
    unsigned d = inf;
    for (j = 0; j < num_serv; ++j)
      d = min(d, cost[i] + dv[i][j]);
    send |= (d != dis[i]);
  if (send) Send();
}
