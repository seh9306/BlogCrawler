#include "stdafx.h"
#include "BlogCrawlService.h"

#include "HerbsutterBlogCrawler.h"
#include "DevMicrosoftBlogCrawler.h"

namespace service
{

BlogCrawlService::BlogCrawlService()
{
}

BlogCrawlService::~BlogCrawlService()
{
}

void BlogCrawlService::CreateCrawlers()
{
	auto blogArticleDao = std::make_shared<dao::BlogArticleDao>();

	blogArticleDao->Initialize();

	crawlers.push_back(std::make_unique<crawler::blog::HerbsutterBlogCrawler>(blogArticleDao));
	crawlers.push_back(std::make_unique<crawler::blog::DevMicrosoftBlogCrawler>(blogArticleDao));
}

}