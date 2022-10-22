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
#include <cassert>
#define MAXLEN 1024

void unix_eror(const char*);
void app_error(const char*);

const char *Inet_ntop(int family, const void *src, char *dst, size_t len);
void Inet_pton(int family, const char *src, void *dst);

int Socket(int domain, int type, int protocol);
void Bind(int sockfd, struct sockaddr*, socklen_t len);
void Listen(int servfd, int blocklog);
int Accept(int servfd, struct cliaddr*, socklen_t &clilen);
int Connect(int servfd, struct servaddr*, socklen_t &servlen);
int Tcp_connect(const char *hostname, const char *service);
int Tcp_listen(const char *hostname, const char *service);
void Close(int sockfd);

int getSendBuf(int sockfd);
void Set_NONBLOCK(int sockfd);
void Set_REUSEADDR(int sockfd);

void unix_error(const char *msg){
	fprintf(stderr, "%s: %s\n", msg, strerror(errno));
	exit(0);
}

void app_error(const char *msg) {
	fprintf(stderr, "%s\n", msg);
	exit(0);
}


const char* Inet_ntop(int family, const void *src, char *dst, size_t size) {
	const char *ret = inet_ntop(family, src, dst, size);

	if ( ret == NULL)
		unix_error("inet_ntop() error");
	return ret;
}

void Inet_pton(int family, const char *src, void *dst) {
	int rc;

	if ( (rc = inet_pton(family, src, dst)) == 0)
		app_error("inet_pton error: invaild dotted-decimal address");
	else if (rc < 0)
		unix_error("inet_pton() error");
}
int Socket(int domain, int type, int protocol){
	int sockfd;

	if ( (sockfd = socket(domain, type, protocol)) < 0)
		unix_error("socket() error");
	return sockfd;
}

void Bind(int sockfd, struct sockaddr* addr, socklen_t len){
	if (bind(sockfd, addr, len) < 0)
		unix_error("bind() error");
}

void Listen(int sockfd, int blocklog){
	if (getenv("LISTENQ") != NULL)
		blocklog = atoi(getenv("LISTENQ"));

	if (listen(sockfd, blocklog) < 0)
		unix_error("listen() error");
}

int Accept(int sockfd, struct sockaddr* addr, socklen_t *addrlen){
	int clntsock;

	if ( (clntsock = accept(sockfd, addr, addrlen)) < 0)
		unix_error("accept() error");
	return clntsock;
}

int Connect(int sockfd, struct sockaddr* addr, socklen_t addrlen){
	int clntsock;

	if ( (clntsock = connect(sockfd, addr, addrlen)) < 0)
		unix_error("connect() error");
	return clntsock;
}

int Tcp_connect(const char *hostname, const char *service) {
	int sockfd;
	struct addrinfo hints, *res, *ressave;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;	//FIXME AF_UNSPEC can resolve IPv4 and IPv6.
	hints.ai_socktype = SOCK_STREAM;

	if ( getaddrinfo(hostname, service, &hints, &res) != 0)
			app_error("getaddrinfo() error");
	ressave = res;

	do {
		sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

		if (sockfd < 0)
			continue;
		if (connect(sockfd, res->ai_addr, res->ai_addrlen) == 0) //success connect
			break;

		Close(sockfd);
		res = res->ai_next;
	} while (res != NULL);
	
	if (res == NULL)
		app_error("Tcp_connect() error");

	freeaddrinfo(ressave);

	return sockfd;
}

int Tcp_listen(const char *hostname, const char *service, socklen_t *addrlenp) {
	int listenfd, n;
	struct addrinfo hints, *res, *ressave;
	memset(&hints, 0, sizeof(hints));
	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_INET; //FIXME AF_UNSPEC
	hints.ai_socktype = SOCK_STREAM;

	//FIXME
	if ( (n =getaddrinfo(NULL, service, &hints, &res)) != 0)
		printf("getaddrinfo() error: %s", gai_strerror(n)), exit(0);

	ressave = res;
	assert(res != NULL);
	do {
		listenfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		
		if (listenfd < 0)
			continue;
		Set_REUSEADDR(listenfd);
		if (bind(listenfd, res->ai_addr, res->ai_addrlen) == 0)
			break;
		Close(listenfd);
		res = res->ai_next;
	} while (res != NULL);

	if (res == NULL)
		app_error("Tcp_listen() error, maybe bind() error");

	if (addrlenp)
		*addrlenp = res->ai_addrlen;
	Listen(listenfd, 5);

	freeaddrinfo(ressave);
	return listenfd;
}

void Close(int sockfd){
	if (close(sockfd) < 0)
		unix_error("close() error");
}

int getSendBuf(int sockfd) {
	int sendbuf;
	socklen_t optlen = sizeof(sendbuf);

	if (getsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &sendbuf, &optlen) < 0)
		unix_error("getsockopt() error");
	return sendbuf;
}

void Set_NONBLOCK(int sockfd) {
	int flags;
	
	/* set sockfd as nonblocking */
	if ( (flags = fcntl(sockfd, F_GETFL, 0)) < 0)
		unix_error("fcntl() GET error");
	flags |= O_NONBLOCK;
	if (fcntl(sockfd, F_SETFL, flags) < 0)
		unix_error("fcntl() SET error");
}

void Set_REUSEADDR(int sockfd) {
	int on = 1;	//not zero: turn on the option
	socklen_t optlen = sizeof(on);

	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (void *)&on, optlen) < 0)
		unix_error("setsockopt() error");
}
