/*========================================================
 *
 *   文件名称 ：NonCopyable.h
 *   创建日期 ：2016.5.18
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#ifndef __NON_COPYABLE_H__
#define __NON_COPYABLE_H__

namespace mars {

class NonCopyable {
protected:
	NonCopyable()  = default;
	~NonCopyable() = default;

private:
	NonCopyable(const NonCopyable&) 			 = delete;
	NonCopyable& operator = (const NonCopyable&) = delete;
};

}

#endif