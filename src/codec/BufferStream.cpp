/*========================================================
 *
 *   文件名称 ：BufferStream.cpp
 *   创建日期 ：2016.8.9
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *   描 述 ：
 *
 *=======================================================*/

#include "BufferStream.h"
#include "Log.h"

using namespace mars;

BufferInputStream::BufferInputStream(RingBuffer* buffer):
buffer_(buffer),
size_(buffer_->size()),
position_(buffer_->headIndex()),
last_returned_size_(0),
readed_count_(0)
{

}

BufferInputStream::~BufferInputStream() {

}

bool BufferInputStream::Next(const void** data, int* size) {
	if (buffer_->empty()) return false;
	if (readed_count_ < size_) {
		if (buffer_->headIndex() > buffer_->tailIndex()) {
			*data = (buffer_->base() + position_);
			last_returned_size_ = size_ - readed_count_;
		} else {
			if (position_ < buffer_->capacity()) {
				*data = (buffer_->base() + position_);
				last_returned_size_ = buffer_->capacity() - position_;
			} else {
				*data = (buffer_->base() + position_);
				last_returned_size_ = buffer_->tailIndex();
			}
		}
		*size = last_returned_size_;
		position_ = (position_ + last_returned_size_) % buffer_->capacity();
		readed_count_ += last_returned_size_;
		return true;
	} else {
		// we're at the end of the buffer.
		last_returned_size_ = 0; // Don't let caller back up.
		return false;
	}
}

void BufferInputStream::BackUp(int count) {
	position_ = (position_ + buffer_->capacity() - count) % buffer_->capacity();
	last_returned_size_ = 0; // Dont' let caller back up further.
	readed_count_ -= count;
}

bool BufferInputStream::Skip(int count) {
	last_returned_size_ = 0; // Don't let caller back up.
	if (count > size_ - readed_count_) {
		position_ = buffer_->tailIndex();
		readed_count_ = size_;
		return false;
	} else {
		position_ = (position_ + count) % buffer_->capacity();
		readed_count_ += count;
		return true;
	}
}

int64_t BufferInputStream::ByteCount() const {
	return readed_count_;
}

BufferOutputStream::BufferOutputStream(RingBuffer* buffer):
buffer_(buffer),
last_returned_size_(0),
written_count_(0)
{

}

BufferOutputStream::~BufferOutputStream() {

}

bool BufferOutputStream::Next(void** data, int* size) {
	if (buffer_->unused() == 0) {
		buffer_->extend(1024);
	}
	if (buffer_->headIndex() <= buffer_->tailIndex()) {
		*data = const_cast<char*>(buffer_->tail());
		last_returned_size_ = buffer_->capacity() - buffer_->tailIndex();
	} else {
		*data = const_cast<char*>(buffer_->tail());
		last_returned_size_ = buffer_->headIndex() - buffer_->tailIndex();
	}
	*size = last_returned_size_;
	written_count_ += last_returned_size_;
	buffer_->skipFromTail(last_returned_size_);
	return true;
}

void BufferOutputStream::BackUp(int count) {
	buffer_->backUpFromTail(count);
	written_count_ -= count;
}

int64_t BufferOutputStream::ByteCount() const {
	return written_count_;
}






















