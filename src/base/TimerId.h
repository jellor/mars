/*========================================================
 *
 *   文件名称 ：TimerId.h
 *   创建日期 ：2016.6.4
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#ifndef __TIMER_ID_H__
#define __TIMER_ID_H__

#include "Timer.h"

namespace mars {

class TimerId {
public:
	TimerId(Timer* timer, int64_t sequence): timer_(timer), sequence_(sequence) {}
	Timer* getTimer() const { return timer_; }

private:
	Timer* const timer_;
	const uint64_t sequence_;
};

}

#endif