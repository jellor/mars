/*========================================================
 *
 *   文件名称 ：HandlerContext.h
 *   创建日期 ：2016.7.28
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#ifndef __HANDLER_CONTEXT_H__
#define __HANDLER_CONTEXT_H__

#include "NonCopyable.h"
#include "Channel.h"

namespace mars {

class HandlerContext : public NonCopyable {
public:
	HandlerContext(const ChannelPtr& channel_ptr);

private:
	ChannelPtr& channel_ptr_;
};

}

#endif