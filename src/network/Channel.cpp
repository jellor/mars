/*========================================================
 *
 *   文件名称 ：Channel.cpp
 *   创建日期 ：2016.6.2
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#include "Channel.h"
#include "Exception.h"
#include "Log.h"
#include <exception>

using namespace mars;

Channel::Channel(EventLoop* event_loop, int sockfd, const IpAddress& local_address, const IpAddress& peer_address):
event_loop_(event_loop),
socket_(sockfd),
local_address_(local_address),
peer_address_(peer_address),
handler_(event_loop, socket_.fd()),
out_buffer_(),
in_buffer_(),
opened_(true),
connect_callback_(nullptr),
read_callback_(nullptr),
write_callback_(nullptr),
close_callback_(nullptr),
error_callback_(nullptr)
{
	socket_.setNoDelay(true);
	handler_.setReadCallback(std::bind(&Channel::handleReadEvent, this));
	handler_.setWriteCallback(std::bind(&Channel::handleWriteEvent, this));
	handler_.enableRead();
}

Channel::~Channel() {
	DEBUG << "Channel Destructor ... ";
}

void Channel::reset() {
	event_loop_ = nullptr;
	socket_  = Socket(-1);
	handler_.reset();
	out_buffer_.clear();
	in_buffer_.clear();
	opened_ 		  = false;
	connect_callback_ = nullptr;
	read_callback_    = nullptr;
	close_callback_   = nullptr;
	error_callback_   = nullptr;
}

void Channel::close() {
	opened_ = false;
}

void Channel::send(const RingBuffer* buffer) {
	// event_loop_->doFunc();
	if (!opened_) {
		WARN << "Channel Is Not Opened";
		return ;
	}
	DEBUG << "Size " << buffer->size() << " Capacity " << buffer->capacity();
	DEBUG << "Head " << buffer->head() << " Base " << buffer->base();
 	send(buffer->head(), buffer->size());
}

void Channel::send(const char* data, unsigned int size) {
	if (!opened_) {
		WARN << "Channel Is Not Opened";
		return ;
	}
	event_loop_->doFunc(std::bind(&Channel::runInEventLoop, this, data, size));
}

int Channel::read(char* data, unsigned int size) {
	if (!opened_) {
		WARN << "Channel Is Not Opened";
		return 0;
	}
	int ret = in_buffer_.read(data, size);
	return ret;
}

int Channel::write(const char* data, unsigned int size) {
	// maybe not in EventLoop.
	int ret = out_buffer_.write(data, size);
	handler_.enableWrite();
	return ret;
}

void Channel::runInEventLoop(const char* data, unsigned int size) {

	DEBUG << "Handler Writable " << handler_.isWritable();
	DEBUG << "Handler Status " << handler_.getStatus();
	DEBUG << "out_buffer size " << out_buffer_.size() << " Capacity " << out_buffer_.capacity();

	if (out_buffer_.empty()) {
		int written_size = ::write(socket_.fd(), data, size);

		if (written_size < 0) {
			written_size = 0; 
			WARN << "Syscall ::write() Occurs Errno => " << errno;
			if (errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN) {

			}
		}

		if (written_size < size) {
			out_buffer_.write(data + written_size, size - written_size);
			handler_.enableWrite();
		}
		
	} else {
		DEBUG << "out_buffer size " << out_buffer_.size() << " Capacity " << out_buffer_.capacity();
		out_buffer_.write(data, size);
		handler_.enableWrite();
	}


	// int written_size = 0;

	// if (out_buffer_.empty()) {
	// 	written_size = ::write(socket_.fd(), data, size);

	// 	if (written_size < 0) {
	// 		written_size = 0;
	// 		WARN << "Syscall ::write() Occurs Errno => " << errno;
	// 		if (errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN) {

	// 		}
	// 	} 
	// }

	// if (written_size < size) {
	// 	out_buffer_.write(data + written_size, size - written_size);
	// 	handler_.enableWrite();
	// }
	

	// out_buffer_.write(data, size);

	// DEBUG << "handler writable " << handler_.isWritable();
	// DEBUG << "handler status " << handler_.getStatus();

	// // handler_.enableWrite();

	// DEBUG << "out_buffer size " << out_buffer_.size() << " Capacity " << out_buffer_.capacity();
	// out_buffer_.write(socket_.fd());
	// if (!out_buffer_.empty()) {
 //        DEBUG << "out_buffer size " << out_buffer_.size() << " Capacity " << out_buffer_.capacity();
 //        handler_.enableWrite();
	// }
	// DEBUG << "Handler Writable " << handler_.isWritable();
	// DEBUG << "Handler Status " << handler_.getStatus();

}

void Channel::handleReadEvent() {
	int size = in_buffer_.read(socket_.fd());
	if (size <= 0) {
		// ? what happen
		int error;
		int ret = Socket::getError(socket_.fd(), &error);
		DEBUG << "Ret " << ret << " error " << error;
		DEBUG << "Receive Size => " << size;
		if (close_callback_ != nullptr) close_callback_(shared_from_this());
	}
	DEBUG << "in_buffer_ Size " << in_buffer_.size() << " Capacity " << in_buffer_.capacity();
	if (read_callback_ != nullptr) {
		read_callback_(shared_from_this());
	}
}

void Channel::handleWriteEvent() {
	if (write_callback_ != nullptr) {
		write_callback_(shared_from_this());
	}

	DEBUG << "Buffer Size " << out_buffer_.size() << " Capacity " << out_buffer_.capacity();

	out_buffer_.write(socket_.fd());

	if (out_buffer_.empty()) {
		DEBUG << "out_buffer_ size " << out_buffer_.size() << " Capacity " << out_buffer_.capacity();
		handler_.disableWrite();
	}

}

