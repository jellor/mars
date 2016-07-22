/*========================================================
 *
 *   文件名称 ：Condition.cpp
 *   创建日期 ：2016.6.1
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#include "Condition.h"
#include <errno.h>
#include <sys/time.h>

using namespace mars;

Condition::Condition(Mutex& mutex):
mutex_(mutex)
{
	pthread_cond_init(&condition_, NULL);
}

Condition::~Condition() {
	pthread_cond_destroy(&condition_);
}

void Condition::wait() {
	pthread_cond_wait(&condition_, &mutex_.getMutex());
}

bool Condition::waitForSeconds(int seconds) {
	struct timespec ts;
	struct timeval  tv;
	gettimeofday(&tv, NULL);
	ts.tv_sec  = tv.tv_sec  + seconds;
	ts.tv_nsec = tv.tv_usec * 1000;
	return pthread_cond_timedwait(&condition_, &mutex_.getMutex(), &ts) == ETIMEDOUT ? false : true;
}

void Condition::signal() {
	pthread_cond_signal(&condition_);
}

void Condition::broadcast() {
	pthread_cond_broadcast(&condition_);
}


