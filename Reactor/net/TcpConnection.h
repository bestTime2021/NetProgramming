#include <string>
#include <memory>
#include <functional>

class Channel;
class Eventloop;

class TcpConnection {
public:
	TcpConnection(Eventloop*, std::string, int);
	void handRead();
	void setConnCallback(std::function<void()> cb) {readCallback_ = cb;}

private:
	Eventloop *eventloop_;
	std::string name_;
	int connfd_;
	std::shared_ptr<Channel> connChannel_;
	std::function<void()> readCallback_;
};
