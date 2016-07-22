/*========================================================
 *
 *   文件名称 ：IpAddress.h
 *   创建日期 ：2016.5.21
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#ifndef __IP_ADDRESS_H__
#define __IP_ADDRESS_H__

#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

namespace mars {

class IpAddress {
public:
	IpAddress(const std::string& ip, uint16_t port);
	IpAddress(uint16_t port, bool isLoopback = true);
	IpAddress(): IpAddress(0) {}
	IpAddress(const struct sockaddr_in& addr): addr_(addr) {};

	uint16_t port() const;
	std::string ip() const;
	std::string toString() const;
	void setAddr(const struct sockaddr_in& addr) { addr_ = addr; }
	const struct sockaddr_in& getAddr() const { return addr_; }
	bool operator ==(const IpAddress& rhs) const;
private:
	struct sockaddr_in addr_;
};

}

#endif