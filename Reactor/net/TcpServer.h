#include <memory>
#include <vector>
#include <functional>

class Eventloop;
class Acceptor;

class TcpServer 
{
public:
	TcpServer(Eventloop *loop, char *hostname, char *service);
	~TcpServer();

	void start();
	void onConn();

	///used by user, he/she can set Action for her self.
	void setAcceptCallback(std::function<void()> aCb)
	{	acceptCallback_ = aCb; }
	void setConnCallback(std::function<void()> cCb)
	{ connCallback_ = cCb; }

private:
	Eventloop* eventloop_;
	//std::map<string, TcpConnection*> connMap_;
	std::shared_ptr<Acceptor> acceptor_;
	std::function<void()> acceptCallback_;
	std::function<void()> connCallback_;
};
