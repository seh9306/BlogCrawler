#ifndef CRAWLER_BLOG_HERBSUTTER_BLOG_CRAWLER_H
#define CRAWLER_BLOG_HERBSUTTER_BLOG_CRAWLER_H

#include "BlogCrawler.h"

namespace crawler
{

namespace blog
{

class HerbsutterBlogCrawler :
	public BlogCrawler
{
public:
	HerbsutterBlogCrawler();
	virtual ~HerbsutterBlogCrawler();

private:
	bool RequestForGettingArticles() override;
	bool InsertArticles() override;

	const char* const GetHost() override;
	const char* const GetArchivePath() override;
	const char* const GetSelectorForArchivesTag() override;
	const char* const GetSelectorForUrlTag() override;
	const char* const GetAttributeNameForUrl() override;
};

}

}

#endif