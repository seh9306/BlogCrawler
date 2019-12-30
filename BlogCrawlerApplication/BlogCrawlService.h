#ifndef SERVICE_BLOGCRAWLSERVICE_H
#define SERVICE_BLOGCRAWLSERVICE_H

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
};

}

#endif