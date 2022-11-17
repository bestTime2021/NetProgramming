#include "net/TcpConnection.h"
#include "net/Channel.h"
#include "net/Eventloop.h"
#include <unistd.h>
TcpConnection::TcpConnection(Eventloop *loop, std::string name, int connfd):
	eventloop_(loop),
	name_(name),
	connfd_(connfd),
	connChannel_(new Channel(eventloop_, connfd_)) {
	connChannel_->setReadCallback(std::bind(&TcpConnection::handRead, this));
	connChannel_->update();
}

void TcpConnection::handRead(){
	char buf[65535];
	int len = ::read(connChannel_->fd(), buf, sizeof(buf));
	if (len == 0){
		::close(connfd_);//FIXME
		handClose();
	}
	else if (len > 0)
		mcb_(this, buf, len);
	else
		abort();
}

void TcpConnection::handClose() {
	//self: do nothing
	//for TcpServer
	closeCb_(this);
}

void TcpConnection::connDestory() {
	if (connCb_)
		connCb_();
	eventloop_->removeChannel(&(*connChannel_));
}
