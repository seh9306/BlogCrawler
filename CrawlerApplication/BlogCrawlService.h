#ifndef SERVICE_BLOG_CRAWL_SERVICE_H
#define SERVICE_BLOG_CRAWL_SERVICE_H

#include "CrawlService.h"

namespace service
{

class BlogCrawlService
	: public CrawlService
{
public:
	BlogCrawlService();
	~BlogCrawlService();
	
	void CreateCrawlers() override;
	void SetProgressObserver(observer::ObserverList& observers) override;
	void SetDao(void* dao) override;
};

}

#endif