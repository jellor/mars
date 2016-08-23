/*========================================================
 *
 *   文件名称 ：Selector.h
 *   创建日期 ：2016.5.18
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#ifndef __SELECTOR_H__
#define __SELECTOR_H__

#include "base/NonCopyable.h"
#include "Handler.h"
#include <stdlib.h>

namespace mars {

class Handler;

class Selector : public NonCopyable {
public:
	Selector()  		= default;
	virtual ~Selector() = default;

	virtual void dispatch(int64_t timeout_usec)  = 0;
	virtual void addHandler(Handler* handler) 	 = 0;
	virtual void updateHandler(Handler* handler) = 0;
	virtual void removeHandler(Handler* handler) = 0;

	static Selector* newSelector();

};

}

#endif
