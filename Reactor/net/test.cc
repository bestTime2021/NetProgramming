#include "net/Channel.h"
#include "net/Eventloop.h"
#include <pthread.h>
#include <iostream>

static Eventloop* g_loop;
void *func(void *arg) {
	g_loop->runAfter(1, [](){std::cout << "first of all\n";});
	g_loop->runAfter(3, [](){std::cout << "hello\n";});
}
int main() {
	pthread_t tid;
	Eventloop loop;
	g_loop = &loop;
	pthread_create(&tid, NULL, func, NULL);
	loop.loop();
}
