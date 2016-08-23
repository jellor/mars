#include "Log.h"
#include "Channel.h"
#include "EventLoop.h"
#include "IpAddress.h"
#include "Timestamp.h"
#include "ChannelPool.h"
#include <string.h>
#include <iostream>
#include <set>

using namespace std;
using namespace mars;

int main() {
	set<ChannelPtr> s;
	EventLoop event_loop;
	IpAddress ip_address_a("127.0.0.1", 80);
	IpAddress ip_address_b("127.0.0.1", 8090);
	int len;
	cin >> len;

	ChannelPool* pool = new ChannelPool(&event_loop, len);
	
	int64_t begin = Timestamp::now().macrosecond();

	for (int i = 0; i < len; i ++) {
		ChannelPtr channel(new Channel(&event_loop, -1, ip_address_a, ip_address_b));
		s.insert(channel);
	}

	int64_t aa = Timestamp::now().macrosecond() - begin;

	DEBUG << Timestamp::now().macrosecond() - begin;
	DEBUG << Timestamp(Timestamp::now().macrosecond() - begin).toString();
	DEBUG << Timestamp(aa).toString();

	int64_t begin_a = Timestamp::now().macrosecond();

	//ChannelPool* pool = new ChannelPool(&event_loop, len);
	for (int i = 0; i < len; i ++) {
		ChannelPtr channel = pool->acquire(&event_loop, -1, ip_address_a, ip_address_b);
	}
	
	int64_t bb = Timestamp::now().macrosecond() - begin_a;

	DEBUG << Timestamp::now().macrosecond() - begin_a;
	DEBUG << Timestamp(Timestamp::now().macrosecond() - begin_a).toString();
	DEBUG << Timestamp(bb).toString();

	DEBUG << "========";
	DEBUG << aa;
	DEBUG << bb;
	event_loop.start();
	
	return 0;
}