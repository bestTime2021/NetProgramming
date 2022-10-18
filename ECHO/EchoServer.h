#include <stdint.h>
#include <cstring>
#include <unistd.h>
#include <functional>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "../Wrapper/wrapper.h"
#include <errno.h>

class EchoServer
{
public:
	EchoServer(int32_t port): port_(port){}

	void start();

private:
	int16_t port_;
	int listening();
	//function<string ()> callback_;
};

int EchoServer::listening(){
	int serv_sock = Socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); //htonl is not essential, but please add it.
	serv_addr.sin_port = htons(port_);
	
	int option = 1;
	socklen_t optlen = sizeof(option);
	setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (void*)&option, optlen);
	
	Bind (serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
	Listen (serv_sock, 5);

	return serv_sock;
}
void EchoServer::start(){
	int serv_sock = listening();
	
	char ret[1024];
	while (1){
		struct sockaddr_in clnt_addr;
		socklen_t addrlen = sizeof (clnt_addr);
		int len = 0;
		int clnt_sock = Accept(serv_sock, (struct sockaddr*)&clnt_addr, &addrlen);
		printf("New CONN\n");
		write(clnt_sock, "FROM Server: You have connected!\n", 34);

		while ( (len = read(clnt_sock, ret, 1024)) > 0){
			*(ret+len) = 0;
			write (clnt_sock, ret, len);//[0, len]
			printf("Recieve from Client: %s", ret);	
		}
		if (len == 0)
			Close(clnt_sock), printf("Client CLOSE\n\n");
		if (len < 0)
			unix_error("read()");
	}
}
