#ifndef UTIL_HTTP_KEEP_ALIVE_CLIENT_H
#define UTIL_HTTP_KEEP_ALIVE_CLIENT_H

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <istream>

using boost::asio::ip::tcp;

namespace util
{

struct HtmlBodyInfo
{
	HtmlBodyInfo() {}
	HtmlBodyInfo(char* body, int size) : body_(body), size_(size) {}
	char* body_;
	int size_;
};

using HtmlBodyInfoList = std::vector<HtmlBodyInfo>;

class HttpKeepAliveClient
{
public:
	HttpKeepAliveClient(boost::asio::io_context& io_context,
		boost::asio::ssl::context& context,
		const tcp::resolver::results_type& endpoints,
		int responseFirstBufSize = 80000000);
	~HttpKeepAliveClient();

	void Send(const char* host, const char* path);

	HtmlBodyInfoList GetHtmlBodyInfos() const;
	
	size_t GetResponseSize() const;

	void GetResponses(std::string & out);

private:
	void Connect(const tcp::resolver::results_type& endpoints);
	void Handshake();
	void SendRequest();
	void Read(const boost::system::error_code& error);

	const char* GetResponseBuf() const;

	boost::asio::ssl::stream<tcp::socket> socket_;
	boost::asio::streambuf request_;
	boost::asio::streambuf response_;
	int requestNum_;
	int responseNum_;
	int responseBufOffset_;
	const char* responseBuf_;

	HtmlBodyInfoList htmlBodyInfos_;
};

}

#endif