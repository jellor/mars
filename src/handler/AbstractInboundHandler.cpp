/*========================================================
 *
 *   文件名称 ：AbstractInboundHandler.cpp
 *   创建日期 ：2016.7.18
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#include "AbstractInboundHandler.h"

using namespace mars;

AbstractInboundHandler::AbstractInboundHandler():
next_(nullptr)
{

}

AbstractInboundHandler::~AbstractInboundHandler() {
	if (next_ != nullptr) {
		delete next_;
	}
}

void AbstractInboundHandler::handle(ChannelPtr channel_ptr, void* object) {
	try {
		onRead(channel_ptr, object);
	} catch (...) {
		onError(channel_ptr);
	}
}

void AbstractInboundHandler::setNext(AbstractInboundHandler* next) {
	next_ = next;
}

AbstractInboundHandler* AbstractInboundHandler::getNext() const {
	return next_;
}