#include "stdafx.h"
#include "HerbsutterBlogCrawler.h"

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

bool HerbsutterBlogCrawler::RequestForGettingArticles()
{
	return false;
}

bool HerbsutterBlogCrawler::InsertArticles()
{
	return false;
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

}

}