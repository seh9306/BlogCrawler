#ifndef UTIL_HTTP_DEFINE_H
#define UTIL_HTTP_DEFINE_H

namespace util
{

constexpr char* const kHttpVersion = "HTTP/1.1";
constexpr char* const kDoctype = "<!DOCTYPE html>";

constexpr int kDoctypeSize = 15;

enum StatusCode {
	kFail = 0,

	kResponseOK = 200,
};

}

#endif