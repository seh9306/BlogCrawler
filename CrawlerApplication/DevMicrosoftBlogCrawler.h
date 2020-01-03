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
	DevMicrosoftBlogCrawler(std::shared_ptr<dao::BlogArticleDao> blogArticleDao);
	~DevMicrosoftBlogCrawler();

private:
	SiteInfo GetArticleSiteInfos(SiteInfo& pageInfos);

	void CatchExceptionalUrlCase(std::string& url) override;
	bool GetAndInsertArticles(SiteInfo& articleSites) override;

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