/*========================================================
 *
 *   文件名称 ：MinHeap.h
 *   创建日期 ：2016.6.4
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#ifndef __MIN_HEAP_H__
#define __MIN_HEAP_H__

#include "NonCopyable.h"
#include <stdlib.h>
#include <iostream>

namespace mars {

// old : #define GREATER(x, y)((x) > (y)) // ? type safe , I spend about 1 hours to solve the bug.
// solve the bug : #define GREATER(x, y) ((*x) > (*y)) // ((x)->greater(y)) or ((x)->timeout > (y)->timeout)
// the better way to solve the bug, the idea is from << effective c++ >> item 2 (Prefer consts, enums, and inlines to #defines).
template <class T>
inline bool GREATER(const T& x, const T& y) {
	return *x > *y;
}

template<class T>
class MinHeapInterface {
public:
	virtual ~MinHeapInterface()             = 0; // ? comment or not
	virtual bool operator > (const T& rhs)  = 0;
	virtual int getIndex() const            = 0;
	virtual void setIndex(int index)        = 0;
};

template<class T>
MinHeapInterface<T>::~MinHeapInterface() {}

template<class T>
class MinHeap : public NonCopyable {
public:
	MinHeap();
	explicit MinHeap(unsigned int size);
	~MinHeap();

	bool empty() const { return size_ == 0; }
	unsigned int size() const { return size_; }
	unsigned int capacity() const { return capacity_; }
	const T* top() const { return empty() ? nullptr : base_[0]; }
	T* pop();
	void push(T* element); // ? element already in base_;
	bool erase(T* element);
	void resize(unsigned int size);

	void print() const {
		for (int i = 0 ; i < size_; i ++ ) {
			std::cout << " Index " << i << " " << base_[i]->getIndex() << " " << base_[i]->timeout() << std::endl;
		}
	}

private:
	void shiftUp(unsigned int holeIndex, T* element);
	void shiftDown(unsigned int holeIndex, T* element);

	T** base_;
	unsigned int size_;
	unsigned int capacity_;

	static const unsigned int kInitSize = 4;
};

template<class T>
MinHeap<T>::MinHeap():
size_(0)
{
	base_ = static_cast<T**>(malloc(kInitSize * sizeof(T*))); // ? base_ == NULL
	capacity_ = kInitSize;
}

template<class T>
MinHeap<T>::MinHeap(unsigned int size):
size_(0)
{
	base_ = static_cast<T**>(malloc(size * sizeof(T*))); // ? base_ == NULL
	capacity_ = size;
}

template<class T>
MinHeap<T>::~MinHeap() {
	if (!empty()) {
		for (int i = 0; i < size_; i ++) {
			delete base_[i];
		}
	}
	if (base_ != NULL) free(base_);
}

template<class T>
T* MinHeap<T>::pop() {
	if (__builtin_expect(size_ == 0, 0)) {
		return nullptr;
	}
	T* top = base_[0];
	shiftDown(0u, base_[--size_]);
	return top;
}

template<class T>
void MinHeap<T>::push(T* element) {
	int index = element->getIndex();
	if (0 <= index && index < size_) {
		// element is already pushed in MinHeap.
		return ;
	}
	if (__builtin_expect(size_ >= capacity_, 0)) {
		unsigned int size = capacity_ << 1;
		resize(size);
	}
	shiftUp(size_, element);
	size_ ++;
}

template<class T>
bool MinHeap<T>::erase(T* element) {
	int index = element->getIndex();
	if (__builtin_expect(0 <= index && index < size_, 1)) {
		if (index != 0 && GREATER(base_[(index - 1) / 2], base_[index])) {
			shiftUp(index, base_[-- size_]);
		} else {
			shiftDown(index, base_[-- size_]);
		}
		return true;
	} else {
		// element is not in MinHeap.
		return false;
	}
}

template<class T>
void MinHeap<T>::resize(unsigned int size) {
	base_ = static_cast<T**>(realloc(base_, size * sizeof(T*)));
	if (base_ == NULL) {
		// realloc() failed.
		capacity_ = 0;
	} else {
		capacity_ = size;
	}
}

template<class T>
void MinHeap<T>::shiftUp(unsigned int holeIndex, T* element) {
	unsigned int parentIndex = (holeIndex - 1) / 2; // ? when holeIndex is zero, what will happen
	while (parentIndex < holeIndex && GREATER(base_[parentIndex], element)) {
		(base_[holeIndex] = base_[parentIndex])->setIndex(holeIndex);
		holeIndex = parentIndex;
		parentIndex = (holeIndex - 1) / 2;
	}
	(base_[holeIndex] = element)->setIndex(holeIndex);
}

template<class T>
void MinHeap<T>::shiftDown(unsigned int holeIndex, T* element) {
	unsigned int minIndex = holeIndex * 2 + 1;
	while (minIndex < size_) {
		minIndex += (minIndex + 1) < size_ && GREATER(base_[minIndex], base_[minIndex + 1]);
		if (GREATER(base_[minIndex], element)) {
			break;
		}
		(base_[holeIndex] = base_[minIndex])->setIndex(holeIndex);
		holeIndex = minIndex;
		minIndex = holeIndex * 2 + 1;
	}
	(base_[holeIndex] = element)->setIndex(holeIndex);
}

}

#endif

