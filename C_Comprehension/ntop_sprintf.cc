/*
 * the type of x is "int", which occupies 4 Bytes
 * so when &x is transfer for char* p
 * every element of p will occupy 1 Byte, and accessing it is equal to accessingevery 8-bits x
 */

#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#define MAXLEN 1024

int main(int argc, char *argv[]){
	assert(argc == 2);
	
	int x = atoi(argv[1]);
	u_char *p = (u_char *)&x;
	char str[MAXLEN];
	sprintf(str, "%d.%d.%d.%d", p[0], p[1],p[2],p[3]);
	printf("%s\n", str);
}
