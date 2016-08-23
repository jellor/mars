/*========================================================
 *
 *   文件名称 ：HttpRequest.cpp
 *   创建日期 ：2016.8.6
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#include "HttpRequest.h"

using namespace mars;

const std::string HttpRequest::HTTP_GET 	= "GET";
const std::string HttpRequest::HTTP_PUT		= "PUT";
const std::string HttpRequest::HTTP_POST	= "POST";
const std::string HttpRequest::HTTP_HEAD	= "HEAD";
const std::string HttpRequest::HTTP_PATCH	= "PATCH";
const std::string HttpRequest::HTTP_TRACE	= "TRACE";
const std::string HttpRequest::HTTP_DELETE	= "DELETE";
const std::string HttpRequest::HTTP_OPTIONS	= "OPTIONS";
const std::string HttpRequest::HTTP_CONNECT	= "CONNECT";

HttpRequest::HttpRequest():
method_(HTTP_GET),
uri_("/"),
version_()
{

}

HttpRequest::HttpRequest(const std::string& method):
method_(method),
uri_("/"),
version_()
{

}

HttpRequest::HttpRequest(const std::string& method, const std::string& uri):
method_(method),
uri_(uri),
version_()
{

}

HttpRequest::HttpRequest(const std::string& method, const std::string& uri, const std::string& version):
method_(method),
uri_(uri),
version_(version)
{

}

HttpRequest::~HttpRequest() {

}

void HttpRequest::setMethod(const std::string& method) {
	method_ = method;
}

const std::string& HttpRequest::getMethod() const {
	return method_;
}

void HttpRequest::setUri(const std::string& uri) {
	uri_ = uri;
}

const std::string& HttpRequest::getUri() const {
	return uri_;
}

void HttpRequest::setVersion(const std::string& version) {
	version_ = version;
}

const std::string& HttpRequest::getVersion() const {
	return version_;
}

void HttpRequest::setHeader(const std::string& name, const std::string& value) {
	headers_[name] = value;
}

const std::string& HttpRequest::getHeader(const std::string& name) const {
	Header::const_iterator it = headers_.find(name);
	if (it != headers_.end()) {
		return (*it).second;
	} else {
		return "";
	}
}

void HttpRequest::setBody(const std::string& body) {
	body_ = body;
}

const std::string& HttpRequest::getBody() const {
	return body_;
}



