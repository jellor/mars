/*========================================================
 *
 *   文件名称 ：EpollSelector.cpp
 *   创建日期 ：2016.5.18
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#ifdef __linux__
#include "EpollSelector.h"

using namespace starnet;

EpollSelector::EpollSelector():
epfd_(epoll_create(1024))
{

}

EpollSelector::~EpollSelector() {

}

void EpollSelector::select(int timeout) {

}

void EpollSelector::addHandler(Handler* handler) {

}

void EpollSelector::updateHandler(Handler* handler) {

}

void EpollSelector::removeHandler(Handler* handler) {

}

#endif // __linux__