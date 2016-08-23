/*========================================================
 *
 *   文件名称 ：Base64.h
 *   创建日期 ：2016.8.4
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#ifndef __BASE64_H__
#define __BASE64_H__

#include <string>

namespace mars {

class Base64 {
public:
	static std::string encode(const std::string& binary_data);
	static std::string decode(const std::string& ascii_data);

private:
	static const char encoding_table[64];
	static const char decoding_table[128];
};

}

#endif