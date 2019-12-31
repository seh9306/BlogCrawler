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

	virtual void CreateCrawlers() = 0;
	void Execute() const;

protected:
	std::vector<std::unique_ptr<crawler::Crawler>> crawlers;

};

}

#endif