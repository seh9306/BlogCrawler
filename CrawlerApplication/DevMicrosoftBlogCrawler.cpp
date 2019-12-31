#include "stdafx.h"
#include "DevMicrosoftBlogCrawler.h"

namespace {
	constexpr char* const kHost = "devblogs.microsoft.com";
	constexpr char* const kArchivePath = "/oldnewthing/";

	// css selector
	constexpr char* const kSelectorForArchivesTag = "div.listdisplay li"; // "#wrapper-footer > div:nth-child(1) > div > div:nth-child(1) > div > div > div > li";
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

DevMicrosoftBlogCrawler::DevMicrosoftBlogCrawler()
{
}

DevMicrosoftBlogCrawler::~DevMicrosoftBlogCrawler()
{
}

bool DevMicrosoftBlogCrawler::RequestForGettingArticles()
{
	return false;
}

bool DevMicrosoftBlogCrawler::InsertArticles()
{
	return false;
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