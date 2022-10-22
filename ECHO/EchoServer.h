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
	EchoServer(char* port): port_(port){}

	void start();

private:
	char* port_;
};


void EchoServer::start(){
	int serv_sock = Tcp_listen(NULL, port_, NULL);
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
