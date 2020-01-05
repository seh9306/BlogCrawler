#include "stdafx.h"
#include "HttpKeepAliveClient.h"

#include <boost/bind.hpp>

namespace
{

constexpr char* const kHttpVersion = "HTTP/1.1";
constexpr char* const kDoctype = "<!DOCTYPE html>";

constexpr int kDoctypeSize = 15;

}

namespace util
{

HttpKeepAliveClient::HttpKeepAliveClient(
	boost::asio::io_context& io_context,
	boost::asio::ssl::context& context,
	const tcp::resolver::results_type& endpoints)
	: socket_(io_context, context),
	requestNum_(0),
	responseNum_(0),
	responseBufOffset_(0),
	responseBuf_(GetResponseBuf())
{
	response_.prepare(500000000);
	Connect(endpoints);
}

HttpKeepAliveClient::~HttpKeepAliveClient()
{
}

void HttpKeepAliveClient::Send(const char* host, const char* path)
{
	++requestNum_;
	std::ostream request_stream(&request_);
	request_stream << "GET " << path << " " << kHttpVersion << "\r\n";
	request_stream << "Host: " << host << "\r\n";
	request_stream << "Accept: */*\r\n";
	request_stream << "Connection: Keep-Alive\r\n\r\n";
}

void HttpKeepAliveClient::Close(const char* host, const char* path)
{
	++requestNum_;
	std::ostream request_stream(&request_);
	request_stream << "GET " << path << " " << kHttpVersion << "\r\n";
	request_stream << "Host: " << host << "\r\n";
	request_stream << "Accept: */*\r\n";
	request_stream << "Connection: Keep-Alive\r\n\r\n";
}

const char* HttpKeepAliveClient::GetResponseBuf() const
{
	return boost::asio::buffer_cast<const char*>(response_.data());
}

HtmlBodyInfoList HttpKeepAliveClient::GetHtmlBodyInfos() const
{
	return htmlBodyInfos_;
}

size_t HttpKeepAliveClient::GetResponseSize() const
{
	return response_.size();
}

void HttpKeepAliveClient::GetResponseLine(std::string& out)
{
	out = { boost::asio::buffers_begin(response_.data()),
		  boost::asio::buffers_end(response_.data()) };
}

void HttpKeepAliveClient::Connect(const tcp::resolver::results_type& endpoints)
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

void HttpKeepAliveClient::Handshake()
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

void HttpKeepAliveClient::SendRequest()
{
	boost::asio::async_write(socket_,
		request_,
		[this](const boost::system::error_code& error, std::size_t length)
	{
		if (!error)
		{
			boost::asio::async_read_until(socket_, response_, "\r\n\r\n",
				boost::bind(&HttpKeepAliveClient::Read, this, boost::asio::placeholders::error));
		}
	});
}

void HttpKeepAliveClient::Read(const boost::system::error_code& error)
{
	if (!error)
	{
		auto size = GetResponseSize();
		responseBuf_ = GetResponseBuf();

		while (size > responseBufOffset_)
		{
			if (std::strncmp("HTTP/1.1", responseBuf_ + responseBufOffset_, 8) == 0)
			{
				HtmlBodyInfo htmlBodyInfo;
				htmlBodyInfo.body_ = const_cast<char*>(responseBuf_) + responseBufOffset_;
				htmlBodyInfos_.push_back(htmlBodyInfo);

				++responseNum_;
			}
			++responseBufOffset_;
		}

		if (requestNum_ == responseNum_)
		{

			auto isEnd = true;
			auto findHtmlBase = const_cast<char*>(responseBuf_) + size;
			auto findHtmlOffset = findHtmlBase;
			while (std::strncmp("</html>", findHtmlOffset - 7, 7))
			{
				if (findHtmlBase - findHtmlOffset > 500)
				{
					isEnd = false;
					break;
				}
				findHtmlOffset--;
			}

			if (isEnd)
			{
				for (int i = 0; i < htmlBodyInfos_.size(); ++i)
				{
					auto& htmlBodyInfo = htmlBodyInfos_.at(i);
					auto offset = htmlBodyInfo.body_;
					offset += 9;
					if (std::strncmp("200", offset, 3) == 0)
					{
						while (std::strncmp("<body", offset, 5))
						{
							++offset;
						}

						htmlBodyInfo.body_ = offset;
						if (htmlBodyInfos_.size() - 1 != i)
						{
							offset = htmlBodyInfos_.at(i + 1).body_;
						}
						else
						{
							offset = const_cast<char*>(responseBuf_) + size;
						}

						while (std::strncmp("</body>", offset - 7, 7))
						{
							--offset;
						}
						*(offset) = NULL;
					}
					else
					{
						*(const_cast<char*>(responseBuf_) + responseBufOffset_ + 3) = NULL;
						htmlBodyInfos_.emplace_back(const_cast<char*>(responseBuf_) + responseBufOffset_, 3);
					}
				}

				boost::system::error_code ec;
				socket_.shutdown(ec);
			}
			else
			{
				boost::asio::async_read(socket_,
					response_,
					boost::asio::transfer_at_least(1),
					boost::bind(&HttpKeepAliveClient::Read, this, boost::asio::placeholders::error));
			}
		}
		else
		{
			boost::asio::async_read(socket_,
				response_,
				boost::asio::transfer_at_least(1),
				boost::bind(&HttpKeepAliveClient::Read, this, boost::asio::placeholders::error));
		}
	}
	else if (error == boost::asio::error::eof)
	{

		/*
		auto constBuf = GetResponseBuf();
		auto size = GetResponseSize();
		char* buf = const_cast<char*>(GetResponseBuf());
		
		while (size > buf - constBuf)
		{
			if (std::strncmp("HTTP/1.1", buf, 8) == 0)
			{
				buf += 9;
				if (std::strncmp("200", buf, 3) == 0) 
				{
					while (std::strncmp("<body", buf, 5))
					{
						++buf;
					}

					char* endOfBody = buf;
					while (std::strncmp("</body>", endOfBody - 7, 7))
					{
						++endOfBody;
					}
					*endOfBody = NULL;

					htmlBodyInfos_.emplace_back(buf, static_cast<int>(endOfBody - buf));

					buf = endOfBody;
				}
				else
				{
					*(buf + 3) = NULL;
					htmlBodyInfos_.emplace_back(buf, 3);
				}
			}
			++buf;
		}*/
	}
	else
	{
	boost::asio::async_read(socket_,
		response_,
		boost::asio::transfer_at_least(1),
		boost::bind(&HttpKeepAliveClient::Read, this, boost::asio::placeholders::error));
	}
}

}