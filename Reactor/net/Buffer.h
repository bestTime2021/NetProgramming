#include <vector>

class Buffer {
public:
	Buffer();

	int readFd(int fd);		//readv() from connfd.
	int readableBytes();	//get the number of readable data; [readIndex, writeIndex);
private:
	std::vector<char> data;
	size_t readIndex;
	size_t writeIndex;
};
