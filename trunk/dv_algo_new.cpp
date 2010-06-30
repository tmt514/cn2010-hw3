#include"serv.h"
#include<algorithm>
using namespace std;
/* Using Bellman-Ford Equation */
bool Server::DV_algo() {
  unsigned i,j,d;
  bool send = false;
  for (i = 0; i < num_serv; ++i){
    unsigned d = inf, nd = i;
		if(i == id) continue;
    for (j = 0; j < num_serv; ++j)
			if(dv[j].size() == num_serv && d > cost[j] + dv[j][i]){
				d = cost[j] + dv[j][i];
	      nd = j;
			}
    send |= (d != dis[i]);
		dis[i] = d;
		next[i] = nd;
  }
  Display();
  PrintDVs();
  if (send){
	  for(i = 0; i < num_serv; ++i){
		  if(dv[i].size() == num_serv){ //connected with i
			  vector<unsigned> t;
			  for(j=0;j<num_serv;j++){
				  t.push_back(dv[i][j]);
				  if(next[j] == i)
					  dv[i][j] = inf;
			  }
			  Send(i);
			  for(j=0;j<num_serv;j++)
				  dv[i][j] = t[j];
		  }
	  }
  }
  return send; 
}
