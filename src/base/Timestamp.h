/*========================================================
 *
 *   文件名称 ：Timestamp.h
 *   创建日期 ：2016.5.29
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#ifndef __TIMESTAMP_H__
#define __TIMESTAMP_H__

#include <stdint.h>
#include <string>

namespace mars {

class Timestamp {
public:
	explicit Timestamp(int64_t usec = 0): usec_(usec) {}

	int64_t macrosecond() const { return usec_; }
	const std::string toString() const;
	static Timestamp now();

private:
	int64_t usec_;
};

}

#endif