#include <memory>
#include <vector>
#include <thread>

class Poll;
class Channel;

class Eventloop {
public:
	Eventloop();
	~Eventloop();

	bool isInLoopThread();
	Eventloop* getEventloopOfCurrentThread();
	void updateChannels(Channel*);
	void loop();

private:
	std::thread::id threadId_;
	std::shared_ptr<Poll> poll_; //FIXME
	std::vector<Channel*> activechannels_;
};
