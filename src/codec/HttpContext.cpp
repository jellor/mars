/*========================================================
 *
 *   文件名称 ：HttpContext.cpp
 *   创建日期 ：2016.5.18
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#include "HttpContext.h"
#include "Log.h"
#include <stdlib.h>

using namespace mars;

HttpContext::HttpContext():
request_(),
response_(),
parse_status_(PARSE_REQUEST_LINE)
{

}

HttpContext::~HttpContext() {

}

void HttpContext::encode(RingBuffer* buffer) {

}

void HttpContext::decode(RingBuffer* buffer) {
	buffer->adjust();
	DEBUG << "head " << buffer->headIndex();
	DEBUG << "tail " << buffer->tailIndex();
	int length = parseRequest(const_cast<char*>(buffer->head()), buffer->size());
	DEBUG << "length " << length;
	if (length < 0) {

	} else {
		buffer->skipFromHead(length);
	}
	
}

PARSE_LINE_STATUS HttpContext::parseLine(char* buffer, int& start_index, const int end_index) {

	char crlf;

	while (start_index < end_index) {
		DEBUG << "start " << start_index;
		DEBUG << "end   " << end_index;
		crlf = buffer[start_index];

		if (crlf == '\r') {

			if (start_index + 1 == end_index) {
				return PARSE_LINE_CONTINUE;
			} else if (buffer[start_index + 1] == '\n') {
				buffer[start_index ++] = '\0';
				buffer[start_index ++] = '\0';
				return PARSE_LINE_OK;
			} else {
				return PARSE_LINE_BAD;
			}

		} else if (crlf == '\n') {

			if (start_index > 0 && buffer[start_index - 1] == '\r') {
				buffer[start_index - 1] = '\0';
				buffer[start_index ++]  = '\0';
				return PARSE_LINE_OK;
			} else {
				return PARSE_LINE_BAD;
			}

		} else {
			start_index ++;
		}

	}

	return PARSE_LINE_CONTINUE;

}

bool HttpContext::parseRequestLine(char* line) {

	char* url = strpbrk(line, " \t");
	if (url == nullptr) {
		return false;
	}

	(*url ++) = '\0';

	if (strcasecmp(line, "GET") == 0) {
		request_.setMethod("GET");
	} else if (strcasecmp(line, "POST") == 0) {
		request_.setMethod("POST");
	} else if (strcasecmp(line, "HEAD") == 0) {
		request_.setMethod("HEAD");
	} else if (strcasecmp(line, "PUT") == 0) {

	} else if (strcasecmp(line, "TRACE") == 0) {

	} else if (strcasecmp(line, "PATCH") == 0) {

	} else if (strcasecmp(line, "DELETE") == 0) {

	} else if (strcasecmp(line, "OPTIONS") == 0) {

	} else if (strcasecmp(line, "CONNECT") == 0) {

	} else {
		return false;
	}

	DEBUG << "Methon => " << request_.getMethod();

	url += strspn(url, " \t");

	char* version = strpbrk(url, " \t");
	if (version == nullptr) {
		return false;
	}

	(*version ++) = '\0';
	version += strspn(version, " \t");

	DEBUG << "version --> " << version;

	if (strcasecmp(version, "HTTP/1.1") == 0) {
		request_.setVersion("HTTP/1.1");
	} else if (strcasecmp(version, "HTTP/1.0") == 0) {
		request_.setVersion("HTTP/1.0");
	} else {
		return false;
	}

	DEBUG << "Version => " << request_.getVersion();
	DEBUG << "url => " << url;

	if (strncasecmp(url, "http://", 7) == 0) {
		url += 7;
		url = strchr(url, '/');
	} else if (strncasecmp(url, "https://", 8) == 0) {
		url += 8;
		url = strchr(url, '/');
	} 

	if (*url != '/' || url == nullptr) {
		return false;
	}

	request_.setUri(url);

	DEBUG << "uri => " << request_.getUri();

	return true;

}

bool HttpContext::parseRequestHeader(char* line) {

	char* name = line;
	char* value = strpbrk(line, ":");
	if (value == nullptr || *value == '\0') {
		return false;
	}
	(*value ++) = '\0';
	if (*value != ' ' || *value == '\t') {
		return false;
	}
	value += strspn(value, " \t");
	if (*value == '\0') {
		return false;
	}
	request_.setHeader(name, value);
	return true;
}

bool HttpContext::parseRequestBody(const char* byte_buffer, int& start, const int end) {
	int len = atoi(request_.getHeader("Content-Length").c_str());
	if (len > end - start) {
		return false;
	} else {
		std::string body(byte_buffer + start, len);
		request_.setBody(body);
		start += len;
	}
}

int HttpContext::parseRequest(char* byte_buffer, const int n) {
	int start_index = 0;
	int line_end_index = 0;
	PARSE_LINE_STATUS parse_line_status;
	while (parse_status_ == PARSE_REQUEST_BODY || (parse_line_status = parseLine(byte_buffer, line_end_index, n)) == PARSE_LINE_OK) {

		DEBUG << "status " << parse_status_;
		DEBUG << "start_index => " << start_index;
		DEBUG << "line_end_index => " << line_end_index;

		char* line = byte_buffer + start_index;
		start_index = line_end_index;

		switch (parse_status_) {
			case PARSE_REQUEST_LINE:

				if (parseRequestLine(line)) {
					parse_status_ = PARSE_REQUEST_HEADER;
				} else {
					return -1;
				}
				break;

			case PARSE_REQUEST_HEADER:

				if (line[0] == '\0' && line[1] == '\0') {
					if ((request_.getHeader("Content-Length")).empty()) {
						DEBUG << "HTTP -----------------------> ";
						parse_status_ = PARSE_REQUEST_LINE;
					} else {
						parse_status_ = PARSE_REQUEST_BODY;
					}
				} else {
					if (parseRequestHeader(line) == false) {
						return -1;	
					} 
				}
				
				break;

			case PARSE_REQUEST_BODY:

				if (parseRequestBody(byte_buffer, start_index, n)) {
					DEBUG << "HTTP -----------------------> ";
					parse_status_ = PARSE_REQUEST_LINE;
				} else {
					return -1;
				}
				
				break;

			default:

				return -1;
		}

	}

	if (parse_line_status == PARSE_LINE_BAD) {
		return -1;
	}

	return start_index;

}














