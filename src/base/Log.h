/*========================================================
 *
 *   文件名称 ：Log.h
 *   创建日期 ：2016.5.29
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#ifndef __LOG_H__
#define __LOG_H__

#include "NonCopyable.h"
#include <string>
#include <functional>

namespace mars {

typedef enum {
		LOG_TRACE = 0,
		LOG_DEBUG = 1,
		LOG_INFO  = 2,
		LOG_WARN  = 3,
		LOG_ERROR = 4,
		LOG_FATAL = 5,
	} LOG_LEVEL;

static const LOG_LEVEL   DEFAULT_LOG_LEVEL = LOG_DEBUG;
static const char*       DEFAULT_LOG_PATH  = "./log";

/***************** Class LogStream *****************/
class LogStream : public NonCopyable {
public:
	LogStream(): buffer_() {}
	~LogStream() {}

    inline LogStream& operator <<(const void* val);
	inline LogStream& operator <<(const char* val);
	inline LogStream& operator <<(bool val);
	inline LogStream& operator <<(char val);
	inline LogStream& operator <<(unsigned char val);
	inline LogStream& operator <<(short val);
	inline LogStream& operator <<(unsigned short val);
	inline LogStream& operator <<(int val);
	inline LogStream& operator <<(unsigned int val);
	inline LogStream& operator <<(long val);
	inline LogStream& operator <<(unsigned long val);
	inline LogStream& operator <<(long long val);
	inline LogStream& operator <<(unsigned long long val);
	inline LogStream& operator <<(float val);
	inline LogStream& operator <<(double val);
	inline LogStream& operator <<(const std::string& val);

	inline std::string toString() { return buffer_.toString(); }
	
private:
	template<class T>
	LogStream& write(const char* fmt, T t);

	// Internal Class
	class Buffer {
	public:
		Buffer(): currentIndex_(0) { memset(buffer_, 0, sizeof(buffer_));}
		void reset()  { currentIndex_ = 0; memset(buffer_, 0, sizeof(buffer_)); }
		char* begin() { return buffer_ + currentIndex_;   }
		char* end()   { return buffer_ + sizeof(buffer_); }
		bool available() const { return currentIndex_ < MAX_BUFFER_SIZE; }	
		int length()     const { return MAX_BUFFER_SIZE - currentIndex_; }
		void add(int length)   { currentIndex_ += length; }
		std::string toString() { return std::string(buffer_); }

		static const int MAX_BUFFER_SIZE = 4 * 1024;
	private:
		char buffer_[MAX_BUFFER_SIZE]; // ? heap object
		int currentIndex_; 
	};
	Buffer buffer_;
};

inline LogStream& LogStream::operator <<(const void* val) {
	return write("0X%p", val);
}

inline LogStream& LogStream::operator <<(const char* val) {
	return write("%s", val);
}

inline LogStream& LogStream::operator <<(bool val) {
	return val ? write("%s", "true") : write("%s", "false");
}

inline LogStream& LogStream::operator <<(char val) {
	return write("%c", val);
}

inline LogStream& LogStream::operator <<(unsigned char val) {
	return write("%c", val);
}

inline LogStream& LogStream::operator <<(short val) {
	return write("%d", static_cast<int>(val));
}

inline LogStream& LogStream::operator <<(unsigned short val) {
	return write("%u", static_cast<unsigned short>(val));
}

inline LogStream& LogStream::operator <<(int val) {
	return write("%d", val);
}

inline LogStream& LogStream::operator <<(unsigned int val) {
	return write("%u", val);
}

inline LogStream& LogStream::operator <<(long val) {
	return write("%ld", val);
}

inline LogStream& LogStream::operator <<(unsigned long val) {
	return write("%lu", val);
}

inline LogStream& LogStream::operator <<(long long val) {
	return write("%lld", val);
}

inline LogStream& LogStream::operator <<(unsigned long long val) {
	return write("%llu", val);
}

inline LogStream& LogStream::operator <<(float val) {
	return write("%.6f", val);
}

inline LogStream& LogStream::operator <<(double val) {
	return write("%.6lf", val);
}

inline LogStream& LogStream::operator <<(const std::string& val) {
	return write("%s", val.c_str());
}

template<class T>
inline LogStream& LogStream::write(const char* fmt, T t) {
	if (buffer_.available()) {
		int len = snprintf(buffer_.begin(), buffer_.length(), fmt, t);
		if (len < 0) {
			buffer_.reset();
		} else if (len >= buffer_.length()) {
			*(buffer_.end() - 1) = '\0';
			buffer_.add(len);
		} else {
			buffer_.add(len);
		}
	}
	return *this;
}

/***************** Class LogImpl *****************/
class LogImpl {
public:
	LogImpl(LOG_LEVEL level, const char* file, const char* func, int line);
	~LogImpl() {}

	LogStream& getLogStream() { return logStream_; }

private:
	LogStream logStream_;
};

/******************  Class Log  ******************/
typedef std::function<void (const char*, size_t size)> LogWriteCallback;
class Log {
public:
	Log(LOG_LEVEL level, const char* file, const char* func, int line);
	~Log();

	bool isEnableLog() const { return isEnableLog_; }
	LogStream& getLogStream() { return logImpl_.getLogStream(); }

	static LOG_LEVEL getLogLevel() { return logLevel_; }
	static void setLogLevel(LOG_LEVEL logLevel) { logLevel_ = logLevel; }
	static const char* getLogPath() { return path_; }
	static void setLogPath(const char* path) { path_ = path; }

	static void setLogWriteCallback(const LogWriteCallback& cb) { logWriteCb_ = cb; }
	static std::string getError();

private:
	LogImpl logImpl_;
	bool isEnableLog_;
	static LOG_LEVEL logLevel_;
	static const char* path_;
	static LogWriteCallback logWriteCb_;
};

/******************  Log Macro  ******************/
#define TRACE Log(LOG_TRACE, __FILE__, __FUNCTION__, __LINE__).getLogStream()
#define DEBUG Log(LOG_DEBUG, __FILE__, __FUNCTION__, __LINE__).getLogStream()
#define INFO  Log(LOG_INFO,  __FILE__, __FUNCTION__, __LINE__).getLogStream()
#define WARN  Log(LOG_WARN,  __FILE__, __FUNCTION__, __LINE__).getLogStream()
#define ERROR Log(LOG_ERROR, __FILE__, __FUNCTION__, __LINE__).getLogStream()
#define FATAL Log(LOG_FATAL, __FILE__, __FUNCTION__, __LINE__).getLogStream()

}

#endif