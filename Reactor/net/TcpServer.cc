#include "net/TcpServer.h"
#include "net/TcpConnection.h"
#include "net/Eventloop.h"
#include "net/Acceptor.h"
#include <string>
#include <stdio.h>

TcpServer::TcpServer(Eventloop *loop, char *hostname, char *service) : 
	eventloop_(loop),
	acceptor_(new Acceptor(loop, hostname, service)),
	tcpConnId_(1){
	using namespace std::placeholders;
	acceptor_->setAcceptCallback(std::bind(&TcpServer::handNewConn, this, _1));
}

TcpServer::~TcpServer() {printf("TcpServer dtor\n"); }

void TcpServer::start(){
	eventloop_->loop();
}

//forward function
void TcpServer::handNewConn(int connfd) {
	std::string connId = std::to_string(tcpConnId_++);
	TcpConnection *connPtr = new TcpConnection(eventloop_, connId, connfd);
	connMap_[connId] = connPtr;
	connPtr->setConnCallback(connCb_);
	connPtr->setMessageCallback(mcb_);
	using namespace std::placeholders;
	connPtr->setCloseCallback(std::bind(&TcpServer::removeConn, this, _1));
}

void TcpServer::removeConn(TcpConnection* conn) {
	connMap_.erase(conn->name());
	eventloop_->queueInLoop(
			std::bind(&TcpConnection::connDestory, conn));
}
