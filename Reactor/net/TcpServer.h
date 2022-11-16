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
	TcpServer(Eventloop *loop, char *hostname, char *service);
	~TcpServer();

	void start();
	void handNewConn(int connfd);

	///used by user, he/she can set Action for her self.
	void setAcceptCallback(std::function<void()> aCb)
	{	acceptCallback_ = aCb; }
	void setMessageCallback(std::function<void(TcpConnection*, char *, ssize_t)> mcb)
	{ mcb_ = mcb; }

private:
	Eventloop* eventloop_;
	std::map<std::string, TcpConnection*> connMap_;
	std::shared_ptr<Acceptor> acceptor_;
	std::function<void()> acceptCallback_;
	std::function<void(TcpConnection*, char*, ssize_t)> mcb_;
	int tcpConnId_;
};
