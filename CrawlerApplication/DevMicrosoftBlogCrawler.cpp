#include "stdafx.h"
#include "DevMicrosoftBlogCrawler.h"

#include <Document.h>
#include <Node.h>

#include "HttpClient.h"

namespace {
	constexpr char* const kHost = u8"devblogs.microsoft.com";
	constexpr char* const kArchivePath = u8"/oldnewthing/";

	// css selector
	constexpr char* const kSelectorForArchivesTag = u8"div.listdisplay li"; // "#wrapper-footer > div:nth-child(1) > div > div:nth-child(1) > div > div > div > li";
	constexpr char* const kSelectorForUrlTag = u8"a";

	// tag name
	constexpr char* const kArticleTagName = u8"article";

	// tag attribute
	constexpr char* const kUrlAttribute = u8"href";

	//constexpr bool kShouldCrawlerEachArticleRequest = true;
}

namespace crawler
{

namespace blog
{

DevMicrosoftBlogCrawler::DevMicrosoftBlogCrawler(
	std::shared_ptr<dao::BlogArticleDao> blogArticleDao)
	: BlogCrawler(blogArticleDao)
{
}

DevMicrosoftBlogCrawler::~DevMicrosoftBlogCrawler()
{
}

const char* const DevMicrosoftBlogCrawler::GetHost() const
{
	return kHost;
}

const char* const DevMicrosoftBlogCrawler::GetArchivePath() const
{
	return kArchivePath;
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

bool DevMicrosoftBlogCrawler::GetAndInsertArticles(HtmlDocList& htmlDocs)
{
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