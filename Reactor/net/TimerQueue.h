#include <time.h>
#include <memory>
#include <vector>

class Timer;
class Eventloop;

class TimerQueue {
typedef std::pair<time_t, Timer*> Entry;
typedef std::vector<Entry> TimerList;

public:
	void addTimer(); //add Timer to pendingFunctors
	std::vector<Entry> getExpired(); //to get Expired Timers by now time.

private:
	Eventloop *eventloop_;
	TimerList timers_;
};
