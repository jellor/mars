/*========================================================
 *
 *   文件名称 ：IpAddress.cpp
 *   创建日期 ：2016.5.21
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *   描 述 ：
 *
 *=======================================================*/

#include "IpAddress.h"
#include <string.h>

using namespace mars;

IpAddress::IpAddress(const std::string& ip, uint16_t port) {
	memset(&addr_, 0, sizeof(struct sockaddr_in));
	addr_.sin_family 	  = AF_INET;
	addr_.sin_port  	  = htons(port);
	if (inet_pton(AF_INET, ip.c_str(), &addr_.sin_addr) <= 0) {
		struct hostent* host = gethostbyname(ip.c_str());
		if (host == NULL) {

		} else {
			addr_.sin_addr.s_addr = *(uint32_t*)(host->h_addr);
		}
	}
}

IpAddress::IpAddress(uint16_t port, bool isLoopback) {
	memset(&addr_, 0, sizeof(struct sockaddr_in));
	addr_.sin_family      = AF_INET;
	addr_.sin_port        = htons(port);
	addr_.sin_addr.s_addr = htonl(isLoopback ? INADDR_LOOPBACK : INADDR_ANY);
}

void IpAddress::reset() {
	memset(&addr_, 0, sizeof(struct sockaddr_in));
}

uint16_t IpAddress::port() const {
	return ntohs(addr_.sin_port);
}

std::string IpAddress::ip() const {
	char ipBuf[32];
	inet_ntop(AF_INET, &addr_.sin_addr, ipBuf, sizeof(ipBuf));
	return ipBuf;
}

std::string IpAddress::toString() const {
	char buf[32];
	uint16_t port = ntohs(addr_.sin_port);
	uint32_t ip   = ntohl(addr_.sin_addr.s_addr);
	snprintf(buf, sizeof(buf), "%d.%d.%d.%d:%d", ip >> 24, (ip >> 16) & 0xFF, (ip >> 8) & 0xFF, ip & 0xFF, port);
	return buf;
}

bool IpAddress::operator ==(const IpAddress& rhs) const {
	return addr_.sin_port == rhs.addr_.sin_port && addr_.sin_addr.s_addr == rhs.addr_.sin_addr.s_addr;
}

