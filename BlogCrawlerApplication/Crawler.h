#ifndef CRAWLER_CRAWLER_H
#define CRAWLER_CRAWLER_H

namespace crawler
{

class Crawler
{
public:
	Crawler() {};
	virtual ~Crawler() {};

	virtual bool Crawl() = 0;
};

}

#endif 
