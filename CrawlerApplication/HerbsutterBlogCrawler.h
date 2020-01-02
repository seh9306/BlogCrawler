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


	// template method
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