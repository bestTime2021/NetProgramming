#include <poll.h>
#include <assert.h>
#include <stdio.h>
#include "Poll.h"
#include "Channel.h"

void Poll::poll(ChannelList *activechannelsp) {
	int nready = ::poll(&*pollfds_.begin(), pollfds_.size(), 500); //FIXME maxi
	if (nready > 0) { //
		fillActiveChannels(activechannelsp, nready);
	} else if (nready == 0) {
	
	} else { //nready < 0
	
	}
}

void Poll::fillActiveChannels(ChannelList* activechannelsp, int nready) {
	//according to pollfds_, filling activechannelsp
	for (struct pollfd &pd: pollfds_) {
		if (pd.fd < -1)
			continue;
		if (pd.revents > 0) {
			nready--;
			auto refer = channelMap_.find(pd.fd);
			Channel *channelp = refer->second;
			//FIXME channel need to set revent
			channelp->setRevents(pd.revents);
			activechannelsp->push_back(channelp);
		}
		if (nready <= 0)
			break;
	}
}

void Poll::updateActiveChannels(Channel* channelp) {//update pollfds[] and map
	if (channelp->index() < 0) {//insert to map and pollfds
		struct pollfd pd;
		pd.fd = channelp->fd();
		pd.events = channelp->events();
		pollfds_.push_back(pd);

		channelp->setIndex(pollfds_.size() - 1);
		channelMap_[channelp->fd()] = channelp;
	} else {//modify map and pollfds
		for(auto pd: pollfds_) {
			if (pd.fd != channelp->fd()) continue;
			pd.events = channelp->events();
		}
		channelMap_[channelp->fd()] = channelp;
	}
}

