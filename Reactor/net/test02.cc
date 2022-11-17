#include "net/Eventloop.h"
#include "net/TcpServer.h"
#include "net/TcpConnection.h"
#include <iostream>
#include <cassert>
#include <unistd.h>
using namespace std;

//connfd readable: printf NEW CONN

void conn() {
	printf("Disconnected\n");
}
void hello(TcpConnection* t, char *buf, ssize_t len) {
	::write(t->fd(), buf, len);
	::write(t->fd(), "I just want to customize something\n", 35); 
	printf("read from client: %s\n", buf);
}

int main(int argc, char *argv[]) {
	assert(argc >= 2);
	char *hostname = NULL, *service = NULL; 
	if (argc == 2)
		service = argv[1];
	else{
		hostname = argv[1];
		service = argv[2];
	}

	Eventloop loop;
	TcpServer server(&loop, hostname, service);
	server.setConnCallback(conn);
	server.setMessageCallback(hello);
	server.start();
}
