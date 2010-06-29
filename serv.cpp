#include"serv.h"
const unsigned kBufSize = 768;
bool Server::Load(const char file[]){  
  FILE *fp = fopen(file, "r");
  if (fp == NULL)
    return false;
  unsigned num_edges;
  if (fscanf(fp, "%u %u", &num_serv, &num_edges) != 2)
    return false;
  serv.resize(num_serv);
  dis.resize(num_serv);
  fill(dis.begin(), dis.end(), inf);
  cost.resize(num_serv);
  fill(cost.begin(), cost.end(), inf);
  next.resize(num_serv);
  dv.resize(num_serv);
  unsigned i;
  for (i = 0; i < num_serv; ++i)
    next[i] = i;
    dv[i].clear();
  for (i = 1; i < num_serv; ++i) {
    unsigned id, port;
    char ip[kBufSize];
    if (fscanf(fp, "%u %s %u", &id, &ip, &port)!=  3)
      return false;
    serv[id].SetDest(ip, port);
  }
  while (num_edges--) {
    unsigned id, c;
    if (fscanf(fp, "%u %u", &id, &c) != 2)
      return false;
    dis[id] = cost[id] = c;
  }
  fclose(fp);
  puts("Load topology file successfully");
  Display();
  return true;
}
bool Server::Init(unsigned i, unsigned port) {
  id = i;
  dis[id] = 0;
  char name[64];
  sprintf(name, "log%d.txt", i);
  w += name;
  return serv_sock.Bind(port);
}
bool Server::Send() {
  unsigned char buf[kBufSize];
  buf[0] = num_serv;
  buf[1] = 0;
  unsigned i;
  for (i = 0; i < num_serv; ++i) {
    buf[2 + i*2] = i;
    buf[2 + i*2 + i] = dis[i];
  }
  for (i = 0; i < num_serv; ++i) {
    w.printf("Send routing message to server %u.\n", i);
    serv[i].Send(2 + num_serv * 2, buf);
  }
}
bool Server::Refresh() {
  puts("Refresh routing table.");  
  w.printf("Refresh %d: Link cost was changed\n", refresh_num++);
  Display();
  return DV_algo();
}
bool Server::Refresh(unsigned id, const vector<unsigned>& dv) {
  puts("Refresh routing table.");
  // TODO: DV_algo
  w.printf("Refresh %d: Receive DV from Server %d\n", refresh_num++, id);
  Display();
  PrintDVs();
  return DV_algo();
}
void Server::Update(unsigned id, unsigned c) {
  int i;
  cost[id] = c;
  puts("Update successfully");
  Refresh();
}
void Server::Display() {
  puts("Destination\tNext hop\tLink cost");
  puts("===========================================");
  for (unsigned i = 0; i < num_serv; ++i) {
    w.printf("Server %u\tServer %u\t", i, next[i]);
    if (dis[i] < inf)
      w.printf("%d\n", dis[i]);
    else
      w.printf("inf\n");
  }
  puts("===========================================");  
}
void Server::PrintDVs() {
  puts("DV of Neighbors:");
  w.printf("\t\t");
  unsigned i,j;
  for (i = 0; i < num_serv; ++i)
    w.printf("\tto %u\n", i);
  for (i = 0; i < num_serv; ++i) {
    if (dv[i].size() == 0)
      continue;
    w.printf("Server %u", i);
    for (j = 0; j < num_serv; ++j)
      if (j > dv[i].size())
        w.printf("\t-");
      else if(dv[i][j] >= inf)
        w.printf("\tinf");
      else
        w.printf("\t%u", dv[i][j]);
  }
}
void Server::Wait() {
  unsigned char buf[kBufSize];
  while (serv_sock.Ready(0)) {
    serv_sock.Recv(kBufSize, buf);
    unsigned i,from_serv;
    for (i = 0; i < num_serv; ++i)
      if (serv[i] == serv_sock)
        break;
    from_serv = i;
    vector<unsigned> t = dv[i];
    for (i = 2; i < 2 + buf[0]*2; ++i)
      t[buf[i]] = buf[i+1];
    if (i >= num_serv) {
      w.printf("Receive routing message from uknown server, discard.\n");
      continue;
    }
    w.printf("Receive routing message from server %u\n", from_serv);
    Refresh(from_serv, t);
  }
}
