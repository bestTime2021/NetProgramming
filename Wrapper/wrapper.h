#include <cerrno>
#include <cstdlib>	//exit()
#include <cstdio>		//fprintf(), FILE obj
#include <cstring>  //strerror()
#include <csignal>	//signal()
#include <arpa/inet.h>	//socklen_t
#include <sys/socket.h>	//socket(), bind(), listen(), accept(), connect()
#include <unistd.h>		//close()

#define MAXLEN 1024

void unix_eror(const char*);
int Socket(int domain, int type, int protocol);
void Bind(int sockfd, struct sockaddr*, socklen_t len);
void Listen(int servfd, int blocklog);
int Accept(int servfd, struct cliaddr*, socklen_t &clilen);
int Connect(int servfd, struct servaddr*, socklen_t &servlen);
void Close(int sockfd);


void unix_error(const char *msg){
	fprintf(stderr, "%s: %s\n", msg, strerror(errno));
	exit(0);
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

void Close(int sockfd){
	if (close(sockfd) < 0)
		unix_error("close() error");
}

