#include "stdafx.h"
#include "HttpClient.h"

#include <boost/bind.hpp>

#include "HttpDefine.h"

namespace util
{

HttpClient::HttpClient(boost::asio::io_context& io_context,
	boost::asio::ssl::context& context,
	const tcp::resolver::results_type& endpoints,
	const char* host,
	const char* path,
	int responseFirstBufSize)
	: socket_(io_context, context),
	statusCode_(StatusCode::kFail)
{
	response_.prepare(responseFirstBufSize);
	std::ostream request_stream(&request_);
	request_stream << kGet << path << " " << kHttpVersion << kNewLine;
	request_stream << kHostHeader << host << kNewLine;
	request_stream << kAcceptAllHeader << kNewLine;
	request_stream << kConnectionCloseHeader << kDoubleNewLine;

	Connect(endpoints);
}

HttpClient::~HttpClient()
{
}

const char* HttpClient::GetResponseBuf() const
{
	return boost::asio::buffer_cast<const char*>(response_.data());
}

size_t HttpClient::GetResponseSize() const
{
	return response_.size();
}

int HttpClient::GetStatusCode() const
{
	return statusCode_;
}

void HttpClient::Connect(const tcp::resolver::results_type& endpoints)
{
	boost::asio::async_connect(socket_.lowest_layer(), endpoints,
		[this](const boost::system::error_code& error,
			const tcp::endpoint& /*endpoint*/)
	{
		if (!error)
		{
			Handshake();
		}
	});
}

void HttpClient::Handshake()
{
	socket_.async_handshake(boost::asio::ssl::stream_base::client,
		[this](const boost::system::error_code& error)
	{
		if (!error)
		{
			SendRequest();
		}
	});
}

void HttpClient::SendRequest()
{
	boost::asio::async_write(socket_,
		request_,
		[this](const boost::system::error_code& error, std::size_t length)
	{
		if (!error)
		{
			boost::asio::async_read_until(socket_, response_, kDoubleNewLine,
				boost::bind(&HttpClient::ReadHeader, this, boost::asio::placeholders::error));
		}
	});
}

void HttpClient::ReadHeader(const boost::system::error_code& error)
{
	if (!error)
	{
		std::istream headerStream(&response_);

		if (!GetResponseCodeFromHeader(headerStream))
		{
			return;
		}

		std::string header;
		while (std::getline(headerStream, header) && header != "\r");

		boost::asio::async_read(socket_,
			response_,
			boost::asio::transfer_at_least(1),
			boost::bind(&HttpClient::ReadBody, this, boost::asio::placeholders::error));
	}
}

void HttpClient::ReadBody(const boost::system::error_code& error)
{
	if (!error)
	{
		boost::asio::async_read(socket_,
			response_,
			boost::asio::transfer_at_least(1),
			boost::bind(&HttpClient::ReadBody, this, boost::asio::placeholders::error));
	}
	else if (error != boost::asio::error::eof)
	{
		statusCode_ = StatusCode::kFail;
	}
	else
	{
		ConsumeUnnecessaryData();
	}
}

bool HttpClient::GetResponseCodeFromHeader(std::istream& headerStream)
{
	std::string protocolAndVersion;
	headerStream >> protocolAndVersion;

	if (protocolAndVersion != kHttpVersion)
	{
		return false;
	}

	headerStream >> statusCode_;
	if (statusCode_ != StatusCode::kResponseOK)
	{
		return false;
	}

	std::string statusMessage;
	headerStream >> statusMessage;

	response_.consume(2);

	return true;
}

void HttpClient::ConsumeUnnecessaryData()
{
	const char* buf = GetResponseBuf();
	while (GetResponseSize() > kDoctypeSize
		&& std::strncmp(kDoctype, buf, kDoctypeSize))
	{
		response_.consume(1);
		buf = GetResponseBuf();
	};
}

}