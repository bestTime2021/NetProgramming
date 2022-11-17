#include <cerrno>
#include <cstdlib>	//exit()
#include <cstdio>		//fprintf(), FILE obj
#include <cstring>  //strerror()
#include <csignal>	//signal()
#include <arpa/inet.h>	//socklen_t
#include <sys/socket.h>	//socket(), bind(), listen(), accept(), connect()
#include <unistd.h>		//close()
#include <fcntl.h>	//fcntl()
#include <netdb.h>	//getaddrinfo()
#include <time.h>		//timerfd_create(), timerfd_settime()
#include <sys/eventfd.h>	//eventfd()
#include <cassert>
#define MAXLEN 1024

void unix_eror(const char*);
void app_error(const char*);

const char *Inet_ntop(int family, const void *src, char *dst, size_t len);
void Inet_pton(int family, const char *src, void *dst);

int Socket(int domain, int type, int protocol);
void Bind(int sockfd, struct sockaddr*, socklen_t len);
void Listen(int servfd, int blocklog);
int Accept(int servfd, struct sockaddr*, socklen_t *clilen);
int Connect(int servfd, struct sockaddr*, socklen_t *servlen);
int Tcp_connect(const char *hostname, const char *service);
int Tcp_listen(const char *hostname, const char *service, socklen_t *addrlen);
void Close(int sockfd);

int getSendBuf(int sockfd);
void Set_NONBLOCK(int sockfd);
void Set_REUSEADDR(int sockfd);


