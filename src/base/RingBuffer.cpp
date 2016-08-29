/*========================================================
 *
 *   文件名称 ：RingBuffer.cpp
 *   创建日期 ：2016.6.1
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *   描 述 ：
 *
 *=======================================================*/

#include "RingBuffer.h"
#include "Log.h"
#include <stdlib.h>
#include <string>
#include <sys/uio.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>

using namespace mars;

RingBuffer::RingBuffer():
head_index(0),
tail_index(0),
size_(0)
{
	buffer_ = static_cast<char*>(malloc(kInitSize));
	if (buffer_ == NULL) {

	}
	capacity_ = kInitSize;
}

RingBuffer::RingBuffer(unsigned int size):
head_index(0),
tail_index(0),
size_(0)
{
	buffer_ = static_cast<char*>(malloc(size));
	if (buffer_ == NULL) {

	}
	capacity_ = size;
}

RingBuffer::~RingBuffer() {
	DEBUG << "RingBuffer Destructor ...";
	if (buffer_ != NULL) {
		// DEBUG << "Free buffer_";
		free(buffer_);
	}
}

int RingBuffer::read(int sockfd) { // ? can loop read()
	if (unused() == 0) resize(0);
	int size;
	if (head_index <= tail_index) {
		struct iovec iv[2];
		iv[0].iov_base = buffer_ + tail_index;
		iv[0].iov_len  = capacity_ - tail_index; // ? tail_index = 0
		iv[1].iov_base = buffer_;
		iv[1].iov_len  = head_index;
		size = ::readv(sockfd, iv, 2);
	} else {
		size = ::read(sockfd, buffer_ + tail_index, head_index - tail_index);
	}
	tail_index = (tail_index + size) % capacity_;
	size_      = size_ + size;
	return size;
}

int RingBuffer::write(int sockfd) { // ? can loop write()
	if (size_ == 0) return 0;
	int size;
	if (head_index >= tail_index) {
		struct iovec iv[2];
		iv[0].iov_base = buffer_ + head_index;
		iv[0].iov_len  = capacity_ - head_index;
		iv[1].iov_base = buffer_;
		iv[1].iov_len  = tail_index;
		size = ::writev(sockfd, iv, 2);
	} else {
		size = ::write(sockfd, buffer_ + head_index, tail_index - head_index);
	}
	if (size < 0) {
		size = 0;
		DEBUG << "--------- IMPORTANT ERROR ----------";
		DEBUG << "ERRNO => " << errno;
		DEBUG << "--------- IMPORTANT ERROR ----------";
	}
	head_index = (head_index + size) % capacity_;
	size_      = size_ - size;
	return size;
}

int RingBuffer::read(char* data, unsigned int size) {
	if (size_ == 0) return 0;
	unsigned int resultSize = size > this->size_ ? this->size_ : size;
	if (head_index >= tail_index) {
		unsigned int len = capacity_ - head_index;
		memcpy(data, buffer_ + head_index, len);
		memcpy(data + len, buffer_, resultSize - len);
	} else {
		memcpy(data, buffer_ + head_index, resultSize);
	}
	head_index = (head_index + resultSize) % capacity_; // ?
	size_ 	   = size_ - resultSize;
	return resultSize;
}

int RingBuffer::write(const char* data, unsigned int size) {
	if (unused() < size) {
		resize(capacity_ + size);
	}
	assert(unused() >= size);

	unsigned int len = capacity_ - tail_index;
	if (size > len) {
		memcpy(buffer_ + tail_index, data, len);
		memcpy(buffer_, data + len, size - len);
	} else {
		memcpy(buffer_ + tail_index, data, size);
	}
	tail_index = (tail_index + size) % capacity_;
	size_      = size_ + size;
	return size;
}

int RingBuffer::write(const std::string& str) {
    return write(str.c_str(), str.size());
}

void RingBuffer::resize(unsigned int size) {
	int tmpSize = capacity_ << 1; // ? tempSize != capacity_
	int newSize = tmpSize > size ? tmpSize : size;
	buffer_ = static_cast<char*>(realloc(buffer_, newSize));
	if (buffer_ == NULL) {

	} else {
		if (head_index  == tail_index && size_ == 0) {
			head_index = 0;
			tail_index = 0;
		} else if (head_index >= tail_index) {
			if (tail_index != 0) { // ? can delete the statement.
				memcpy(buffer_ + capacity_, buffer_, tail_index);
			}
			tail_index += capacity_;
		}
		capacity_ = newSize;
	}
}

void RingBuffer::extend(unsigned int size) {
	int newSize = capacity_ + size;
	buffer_ = static_cast<char*>(realloc(buffer_, newSize));
	if (buffer_ == NULL) {

	} else {
		if (head_index == tail_index && size_ == 0) {
			head_index = 0;
			tail_index = 0;
		} else if (head_index >= tail_index) {
			if (tail_index != 0) { // ? can delete the statement.
				memcpy(buffer_ + capacity_, buffer_, tail_index);
			}
			tail_index += capacity_;
		}
		capacity_ = newSize;
	}
}

bool RingBuffer::backUpFromHead(unsigned int count) {
	if (size_ + count > capacity_) return false;
	head_index = (head_index + capacity_ - count) % capacity_;
	size_	   = size_ + count;
}

bool RingBuffer::skipFromHead(unsigned int count) {
	if (size_ < count) return false;
	head_index = (head_index + count) % capacity_;
	size_      = size_ - count;
}

bool RingBuffer::backUpFromTail(unsigned int count) {
	if (size_ < count) return false;
	tail_index = (tail_index + capacity_ - count) % capacity_;
	size_      = size_ - count;
	return true;
}

bool RingBuffer::skipFromTail(unsigned int count) {
	if (size_ + count > capacity_) return false;
	tail_index = (tail_index + count) % capacity_;
	size_      = size_ + count;
}

void RingBuffer::adjust() {
	if (head_index >= tail_index && size_ != 0) {
		char* tmp = static_cast<char*>(malloc(tail_index));
		memcpy(tmp, buffer_, tail_index);
		memcpy(buffer_, buffer_ + head_index, capacity_ - head_index);
		memcpy(buffer_ + (capacity_ - head_index), tmp, tail_index);
		head_index = 0;
		tail_index = size_;
	}
	assert(head_index <= tail_index);
}

void RingBuffer::clear() {
	head_index = 0;
	tail_index = 0;
	size_	   = 0;
}




