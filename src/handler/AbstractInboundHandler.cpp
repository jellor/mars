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
	// DEBUG << "AbstractInboundHandler Destructor ...";
	// if (next_ != nullptr) {
	// 	delete next_;
	// }
}

void AbstractInboundHandler::setNext(AbstractInboundHandler* next) {
	next_ = next;
}

AbstractInboundHandler* AbstractInboundHandler::getNext() const {
	return next_;
}