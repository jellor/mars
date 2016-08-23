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
	AbstractHandler() {};
	virtual ~AbstractHandler() { DEBUG << "AbstractHandler Destructor ..."; };

	virtual const std::string getName() { return "AbstractHandler"; }

};

}

#endif