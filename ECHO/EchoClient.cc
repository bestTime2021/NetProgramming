#include <sys/socket.h>	//socket(), connect()
#include <arpa/inet.h>	//inet_pton
#include <cstring>			//memXXX
#include <cassert>			//assert()
#include <cstdio>
#include <cstdlib>			//atoi()
#include <unistd.h>
#include <csignal>
#include "../Wrapper/wrapper.h"

void sig_handle(int signo){
	printf("recieve a RST\n");
}
int main(int argc, char **argv)
{
	signal(SIGPIPE, sig_handle);
	assert(argc == 3);
	
	int clnt_sock;
	char buf[1024];

	clnt_sock = Tcp_connect(argv[1], argv[2]);
	if (read(clnt_sock, buf, 1024) > 0)
		fputs(buf, stdout);

	while (1){
		printf("\nSEND: ");
		if (fgets(buf, 1024, stdin) != NULL){
			int len;
			if ( (len = write(clnt_sock, buf, strlen(buf))) < 0)
				unix_error("write() error");
			if (read(clnt_sock, buf, len) <= 0)
				printf("read()<=0, exit\n"), exit(0);
			if (len > 0)
				printf("ECHO: %s", buf);
			}
	}
	
}

