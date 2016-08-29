/*========================================================
 *
 *   文件名称 ：ChannelPool.cpp
 *   创建日期 ：2016.5.18
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#include "ObjectPool.h"
#include <functional>

using namespace mars;

template<class T>
ObjectPool<T>::ObjectPool():
deque_()
{
}

template<class T>
ObjectPool<T>::~ObjectPool() {
	while (!deque_.empty()) {
		T* element = deque_.front();
		delete element;
		deque_.pop_front();
	}
}

template<class T>
std::shared_ptr<T> ObjectPool<T>::acquire() {
	if (deque_.empty()) {
		std::shared_ptr<T> ptr(new T(), [this] (T* ptr) {
			deque_.push_front(ptr);
		});
		return ptr;
	} else {
		T* element = deque_.front();
		std::shared_ptr<T> ptr(element, [this] (T* ptr) {
			deque_.push_front(ptr);
		});
		deque_.pop_front();
		return ptr;
	}
}
