/*========================================================
 *
 *   文件名称 ：File.h
 *   创建日期 ：2016.6.7
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#ifndef __FILE_H__
#define __FILE_H__

namespace mars {

class File {
	static bool isExisted(const char* filename);
	static bool getFileStatus(const char* filename, char *buf, int size);
};

}

#endif