/*========================================================
 *
 *   文件名称 ：Timer.cpp
 *   创建日期 ：2016.6.4
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#include "Timer.h"

using namespace mars;

std::atomic<uint64_t> Timer::created_count_(0);

Timer::Timer(const TimerCallback& cb, int64_t timeout, double interval, int counter = 1):
timer_callback_(cb),
timeout_(timeout),
interval_(interval),
counter_(counter),
sequence_(getCreatedCount())
{
	created_count_ ++;
}

Timer::~Timer() {

}

bool Timer::isValid() {
	if (counter_ > 0 || counter_ == -1) {
		timeout_ += static_cast<int64_t>(interval_ * 1000000);
		return true;
	} else {
		return false;
	}
}

bool Timer::operator >(const Timer& rhs) {
	return this->timeout_ > rhs.timeout_;
}

int Timer::getIndex() const {
	return index_;
}

void Timer::setIndex(int index) {
	index_ = index;
}

