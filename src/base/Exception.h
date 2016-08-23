/*========================================================
 *
 *   文件名称 ：Exception.h
 *   创建日期 ：2016.6.15
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

#include <exception>
#include <string>

namespace mars {

class Exception : public std::exception {
public:
	Exception(const char* what) noexcept : what_(what) {}
	Exception(const std::string& what) noexcept : what_(what) {}
	virtual ~Exception() noexcept {}

	virtual const char* what() const noexcept override { return what_.c_str(); };
	const std::string toString() const noexcept;
	const std::string getStackTrace() const noexcept;

private:
	std::string demangle(const char* name) const;

	enum { MAX_STACK_TRACE_FRAME_SIZE = 100 };
	std::string what_;
};

}

#endif