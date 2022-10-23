#include <iostream>
#include <stdint.h>
#include <cstring>
#include <unistd.h>
#include <functional>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "../Wrapper/wrapper.h"
#include <errno.h>
#include <vector>
using namespace std;
class EchoServer
{
public:
	EchoServer(int32_t port): port_(port){}

	void setCallback(function<string ()> c){
		callback_ = c;
	}

	void start();

private:
	int16_t port_;
	function<string ()> callback_;
	int listening();
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
void EchoServer::start()
{
	///select_return : nready
	///select_fri_arg: maxfd + 1
	///client_maxindex: maxi
	int maxi, maxfd, listenfd, connfd;
	vector<int> client(FD_SETSIZE, -1);
	int nready;
	fd_set rset, allset;
	char buf[1024];
	socklen_t clilen;
	struct sockaddr_in cliaddr;

	listenfd = listening();
	maxfd = listenfd;
	maxi = -1;
	FD_ZERO(&allset);
	FD_SET(listenfd, &allset);

	while (1){
		rset = allset;
		nready = select(maxfd+1, &rset, NULL, NULL, NULL);

		if (FD_ISSET(listenfd, &rset)){
			clilen = sizeof(cliaddr);
			connfd = Accept(listenfd, (sockaddr *)&cliaddr, &clilen);
			printf("NEW CONN\n");
			write(connfd, "You have connected with SELECTSERVER\n", 37);
			int i;
			for (i = 0; i < FD_SETSIZE; ++i){
				if (client[i] < 0){
					client[i] = connfd;
					break;
				}
			}
			if (i == FD_SETSIZE){
				printf("too many connects\n");
				exit(0);
			}

			FD_SET(connfd, &allset);
			if (connfd > maxfd)
				maxfd = connfd;
			if (i > maxi)
				maxi = i;

			if (--nready <= 0)
				continue;
		}
		
		for (int i = 0; i <= maxi; i++) {
			int sockfd, readlen;
			if ( (sockfd = client[i]) < 0)
				continue;
			if (FD_ISSET(sockfd, &rset)) {
				if ( (readlen = read(sockfd, buf, 1024)) == 0){ //FIN
					Close(sockfd);
					printf("CLOSE CONN\n\n");
					FD_CLR(sockfd, &allset);	
					client[i] = -1;
				}
				*(buf+readlen) = 0;
				printf("RECIEVE FROM CLIENT: %s", buf);
				write(sockfd, buf, readlen);
				if (--nready <= 0)
					break;
			}
		}
	}
}
