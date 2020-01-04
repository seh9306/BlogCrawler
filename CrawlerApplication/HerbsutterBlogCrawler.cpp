#include "stdafx.h"
#include "HerbsutterBlogCrawler.h"

#include <Document.h>
#include <Node.h>

#include "Article.h"
#include "BlogArticleDao.h"

namespace {

constexpr char* const kHost = u8"herbsutter.com";
constexpr char* const kIndexPath = u8"/";
constexpr char* const kPagePath = u8"page/";

// css selector
constexpr char* const kSelectorForArchivesTag = u8"#archives-4 > ul > li";
constexpr char* const kSelectorForUrlTag = u8"a";
constexpr char* const kSelectorTitleAndUrlTag = u8"header > h1 > a";
constexpr char* const kSelectorContentTag = u8"div.entry-content";
constexpr char* const kSelectorImgTag = u8"img";

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
	SiteInfo pageInfo;

	pageInfo.emplace_back(kIndexPath, GetMainDocument());
	auto pageIndex = 1;

	while (true)
	{
		std::string path(kIndexPath);
		path.append(kPagePath);
		path.append(std::to_string(++pageIndex));
		path.append("/");

		auto site = RequestAndGetDoc(path);

		if (site.second.get() == nullptr)
		{
			break;
		}

		pageInfo.emplace_back( site.first, std::move(site.second));
	}

	return pageInfo;
}

bool HerbsutterBlogCrawler::GetAndInsertArticles(SiteInfo& HtmlDocuments)
{
	model::ArticleList articles;

	for (auto& HtmlDocument : HtmlDocuments)
	{
		auto articleSelections = HtmlDocument.second->find(kArticleTagName);
		auto articleNum = articleSelections.nodeNum();

		for (auto i = 0; i < articleNum; ++i)
		{
			auto articleSelection = articleSelections.nodeAt(i);
			auto titleAndUrlTagSelection = articleSelection.find(kSelectorTitleAndUrlTag);
			auto titleAndUrlTag = titleAndUrlTagSelection.nodeAt(0);

			model::Article article;

			article.title_ = titleAndUrlTag.text();
			article.url_ = titleAndUrlTag.attribute(kUrlAttribute);

			auto contentTagSelection = articleSelection.find(kSelectorContentTag);
			auto contentTag = contentTagSelection.nodeAt(0);

			article.content_ = contentTag.text();

			auto imgTagSelection = contentTag.find(kSelectorImgTag);

			if (imgTagSelection.nodeNum() != 0)
			{
				auto url = imgTagSelection.nodeAt(0).attribute(kSrcAttribute);
				article.imagePath_ = DownloadImage(url);
			}

			articles.emplace_back(article);
		}
	}

	if (!blogArticleDao_->InsertArticles(articles))
	{
		return false;
	}

	return true;
}

void HerbsutterBlogCrawler::CatchExceptionalUrlCase(std::string& url)
{
	/* Do nothing in this class */
}

}

}