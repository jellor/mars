/*========================================================
 *
 *   文件名称 ：Condition.h
 *   创建日期 ：2016.6.1
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#ifndef __CONDITION_H__
#define __CONDITION_H__

#include "Mutex.h"

namespace mars {

class Condition {
public:
	Condition(Mutex& mutex);
	~Condition();

	void wait();
	bool waitForSeconds(int seconds);
	void signal();
	void broadcast();

private:
	Mutex& mutex_;
	pthread_cond_t condition_;
};

}

#endif