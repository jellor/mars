/*========================================================
 *
 *   文件名称 ：Selector.cpp
 *   创建日期 ：2016.5.18
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#include "Selector.h"
#include "selector/EpollSelector.h"
#include "selector/KqueueSelector.h"

using namespace mars;

Selector* Selector::newSelector() {
#ifdef __linux__
	return new EpollSelector();
#elif __APPLE__
	return new KqueueSelector();
#else
#error "Platform Not Supported"
#endif
}