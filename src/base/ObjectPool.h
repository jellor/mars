/*========================================================
 *
 *   文件名称 ：ObjectPool.h
 *   创建日期 ：2016.5.18
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#ifndef __OBJECT_POOL_H__
#define __OBJECT_POOL_H__


#include <deque>
#include <memory>

namespace mars {

template<class T>
class ObjectPool{
public:
	ObjectPool();
	~ObjectPool();

	std::shared_ptr<T> acquire();

	int size()   const { return deque_.size();  }
	bool empty() const { return deque_.empty(); }

private:
	std::deque<T*> deque_;
};

}

#endif