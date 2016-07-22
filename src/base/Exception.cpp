/*========================================================
 *
 *   文件名称 ：Exception.cpp
 *   创建日期 ：2016.6.15
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#include "Exception.h"
#include "Log.h"
#include <sstream>
#include <cxxabi.h>
#include <execinfo.h>
#include <stdlib.h>

using namespace mars;

const std::string Exception::toString() const noexcept {
	std::stringstream ss("");
	if (!what_.empty()) {
		ss << "What: " << what_ << "  |  ";
	}
	ss << "Stack Trace: " << getStackTrace();
	return ss.str();
}

const std::string Exception::getStackTrace() const noexcept {
	void* buffer[MAX_STACK_TRACE_FRAME_SIZE];
	int size = backtrace(buffer, MAX_STACK_TRACE_FRAME_SIZE);
	if (size == 0) {
		return "Stack Back Is Empty";
	}
	char** strings = backtrace_symbols(buffer, size);
	if (strings == NULL) {
		return "Backtrace_symbols Return Is NULL";
	}
	std::string result;
	for (int i = 0; i < size; i ++) {
		result.append(demangle(strings[i]));
		result.push_back('\n');
	}
	free(strings);
	return result;
}

std::string Exception::demangle(const char* name) const {
	int status;
	std::string demangled(name);
	std::string::size_type begin = demangled.find('(');
	std::string::size_type end   = demangled.find('+', begin);
	DEBUG << "Begin " << begin << " End " << end;
	if (begin == std::string::npos || end == std::string::npos) {
		DEBUG << "Name Is Parse Error";
		return name;
	}
	char* realName = abi::__cxa_demangle(demangled.substr(begin, end).c_str(), nullptr, nullptr, &status);
	DEBUG << "Status Is " << status;
	if (realName == NULL) {
		DEBUG << "ReaL Name Is NULL";
		return name;
	}
	demangled = realName;
	free(realName);
	return demangled;
}









