#ifndef SERVICE_CRAWL_H
#define SERVICE_CRAWL_H

#include <memory>
#include <vector>

#include "Crawler.h"

namespace service
{

class CrawlService
{
public:
	CrawlService();
	virtual ~CrawlService();

	virtual void CreateCrawlers() = 0;
	void Execute();

protected:
	std::vector<std::unique_ptr<crawler::Crawler>> crawlers;

};

}

#endif