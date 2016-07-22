/*========================================================
 *
 *   文件名称 ：Mutex.h
 *   创建日期 ：2016.5.29
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#ifndef __MUTEX_H__
#define __MUTEX_H__

#include "NonCopyable.h"
#include "Exception.h"
#include <pthread.h>

namespace mars {

class Mutex : public NonCopyable {
public:
	Mutex() {
		pthread_mutexattr_t mutexattr;
		pthread_mutexattr_init(&mutexattr);
		pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_RECURSIVE);
		if (pthread_mutex_init(&mutex_, &mutexattr)) {
			pthread_mutexattr_destroy(&mutexattr);
			throw Exception("Cannot Create Mutex");
		}
		pthread_mutexattr_destroy(&mutexattr);
	}
	~Mutex() { pthread_mutex_destroy(&mutex_); }

	inline void lock();
	inline void unlock();

	pthread_mutex_t& getMutex() { return mutex_; }

private:
	pthread_mutex_t mutex_;
};

inline void Mutex::lock() {
	if (0 != pthread_mutex_lock(&mutex_)) {
		throw Exception("Cannot Lock Mutex");
	}
}

inline void Mutex::unlock() {
	if (0 != pthread_mutex_unlock(&mutex_)) {
		throw Exception("Cannot Unlock Mutex");
	}
}

}

#endif

