#include "stdafx.h"
#include "BlogCrawler.h"
#include <Node.h>

#include "BlogArticleDao.h"
#include "HttpClient.h"

namespace crawler
{

namespace blog
{

BlogCrawler::BlogCrawler()
	: pageIndex_(1),
	requestPageNumberDegree_(10),
	ctx_(boost::asio::ssl::context::sslv23),
	blogArticleDao_(nullptr)
{
	ctx_.set_verify_mode(boost::asio::ssl::verify_none);
}

BlogCrawler::~BlogCrawler()
{
}

bool BlogCrawler::Crawl()
{
	pageIndex_ = 1;

	while (true)
	{
		auto pageSiteInfos = GetPageSiteInfos();

		if (pageSiteInfos.size() < requestPageNumberDegree_)
		{
			break;
		}

		if (!GetAndInsertArticles(pageSiteInfos))
		{
			return false;
		}
	}

	return true;
}

void BlogCrawler::SetDao(void* dao)
{
	blogArticleDao_ = *(static_cast<std::shared_ptr<dao::BlogArticleDao>*>(dao));
}

std::string BlogCrawler::DownloadImage(std::string& url)
{
	return std::string();
}

Site BlogCrawler::RequestAndGetDoc(std::string path)
{
	boost::asio::io_context ioContextToGetArticles;
	tcp::resolver resolver(ioContextToGetArticles);
	tcp::resolver::query query(GetHost(), "https");
	auto endpoints = resolver.resolve(query);

	auto httpClient = std::make_unique<util::HttpClient>(ioContextToGetArticles, ctx_, endpoints, GetHost(), path.c_str());

	ioContextToGetArticles.run();

	auto statusCode = httpClient->GetStatusCode();
	if (statusCode != util::HttpClient::kResponseOK)
	{
		return { path, nullptr };
	}

	auto bufPerClient = httpClient->GetResponseBuf();
	auto sizePerClient = httpClient->GetResponseSize();

	std::string pagePerClient(bufPerClient, sizePerClient);
		 
	auto htmlDocument = std::make_unique<HtmlDocument>();;
	htmlDocument->parse(pagePerClient);
	
	return { path, std::move(htmlDocument) };
}

SiteInfo BlogCrawler::RequestAndGetDoc(UrlList& urls)
{
	auto urlSize = urls.size();

	std::vector<std::unique_ptr<util::HttpClient>> httpClients;
	httpClients.reserve(urlSize);

	for (auto i = 0; i < urlSize; ++i)
	{
		boost::asio::io_context ioContextToGetArticles;
		tcp::resolver resolver(ioContextToGetArticles);
		tcp::resolver::query query(GetHost(), "https");
		auto endpoints = resolver.resolve(query);

		auto path = urls.at(i).c_str();

		httpClients.emplace_back(
			std::make_unique<util::HttpClient>(ioContextToGetArticles, ctx_, endpoints, GetHost(), path)
		);

		ioContextToGetArticles.run();
	}

	SiteInfo archiveInfos;

	for (auto i = 0; i < urlSize; ++i)
	{
		auto& client = httpClients.at(i);
		auto bufPerClient = client->GetResponseBuf();
		auto sizePerClient = client->GetResponseSize();

		std::string pagePerClient(bufPerClient, sizePerClient);
		archiveInfos.emplace_back(urls.at(i), std::make_unique<HtmlDocument>());

		auto& htmlDocument = archiveInfos.at(i).second;
		htmlDocument->parse(pagePerClient);
	}

	return archiveInfos;
}

std::unique_ptr<HtmlDocument> BlogCrawler::GetMainDocument()
{
	auto host = GetHost();

	boost::asio::io_context ioContext;
	tcp::resolver resolver(ioContext);
	tcp::resolver::query query(host, "https");
	auto endpoints = resolver.resolve(query);

	util::HttpClient httpClient(ioContext, ctx_, endpoints, host, GetIndexPath());
	ioContext.run();

	auto buf = httpClient.GetResponseBuf();
	size_t size = httpClient.GetResponseSize();

	std::string page(buf, size);
	auto htmlDocument = std::make_unique<HtmlDocument>();
	htmlDocument->parse(page);

	return htmlDocument;
}

}

}