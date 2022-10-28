#include "net/Channel.h"
#include "net/Eventloop.h"
int main() {
	Eventloop loop;
	Channel channel(&loop, 0);
	channel.update();
	loop.loop();
}
