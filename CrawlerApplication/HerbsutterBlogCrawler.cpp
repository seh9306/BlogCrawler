#include "stdafx.h"
#include "HerbsutterBlogCrawler.h"

#include <Document.h>
#include <Node.h>

namespace {
	constexpr char* const kHost = "herbsutter.com";
	constexpr char* const kArchivePath = "/";

	// css selector
	constexpr char* const kSelectorForArchivesTag = "#archives-4 > ul > li";
	constexpr char* const kSelectorForUrlTag = "a";

	// tag name
	constexpr char* const kArticleTagName = "article";

	// tag attribute
	constexpr char* const kUrlAttribute = "href";

	//constexpr bool kShouldCrawlerRequestEachArticle = false;
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

const char* const HerbsutterBlogCrawler::GetArchivePath() const
{
	return kArchivePath;
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

bool HerbsutterBlogCrawler::GetAndInsertArticles(HtmlDocList& htmlDocs)
{
	for (auto& htmlDoc : htmlDocs)
	{
		auto articleSelections = htmlDoc->find(kArticleTagName);
		auto articleNum = articleSelections.nodeNum();

		for (auto i = 0; i < articleNum; ++i)
		{
			auto article = articleSelections.nodeAt(i);
		}
	}

	return true;
}

void HerbsutterBlogCrawler::CatchExceptionalUrlCase(std::string & url)
{
	/* Do nothing in this class */
}

}

}