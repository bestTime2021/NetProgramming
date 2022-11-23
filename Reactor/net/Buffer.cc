#include "net/Buffer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/uio.h>
#include <unistd.h>

Buffer::Buffer():
	data(1024),
	readIndex(0),
	writeIndex(0) {}

int Buffer::readFd(int fd) {
	char extraBuf[65535];
	struct iovec iov[2];
	int len;

	iov[0].iov_base = (void*) &data[writeIndex];
	iov[0].iov_len = data.size() - writeIndex;
	iov[1].iov_base = (void*) extraBuf;
	iov[1].iov_len = sizeof(extraBuf);

	len = ::readv(fd, iov, 2);
	if (len < 0) {
		printf("readv Error\n");
		abort();
	} else if (len <= data.size()-writeIndex) {
		writeIndex += len;
	} else { //extraBuf also has data.
		int extraLen = strlen(extraBuf);
		for (int i = 0; i < extraLen; ++i) {
			data.push_back(extraBuf[i]);
			writeIndex++;
		}
	}
	return len;
}

int Buffer::readableBytes() {
	return (writeIndex - readIndex);
}
