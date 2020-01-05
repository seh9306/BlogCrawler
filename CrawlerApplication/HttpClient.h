#ifndef UTIL_HTTP_CLIENT_H
#define UTIL_HTTP_CLIENT_H

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <istream>

using boost::asio::ip::tcp;

namespace util
{

class HttpClient
{
public:
	HttpClient(boost::asio::io_context& io_context,
		boost::asio::ssl::context& context,
		const tcp::resolver::results_type& endpoints,
		const char* host,
		const char* path);
	~HttpClient();

	const char* GetResponseBuf() const;
	size_t GetResponseSize() const;
	int GetStatusCode() const;

private:
	void Connect(const tcp::resolver::results_type& endpoints);
	void Handshake();
	void SendRequest();
	void ReadHeader(const boost::system::error_code& error);
	void ReadBody(const boost::system::error_code& error);

	bool GetResponseCodeFromHeader(std::istream& header);
	void ConsumeUnnecessaryData();

	boost::asio::ssl::stream<tcp::socket> socket_;
	boost::asio::streambuf request_;
	boost::asio::streambuf response_;

	int statusCode_;
};

}

#endif