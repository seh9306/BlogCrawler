#include "stdafx.h"
#include "DevMicrosoftBlogCrawler.h"

#include <Document.h>
#include <Node.h>

#include "Article.h"
#include "BlogArticleDao.h"
#include "ProgressDefine.h"

namespace {

const char* const kHost = u8"devblogs.microsoft.com";
const char* const kIndexPath = u8"/oldnewthing/";
const char* const kPagePath = u8"page/";

// css selector
const char* const kSelectorForArchivesTag = u8"div.listdisplay li";
const char* const kSelectorForUrlTag = u8"a";
const char* const kSelectorForLastPageTag = u8"li.page-item > a.page-link";
const char* const kSelectorForArticleUrlTag = u8"#most-recent article header > h5 > a";
const char* const kSelectorForTitle = u8"#featured > div > h1";
const char* const kSelectorForDate = u8"#featured > div > div.entry-meta > p";

const char* const kDateFormat = "%Y-%m-%d %H:%M:%S";

constexpr int workerNum = 6;

}

namespace crawler
{

namespace blog
{

DevMicrosoftBlogCrawler::DevMicrosoftBlogCrawler()
	: lastPageNumber_(0)
{
}

DevMicrosoftBlogCrawler::~DevMicrosoftBlogCrawler()
{
}

const char* const DevMicrosoftBlogCrawler::GetHost() const
{
	return kHost;
}

const char* const DevMicrosoftBlogCrawler::GetIndexPath() const
{
	return kIndexPath;
}

const char* const DevMicrosoftBlogCrawler::GetSelectorForArchivesTag() const
{
	return kSelectorForArchivesTag;
}

const char* const DevMicrosoftBlogCrawler::GetSelectorForUrlTag() const
{
	return kSelectorForUrlTag;
}

const char* const DevMicrosoftBlogCrawler::GetAttributeNameForUrl() const
{
	return kUrlAttribute;
}

SiteInfo DevMicrosoftBlogCrawler::GetArticleSiteInfos(SiteInfo& pageInfos)
{
	SiteInfo siteInfos;
	std::mutex mutex;
	std::vector<std::thread> workers;
	workers.reserve(workerNum);
	auto quotaPage = pageInfos.size() / workerNum;

	for (int i = 0; i < workerNum; ++i)
	{
		std::thread worker([&](int id) 
		{			
			UrlList articleUrls;
			auto start = id * quotaPage + 1;
			auto end = id + 1 == workerNum ? pageInfos.size() : (id + 1) * quotaPage + 1;

			for (; start < end; ++start)
			{
				auto& pageDoc = pageInfos.at(start).second;
				auto articlesSelector = pageDoc->find(kSelectorForArticleUrlTag);
				auto articleNum = articlesSelector.nodeNum();

				for (int i = 0; i < articleNum; ++i)
				{
					auto& articleDoc = articlesSelector.nodeAt(i);
					auto urlTagSelector = articleDoc.find(kSelectorForArticleUrlTag);
					if (urlTagSelector.nodeNum() == 0)
					{
						continue;
					}
					auto urlTag = urlTagSelector.nodeAt(0);
					auto url = urlTag.attribute(kUrlAttribute);

					ModifyWrongUrl(url);
					articleUrls.emplace_back(url);
				}
			}

			Notify(observer::kCompleteDevMicrosoftBlogMakeArticleUrls);

			auto partSiteInfos = RequestAndGetDoc(articleUrls);

			Notify(observer::kCompleteDevMicrosoftBlogParseArticles);

			std::lock_guard<std::mutex> guard(mutex);
			for (auto& siteInfo : partSiteInfos)
			{
				siteInfos.emplace_back(std::move(siteInfo));
			}

		}, i);

		workers.emplace_back(std::move(worker));
	}

	for (auto& worker : workers)
	{
		worker.join();
	}

	Notify(observer::kCompleteDevMicrosoftBlogRequestAndGetArticles);

	return siteInfos;
}

SiteInfo DevMicrosoftBlogCrawler::GetPageSiteInfos()
{
	int lastPageNumber = GetLastPageNumber();
	
	SiteInfo siteInfos;
	std::mutex mutex;
	std::vector<std::thread> workers;
	workers.reserve(workerNum);
	auto quotaPage = lastPageNumber / workerNum;

	for (int i = 0; i < workerNum; ++i)
	{
		std::thread worker([&](int id) 
		{
			UrlList pageUrls;
			auto start = id * quotaPage + 1;
			auto end = (id + 1 == workerNum ? lastPageNumber : (id + 1) * quotaPage) + 1;

			for (;start < end; ++start)
			{
				std::string pageUrl(kIndexPath);
				pageUrl.append(kPagePath);
				pageUrl.append(std::to_string(start));

				pageUrls.emplace_back(pageUrl);
			}

			Notify(observer::kCompleteDevMicrosoftBlogMakePageUrls);

			auto partSiteInfos = RequestAndGetDoc(pageUrls);

			Notify(observer::kCompleteDevMicrosoftBlogParsePages);

			std::lock_guard<std::mutex> guard(mutex);
			for (auto& siteInfo : partSiteInfos)
			{
				siteInfos.emplace_back(std::move(siteInfo));
			}

		}, i);

		workers.emplace_back(std::move(worker));
	}

	for (auto& worker : workers)
	{
		worker.join();
	}
	
	Notify(observer::kCompleteDevMicrosoftBlogRequestAndGetPages);

 	return siteInfos;
}

int DevMicrosoftBlogCrawler::GetLastPageNumber()
{
	if (lastPageNumber_ == 0)
	{
		auto mainDoc = GetMainDocument();

		auto lastPageSelector = mainDoc->find(kSelectorForLastPageTag);
		if (lastPageSelector.nodeNum() != 0)
		{
			auto lastPageTag = lastPageSelector.nodeAt(0);
			lastPageNumber_ = std::stoi(lastPageTag.text());
		}
	}

	return lastPageNumber_;
}

bool DevMicrosoftBlogCrawler::GetAndInsertArticles(SiteInfo& pageSiteInfos)
{
	auto articleSites = GetArticleSiteInfos(pageSiteInfos);

	model::ArticleList articles;

	for (auto& articleInfo : articleSites)
	{
		auto& articleDoc = articleInfo.second;
		if (articleDoc.get() == nullptr)
		{
			continue;
		}
		auto titleSelector = articleDoc->find(kSelectorForTitle);
		if (titleSelector.nodeNum() == 0)
		{
			continue;
		}

		auto articleSelector = articleDoc->find(kArticleTagName);
		if (articleSelector.nodeNum() == 0)
		{
			continue;
		}

		auto dateSelector = articleDoc->find(kSelectorForDate);
		if (articleSelector.nodeNum() == 0)
		{
			continue;
		}

		if (titleSelector.nodeNum() == 0)
		{
			continue;
		}
		auto titleNode = titleSelector.nodeAt(0);

		if (articleSelector.nodeNum() == 0)
		{
			continue;
		}

		auto articleNode = articleSelector.nodeAt(0);

		if (dateSelector.nodeNum() == 0)
		{
			continue;
		}

		auto dateNode = dateSelector.nodeAt(0);

		auto date = GetDateFromString(articleInfo.first, kDateFormat);

		auto imgTagSelection = articleNode.find(kImgTagName);

		std::string imagePath;
		if (imgTagSelection.nodeNum() != 0)
		{
			auto url = imgTagSelection.nodeAt(0).attribute(kSrcAttribute);
			ModifyWrongUrl(url);
			imagePath = DownloadImage(url);
		}

		articles.emplace_back(titleNode.text(), articleInfo.first, imagePath, articleNode.text());
	}

	Notify(observer::kCompleteDevMicrosoftBlogMappingArticles);

	if (!blogArticleDao_->InsertArticles(articles))
	{
		return false;
	}

	Notify(observer::kCompleteDevMicrosoftBlogInsertArticles);

	return true;
}

}

}