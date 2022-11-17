#include <string>
#include <memory>
#include <functional>

class Channel;
class Eventloop;

class TcpConnection {
public:
	typedef std::function<void(TcpConnection*, char*, ssize_t)> MessageCallback;
	typedef std::function<void()> ConnCallback;
	typedef std::function<void(TcpConnection*)> CloseCallback; //not for user

	TcpConnection(Eventloop*, std::string, int);
	void handRead();
	void handClose();
	void connDestory();

	void setMessageCallback(MessageCallback mcb) { mcb_ = mcb; }
	void setConnCallback(ConnCallback ccb) { connCb_ = ccb; }
	void setCloseCallback(CloseCallback ccb) { closeCb_ = ccb;}

	//test
	int fd(){return connfd_;}
	std::string name() { return name_; }

private:
	Eventloop *eventloop_;
	std::string name_;
	int connfd_;
	std::shared_ptr<Channel> connChannel_;

	MessageCallback mcb_;
	ConnCallback connCb_;
	CloseCallback closeCb_;
};
