/*========================================================
 *
 *   文件名称 ：Timer.h
 *   创建日期 ：2016.6.4
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#ifndef __TIMER_H__
#define __TIMER_H__

#include "MinHeap.h"
#include <functional>
#include <atomic>

namespace mars {

typedef std::function<void(void)> TimerCallback;

class Timer : public MinHeapInterface<Timer>, public NonCopyable {	
public:
	explicit Timer(const TimerCallback& cb, int64_t timeout, double interval, int counter);
	~Timer();

	void setTimerCallback(const TimerCallback& cb) { timer_callback_ = cb; }
	void run() { timer_callback_(); if (counter_ != -1 ) counter_ --; }
	bool isValid();
	int64_t timeout() const { return timeout_; }

	// MinHeapInterface implement
	bool operator >(const Timer& rhs) override;
	int  getIndex() const override;
	void setIndex(int index) override;
	const uint64_t getSequence() const { return sequence_; }

	static uint64_t getCreatedCount() { return created_count_.load(); }

private:
	TimerCallback timer_callback_; 
	int64_t timeout_;
	const double interval_;
	int counter_; // -1 => forever, 0 => remove immediately, x > 0 => will trigger x timeout. 
	int index_;
	const uint64_t sequence_;

	static std::atomic<uint64_t> created_count_;
};

}

#endif