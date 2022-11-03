#include "net/Acceptor.h"
#include <iostream>
#include <cassert>
using namespace std;

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

	Acceptor acceptor(hostname, service, hello);
	acceptor.start();
}
