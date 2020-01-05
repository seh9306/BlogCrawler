#include "stdafx.h"
#include "BlogCrawler.h"
#include <Node.h>

#include "BlogArticleDao.h"
#include "HttpClient.h"
#include "HttpKeepAliveClient.h"

namespace {

constexpr int kRequestPageNumberDegree = 5;

}

namespace crawler
{

namespace blog
{

BlogCrawler::BlogCrawler()
	: pageIndex_(1),
	requestPageNumberDegree_(kRequestPageNumberDegree),
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
	int notifyCount = 50;

	while (true)
	{
		auto pageSiteInfos = GetPageSiteInfos();

		/*if (pageSiteInfos.size() < requestPageNumberDegree_)
		{
			break;
		}*/

		if (!GetAndInsertArticles(pageSiteInfos))
		{
			return false;
		}

		Notify(pageIndex_);
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
	boost::asio::io_context ioContextToGetArticles;
	tcp::resolver resolver(ioContextToGetArticles);
	tcp::resolver::query query(GetHost(), "https");
	auto endpoints = resolver.resolve(query);

	util::HttpKeepAliveClient httpKeepAliveClient(ioContextToGetArticles, ctx_, endpoints);

	for (auto url : urls)
	{
		httpKeepAliveClient.Send(GetHost(), url.c_str());
	}
	
	ioContextToGetArticles.run();

	std::string stringForResponse;
	httpKeepAliveClient.GetResponseLine(urls, stringForResponse);

	
	/**/

	return GetHtmlBody(stringForResponse);
}

SiteInfo BlogCrawler::GetHtmlBody(UrlList& urls, std::string stringForResponse)
{
	auto buffer = stringForResponse.c_str();

	SiteInfo siteInfo;
	int i = 0;
	for (; i < htmlBodyInfos.size(); ++i)
	{
		auto htmlBodyInfo = htmlBodyInfos.at(i);
		auto htmlDocument = std::make_unique<HtmlDocument>();;
		htmlDocument->parse(htmlBodyInfo.body_);
		siteInfo.emplace_back(urls.at(i), std::move(htmlDocument));
	}

	if (i != urls.size())
	{
		urls.erase(urls.begin(), urls.begin() + i);
		auto remainSiteInfo = RequestAndGetDoc(urls);
	}

	return SiteInfo();
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