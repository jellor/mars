/*========================================================
 *
 *   文件名称 ：Thread.h
 *   创建日期 ：2016.5.27
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#ifndef __THREAD_H__
#define __THREAD_H__

#include "NonCopyable.h"
#include <atomic>
#include <functional>
#include <pthread.h>
#include <sys/types.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <memory>

namespace mars {

typedef std::function<void(void)> ThreadFunc;

class Thread : public NonCopyable {
public:
	Thread(const ThreadFunc& thread_func = nullptr, const std::string& name = std::string());
	~Thread();

	void setThreadFunc(const ThreadFunc& thread_func);
	void setThreadName(const std::string& name);
	bool start();
	bool join();
	bool cancel();
	bool isStarted() const { return started_; }
	int  getId() const { return *tid_; }

	static void sleep(double seconds) { usleep(seconds * 1000000); }
	static bool isMainThread() { return strcmp(getCurrentThreadName().c_str(), "Main") == 0 ? true : false; }
	static int getCreatedCount() { return create_count_.load(); }
	static int getCurrentThreadId();
	static std::string getCurrentThreadName();

private:
	bool started_;
	bool joined_;
	std::string name_;
	std::shared_ptr<pid_t> tid_;
	pthread_t thread_id_;
	ThreadFunc thread_func_;

	static std::atomic<unsigned int> create_count_;
};

}

#endif