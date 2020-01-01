#ifndef CRAWLER_BLOG_DEV_MICROSOFT_BLOG_CRAWLER_H
#define CRAWLER_BLOG_DEV_MICROSOFT_BLOG_CRAWLER_H

#include "BlogCrawler.h"

namespace crawler
{

namespace blog
{

class DevMicrosoftBlogCrawler :
	public BlogCrawler
{
public:
	DevMicrosoftBlogCrawler();
	~DevMicrosoftBlogCrawler();

private:
	const char* const GetHost() const override;
	const char* const GetArchivePath() const override;
	const char* const GetSelectorForArchivesTag() const override;
	const char* const GetSelectorForUrlTag() const override;
	const char* const GetAttributeNameForUrl() const override;

	bool GetAndInsertArticles(HtmlDocList& htmlDocs) override;
	void CatchExceptionalUrlCase(std::string& url) override;
};

}

}

#endif