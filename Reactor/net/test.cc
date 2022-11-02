#include "net/Channel.h"
#include "net/Eventloop.h"
#include <iostream>
void hello() {
	std::cout << "Hello world\n";
}
void hello2() {
	std::cout << "Hello world2\n";
}
int main() {
	Eventloop loop;
//	Channel channel(&loop, 0);
//	channel.update();
	int interval = 3;
	for (int i = 0; i < 5; i++, interval += 3)
	loop.runAfter(interval, hello);
	loop.loop();
}
