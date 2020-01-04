#ifndef SERVICE_CRAWL_SERVICE_H
#define SERVICE_CRAWL_SERVICE_H

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

	void Execute() const;

	virtual void CreateCrawlers() = 0;
	virtual void SetDao(void* dao) = 0;

protected:
	std::vector<std::unique_ptr<crawler::Crawler>> crawlers;

};

}

#endif