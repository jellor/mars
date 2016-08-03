#include <iostream>
#include "Timer.h"
#include "Log.h"
#include "EventLoop.h"
#include "Thread.h"
#include "Timestamp.h"

using namespace std;
using namespace mars;

int main() {
	EventLoop event_loop;
	DEBUG << "lock";
	TimerId timerId = event_loop.addTimer([] {
		DEBUG << "Hello World";
	},Timestamp::now().macrosecond(), 5, 10);
	Thread thread([=, &event_loop] {
		Thread::sleep(12);
		DEBUG << Thread::getCurrentThreadName() << " id " << Thread::getCurrentThreadId();
		event_loop.removeTimer(timerId);
	}, "func");
	thread.start();
	
	event_loop.start();
	thread.join();
	return 0;
}