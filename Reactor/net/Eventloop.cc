#include "net/Eventloop.h"
#include "net/Poll.h"
#include "net/TimerQueue.h"
#include <sys/eventfd.h>
#include <unistd.h>
#include <stdint.h>
__thread Eventloop* t_loopInThisThread = 0;
Eventloop::Eventloop(): poll_(new Poll()), timerqueue_(new TimerQueue(this)), 
	eventloopfd_(::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC)), 
	eventloopChannel_(new Channel(this, eventloopfd_))
{
	if (t_loopInThisThread != nullptr) {
		abort();	//FIXME
	} else {
		t_loopInThisThread = this;
	}
	eventloopChannel_->setReadCallback(std::bind(&Eventloop::waitForWaking, this));
	eventloopChannel_->update();
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
		doPendingFunctors();
	}
}

void Eventloop::doPendingFunctors() {
	std::vector<Functor> functors;
	functors.swap(pendingFunctors_);

	for (auto &func : functors) {
		func();
	}
}

void Eventloop::addToPendingFunctors(Functor f) {
	pendingFunctors_.push_back(f);
}

void Eventloop::runAfter(time_t delay, TimerCallback cb) {
	time_t expiration = time(NULL) + delay;
	timerqueue_->addTimer(expiration, cb);
}

void Eventloop::waitForWaking() {
	uint64_t makeCounter0 = 1;
	if (::read(eventloopfd_, &makeCounter0, sizeof(makeCounter0)) < 0)
		abort();
}

void Eventloop::wakeup() {
	uint64_t on = 1;
	if (::write(eventloopfd_, &on, sizeof(on)) < 0)
		abort();
}

void Eventloop::runInLoop(Functor func) {
	if (isInLoopThread())
		func();
	else 
		queueInLoop(func);
}

void Eventloop::queueInLoop(Functor func) {
	pendingFunctors_.push_back(func);
	if (!isInLoopThread())
		wakeup();
}

