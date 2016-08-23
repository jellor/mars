/*========================================================
 *
 *   文件名称 ：Configure.h
 *   创建日期 ：2016.6.7
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#ifndef __CONFIGURE_H__
#define __CONFIGURE_H__

#include <map>
#include <string>

namespace mars {

class Configure {
public:
	Configure(const char* file_name);
	~Configure();

	const char* getValueByName(const char* name);
	bool  setValueByName(const char* name, const char* value);

private:
	void loadFile(const char* file_name);
	bool saveFile(const char* file_name);
	void parseLine(char* line);
	char* trim(char* line) const; 
	
	std::string file_name_;
	bool loaded_;
	std::map<std::string, std::string> configure_map_;
};

}

#endif