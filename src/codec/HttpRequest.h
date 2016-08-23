/*========================================================
 *
 *   文件名称 ：HttpRequest.h
 *   创建日期 ：2016.8.6
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#ifndef __HTTP_REQUEST_H__
#define __HTTP_REQUEST_H__

#include <map>
#include <string>

namespace mars {

class HttpRequest {
public:
	HttpRequest();
	HttpRequest(const std::string& method);
	HttpRequest(const std::string& method, const std::string& uri);
	HttpRequest(const std::string& method, const std::string& uri, const std::string& version);
	~HttpRequest();

	void setMethod(const std::string& method);
	const std::string& getMethod() const;

	void setUri(const std::string& uri);
	const std::string& getUri() const;

	void setVersion(const std::string& version);
	const std::string& getVersion() const;

	void setHeader(const std::string& name, const std::string& value);
	const std::string& getHeader(const std::string& name) const;

	void setBody(const std::string& body);
	const std::string& getBody() const;

private:
	typedef std::map<const std::string, std::string> Header;
	std::string method_;
	std::string uri_;
	std::string version_;
	Header headers_;
	std::string body_;

	static const std::string HTTP_GET;
	static const std::string HTTP_PUT;
	static const std::string HTTP_POST;
	static const std::string HTTP_HEAD;
	static const std::string HTTP_PATCH;
	static const std::string HTTP_TRACE;
	static const std::string HTTP_DELETE;
	static const std::string HTTP_OPTIONS;
	static const std::string HTTP_CONNECT;
};

}

#endif