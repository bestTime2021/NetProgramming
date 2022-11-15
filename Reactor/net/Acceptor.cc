#include "net/Acceptor.h"
#include "net/Eventloop.h"
#include "net/Channel.h"

Acceptor::Acceptor(Eventloop *loop, char *hostname, char *service) :
	eventloop_(loop), 
	listenfd_(listen(hostname, service)), 
	listenChannel_(new Channel(eventloop_, listenfd_))
{
	listenChannel_->setReadCallback(std::bind(&Acceptor::handleAccept, this));
	listenChannel_->update();
}

int Acceptor::listen(char *hostname, char *service){
	int listenfd = Tcp_listen(hostname, service, NULL);
	return listenfd;
}

void Acceptor::handleAccept() {
	int connfd = Acceptor::accept(listenfd_, NULL, NULL);
	printf("connfd: %d\n", connfd);
	acceptCb_();
	printf("Accptor::handleConn() done\n");
}

int Acceptor::accept(int sockfd, struct sockaddr* addr, socklen_t *addrlen) {
	int connfd = Accept(sockfd, addr, addrlen);
	return connfd;
}
