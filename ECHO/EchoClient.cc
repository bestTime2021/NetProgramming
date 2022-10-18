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
	struct sockaddr_in serv_addr;
	char buf[1024];

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family      = AF_INET;
  inet_pton(AF_INET, argv[1], &serv_addr.sin_addr);
	serv_addr.sin_port				= htons(atoi(argv[2]));

	clnt_sock = Socket(AF_INET, SOCK_STREAM, 0);
	Connect(clnt_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	if (read(clnt_sock, buf, 1024) > 0)
		fputs(buf, stdout);

	while (1){
		printf("\nSEND: ");
		if (fgets(buf, 1024, stdin) != NULL){
			int len;
			if ( (len = write(clnt_sock, buf, strlen(buf))) < 0)
				unix_error("write() error");
			if (read(clnt_sock, buf, len) <= 0)
				printf("read()<=0\n, exit"), exit(0);
			if (len > 0)
				printf("ECHO: %s", buf);
			}
	}
	
}

