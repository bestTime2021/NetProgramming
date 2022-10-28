#include <functional>
class Eventloop;

class Channel {
public:
	Channel(Eventloop *eventloop, int fd):
		eventloop_(eventloop),
		fd_(fd),
		events_(0),
		revents_(0),
		index_(-1){}

	const int fd() { return fd_; }
	int events() { return events_; }
	int revents() { return revents_; }
	int index() { return index_; }

	void setEvents(int events) { events_ = events; }
	void setRevents(int revents) { revents_ = revents; }
	void setIndex(int index) { index_ = index; }
	void setReadCallback();

	void handleEvent();

	void update();
private:

	Eventloop *eventloop_;
	const int fd_;
	int events_;
	int revents_;
	int index_;	//for poll

	std::function<void()> readCallback_;
};
