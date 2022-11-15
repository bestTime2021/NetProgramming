#include "net/TcpServer.h"
#include "net/Eventloop.h"
#include "net/Acceptor.h"
#include <stdio.h>

TcpServer::TcpServer(Eventloop *loop, char *hostname, char *service) : 
	eventloop_(loop),
	acceptor_(new Acceptor(loop, hostname, service)){
	acceptor_->setAcceptCallback(std::bind(&TcpServer::onConn, this));
}

TcpServer::~TcpServer() {printf("TcpServer dtor\n"); }

void TcpServer::start(){
	eventloop_->loop();
}

//forward function
void TcpServer::onConn() {
	printf("TcpServer::onConn() do self thing\n");
	printf("and then TcpServer::onConn() do customer thing\n");
	acceptCallback_();
	printf("onConn done\n");
}
