#include"serv.h"
#include<algorithm>
using namespace std;
/* Using Bellman-Ford Equation */
bool Server::DV_algo() {
  unsigned i, j, d;
  bool send = false, flag = false;
  vector<unsigned> pre = dis;
  for (i = 0; i < num_serv; ++i){
    unsigned d = inf, nd = i;
		if(i == id) continue;
    for (j = 0; j < num_serv; ++j)
			if(dv[j].size() == num_serv && cost[j] + dv[j][i] < d){
				d = cost[j] + dv[j][i];
	      nd = j;
			}
    if (cost[i] < d)
      d = cost[i], nd = i;
    send |= (d != dis[i]);
    flag |= mark[i];
		dis[i] = d;
		next[i] = nd;
  }
  Display();
  PrintDVs();
  if (!send && !flag) return send;
  for (i = 0; i < num_serv; ++i) {
	  if(cost[i] >= inf) continue;
    if (!send && flag && !mark[i]) continue;
    vector<unsigned> t = dis;    
    bool up = false;
    for (j = 0; j < num_serv; ++j)
      if (dis[j] > pre[j]) { // && next[j] == i)
        up = true;
        dis[j] = cost[j];
      }
    Send(i);
    mark[i] = up;
    dis = t;
	}
  return send; 
}
