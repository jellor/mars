/*========================================================
 *
 *   文件名称 ：Configure.cpp
 *   创建日期 ：2016.6.7
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#include "Configure.h"
#include "Log.h"

using namespace mars;

Configure::Configure(const char* file_name):
file_name_(file_name),
loaded_(false)
{
	loadFile(file_name);
}

Configure::~Configure() {

}

const char* Configure::getValueByName(const char* name) {
	if (! loaded_) return NULL;
	const char* ret = NULL;
	std::map<std::string, std::string>::iterator it = configure_map_.find(name);
	if (it != configure_map_.end()) {
		ret = static_cast<const char*>(it->second.c_str());
	}
	return ret;
}

bool Configure::setValueByName(const char* name, const char* value) {
	if (! loaded_) return false;
	std::map<std::string, std::string>::iterator it = configure_map_.find(name);
	if (it != configure_map_.end()) {
		it->second = value;
	} else {
		configure_map_.insert(std::make_pair(name, value));
	}
	return saveFile(file_name_.c_str());
}

void Configure::loadFile(const char* file_name) {
	file_name_ = file_name;
	FILE* file = fopen(file_name, "r");
	if (file == NULL) {
		WARN << "Can not open the file " << file_name;
		return;
	}
	char buf[512];
	while (fgets(buf, sizeof(buf), file) != NULL) {
		int len = strlen(buf);
		if (buf[len - 1] == '\n') {
			buf[len - 1] = 0;
		}
		char* p = strchr(buf, '#');
		if(p != NULL) *p = 0;
		if (strlen(buf) == 0) continue;
		parseLine(buf);
	} 
	fclose(file);
	loaded_ = true;
}

bool Configure::saveFile(const char* file_name) {
	FILE* file = fopen(file_name, "w");
	if (file == NULL) {
		return false;
	}
	char buf[512];
	for (std::map<std::string, std::string>::iterator it = configure_map_.begin(); it != configure_map_.end(); it ++) {
		memset(buf, 0, sizeof(buf)); // ? can remove this operation
		snprintf(buf, sizeof(buf), "%s = %s\n", it->first.c_str(), it->second.c_str());
		int ret = fwrite(buf, strlen(buf), 1, file);
		if (ret != 1) {
			fclose(file);
			return false;
		}
	}
	fclose(file);
	return true;
}

void Configure::parseLine(char* line) { 
	char* p = strchr(line, '=');
	*p = 0;
	char* key = trim(line);
	char* value = trim(p + 1);
	if (key != NULL && value != NULL) {
		configure_map_.insert(std::make_pair(key, value));
	}
}

char* Configure::trim(char* str) const {
	char* begin = str;
	char* end   = str + strlen(str) - 1;
	while (*begin == ' ' || *begin == '\t') {
		begin ++;
	}
	if (strlen(begin) == 0) return NULL;

	while (*end == ' ' || *end == '\t') {
		*end = 0;
		end --;
	}
	return begin;
}


