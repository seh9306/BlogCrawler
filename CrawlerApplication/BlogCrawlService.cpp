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
	crawlers.push_back(std::make_unique<crawler::blog::HerbsutterBlogCrawler>());
	crawlers.push_back(std::make_unique<crawler::blog::DevMicrosoftBlogCrawler>());
}

}