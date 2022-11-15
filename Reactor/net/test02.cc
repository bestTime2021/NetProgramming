#include "net/Eventloop.h"
#include "net/TcpServer.h"
#include <iostream>
#include <cassert>
using namespace std;

//connfd readable: printf NEW CONN

void hello() {
	cout << "NEW CONN!\n" << endl;
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
	server.setAcceptCallback(hello);
	server.start();
}
