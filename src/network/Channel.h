/*========================================================
 *
 *   文件名称 ：Channel.h
 *   创建日期 ：2016.6.2
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#ifndef __CHANNEL_HANDLER_H__
#define __CHANNEL_HANDLER_H__

#include "RingBuffer.h"
#include "IpAddress.h"
#include "EventLoop.h"
#include "Handler.h"
#include "Socket.h"
#include <memory>
#include <string>
#include <assert.h>

namespace mars {

class HandlerChain;

class Channel : public std::enable_shared_from_this<Channel> {

	typedef std::function<void(const std::shared_ptr<Channel>&)> EventCallback;

public:
	Channel();
	Channel(EventLoop* event_loop, int sockfd, const IpAddress& local_address, const IpAddress& peer_address);
	~Channel();

	void setConnectCallback(const EventCallback& cb) { connect_callback_ = cb; }
	void setReadCallback(const EventCallback& cb) 	 { read_callback_    = cb; }
	void setWriteCallback(const EventCallback& cb)   { write_callback_ = cb;   }
	void setCloseCallback(const EventCallback& cb)   { close_callback_ = cb;   }
	void setErrorCallback(const EventCallback& cb)   { error_callback_ = cb;   }

	bool isOpen() const { return opened_; }
	void reset();
	void reset(EventLoop* event_loop, int sockfd, const IpAddress& local_address, const IpAddress& peer_address);

	void enableRead() {
		assert(opened_ == true);
		handler_.enableRead();
	}

	void enableWrite() {
		assert(opened_ == true);
		handler_.enableWrite();
	}

	void disableRead() {
		assert(opened_ == true);
		handler_.disableRead();
	}

	void disableWrite() {
		assert(opened_ == true);
		handler_.disableWrite();
	}

	void disableBoth() {
		assert(opened_ == true);
		handler_.disableRead();
		handler_.disableWrite();
	}

	void shutdownReceive();
	void shutdownSend();
	void close();

	void metadata();
	std::string toString() const;

	void send(const RingBuffer* buffer); // TODO
	void send(const char* data, unsigned int size);

	EventLoop* getEventLoop() const { return event_loop_; }
	Socket*  getSocket() 	  { return &socket_; 	}
	Handler* getHandler()  	  { return &handler_;   }

	const IpAddress& getLocalAddress()  const { return local_address_; }
	const IpAddress& getPeerAddress()   const { return peer_address_;  }

	RingBuffer* getMutableOutBuffer() const { return const_cast<RingBuffer*>(&out_buffer_); }
	RingBuffer* getMutableInBuffer()  const { return const_cast<RingBuffer*>(&in_buffer_);  }
	const RingBuffer* getOutBuffer() const { return &out_buffer_; }
	const RingBuffer* getInBuffer()  const { return &in_buffer_;  }

	void setEventLoop(EventLoop* event_loop) {
		event_loop_ = event_loop;
		handler_.setEventLoop(event_loop);
	}

	void setSocket(int sockfd) {
		socket_.reset(sockfd);
	}

	void setLocalAddress(const IpAddress& local_address) {
		local_address_ = local_address;
	}

	void setPeerAddress(const IpAddress& peer_address) {
		peer_address_ = peer_address;
	}



	void setHandlerChain(HandlerChain* handler_chain) {
		handler_chain_ = handler_chain;
	}

	HandlerChain* getChain() const {
		return handler_chain_;
	}

private:
	void shutdownReceiveInEventLoop();
	void shutdownSendInEventLoop();
	void closeInEventLoop();
	int read(char* data, unsigned int size);
	int write(const char* data, unsigned int size);
	void sendBufferInEventLoop(const RingBuffer* buffer);
	void sendInEventLoop(const char* data, unsigned int size);
	void handleReadEvent();
	void handleWriteEvent();

	EventLoop* event_loop_;
	Socket socket_;
	IpAddress local_address_;
	IpAddress peer_address_;
	Handler handler_;
	RingBuffer out_buffer_;
	RingBuffer in_buffer_;

	std::atomic<bool> opened_;

	EventCallback connect_callback_;
	EventCallback read_callback_;
	EventCallback write_callback_;
	EventCallback close_callback_;
	EventCallback error_callback_;

	HandlerChain* handler_chain_;

};

typedef std::shared_ptr<Channel> ChannelPtr;

}

#endif
