/*========================================================
 *
 *   文件名称 ：Handler.cpp
 *   创建日期 ：2016.5.18
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#include "Handler.h"

using namespace mars;

Handler::Handler(EventLoop* event_loop, int fd):
event_loop_(event_loop),
fd_(fd),
status_(NEW),
readable_(false),
writable_(false),
read_callback_(nullptr),
write_callback_(nullptr),
close_callback_(nullptr),
error_callback_(nullptr)
{

}

Handler::~Handler() {
	readable_ = false;
	writable_ = false;
	remove();
}

void Handler::reset() {
	remove();
	event_loop_ 	= nullptr;
	fd_         	= -1;
	status_     	= NEW;
	readable_   	= false;
	writable_  	 	= false;
	read_callback_  = nullptr;
	write_callback_ = nullptr;
	close_callback_ = nullptr;
	error_callback_ = nullptr;
}

void Handler::update() {
	if ((status_ == NEW || status_ == DEL) && (isReadable() || isWritable())) {
		event_loop_->addHandler(this);
	} else if (status_ == ADD && (isReadable() || isWritable())) {
		event_loop_->updateHandler(this);
	} else {
		event_loop_->removeHandler(this);
	}
}

