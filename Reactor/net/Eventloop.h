#include <memory>
#include <vector>
#include <thread>
#include <functional>

class Poll;
class Channel;
class TimerQueue;

class Eventloop {
typedef std::function<void()> Functor;
typedef std::function<void()> TimerCallback;
public:
	Eventloop();
	~Eventloop();

	bool isInLoopThread();
	Eventloop* getEventloopOfCurrentThread();
	void updateChannels(Channel*);
	void loop();
	void doPendingFunctors();
	void addToPendingFunctors(Functor);
	void runAfter(time_t, TimerCallback);
	void waitForWaking();
	void wakeup();
	void runInLoop(Functor);
	void queueInLoop(Functor);

private:
	std::thread::id threadId_;
	std::shared_ptr<Poll> poll_; //FIXME
	std::vector<Channel*> activechannels_;
	std::shared_ptr<TimerQueue> timerqueue_;
	int eventloopfd_;
	std::shared_ptr<Channel> eventloopChannel_;
	
	std::vector<Functor> pendingFunctors_;	//add, tarverse
};
