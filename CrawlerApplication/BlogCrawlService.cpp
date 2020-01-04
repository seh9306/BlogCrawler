#include "stdafx.h"
#include "BlogCrawlService.h"

#include "BlogCrawler.h"
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
	//crawlers.push_back(std::make_unique<crawler::blog::DevMicrosoftBlogCrawler>());
}

void BlogCrawlService::SetDao(void* dao)
{
	for (auto& crawler : crawlers)
	{
		crawler->SetDao(dao);
	}
}

}