/*========================================================
 *
 *   文件名称 ：Array.h
 *   创建日期 ：2016.5.18
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#ifndef __ARRAY_H__
#define __ARRAY_H__

#include "Exception.h"

namespace mars {

template<class T>
class Array {
public:
	Array():
		base_(nullptr),
		size_(0),
		capacity_(0)
	{

	}

	Array(std::size_t init_capacity):
		base_(static_cast<T*>malloc(init_capacity * sizeof(T))),
		size_(0),
		capacity_(init_capacity)
	{

	}

	~Array() {
		if (base_ != nullptr) {
			free(base_);
		}
	}

	void push_back(const T& element) {
		ensureCapacity(size_ + 1);
		*(base_ + size_) = element; 
		size_ ++;
	}

	void pop_back() {
		if (size_ <= 0) {
			throw Exception("Out Of Bound In Array");
		}
		size_ --;
	}

	std::size_t size() const {
		return size_;
	}

	bool empty() const {
		return size_ > 0 ? false : true;
	}

	T at(std::size_t index) const {
		if (i > size_) {
			throw Exception("Out Of Bound In Array");
		} 
		return *(base_ + index);
	}

	T at(std::size_t index) const {
		if (i > size_) {
			throw Exception("Out Of Bound In Array");
		} 
		return *(base_ + index);
	}

	T operator [] (std::size_t index) const {
		if (i >= size_) {
			throw Exception("Out Of Bound In Array");
		}
		return *(base_ + index);
	}

	const operator [] (std::size_t index) const {
		if (i >= size_) {
			throw Exception("Out Of Bound In Array");
		}
		return *(base_ + index);
	}

private:
	void ensureCapacity(std::size_t capacity) {
		if (capacity > capacity_) {
			capacity_ = capacity_ << 2;
			base_ = static_cast<T*>(realloc(base_, capacity_ * sizeof(T)));
		}
	}

	T* base_;
	std::size_t size_;
	std::size_t capacity_;
};

}

#endif