#include<cstdio>
#include<cstring>
#include"serv.h"
int main(int argc, char **argv) {
  unsigned int i, port = 65637, id = 257;
  bool topo = true;
  Server serv;
  for (i = 1; i + 1 < argc; ++i) {
    if (!strcmp(argv[i], "-p"))
      sscanf(argv[++i], "%u", &port);
    else if (!strcmp(argv[i], "-t"))
      topo |= serv.Load(argv[++i]);
    else if (!strcmp(argv[i], "-d"))
      sscanf(argv[++i], "%u", &id);
  }
  if (!topo || !serv.Init(id, port)){
	  fprintf(stderr, "Usage: DV_routing -p <port> -t <topo-file> -d <server-id>\n");
    return 0;
  }
  while (1) {
    printf("> ");
    fflush(stdout);
    serv.Wait();
    char cmd[64];
    scanf("%s", cmd);
    if (!strcmp(cmd, "send"))
      serv.Send();
    else if (!strcmp(cmd, "update")) {
      unsigned id, cost;
      scanf("%u %u", &id, &cost);
      serv.Update(id, cost);
    }
    else if (!strcmp(cmd, "display"))
      serv.Display();
    else if (!strcmp(cmd, "end"))
      break;
  }
}
