#include "stdafx.h"
#include "DevMicrosoftBlogCrawler.h"

#include <Document.h>
#include <Node.h>

#include "Article.h"
#include "BlogArticleDao.h"

namespace {

constexpr char* const kHost = u8"devblogs.microsoft.com";
constexpr char* const kIndexPath = u8"/oldnewthing/";
constexpr char* const kPagePath = u8"page/";

// css selector
constexpr char* const kSelectorForArchivesTag = u8"div.listdisplay li"; // "#wrapper-footer > div:nth-child(1) > div > div:nth-child(1) > div > div > div > li";
constexpr char* const kSelectorForUrlTag = u8"a";
constexpr char* const kSelectorForLastPageTag = u8"li.page-item > a.page-link";
constexpr char* const kSelectorForArticleUrlTag = u8"#most-recent article header > h5 > a";
constexpr char* const kSelectorForTitle = u8"#featured > div > h1";

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
	UrlList articleUrls;
	for (auto& pageInfo : pageInfos)
	{
		auto& pageDoc = pageInfo.second;
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

			articleUrls.emplace_back(urlTag.attribute(kUrlAttribute));
		}
	}

	/*SiteInfo siteInfo;

	for (auto articleUrl : articleUrls)
	{
		auto site = RequestAndGetDoc(articleUrl);
		siteInfo.emplace_back(site.first, std::move(site.second));
	}*/

	return RequestAndGetDoc(articleUrls);
}

SiteInfo DevMicrosoftBlogCrawler::GetPageSiteInfos()
{
	int lastPageNumber = GetLastPageNumber();
	int goalPageNumber = pageIndex_ + requestPageNumberDegree_;

	if (lastPageNumber + 1 < goalPageNumber)
	{
		goalPageNumber = lastPageNumber;
	}

	UrlList pageUrls;
	for (int i = pageIndex_; i < goalPageNumber; ++i)
	{
		std::string pageUrl(kIndexPath);
		pageUrl.append(kPagePath);
		pageUrl.append(std::to_string(i));

		pageUrls.emplace_back(pageUrl);
	}

	pageIndex_ = goalPageNumber;

	return RequestAndGetDoc(pageUrls);
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

		auto imgTagSelection = articleNode.find(kImgTagName);

		std::string imagePath("");
		if (imgTagSelection.nodeNum() != 0)
		{
			auto url = imgTagSelection.nodeAt(0).attribute(kSrcAttribute);
			imagePath = DownloadImage(url);
		}

		articles.emplace_back(titleNode.text(), articleInfo.first, imagePath, articleNode.text());
	}

	if (!blogArticleDao_->InsertArticles(articles))
	{
		return false;
	}

	return true;
}

void DevMicrosoftBlogCrawler::CatchExceptionalUrlCase(std::string& url)
{
	auto index = url.find("http");
	if (index != 0)
	{
		url.erase(url.begin(), url.begin() + index);
	}
}

}

}