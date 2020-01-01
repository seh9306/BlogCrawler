#include "stdafx.h"
#include "BlogCrawler.h"


#include <Node.h>

#include "HttpClient.h"

namespace crawler
{

namespace blog
{

BlogCrawler::BlogCrawler(std::shared_ptr<dao::BlogArticleDao>& blogArticleDao)
	: blogArticleDao_(blogArticleDao),
	ctx_(boost::asio::ssl::context::sslv23)
{
	ctx_.set_verify_mode(boost::asio::ssl::verify_none);
}

BlogCrawler::~BlogCrawler()
{
}

bool BlogCrawler::Crawl()
{
	auto archiveUrls = GetArchiveUrls();

	if (archiveUrls.empty())
	{
		return false;
	}

	auto archiveDocs = ReqeustAndGetArchiveDocs(archiveUrls);

	if (archiveDocs.empty())
	{
		return false;
	}

	if (GetAndInsertArticles(archiveDocs))
	{
		return false;
	}

	return true;
}

UrlList BlogCrawler::GetArchiveUrls()
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

	for (int i = 0; i < 3/*archiveNum*/; ++i)
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

HtmlDocList BlogCrawler::ReqeustAndGetArchiveDocs(UrlList& urls)
{
	auto urlSize = urls.size();

	std::vector<std::unique_ptr<util::HttpClient>> httpClients;
	httpClients.reserve(urlSize);

	boost::asio::io_context ioContextToGetArticles;
	tcp::resolver resolver(ioContextToGetArticles);
	tcp::resolver::query query(GetHost(), "https");
	auto endpoints = resolver.resolve(query);

	for (auto i = 0; i < urlSize; ++i)
	{
		auto path = urls.at(i).c_str();

		httpClients.emplace_back(
			std::make_unique<util::HttpClient>(ioContextToGetArticles, ctx_, endpoints, GetHost(), path)
		);
	}

	ioContextToGetArticles.run();

	std::vector<std::unique_ptr<Document>> archiveDocs;

	for (auto i = 0; i < urlSize; ++i)
	{
		auto& client = httpClients.at(i);
		auto bufPerClient = client->GetResponseBuf();
		auto sizePerClient = client->GetResponseSize();

		std::string pagePerClient(bufPerClient, sizePerClient);
		archiveDocs.push_back(std::make_unique<Document>());
		archiveDocs.at(i)->parse(pagePerClient);		
	}

	return archiveDocs;
}

}

}