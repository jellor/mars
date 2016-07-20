/*========================================================
 *
 *   文件名称 ：EpollSelector.h
 *   创建日期 ：2016.5.18
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#ifndef __EPOLL_SELECTOR_H__
#define __EPOLL_SELECTOR_H__

#ifdef __linux__
#include "Selector.h"

namespace starnet {

class EpollSelector : public Selector {
public:
	EpollSelector();
	~EpollSelector();
	
	void select(int timeout) override;
	void addHandler(Handler* handler) override;
	void updateHandler(Handler* handler) override;
	void removeHandler(Handler* handler) override;

private:
	const int epfd_;

};

}

#endif // __linux__
#endif