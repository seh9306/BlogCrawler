#include "stdafx.h"
#include "BlogCrawler.h"

#include <fstream>
#include <ctime>   
#include <iomanip>
#include <Node.h>

#include "BlogArticleDao.h"
#include "HttpClient.h"
#include "HttpDefine.h"
#include "HttpKeepAliveClient.h"
#include "ProgressDefine.h"

namespace
{

constexpr char* kLocale = "de_DE.utf-8";
constexpr char* kAppendTime = " 00:00:00";

}

namespace crawler
{

namespace blog
{

BlogCrawler::BlogCrawler()
	: ctx_(boost::asio::ssl::context::sslv23),
	blogArticleDao_(nullptr)
{
	ctx_.set_verify_mode(boost::asio::ssl::verify_none);
}

BlogCrawler::~BlogCrawler()
{
}

bool BlogCrawler::Crawl()
{
	auto pageSiteInfos = GetPageSiteInfos();

	if (!GetAndInsertArticles(pageSiteInfos))
	{
		return false;
	}

	Notify(observer::kComplete);

	return true;
}

void BlogCrawler::SetDao(void* dao)
{
	blogArticleDao_ = *(static_cast<std::shared_ptr<dao::BlogArticleDao>*>(dao));
}

std::string BlogCrawler::DownloadImage(std::string& urlPath)
{
	auto protocol = urlPath.find(u8"//");
	if (protocol == std::string::npos)
	{
		return "";
	}

	auto index = urlPath.find(u8'/', protocol + 2);
	if (index == std::string::npos)
	{
		return "";
	}

	auto host = urlPath.substr(protocol + 2, index - (protocol + 2));
	auto path = urlPath.substr(index);

	return RequestAndGetImage(host, path);
}

std::string BlogCrawler::RequestAndGetImage(std::string& host, std::string& urlPath)
{
	boost::asio::io_context ioContextToGetArticles;
	tcp::resolver resolver(ioContextToGetArticles);

	ModifyWrongUrl(host);
	tcp::resolver::query query(host, util::kHttps);
	auto endpoints = resolver.resolve(query);

	auto httpClient = std::make_unique<util::HttpClient>(ioContextToGetArticles, ctx_, endpoints, host.c_str(), urlPath.c_str());

	ioContextToGetArticles.run();

	auto dotIndex = urlPath.find(u8'.');
	auto fileNameStart = urlPath.rfind(u8'/', dotIndex);
	auto fileNameEnd = urlPath.find(u8'?', dotIndex);

	auto realFileName = urlPath.substr(fileNameStart + 1, fileNameEnd - (fileNameStart + 1));
	auto imagePath = std::string(u8"images\\");
	auto fullPath = imagePath.append(realFileName);

	if (httpClient->GetStatusCode() != util::kResponseOK)
	{
		return "";
	}

	auto imageBuf = httpClient->GetResponseBuf();
	auto size = httpClient->GetResponseSize();

	std::ofstream writeFile(fullPath.data(), std::ios::binary);

	if (writeFile.is_open()) 
	{
		writeFile.write(imageBuf, size);
		writeFile.close();
	}

	return imagePath;
}

SiteInfo BlogCrawler::RequestAndGetDoc(UrlList& urls)
{
	boost::asio::io_context ioContextToGetArticles;
	tcp::resolver resolver(ioContextToGetArticles);
	tcp::resolver::query query(GetHost(), util::kHttps);
	auto endpoints = resolver.resolve(query);

	util::HttpKeepAliveClient httpKeepAliveClient(ioContextToGetArticles, ctx_, endpoints);

	for (auto url : urls)
	{
		httpKeepAliveClient.Send(GetHost(), url.c_str());
	}
	
	ioContextToGetArticles.run();

	std::string stringForResponse;
	httpKeepAliveClient.GetResponses(stringForResponse);

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

	if (i != urls.size()
		&& htmlBodyInfos.size() != 0)
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
		if (std::strncmp(util::kHttpVersion, offset, 8) == 0)
		{
			offset += 9;
			if (std::strncmp(util::kResponseOKStringNumber, offset, 3) == 0)
			{
				while (end > offset + 5
					&& std::strncmp(util::kOpenBodyTag, offset, 5))
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
					&& std::strncmp(util::kCloseBodyTag, offset, 7))
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
	UrlList urls;
	
	urls.emplace_back(GetIndexPath());
	auto siteInfo = RequestAndGetDoc(urls);

	return std::move(siteInfo.at(0).second);
}

std::time_t BlogCrawler::GetDate(std::string& url, const char* format)
{
	auto protocol = url.find(u8"//");
	if (protocol == std::string::npos)
	{
		return 0;
	}

	auto index = url.find(u8'/', protocol + 2);
	if (index == std::string::npos)
	{
		return 0;
	}

	auto endindex = url.rfind(u8'/', url.size() - 2);
	if (endindex == std::string::npos)
	{
		return 0;
	}

	auto date = url.substr(index + 1, endindex - index - 1);

	struct std::tm tm;
	std::istringstream ss(date.append(kAppendTime).c_str());
	ss.imbue(std::locale(kLocale));
	ss >> std::get_time(&tm, format);

	return mktime(&tm);
}

std::time_t BlogCrawler::GetDateFromString(std::string& date, const char* format)
{
	auto splitDate = date.substr(43, date.find('-') - 43);

	auto year = splitDate.substr(0, 4);
	auto month = splitDate.substr(4, 2);
	auto day = splitDate.substr(6, 2);

	auto target = year;
	target.append("-");
	target.append(month);
	target.append("-");
	target.append(day);

	struct std::tm tm;
	std::istringstream ss(target.append(kAppendTime).c_str());
	ss.imbue(std::locale(kLocale));
	ss >> std::get_time(&tm, format);

	return mktime(&tm);
}

void BlogCrawler::ModifyWrongUrl(std::string& url)
{
	auto index = url.find("\n");
	if (index == std::string::npos)
	{
		return;
	}

	auto endIndex = url.find("\n", index + 1);

	if (endIndex == std::string::npos)
	{
		url.erase(url.begin() + index);
		return;
	}

	url.erase(url.begin() + index, url.begin() + endIndex);

	index = url.find("\n");
	if (index != std::string::npos)
	{
		ModifyWrongUrl(url);
	}
}

}

}