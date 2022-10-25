#include <sys/socket.h>	//socket(), connect()
#include <arpa/inet.h>	//inet_pton
#include <cstring>			//memXXX
#include <cassert>			//assert()
#include <cstdio>
#include <cstdlib>			//atoi()
#include <unistd.h>
#include <pthread.h>
#include <csignal>
#include "../Wrapper/wrapper.h"

static int sockfd;
void* keyboard(void *arg);
void str_cli(int sockfd)
{
	int n;
	pthread_t tid;
	char recvbuf[MAXLEN];
	//Tcp_connect
	//Pthread_create(&tid, NULL, keyboard(FILE *fp, int sock))
	//Recv from sock and put them to stdout
	

	pthread_create(&tid, NULL, keyboard, NULL);

	while ( (n=read(sockfd, recvbuf, MAXLEN)) > 0){
		write(1, recvbuf, n);
	}
	if (n < 0)
		unix_error("read()");
	if (n == 0) printf("str_cli exit\n");
}

void* keyboard(void *arg) {
	char sendline[MAXLEN];

	int n;	
	/* set sockfd as nonblocking */
	while ((n =read(0, sendline, MAXLEN)) > 0) {
			write(sockfd, sendline, n),printf("%ld\n", strlen(sendline));
	}
	unix_error("read()");
	shutdown(sockfd, SHUT_WR);
printf("keyboard exit\n");
	return NULL;
}
 
int main(int argc, char **argv) {
	sockfd = Tcp_connect(argv[1], argv[2]);
	str_cli(sockfd);

	printf("exit() main\n");
}
