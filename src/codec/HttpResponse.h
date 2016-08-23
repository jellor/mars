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

	void setStatusCode(int status_code);
	int getStatusCode() const;
	void setStatusMessage(const std::string& status_message);
	const std::string& getStatusMessage() const;
	void setHeader(const std::string& name, const std::string& value);
	const std::string& getHeader(const std::string& name) const;

private:
	typedef std::map<const std::string, std::string> Header;
	std::string version_;
	int status_code_;
	std::string status_message_;

	Header headers_;
};

}

#endif