#include "Channel.h"
#include "Eventloop.h"

#include <unistd.h>
#include <poll.h> //FIXME
#include <stdio.h>

void Channel::handleEvent() {
//	printf("Channel::handleEvent()\n");
	if (revents_ & POLLIN) {
		readCallback_();
		printf("Channel::fd_ %d readable\n", fd_);
		char buf[1024];
		int len = read(fd_, buf, sizeof(buf));
		write(fd_, buf, len);
		*(buf+len) = 0;
		printf("%s\n", buf);
	}
}

void Channel::update() {
	printf("Channel::update()\n");
	events_ = POLLIN;
	eventloop_->updateChannels(this);
}
