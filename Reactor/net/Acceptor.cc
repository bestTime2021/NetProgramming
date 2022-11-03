#include "net/Acceptor.h"
#include "net/Eventloop.h"
#include "net/Channel.h"
void Acceptor::start() {eventloop_->loop();}

Acceptor::Acceptor(char *hostname, char *service, ConnCallback cb) :
	eventloop_(new Eventloop()), 
	listenfd_(listen(hostname, service)), 
	listenChannel_(new Channel(eventloop_, listenfd_)),
	connCb_(cb) 
{
	listenChannel_->setReadCallback(std::bind(&Acceptor::handleConn, this));
	listenChannel_->update();
}

int Acceptor::listen(char *hostname, char *service){
	int listenfd = Tcp_listen(hostname, service, NULL);
	return listenfd;
}

void Acceptor::handleConn() {
	int connfd = Acceptor::accept(listenfd_, NULL, NULL);
	printf("connfd: %d\n", connfd);
	Channel *channel = new Channel(eventloop_, connfd);
	channel->setReadCallback(connCb_);
	channel->update();
	printf("Accptor::handleConn()\n");
}

int Acceptor::accept(int sockfd, struct sockaddr* addr, socklen_t *addrlen) {
	int connfd = Accept(sockfd, addr, addrlen);
	return connfd;
}
