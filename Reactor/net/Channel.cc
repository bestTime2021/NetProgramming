#include "Channel.h"
#include "Eventloop.h"

#include <poll.h> //FIXME
#include <stdio.h>

void Channel::handleEvent() {
	printf("Channel::handleEvent()\n");
	if (revents_ & POLLIN) {
		readCallback_();
	}
}

void Channel::update() {
	printf("Channel::update()\n");
	events_ = POLLIN;
	eventloop_->updateChannels(this);
}
