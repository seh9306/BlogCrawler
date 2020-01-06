#ifndef UTIL_HTTP_DEFINE_H
#define UTIL_HTTP_DEFINE_H

namespace util
{

constexpr char* const kDoctype = u8"<!DOCTYPE html>";
constexpr char* const kCloseHtmlTag = u8"</html>";
constexpr char* const kOpenBodyTag = u8"<body";
constexpr char* const kCloseBodyTag = u8"</body>";

constexpr char* const kHttpVersion = u8"HTTP/1.1";
constexpr char* const kHttps = u8"https";

// http request header
constexpr char* const kGet = u8"GET ";
constexpr char* const kHostHeader = u8"Host: ";
constexpr char* const kAcceptAllHeader = u8"Accept: */*";
constexpr char* const kConnectionKeepAliveHeader = u8"Connection: Keep-Alive";
constexpr char* const kConnectionCloseHeader = u8"Connection: close";

// http response header
constexpr char* const kContentLength = u8"content-length";

constexpr char* const kNewLine = u8"\r\n";
constexpr char* const kDoubleNewLine = u8"\r\n\r\n";

constexpr int kDoctypeSize = 15;

enum StatusCode {
	kFail = 0,

	kResponseOK = 200,
};

constexpr char* const kResponseOKStringNumber = u8"200";


}

#endif