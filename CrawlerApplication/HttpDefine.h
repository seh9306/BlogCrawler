#ifndef UTIL_HTTP_DEFINE_H
#define UTIL_HTTP_DEFINE_H

namespace util
{

const char* const kDoctype = u8"<!DOCTYPE html>";
const char* const kCloseHtmlTag = u8"</html>";
const char* const kOpenBodyTag = u8"<body";
const char* const kCloseBodyTag = u8"</body>";

const char* const kHttpVersion = u8"HTTP/1.1";
const char* const kHttps = u8"https";

// http request header
const char* const kGet = u8"GET ";
const char* const kHostHeader = u8"Host: ";
const char* const kAcceptAllHeader = u8"Accept: */*";
const char* const kConnectionKeepAliveHeader = u8"Connection: Keep-Alive";
const char* const kConnectionCloseHeader = u8"Connection: close";

// http response header
const char* const kContentLength = u8"content-length";

const char* const kNewLine = u8"\r\n";
const char* const kDoubleNewLine = u8"\r\n\r\n";

constexpr int kDoctypeSize = 15;

enum StatusCode {
	kFail = 0,

	kResponseOK = 200,
};

const char* const kResponseOKStringNumber = u8"200";


}

#endif