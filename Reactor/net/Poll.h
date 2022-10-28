#include <map>
#include <vector>
#include <poll.h>

class Channel;
class Eventloop;

class Poll {
typedef std::vector<Channel*> ChannelList;
typedef std::map<int, Channel*> ChannelMap;
public:
	void poll(ChannelList *activechannelsp);

	void updateActiveChannels(Channel*);

private:
	void fillActiveChannels(ChannelList*, int nready);

	Eventloop *eventloop_;				//bind with a Eventloop
	ChannelMap channelMap_;				//find a fdToChannel
	std::vector<pollfd> pollfds_;	//push every not null pollfd
};
