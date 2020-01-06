#include "stdafx.h"
#include "HerbsutterBlogCrawler.h"

#include <ctime>   
#include <Document.h>
#include <iomanip>
#include <Node.h>

#include "Article.h"
#include "BlogArticleDao.h"
#include "ProgressDefine.h"

namespace 
{

constexpr char* const kHost = u8"herbsutter.com";
constexpr char* const kIndexPath = u8"/";
constexpr char* const kPagePath = u8"page/";

// css selector
constexpr char* const kSelectorForArchivesTag = u8"#archives-4 > ul > li";
constexpr char* const kSelectorForUrlTag = u8"a";
constexpr char* const kSelectorTitleAndUrlTag = u8"header > h1 > a";
constexpr char* const kSelectorContentTag = u8"div.entry-content";
constexpr char* const kSelectorImgTag = u8"img";

constexpr int pageLimit = 10;

constexpr char* const kDateFormat = "%Y/%m/%d %H:%M:%S";

}

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

const char* const HerbsutterBlogCrawler::GetHost() const
{
	return kHost;
}

const char* const HerbsutterBlogCrawler::GetIndexPath() const
{
	return kIndexPath;
}

const char* const HerbsutterBlogCrawler::GetSelectorForArchivesTag() const
{
	return kSelectorForArchivesTag;
}

const char* const HerbsutterBlogCrawler::GetSelectorForUrlTag() const
{
	return kSelectorForUrlTag;
}

const char* const HerbsutterBlogCrawler::GetAttributeNameForUrl() const
{
	return kUrlAttribute;
}

SiteInfo HerbsutterBlogCrawler::GetPageSiteInfos()
{
	UrlList urls;

	int pageIndex = 1;

	SiteInfo pageInfos;

	while (true)
	{
		urls.clear();
		if (pageIndex == 1)
		{
			urls.emplace_back(kIndexPath);
		}

		for (int i = 0; i < pageLimit; ++i)
		{
			std::string path(kIndexPath);
			path.append(kPagePath);
			path.append(std::to_string(++pageIndex));
			path.append(u8"/");

			urls.emplace_back(path);
		}

		Notify(observer::kCompleteHerbsutterBlogMakePageUrls);
		
		auto partSiteInfos = RequestAndGetDoc(urls);

		Notify(observer::kCompleteHerbsutterBlogParsePages);

		for (auto& siteInfo : partSiteInfos)
		{
			pageInfos.emplace_back(std::move(siteInfo.first), std::move(siteInfo.second));
		}
		
		if (partSiteInfos.size() < 10)
		{
			break;
		}	
	}
	
	Notify(observer::kCompleteHerbsutterBlogReqeustAndGetPages);

	return pageInfos;
}

bool HerbsutterBlogCrawler::GetAndInsertArticles(SiteInfo& pageSiteInfos)
{
	model::ArticleList articles;

	for (auto& pageSiteInfo : pageSiteInfos)
	{
		auto articleSelections = pageSiteInfo.second->find(kArticleTagName);
		auto articleNum = articleSelections.nodeNum();

		for (auto i = 0; i < articleNum; ++i)
		{
			auto articleSelection = articleSelections.nodeAt(i);
			auto titleAndUrlTagSelection = articleSelection.find(kSelectorTitleAndUrlTag);
			if (titleAndUrlTagSelection.nodeNum() == 0)
			{
				continue;
			}
			auto titleAndUrlTag = titleAndUrlTagSelection.nodeAt(0);

			model::Article article;

			article.title_ = titleAndUrlTag.text();
			article.url_ = titleAndUrlTag.attribute(kUrlAttribute);

			if (article.url_.empty())
			{
				continue;
			}

			article.date_ = GetDate(article.url_, kDateFormat);

			auto contentTagSelection = articleSelection.find(kSelectorContentTag);

			if (contentTagSelection.nodeNum() == 0) 
			{
				article.content_ = u8"empty";
			}
			else 
			{
				auto contentTag = contentTagSelection.nodeAt(0);

				article.content_ = contentTag.text();
				auto imgTagSelection = contentTag.find(kSelectorImgTag);

				if (imgTagSelection.nodeNum() != 0)
				{
					auto url = imgTagSelection.nodeAt(0).attribute(kSrcAttribute);
					ModifyWrongUrl(url);
					article.imagePath_ = DownloadImage(url);
				}
			}			

			articles.emplace_back(article);
		}
	}

	Notify(observer::kCompleteHerbsutterBlogMappingArticles);

	if (!blogArticleDao_->InsertArticles(articles))
	{
		return false;
	}

	Notify(observer::kCompleteHerbsutterBlogInsertArticles);
	
	return true;
}

}

}