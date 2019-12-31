#ifndef CRAWLER_BLOG_CRAWLER_H
#define CRAWLER_BLOG_CRAWLER_H

#include <boost/asio/ssl.hpp>
#include <vector>

#include "Crawler.h"

namespace crawler
{

namespace blog
{

class BlogCrawler
	: public Crawler
{
public:
	BlogCrawler();
	virtual ~BlogCrawler();

	bool Crawl() override;

protected:
	boost::asio::ssl::context ctx_;

private:
	std::vector<std::string> GetArticlesUrl();

	virtual bool RequestForGettingArticles() = 0;
	virtual bool InsertArticles() = 0;

	virtual const char* const GetHost() = 0;
	virtual const char* const GetArchivePath() = 0;
	virtual const char* const GetSelectorForArchivesTag() = 0;
	virtual const char* const GetSelectorForUrlTag() = 0;
	virtual const char* const GetAttributeNameForUrl() = 0;

	virtual void CatchExceptionalUrlCase(std::string& url);

};

}

}

#endif