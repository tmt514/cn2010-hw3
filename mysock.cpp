#include"mysock.h"
#include<ctime>
#include<cstdio>
#include<cstring>
#include<unistd.h>
#include<sys/time.h>
#include<arpa/inet.h>
#include<sys/socket.h>
/* open an UDP connection */
MySock::MySock() {
	sd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sd < 0)
		perror("socket");
}
bool MySock::operator==(const sockaddr_in& s)const {
  return from.sin_addr.s_addr == s.sin_addr.s_addr
    && from.sin_port == s.sin_port;
}
/* bind to a specified port */
bool MySock::Bind(unsigned port) {
	sockaddr_in addr;
	bzero(&addr,sizeof(addr));
	addr.sin_family = AF_INET;  
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(port);
	int r = bind(sd,(sockaddr *)&addr,sizeof(addr));
	if (r < 0) {
		perror("bind");
    return false;
  }
	return true;
}
/* set a neighbor's destination IP and port */
sockaddr_in MySock::SetDest(const char ip[],unsigned port) {
	bzero(&dest,sizeof(dest));
	dest.sin_family = AF_INET;
	dest.sin_addr.s_addr = inet_addr(ip);
	dest.sin_port = htons(port);
  return dest;
}
/* non-blocking IO */
bool MySock::Ready(int t) {
	fd_set fds;
	FD_ZERO(&fds);
 	FD_SET(sd, &fds);
  FD_SET(0, &fds);
 	select(sd + 1, &fds, NULL, NULL, NULL);
 	return FD_ISSET(sd, &fds);
}
/*int MySock::Send(unsigned len, const void *buf) {
  return sendto(sd, buf, len, 0, (sockaddr *)&dest, sizeof(dest));
}*/

/* send to a specified destination */
int MySock::Send(sockaddr_in d, unsigned len, const void *buf) {
  return sendto(sd, buf, len, 0, (sockaddr *)&d, sizeof(d));
}
/* receive DV from a neighbor */
int MySock::Recv(unsigned len, void *buf) {
  socklen_t from_len = sizeof(from);
  return recvfrom(sd, buf, len, 0, (sockaddr *)&from, &from_len);
}

inline long long Wait(long long t){
	static long long last = 0;
	struct timespec tp;
	if (clock_gettime(CLOCK_REALTIME, &tp)) {
		perror("clock_gettime()");
		return -1;
	}
	long long rt = t - (tp.tv_sec * 1000000000ll + tp.tv_nsec - last)/1000;
	last = tp.tv_sec * 1000000000ll + tp.tv_nsec;  
	if (rt > 0) usleep(rt);
	return 0;
}

inline long long Timer(int type){
	static long long last = 0;
	struct timespec tp;
	if (clock_gettime(CLOCK_REALTIME, &tp)) {
		perror("clock_gettime()");
		return -1;
	}
	long long rt = (tp.tv_sec * 1000000000ll + tp.tv_nsec - last)/1000;
	if(type == 1)
		last = tp.tv_sec * 1000000000ll + tp.tv_nsec;
	return rt;
}
