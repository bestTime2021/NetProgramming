#include <string>
#include <map>
#include <memory>
#include <vector>
#include <functional>

class Eventloop;
class Acceptor;
class TcpConnection;

class TcpServer 
{
public:
	typedef std::function<void(TcpConnection*, char*, ssize_t)> MessageCallback;
	typedef std::function<void()> ConnCallback;
	TcpServer(Eventloop *loop, char *hostname, char *service);
	~TcpServer();

	void start();
	void handNewConn(int connfd);

	void removeConn(TcpConnection*);

	void setConnCallback(ConnCallback ccb) {	connCb_ = ccb; }
	void setMessageCallback(MessageCallback mcb) { mcb_ = mcb; }

private:
	Eventloop* eventloop_;
	std::map<std::string, TcpConnection*> connMap_;
	std::shared_ptr<Acceptor> acceptor_;
	
	ConnCallback connCb_;
	MessageCallback mcb_;
	int tcpConnId_;
};
