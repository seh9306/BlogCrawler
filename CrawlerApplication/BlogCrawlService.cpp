#include "stdafx.h"
#include "BlogCrawlService.h"

#include "HerbsutterBlogCrawler.h"

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
}

}