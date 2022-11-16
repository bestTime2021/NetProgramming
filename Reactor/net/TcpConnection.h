#include <string>
#include <memory>
#include <functional>

class Channel;
class Eventloop;

class TcpConnection {
public:
	TcpConnection(Eventloop*, std::string, int);
	void handRead();
	void setMessageCallback(std::function<void(TcpConnection*, char*, ssize_t)> mcb) {mcb_ = mcb;}

	//test
	int fd(){return connfd_;}

private:
	Eventloop *eventloop_;
	std::string name_;
	int connfd_;
	std::shared_ptr<Channel> connChannel_;
	std::function<void(TcpConnection*, char*, ssize_t)> mcb_;
};
