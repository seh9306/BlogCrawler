#ifndef CRAWLER_HERBSUTTER_BLOG_CRAWLER_H
#define CRAWLER_HERBSUTTER_BLOG_CRAWLER_H

#include "BlogCrawler.h"

namespace {
	constexpr char* const kHost = "herbsutter.com";
	constexpr char* const kArchivePath = "/";
}

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
	bool RequestForGettingArchives() override;
	bool RequestForGettingArticles() override;
	bool InsertArticles() override;
};

}

}

#endif