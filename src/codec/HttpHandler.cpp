/*========================================================
 *
 *   文件名称 ：HttpHandler.cpp
 *   创建日期 ：2016.5.18
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#include "HttpHandler.h"

using namespace mars;

HttpHandler::HttpHandler():
http_context_()
{
    http_context_.setHttpCallback(std::bind(&HttpHandler::onHttp, this, std::placeholders::_1));
}

HttpHandler::~HttpHandler() {

}

void HttpHandler::onReceive(const ChannelPtr& channel_ptr, void* object) {
    if (http_context_.decode(channel_ptr) == false) {
        channel_ptr->getChain()->close(channel_ptr);
    }
}

void HttpHandler::onHttp(const ChannelPtr& channel_ptr) {
    DEBUG << http_context_.getRequest().toString();
    std::string str("<html><head><title>xxx</title></head><body></body></html>");
    const HttpRequest request = http_context_.getRequest();
    HttpResponse response(request.getVersion());
    response.setBody(str);
    DEBUG << response.toString();
    sendResponse(channel_ptr, response);
}

void HttpHandler::sendResponse(const ChannelPtr& channel_ptr, const HttpResponse& response) {
    RingBuffer* buffer = http_context_.encode(response);
    channel_ptr->getChain()->send(channel_ptr, buffer->head(), buffer->size());
}
