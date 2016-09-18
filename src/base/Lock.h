/*========================================================
 *
 *   文件名称 ：Lock.h
 *   创建日期 ：2016.5.29
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#ifndef __LOCK_H__
#define __LOCK_H__

#include "NonCopyable.h"
#include "Exception.h"
#include "Mutex.h"
#include <pthread.h>

namespace mars {

class Lock : public NonCopyable {
public:
	explicit Lock(Mutex& mutex): mutex_(mutex) { mutex_.lock(); }
	~Lock() { mutex_.unlock(); }

private:
	Mutex& mutex_;
};

}

#endif
