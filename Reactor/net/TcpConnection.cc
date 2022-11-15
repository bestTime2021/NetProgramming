#include "net/TcpConnection.h"
#include "net/Channel.h"
#include "net/Eventloop.h"

TcpConnection::TcpConnection(Eventloop *loop, std::string name, int connfd):
	eventloop_(loop),
	name_(name),
	connfd_(connfd),
	connChannel_(new Channel(eventloop_, connfd_)) {
	connChannel_->setReadCallback(std::bind(&TcpConnection::handRead, this));
	connChannel_->update();
}

void TcpConnection::handRead(){
	readCallback_();
}
