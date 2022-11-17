#include "net/Timer.h"
#include "net/Eventloop.h"
#include "net/TimerQueue.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

void resetTimerfd(int timerfd_, time_t expirationT) {
	int expiration = expirationT - time(NULL);
	struct itimerspec newValue;
	struct itimerspec oldValue;
	memset(&newValue, 0, sizeof(newValue));
	memset(&oldValue, 0, sizeof(oldValue));

	newValue.it_value.tv_sec = expiration;
	newValue.it_value.tv_nsec = 0;
	::timerfd_settime(timerfd_, 0, &newValue, &oldValue);
	printf("resetTimerfd %ds\n", expiration);
}

void TimerQueue::addTimer(time_t expiration, TimerCallback tcb) {

	if (tcb)printf("TimerQueue::addTimer()\n");
	Timer *timer = new Timer(expiration, tcb);
	eventloop_->runInLoop(std::bind(&TimerQueue::addTimerInLoop, this, timer));
}

void TimerQueue::addTimerInLoop(Timer* timer) {
	assert(eventloop_->isInLoopThread());
	if (timers_.empty()) 
		resetTimerfd(timerfd_, timer->expiration());
	timers_.insert(Entry(timer->expiration(), timer));
}

void TimerQueue::handleRead() {
	printf("TimerQueue::handleRead()\n");
	std::vector<Entry> expired;
	getExpired(time(NULL), expired);
	for(auto &iter : expired) {
		iter.second->runCallback();
	}
	//erase() expired
	//resetTimerfd
	
	time_t expiration = (timers_.empty()) ? time(NULL) : timers_.begin()->second->expiration();
	resetTimerfd(timerfd_, expiration);
}

void TimerQueue::getExpired(time_t now, std::vector<Entry>& expired) {
	printf("TimerQueue::getExpired()\n");
	//use sentry to find expired from timers_
	Entry sentry(now, reinterpret_cast<Timer*>(UINTPTR_MAX));
	auto iter = timers_.lower_bound(sentry);
	std::copy(timers_.begin(), iter, back_inserter(expired));

	timers_.erase(timers_.begin(), iter);
}
