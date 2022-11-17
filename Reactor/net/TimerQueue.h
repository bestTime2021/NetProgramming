#include <time.h>
#include <sys/timerfd.h>
#include <memory>
#include <vector>
#include <set>
#include <functional>

class Timer;
class Eventloop;
#include "net/Channel.h"

class TimerQueue {
typedef std::function<void()> TimerCallback;
typedef std::pair<time_t, Timer*> Entry;
typedef std::set<Entry> TimerList;

public:
	TimerQueue(Eventloop *e): eventloop_(e),
	timerfd_(::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC)),
	timerchannel_(e, timerfd_), 
	timers_()
	{
		timerchannel_.setReadCallback(std::bind(&TimerQueue::handleRead, this));
		timerchannel_.update();
	}
	void addTimer(time_t expiration, TimerCallback); //add Timer to pendingFunctors
	void addTimerInLoop(Timer*);
	void handleRead();
	void getExpired(time_t now, std::vector<Entry>&); //to get Expired Timers by now time.
	void reset(TimerList &expired, time_t now);

private:
	Eventloop *eventloop_;
	int timerfd_;
	Channel timerchannel_;
	TimerList timers_;
};
