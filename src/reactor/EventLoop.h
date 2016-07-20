/*========================================================
 *
 *   文件名称 ：EventLoop.h
 *   创建日期 ：2016.5.18
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#ifndef __EVENT_LOOP_H__
#define __EVENT_LOOP_H__

#include "Selector.h"

namespace mars {

class EventLoop {
public:
	EventLoop();
	~EventLoop();

	void start();
	void stop();

	void addHandler(Handler* handler);
	void updateHandler(Handler* handler);
	void removeHandler(Handler* handler);

private:
	void loop();

	Selector* selector_;
	bool started_;
};

}

#endif