#include "stdafx.h"
#include "HerbsutterBlogCrawler.h"

#include "Document.h"
#include "Node.h"
#include "HttpClient.h"

namespace crawler
{

namespace blog
{

HerbsutterBlogCrawler::HerbsutterBlogCrawler()
{
}


HerbsutterBlogCrawler::~HerbsutterBlogCrawler()
{
}

bool HerbsutterBlogCrawler::RequestForGettingArchives()
{
	boost::asio::io_context ioContext;
	tcp::resolver resolver(ioContext);
	tcp::resolver::query query(kHost, "https");
	auto endpoints = resolver.resolve(query);

	util::HttpClient httpClient(ioContext, ctx_, endpoints, kHost, kArchivePath);
	ioContext.run();

	int statusCode = httpClient.GetStatusCode();

	if (statusCode != util::HttpClient::kResponseOK)
	{
		return false;
	}

	auto buf = httpClient.GetResponseBuf();
	auto size = httpClient.GetResponseSize();

	return true;
}

bool HerbsutterBlogCrawler::RequestForGettingArticles()
{
	return false;
}

bool HerbsutterBlogCrawler::InsertArticles()
{
	return false;
}

}

}