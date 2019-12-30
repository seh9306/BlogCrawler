#ifndef CRAWLER_BLOGCRAWLER_H
#define CRAWLER_BLOGCRAWLER_H

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <vector>

#include "Article.h"
#include "Crawler.h"

using boost::asio::ip::tcp;

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
	std::vector<model::Article> articles;

	boost::asio::io_context ioContext_;
	boost::asio::ssl::context ctx_;
	tcp::resolver resolver_;

private:
	virtual bool RequestForGettingArchives() = 0;
	virtual bool RequestForGettingArticles() = 0;
	virtual bool InsertArticles() = 0;
};

}

}

#endif