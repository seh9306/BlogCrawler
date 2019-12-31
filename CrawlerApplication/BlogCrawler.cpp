#include "stdafx.h"
#include "BlogCrawler.h"

#include <Document.h>
#include <Node.h>

#include "HttpClient.h"

namespace crawler
{

namespace blog
{

BlogCrawler::BlogCrawler()
	: ctx_(boost::asio::ssl::context::sslv23)
{
	ctx_.set_verify_mode(boost::asio::ssl::verify_none);
}

BlogCrawler::~BlogCrawler()
{
}

bool BlogCrawler::Crawl()
{
	auto urls = GetArticlesUrl();

	if (urls.empty())
	{
		return false;
	}

	if (!RequestForGettingArticles())
	{
		return false;
	}
	
	if (!InsertArticles())
	{
		return false;
	}

	return true;
}

std::vector<std::string> BlogCrawler::GetArticlesUrl()
{
	boost::asio::io_context ioContext;
	tcp::resolver resolver(ioContext);
	tcp::resolver::query query(GetHost(), "https");
	auto endpoints = resolver.resolve(query);

	util::HttpClient httpClient(ioContext, ctx_, endpoints, GetHost(), GetArchivePath());
	ioContext.run();

	auto buf = httpClient.GetResponseBuf();
	size_t size = httpClient.GetResponseSize();

	std::string page(buf, size);
	Document doc;
	doc.parse(page);

	CSelection archiveSelection = doc.find(GetSelectorForArchivesTag());
	size_t archiveNum = archiveSelection.nodeNum();

	std::vector<std::string> urls;
	urls.reserve(archiveNum);

	for (int i = 0; i < archiveNum; ++i)
	{
		auto archiveItem = archiveSelection.nodeAt(i);
		auto selectionItem = archiveItem.find(GetSelectorForUrlTag());
		auto urlTag = selectionItem.nodeAt(0);
		auto url = urlTag.attribute(GetAttributeNameForUrl());

		CatchExceptionalUrlCase(url);

		if (!url.empty())
		{
			urls.push_back(url);
		}
	}

	return urls;
}

void BlogCrawler::CatchExceptionalUrlCase(std::string& url)
{
	/* Do nothing */
}

}

}