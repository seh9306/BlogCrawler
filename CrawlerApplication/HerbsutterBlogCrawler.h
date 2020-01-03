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
	HerbsutterBlogCrawler(std::shared_ptr<dao::BlogArticleDao>);
	virtual ~HerbsutterBlogCrawler();

private:
	void CatchExceptionalUrlCase(std::string& url) override;
	bool GetAndInsertArticles(SiteInfo& HtmlDocuments) override;

	const char* const GetHost() const override;
	const char* const GetIndexPath() const override;
	const char* const GetSelectorForArchivesTag() const override;
	const char* const GetSelectorForUrlTag() const override;
	const char* const GetAttributeNameForUrl() const override;

	SiteInfo GetPageSiteInfos() override;
};

}

}

#endif