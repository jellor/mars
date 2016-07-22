/*========================================================
 *
 *   文件名称 ：Log.cpp
 *   创建日期 ：2016.5.18
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *   描 述 ：
 *
 *=======================================================*/

#include "Log.h"
#include "Thread.h"
#include <errno.h>
#include <sys/time.h>

using namespace mars;

static const char* LOG_LEVEL_STRING[] = {
	"TRACE ",
	"DEDUG ",
	"INFO  ",
	"WARN  ",
	"ERROR ",
	"FATAL ",
};

void defaultLogWriteCb(const char* data, size_t size) {
	fwrite(data, 1, size, stdout);
}

/***************** Class LogImpl *****************/
LogImpl::LogImpl(LOG_LEVEL level, const char* file, const char* func, int line): 
logStream_() 
{
	struct timeval tv;
	struct tm tm;
	char buf[32];
	gettimeofday(&tv, NULL);
	localtime_r(&tv.tv_sec, &tm);
	snprintf(buf, sizeof(buf), 
		"%4d-%02d-%02d %02d:%02d:%02d.%06d ", 
		tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, tv.tv_usec);
	logStream_ << buf;
	const char* fileName = file; //strrchr(file, '/');
	if (fileName == NULL) {
		fileName = file;
	} else {
		fileName += 1;
	}
	logStream_ << Thread::getCurrentThreadName() << ' ' << Thread::getCurrentThreadId() << ' ';
	logStream_ << fileName << ' ' << func << ' ' << line << ' ' << LOG_LEVEL_STRING[level];
}

/******************  Class Log  ******************/
LOG_LEVEL Log::logLevel_ 	 = DEFAULT_LOG_LEVEL;
const char* Log::path_   	 = DEFAULT_LOG_PATH;
LogWriteCallback Log::logWriteCb_ = defaultLogWriteCb;

Log::Log(LOG_LEVEL level, const char* file, const char* func, int line):
logImpl_(level, file, func, line),
isEnableLog_(Log::getLogLevel() <= level)
{}

Log::~Log() {
	getLogStream() << '\n';
	std::string str = getLogStream().toString();
	logWriteCb_(str.c_str(), str.size());
}

std::string Log::getError() {
	char buf[256];
	strerror_r(errno, buf, sizeof(buf));
	return std::string(buf);
}


