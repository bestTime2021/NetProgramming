#include <time.h>
#include <atomic>
#include <functional>
#include <stdio.h>

class Timer {
typedef std::function<void()> TimerCallback;
public:
static int s_numCreated;	//FIXME
	Timer(time_t expiration, TimerCallback cb) : 
		cb_(cb), expiration_(expiration), sequeue_(++s_numCreated) {}
	time_t expiration() { return expiration_; }
	void runCallback() {if(cb_) cb_();printf("Timer::runCallback()\n");  }

private:
	TimerCallback cb_;
	time_t expiration_;
	int sequeue_;
};

