#include "net/Eventloop.h"
#include "net/Poll.h"
#include "net/Channel.h"

Eventloop::Eventloop(): poll_(new Poll()) {}

void Eventloop::updateChannels(Channel *channelp) {
	poll_->updateActiveChannels(channelp);
}

void Eventloop::loop() {
	while (1) {
		activechannels_.clear();
		poll_->poll(&activechannels_);
		for (auto channelp: activechannels_) {
			channelp->handleEvent();
		}
	}
}
