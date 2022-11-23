#include "net/Eventloop.h"
#include "net/Buffer.h"
#include "net/TcpServer.h"
#include "net/TcpConnection.h"
#include <stdio.h>
#include <assert.h>
#include <unistd.h>

void onConn();
void onMessage(TcpConnection*, Buffer*);

int main(int argc, char *argv[]) {
	assert(argc >= 2);
	
	char *hostname, *service;
	if (argc == 2) {
		hostname = NULL;
		service = argv[1];
	} else {
		hostname = argv[1];
		service = argv[2];
	}

	Eventloop loop;
	TcpServer server(&loop, hostname, service);
	server.setConnCallback(onConn);
	server.setMessageCallback(onMessage);
	server.start();
}

void onConn() { printf("disConn\n"); }
void onMessage(TcpConnection* tcpConn, Buffer* buf) {
	::write(tcpConn->fd(), "Hi!", 3);//FIXME for echo.
	printf("buf have read %d bytes\n", buf->readableBytes());
}

