#ifndef CRAWLER_BLOG_CRAWLER_H
#define CRAWLER_BLOG_CRAWLER_H

#include <boost/asio/ssl.hpp>
#include <Document.h>
#include <memory>
#include <vector>

#include "BlogArticleDao.h"
#include "Crawler.h"

namespace crawler
{

namespace blog
{

// tag name
constexpr char* const kArticleTagName = u8"article";
constexpr char* const kImgTagName = u8"img";

// tag attribute
constexpr char* const kUrlAttribute = u8"href";
constexpr char* const kSrcAttribute = u8"src";

using UrlList = std::vector<std::string>;
using HtmlDocument = Document;
using Site = std::pair<std::string, std::unique_ptr<HtmlDocument>>;
using SiteInfo = std::vector<Site>;

class BlogCrawler
	: public Crawler
{
public:
	BlogCrawler(std::shared_ptr<dao::BlogArticleDao>& blogArticleDao);
	virtual ~BlogCrawler();

	bool Crawl() override;

protected:
	std::string DownloadImage(std::string& url);

	std::unique_ptr<HtmlDocument> GetMainDocument();
	Site RequestAndGetDoc(std::string path);
	SiteInfo RequestAndGetDoc(UrlList& urls);

	boost::asio::ssl::context ctx_;
	std::shared_ptr<dao::BlogArticleDao> blogArticleDao_;

private:	
	// template method
	virtual void CatchExceptionalUrlCase(std::string& url) = 0;
	virtual bool GetAndInsertArticles(SiteInfo& HtmlDocuments) = 0;

	virtual const char* const GetHost() const = 0;
	virtual const char* const GetIndexPath() const = 0;
	virtual const char* const GetSelectorForArchivesTag() const = 0;
	virtual const char* const GetSelectorForUrlTag() const = 0;
	virtual const char* const GetAttributeNameForUrl() const = 0;

	virtual SiteInfo GetPageSiteInfos() = 0;
};

}

}

#endif