/*========================================================
 *
 *   文件名称 ：Thread.cpp
 *   创建日期 ：2016.5.27
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#include "Thread.h"
#include "Exception.h"
#include <stdio.h>
#include <sys/syscall.h> 
#include <unistd.h>
#include <errno.h>

using namespace mars;

__thread pid_t thread_tid = 0;
__thread const char* thread_name = NULL;


void afterFork() {
	thread_tid  = 0;
	Thread::getCurrentThreadId();
	thread_name = "Main";
}

class ThreadInitialer {
public:
	ThreadInitialer() {
		//thread_tid  = static_cast<pid_t>(Thread::getCurrentThreadId()); // ? 不删除这行会有bug or 注意下列的执行顺序
		thread_tid = 0;
		thread_tid = Thread::getCurrentThreadId();
		thread_name = "Main";	
		pthread_atfork(NULL, NULL, afterFork);
		printf("ThreadInitialer Successful\n");
	}
};
ThreadInitialer threadInitialer;


/*************** Internal Class ThreadSpace ***************
 *
 *   描 述 ：主要是用于线程内的执行
 *
 *********************************************************/
class ThreadSpace {
public:
	ThreadSpace(const ThreadFunc& thread_func, const std::string& name, const std::shared_ptr<pid_t>& tid);
	~ThreadSpace();

	void startThread();
	void release();

private:
	std::weak_ptr<pid_t> tid_;
	std::string name_;
	ThreadFunc thread_func_;
};

ThreadSpace::ThreadSpace(const ThreadFunc& thread_func, const std::string& name, const std::shared_ptr<pid_t>& tid):
thread_func_(thread_func),
name_(name),
tid_(tid)
{}

ThreadSpace::~ThreadSpace() {
}

// 在新创建线程中执行
void ThreadSpace::startThread() {
	std::shared_ptr<pid_t> shared_tid = tid_.lock();
	if (shared_tid != NULL) {
		*shared_tid = static_cast<pid_t>(Thread::getCurrentThreadId());
		shared_tid.reset();
	}
	thread_name = name_.empty() ? "Unknown Thread" : name_.c_str();

#ifdef __linux__
	pthread_setname_np(thread_self(), name_.c_str());
#elif __APPLE__
	pthread_setname_np(name_.c_str());
#else
#error "Platform Not Supported"
#endif

	try {
		thread_func_();
	} catch (const Exception& e) {

	} catch (const std::exception& e) {

	} catch (...) {

	} 
	
}
/*********************************************************/

// 线程的入口函数
void* threadFunc(void* arg) {
	ThreadSpace* thread = static_cast<ThreadSpace*>(arg);
	thread->startThread();
	delete thread;
	return NULL;
}

std::atomic<unsigned int> Thread::create_count_(0);

Thread::Thread(const ThreadFunc& thread_func, const std::string& name):
started_(false),
joined_(false),
name_(name),
tid_(new pid_t(0)),
thread_id_(0),
thread_func_(thread_func)
{	
	create_count_++;
	if (name_.empty()) {
		char buf[32];
		snprintf(buf, sizeof(buf), "Unknown Thread %d", create_count_.load());
		name_ = buf;
	}
}

Thread::~Thread() {
	if (started_ && !joined_) {
		printf("Detach");
		pthread_detach(thread_id_);
	}
}

void Thread::setThreadFunc(const ThreadFunc& thread_func) {
	thread_func_ = thread_func;
} 

void Thread::setThreadName(const std::string& name) {
	name_ = name;
}

bool Thread::start() {
	if (thread_func_ == nullptr) return false;
	started_ = true;
	ThreadSpace* arg = new ThreadSpace(thread_func_, name_, tid_);
	int ret = pthread_create(&thread_id_, NULL, threadFunc, static_cast<void*>(arg));
	if (ret == 0) {
		return true;
	} else {
		started_ = false;
		delete arg;
		return false;
	}
}

bool Thread::join() {
	int ret = -1;
	if (started_ && !joined_) {
		joined_  = true;
		ret = pthread_join(thread_id_, NULL);
		started_ = false;	
	}
	return ret == 0 ? true : false;
}

bool Thread::cancel() {
	int ret = -1;
	if (thread_id_ != 0) {
		ret = pthread_cancel(thread_id_);
		started_ = false;
	}
	return ret == 0 ? true : false;
}

pid_t Thread::getCurrentThreadId() {
	if (thread_tid == 0) {

#ifdef __linux__
		thread_tid = static_cast<pid_t>(syscall(SYS_gettid));
#elif __APPLE__
		static pid_t appleTid = 1;
		thread_tid = appleTid ++;
#else
#error "Platform Not Supported"
#endif
	
		if (errno) {
			printf("Happened errno In Thread::getCurrentThreadId %s\n", strerror(errno));
		}
	}
	return thread_tid;
}

std::string Thread::getCurrentThreadName() {
	if (thread_name == NULL) {
		thread_name = "Unknown Thread";
	}
	return thread_name;
}


