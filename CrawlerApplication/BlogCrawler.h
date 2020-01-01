#ifndef CRAWLER_BLOG_CRAWLER_H
#define CRAWLER_BLOG_CRAWLER_H

#include <boost/asio/ssl.hpp>
#include <vector>

#include <Document.h>

#include "Crawler.h"

namespace crawler
{

namespace blog
{

typedef std::vector<std::string> UrlList;
typedef std::vector<std::unique_ptr<Document>> HtmlDocList;

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
	UrlList GetArchiveUrls();
	HtmlDocList ReqeustAndGetArchiveDocs(UrlList& urls);

	// template method
	virtual const char* const GetHost() const = 0;
	virtual const char* const GetArchivePath() const = 0;
	virtual const char* const GetSelectorForArchivesTag() const = 0;
	virtual const char* const GetSelectorForUrlTag() const = 0;
	virtual const char* const GetAttributeNameForUrl() const = 0;

	virtual bool GetAndInsertArticles(HtmlDocList& htmlDocs) = 0;
	virtual void CatchExceptionalUrlCase(std::string& url) = 0;
};

}

}

#endif