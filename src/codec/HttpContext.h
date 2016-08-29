/*========================================================
 *
 *   文件名称 ：HttpContext.h
 *   创建日期 ：2016.8.6
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#ifndef __HTTP_CONTEXT_H__
#define __HTTP_CONTEXT_H__

#include "HttpRequest.h"
#include "HttpResponse.h"
#include "RingBuffer.h"
#include "Channel.h"
#include <string>
#include <functional>

namespace mars {

typedef enum {
	PARSE_LINE_OK    		= 0,
	PARSE_LINE_CONTINUE		= 1,
	PARSE_LINE_BAD 			= 2,
} PARSE_LINE_STATUS;

typedef enum {
	BAD = 0,
	OK  = 1,
} HTTP_CODE;

typedef enum {
	PARSE_REQUEST_LINE   = 0,
	PARSE_REQUEST_HEADER = 1,
	PARSE_REQUEST_BODY   = 2,
} PARSE_STATUS;

class HttpContext {

    typedef std::function<void(const ChannelPtr&)> HttpCallback;

public:
	HttpContext();
	~HttpContext();

	void setHttpCallback(const HttpCallback& cb) { http_cb_ = cb; }

	RingBuffer* encode(const HttpResponse& response);
	bool decode(const ChannelPtr& channel_ptr);

	const HttpRequest& getRequest() const { return request_; }


	PARSE_LINE_STATUS parseLine(char* byte_buffer, int& start_index, const int end_index);
	bool parseRequestLine(char* line);
	bool parseRequestHeader(char* line);
	bool parseRequestBody(const char* byte_buffer, int& start, const int end);
	int  parseRequest(const ChannelPtr& channel_ptr, char* byte_buffer, const int n);

private:
	HttpRequest  request_;
	HttpResponse response_;
	PARSE_STATUS parse_status_;

	HttpCallback http_cb_;
};

}

#endif
