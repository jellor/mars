/*========================================================
 *
 *   文件名称 ：SinkHandler.cpp
 *   创建日期 ：2016.7.28
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#include "SinkHandler.h"
#include "Log.h"

using namespace mars;

void SinkHandler::send(const ChannelPtr& channel_ptr, void* object) {
	//channel_ptr->send(data, len);
}

void SinkHandler::send(const ChannelPtr& channel_ptr, char* data, int len) {
	channel_ptr->send(data, len);
}

void SinkHandler::sendTo(const ChannelPtr& channel_ptr, void* object) {

}

void SinkHandler::connect(const IpAddress& remote_address) {

}

void SinkHandler::shutdownSend(const ChannelPtr& channel_ptr) {
	WARN << "Shutdown " << channel_ptr->getPeerAddress().toString() << " Send";
	channel_ptr->shutdownSend();
}

void SinkHandler::shutdownReceive(const ChannelPtr& channel_ptr) {
	WARN << "Shutdown " << channel_ptr->getPeerAddress().toString() << " Receive";
	channel_ptr->shutdownReceive();
}

void SinkHandler::close(const ChannelPtr& channel_ptr) {
	WARN << "Close " << channel_ptr->getPeerAddress().toString() << " Send";
	channel_ptr->close();
}



  


