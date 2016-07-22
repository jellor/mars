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
	Task(const TaskCallback& cb = nullptr): taskCb_(cb) {}
	virtual void run() { if (taskCb_ != nullptr) taskCb_(); }

	Task(const Task& rhs) {
		taskCb_ = rhs.taskCb_;
		DEBUG << "Task Copy Constructor";
	}

private:
	TaskCallback taskCb_;
};

}

#endif