/*========================================================
 *
 *   文件名称 ：Task.h
 *   创建日期 ：2016.6.9
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#ifndef __TASK_H__
#define __TASK_H__

#include "Log.h"

namespace mars {

typedef std::function<void(void)> TaskCallback;

class Task {
public:
	Task(const TaskCallback& cb = nullptr): task_callback_(cb) {}
	Task(const Task& rhs) {
		task_callback_ = rhs.task_callback_;
		DEBUG << "Task Copy Constructor";
	}
	virtual ~Task() = default;

	void start() { if (task_callback_ != nullptr) task_callback_(); run(); }
	virtual void run() {}

private:
	TaskCallback task_callback_;
};

}

#endif
