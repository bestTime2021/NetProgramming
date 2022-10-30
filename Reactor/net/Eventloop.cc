#include "net/Eventloop.h"
#include "net/Poll.h"
#include "net/Channel.h"

__thread Eventloop* t_loopInThisThread = 0;
Eventloop::Eventloop(): poll_(new Poll()) {
	if (t_loopInThisThread != nullptr) {
		abort();	//FIXME
	} else {
		t_loopInThisThread = this;
	}
}

Eventloop::~Eventloop() {
	t_loopInThisThread = nullptr;
}

bool Eventloop::isInLoopThread() { return threadId_ == std::this_thread::get_id(); }

Eventloop* Eventloop::getEventloopOfCurrentThread() {
	return t_loopInThisThread;
}

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
