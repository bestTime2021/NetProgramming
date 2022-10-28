#include "Channel.h"
#include "Eventloop.h"

#include <poll.h> //FIXME
#include <stdio.h>

void Channel::handleEvent() {
	if (revents_ & POLLIN) {
		for (int i = 0; i < 5; ++i)
			printf("you will call a func() in Channel::handleEvent 5 times\n");		
		printf("done\n\n");
	}
}

void Channel::update() {
	events_ = POLLIN;
	eventloop_->updateChannels(this);
}
