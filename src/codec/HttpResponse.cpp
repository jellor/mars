/*========================================================
 *
 *   文件名称 ：HttpResponse.cpp
 *   创建日期 ：2016.8.6
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#include "HttpResponse.h"

using namespace mars;

HttpResponse::HttpResponse():
version_("HTTP/1.0"),
status_code_(200),
status_message_("OK"),
headers_()
{

}

HttpResponse::HttpResponse(const std::string& version):
version_(version),
status_code_(200),
status_message_("OK"),
headers_()
{

}

HttpResponse::~HttpResponse() {
	
}

void HttpResponse::setStatusCode(int status_code) {
	status_code_ = status_code;
}

int HttpResponse::getStatusCode() const {
	return status_code_;
}

void HttpResponse::setStatusMessage(const std::string& status_message) {
	status_message_ = status_message;
}

const std::string& HttpResponse::getStatusMessage() const {
	return status_message_;
}

void HttpResponse::setHeader(const std::string& name, const std::string& value) {
	headers_[name] = value;
}

const std::string& HttpResponse::getHeader(const std::string& name) const {
	Header::const_iterator it = headers_.find(name);
	if (it != headers_.end()) {
		return (*it).second;
	} else {
		return "";
	}
}
