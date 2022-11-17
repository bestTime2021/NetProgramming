#include "net/Channel.h"
#include "net/Eventloop.h"

#include <unistd.h>
#include <poll.h> //FIXME
#include <stdio.h>

void Channel::handleEvent() {
	if ((revents_ & POLLHUP) && !(revents_ & POLLIN)) {
		if(closeCallback_)
			closeCallback_();
	}
	if (revents_ & POLLIN) {
		if (readCallback_)
			readCallback_();
		printf("Channel::fd_ %d readable\n", fd_);
	}
}

void Channel::update() {
	printf("Channel::update()\n");
	events_ = POLLIN | POLLHUP;
	eventloop_->updateChannels(this);
}
