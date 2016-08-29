/*========================================================
 *
 *   文件名称 ：HttpResponse.h
 *   创建日期 ：2016.8.6
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#ifndef __HTTP_RESPONSE_H__
#define __HTTP_RESPONSE_H__

#include <map>
#include <string>

namespace mars {

class HttpResponse {
public:
	HttpResponse();
	HttpResponse(const std::string& version);
	~HttpResponse();

    void setVersion(const std::string& version);
	const std::string& getVersion() const;
	void setStatusCode(int status_code);
	int getStatusCode() const;
	void setStatusMessage(const std::string& status_message);
	const std::string& getStatusMessage() const;
	void setHeader(const std::string& name, const std::string& value);
	const std::string& getHeader(const std::string& name) const;
	void setBody(const std::string& body);
	void setBody(const char* data, int length);
	const std::string& getBody() const;

	const std::string toString() const;

private:
	typedef std::map<const std::string, std::string> Header;
	std::string version_;
	int status_code_;
	std::string status_message_;
	std::string body_;

	Header headers_;
};

}

#endif
