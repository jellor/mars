/*========================================================
 *
 *   文件名称 ：EventLoop.h
 *   创建日期 ：2016.5.18
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#ifndef __EVENT_LOOP_H__
#define __EVENT_LOOP_H__

#include "Selector.h"
#include "Lock.h"
#include "Thread.h"
#include "MinHeap.h"
#include "TimerId.h"
#include "NonCopyable.h"
#include <functional>
#include <vector>
#include <memory>

namespace mars {

class Handler;
class Selector;

class EventLoop : public NonCopyable {

	typedef std::function<void(void)> Func;

public:
	EventLoop();
	~EventLoop();

	void start();
	void stop();

	bool isStarted() const { return started_; }
	bool isInSelfThread() const { return thread_id_ == Thread::getCurrentThreadId(); }
	void doFunc(const Func& func);
	TimerId addTimer(const TimerCallback& cb, int64_t timeout, double interval, int counter = 1);
	void removeTimer(const TimerId& timerId);

	void addHandler   (Handler* handler);
	void updateHandler(Handler* handler);
    void removeHandler(Handler* handler);

   	const void* getContext() const { return context_; }
   	void* getMutableContext() const { return context_; }
   	void setContext(void* context) { context_ = context; }
  
private:
	void pushTimer(Timer* timer);
	void eraseTimer(Timer* timer);
	void loop();
	void doTimeout(int64_t now);
	void doFuncs();
	void pushFunc(const Func& func);
	void wakeup();
	void consume();

	std::atomic<bool> started_;
	const pid_t thread_id_;
	void* context_;
	std::unique_ptr<Selector> selector_;
	std::vector<Func> funcs_;
	Mutex mutex_;
	std::atomic<bool> calling_funcs_;
	MinHeap<Timer> heap_;

	Handler* read_handler_;
	Handler* write_handler_;
};

}

#endif