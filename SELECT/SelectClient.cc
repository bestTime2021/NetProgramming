#include <sys/socket.h>	//socket(), connect()
#include <arpa/inet.h>	//inet_pton
#include <cstring>			//memXXX
#include <cassert>			//assert()
#include <cstdio>
#include <cstdlib>			//atoi()
#include <unistd.h>
#include <sys/select.h> //select() and FD_XXX()
#include <sys/time.h>
#include "../Wrapper/wrapper.h"

#define max(a, b) a > b ? a : b
void Str_cli(FILE *fp, int sockfd);
int main(int argc, char **argv)
{
	assert(argc == 3);
	int connfd;
	
	connfd = Tcp_connect(argv[1], argv[2]);
	
	Str_cli(stdin, connfd);

	exit(0);
}

void Str_cli(FILE *fp, int sockfd) {
	int maxfdp1, stdineof;
	ssize_t n, nwritten;
	fd_set rset, wset;
	char to[MAXLEN], fr[MAXLEN];
	char *toiptr, *tooptr, *froptr, *friptr;

	Set_NONBLOCK(sockfd);
	Set_NONBLOCK(fileno(stdin));
	Set_NONBLOCK(fileno(stdout));

	toiptr = tooptr = to;
	friptr = froptr = fr;
	stdineof = 0;

	maxfdp1 = max(1, sockfd) + 1;
	while (1) {
		FD_ZERO(&rset);
		FD_ZERO(&wset);
		if (stdineof == 0 && toiptr < &to[MAXLEN]) //to Server
			FD_SET(STDIN_FILENO, &rset);
		if (friptr < &fr[MAXLEN])
			FD_SET(sockfd, &rset);
		if (tooptr != toiptr)
			FD_SET(sockfd, &wset);
		if (froptr != friptr)
			FD_SET(STDOUT_FILENO, &wset);

		select(maxfdp1, &rset, &wset, NULL, NULL);
		if (FD_ISSET(STDIN_FILENO, &rset)) {
			if ( (n = read(STDIN_FILENO, toiptr, &to[MAXLEN]-toiptr)) < 0) {
				if (errno != EWOULDBLOCK)
					unix_error("read() in select error");
			} else if (n == 0) {
				fprintf(stderr, "EOF on stdin\n");
				stdineof = 1;
				if (tooptr == toiptr)		//nothing to write
					shutdown(sockfd, SHUT_WR);
			} else {//n > 0 read success.
				fprintf(stderr, "read() %ld bytes from stdin\n", n);
				toiptr += n;
				FD_SET(sockfd, &wset);
			}
		}

		if (FD_ISSET(sockfd, &rset)) {	//read from sockfd
			if ( (n = read(sockfd, friptr, &fr[MAXLEN]-friptr)) < 0) {
				if (errno != EWOULDBLOCK)
					unix_error("read() error in select\n");
			} else if (n == 0) {
				fprintf(stderr, "EOF on sockfd\n");
				if (stdineof == 1)
					return;
				else
					unix_error("Server Close");
			} else {
				fprintf(stderr, "read() %ld bytes from SERVER\n", n);
				friptr += n;
				FD_SET(STDOUT_FILENO, &wset);
			}
		}
		
		if (FD_ISSET(STDOUT_FILENO, &wset)) {
			if ( (nwritten = write(STDOUT_FILENO, froptr, friptr-froptr)) < 0) {
				if (errno != EWOULDBLOCK)
					unix_error("write() error in Select stdout");
			} else {
				fprintf(stderr, "write() %ld bytes to stdout\n\n", nwritten);
				froptr += nwritten;
				if (froptr == friptr)
					froptr = friptr = fr;
			}
		}
		
		if (FD_ISSET(sockfd, &wset)) { //buf have data send to SERVER
			if ( (nwritten = write(sockfd, tooptr, toiptr-tooptr)) < 0) {
				if (errno != EWOULDBLOCK)
					unix_error("write() error in select sockfd");
			} else{// if (nwritten >= 0)
				fprintf(stderr, "write() %ld bytes to SERVER\n", nwritten);
				tooptr += nwritten;
				if (tooptr == toiptr){
					tooptr = toiptr = to;
					if (stdineof == 1)
						shutdown(sockfd, SHUT_WR);
				}
			}
		}
	}
}
