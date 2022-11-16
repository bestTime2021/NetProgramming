#include <memory>
#include <functional>

#include "net/wrapper.h"

class Channel;
class Eventloop;

class Acceptor {
public:
typedef std::function<void()> ConnCallback;
	Acceptor(Eventloop *loop, char *hostname, char *service);
	~Acceptor() = default;
	int listen(char*, char*);
	void handleAccept();
	int accept(int, sockaddr*, socklen_t*);	
	void setAcceptCallback(std::function<void(int)> aCb)
	{	acceptCb_ = aCb; }

private:
	Eventloop* eventloop_;
	int listenfd_;
	std::shared_ptr<Channel> listenChannel_;
	std::function<void(int)> acceptCb_;
} ;
