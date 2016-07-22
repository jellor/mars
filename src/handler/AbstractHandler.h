/*========================================================
 *
 *   文件名称 ：AbstractHandler.h
 *   创建日期 ：2016.5.18
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#ifndef __ABSTRACT_HANDLER_H__
#define __ABSTRACT_HANDLER_H__

#include "NonCopyable.h"
#include "Channel.h"
#include <string>

namespace mars {

class AbstractHandler : public NonCopyable {
public:
	AbstractHandler(): name_() {};
	virtual ~AbstractHandler() = default;

	virtual void handle(ChannelPtr channel_ptr, void* object) = 0;

	virtual const std::string& getName() const { return name_; }
	virtual void setName(const std::string& name)    { name_ = name; }

private:
	std::string name_;
};

}

#endif