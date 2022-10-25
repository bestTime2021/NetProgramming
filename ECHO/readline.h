#include <cstdlib>
#include <unistd.h>
#include <errno.h>
#define MAXLEN 1024

static int remains;
static char recvbuf[MAXLEN];
static char *willread;

int my_read(int sockfd, char *c) {
	if (remains <= 0) { //should read()
		while ( (remains = read(sockfd, recvbuf, MAXLEN)) < 0)
			if (errno != EINTR)
				abort();
		if (remains == 0)
			return 0;
		willread = recvbuf;
	}

	*c = *willread++;
	remains--;

	return 1;
}

int readline(int sockfd, char *line) {
	int n;
	char c;

	n = 0;
	while (my_read(sockfd, &c) == 1) {
		*line++ = c;
		n++;
		if (c == '\n') break;
	}
	return n; //0 or n, but if error abort it.
}
