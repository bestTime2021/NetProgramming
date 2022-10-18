#include "EchoServer.h"
#include <cassert>
#include <cstring>

int main(int argc, char **argv){
	assert (argc == 2); //only having a port

	EchoServer echoServer(atoi(argv[1]));
	echoServer.start();
}
