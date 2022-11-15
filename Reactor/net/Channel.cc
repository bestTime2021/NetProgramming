#include "Channel.h"
#include "Eventloop.h"

#include <unistd.h>
#include <poll.h> //FIXME
#include <stdio.h>

void Channel::handleEvent() {
//	printf("Channel::handleEvent()\n");
	if (revents_ & POLLIN) {
		char buf[1024];
		::read(fd_, buf, sizeof(buf));//discard
		readCallback_();
		printf("Channel::fd_ %d readable\n", fd_);
	}
}

void Channel::update() {
	printf("Channel::update()\n");
	events_ = POLLIN;
	eventloop_->updateChannels(this);
}
