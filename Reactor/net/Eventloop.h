#include <memory>
#include <vector>

class Poll;
class Channel;

class Eventloop {
public:
	Eventloop();
	~Eventloop() = default;

	void updateChannels(Channel*);
	void loop();

private:
	std::shared_ptr<Poll> poll_; //FIXME
	std::vector<Channel*> activechannels_;
};
