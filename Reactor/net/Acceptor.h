#include <memory>
#include <functional>

#include "net/wrapper.h"

class Channel;
class Eventloop;

class Acceptor {
public:
typedef std::function<void()> ConnCallback;
	Acceptor(char *hostname, char *service, ConnCallback);
	~Acceptor() = default;
	int listen(char*, char*);
	void handleConn();
	int accept(int, sockaddr*, socklen_t*);	
	void start();//FIXME

private:
	Eventloop* eventloop_;
	int listenfd_;
	std::shared_ptr<Channel> listenChannel_;
	std::function<void()> connCb_;
} ;
