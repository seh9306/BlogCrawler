#include "stdafx.h"
#include "HerbsutterBlogCrawler.h"

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
	tcp::resolver resolver(ioContext_);
	tcp::resolver::query query(kHost, "https");
	auto endpoints = resolver.resolve(query);

	util::HttpClient httpClient(ioContext_, ctx_, endpoints, kHost, kArchivePath);
	ioContext_.run();

	int statusCode = httpClient.GetResponseCode();

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