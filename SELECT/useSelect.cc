#include "SelectServer.h"
#include <cassert>
#include <string>
#include <time.h>
using namespace std;


int main(int argc, char **argv){
	assert (argc == 2); //only having a port

	EchoServer echoServer(atoi(argv[1]));
	echoServer.start();
}
