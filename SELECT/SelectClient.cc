#include <sys/socket.h>	//socket(), connect()
#include <arpa/inet.h>	//inet_pton
#include <cstring>			//memXXX
#include <cassert>			//assert()
#include <cstdio>
#include <cstdlib>			//atoi()
#include <unistd.h>
#include <sys/select.h> //select() and FD_XXX()
#include <sys/time.h>
#include <csignal>
#include "../Wrapper/wrapper.h"

void sig_handle(int signo){
	printf("recieve a RST\n");
}
int main(int argc, char **argv)
{
	signal(SIGPIPE, sig_handle);
	assert(argc == 3);
	
	int clnt_sock, stdineof;
	struct sockaddr_in serv_addr;
	char buf[1024];

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family      = AF_INET;
  inet_pton(AF_INET, argv[1], &serv_addr.sin_addr);
	serv_addr.sin_port				= htons(atoi(argv[2]));

		//socket(), connect()
	clnt_sock = socket(AF_INET, SOCK_STREAM, 0);
	connect(clnt_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	if (read(clnt_sock, buf, 1024) > 0)
		fputs(buf, stdout);

	int maxfdp1;
	fd_set fdset;
	int len = 0;
	while (1){
		FD_SET(clnt_sock, &fdset);
		FD_SET(0, &fdset);
		maxfdp1 = clnt_sock + 1; //max(clnt_sock, 0);
		select(maxfdp1, &fdset, NULL, NULL, NULL);
		
		if (FD_ISSET(0, &fdset)) {
			if ( (len = read(fileno(stdin), buf, 1024)) == 0){
				stdineof = 1;
				shutdown(clnt_sock, SHUT_WR);
				FD_CLR(fileno(stdin), &fdset);
				continue;
			}

			len = write(clnt_sock, buf, len);
		}
		
		if (FD_ISSET(clnt_sock, &fdset)){	//clnt_sock readable
			if ( (len = read(clnt_sock, buf, len)) == 0){
				if (stdineof == 1)
					return 0;
				else
					unix_error("SERVER terminated prematurely");
			}
			write(fileno(stdout), buf, len);
		}
	}
	
}

