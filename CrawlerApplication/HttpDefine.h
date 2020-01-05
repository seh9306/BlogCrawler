#ifndef UTIL_HTTP_DEFINE_H
#define UTIL_HTTP_DEFINE_H

namespace util
{


constexpr char* const kDoctype = u8"<!DOCTYPE html>";
constexpr char* const kCloseHtmlTag = u8"</html>";
constexpr char* const kCloseBodyTag = u8"</body>";

constexpr char* const kHttpVersion = "HTTP/1.1";
constexpr char* const kHttps = "https";

// http request header
constexpr char* const kGet = "GET ";
constexpr char* const kHostHeader = "Host: ";
constexpr char* const kAcceptAllHeader = "Accept: */*";
constexpr char* const kConnectionKeepAliveHeader = "Connection: Keep-Alive";
constexpr char* const kConnectionCloseHeader = "Connection: close";

constexpr char* const kNewLine = "\r\n";
constexpr char* const kDoubleNewLine = "\r\n\r\n";

constexpr int kDoctypeSize = 15;

enum StatusCode {
	kFail = 0,

	kResponseOK = 200,
};

}

#endif