#include"serv.h"
using namespace std;
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
  dis.fill(inf);
  cost.resize(num_serv);
  cost.fill(inf);
  next.resize(num_serv);
  dv.resize(num_serv);
  mark.resize(num_serv);
  mark.fill(false);
  unsigned i;
  for (i = 0; i < num_serv; ++i) {
    next[i] = i;
    dv[i].clear();
  }
  for (i = 0; i < num_edges; ++i) {
    unsigned id, port;
    char ip[64];
    if (fscanf(fp, "%u %s %u", &id, ip, &port) !=  3)
      return false;
    serv[id] = serv_sock.SetDest(ip, port);
  }
  while (num_edges--) {
    unsigned id, c;
    if (fscanf(fp, "%u %u", &id, &c) != 2)
      return false;
    dis[id] = cost[id] = c;
  }
  fclose(fp);
  printf("Load topology file successfully\n");
  return true;
}
bool Server::Init(unsigned i, unsigned port) {
  id = i;
  dis[id] = 0;
  Display();  
  char name[64];
  refresh_num = 1;
  sprintf(name, "log%u.txt", id);
  w += name;
  return serv_sock.Bind(port);
}
bool Server::Send() {
  unsigned char buf[kBufSize];
  unsigned i, j;
  for (i = j = 0; i < num_serv; ++i)
    if (dis[i] < inf) ++j;
  buf[0] = j;
  buf[1] = 0;
  for (i = 0; i < num_serv; ++i) {
    if (dis[i] >= inf) continue;
    buf[2 + --j*2] = i;
    buf[2 + j*2 + 1] = dis[i];
  }
  for (i = 0; i < num_serv; ++i) {
    printf("Send routing message to server %u.\n", i);
    serv_sock.Send(serv[i], 2 + num_serv * 2, buf);
  }
}
bool Server::Send(int dest_id) {
  unsigned char buf[kBufSize];
  unsigned i, j;
  for (i = j = 0; i < num_serv; ++i)
    if (dis[i] < inf) ++j;
  buf[0] = j;
  buf[1] = 0;
  for (i = 0; i < num_serv; ++i) {
    if (dis[i] >= inf) continue;
    buf[2 + --j*2] = i;
    buf[2 + j*2 + 1] = dis[i];
  }
  printf("Send routing message to server %u.\n", dest_id);
  serv_sock.Send(serv[dest_id], 2 + num_serv * 2, buf);
}
bool Server::Refresh() {
  printf("Refresh routing table.\n");  
  w.printf("Refresh %d: Link cost was changed\n", refresh_num++);
  //Display();
  return DV_algo();
}
bool Server::Refresh(unsigned id) {
  printf("Refresh routing table.\n");
  w.printf("Refresh %d: Receive DV from Server %d\n", refresh_num++, id);
  return DV_algo();
}
void Server::Update(unsigned id, unsigned c) {
  int i;
  cost[id] = c;
  printf("Update successfully\n");
  Refresh();
}
void Server::Display() {
  w.printf("Destination\tNext hop\tLink cost\n");
  w.printf("===========================================\n");
  for (unsigned i = 0; i < num_serv; ++i) {
    if (i == id) continue;
    w.printf("Server %u\tServer %u\t", i, next[i]);
    if (dis[i] < inf)
      w.printf("%d\n", dis[i]);
    else
      w.printf("inf\n");
  }
  w.printf("===========================================\n");
}
void Server::PrintDVs() {
  w.printf("DV of Neighbors:\n");
  unsigned i,j;
  w.printf("\t");
  for (i = 0; i < num_serv; ++i)
    w.printf("\tto %u", i);
  w.printf("\n");
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
    w.printf("\n");
  }
  w.printf("\n");
}
void Server::Wait() {
  unsigned char buf[kBufSize];
  while (serv_sock.Ready(0)) {
    serv_sock.Recv(kBufSize, buf);
    unsigned i, from_serv;
    for (i = 0; i < num_serv; ++i)
      if (serv_sock == serv[i])
        break;
    from_serv = i;
    dv[from_serv].resize(num_serv);
    dv[from_serv].fill(inf);
    for (i = 2; i < 2 + buf[0]*2; i+=2)
      dv[from_serv][buf[i]] = buf[i+1];
    if (from_serv >= num_serv) {
      printf("Received routing message from uknown server, discard.\n");
      continue;
    }
    printf("Received routing message from server %u\n", from_serv);
    Refresh(from_serv);
  }
}
