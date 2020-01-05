#include "stdafx.h"
#include "HttpKeepAliveClient.h"

#include <boost/bind.hpp>

#include "HttpDefine.h"

namespace util
{

HttpKeepAliveClient::HttpKeepAliveClient(
	boost::asio::io_context& io_context,
	boost::asio::ssl::context& context,
	const tcp::resolver::results_type& endpoints,
	int responseFirstBufSize)
	: socket_(io_context, context),
	requestNum_(0),
	responseNum_(0),
	responseBufOffset_(0),
	responseBuf_(GetResponseBuf())
{
	response_.prepare(responseFirstBufSize);
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
	auto responseSize = response_.size();
	out.reserve(responseSize);

	auto baseOffset = GetResponseBuf();
	auto limit = baseOffset + responseSize;
	auto startOffset = const_cast<char*>(baseOffset);
	auto offset = startOffset;

	while (offset + 10 < limit)
	{
		if (std::strncmp(offset, "\n00001305\r\n", 11) == 0)
		{
			out.append(startOffset, offset - 1);
			startOffset = offset + 11;
		}
		else if (std::strncmp(offset, "\n00004000\r\n\n", 12) == 0)
		{
			out.append(startOffset, offset - 1);
			startOffset = offset + 12;
		}
		else if (std::strncmp(offset, "\n00006000\r\n\n", 12) == 0)
		{
			out.append(startOffset, offset - 1);
			startOffset = offset + 12;
		}
		else if (std::strncmp(offset, "\n00004000\r\n", 11) == 0)
		{
			out.append(startOffset, offset - 1);
			startOffset = offset + 11;
		}
		else if (std::strncmp(offset, "\n00006000\r\n", 11) == 0)
		{
			out.append(startOffset, offset - 1);
			startOffset = offset + 11;
		}

		++offset;
	}
	out.append(startOffset, offset + 10);
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
			while (responseBuf_ < findHtmlOffset
				&& std::strncmp("</html>", findHtmlOffset - 7, 7)
				&& std::strncmp("</body>", findHtmlOffset - 7, 7))
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
				// close socket keep alive end.
				boost::system::error_code ec;
				socket_.shutdown(ec);
				return;
			}
		}

		boost::asio::async_read(socket_,
			response_,
			boost::asio::transfer_at_least(1),
			boost::bind(&HttpKeepAliveClient::Read, this, boost::asio::placeholders::error));

	}

}

}