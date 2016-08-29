/*========================================================
 *
 *   文件名称 ：RingBuffer.h
 *   创建日期 ：2016.6.1
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#ifndef __RING_BUFFER_H__
#define __RING_BUFFER_H__

#include "NonCopyable.h"
#include <string.h>
#include <string>

namespace mars {

class RingBuffer : public NonCopyable {
public:
	RingBuffer();
	RingBuffer(unsigned int size);
	~RingBuffer();

	int read(int sockfd);
	int write(int sockfd);
	int read(char* data, unsigned int size);
	int write(const char* data, unsigned int size);
	int write(const std::string& str);
	bool empty() const { return size_ == 0; }
	const char* base() const { return buffer_; }
	const char* head() const { return buffer_ + head_index; }
	const char* tail() const { return buffer_ + tail_index; }
	unsigned int headIndex() const { return head_index; }
	unsigned int tailIndex() const { return tail_index; }
	unsigned int size() const { return size_; }
	unsigned int capacity() const { return capacity_; }
	unsigned int unused() const { return capacity_ - size_; }
	void resize(unsigned int size);
	void extend(unsigned int size);

	bool backUpFromHead(unsigned int count);
	bool skipFromHead(unsigned int count);
	bool backUpFromTail(unsigned int count);
	bool skipFromTail(unsigned int count);

	void adjust();
	void clear();

private:
	unsigned int head_index; // ? signed int head_index
	unsigned int tail_index;
	unsigned int size_;
	unsigned int capacity_;
	char* buffer_;

	static const unsigned int kInitSize = 256;
};

}

#endif
