#ifndef CRAWLER_CRAWLER_H
#define CRAWLER_CRAWLER_H

#include "ProgressSubject.h"

namespace crawler
{

class Crawler
	: public observer::ProgressSubject
{
public:
	Crawler() {};
	virtual ~Crawler() {};

	virtual bool Crawl() = 0;
	virtual void SetDao(void* dao) = 0;
};

}

#endif 
