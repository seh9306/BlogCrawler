#include "stdafx.h"
#include "BlogCrawler.h"

#include <Node.h>

#include "BlogArticleDao.h"
#include "HttpClient.h"
#include "HttpDefine.h"
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

	auto pageSiteInfos = GetPageSiteInfos();

	if (!GetAndInsertArticles(pageSiteInfos))
	{
		return false;
	}

	Notify(pageIndex_);

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
	if (statusCode != util::StatusCode::kResponseOK)
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
	httpKeepAliveClient.GetResponseLine(stringForResponse);

	auto htmlBodyInfos = GetHtmlBody(urls, stringForResponse);

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
		auto& url = urls.at(i);
		if (url.empty())
		{
			++i;
		}
		urls.erase(urls.begin(), urls.begin() + i);

		auto remainSiteInfo = RequestAndGetDoc(urls);
	}

	return siteInfo;
}

util::HtmlBodyInfoList BlogCrawler::GetHtmlBody(UrlList& urls, std::string& stringForResponse)
{
	auto buffer = stringForResponse.c_str();
	auto offset = const_cast<char*>(buffer);
	auto end = buffer + stringForResponse.size();

	util::HtmlBodyInfoList htmlBodyInfos;

	while (end > offset + 8)
	{
		if (std::strncmp("HTTP/1.1", offset, 8) == 0)
		{
			offset += 9;
			if (std::strncmp("200", offset, 3) == 0)
			{
				while (end > offset + 5
					&& std::strncmp("<body", offset, 5))
				{
					++offset;
				}

				if (end <= offset + 5)
				{
					break;
				}

				util::HtmlBodyInfo htmlBodyInfo;
				htmlBodyInfo.body_ = offset;

				while (end > offset + 7
					&& std::strncmp("</body>", offset, 7))
				{
					++offset;
				}

				if (end <= offset + 7)
				{
					break;
				}

				*(offset + 7) = NULL;

				htmlBodyInfos.push_back(htmlBodyInfo);
			}
		}
		++offset;
	}

	auto size = htmlBodyInfos.size();

	return htmlBodyInfos;
}

std::unique_ptr<HtmlDocument> BlogCrawler::GetMainDocument()
{
	UrlList urlList;
	
	urlList.emplace_back(GetIndexPath());
	auto siteInfo = RequestAndGetDoc(urlList);

	return std::move(siteInfo.at(0).second);
}

}

}