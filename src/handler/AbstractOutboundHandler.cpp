/*========================================================
 *
 *   文件名称 ：AbstractOutboundHandler.cpp
 *   创建日期 ：2016.7.18
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#include "AbstractOutboundHandler.h"

using namespace mars;

AbstractOutboundHandler::AbstractOutboundHandler():
next_(nullptr)
{

}

AbstractOutboundHandler::~AbstractOutboundHandler() {
	if (next_ != nullptr) {
		delete next_;
	}
}

void AbstractOutboundHandler::handle(ChannelPtr channel_ptr, void* object) {
	try {
		onWrite(channel_ptr, object);
	} catch (...) {
		onError(channel_ptr);
	}
}

void AbstractOutboundHandler::setNext(AbstractOutboundHandler* next) {
	next_ = next;
}

AbstractOutboundHandler* AbstractOutboundHandler::getNext() const {
	return next_;
}