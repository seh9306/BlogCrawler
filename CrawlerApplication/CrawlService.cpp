#include "stdafx.h"
#include "CrawlService.h"

namespace service
{

CrawlService::CrawlService()
{
}

CrawlService::~CrawlService()
{
}

void CrawlService::Execute() const
{
	for (auto& crawler : crawlers)
	{
		crawler->Crawl();
	}
}

}