/*========================================================
 *
 *   文件名称 ：KqueueSelector.h
 *   创建日期 ：2016.5.18
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#ifndef __KQUEUE_SELECTOR_H__
#define __KQUEUE_SELECTOR_H__

#ifdef __APPLE__
#include "../Selector.h"
#include "../Handler.h"
#include <set>
#include <vector>
#include <sys/event.h>

namespace starnet {

class KqueueSelector : public Selector {
public:
	KqueueSelector();
	~KqueueSelector();

	void select(int timeout) override;
	void addHandler(Handler* handler) override;
	void updateHandler(Handler* handler) override;
	void removeHandler(Handler* handler) override;

private:
	const int kqfd_;
	std::set<Handler*> handler_set_;
	std::vector<struct kevent> active_event_list_;
};

}

#endif // __APPLE__
#endif