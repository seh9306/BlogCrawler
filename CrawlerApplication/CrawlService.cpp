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

void CrawlService::Execute()
{
	for (auto& crawler : crawlers)
	{
		crawler->Crawl();
	}
}

}